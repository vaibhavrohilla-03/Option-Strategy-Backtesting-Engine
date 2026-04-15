#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "ChronoCast.h"

namespace py = pybind11;

namespace bindings {

    void init_enum_bindings(py::module_& m);
    void init_data_structure_bindings(py::module_& m);
    void init_event_bindings(py::module_& m);
    void init_strategy_bindings(py::module_& m);
    void init_engine_bindings(py::module_& m);
    void init_pricing_bindings(py::module_& m);

}
