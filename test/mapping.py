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
    def printDeviceInformation(self):
        print "Device address: %s"%bin(self.device.address)
    def printProperties(self):
        print "Available properties:"
        for i in self.device.properties.keys():
            print "'%s'"%i
    def read(self, property):
        p = self.device.properties[property]
        if ('r' in p.rw):
            return p.unit
        else:
            raise Exception('Property cannot be read')

def main():
    M = Measurement("DS3232")
    M.printDeviceInformation()
    M.printProperties()
    print M.read("seconds")


if (__name__ == '__main__'):
    main()
