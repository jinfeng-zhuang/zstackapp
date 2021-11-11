#include "network_protocol_xg9900.h"

#include <string.h>

extern unsigned short network_protocol_callback_simp(unsigned char *buffer, unsigned short length);
extern unsigned short network_protocol_callback_spec(unsigned char *buffer, unsigned short length);
extern unsigned short network_protocol_callback_dcfg(unsigned char *buffer, unsigned short length);
extern unsigned short network_protocol_callback_dcmd(unsigned char *buffer, unsigned short length);
extern unsigned short network_protocol_callback_read(unsigned char *buffer, unsigned short length);

#define NULL ((void *)0)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

static struct network_protocol_proc {
  char fourcc[4];
  network_protocol_callback cb;
} network_protocol_callbacks[] = {
  {"PING", network_protocol_callback_ping},
  {"ACK ", network_protocol_callback_ack},
  {"SIMP", network_protocol_callback_simp},
  {"SPEC", network_protocol_callback_spec},
  {"DCFG", network_protocol_callback_dcfg},
  {"DCMD", network_protocol_callback_dcmd},
  {"READ", network_protocol_callback_read},
};

network_protocol_callback network_protocol_search(char *fourcc)
{
  int i;

  for (i = 0; i < ARRAY_SIZE(network_protocol_callbacks); i++) {
    if (0 == memcmp(fourcc, network_protocol_callbacks[i].fourcc, 4)) {
      if (network_protocol_callbacks[i].cb) {
        return network_protocol_callbacks[i].cb;
      }
    }
  }

  return NULL;
}
