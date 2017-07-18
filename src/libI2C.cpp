#include "libI2C.h"

PYBIND11_PLUGIN(example) {
    py::module m("example", "pybind11 example plugin");
    py::class_<Board>(m, "Board")
      .def(py::init<const int>())
      .def("getID", &Board::getID);
    return m.ptr();
}
