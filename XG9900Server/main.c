#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "main.h"

#define VERSION ("1.0.0")

#define portnumber 9901

#define IMEI_SIZE (15)
#define SN_SIZE (16)

#define NAME_SIZE (256)

#define TXBUF_SIZE (2048)

#define PARA_LEN (360)
#define SPEC_LEN (5516)

#define CMD_DOWNLOAD_CONFIG (1)
#define CMD_UPLOAD_PARA (2)
#define CMD_UPLOAD_SPEC (3)
#define CMD_RESET (4)
#define CMD_UPLOAD_START_TIME (5)
#define CMD_UPLOAD_VERSION (6)
#define CMD_DOWNLOAD_FIRMWARE (7)
#define CMD_CHECK_USER_COMMAND (8)

int server_fd;

void signal_proc(int signo) {
  int i;

  close(server_fd);

  for (i = 0; i < 3; i++) {
    sleep(1);
  }

  _exit(0);
}

int main(int argc, char *argv[]) {
  int new_fd;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  int sin_size;
  pthread_t pid;

  log_thread_init();

  log2buffer("XG9900 Server %s\n", VERSION);

  client_thread_int();

  signal(SIGINT, signal_proc);
  signal(SIGTERM, signal_proc);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    return 0;
  }

  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  memset(&server_addr, 0, sizeof(struct sockaddr_in));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(portnumber);
  if (bind(server_fd, (struct sockaddr *)(&server_addr),
           sizeof(struct sockaddr)) == -1) {
    log2buffer("port %d already used\n", portnumber);
    goto SERVER_ERROR;
  }

  log2buffer("Bind done\n");

  if (listen(server_fd, 5) == -1) {
    log2buffer("Listen error:%s\n\a", strerror(errno));
    goto SERVER_ERROR;
  }

  while (1) {
    sin_size = sizeof(struct sockaddr_in);
    new_fd = accept(server_fd, (struct sockaddr *)(&client_addr), &sin_size);
    if (new_fd == -1) {
      log2buffer("Server FD [%d] Accept error:%s Try again.\n\a", server_fd,
                 strerror(errno));
      goto SERVER_ERROR;
    } else {
      if (0 != client_thread_start(client_thread, (void *)new_fd)) {
        log2buffer("New connection: but thread number out of range, defuse\n");
      } else {
        log2buffer("New connection [%d] from [%s]\n", new_fd,
                   inet_ntoa(client_addr.sin_addr));
      }
    }
  }

SERVER_ERROR:

  log2buffer("SERVER END\n");

  sleep(1);

  close(server_fd);

  return 0;
}
