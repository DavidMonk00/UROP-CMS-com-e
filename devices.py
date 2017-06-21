import Property as P

DS3232 = {'seconds':P.Property(0x00,1,'rw','s'),
          'minutes':P.Property(0x01,1,'rw','m'),
          'hours':P.Property(0x02,1,'rw','h')}


devices = {"DS3232":DS3232}
