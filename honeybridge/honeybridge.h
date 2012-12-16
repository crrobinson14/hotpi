#ifndef __HONEYBRIDGE_H_
#define __HONEYBRIDGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>
#include <ulimit.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/ioctl.h>

// Raspberry Pi specific
#include <asm-generic/int-l64.h>

// General definitions
#define BUFFER_SIZE 2048

// Logging
extern void log_init(void);
extern void log_msg(int level, const char *func, int flush, char *fmt, ...) __attribute__ ((format (printf, 4, 5)));

#define INFO(sync, format, args...) log_msg(LOG_INFO, __FUNCTION__, sync, format , ## args);
#define CRIT(sync, format, args...) log_msg(LOG_CRIT, __FUNCTION__, sync, format , ## args);
#define ERROR(sync, format, args...) log_msg(LOG_ERR, __FUNCTION__, sync, format , ## args);
#define DEBUG(sync, format, args...) log_msg(LOG_DEBUG, __FUNCTION__, sync, format , ## args);

// Utils
void daemonize(void);
__u32 hex_to_int(char *buffer, int len);
__u32 ascii_to_int(char *buffer, int len);

// Serial I/O
void read_serial(int fd);
int serial_init(char *port);

typedef struct enviramsg {
  __u8 priority;   // 0 = low, 0x40 = medium, 0x80 = high
  __u16 type;
  __u8 instance;
  __u8 command;    // 0 = query, 0x40 = report, 0x80 = change
  __u8 length;
  __u8 payload[32];
  __u8 checksum;
} ENVIRAMSG;

// M 1081 3D R 05 23 64 02 30 00 DC
typedef struct envirahdr {
  char priority;      // H, M, L
  char spc1;          // _
  char type[4];       // 1081
  char spc2;          // _
  char instance[2];   // 3D
  char spc3;          // _
  char command;       // Q, R, C
  char spc4;          // _
  char length[2];     // 05
  char spc5;
  char payload;
} ENVIRAHDR;

#endif