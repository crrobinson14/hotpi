#ifndef __LIB_CONFIG_H_
#define __LIB_CONFIG_H_

#define CONFIG_TYPE_INT 1
#define CONFIG_TYPE_STR 2
#define CONFIG_TYPE_FLOAT 3
#define CONFIG_TYPE_CALLBACK 4

typedef struct config CONFIG;
struct config {
	char *parameter;
	int type;
	void *value;
};

void read_config(const char *filename, CONFIG *c);

#endif
