#include "libI2C.h"

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
    return m.ptr();
}
