'''
    mapping.py
    Purpose: test the concept of mapping properties and devices in python before
    translating to C++
    @author David Monk
'''

import devices as d

class Measurement:
    def __init__(self, device):
        self.device = d.devices[device]
    def printProperties(self):
        for i in self.device.keys():
            print i
    def read(self, property):
        p = self.device[property]
        if ('r' in p.rw):
            return p.unit

def main():
    M = Measurement("DS3232")
    #M.printProperties()
    print M.read("seconds")


if (__name__ == '__main__'):
    main()
