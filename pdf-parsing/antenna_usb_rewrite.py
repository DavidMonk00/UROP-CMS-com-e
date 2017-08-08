import usb
import time
import threading
import logging
from datetime import timedelta, datetime
import sys
import json

class Cp2130():
    def __init__(self):
        self.dev = None  # attribute for storing usb device pointer
        logging.basicConfig(filename='Cp2130.log', filemode='w', level=logging.DEBUG)  # configuration of logging functionality, overwritting mode

    def find(self):
        self.dev = usb.core.find(idVendor=0x10c4, idProduct=0x87a0)  # both IDs are the same for brand new chips, can be modified in OTP
        if self.dev:
            logging.debug("Segmented antenna has been found in the system!")
            logging.debug(self.dev)
            return 0
        else:
            logging.warning("Could not find segmented antenna in the system, no results "
                            "for {idVendor=0x10c4, idProduct=0x87a0}")
            return -1

    def getGPIOChipSelect(self):
        r"""
        returns 4 bytes, 2 first bytes corresponding to channel CS enable, 2 last bytes correspond to pin CS enable
        Channel CS enable bitmap:
        ----------------------------------------------------------------
        |offset| bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 |
        |   0  |              reserved            | ch10 |  ch9 |  ch8 |
        |   1  |  ch7 |  ch6 |  ch5 |  ch4 |  ch3 |  ch2 |  ch1 |  ch0 |
        ----------------------------------------------------------------
        """
        return self.dev.ctrl_transfer(0xC0, 0x24, 0, 0, 0x04)

    def getSPIDelay(self, channel):
        return self.dev.ctrl_transfer(0xC0, 0x32, 0, channel, 0x08)

    def setSPIDelay(self, channel, mask_bitmap, inter_byte_delay, post_assert_delay, pre_deassert_delay):
        data_string = chr(channel) + chr(mask_bitmap) + chr(inter_byte_delay / 256) + chr(inter_byte_delay % 256) + chr(post_assert_delay / 256) + chr(post_assert_delay % 256) + chr(pre_deassert_delay / 256) + chr(pre_deassert_delay % 256)
        self.dev.ctrl_transfer(0x40, 0x33, 0, 0, data_string)

    def getSPIWord(self):
        return self.dev.ctrl_transfer(0xC0, 0x30, 0, 0, 0x0B)

    def pinSetHigh(self, chip_number):
        data_string = chr(chip_number) + '\x02\x00'
        self.dev.ctrl_transfer(0x40, 0x23, 0, 0, data_string)

    def pinResetLow(self, chip_number):
        data_string = chr(chip_number) + '\x02\x01'
        self.dev.ctrl_transfer(0x40, 0x23, 0, 0, data_string)

    def setGPIOChipSelect(self, chip_number):
        # selects given channel (slave device) for an upcoming SPI transmission
        data_string = chr(chip_number) + '\x02'
        # protocol defined message, see cp2130 interface specification AN792 page 27
        self.dev.ctrl_transfer(0x40, 0x25, 0, 0, data_string)

    def setSPIWord(self, channel, channel_word):
        # changes SPI configuration for a given channel (clock frequency, polarity, phase, etc.)
        data_string = chr(channel) + channel_word
        # protocol defined message, see cp2130 interface specification AN792 page 31
        self.dev.ctrl_transfer(0x40, 0x31, 0, 0, data_string)

    def writeSPI(self, length, string_data_buffer, endpoint=0x01, command_id='\x01', reserved1='\x00\x00', reserved2='\x00', timeout=500):
        string_buffer = reserved1 + command_id + reserved2 + chr(length % 256) + chr(length / 256) + reserved1 + string_data_buffer
        # protocol defined message, see cp2130 interface specification AN792 page 8
        return self.dev.write(endpoint, string_buffer, timeout=timeout)

    def readSPI(self, length, endpoint=0x01, command_id='\x00', reserved1='\x00\x00', reserved2='\x00', timeout=500):
        string_buffer = reserved1 + command_id + reserved2 + chr(length % 256) + chr(length / 256) + reserved1
        # protocol defined message, see cp2130 interface specification AN792 page 7
        return self.dev.write(endpoint, string_buffer, timeout=timeout)

    def writeReadSPI(self, length, string_data_buffer, endpoint=0x01, command_id='\x02', reserved1='\x00\x00', reserved2='\x00', timeout=500):
        string_buffer = reserved1 + command_id + reserved2 + chr(length % 256) + chr(length / 256) + reserved1 + string_data_buffer
        # protocol defined message, see cp2130 interface specification AN792 page 7
        return self.dev.write(endpoint, string_buffer, timeout=timeout)

    def readUSB(self, size, endpoint=0x82, timeout=500):
        return self.dev.read(endpoint, size, interface=0, timeout=timeout)



    def configureSPIChannel(self, analog_switch_cs):
        if analog_switch_cs == 0 or analog_switch_cs == 1:
            self.setGPIOChipSelect(analog_switch_cs)

            time.sleep(0.05)
            logging.debug("GPIO chip select: ")
            logging.debug(self.getGPIOChipSelect())

            time.sleep(0.05)
            self.setSPIWord(analog_switch_cs, '\x3B')  # changing SPI freq (last 3 bits: '000' highest '111' lowest)

            time.sleep(0.05)
            logging.debug("SPI channels settings: ")
            logging.debug(self.getSPIWord())

            time.sleep(0.05)
            self.setSPIDelay(analog_switch_cs, 2, 0, 1, 0)
            logging.debug("delays for channel " + str(analog_switch_cs) + ": ")
            logging.debug(self.getSPIDelay(analog_switch_cs))
            time.sleep(0.05)
        else:
            logging.warning("Unexpected argument: analog_switch_cs in configureSPIChannel method!")


    def readBCM53134M(self, page, offset, length):
        #print "Step 1) Poll SPI Staus Reg (0xFE) with BCM53134M Normal Read Command (OpCode = 0x60)"
        usb_to_spi.writeReadSPI(3, '\x60\xFE\x00')
        wait_until = datetime.now() + timedelta(milliseconds=100)
        try:
            while 1:
                ret = usb_to_spi.readUSB(3)
                # Bit 7 = SPIF - SPI R/W complete Flag
                if ret[2] & 0x80 == 0:
                    break
                elif (datetime.now() > wait_until):
                    sys.exit("Timeout waiting for Ethernet switch BCM53134M")
        except:
            pass

        #print "Step 2) Write page into SPI Page Reg (0xFF) with BCM53134M Normal Write Command (OpCode = 0x61)"
        cmd = '\x61\xFF' + chr(page)
        usb_to_spi.writeSPI(3, cmd)

        #print "Step 3) Setup offset with BCM53134M Normal Read Command (OpCode = 0x60)"
        cmd = '\x60' + chr(offset) + '\x00'
        usb_to_spi.writeSPI(3, cmd)

        #print "Step 4) Poll SPI Status Reg (0xFE) with BCM53134M Normal Read Command (OpCode = 0x60)"
        wait_until = datetime.now() + timedelta(milliseconds=100)
        try:
            while 1:
                usb_to_spi.writeReadSPI(3, '\x60\xFE\x00')
                ret = usb_to_spi.readUSB(3)
                # Bit 5 = RACK Flag
                if ret[2] & 0x20 != 0:
                    break
                elif (datetime.now() > wait_until):
                    print "Timeout waiting for Ethernet switch BCM53134M"
                    sys.exit()
        except:
            pass

        #print "Step 5) Access Data Reg (0xF0) with Normal Read Command (OpCode = 0x60)"
        str_empty = '\x00' * length
        usb_to_spi.writeReadSPI(2 + length, '\x60\xF0' + str_empty)
        # Clock out first data byte on MISO line: 0 0 0 0 0 0 0 0 (byte 0 = bit 7 to bit 0 = MSB to LSB).
        # Clock out next byte (in this case, last) on MISO line: 0 0 0 0 1 1 1 0 (byte 1 = bit 15 to bit 8).
        byte_array = usb_to_spi.readUSB(2 + length)
        #print byte_array
        reg = 0
        for i, e in enumerate(byte_array[2::1]):
            #print "i = ", i, " e = ", e
            reg = reg | (e << (8*i))
        return reg


    def writeBCM53134M(self, page, offset, string_buffer):
        #print "Step 1) Poll SPI Staus Reg (0xFE) with BCM53134M Normal Read Command (OpCode = 0x60)"
        usb_to_spi.writeReadSPI(3, '\x60\xFE\x00')
        wait_until = datetime.now() + timedelta(milliseconds=100)
        try:
            while 1:
                ret = usb_to_spi.readUSB(3)
                # Bit 7 = SPIF - SPI R/W complete Flag
                if ret[2] & 0x80 == 0:
                    break
                elif (datetime.now() > wait_until):
                    sys.exit("Timeout waiting for Ethernet switch BCM53134M")
        except:
            pass

        #print "Step 2) Write page into SPI Page Reg (0xFF) with BCM53134M Normal Write Command (OpCode = 0x61)"
        cmd = '\x61\xFF' + chr(page)
        usb_to_spi.writeSPI(3, cmd)

        #print "Step 3) Setup offset with BCM53134M Normal Write Command (OpCode = 0x61)"
        cmd = '\x61' + chr(offset) + string_buffer
        usb_to_spi.writeSPI(2 + len(string_buffer), cmd)



if __name__ == "__main__":
    usb_to_spi = Cp2130()
    if usb_to_spi.find() == 0:
        print "Start with SPI Config"
        usb_to_spi.configureSPIChannel(0)
    with open('results.json') as f_json:
        book = json.load(f_json)
    for page in book:
        print "Page = %s"%page
        for address in book[page]['addresses']:
            reg = usb_to_spi.readBCM53134M(page=int(page), offset=int(book[page]['addresses'][address]['offset']), length=int(book[page]['addresses'][address]['word-length']))
            print "%s = "%address, reg
