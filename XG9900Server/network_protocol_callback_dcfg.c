#include "time.h"

#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <sys/stat.h>

unsigned short network_protocol_callback_dcfg(unsigned char *buffer, unsigned short length)
{
  unsigned long long timestamp;
  unsigned char sn[16+1];

  DIR *dir;
  unsigned char path[FILENAME_MAX];
  unsigned char filename[FILENAME_MAX];
  FILE *fp;

  unsigned int payload_size;

  if (8 + 16 != length) {
    printf("DCFG: Length not valid\n");
    return -1;
  }
  
  memcpy(sn, &buffer[8], 16);
  sn[16] = '\0';

  memset(path, 0, FILENAME_MAX);
  snprintf(path, FILENAME_MAX, "/tmp/%s", sn);
  dir = opendir(path);
  if (NULL == dir) {
    mkdir(path, 0755);
  }
  closedir(dir);

  memset(filename, 0, FILENAME_MAX);
  snprintf(filename, FILENAME_MAX, "/tmp/%s/config.json", sn);
  
  fp = fopen(filename, "r");
  if (NULL == fp) {
    log2buffer("%s not found\n", filename);
    return -1;
  }

  fseek(fp, 0, SEEK_END);
  payload_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  if (0 == payload_size)
    return -1;

  // TODO split if big then
  if (payload_size > 65536)
    return -1;

  fread(buffer, sizeof(char), payload_size, fp);

  fclose(fp);

  return payload_size;
}
