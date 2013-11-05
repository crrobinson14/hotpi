/**
 * Filling - HotPi zone controller server-side bridge (talks to Crust via RS-485)
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

/**
 * The serial protocol is as follows (19200, 8N1):
 *   Filling to Crust:
 *     Fzs\r\n - Force zone 'z' to state 's', where:
 *               Zone is the zone number, 1-8
 *               State is 0 for "pass through" or 1 for "force on"
 *
 *   Crust to Filling:
 *     Fzzzzzzzz\r\n - List zone "force" states. Each 'z' is a zone from 1-8,
 *                     with 0/1 states matching the Force command from Filling.
 *
 *     Szzzzzzzz\r\n - List zone end switch states. Same format as above.
 */

#include "filling.h"

static char serial_buffer[BUFFER_SIZE];
static int in_buffer = 0;

int handle_msg(char *msg, int bytes) {
    return bytes;

}

void read_serial(int fd) {
  int bytes, total, proc;

  // Data on the serial port
  ioctl(fd, FIONREAD, &bytes);
  if (bytes < 0) return;

  bytes = read(fd, &serial_buffer[in_buffer], BUFFER_SIZE - in_buffer);
  if (bytes == -1) {
    ERROR(1, "Error reading from serial port: %d (%s)\n", errno, strerror(errno));
  } else {
    fwrite(&serial_buffer[in_buffer], 1, bytes, stdout);
    fflush(stdout);

    in_buffer += bytes;
  }

  if (in_buffer > 0) {
    for (total = 0, proc = 1; proc && in_buffer > 0 && total < in_buffer; total += proc) {
      proc = handle_msg(&serial_buffer[total], in_buffer - total);
    }

    if (total && in_buffer > 0) {
      if (total >= in_buffer) {
        in_buffer = 0;
      } else {
        memmove(serial_buffer, &(serial_buffer[total]), in_buffer - total);
        in_buffer -= total;
      }
    }
  }
}

int serial_init(char *port) {
  struct termios options;
  int fd;

  INFO(1, "Opening serial port %s\n", port);

  fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (!fd) {
    CRIT(1, "Error opening serial port: %d (%s)", errno, strerror(errno));
  }

  /* set raw input, 1 second timeout */
  fcntl(fd, F_SETFL, 0);
  tcgetattr(fd, &options);
  options.c_cflag |= (CLOCAL | CREAD);
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_oflag &= ~OPOST;
  options.c_cc[VMIN]  = 0;
  options.c_cc[VTIME] = 10;

  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  cfsetispeed(&options, B19200);
  cfsetospeed(&options, B19200);

  tcsetattr(fd, TCSANOW, &options);

  return fd;
}
