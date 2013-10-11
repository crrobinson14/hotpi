hotpiConfig = {
    'devices': {
        'boilertemp': {
            'name': 'Boiler Temp',
            'type': 'ds18b20',
            'location': '/mnt/1wire/uncached/28.DF9F3A040000',
        },
        'returntemp': {
            'name': 'Return Temp',
            'type': 'ds18b20',
            'location': '/mnt/1wire/uncached/28.71B93A040000',
        },
        'woodcirc': {
            'name': 'Wood Circ',
            'type': 'relay',
            'location': '/mnt/1wire/uncached/3A.4C8607000000',
        },
        'wooddraft': {
            'name': 'Wood Draft',
            'type': 'relay',
            'location': '/mnt/1wire/uncached/3A.518607000000',
        },
        'tstatdown': {
            'name': 'Downstairs Tstat',
            'type': 'ct30',
            'location': '192.168.1.61',
        },
        'tstatfront': {
            'name': 'Front Room Tstat',
            'type': 'ct30',
            'location': '192.168.1.62',
        },
        'tstatbed': {
            'name': 'Upstairs Tstat',
            'type': 'ct30',
            'location': '192.168.1.63',
        },
        #'zonedown': {
        #    'name': 'Downstairs Zone',
        #    'type': 'relay',
        #    'location': '/dev/null',
        #},
        #'zonefront': {
        #    'name': 'Front Room Zone',
        #    'type': 'relay',
        #    'location': '/dev/null',
        #},
        #'zoneup': {
        #    'name': 'Upstairs Zone',
        #    'type': 'relay',
        #    'location': '/dev/null',
        #},
        #'tankfill': {
        #    'name': 'Fill Tank',
        #    'type': 'relay',
        #    'location': '/dev/null',
        #},
        #'tankdrain': {
        #    'name': 'Drain Tank',
        #    'type': 'relay',
        #    'location': '/dev/null',
        #},
    },
}
