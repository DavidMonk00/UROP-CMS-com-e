#include "I2CRegister.h"

PCIClockPLLModeRegister::PCIClockPLLModeRegister(uint32_t addr, std::string rw) {
   reg = addr;
   const char* rw_char = rw.c_str();
   const char* end = rw_char + sizeof(rw_char)/sizeof(rw_char[0]);
   const char* position = std::find(rw_char, end, 'r');
   b_read = position != end;
   position = std::find(rw_char, end, 'w');
   b_write = position != end;
}

PCIClockPLLModeRegister::~PCIClockPLLModeRegister(void) {}

units_variant PCIClockPLLModeRegister::read(I2C_base* i2c_ptr, uint32_t address) {
   if (b_read) {
      uint8_t buffer = 0;
      i2c_ptr->receiveData(address, (char*)&buffer, 1, reg);
      uint8_t modes = 0b00000110&buffer;
      units_variant var;

      switch (modes) {
         case 0b00000000:
            var = 0.0;
            break;
         case 0b00000010:
            var = 1.0;
            break;
         case 0b00000110:
            var = 2.0;
            break;
      }
      return var;
   } else {
      printf("Register cannot be read.\n");
      exit(-1);
   }
}

void PCIClockPLLModeRegister::write(I2C_base* i2c_ptr, uint32_t address, units_variant value) {
   std::unordered_map<double, uint8_t> modes = {{2, 0xE},
                                                {1, 0xA},
                                                {0, 0x8}};
   if (b_write) {
      double s = boost::get<double>(value);
      uint8_t buffer = modes[s];
      i2c_ptr->sendData(address, (char*)&buffer, 1, reg);
   } else {
      printf("Register cannot be written to.\n");
      exit(-1);
   }
}


PCIClockOutputEnableRegister::PCIClockOutputEnableRegister(uint32_t addr, std::string rw) {
   reg = addr;
   const char* rw_char = rw.c_str();
   const char* end = rw_char + sizeof(rw_char)/sizeof(rw_char[0]);
   const char* position = std::find(rw_char, end, 'r');
   b_read = position != end;
   position = std::find(rw_char, end, 'w');
   b_write = position != end;
}

PCIClockOutputEnableRegister::~PCIClockOutputEnableRegister(void) {}

units_variant PCIClockOutputEnableRegister::read(I2C_base* i2c_ptr, uint32_t address) {
   if (b_read) {
      uint8_t* buffer = (uint8_t*)malloc(2*sizeof(uint8_t));
      i2c_ptr->receiveData(address, (char*)buffer, 2, reg);
      uint8_t out = (0b11&(buffer[0]>>1))|(0b1100&(buffer[0]>>3))|((0b1&buffer[1])<<4)|((0b1&(buffer[1]>>2))<<5);
      units_variant var = out;
      return var;
   } else {
      printf("Register cannot be read.\n");
      exit(-1);
   }
}

void PCIClockOutputEnableRegister::write(I2C_base* i2c_ptr, uint32_t address, units_variant value) {
   if (b_write) {
      uint8_t out = (uint8_t)boost::get<double>(value);
      uint8_t* buffer = (uint8_t*)malloc(2*sizeof(uint8_t));
      buffer[0] = ((0b0011&out)<<1)|((0b1100&out)<<3);
      buffer[1] = (0b1&(out>>4))|(0b100&(out>>3));
      i2c_ptr->sendData(address, (char*)buffer, 2, reg);
   } else {
      printf("Register cannot be written to.\n");
      exit(-1);
   }
}
