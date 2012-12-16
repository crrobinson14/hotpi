#include "honeybridge.h"

typedef struct aquastatus {
  int circulator;
  float tempf;
  float tempc;
} AQUASTATUS;

static AQUASTATUS aquastatus;

void output_status(void) {
  FILE *fp = fopen("/tmp/honeybridge.txt", "w");
  if (!fp) return;

  fprintf(fp, "Aquastat Status\n"
              "Circulator: %s\n"
              "Temp: %.2fF %.2fC\n",
              (aquastatus.circulator == 1) ? "On" : "Off",
              aquastatus.tempf, aquastatus.tempc);

  fclose(fp);
}

/**
 * This is just a hard-coded message for now. Not really sure what we want
 * out of it.
 */
void process_enviracom(ENVIRAMSG *msg) {
  __u16 tempcode;

  // We only process 'R' messages
  if (msg->command != 0x40) {
    return;
  }

  switch (msg->type) {
    // M 1081 3D R 05 23 64 02 30 00 DC
    case 0x1081:
      break;

    // M 1082 3D R 05 1B C6 02 30 00 45

    // M 1084 3D R 08 2D 32 15 40 04 56 01 18 A0

    // M 1085 3D R 08 28 C8 10 EA 05 6E 02 30 E3

    // M 3114 3D R 03 01 C8 11 C3
    case 0x3114:
      break;

    // Circulator status
    // M 3180 3D R 01 C8 45
    case 0x3180:
      aquastatus.circulator = (msg->payload[0] == 0xC8) ? 1 : 0;
      break;

    // Boiler temperature
    case 0x3200:
      tempcode = (msg->payload[0] << 8) + (msg->payload[1]);
      aquastatus.tempf = (tempcode * 0.018) + 32;
      aquastatus.tempc = (tempcode / 100.0);
      break;

    // M 3E70 11 R 05 00 00 01 80 0C D7
    case 0x3E70:
      break;
  }

  output_status();
}
