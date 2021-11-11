#include <string.h>

extern unsigned short crc16_ccitt(unsigned char *pbuf, unsigned int len);

#define MAGIC_NUMBER  "XG90"

#define BUFFER_SIZE (65536)
#define HEADER_LEN  (12)

static unsigned char buffer[BUFFER_SIZE];
static unsigned char frame_number = 0;

unsigned char *network_protocol_packup(char* fourcc, unsigned char* buf, unsigned short length, unsigned short *total)
{
  unsigned short crc;

  *total = 0;

  if ((NULL == fourcc) || (NULL == buf) || (HEADER_LEN + length + 2 > BUFFER_SIZE))
    return NULL;

  memcpy(buffer, MAGIC_NUMBER, 4);
  memcpy(&buffer[4], fourcc, 4);
  buffer[8] = frame_number++;
  buffer[9] = 0;
  buffer[10] = length >> 8;
  buffer[11] = length & 0xFF;

  memcpy(&buffer[12], buf, length);

  crc = crc16_ccitt(buffer, HEADER_LEN + length);

  buffer[HEADER_LEN + length + 0] = crc >> 8;
  buffer[HEADER_LEN + length + 1] = crc & 0xFF;

  *total = HEADER_LEN + length + 2;

  return buffer;
}
