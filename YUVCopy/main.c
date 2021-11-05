#include <zstack/zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[]) {
  int i;
  int x, y;
  int ret;
  u64 offset;
  struct rect *rect;
  u8 Y, U, V;

  if (param_parser(argc, argv, &app) == -1) {
    print_usage();
    return -1;
  }

  log_init(app.param.log_config);

  app.wrap = YUV_Wrapper_Init(app.param.format);
  if (NULL == app.wrap) {
    return -1;
  }

  app.frame_size = app.wrap->FrameSize(app.param.width, app.param.height);
  app.input_file_size = file_size(app.param.input_filename);
  app.frame_num = app.input_file_size / app.frame_size;
  app.input_file_content = malloc(app.frame_size);

  app.out_frame_size = app.wrap->FrameSize(app.param.rect.w, app.param.rect.h);
  app.output_file_content = malloc(app.out_frame_size);

  rect = &app.param.rect;

  // parameter check again
  if (0 == app.param.count)
    app.param.count = app.frame_num;

  if (app.param.count > app.frame_num)
    app.param.count = app.frame_num;

  if (NULL == app.input_file_content)
    goto END;

  if (NULL == app.output_file_content)
    goto END;

  // TODO add an API to check if a rect totally contained by another rect
  if (app.param.rect.x > app.param.width) {
    warn("rect out of range\n");
    goto END;
  }

  // avoid append to old file
  file_save(app.param.output_filename, NULL, 0);

  // start loop

  offset = 0;
  for (i = 0; i < app.param.count; i++) {
    ret = file_load(app.param.input_filename, offset, app.input_file_content,
                    app.frame_size);

    if (ret != app.frame_size)
      goto END;

    offset += app.frame_size;

    // copy rect from the frame
    for (y = rect->y; y < rect->y + rect->h; y++) {
      for (x = rect->x; x < rect->x + rect->w; x++) {
        Y = app.wrap->GetY(app.input_file_content, app.param.width,
                           app.param.height, x, y);
        U = app.wrap->GetU(app.input_file_content, app.param.width,
                           app.param.height, x, y);
        V = app.wrap->GetV(app.input_file_content, app.param.width,
                           app.param.height, x, y);

        app.wrap->SetY(app.output_file_content, rect->w, rect->h, x - rect->x,
                       y - rect->y, Y);
        app.wrap->SetU(app.output_file_content, rect->w, rect->h, x - rect->x,
                       y - rect->y, U);
        app.wrap->SetV(app.output_file_content, rect->w, rect->h, x - rect->x,
                       y - rect->y, V);
      }
    }

    info("save frame\n");
    file_append(app.param.output_filename, app.output_file_content,
                app.out_frame_size);
  }

  info("Complete\n");

END:
  info("END\n");

  if (app.input_file_content)
    free(app.input_file_content);

  if (app.output_file_content)
    free(app.output_file_content);

  return 0;
}
