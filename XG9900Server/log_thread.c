#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define LOG_BUFFER_SIZE (128)

#define log_file "/tmp/xg9901.log"

static FILE *fp = NULL;
static int module_inited = 0;
static pthread_mutex_t log_mutex;

static int busy = 0;

int log2buffer(const char *fmt, ...) {
  struct tm *local_time;
  time_t now;
  va_list args;
  char log_buffer[LOG_BUFFER_SIZE];

  if (!module_inited)
    return 0;

  time(&now);
  local_time = localtime(&now);

  pthread_mutex_lock(&log_mutex);

  memset(log_buffer, 0, LOG_BUFFER_SIZE);
  va_start(args, fmt);
  vsnprintf(log_buffer, LOG_BUFFER_SIZE, fmt, args);
  va_end(args);

  pthread_mutex_unlock(&log_mutex);

  printf(log_buffer);
  fflush(stdout);

  if (fp) {
    fprintf(fp, "[%d-%d_%d:%d] %s", local_time->tm_mon + 1, local_time->tm_mday,
            local_time->tm_hour, local_time->tm_min, log_buffer);
    fflush(fp);
  }

  return strlen(log_buffer);
}

int log_thread_init(void) {
  pthread_t pid;

  pthread_mutex_init(&log_mutex, NULL);

  fp = fopen(log_file, "a");
  if (NULL == fp) {
    printf("log_dump_thread: failed to open log file\n");
    return -1;
  }

  module_inited = 1;

  return 0;
}
