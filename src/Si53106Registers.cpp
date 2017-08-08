/**
  @file Si53106Registers.cpp
  @brief Defines functions for the derived classes specific to the Si53106 component.
  @author David Monk - Imperial College London
  @version 1.0
*/

#include "I2CRegister.hpp"

/**
  @brief Class constructor.
  @param r - Address of register within I2C device.
*/
PCIClockPLLModeRegister::PCIClockPLLModeRegister(uint32_t addr, std::string rw) {
   reg = addr;
   const char* rw_char = rw.c_str();
   const char* end = rw_char + sizeof(rw_char)/sizeof(rw_char[0]);
   const char* position = std::find(rw_char, end, 'r');
   b_read = position != end;
   position = std::find(rw_char, end, 'w');
   b_write = position != end;
}

/**
   @brief Class destructor.
*/
PCIClockPLLModeRegister::~PCIClockPLLModeRegister(void) {}

/**
  @brief Read data from register.
  @param i2c_ptr - Pointer to I2C_base class used for transport.
  @return units_variant containing quantity of correct type.
*/
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

/**
  @brief Write data to register.
  @param i2c_ptr - pointer to I2C_base class used for transport
  @param value - Data to be written. Must be of correct type.
*/
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

/**
  @brief Class constructor.
  @param r - Address of register within I2C device.
*/
PCIClockOutputEnableRegister::PCIClockOutputEnableRegister(uint32_t addr, std::string rw) {
   reg = addr;
   const char* rw_char = rw.c_str();
   const char* end = rw_char + sizeof(rw_char)/sizeof(rw_char[0]);
   const char* position = std::find(rw_char, end, 'r');
   b_read = position != end;
   position = std::find(rw_char, end, 'w');
   b_write = position != end;
}

/**
   @brief Class destructor.
*/
PCIClockOutputEnableRegister::~PCIClockOutputEnableRegister(void) {}

/**
  @brief Read data from register.
  @param i2c_ptr - Pointer to I2C_base class used for transport.
  @return units_variant containing quantity of correct type.
*/
units_variant PCIClockOutputEnableRegister::read(I2C_base* i2c_ptr, uint32_t address) {
   if (b_read) {
      uint8_t* buffer = (uint8_t*)malloc(2*sizeof(uint8_t));
      i2c_ptr->receiveData(address, (char*)buffer, 2, reg);
      uint8_t out = (0b11&(buffer[0]>>1))|(0b1100&(buffer[0]>>3))|((0b1&buffer[1])<<4)|((0b1&(buffer[1]>>2))<<5);
      free(buffer);
      units_variant var = out;
      return var;
   } else {
      printf("Register cannot be read.\n");
      exit(-1);
   }
}

/**
  @brief Write data to register.
  @param i2c_ptr - pointer to I2C_base class used for transport
  @param value - Data to be written. Must be of correct type.
*/
void PCIClockOutputEnableRegister::write(I2C_base* i2c_ptr, uint32_t address, units_variant value) {
   if (b_write) {
      uint8_t out = (uint8_t)boost::get<double>(value);
      uint8_t* buffer = (uint8_t*)malloc(2*sizeof(uint8_t));
      buffer[0] = ((0b0011&out)<<1)|((0b1100&out)<<3);
      buffer[1] = (0b1&(out>>4))|(0b100&(out>>3));
      i2c_ptr->sendData(address, (char*)buffer, 2, reg);
      free(buffer);
   } else {
      printf("Register cannot be written to.\n");
      exit(-1);
   }
}
