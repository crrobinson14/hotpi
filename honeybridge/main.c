#include "honeybridge.h"

int main(int argc, char *argv[]) {
  struct timeval timeout;
  fd_set input;
  int max_fd, serial_fd, n;

  log_init();

  if (argc < 2) {
    printf("Usage: honeybridge /dev/ttyUSB0\n");
    return -1;
  }

  max_fd = serial_fd = serial_init(argv[1]);

  // Main loop
  for (;;) {
    FD_ZERO(&input);
    FD_SET(serial_fd, &input); // All we have for now - we'll add the pipes later

    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    n = select(max_fd + 1, &input, NULL, NULL, &timeout);

    if (n < 0) {
      ERROR(1, "select failed");
    } else if (n > 0) {
      if (FD_ISSET(serial_fd, &input)) {
        read_serial(serial_fd);
      }
    }

    // TODO: Housekeeping
  }

  return 0;
}
