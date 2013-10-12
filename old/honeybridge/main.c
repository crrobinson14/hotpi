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

  //daemonize();
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



/*
HL 195
HlD 10

>M 3200 3D R 02 1E 96 85
>M 3180 3D R 01 00 8D
>M 3114 3D R 03 01 00 10 0A
>M 3E70 11 R 05 00 00 01 80 0C D7
>M 1085 3D R 08 28 C8 10 EA 05 6E 02 30 E3
>M 1084 3D R 08 2D 32 15 40 04 56 01 18 A0
>M 1082 3D R 05 1D F6 02 30 00 73
>M 1081 3D R 05 23 64 02 30 00 DC


HL 194
HlD 9

>M 1081 3D R 05 23 28 02 30 00 90
>M 3114 3D R 03 01 C8 13 C1
>M 3E70 11 R 05 C8 00 08 80 0C 16


Message 1081 bytes 0/1 are the HL.


*/