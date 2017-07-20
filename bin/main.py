from libI2C import *

def main():
    A = ATCABoard("SEMA")
    buses = [str(i) for i in A.getBuses()]
    A.setBus(buses[0])
    devices = [str(i) for i in A.getDevices()]
    A.setDevice(devices[0])
    properties = [str(i) for i in A.getProperties()]
    for i in properties:
        print i, A.read(i)


if (__name__ == '__main__'):
    main()
