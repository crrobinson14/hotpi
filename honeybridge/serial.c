#include "honeybridge.h"

static char serial_buffer[BUFFER_SIZE];
static int in_buffer = 0;
static ENVIRAMSG emsg;

int handle_msg(char *msg, int bytes) {
  int length, i;
  char *end;
  ENVIRAHDR *hdr;

  switch (msg[0]) {
    // These occur at message boundaries so we ignore them
    case 0x0A: return 1;
    case 0x0D: return 1;
    case '>': return 1;

    // We discard these messages at the moment
    case '[':
      return bytes;

    // One of the message types we know. Qualify and decode the message.
    case 'H':
    case 'M':
    case 'L':
      // See if we have a complete message terminated with a newline. The spec
      // says we should get 0x0D 0x0A, but I'm getting 0x0A 0x0A on mine.
      end = memchr(msg, 0x0A, bytes);
      if (!end) return 0;

      // We need at least 17 bytes to contain even a 0-byte-payload message
      length = end - msg;
      if (length < 17) {
        return bytes;
      }

      // Decode the message. We don't bother with the checksum right now.
      hdr = (ENVIRAHDR *) msg;
      emsg.priority = (hdr->priority == 'H') ? 0x00 : (hdr->priority == 'M' ? 0x40 : 0x80);
      emsg.type = hex_to_int(hdr->type, sizeof(hdr->type));
      emsg.instance = hex_to_int(hdr->instance, sizeof(hdr->instance));
      emsg.command = (hdr->command == 'Q') ? 0x00 : (hdr->command == 'R' ? 0x40 : 0x80);
      emsg.length = hex_to_int(hdr->length, sizeof(hdr->length));
      for (i = 0; i < emsg.length && i < sizeof(emsg.payload); i++) {
        emsg.payload[i] = hex_to_int(&hdr->payload[i * 3], 2);
      }

      process_enviracom(&emsg);

      return length;

    // Unknown message - probably started mid-stream. Discard to sync up
    default:
      return bytes;
  }
}

void read_serial(int fd) {
  int bytes, i, total, proc;

  // Data on the serial port
  ioctl(fd, FIONREAD, &bytes);
  if (bytes < 0) return;

  bytes = read(fd, &serial_buffer[in_buffer], BUFFER_SIZE - in_buffer);
  if (bytes == -1) {
    ERROR(1, "Error reading from serial port: %d (%s)\n", errno, strerror(errno));
  } else {
    for (i = 0; i < bytes; i++) {
      fwrite(&serial_buffer[in_buffer], 1, bytes, stdout);
      fflush(stdout);
    }

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
