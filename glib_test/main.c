#include <zstack/zstack.h>

#include "main.h"

#include <glib.h>

struct application app;

int main(int argc, char *argv[])
{
    gchar ch[128];

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    g_sprintf(ch, "hello world");

    return 0;
}
