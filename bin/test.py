from libI2C import *

def main():
    string = "1.33e+08 Hz"
    string_list = string.split(" ")
    index = string_list[0].find("e")
    if (index != -1):
        exponent = string_list[0][index+1:]
        if (exponent[0] == "+"):
            exponent = exponent[1:]
        value = string_list[0][:index]
        print float(value), int(exponent)/3

if (__name__ == '__main__'):
    main()
