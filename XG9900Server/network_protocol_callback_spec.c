#include "time.h"

#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <sys/stat.h>

unsigned short network_protocol_callback_spec(unsigned char *buffer, unsigned short length)
{
  unsigned long long timestamp;
  unsigned char sn[16+1];
  unsigned char *payload;

  DIR *dir;
  unsigned char path[FILENAME_MAX];
  unsigned char filename[FILENAME_MAX];
  struct datetime *dt;
  FILE *fp;
  
  if (8 + 16 + 5516 != length) {
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
  snprintf(filename, FILENAME_MAX, "/tmp/%s/%d-%02d-%02d_%02d-%02d-%02d_%d.vspe",
      sn,
      dt->year, dt->month, dt->day,
      dt->hour, dt->min, dt->sec, dt->usec);
  
  fp = fopen(filename, "ab");
  if (NULL == fp)
    return -1;

  fwrite(payload, sizeof(char), length - 8 - 16, fp);

  printf("SPEC done\n");

  return 0;
}
