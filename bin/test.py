from libI2C import *

def main():
    I = I2CRaw()
    print "Retimer 1"
    for i in range(0x100):
        x = int(I.read(0x4e, i))
        if (x):
            print hex(i),hex(x)
    print "Retimer 2"
    for i in range(0x100):
        x = int(I.read(0x30, i))
        if (x):
            print hex(i),hex(x)

if (__name__ == '__main__'):
    main()
