from libI2C import *

def main():
    I = I2CRaw()
    for i in range(0,0xFF,2):
        x = I.read(i,0x00)
        if (x != 0xF0):
            print hex(i), hex(int(x))
            

if (__name__ == '__main__'):
    main()
