#include "main.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <zstack/zstack.h>

struct application app;

int main(int argc, char *argv[]) {
  u32 len = SIZE_1MB;
  u8 *buffer;

  u32 tmp_len;
  u8 *tmp_buf;

  const AVCodec *codec;
  AVCodecParserContext *parser;
  AVCodecContext *ctx = NULL;
  AVPacket *pkt;

  int i;

#if 0
    AVStream* stream;
    AVCodecParameters* par;
#endif

  if (param_parser(argc, argv, &app) == -1) {
    print_usage();
    return -1;
  }

  log_init(app.param.log_config);

  // av_log_set_level(AV_LOG_TRACE);

  buffer = file_load_simple(app.param.input_filename, 0, &len);
  if (NULL == buffer)
    return -1;

  codec = avcodec_find_decoder(AV_CODEC_ID_MPEG2VIDEO);
  parser = av_parser_init(codec->id);
  ctx = avcodec_alloc_context3(codec);
  pkt = av_packet_alloc();

  avcodec_open2(ctx, codec, NULL);

  tmp_buf = buffer;
  tmp_len = len;

  for (i = 0; i < 20; i++) {
    av_parser_parse2(parser, ctx, &pkt->data, &pkt->size, tmp_buf, tmp_len,
                     AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);

    // info("PKT: Start = %#x , Size = %#x\n", pkt->data - buffer, pkt->size);
    tmp_buf += pkt->size;
    tmp_len -= pkt->size;

    info("Parser (%dx%d): pict_type = %d pts = %lld repeat_pict = %d "
         "field_order = %d, picture_structure = %d\n",
         parser->width, parser->height, parser->pict_type, parser->pts,
         parser->repeat_pict, parser->field_order, parser->picture_structure);

    // info("%s %s@%d\n", avcodec_get_name(codec->id),
    // av_get_profile_name(codec, ctx->profile), ctx->level);

    AVFrame *frame;
    frame = av_frame_alloc();

    avcodec_send_packet(ctx, pkt);

    avcodec_receive_frame(ctx, frame);

#if 1
    info("pict_type = %d top_field_first = %d, repeat_pict = %d, interlace = "
         "%d\n",
         frame->pict_type, frame->top_field_first, frame->repeat_pict,
         frame->interlaced_frame);
#endif

    av_frame_free(&frame);
  }

  if (buffer) {
    free(buffer);
  }

  av_parser_close(parser);
  avcodec_free_context(&ctx);
  av_packet_free(&pkt);

  return 0;
}
