#include <string>
#include <cstdlib>
#include <unordered_map>
#include <stdlib.h>
#include <functional>
#include <stdio.h>

class I2CBaseRegister {
public:
   virtual void read(void* i2c_ptr, uint32_t addr) = 0;
   virtual void write(void* i2c_ptr, uint32_t addr) = 0;
};

class GenericI2CRegister : public I2CBaseRegister {
   std::function<void(void*, uint32_t)> mRead, mWrite;
public:
   GenericI2CRegister(std::function<void(void*, uint32_t)> read_func, std::function<void(void*, uint32_t)> write_func) {
      mRead = read_func;
      mWrite = write_func;
   }
   void read(void* i2c_ptr, uint32_t addr) {
      mRead(i2c_ptr, addr);
   }
   void write(void* i2c_ptr, uint32_t addr) {
      mWrite(i2c_ptr, addr);
   }
};

auto lambda_read = [](void* i2c_ptr, uint32_t addr) {
   printf("Reading something to address: %d.\n", addr);
};
auto lambda_write = [](void* i2c_ptr, uint32_t addr) {
   printf("Writing something to address: %d.\n", addr);
};

std::unordered_map<std::string, I2CBaseRegister*> registers = {{"temperature", new GenericI2CRegister(lambda_read,lambda_write)},
                                                               {"humidity",    new GenericI2CRegister(lambda_read,lambda_write)}};

int main(void) {
   I2CBaseRegister* G = registers["temperature"];
   void* ptr; //pointer to I2C object
   G->read(ptr, 0x00);
   G->write(ptr, 0x01);
   return 0;
}
