#include "libI2C.h"

PYBIND11_PLUGIN(libI2C) {
    py::module m("libI2C", "python wrapper for I2C stack.");
    py::class_<Board>board(m, "Board");
    py::class_<ATCABoard>(m, "ATCABoard", board)
      .def(py::init<const std::string>())
      .def("setBus", py::overload_cast<std::string>(&ATCABoard::setBus), "Set the bus.")
      .def("setDevice", py::overload_cast<std::string>(&ATCABoard::setDevice), "Set device.")
      .def("read", &ATCABoard::read)
      .def("write", &ATCABoard::write);
      ;
    return m.ptr();
}
