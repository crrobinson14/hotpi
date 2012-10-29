hotpi PCB
=========

The Raspberry Pi board has a number of GPIO pins, but they run at 3.3V with low
sink/source capacity, and it has no ADC channels. To be useful as a controller
the system needs to be able to monitor a variety of temperatures and switches,
and drive relays to control the various devices. This PCB does the following:

 - Serves as a carrier to hold the Raspberry Pi main board
 - Provides 8 SPDT relays with status LEDs that can control external devices
 - Provides 10 input channels that can read either thermistors or limit switches
   plus an additional on-board temperature sensor (basement temperature input)
 - Has three push-button switches and two status LEDs for general user interface
 - Holds the terminal strips that are used to make the electrical connections

These board files are in DipTrace format. DipTrace is an excellent schematic/PCB
design tool with a free version for demo purposes. But you don't need to use it
to view these files - Gerbers are also included here, as well as a PDF of the
schematic.
