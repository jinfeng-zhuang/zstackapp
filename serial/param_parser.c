#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include <getopt/getopt.h>
#include <stdlib.h>
#include <string.h>
#include <zstack/zstack.h>

enum {
  OPTION_VERSION = 1,
  OPTION_HELP,
  OPTION_LOG,
  OPTION_LIST,
  OPTION_PORT,
};

static struct option opts[] = {{"version", no_argument, 0, OPTION_VERSION},
                               {"help", no_argument, 0, OPTION_HELP},
                               {"log", required_argument, 0, OPTION_LOG},
                               {"list", no_argument, 0, OPTION_LIST},
                               {"port", required_argument, 0, OPTION_PORT},
                               {0, 0, 0, 0}};

int param_parser(int argc, char *argv[], struct application *app) {
  int c;
  struct time tm;

  if (argc <= 1)
    return -1;

  while ((c = getopt_long(argc, argv, "", opts, NULL)) != -1) {
    switch (c) {
    case OPTION_VERSION:
      PRINT("Version: %s\n", version);
      return -1;
    case OPTION_HELP:
      return -1;
    case OPTION_LOG:
      if (strlen(optarg) >= LOG_CONFIG_LENGTH) {
        PRINT("log config out of range (0, %d)\n", LOG_CONFIG_LENGTH);
        return -1;
      }
      strncpy(app->param.log_config, optarg, LOG_CONFIG_LENGTH);
      break;
    case OPTION_LIST:
      app->param.list_flag = 1;
      break;
    case OPTION_PORT:
      app->param.port = atoi(optarg);
      break;
    default:
      return -1;
    }
  }

  if (optind < argc) {
    if (strlen(argv[optind]) >= FILENAME_MAX)
      return -1;
    memcpy(app->param.input_filename, argv[optind], strlen(argv[optind]));
  }

  /*************************************************************************
   * Do param validation
   ************************************************************************/

  if (0 == app->param.output_filename[0]) {
    get_time(&tm);
    snprintf(app->param.output_filename, FILENAME_MAX,
             "%d-%02d-%02d_%02d-%02d-%02d", tm.year, tm.month, tm.day, tm.hour,
             tm.min, tm.sec);
  }

  return 0;
}
