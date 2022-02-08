#include <zstack/bitstream.h>
#include <stdio.h>

int bitstream_init(struct bitstream* bs, unsigned char* buffer, unsigned int length)
{
  memset(bs, 0, sizeof(struct bitstream));

  bs->buffer = buffer;
  bs->length = length;

  bs->bitpos = 0;
  bs->bitlen = length * 8;

  return 0;
}

int bitstream_copy(struct bitstream* bs, unsigned char* buffer, unsigned int length)
{
  unsigned int byte_count;
  unsigned int byte_offset;

  byte_count = bs->bitpos >> 3;
  byte_offset = bs->bitpos & 0x7;

  if (byte_offset) {
    printf("warning: bitstream not aligned\n");
    return -1;
  }

  if (byte_count + length > bs->length) {
    printf("warning: bitstream out of range\n");
    return -1;
  }

  memcpy(buffer, &bs->buffer[byte_count], length);

  bs->bitpos += length * 8;

  return 0;
}

int bitstream_eof(struct bitstream* bs)
{
  return (bs->bitpos == bs->bitlen) ? 1 : 0;
}

unsigned int bitstream_u(struct bitstream* bs, int count)
{
  unsigned int value = 0;
  int i;

  unsigned int byte_count;
  unsigned int byte_offset;

  if (count > 32) {
    printf("warning: bitstream count > 32, return 0\n");

    bs->bitpos += count;

    return 0;
  }

  if (bs->bitpos + count > bs->bitlen) {
    printf("warning: bitstream out of range: %d %d %d\n",
      bs->bitpos, count, bs->bitlen);
    return 0;
  }

  for (i = 0; i < count; i++, bs->bitpos++) {
    byte_count = bs->bitpos >> 3;
    byte_offset = bs->bitpos & 0x7;

    value = value << 1;

    value = value | ((bs->buffer[byte_count] & (1 << (7 - byte_offset))) ? 1 : 0);
  }

  return value;
}
