#include <zstack/zstack.h>

#include "main.h"

#include "ts.h"
#include <caption/cea708.h>

typedef unsigned int (*flcb)(unsigned char* buffer, unsigned int length);

// size: callback required block size
int file_load_cb(const char* filename, flcb cb, unsigned int size);

extern int bitstream_copy(struct bitstream* bs, unsigned char* buffer, unsigned int length);

struct application app;

void dump_ts_header(struct ts_header* h)
{
  printf("\nTS Header\n");
  printf("------------------------------------------------\n");
  printf("%-40s: 0x%x\n", "sync_byte", h->sync_byte);
  printf("%-40s: %d\n", "transport_error_indicator", h->transport_error_indicator);
  printf("%-40s: %d\n", "payload_unit_start_indicator", h->payload_unit_start_indicator);
  printf("%-40s: %d\n", "transport_priority", h->transport_priority);
  printf("%-40s: 0x%x\n", "pid", h->pid);
  printf("%-40s: %d\n", "transport_scrambling_control", h->transport_scrambling_control);
  printf("%-40s: %d\n", "adaptation_field_control", h->adaptation_field_control);
  printf("%-40s: %d\n", "continuity_counter", h->continuity_counter);
}

int parse_ts_header(struct ts_header* h, unsigned char* buffer, unsigned int length)
{
  struct bitstream bs;

  bitstream_init(&bs, buffer, length);

  h->sync_byte = bitstream_u(&bs, 8);
  h->transport_error_indicator = bitstream_u(&bs, 1);
  h->payload_unit_start_indicator = bitstream_u(&bs, 1);
  h->transport_priority = bitstream_u(&bs, 1);
  h->pid = bitstream_u(&bs, 13);
  h->transport_scrambling_control = bitstream_u(&bs, 2);
  h->adaptation_field_control = bitstream_u(&bs, 2);
  h->continuity_counter = bitstream_u(&bs, 4);

  return 0;
}

int parse_ts_adaption(struct ts_adaption* h, unsigned char* buffer, unsigned int length)
{
  h->adaptation_field_length = buffer[0];
  h->flag = buffer[1];
  memcpy(h->PCR, &buffer[2], 5);

  return 0;
}

void dump_ts_pat(struct ts_pat* h)
{
  u16 i;

  printf("\nTS PAT\n");
  printf("------------------------------------------------\n");
  printf("%-40s: 0x%x\n", "table_id", h->table_id);
  printf("%-40s: 0x%x\n", "section_syntax_indicator", h->section_syntax_indicator);
  printf("%-40s: 0x%x\n", "zero", h->zero);
  printf("%-40s: 0x%x\n", "reserved", h->reserved);
  printf("%-40s: 0x%x\n", "section_length", h->section_length);
  printf("%-40s: 0x%x\n", "transport_stream_id", h->transport_stream_id);
  printf("%-40s: 0x%x\n", "reserved2", h->reserved2);
  printf("%-40s: 0x%x\n", "version_number", h->version_number);
  printf("%-40s: 0x%x\n", "current_next_indicator", h->current_next_indicator);
  printf("%-40s: 0x%x\n", "section_number", h->section_number);
  printf("%-40s: 0x%x\n", "last_section_number", h->last_section_number);

  for (i = 0; i < h->item_count; i++) {
    printf("    %-36s: %d : 0x%x\n", "program_number:PID",
      h->program[i].program_number,
      h->program[i].PID);
  }

  printf("%-40s: 0x%x\n", "CRC32", h->CRC32);
}

int parse_ts_pat(struct ts_pat* h, unsigned char* buffer, unsigned int length)
{
  u16 item_count;
  u16 i;
  struct bitstream bs;

  bitstream_init(&bs, buffer, length);

  h->table_id = bitstream_u(&bs, 8);
  h->section_syntax_indicator = bitstream_u(&bs, 1);
  h->zero = bitstream_u(&bs, 1);
  h->reserved = bitstream_u(&bs, 2);
  h->section_length = bitstream_u(&bs, 12);
  h->transport_stream_id = bitstream_u(&bs, 16);
  h->reserved2 = bitstream_u(&bs, 2);
  h->version_number = bitstream_u(&bs, 5);
  h->current_next_indicator = bitstream_u(&bs, 1);
  h->section_number = bitstream_u(&bs, 8);
  h->last_section_number = bitstream_u(&bs, 8);

  item_count = (h->section_length - 9) / 4;
  h->item_count = item_count;

  // 'section_length' is 12b, max item count < 1021
  for (i = 0; i < item_count; i++) {
    h->program[i].program_number = bitstream_u(&bs, 16);
    h->program[i].reserved = bitstream_u(&bs, 3);
    h->program[i].PID = bitstream_u(&bs, 13);
  }

  h->CRC32 = bitstream_u(&bs, 32);

  return 0;
}

