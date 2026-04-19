#include "Bindings.h"
#include "Options.h"
#include "OptionChain.h"
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "ChronoCast.h"

namespace bindings {

    void init_data_structure_bindings(py::module_& m) {
        
        py::class_<Greeks>(m, "Greeks")
            .def(py::init<>())
            .def_readwrite("delta", &Greeks::delta)
            .def_readwrite("gamma", &Greeks::gamma)
            .def_readwrite("theta", &Greeks::theta)
            .def_readwrite("vega", &Greeks::vega)
            .def_readwrite("rho", &Greeks::rho);

        py::class_<Marketdata>(m, "Marketdata")
            .def(py::init<>())
            .def_readwrite("underlying_price", &Marketdata::underlying_price)
            .def_readwrite("open", &Marketdata::open)
            .def_readwrite("high", &Marketdata::high)
            .def_readwrite("low", &Marketdata::low)
            .def_readwrite("close", &Marketdata::close)
            .def_readwrite("implied_volatility", &Marketdata::implied_volatility)
            .def_readwrite("volume", &Marketdata::volume)
            .def_readwrite("open_interest", &Marketdata::open_interest)
            .def_readwrite("risk_free_rate", &Marketdata::risk_free_rate)
            .def_readwrite("dividend_yield", &Marketdata::dividend_yield);

        py::class_<OptionContract>(m, "OptionContract")
            .def(py::init<>())
            .def(py::self < py::self)
            .def("value_at_expiration", &OptionContract::valueAtExpiration)
            .def("profit_at_expiration", &OptionContract::profitAtExpiration)
            .def_property_readonly("date", &OptionContract::getDate)
            .def_property_readonly("strike", &OptionContract::getStrike)
            .def_property_readonly("type", &OptionContract::getType)
            .def_property_readonly("symbol", &OptionContract::getSymbol)
            .def_property_readonly("underlying_symbol", &OptionContract::getUnderlyingSymbol)
            .def_property_readonly("multiplier", &OptionContract::getMultiplier)
            .def_property_readonly("expiration", &OptionContract::getExpiration)

            .def_property("marketdata", &OptionContract::getMarketdata, &OptionContract::setMarketdata)
            .def_property("greeks", &OptionContract::getGreeks, &OptionContract::setGreeks);

        py::class_<OptionChain, std::shared_ptr<OptionChain>>(m, "OptionChain")
            .def(py::init<std::chrono::year_month_day, std::vector<OptionContract>>())
            .def_property_readonly("date", &OptionChain::getDate)
            .def("get_all_contracts", &OptionChain::getAllContracts, py::return_value_policy::reference_internal)
            .def("get_contracts", &OptionChain::getContracts, py::return_value_policy::reference_internal)
            .def("get_contracts_by_strike_range", &OptionChain::getContractsByStrikeRange, py::return_value_policy::reference_internal)
            .def("get_available_expiries", &OptionChain::getAvailableExpiries)
            .def("get_available_strikes", &OptionChain::getAvailableStrikes)
            .def("get_atm", &OptionChain::getATM, py::return_value_policy::reference_internal)
            .def("get_underlying_price", &OptionChain::getUnderlyingPrice)
            .def("find_contract", &OptionChain::findContract, py::return_value_policy::reference_internal)
            .def("find_by_symbol", py::overload_cast<const std::string&>(&OptionChain::findBySymbol, py::const_), py::return_value_policy::reference_internal);
    }
}





