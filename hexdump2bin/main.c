#include <zstack/zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.input_file_content = file_load_simple(app.param.input_filename, 0, &app.input_file_size);
    if (NULL == app.input_file_content)
        return -1;

    u8* buffer = malloc(app.input_file_size);
    memset(buffer, 0, app.input_file_size);

    int i, j;
    for (i = 0, j = 0; i < app.input_file_size; i++) {
        if ((app.input_file_content[i] != ' ') &&
            (app.input_file_content[i] != '\r') &&
            (app.input_file_content[i] != '\n')) {
            buffer[j++] = app.input_file_content[i];
        }
    }

    u32 binlen = j / 2;
    u8* binbuf = malloc(binlen);
    u8 c;
    char tmp[3];

    for (i = 0; i < binlen; i++) {
        tmp[0] = buffer[i * 2 + 0];
        tmp[1] = buffer[i * 2 + 1];
        tmp[2] = 0;

        c = strtoul(tmp, NULL, 16);

        binbuf[i] = c;
    }

    hexdump(binbuf, binlen, 0);

    file_save(app.param.output_filename, binbuf, binlen);

    return 0;
}
