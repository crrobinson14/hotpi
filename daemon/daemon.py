import time
from config import hotpiConfig
from lib.devices import deviceManager
from lib.files import *

dmgr = deviceManager()
thermostats = ['tstatdown', 'tstatbed', 'tstatfront']
zones = ['zonedown', 'zonebed', 'zonefront']

while True:
    dmgr.poll()

    tstatbed = hotpiConfig['devices']['tstatbed']['handler']
    tstatdown = hotpiConfig['devices']['tstatdown']['handler']
    tstatfront = hotpiConfig['devices']['tstatfront']['handler']
    tstatbed = hotpiConfig['devices']['tstatbed']['handler']
    woodcirc = hotpiConfig['devices']['woodcirc']['handler']
    wooddraft = hotpiConfig['devices']['wooddraft']['handler']

    zonedown = hotpiConfig['devices']['zonedown']['handler']
    zonefront = hotpiConfig['devices']['zonefront']['handler']
    zonebed = hotpiConfig['devices']['zonebed']['handler']

    coldstart = hotpiConfig['devices']['coldstart']['handler']
    quietmode = hotpiConfig['devices']['quietmode']['handler']

    # dmgr.get('wooddraft').set(0)

    boilertemp = int(dmgr.get('boilertemp').status['temperature'])
    returntemp = int(dmgr.get('returntemp').status['temperature'])
    [dmgr.get(t).setMessage(boilertemp) for t in thermostats]

    # Force all the zones open for now
    [dmgr.get(z).set(1) for z in zones]

    demand = (tstatfront.status['temp'] < 71)

    USEDRAFT = 1 if quietmode.state is 0 else 0

    if boilertemp < 125:
        # Below 125 we assume the boiler is off
        if coldstart.state is 1:
            dmgr.globalstat = "Below 125 (COLD START)"
            [dmgr.get(t).setLEDs(0) for t in thermostats]
            dmgr.get('woodcirc').set(0)
            dmgr.get('wooddraft').set(USEDRAFT)
        else:
            dmgr.globalstat = "Below 125 (Offline)"
            [dmgr.get(t).setLEDs(0) for t in thermostats]
            dmgr.get('woodcirc').set(0)
            dmgr.get('wooddraft').set(0)

    elif boilertemp < 145:
        if coldstart.state is 1:
            dmgr.globalstat = "Below 145 (COLD START)"
            [dmgr.get(t).setLEDs(0) for t in thermostats]
            dmgr.get('woodcirc').set(0)
            dmgr.get('wooddraft').set(USEDRAFT)
        else:
            # Below 145 we try to preserve coals
            dmgr.globalstat = "Below 145 (Preserving Coals)"
            [dmgr.get(t).setLEDs(2) for t in thermostats]
            dmgr.get('woodcirc').set(0)
            dmgr.get('wooddraft').set(0)

    elif boilertemp < 165:
        # Below 165 we get the fire going
        dmgr.globalstat = "Below 165 (Firing)"
        coldstart.setState(0)
        [dmgr.get(t).setLEDs(2) for t in thermostats]
        dmgr.get('woodcirc').set(0)
        dmgr.get('wooddraft').set(USEDRAFT)

    elif boilertemp < 185:
        # From 165-186 we are demand driven. At the moment, "demand" is the
        # "front room thermostat" until we add more logic.
        [dmgr.get(t).setLEDs(1) for t in thermostats]
        coldstart.setState(0)

        if (demand):
            dmgr.globalstat = "Demand (Firing/Circulating)"
            dmgr.get('woodcirc').set(1)
            dmgr.get('wooddraft').set(USEDRAFT)
        else:
            dmgr.globalstat = "No Demand (Standby)"
            dmgr.get('woodcirc').set(0)
            dmgr.get('wooddraft').set(0)

    elif boilertemp < 195:
        dmgr.globalstat = "High Limit, Circulating"
        [dmgr.get(t).setLEDs(1) for t in thermostats]
        dmgr.get('woodcirc').set(1)
        dmgr.get('wooddraft').set(0)

    else:
        dmgr.globalstat = "Overheat (Dumping)"
        [dmgr.get(t).setLEDs(4) for t in thermostats]
        dmgr.get('woodcirc').set(1)
        dmgr.get('wooddraft').set(0)

    status = "upstairs,%.2f\n\
downstairs,%.2f\n\
frontroom,%.2f\n\
boiler,%.2f\n\
return,%.2f\n\
mode,%s\n\
devices,Circ: %s Draft: %s\n" % (
        tstatbed.status['temp'],
        tstatdown.status['temp'],
        tstatfront.status['temp'],
        dmgr.get('boilertemp').status['temperature'],
        dmgr.get('returntemp').status['temperature'],
        dmgr.globalstat,
        'ON' if dmgr.get('woodcirc').status['output'] is 1 else 'OFF',
        'ON' if dmgr.get('wooddraft').status['output'] is 1 else 'OFF')

    writeFileValue('/var/run/hotpi/status', status)

    time.sleep(10)
