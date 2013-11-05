import requests
from files import *


class ds18b20():
    config = {}
    status = {'temperature': 0.0}

    def __init__(self, config):
        self.config = config
        pass

    def __str__(self):
        return '%s: %f' % (self.config['name'],
                           self.status['temperature'])

    def poll(self):
        filename = readFileValue("%s/temperature" % self.config['location'])
        self.status = {
            'temperature': float(filename)
        }
