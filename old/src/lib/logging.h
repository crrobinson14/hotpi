#ifndef __LIB_LOGGING_H_
#define __LIB_LOGGING_H_

extern void log_init(const char *ident);
extern void log_msg(int level, const char *func, int flush, char *fmt, ...) __attribute__ ((format (printf, 4, 5)));

#endif
