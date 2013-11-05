#include "honeybridge.h"

void daemonize(void)
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
		fp = fopen("pid", "w");
		if (fp) {
			fprintf(fp, "%d\n", pid);
			fclose(fp);
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
	if (setrlimit(RLIMIT_CORE, &r) == -1)
		log_msg(LOG_ERR, __FUNCTION__, 1, "Could not set core size: %s", strerror(errno));

	r.rlim_cur = 8192;
	r.rlim_max = 8192;
	if (setrlimit(RLIMIT_NOFILE, &r) == -1)
		log_msg(LOG_ERR, __FUNCTION__, 1, "Could not set file-max: %s", strerror(errno));

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

__u32 hex_to_int(char *buffer, int len)
{
	register __u32 rval = 0;
	register int i;

	for (i = 0; i < len; i++) {
		rval = rval << 4;
		rval += hex_int_values[(unsigned char) *(buffer+i)];
	}

	return rval;
}

__u32 ascii_to_int(char *buffer, int len)
{
	int count, val;
	char *p;

	for (p = buffer, count = val = 0; count < len; count++, p++) {
		if (*p >= '0' && *p <= '9')
			val = (val*10) + (*p - '0');
	}

	return val;
}
