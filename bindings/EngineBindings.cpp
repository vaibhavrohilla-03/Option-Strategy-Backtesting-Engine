#include "Bindings.h"
#include "BrokerModel.h"
#include "DataHandler.h"
#include "Portfolio.h"
#include "Backtester.h"


namespace bindings {

    void init_engine_bindings(py::module_& m) {

        py::class_<DataHandler, std::shared_ptr<DataHandler>>(m, "DataHandler")
            .def("get_chain_count", &DataHandler::getChainCount)
            .def("get_total_contracts", &DataHandler::getTotalContracts)
            .def("get_latest_price", &DataHandler::getLatestPrice, py::arg("symbol"))
            .def("get_contract", &DataHandler::getContract, py::arg("symbol"), py::return_value_policy::reference_internal);

        
        py::class_<BrokerConfig>(m, "BrokerConfig")
            .def(py::init<>())
            .def_readwrite("commission", &BrokerConfig::commission)
            .def_readwrite("slippage", &BrokerConfig::slippage);

        py::class_<Portfolio, std::shared_ptr<Portfolio>>(m, "Portfolio")
            .def("get_positions", &Portfolio::getPositions)
            .def("get_current_capital", &Portfolio::getCurrentCapital)
            .def("has_position", &Portfolio::hasPosition)
            .def("get_position_quantity", &Portfolio::getPositionQuantity);

        py::class_<Backtester>(m, "Backtester")
            .def(py::init<double, std::shared_ptr<Strategy>, BrokerConfig>(), 
                 py::arg("initial_capital"), py::arg("strat"), py::arg("brokerConfig"),
                 py::keep_alive<1, 3>())
            .def("add_data", &Backtester::addData, py::arg("type"), py::arg("path"))
            .def("run", &Backtester::run)
            .def_property_readonly("portfolio", &Backtester::getPortfolio, py::return_value_policy::reference_internal);
    }


}
