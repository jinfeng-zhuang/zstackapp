#include <zstack/zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[]) {
  unsigned char *buffer;

  if (param_parser(argc, argv, &app) == -1) {
    print_usage();
    return -1;
  }

  log_init(app.param.log_config);

  app.input_file_content =
      file_load_simple(app.param.input_filename, 0, &app.input_file_size);
  app.input_file_content2 =
      file_load_simple(app.param.input_filename2, 0, &app.input_file_size2);

  if ((app.input_file_content == NULL) || (NULL == app.input_file_content2)) {
    return -1;
  }

  buffer = malloc(app.input_file_size + app.input_file_size2);
  if (NULL == buffer)
    return -1;

  memcpy(buffer, app.input_file_content, app.input_file_size);
  memcpy(&buffer[app.input_file_size], app.input_file_content2,
         app.input_file_size2);

  file_save(app.param.output_filename, buffer,
            app.input_file_size + app.input_file_size2);

  return 0;
}
