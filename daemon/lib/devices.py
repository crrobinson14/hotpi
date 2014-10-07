import os
from config import hotpiConfig
from ds18b20 import ds18b20
from ct30 import ct30
from relay import relay
from files import *


class deviceManager():
    def __init__(self):
        for key in hotpiConfig['devices'].keys():
            device = hotpiConfig['devices'][key]

            if device['type'] == 'ds18b20':
                device['handler'] = ds18b20(device)

            elif device['type'] == 'ct30':
                device['handler'] = ct30(device)

            elif device['type'] == 'relay':
                device['handler'] = relay(device)

    def poll(self):
        print "Polling..."
        status = ""
        for key in hotpiConfig['devices'].keys():
            device = hotpiConfig['devices'][key]
            device['handler'].poll()

            d = str(device['handler'])
            status = status + d + os.linesep

        writeFileValue('/tmp/status.txt', status)
        print status

    def get(self, device):
        return hotpiConfig['devices'][device]['handler']
