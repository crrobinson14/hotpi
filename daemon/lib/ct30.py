import requests
import json
from files import *


class ct30():
    config = {}
    message = ''
    ledstate = -1
    status = {'temp': 73.0, 'fstate': 0, 'tmode': 2, 'fmode': 0, 'hold': 0,
              'time': {'minute': 38, 'day': 5, 'hour': 13}, 'override': 0,
              'message': '', 'tstate': 0, 't_cool': 85.0, 't_type_post': 0,
              'ledstate': 0, 'set': 0}

    def __init__(self, config):
        self.config = config
        pass

    def __str__(self):
        # {'name': 'Front Room Tstat', u'temp': 73.0, 'ledstate': -1,
        # u'fstate': 0, u'tmode': 2, u'fmode': 0, u'override': 0, 'message': '',
        # u'time': {u'minute': 38, u'day': 5, u'hour': 13},
        # u'hold': 0, u'tstate': 0, u't_cool': 85.0, u't_type_post': 0}
        fmt = '%s: %f (set %f) fstate %d tmode %d fmode %d tstate %d'
        return fmt % (self.config['name'],
                      self.status['temp'],
                      self.status['set'],
                      self.status['fstate'],
                      self.status['tmode'],
                      self.status['fmode'],
                      self.status['tstate'])

    def setLEDs(self, state):
        # The CT-30 doesn't like it if we write too much, so only do the change
        # if we actually need to.
        if state == self.ledstate:
            return

        self.ledstate = state

        url = "http://%s/tstat/led" % self.config['location']
        data = {'energy_led': int(state)}
        headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}

        r = requests.post(url, data=json.dumps(data), headers=headers)

        response = json.loads(r.text)
        return response['success'] == 0

    def setMessage(self, message):
        # The CT-30 doesn't like it if we write too much, so only do the change
        # if we actually need to.
        if message == self.message:
            return

	try:
	        self.message = message

	        url = "http://%s/tstat/pma" % self.config['location']
        	data = {'message': str(message)}
	        headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}

        	r = requests.post(url, data=json.dumps(data), headers=headers)

	        response = json.loads(r.text)
        	return response['success'] == 0
	except:
		pass

    def poll(self):
	try:
	        url = "http://%s/tstat" % self.config['location']
	        r = requests.get(url)

	        self.status = json.loads(r.text)

	        self.status['ledstate'] = self.ledstate
	        self.status['message'] = self.message

	        if self.status['tmode'] == 2:
      			self.status['set'] = self.status['t_cool']
        	elif self.status['tmode'] == 1:
            		self.status['set'] = self.status['t_heat']
        	else:
            		self.status['set'] = 0
	except:
		pass

