#include "I2CSema.h"
using namespace std;

int main() {
  I2CSema I = I2CSema();
  char msg[] = "Hello world";
  char* data = (char*)malloc(11*sizeof(char));
  for (int i = 0; i < 11; i++) {
    data[i] = msg[i];
    printf("%d ", data[i]);
  }
  printf("\n");
  I.sendData(data, 11);
  uint32_t max = I.getBusCap();
  printf("Max block length: %d\n", max);
  max = 0x20;
  unsigned char* dat = (unsigned char*)malloc(max*sizeof(unsigned char));
  dat = (unsigned char*)I.receiveData((char*)dat, (uint32_t)max);
  for (int i = 0; i < max; i++) {
    if (dat[i] < 0) {
      dat[i] = 256 + dat[i];
    }
    printf("%03d ", dat[i]);
    if (i%8 == 7) {
      printf("\n");
    }
  }
  return 0;
}