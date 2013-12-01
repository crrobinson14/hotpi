import time
from config import hotpiConfig
from lib.devices import deviceManager

dmgr = deviceManager()
thermostats = ['tstatdown', 'tstatbed', 'tstatfront']
zones = ['zonedown', 'zonebed', 'zonefront']

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

    # dmgr.get('wooddraft').set(0)

    boilertemp = int(dmgr.get('boilertemp').status['temperature'])
    [dmgr.get(t).setMessage(boilertemp) for t in thermostats]

    # Force all the zones open for now
    [dmgr.get(z).set(1) for z in zones]

    demand = (tstatfront.status['temp'] < 70)

    if boilertemp < 125:
        # Below 125 we assume the boiler is off
        dmgr.globalstat = "Below 125, offline"
        [dmgr.get(t).setLEDs(0) for t in thermostats]
        dmgr.get('woodcirc').set(0)
        dmgr.get('wooddraft').set(0)

    elif boilertemp < 145:
        # Below 145 we try to preserve coals
        dmgr.globalstat = "Below 145, preserving coals"
        [dmgr.get(t).setLEDs(2) for t in thermostats]
        dmgr.get('woodcirc').set(0)
        dmgr.get('wooddraft').set(0)

    elif boilertemp < 165:
        # Below 165 we get the fire going
        dmgr.globalstat = "Below 165, firing"
        [dmgr.get(t).setLEDs(2) for t in thermostats]
        dmgr.get('woodcirc').set(0)
        dmgr.get('wooddraft').set(1)

    elif boilertemp < 185:
        # From 165-186 we are demand driven. At the moment, "demand" is the
        # "front room thermostat" until we add more logic.
        [dmgr.get(t).setLEDs(1) for t in thermostats]

        if (demand):
            dmgr.globalstat = "Below 185, demand, firing/circulating"
            dmgr.get('woodcirc').set(1)
            dmgr.get('wooddraft').set(1)
        else:
            dmgr.globalstat = "Below 185, no demand, standing by"
            dmgr.get('woodcirc').set(0)
            dmgr.get('wooddraft').set(0)

    else:
        dmgr.globalstat = "Above 185, overheating, dumping heat"
        [dmgr.get(t).setLEDs(4) for t in thermostats]
        dmgr.get('woodcirc').set(1)
        dmgr.get('wooddraft').set(0)

    time.sleep(10)
