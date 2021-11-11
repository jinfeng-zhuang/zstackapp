#include "time.h"

#include <sys/time.h>
#include <time.h>

static struct datetime dt;

struct datetime *datetime_get(void)
{
	struct tm *tm;
	time_t now;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	time(&now);
	tm = localtime(&now);
  
  dt.year = tm->tm_year + 1900;
  dt.month = tm->tm_mon + 1;
  dt.day = tm->tm_mday;
  dt.hour = tm->tm_hour;
  dt.min = tm->tm_min;
  dt.sec = tm->tm_sec;
  dt.usec = tv.tv_usec;
  
  return &dt;
}
