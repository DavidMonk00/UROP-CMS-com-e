#include <pybind11/pybind11.h>

class Board {
public:
   int ID;
   Board(int id) {
      ID = id;
   }
   ~Board(void) {}
   const int getID(void) const {
      return ID;
   }
};

namespace py = pybind11;

PYBIND11_PLUGIN(example) {
    py::module m("example", "pybind11 example plugin");
    py::class_<Board>(m, "Board")
      .def(py::init<const int>())
      .def("getID", &Board::getID);
    return m.ptr();
}
