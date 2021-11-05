
#include <libavformat/avformat.h>
#include <libavutil/dict.h>



int main(int argc, char *argv[])
{


    AVFormatContext* fmt_ctx = NULL;
    AVDictionaryEntry* tag = NULL;
    int ret;
    AVPacket packet;
    int video_stream_index_ = -1;

    // ����õ�������������������Ϣ����ð���һ֡���ݣ�ʧ�ܵ�����¶��һЩ�ٵ��øú���
    if ((ret = avformat_open_input(&fmt_ctx, argv[1], NULL, NULL)))
        return ret;

    ret = avformat_find_stream_info(fmt_ctx, NULL);
    if (ret < 0) {
        printf("avformat_find_stream_info return %d\n", ret);
        return -1;
    }

    av_dump_format(fmt_ctx, 0, "", 0);

    for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index_ = i;
            break;
        }
    }

    // ref: xilixili.net/2018/08/20/ffmpeg-got-raw-h264/
    // https://www.jianshu.com/p/21dfd64add77
    while (av_read_frame(fmt_ctx, &packet) >= 0) {
        if (packet.stream_index == video_stream_index_) {
            printf("Video Package PTS %llx: %x %x %x %x\n", packet.pts,
                packet.data[0],
                packet.data[1],
                packet.data[2],
                packet.data[3]
                );
        }
    }

    avformat_close_input(&fmt_ctx);

    return 0;
}
