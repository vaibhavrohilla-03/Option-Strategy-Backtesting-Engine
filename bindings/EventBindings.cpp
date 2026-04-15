#include "Bindings.h"
#include "Event.h"
#include "OptionChain.h"

namespace bindings {

    void init_event_bindings(py::module_& m) {
        
        py::class_<Event, std::shared_ptr<Event>>(m, "Event")
            .def_readonly("type", &Event::type)
            .def_readonly("timestamp", &Event::timestamp);

        py::class_<MarketEvent, Event, std::shared_ptr<MarketEvent>>(m, "MarketEvent")
            .def(py::init<const std::chrono::year_month_day&, std::shared_ptr<OptionChain>>())
            .def_readonly("chain", &MarketEvent::chain);

        py::class_<SignalEvent, Event, std::shared_ptr<SignalEvent>>(m, "SignalEvent")
            .def(py::init<const std::chrono::year_month_day, const std::string&, SignalType, int, OrderMode, double>(), 
                 py::arg("ts"), py::arg("sym"), py::arg("stype"), py::arg("qty"), 
                 py::arg_v("mode", OrderMode::Market, "OrderMode.Market"), py::arg("price") = 0.0)
            .def_readwrite("symbol", &SignalEvent::symbol)
            .def_readwrite("signaltype", &SignalEvent::signaltype)
            .def_readwrite("quantity", &SignalEvent::quantity)
            .def_readwrite("suggestedmode", &SignalEvent::suggestedmode)
            .def_readwrite("limitprice", &SignalEvent::limitprice);

        py::class_<OrderEvent, Event, std::shared_ptr<OrderEvent>>(m, "OrderEvent")
            .def(py::init<const std::chrono::year_month_day, const std::string&, OrderType, OrderMode, int, double>(),
                 py::arg("ts"), py::arg("sym"), py::arg("ordtype"), py::arg("mode"), 
                 py::arg("qty"), py::arg("tprice") = 0.0)
            .def_readwrite("symbol", &OrderEvent::symbol)
            .def_readwrite("ordertype", &OrderEvent::ordertype)
            .def_readwrite("quantity", &OrderEvent::quantity)
            .def_readwrite("targetprice", &OrderEvent::targetprice)
            .def_readwrite("ordermode", &OrderEvent::ordermode);

        py::class_<FillEvent, Event, std::shared_ptr<FillEvent>>(m, "FillEvent")
            .def(py::init<const std::chrono::year_month_day, const std::string&, int, double, OrderType, double, double, int>())
            .def_readwrite("symbol", &FillEvent::symbol)
            .def_readwrite("quantity", &FillEvent::quantity)
            .def_readwrite("direction", &FillEvent::direction)
            .def_readwrite("fillprice", &FillEvent::fillPrice)
            .def_readwrite("slippage", &FillEvent::slippage)
            .def_readwrite("commision", &FillEvent::commision)
            .def_readwrite("multiplier", &FillEvent::multiplier);
            
    }
}
