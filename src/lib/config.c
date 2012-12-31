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

// Config files consist of one parameter per line, with a space between the
// parameter name and its value, as in:
//   name=value
//
// Comments may be prefixed with a '#' or other punctuation.  - they are not
// identified as such, but the '#' will prevent the line from matching a
// known parameter name, so it works as expected.

/**
 * Read a configuration file from disk.
 */
void read_config(const char *filename, CONFIG *c)
{
	FILE *fp;
	char linebuf[1024], *line, *name, *value, *delim;
	void (*config_callback) (char *str);
	int i;

	fp = fopen(filename, "r");
	if (!fp) {
		CRIT(1, "Unable to open config file %s for reading.", filename);
	}

	while (!feof(fp)) {
		line = fgets(linebuf, 1024, fp);
		if (!line) continue;

		if (strlen(linebuf) < 2)
			continue;

		delim = strchr(linebuf, '=');
		if (!delim) {
			continue;
		}

		*delim++ = '\0';

		name = str_trim(linebuf);
		value = str_trim(delim);

		for (i = 0; c[i].parameter; i++) {
			if (!strcmp(name, c[i].parameter)) {
				switch (c[i].type) {
				 case CONFIG_TYPE_INT:
					* (int *) c[i].value = atoi(value);
					INFO(1, "New setting for %s: %d", name, * (int *) c[i].value);
					break;

				 case CONFIG_TYPE_STR:
					strncpy(c[i].value, value, 256);
					INFO(1, "New setting for %s: %s", name, value);
					break;

				 case CONFIG_TYPE_CALLBACK:
					config_callback = c[i].value;
					config_callback(name);
					break;

				 case CONFIG_TYPE_FLOAT:
					* (double *) c[i].value = atof(value);
					INFO(1, "New setting for %s: %f", name, * (double *) c[i].value);
					break;
				}
			}
		}
	}

	fclose(fp);
}
