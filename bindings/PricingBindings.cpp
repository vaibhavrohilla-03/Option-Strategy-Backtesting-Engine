#include "Bindings.h"
#include "Pricing/BlackScholes.h"
#include "Options.h"

namespace bindings {

    void init_pricing_bindings(py::module_& m) {
        
        py::class_<BlackScholesModel>(m, "BlackScholesModel")
            .def(py::init<int>(), py::arg("iterations") = 100)
            .def("calculate_price", &BlackScholesModel::calculatePrice, py::arg("option"))
            .def("calculate_greeks", &BlackScholesModel::calculateGreeks, py::arg("option"));
            
    }
}
