#include "pcigen.h"

PCIGen::PCIGen(uint32_t addr) {
  I2CSema* I2C = new I2CSema(EAPI_ID_I2C_EXTERNAL, addr, true);
}

PCIGen::~PCIGen(void) {}

void PCIGen::setPLLMode( tPllBandwidth mode ) {
  char bits( mode );
  // if (mode == LOW)           bits = 0b1000;
  // else if ( mode == MEDIUM ) bits = 0b1010;
  // else                       bits = 0b1110;
  I2C->sendData(&bits, 1, 0x0);
}

void PCIGen::enableOutput(void) {
  char bits = 0b10;
  I2C->sendData(&bits, 1, 0x1);
}

int main() {
  PCIGen pci = PCIGen(0x0);
  //pci.setPLLMode("low");
  pci.enableOutput();
  return 0;
}
