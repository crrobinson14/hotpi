/**
 * HotPi services library
 * Copyright (c) 2012-2013 Chad Robinson
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "hotpi.h"

/**
 * Open the logging facility.
 */
void log_init(const char *ident)
{
	openlog(ident, LOG_CONS | LOG_NDELAY | LOG_PID, LOG_USER);

	INFO(1, "Program started.");
}

/**
 * Return a formatted string that can be passed to sprintf or another logging
 * function without it choking on binary data. This function is NOT thread safe!
 */
char *log_convert_msg(char *msg, int bytes)
{
	static char outbuf[65536];
	int outbytes, inbyte;

	for (outbytes = inbyte = 0; inbyte < bytes && outbytes < 65536-4; inbyte++) {
		if (msg[inbyte] < 0x20 || msg[inbyte] > 0x7E) {
			sprintf(&outbuf[outbytes], "<%02X>", (unsigned char) msg[inbyte]);
			outbytes += 4;
		} else {
			outbuf[outbytes] = msg[inbyte];
			outbytes++;
		}
		outbuf[outbytes] = 0x00;
	}

	return outbuf;
}

/**
 * Return a formatted string that can be passed to sprintf or another logging
 * function without it choking on binary data. This function is NOT thread safe!
 */
char *log_hex_msg(char *msg, int bytes)
{
	static char outbuf[65536];
	int outbytes, inbyte;

	for (outbytes = inbyte = 0; inbyte < bytes && outbytes < 65536-4; inbyte++) {
		sprintf(&outbuf[outbytes], "<%02X>", (unsigned char) msg[inbyte]);
		outbytes += 4;
		outbuf[outbytes] = 0x00;
	}

	return outbuf;
}

/**
 * Log a message to the logging service.
 */
void log_msg(int level, const char *func, int flush, char *fmt, ...)
{
	va_list ap;

#ifdef RELEASE
	if (level == LOG_DEBUG) return;
#endif

	va_start(ap, fmt);
	vsyslog(level, fmt, ap);
	va_end(ap);

	if (level == LOG_CRIT) exit(-1);
}
