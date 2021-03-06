#define _CRT_SECURE_NO_WARNINGS

#include <zstack.h>

#include <3rdparty/getopt.h>

#include "main.h"

enum {
  OPTION_VERSION = 1,
  OPTION_HELP,
  OPTION_LOG,
  OPTION_FREQ,
  OPTION_NUMBER,
  OPTION_SAMPLE,
  OPTION_FORMAT,
};

static struct option opts[] = {{"version", no_argument, 0, OPTION_VERSION},
                               {"help", no_argument, 0, OPTION_HELP},
                               {"log", required_argument, 0, OPTION_LOG},
                               {"freq", required_argument, 0, OPTION_FREQ},
                               {"number", required_argument, 0, OPTION_NUMBER},
                               {"sample", required_argument, 0, OPTION_SAMPLE},
                               {"format", required_argument, 0, OPTION_FORMAT},
                               {0, 0, 0, 0}};

int param_parser(int argc, char *argv[], struct application *app) {
  int c;

  if (argc <= 1)
    return -1;

  while ((c = getopt_long(argc, argv, "", opts, NULL)) != -1) {
    switch (c) {
    case OPTION_VERSION:
      log_info("Version: %s\n", version);
      break;
    case OPTION_HELP:
      log_info(usage);
      break;
    case OPTION_LOG:
      if (strlen(optarg) >= LOG_CONFIG_LENGTH) {
        log_info("log config out of range (0, %d)\n", LOG_CONFIG_LENGTH);
        return -1;
      }
      strncpy(app->param.log_config, optarg, LOG_CONFIG_LENGTH);
      break;
    case OPTION_FREQ:
      app->param.freq = atoi(optarg);
      break;
    case OPTION_NUMBER:
      app->param.number = atoi(optarg);
      break;
    case OPTION_SAMPLE:
      app->param.sample = atoi(optarg);
      break;
    case OPTION_FORMAT:
      if (0 == strcmp(optarg, "c-array")) {
        app->param.format = FORMAT_CARRAY;
      }
      break;
    default:
      return -1;
    }
  }

  if (optind < argc) {
    if (strlen(argv[optind]) >= FILENAME_MAX)
      return -1;
    memcpy(app->param.filename, argv[optind], strlen(argv[optind]));
  }

  // Do param validation
  if ((0 == app->param.number) || (0 == app->param.freq) ||
      (0 == app->param.sample))
    return -1;

  return 0;
}
