def isNumber(x):
    try:
        int(x)
        return True
    except ValueError:
        try:
            int(x[2:])
            return True
        except ValueError:
            return False

def isHex(x):
    try:
        int(x, 16)
        return True
    except ValueError:
        return False

def findHex(line):
    index = line.index("0x");
    return "0x"+ str(int(line[index:index+4], 16))

class Bit:
    def __init__(self, bit, name, rw, description, default):
        self.bit = bit
        self.name = name
        self.description = description
        self.default = default

class Address:
    bits = []
    def __init__(self, offset, bits, description):
        self.offset = offset
        self.bits = bits
        self.description = description
    def getBits(self, lines):
        self.bits = []
        self.tables = [i for i in lines if (self.description in i and "Table" in i)]
        for i in self.tables:
            index = lines.index(i)
            index +=9
            while (isNumber(lines[index+2][:2])):
                bit = lines[index+2]
                name = lines[index+4]
                rw = lines[index+6]
                desc = ""
                index+=8
                print bit
                if (isNumber(lines[index])):
                    default = lines[index]
                    index+=1
                    while (lines[index] != ""):
                        desc += (" " + lines[index])
                        index+=1
                else:
                    while (lines[index] != ""):
                        desc += (" " + lines[index])
                        index+=1
                    default = lines[index+1] if isHex(lines[index+1]) else findHex(desc)
                self.bits.append(Bit(bit,name,rw,desc,default))
                index+=1


class Page:
    addresses = []
    def __init__(self, number, description):
        self.number = number
        self.description = description
    def getAddresses(self, lines):
        self.addresses = []
        self.tables = [i for i in lines if ("Page "+self.number in i and "Table" in i)]
        for i in self.tables:
            index = lines.index(i)+5
            while (lines[index+2][:2] == "0x"):
                address = lines[index+2]
                bits = lines[index+4]
                desc = lines[index+6][1:]
                desc = desc[:desc.index("'")]
                self.addresses.append(Address(address, bits, desc))
                index +=6
