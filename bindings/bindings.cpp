#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(py_backtester, m, py::mod_gil_not_used()) {

	m.doc() = "test";
	m.def("add", &add, "A function that adds two numbers");
}