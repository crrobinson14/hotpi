#include "honeybridge.h"

#define LOG_BUF_SIZE 1024
char log_buffer[LOG_BUF_SIZE];

void log_init(void)
{
  openlog("honeybridge", LOG_CONS | LOG_PID, LOG_USER);
  syslog(LOG_INFO, "started");
}

void log_msg(int level, const char *func, int flush, char *fmt, ...)
{
	int bufchars = 0;
	va_list ap;

	switch(level) {
	 case LOG_CRIT:
		bufchars += sprintf(log_buffer, "ERROR: %s(): ", func);
		break;
	 case LOG_ERR:
		bufchars += sprintf(log_buffer, "ERROR: %s(): ", func);
		break;
	 case LOG_INFO:
		bufchars += sprintf(log_buffer, "INFO: ");
		break;
	 case LOG_DEBUG:
#ifdef RELEASE
		return;
#endif
		bufchars += sprintf(log_buffer, "DEBUG: %s(): ", func);
		break;
	}

	va_start(ap, fmt);
	bufchars += vsnprintf(&log_buffer[bufchars], (LOG_BUF_SIZE)-bufchars, fmt, ap);
	va_end(ap);

	bufchars++;
  syslog(level, log_buffer);

	if (level == LOG_CRIT) exit(-1);
}
