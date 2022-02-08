#include <zstack/zstack.h>
#include <zstack/dp.h>

#include "main.h"

#define ITEM_SIZE  (64)

typedef int (*callback)(int count, char* item[ITEM_SIZE]);

extern int cmdlist_parse(const char* str);
extern int cmd_parser_register(callback cb);

struct application app;

int _callback(int count, char* item[ITEM_SIZE])
{
  printf("Command = %s\n", item[0]);

  return 0;
}

int main(int argc, char *argv[]) {
  if (param_parser(argc, argv, &app) == -1) {
    print_usage();
    return -1;
  }

  log_init(app.param.log_config);
  
  //cmd_parser_register(_callback);

  //cmdlist_parse("log funcA; log funcB; play 100; stop; set dumpyuv=5; start; break resolution=1280x720; break stride > 480; dumpyuv 1");

  int i;
  for (i = 0; i < 5; ++i) {
    printf("i = %d\n", i);
  }

#if 0
  char* str = "setVideoInformation:api"; 
  debug_point_init(str, strlen(str));
  
  DP(DP_API, "Hello World");
#endif

  return 0;
}