int parse_ts_pmt(struct ts_pmt* h, unsigned char* buffer, unsigned int length)
{
  u16 item_count = 0;
  struct bitstream bs;
  u16 remain;
  u16 es_info_total_len = 0;

  bitstream_init(&bs, buffer, length);

  h->table_id = bitstream_u(&bs, 8);
  h->section_syntax_indicator = bitstream_u(&bs, 1);
  h->zero = bitstream_u(&bs, 1);
  h->reserved = bitstream_u(&bs, 2);
  h->section_length = bitstream_u(&bs, 12);
  h->program_number = bitstream_u(&bs, 16);
  h->reserved1 = bitstream_u(&bs, 2);
  h->version_number = bitstream_u(&bs, 5);
  h->current_next_indicator = bitstream_u(&bs, 1);
  h->section_number = bitstream_u(&bs, 8);
  h->last_section_number = bitstream_u(&bs, 8);
  h->reserved2 = bitstream_u(&bs, 3);
  h->PCR_PID = bitstream_u(&bs, 13);
  h->reserved3 = bitstream_u(&bs, 4);
  h->program_info_length = bitstream_u(&bs, 12);

  remain = h->section_length - 9 - 4;

  while (0 == bitstream_eof(&bs)) {
    h->streams[item_count].stream_type = bitstream_u(&bs, 8);
    h->streams[item_count].reserved4 = bitstream_u(&bs, 3);
    h->streams[item_count].elementary_PID = bitstream_u(&bs, 13);
    h->streams[item_count].reserved5 = bitstream_u(&bs, 4);
    h->streams[item_count].ES_info_length = bitstream_u(&bs, 12);

    bitstream_u(&bs, h->streams[item_count].ES_info_length * 8);

    bitstream_copy(&bs,
      &h->ES_info[es_info_total_len],
      h->streams[item_count].ES_info_length);

    es_info_total_len += h->streams[item_count].ES_info_length;

    item_count++;
  }

  h->CRC32 = bitstream_u(&bs, 32);

  return 0;
}

void dump_ts_pmt(struct ts_pmt* h)
{
  u16 i;

  printf("\nTS PAT\n");
  printf("------------------------------------------------\n");
  printf("%-40s: 0x%x\n", "table_id", h->table_id);
  printf("%-40s: 0x%x\n", "section_syntax_indicator", h->section_syntax_indicator);
  printf("%-40s: 0x%x\n", "zero", h->zero);
  printf("%-40s: 0x%x\n", "reserved", h->reserved);
  printf("%-40s: 0x%x\n", "section_length", h->section_length);
  printf("%-40s: 0x%x\n", "transport_stream_id", h->program_number);
  printf("%-40s: 0x%x\n", "reserved2", h->reserved2);
  printf("%-40s: 0x%x\n", "version_number", h->version_number);
  printf("%-40s: 0x%x\n", "current_next_indicator", h->current_next_indicator);
  printf("%-40s: 0x%x\n", "section_number", h->section_number);
  printf("%-40s: 0x%x\n", "last_section_number", h->last_section_number);

  for (i = 0; i < h->stream_count; i++) {
    printf("    %-36s: %d : 0x%x : %d\n",
      "stream_type:elementary_PID:ES_info_length",
      h->streams[i].stream_type,
      h->streams[i].elementary_PID,
      h->streams[i].ES_info_length
      );
  }

  printf("%-40s: 0x%x\n", "CRC32", h->CRC32);
}

int is_pmt(struct ts_pat* pat, u16 pid)
{
  u16 i;

  for (i = 0; i < pat->item_count; i++) {
    if (pid == pat->program[i].PID)
      return 1;
  }

  return 0;
}

