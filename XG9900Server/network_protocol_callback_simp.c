#include "time.h"

#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <sys/stat.h>

unsigned short network_protocol_callback_simp(unsigned char *buffer, unsigned short length)
{
  unsigned long long timestamp;
  unsigned char sn[16+1];
  unsigned char *payload;

  DIR *dir;
  unsigned char path[FILENAME_MAX];
  unsigned char filename[FILENAME_MAX];
  struct datetime *dt;
  FILE *fp;

  printf("Enter SIMP process\n");
  
  if (8 + 16 + 360 != length) {
    return -1;
  }
  
  payload = &buffer[8 + 16];
  
  memcpy(sn, &buffer[8], 16);
  sn[16] = '\0';

  memset(path, 0, FILENAME_MAX);
  snprintf(path, FILENAME_MAX, "/tmp/%s", sn);
  dir = opendir(path);
  if (NULL == dir) {
    mkdir(path, 0755);
  }
  closedir(dir);

  dt = datetime_get();
  memset(filename, 0, FILENAME_MAX);
  snprintf(filename, FILENAME_MAX, "/tmp/%s/%d-%02d-%02d.para",
      sn,
      dt->year, dt->month, dt->day);
  
  fp = fopen(filename, "ab");
  if (NULL == fp)
    return -1;

  fwrite(payload, sizeof(char), length - 8 - 16, fp);

  printf("SIMP done\n");

  return 0;
}
