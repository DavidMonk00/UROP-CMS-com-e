CC = g++
CFLAGS = -lsemaeapi -L/opt/Sema/lib -I/opt/Sema/include
PROG = SEMA

OBJS = SEMA.o diagnostics.o I2CSema.o I2C.o

SRCS = SEMA.cpp diagnostics.cpp I2CSema.cpp I2C.cpp

SEMA: $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(CFLAGS)

.cpp.o:
	$(CC) -c $*.cpp $(CFLAGS)

clean:
	rm -f $(OBJS) $(PROG)

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)
# DO NOT DELETE

SEMA.o: I2CSema.h /usr/include/stdio.h /usr/include/features.h
SEMA.o: /usr/include/stdc-predef.h /usr/include/libio.h
SEMA.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/string.h
SEMA.o: /usr/include/xlocale.h /opt/Sema/include/Sema.h
SEMA.o: /opt/Sema/include/linux/EApiOs.h /usr/include/stdlib.h
SEMA.o: /usr/include/alloca.h /usr/include/assert.h /usr/include/inttypes.h
SEMA.o: /usr/include/stdint.h /usr/include/unistd.h /usr/include/getopt.h
SEMA.o: /usr/include/time.h /usr/include/ctype.h /usr/include/endian.h
SEMA.o: /opt/Sema/include/EApi.h /opt/Sema/include/EApiCOM0.h
SEMA.o: /opt/Sema/include/EApiTCA.h /opt/Sema/include/EApiETX.h
SEMA.o: /opt/Sema/include/EApiQ7.h /opt/Sema/include/EApiMOPS.h
SEMA.o: /opt/Sema/include/EApiPISA.h /opt/Sema/include/EApiEPIC.h
SEMA.o: /opt/Sema/include/semaeapi.h /opt/Sema/include/adlink/SEMA_define.h
SEMA.o: diagnostics.h
diagnostics.o: diagnostics.h /usr/include/stdio.h /usr/include/features.h
diagnostics.o: /usr/include/stdc-predef.h /usr/include/libio.h
diagnostics.o: /usr/include/_G_config.h /usr/include/wchar.h
diagnostics.o: /usr/include/string.h /usr/include/xlocale.h
diagnostics.o: /opt/Sema/include/Sema.h /opt/Sema/include/linux/EApiOs.h
diagnostics.o: /usr/include/stdlib.h /usr/include/alloca.h
diagnostics.o: /usr/include/assert.h /usr/include/inttypes.h
diagnostics.o: /usr/include/stdint.h /usr/include/unistd.h
diagnostics.o: /usr/include/getopt.h /usr/include/time.h /usr/include/ctype.h
diagnostics.o: /usr/include/endian.h /opt/Sema/include/EApi.h
diagnostics.o: /opt/Sema/include/EApiCOM0.h /opt/Sema/include/EApiTCA.h
diagnostics.o: /opt/Sema/include/EApiETX.h /opt/Sema/include/EApiQ7.h
diagnostics.o: /opt/Sema/include/EApiMOPS.h /opt/Sema/include/EApiPISA.h
diagnostics.o: /opt/Sema/include/EApiEPIC.h /opt/Sema/include/semaeapi.h
diagnostics.o: /opt/Sema/include/adlink/SEMA_define.h
I2CSema.o: I2CSema.h /usr/include/stdio.h /usr/include/features.h
I2CSema.o: /usr/include/stdc-predef.h /usr/include/libio.h
I2CSema.o: /usr/include/_G_config.h /usr/include/wchar.h
I2CSema.o: /usr/include/string.h /usr/include/xlocale.h
I2CSema.o: /opt/Sema/include/Sema.h /opt/Sema/include/linux/EApiOs.h
I2CSema.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/assert.h
I2CSema.o: /usr/include/inttypes.h /usr/include/stdint.h
I2CSema.o: /usr/include/unistd.h /usr/include/getopt.h /usr/include/time.h
I2CSema.o: /usr/include/ctype.h /usr/include/endian.h
I2CSema.o: /opt/Sema/include/EApi.h /opt/Sema/include/EApiCOM0.h
I2CSema.o: /opt/Sema/include/EApiTCA.h /opt/Sema/include/EApiETX.h
I2CSema.o: /opt/Sema/include/EApiQ7.h /opt/Sema/include/EApiMOPS.h
I2CSema.o: /opt/Sema/include/EApiPISA.h /opt/Sema/include/EApiEPIC.h
I2CSema.o: /opt/Sema/include/semaeapi.h
I2CSema.o: /opt/Sema/include/adlink/SEMA_define.h diagnostics.h
I2C.o: I2C.h /usr/include/stdio.h /usr/include/features.h
I2C.o: /usr/include/stdc-predef.h /usr/include/libio.h
I2C.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/string.h
I2C.o: /usr/include/xlocale.h /opt/Sema/include/Sema.h
I2C.o: /opt/Sema/include/linux/EApiOs.h /usr/include/stdlib.h
I2C.o: /usr/include/alloca.h /usr/include/assert.h /usr/include/inttypes.h
I2C.o: /usr/include/stdint.h /usr/include/unistd.h /usr/include/getopt.h
I2C.o: /usr/include/time.h /usr/include/ctype.h /usr/include/endian.h
I2C.o: /opt/Sema/include/EApi.h /opt/Sema/include/EApiCOM0.h
I2C.o: /opt/Sema/include/EApiTCA.h /opt/Sema/include/EApiETX.h
I2C.o: /opt/Sema/include/EApiQ7.h /opt/Sema/include/EApiMOPS.h
I2C.o: /opt/Sema/include/EApiPISA.h /opt/Sema/include/EApiEPIC.h
I2C.o: /opt/Sema/include/semaeapi.h /opt/Sema/include/adlink/SEMA_define.h
I2C.o: diagnostics.h
