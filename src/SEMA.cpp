#include "I2CSema.h"
using namespace std;

int main() {

  I2CSema I = I2CSema(EAPI_ID_I2C_EXTERNAL, DS3232);
  char msg[] = "Hello World";
  int length = 11;
  char* data = (char*)malloc(length*sizeof(char));
  printf("Sending %d bytes...\n", length);
  for (int i = 0; i < length; i++) { data[i] = msg[i]; }
  I.sendData(data, length, 0x15);
  printf("Done.\n");

  int max = 0x20;
  char* dat = (char*)malloc(max*sizeof(unsigned char));
  dat = I.receiveData(dat, (uint32_t)max, 0x00);
  printf("Reading %d bytes:\n", max);
  for (int i = 0; i < max; i++) {
    printf("%03d ", (unsigned char)dat[i]);
    if (i%8 == 7) {
      printf("\n");
    }
  }
  return 0;
}
