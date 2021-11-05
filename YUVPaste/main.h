#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <zstack/types.h>
#include <zstack/yuv.h>
#include <zstack/graphic.h>

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char input_filename[FILENAME_MAX];
        char crop_filename[FILENAME_MAX];
        char output_filename[FILENAME_MAX];
        enum YUVFormat format;
        int width;
        int height;
        struct rect rect;
        int count;
    } param;

    u8* input_file_content;
    u64 input_file_size;

    u8* crop_file_content;
    u64   crop_file_size;

    struct YUV_Wrapper* wrap;
    int frame_size;
    int frame_num;

    int crop_frame_size;
    int crop_frame_num;
};

#ifdef __cplusplus
}
#endif

#endif