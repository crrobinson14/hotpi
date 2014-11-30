# apt-get install python-rpi.gpio
# pip install i2c
# pip install smbus


#import RPi.GPIO as GPIO

#GPIO.setmode(GPIO.BCM)

#GPIO.setup(17, GPIO.IN)

import smbus
import time

bus = smbus.SMBus(0)  # Rev 1 Pi uses 0
#bus = smbus.SMBus(1) # Rev 2 Pi uses 1

DEVICE = 0x22  # Device address (A0-A2)
IODIRA = 0x00  # Pin direction register
GPIOA = 0x12  # Register for inputs

# Set all GPB pins as outputs
# bus.write_byte_data(DEVICE, IODIRB, ...)

# Set first 7 GPA pins as outputs and
# last one as input.
#bus.write_byte_data(DEVICE,IODIRA,0x80)

# Set all GPA pins as inputs
bus.write_byte_data(DEVICE, IODIRA, 0xFF)

# Loop until user presses CTRL-C
while True:
    # Read state of GPIOA register
    MySwitch = bus.read_byte_data(DEVICE, GPIOA)

    # blue: 111100
    print "{0:b}".format(MySwitch)

    if MySwitch & 0b10000000 == 0b10000000:
        print "Switch was pressed!"

    time.sleep(0.5)
