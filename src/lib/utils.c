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

struct timeval curtime;

/**
 * Trim a string of whitespace. Note that for efficiency, this is done IN PLACE,
 * and the pointer that is returned is just the offset into the original string
 * where the "real" string starts.
 */
char *str_trim(char *str) {
  char *end;

  // Leading
  while (isspace(*str)) {
		str++;
	}

  if (!*str) return str;

  // Trailing
  end = str + strlen(str) - 1;
  while (end > str && isspace(*end)) {
		end--;
  }

  *(end+1) = '\0';
  return str;
}

/**
 * Set the current time of day, or get it (if called with NULL).
 */
void set_time(struct timeval *tv)
{
	if (!tv) {
		gettimeofday(&curtime, NULL);
	} else {
		curtime.tv_sec = tv->tv_sec;
		curtime.tv_usec = tv->tv_usec;
	}
}

/**
 * Daemonize the current process.
 */
void daemonize(const char *pidfile)
{
	FILE *fp;
	int pid;
	struct rlimit r;

	pid = fork();
	if (pid == -1) {
		CRIT(1, "Could not fork daemon process: %s", strerror(errno));
	}

	if (pid > 0) {
		printf("Service started as process %d\n", pid);
		fp = fopen(pidfile, "w");
		if (fp) {
			fprintf(fp, "%d\n", pid);
			fclose(fp);
		} else {
			CRIT(1, "Could not open PID file %s", pidfile);
		}

		exit(0);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	if (setsid() == -1) {
		ERROR(1, "Could not set session ID: %s", strerror(errno));
	}

	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	r.rlim_cur = 10000000;
	r.rlim_max = 10000000;
	if (setrlimit(RLIMIT_CORE, &r) == -1) {
		ERROR(1, "Could not set core size: %s", strerror(errno));
	}

	r.rlim_cur = 8192;
	r.rlim_max = 8192;
	if (setrlimit(RLIMIT_NOFILE, &r) == -1) {
		ERROR(1, "Could not set file-max: %s", strerror(errno));
	}

	setpgid(pid, pid);
	tcsetpgrp(0, pid);
}

static unsigned char hex_int_values[256] = {
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // 00-0F
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // 10-1F
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // 20-2F
	0,  1,  2,  3,  4,  5,  6,  7,    8,  9,  0,  0,  0,  0,  0,  0, // 30-3F
	0, 10, 11, 12, 13, 14, 15,  0,    0,  0,  0,  0,  0,  0,  0,  0, // 40-4F
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // 50-5F
	0, 10, 11, 12, 13, 14, 15,  0,    0,  0,  0,  0,  0,  0,  0,  0, // 60-6F
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // 70-7F
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // 80-8F
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // 90-9F
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // A0-AF
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // B0-BF
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // C0-CF
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // D0-DF
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, // E0-EF
	0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0  // F0-FF
};

/**
 * Convert a hex string to an unsigned integer.
 */
unsigned int hex_to_int(char *buffer, int len)
{
	register uint32_t rval = 0;
	register int i;

	for (i = 0; i < len; i++) {
		rval = rval << 4;
		rval += hex_int_values[(unsigned char) *(buffer+i)];
	}

	return rval;
}

/**
 * Convert an ASCII string to an unsigned integer.
 */
unsigned int ascii_to_int(char *buffer, int len)
{
	int count, val;
	char *p;

	for (p = buffer, count = val = 0; count < len; count++, p++) {
		if (*p >= '0' && *p <= '9')
			val = (val*10) + (*p - '0');
	}

	return val;
}

/**
 * Helper function to determine whether a file exists.
 */
int file_exists(char *filename)
{
	struct stat st;

	return !stat(filename, &st);
}

/**
 * Helper function to obtain just the last-modification time for a file.
 */
time_t file_mtime(char *filename)
{
	struct stat st;

	if (stat(filename, &st)) {
		return 0;
	}

	return st.st_mtime;
}

/**
 * Helper function to delete a file.
 */
void file_nuke(char *filename)
{
	unlink(filename);
}

/**
 * Helper file to force-set the last-modification time for a file to "now".
 */
void file_touch(char *filename)
{
	FILE *fp;

	fp = fopen(filename, "w");
	if (fp) fclose(fp);
}

void csv_load(void *p, char *filename, void (*csv_callback) (void *p, int count, char **parms))
{
	FILE *fp;
	char buffer[2048];
	char *parms[256], *c;
	int count, length;

	fp = fopen(filename, "r");
	if (!fp) return;

	while (!feof(fp)) {
		c = fgets(buffer, sizeof(buffer), fp);
		length = strlen(buffer);

		parms[0] = buffer;
		for (count = 0, c = &buffer[0]; count < 256 && *c && c < &buffer[2048]; c++) {
			switch (*c) {
			 case '\n':
			 case '\r':
			 case '\t':
				*c = '\0';
				break;

			 case ',':
				count++;
				*c = '\0';
				c++;
				parms[count] = c;
				break;
			}
		}

		if (!*c && c < &buffer[2048]) {
			count++;
		}

		if (count && csv_callback) csv_callback(p, count, parms);
	}
}

/**
 * Split a string into chunks. This is much safer than strtok(), although it
 * does still overwrite the original string.
 */
void split_delimited(char *str, char delimiter, char *output[], int max_entries, int *found)
{
	int length = strlen(str);
	char *c;

	*found = 1;
	output[0] = str;

	for (c = str; *c && c < (str + length) && *found < max_entries; c++) {
		if (*c == delimiter) {
			*c = '\0';
			output[*found] = c+1;
			(*found)++;
    }
  }
}

/**
 * Read a delimited list of integers from a string.
 */
void read_delimited_ints(char *str, char delim, int max, int **vals)
{
    char buffer[2048];
    char *c, *c2;
    int i;

    strncpy(buffer, str, 2048);
    buffer[2047] = '\0';

    for (c = buffer, i = 0; i < max; i++) {
        c2 = strchr(c, delim);
        if (!c2) c2 = c + strlen(c);
        *c2 = '\0';
        *vals[i] = atoi(c);
        c = c2 + 1;
    }
}
