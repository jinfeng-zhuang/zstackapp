#ifndef _ZSTACK_TS_H_
#define _ZSTACK_TS_H_

#define TS_PACKAGE_SIZE (188)

#define TS_PID_PAT  (0)

struct ts_header {
  u32 sync_byte : 8;
  u32 transport_error_indicator : 1;
  u32 payload_unit_start_indicator : 1;
  u32 transport_priority : 1;
  u32 pid : 13;
  u32 transport_scrambling_control : 2;
  u32 adaptation_field_control : 2;
  u32 continuity_counter : 2;
};

struct ts_adaption {
  u8 adaptation_field_length;
  u8 flag;
  u8 PCR[5];
  u8 stuffing_bytes[1];
};

struct ts_pat {
  u8 table_id;
  u16 section_syntax_indicator : 1;
  u16 zero : 1;
  u16 reserved : 2;
  u16 section_length : 12;
  u16 transport_stream_id;
  u8 reserved2 : 2;
  u8 version_number : 5;
  u8 current_next_indicator : 1;
  u8 section_number;
  u8 last_section_number;
  struct {
    u16 program_number;
    u16 reserved : 3;
    u16 PID : 13;
  } program[1024];
  u32 CRC32;

  // defined by developer
  u16 item_count;
};

struct ts_pmt {
  u8 table_id;
  u16 section_syntax_indicator : 1;
  u16 zero : 1;
  u16 reserved : 2;
  u16 section_length : 12;
  u16 program_number : 16;
  u8 reserved1 : 2;
  u8 version_number : 5;
  u8 current_next_indicator : 1;
  u8 section_number;
  u8 last_section_number;
  u16 reserved2 : 3;
  u16 PCR_PID : 13;
  u16 reserved3 : 4;
  u16 program_info_length : 12;
  struct {
    u8 stream_type;
    u16 reserved4 : 3;
    u16 elementary_PID : 13;
    u16 reserved5 : 4;
    u16 ES_info_length : 12;
  } streams[1024];
  u32 CRC32;

  // user defined
  u16 stream_count;
  u8 ES_info[4096];
};

#endif
