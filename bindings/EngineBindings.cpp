#include "Bindings.h"
#include "BrokerModel.h"
#include "Portfolio.h"
#include "Backtester.h"

namespace bindings {

    void init_engine_bindings(py::module_& m) {
        
        py::class_<BrokerConfig>(m, "BrokerConfig")
            .def(py::init<>())
            .def_readwrite("commission", &BrokerConfig::commission)
            .def_readwrite("slippage", &BrokerConfig::slippage);

        py::class_<Portfolio, std::shared_ptr<Portfolio>>(m, "Portfolio")
            .def("get_positions", &Portfolio::getPositions, py::return_value_policy::reference_internal)
            .def("get_current_capital", &Portfolio::getCurrentCapital)
            .def("has_position", &Portfolio::hasPosition)
            .def("get_position_quantity", &Portfolio::getPositionQuantity)
            .def("create_equity_dataframe_csv_and_metrics", &Portfolio::create_equity_dataframe_CSV_and_Metrics, py::arg("filepath"));

        py::class_<Backtester>(m, "Backtester")
            .def(py::init<double, std::shared_ptr<Strategy>, BrokerConfig>(), 
                 py::arg("initial_capital"), py::arg("strat"), py::arg("brokerConfig"))
            .def("add_data", &Backtester::addData, py::arg("type"), py::arg("path"))
            .def("run", &Backtester::run)
            .def_property_readonly("portfolio", &Backtester::getPortfolio, py::return_value_policy::reference_internal);
            
    }
}