unsigned int callback(unsigned char* buffer, unsigned int length)
{
  static struct ts_header header;
  static struct ts_pat pat;
  static struct ts_pmt pmt;

  unsigned char adaptation_field_length;

  parse_ts_header(&header, buffer, length);

  if (header.adaptation_field_control == 1) {
    // no adaptation, payload only
  }
  else if (header.adaptation_field_control == 2) {
    // adaption only
  }
  else if (header.adaptation_field_control == 3) {
    // payload .... adaption (0xFF) ...
  }

  adaptation_field_length = buffer[4];

  if (adaptation_field_length > 0) {
    // parse adaption area
  }
  else {
    if (TS_PID_PAT == header.pid) {
      hexdump(buffer, length, HEXDUMP_ADDR);

      parse_ts_pat(
        &pat,
        &buffer[4 + 1 + adaptation_field_length],
        length - 4 - 1 - adaptation_field_length
      );

      dump_ts_pat(&pat);
    }
    else if (is_pmt(&pat, header.pid)) {
      hexdump(buffer, length, HEXDUMP_ADDR);

      parse_ts_pmt(
        &pmt,
        &buffer[4 + 1 + adaptation_field_length],
        length - 4 - 1 - adaptation_field_length
      );

      dump_ts_pmt(&pmt);

      return 0;
    }
  }

  return length;
}

#define COUNT (4 + 4 + 1 + 2 + 3 * 20)

unsigned int callback_parse_user_data(unsigned char* buffer, unsigned int length)
{
  unsigned int i;
  static unsigned char temp[COUNT] = { 0 };

  static int count = 0;

  if (length <= COUNT) {
    return -1;
  }

  for (i = 0; i < length - COUNT; i++) {
    if ((buffer[i] == 0x00) &&
      (buffer[i+1] == 0x00) &&
      (buffer[i+2] == 0x01) &&
      (buffer[i+3] == 0xB2))
    {
      //printf("i = %d\n", i);
      if (0 == memcmp(temp, &buffer[i], COUNT)) {
        continue;
      }
      memcpy(temp, &buffer[i], COUNT);

      //hexdump(&buffer[i], COUNT, HEXDUMP_ASCII);
      //hexdump(buffer, 4, HEXDUMP_ASCII);

#if 1
      cea708_t cea708;
      uint8_t* data;

      data = &buffer[i + 4];
      length = 4 + 1 + 2 + 3 * 20;

      libcaption_stauts_t status = cea708_parse_h262(data, length, &cea708);

#if 0
      int ix;
      char x;

      for (ix = 0; ix < cea708.user_data.cc_count; ix++) {
        if (cea708.user_data.cc_data[ix].cc_valid) {
          if ((cea708.user_data.cc_data[ix].cc_type == 0) ||
            (cea708.user_data.cc_data[ix].cc_type == 1))
           {
            //printf("%04x ", cea708.user_data.cc_data[ix].cc_data);
#if 0
            x = cea708.user_data.cc_data[ix].cc_data >> 8;
            if ((x >= 'A') && (x <= 'Z')) {
              printf("%c ", x);
            }
            x = cea708.user_data.cc_data[ix].cc_data;
            if ((x >= 'A') && (x <= 'Z')) {
              printf("%c ", x);
            }
#endif

            eia608_dump(cea708.user_data.cc_data[ix].cc_data);
          }
        }
      }
#endif
      cea708_dump(&cea708);
#endif

      break;
    }
  }

  return i;
}

int main(int argc, char *argv[]) {
  unsigned int offset = 0;
  unsigned int length = 1 << 20;
  unsigned char* buffer = malloc(1 << 20);
  unsigned int ret;

  if (param_parser(argc, argv, &app) == -1) {
    print_usage();
    return -1;
  }

  log_init(app.param.log_config);

#if 1
  ret = file_load_cb(app.param.filename, callback_parse_user_data, 188);
  //ret = file_load_cb(app.param.filename, callback, 188);
  if (-1 == ret) {
    printf("Error while reading\n");
  }
#elif 0
  cea708_t cea708;
  uint8_t *data;

  data = file_load_simple(argv[1], 0, &length);

  libcaption_stauts_t status = cea708_parse_h262(data, length, &cea708);

  cea708_dump(&cea708);
#else
  eia608_dump(0x8080);
#endif

  return 0;
}
