#pragma once
#include <cstdlib>
#include <stdio.h>
using namespace std;

class I2C {
public:
  I2C(void); //Initialise class
  ~I2C(void); //Class destructor
  void sendData(char* data); //Send data
  char* receiveData(char* buffer); //Receive data
};
