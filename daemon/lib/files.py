import requests
import json


def readFileValue(filename):
    f = open(filename)
    outval = f.read()
    f.close()

    return outval


def writeFileValue(filename, value):
    f = open(filename, 'w')
    outval = f.write(str(value))
    f.close()
