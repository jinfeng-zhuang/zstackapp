#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <zstack/graphic.h>
#include <zstack/types.h>
#include <zstack/yuv.h>

#define LOG_CONFIG_LENGTH (256)

struct application {
  struct {
    char log_config[LOG_CONFIG_LENGTH];
    char input_filename[FILENAME_MAX];
    char output_filename[FILENAME_MAX];
    enum YUVFormat format;
    int width;
    int height;
    struct rect rect;
    int count;
    int step;
  } param;

  u8 *input_file_content;
  u64 input_file_size;

  struct YUV_Wrapper *wrap;
  int frame_size;
  int frame_num;

  int out_frame_size;
};

#ifdef __cplusplus
}
#endif

#endif