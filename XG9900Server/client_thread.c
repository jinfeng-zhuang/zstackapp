#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "main.h"
#include "network_protocol_xg9900.h"

extern unsigned short crc16_ccitt(unsigned char *pbuf, unsigned int len);
extern unsigned char *network_protocol_packup(char* fourcc, unsigned char* buf, unsigned short length, unsigned short *total);

void *client_thread(void *arg) {
  int fd = (int)arg;

  int iDataNum;
  unsigned char *recv_buffer;
  unsigned int total_recv = 0;

  network_protocol_callback cb;
  
  unsigned char frame_type[4];
  unsigned char frame_number;
  unsigned char frame_flags;
  unsigned short frame_datalen;
  unsigned short frame_crc;
  
  unsigned short crc_calc;

  recv_buffer = (unsigned char *)malloc(RECV_BUFFER_MAX);
  if (NULL == recv_buffer) {
    log2buffer("malloc failed for client\n");
    goto CLIENT_DISCONNECT;
  }

  while (1) {

    memset(recv_buffer, 0, RECV_BUFFER_MAX);

    //=======================================
    // receive magic
    //=======================================

    iDataNum = recv(fd, recv_buffer, 4, 0);
    if (iDataNum != 4) {
      log2buffer("[%d] Receive error 1\n", fd);
      goto CLIENT_DISCONNECT;
    }

    // TODO
    while (1) {
        if (0 == memcmp(recv_buffer, "XG90", 4)) {
            break;
        }
        
        log2buffer("magic not found!\n");

        iDataNum = recv(fd, &recv_buffer[4], 1, 0);
        if (iDataNum != 1) {
          log2buffer("[%d] Receive error 2\n", fd);
          goto CLIENT_DISCONNECT;
        }

        memmove(recv_buffer, &recv_buffer[1], 4);
    }

    //=======================================
    // receive remaining header
    //=======================================
    
    iDataNum = recv(fd, &recv_buffer[4], 8, 0);
    if (iDataNum != 8) {
      log2buffer("[%d] Receive error 3\n", fd);
      goto CLIENT_DISCONNECT;
    }

    memcpy(frame_type, &recv_buffer[4], 4);
    frame_number = recv_buffer[8];
    frame_flags = recv_buffer[9];
    frame_datalen = recv_buffer[10] << 8 | recv_buffer[11];

    //=======================================
    // receive payload
    //=======================================

    total_recv = 0;
    while (total_recv < frame_datalen) {
      iDataNum = recv(fd, &recv_buffer[12 + total_recv], frame_datalen - total_recv, 0);
      if (iDataNum <= 0) {
        log2buffer("[%d] Recv failed\n", fd);
        goto CLIENT_DISCONNECT;
      }
      
      total_recv += iDataNum;
    }

    //=======================================
    // receive CRC16
    //=======================================
    
    iDataNum = recv(fd, &recv_buffer[12 + frame_datalen], 2, 0);
    if (iDataNum != 2) {
      log2buffer("[%d] Receive CRC16 error\n", fd);
      goto CLIENT_DISCONNECT;
    }

    frame_crc = recv_buffer[12 + frame_datalen] << 8 | recv_buffer[12 + frame_datalen + 1];
    
    //=======================================
    // CRC16 Check
    //=======================================
    
    crc_calc = crc16_ccitt(recv_buffer, 12 + frame_datalen);
    
    if (crc_calc != frame_crc) {
      log2buffer("[%d] CRC not correct: %x\n", fd, frame_crc);
      goto CLIENT_DISCONNECT;
    }
    
    //=======================================
    // Call callback of types
    //=======================================

    //log2buffer("TYPE: %c%c%c%c\n", frame_type[0], frame_type[1], frame_type[2], frame_type[3]);
    cb = network_protocol_search(frame_type);
    if (NULL != cb) {
      unsigned short ret = cb(&recv_buffer[12], frame_datalen);
      if (0 == ret) {
        //log2buffer("Done without send\n");
      }
      // TODO data size = 65536, not -1
      else if ((unsigned short)(-1) == ret) {
        log2buffer("Failed\n");
      }
      else {
        unsigned short total = 0;
        unsigned char *pkg = network_protocol_packup(frame_type, &recv_buffer[12], ret, &total);
        //log2buffer("Try send package to client: %d bytes; %x\n", total, ret);
        send(fd, pkg, total, 0);
      }
    }
    else {
      log2buffer("Callback not found\n");
    }
  }

CLIENT_DISCONNECT:
  log2buffer("client [%d] disconnect\n", fd);
  client_thread_end();

  if (recv_buffer)
    free(recv_buffer);

  close(fd);
}
