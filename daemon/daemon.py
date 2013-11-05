import time
from config import hotpiConfig
from lib.devices import deviceManager

dmgr = deviceManager()
thermostats = ['tstatdown', 'tstatbed', 'tstatfront']
zones = ['zonedown', 'zonebed', 'zonefront']

# Oil boilers are demand-driven systems. They're sized to produce a bit more
# total output than the system's maximum total demand, and demand signals drive
# them to fire.
#
# Wood systems are different. The fire can be damped but not "shut off", and
# damping it down is very inefficient. Therefore it's important to extract as
# much heat from it as efficiently as possible while the fire is burning. HotPi
# 

while True:
    dmgr.poll()

    tstatdown = hotpiConfig['devices']['tstatdown']['handler']
    tstatfront = hotpiConfig['devices']['tstatfront']['handler']
    tstatbed = hotpiConfig['devices']['tstatbed']['handler']
    woodcirc = hotpiConfig['devices']['woodcirc']['handler']
    wooddraft = hotpiConfig['devices']['wooddraft']['handler']

    zonedown = hotpiConfig['devices']['zonedown']['handler']
    zonefront = hotpiConfig['devices']['zonefront']['handler']
    zonebed = hotpiConfig['devices']['zonebed']['handler']

    dmgr.get('wooddraft').set(0)

    boilertemp = int(dmgr.get('boilertemp').status['temperature'])
    [dmgr.get(t).setMessage(boilertemp) for t in thermostats]

    [dmgr.get(z).set(1) for z in zones]

    if boilertemp < 100:
        # Below 125 we assume the boiler is off
        print "Below 125"
        [dmgr.get(t).setLEDs(0) for t in thermostats]
        dmgr.get('woodcirc').set(0)
        dmgr.get('wooddraft').set(0)

    elif boilertemp < 145:
        # Below 145 we assume the boiler is on, but not ready to heat
        print "Below 145"
        [dmgr.get(t).setLEDs(2) for t in thermostats]
        dmgr.get('woodcirc').set(0)
        # dmgr.get('wooddraft').set(1)

    elif boilertemp < 165:
        print "Below 165"
        [dmgr.get(t).setLEDs(2) for t in thermostats]
        dmgr.get('woodcirc').set(0)
        dmgr.get('wooddraft').set(0)

    elif boilertemp < 185:
        print "Below 185"
        [dmgr.get(t).setLEDs(1) for t in thermostats]
        # TODO: Set based on demand
        dmgr.get('woodcirc').set(1)
        dmgr.get('wooddraft').set(0)

    else:
        print "Above 185"
        [dmgr.get(t).setLEDs(4) for t in thermostats]
        dmgr.get('woodcirc').set(1)
        dmgr.get('wooddraft').set(0)

    time.sleep(10)
