#include <zstack/zstack.h>

#include "main.h"

struct application app;

extern int gst_basic_dynamic_pipeline(int argc, char* argv[]);
extern int gst_basic_time(void);

int main(int argc, char *argv[])
{
    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    gst_init(&argc, &argv);

    //gst_basic_dynamic_pipeline(&argc, &argv);
    gst_basic_time();

    return 0;
}
