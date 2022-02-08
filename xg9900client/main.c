#include <zstack/zstack.h>
#include <xg9900/network.h>
#include <stdarg.h>

#include "main.h"

extern unsigned char* network_protocol_packup(char* fourcc, unsigned char* buf, unsigned short length, unsigned short* total);

struct application app;

static unsigned char buffer[1<<20];

#define FILE2READ "config.json"

void logerr(char* name, ...)
{
  va_list ap;

  va_start(ap, name);

  while (1) {
    char* format = va_arg(ap, char*);
    if (format == NULL)
      break;

    printf("= %s\n", format);
  }

  printf("END\n");

  va_end(ap);
}

int main(int argc, char *argv[]) {
  int ret;
  unsigned char frame_number = 0;
  unsigned short crc;
  unsigned short total = 0;
  unsigned char* pkg;
  unsigned int datalen;

  logerr(__FUNCTION__, __FILE__, "main");

  return 0;

  if (param_parser(argc, argv, &app) == -1) {
    print_usage();
    return -1;
  }

  log_init(app.param.log_config);

  xg9900_network_init();
  
  info("Connecting...\n");

  ret = xg9900_connect();
  if (-1 == ret) {
    goto END;
  }
  
  info("Connected\n");

  int i;
  for (i = 0; i < 8; i++) {
    buffer[i] = '0';
  }
  for (i = 8; i < 8 + 16; i++) {
    buffer[i] = 'Z';
  }

#if 0
  datalen = 360;
#elif 0
  datalen = 5516;
#else
  datalen = strlen(FILE2READ);
#endif

  datalen += 8 + 16;

#if 0
  for (i = 8 + 16; i < datalen; i++) {
    buffer[i] = '0';
  }
#else
  memcpy(&buffer[8 + 16], FILE2READ, strlen(FILE2READ));
#endif

  pkg = network_protocol_packup("READ", buffer, datalen, &total);

  if (NULL == pkg)
    goto END;

  while (1) {
    memset(buffer, 0, sizeof(buffer));

    ret = xg9900_send(pkg, (int)total);
    if (ret == -1)
      break;

#if 1 
    ret = xg9900_recv(buffer, sizeof(buffer));
    if (ret == -1)
      break;

    hexdump(buffer, 32, HEXDUMP_ASCII);
#endif

    msleep(1000);
  }

END:
  xg9900_disconnect();

  xg9900_network_deinit();

  return 0;
}
