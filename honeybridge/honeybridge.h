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
#include <stdarg.h>
#include <sys/time.h>
#include <sys/resource.h>

// Raspberry Pi specific
#include <asm-generic/int-l64.h>

// Logging
extern void log_init(void);
extern void log_msg(int level, const char *func, int flush, char *fmt, ...) __attribute__ ((format (printf, 4, 5)));

#define INFO(sync, format, args...) log_msg(LOG_INFO, __FUNCTION__, sync, format , ## args);
#define CRIT(sync, format, args...) log_msg(LOG_CRIT, __FUNCTION__, sync, format , ## args);
#define ERROR(sync, format, args...) log_msg(LOG_ERR, __FUNCTION__, sync, format , ## args);
#define DEBUG(sync, format, args...) log_msg(LOG_DEBUG, __FUNCTION__, sync, format , ## args);

// Utils
void daemonize(void);
void log_msg(int level, const char *func, int flush, char *fmt, ...);

#endif