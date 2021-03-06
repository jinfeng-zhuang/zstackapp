const char *usage =
    "Usage: YUVGen [OPTION]...[FILE]...\n"
    "\n"
    "OPTIONS:\n"
    "\n"
    "  --version     version info\n"
    "  --help        this message\n"
    "  --log         log config\n"
    "  --resolution  resolution\n"
    "  --stride      stride\n"
    "  --format      format: NV12\n"
    "  --framerate   frame rate\n"
    "  --field       field flag\n"
    "  --duration    in second\n"
    "\n"
    "Examples:\n"
    "\n"
    "  YUVGen --resolution=176x144 --stride=256 --format=NV12 --framerate=25 "
    "--duration=10 output.yuv\n"
    "\n"
    "Source located at https://github.com/jinfeng-zhuang/zstack\n"
    "Copyright @ 2021, Jinfeng Zhuang\n";