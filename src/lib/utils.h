#ifndef __LIB_UTILS_H_
#define __LIB_UTILS_H_

extern struct timeval curtime;

extern char *str_trim(char *str);

extern void set_time(struct timeval *tv);

extern void daemonize(const char *pidfile);

extern unsigned int hex_to_int(char *buffer, int len);
extern unsigned int ascii_to_int(char *buffer, int len);

extern int file_exists(char *filename);
extern void file_nuke(char *filename);
extern void file_touch(char *filename);
extern time_t file_mtime(char *filename);

extern void csv_load(void *p, char *filename, void (*csv_callback) (void *p, int count, char **parms));
extern void read_delimited_ints(char *str, char delim, int max, int **vals);
extern void split_delimited(char *str, char delimiter, char *output[], int max_entries, int *found);

#endif
