#include "Bindings.h"

#include "Event.h"
#include "DataSourceFactory.h"
#include "Options.h"

namespace bindings {

    void init_enum_bindings(py::module_& m) {
        
        py::enum_<EventType>(m, "EventType")
            .value("MarketEvent", EventType::MarketEvent)
            .value("SignalEvent", EventType::SignalEvent)
            .value("OrderEvent", EventType::OrderEvent)
            .value("FillEvent", EventType::FillEvent)
            .value("None", EventType::None);

        py::enum_<SignalType>(m, "SignalType")
            .value("Long", SignalType::Long)
            .value("Short", SignalType::Short)
            .value("Exit", SignalType::Exit);

        py::enum_<OrderType>(m, "OrderType")
            .value("Buy", OrderType::Buy)
            .value("Sell", OrderType::Sell);

        py::enum_<OrderMode>(m, "OrderMode")
            .value("Market", OrderMode::Market)
            .value("Limit", OrderMode::Limit)
            .value("Stop", OrderMode::Stop);

        py::enum_<DataSourceType>(m, "DataSourceType")
            .value("CSV", DataSourceType::CSV)
            .value("URL", DataSourceType::URL);

        py::enum_<OptionType>(m, "OptionType")
            .value("Call", OptionType::Option_Call)
            .value("Put", OptionType::Option_Put);
            
    }

}
