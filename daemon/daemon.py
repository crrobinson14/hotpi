import time
from config import hotpiConfig
from lib.devices import deviceManager

dmgr = deviceManager()
thermostats = ['tstatdown', 'tstatfront', 'tstatbed']

while True:
    dmgr.poll()

    tstatdown = hotpiConfig['devices']['tstatdown']['handler']
    tstatfront = hotpiConfig['devices']['tstatfront']['handler']
    tstatbed = hotpiConfig['devices']['tstatbed']['handler']
    woodcirc = hotpiConfig['devices']['woodcirc']['handler']
    wooddraft = hotpiConfig['devices']['wooddraft']['handler']

    boilertemp = int(dmgr.get('boilertemp').status['temperature'])
    [dmgr.get(t).setMessage(boilertemp) for t in thermostats]

    if boilertemp < 125:
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
        dmgr.get('wooddraft').set(1)

    elif boilertemp < 165:
        print "Below 165"
        [dmgr.get(t).setLEDs(2) for t in thermostats]
        dmgr.get('woodcirc').set(0)
        dmgr.get('wooddraft').set(1)

    elif boilertemp < 185:
        print "Below 185"
        [dmgr.get(t).setLEDs(1) for t in thermostats]
        # TODO: Set based on demand
        dmgr.get('woodcirc').set(1)
        dmgr.get('wooddraft').set(1)

    else:
        print "Above 185"
        [dmgr.get(t).setLEDs(4) for t in thermostats]
        dmgr.get('woodcirc').set(1)
        dmgr.get('wooddraft').set(0)

    time.sleep(10)
