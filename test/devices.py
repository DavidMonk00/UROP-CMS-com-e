import Property as P

class Device:
    def __init__(self, address, properties):
        self.properties = properties
        self.address = address

DS3232 = {'seconds':P.Property(0x00,1,'rw','s'),
          'minutes':P.Property(0x01,1,'rw','m'),
          'hours':P.Property(0x02,1,'rw','h')}

devices = {"DS3232":Device(0b11010000, DS3232)}
