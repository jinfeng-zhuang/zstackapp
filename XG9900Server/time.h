#ifndef ZSTACK_TIME_H
#define ZSTACK_TIME_H

struct datetime {
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec;
  int usec;
};

extern struct datetime *datetime_get(void);

#endif