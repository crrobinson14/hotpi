import requests
import json
import os.path
import time
from files import *


class semafile():
    config = {}
    state = 0
    created = None

    def __init__(self, config):
        self.config = config
        pass

    def __str__(self):

        return '%s: %s (%s)' % (self.config['name'],
                                'Enabled' if (self.state == 1) else 'Disabled',
                                time.ctime(self.created))

    def setState(self, state):
        if state == self.state:
            return

        self.state = state

        try:
            writeFileValue(self.config['filename'], state)
        except:
            pass

    def poll(self):
        try:
            value = readFileValue(self.config['filename'])
            self.state = int(value)

            self.created = s.path.getctime(self.config['filename'])

        except:
            pass
