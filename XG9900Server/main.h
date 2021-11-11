#ifndef MAIN_H
#define MAIN_H

#define RECV_BUFFER_MAX (1<<20)

extern int client_thread_int(void);
extern int client_thread_start(void *(*start_rtn)(void *), void *arg);
extern int client_thread_end(void);

extern void *client_thread(void *arg);

extern int log_thread_init(void);
extern int log2buffer(const char *fmt, ...);
//
//#define log2buffer printf

extern void *log_dump_thread(void *arg);

extern unsigned short crc16_ccitt(unsigned char *pbuf, unsigned int len);

#endif
