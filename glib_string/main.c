#include <zstack/zstack.h>

#include "main.h"

#include <glib.h>
#include <glib/gprintf.h>

struct application app;

int main(int argc, char *argv[]) {
  gchar ch[128];

  if (param_parser(argc, argv, &app) == -1) {
    print_usage();
    return -1;
  }

  log_init(app.param.log_config);

  char* str = "Hello World";

  char* new_str = g_strdup(str);

  printf("str = %p new_str = %p\n", str, new_str);

  printf("%s\n", g_strnfill(10, 'Z'));

  //printf("%s\n", g_stpcpy("Hello", "World"));

  return 0;
}
