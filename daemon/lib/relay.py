import requests
from files import *


class relay():
    config = {}
    status = {'output': 0, 'input': 0}

    def __init__(self, config):
        self.config = config
        self.poll()

    def __str__(self):
        return '%s: %s,%s' % (self.config['name'],
                              'ON' if self.status['output'] == 1 else 'OFF',
                              'HIGH' if self.status['input'] == 1 else 'LOW')

    def set(self, state):
        writeFileValue("%s/PIO.B" % self.config['location'], state)

    def poll(self):
        self.status = {
            'output': int(readFileValue("%s/PIO.B" % self.config['location'])),
            'input': int(readFileValue("%s/sensed.A" % self.config['location'])),
        }
