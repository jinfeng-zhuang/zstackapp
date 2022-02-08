#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// return how many bytes processed
// if return 0: stop
typedef unsigned int (*flcb)(unsigned char* buffer, unsigned int length);

// size: callback required block size
int file_load_cb(const char *filename, flcb cb, unsigned int size)
{
  FILE* fp = NULL;
  size_t bytesread;
  unsigned char* buffer = NULL;
  size_t bytesproc;
  size_t bytes2read;
  size_t offset;
  size_t remain;
  int eof = 0;
  int ret = 0;

  if ((NULL == filename) || (NULL == cb) || (0 == size)) {
    ret = -1;
    goto END;
  }

  fp = fopen(filename, "rb");
  if (NULL == fp) {
    ret = -1;
    goto END;
  }

  buffer = (unsigned char* )malloc(size);
  if (NULL == buffer) {
    ret = -1;
    goto END;
  }

  bytes2read = size;
  offset = 0;

  unsigned char tmpbuf[188];

  while (1) {
    bytesread = fread(&buffer[offset], sizeof(unsigned char), bytes2read, fp);
    if (bytes2read != bytesread) {
      if (ferror(fp)) {
        printf("file_load_cb read failed\n");
        break;
      }
      else {
        printf("file_load_cb reach end: %d %lld %lld\n", size, bytes2read, bytesread);
        eof = 1;
      }
    }

    bytesproc = cb(buffer, bytesread);
    if (bytesproc < bytesread) {
      remain = bytesread - bytesproc;
      memcpy(buffer, &buffer[bytesproc], remain);
      bytes2read = size - remain;
      offset = remain;
    }
    else {
      bytes2read = size;
      offset = 0;
    }

    if (UINT_MAX == bytesproc) {
      printf("Warning: user stop file_load_cb\n");
      break;
    }

    if (eof) {
      break;
    }
  }

END:
  if (buffer) {
    free(buffer);
  }
  if (fp) {
    fclose(fp);
  }

  return ret;
}
