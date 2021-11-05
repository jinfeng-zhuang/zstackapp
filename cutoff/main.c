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
    if (NULL == app.input_file_content) {
        warn("file load failed\n");
        return -1;
    }

    if (0 != app.param.end) {
        if (app.param.end > app.param.start) {
            app.output_file_size = app.param.end - app.param.start;
        }
        else {
            warn("end > start\n");
            return -1;
        }
    }
    else if (0 != app.param.size) {
        app.output_file_size = app.param.size;
        if (app.output_file_size + app.param.start > app.input_file_size) {
            app.output_file_size = app.input_file_size - app.param.start;
        }
    }
    else {
        app.output_file_size = app.input_file_size - app.param.start;
    }

    file_save(app.param.output_filename, &app.input_file_content[app.param.start], app.output_file_size);

    info("%s", app.param.output_filename);

    return 0;
}
