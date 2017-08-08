/**
  @file libI2C.hpp
  @brief Header file for Python binding library.
  @author David Monk - Imperial College London
  @version 1.0
*/

#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "ATCABoard.hpp"
#include "COMETestBoard.hpp"
#include "I2CRaw.hpp"

namespace py = pybind11;
