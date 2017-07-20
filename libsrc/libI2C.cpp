#include "libI2C.h"

I2CRaw::I2CRaw() {
   i2c = new I2CSema(EAPI_ID_I2C_EXTERNAL);
   requestBus();
   setFanOut(0b00000010);
}
I2CRaw::~I2CRaw() {
   if (checkAvailability()) {
      buffer = 0x00;
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   }
   delete i2c;
}
bool I2CRaw::checkAvailability(void) {
   i2c->receiveData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   return (buffer >> 1) & 0b1;
}
void I2CRaw::requestBus(void) {
   buffer = 0x7b; //enable interrupt
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x05);
   buffer = 0x00; //set request time
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x03);
   buffer = 0x01; //request downstream
   i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   if (checkAvailability()) {
      buffer = 0x05; //enable downstream
      i2c->sendData(ATCA_ARBITER, (char*)&buffer, 1, 0x01);
   } else {
      std::cout << "Bus not available." << '\n';
      exit(-1);
   }
}
void I2CRaw::setFanOut(uint8_t buses) {
   buffer = buses;
   i2c->sendData(ATCA_FANOUT, (char*)&buffer, 1, 0x00);
}
int I2CRaw::read(uint32_t address, uint32_t start_point) {
   i2c->receiveData(address, (char*)&buffer, 1, start_point);
   return (int)buffer;
}
void I2CRaw::write(uint32_t address, uint8_t buffer, uint32_t start_point) {
   i2c->sendData(address, (char*)&buffer, 1, start_point);
}

PYBIND11_PLUGIN(libI2C) {
   py::module m("libI2C", "python wrapper for I2C stack.");
   py::class_<Board>board(m, "Board");
   py::class_<ATCABoard>(m, "ATCABoard", board)
      .def(py::init<const std::string>())
      .def("setBus", py::overload_cast<std::string>(&ATCABoard::setBus), "Set the bus.")
      .def("setDevice", py::overload_cast<std::string>(&ATCABoard::setDevice), "Set device.")
      .def("setDevice", py::overload_cast<std::string, std::string>(&ATCABoard::setDevice), "Set bus and device.")
      .def("getDevices", py::overload_cast<>(&ATCABoard::getDevices), "Get devices on set bus.")
      .def("getDevices", py::overload_cast<std::string>(&ATCABoard::getDevices), "Get devices on given bus.")
      .def("getProperties", py::overload_cast<>(&ATCABoard::getProperties), "Get properties of set device.")
      .def("getProperties", py::overload_cast<std::string>(&ATCABoard::getProperties), "Get properties of given device.")
      .def("getProperties", py::overload_cast<std::string, std::string>(&ATCABoard::getProperties), "Get properties of given device on given bus.")
      .def("read", &ATCABoard::read)
      .def("write", &ATCABoard::write);
   py::class_<I2CRaw>(m, "I2CRaw")
      .def(py::init<>())
      .def("read", &I2CRaw::read)
      .def("write", &I2CRaw::write);
   return m.ptr();
}
