#include "Bindings.h"
#include "Strategy.h"

class PyStrategy : public Strategy {
public:
    
    using Strategy::Strategy; 
    
    using Strategy::pushSignal;

    std::shared_ptr<DataHandler> getDataHandler() const {return m_dataHandler; }
    std::shared_ptr<Portfolio> getPortfolio() const { return m_portfolio; }


    void onEvent(std::shared_ptr<Event> event) override {
        
            PYBIND11_OVERRIDE_PURE_NAME(
                void, 
                Strategy,
                "on_event",  
                onEvent, 
                event
            );
        
    }
};

namespace bindings {

    void init_strategy_bindings(py::module_& m) {
        
        py::class_<Strategy, PyStrategy, std::shared_ptr<Strategy>>(m, "Strategy")

            .def(py::init<const std::string&>(), py::arg("name"))
            .def("on_event", &Strategy::onEvent, py::arg("event"))
            

            .def("push_signal", &PyStrategy::pushSignal, py::arg("signal"))
            
            .def_readonly("Strategyname", &Strategy::Strategyname)

            .def_property_readonly("portfolio", [](Strategy& self) {
                return static_cast<PyStrategy&>(self).getPortfolio();
            })

            .def_property_readonly("datahandler", [](Strategy& self) {
                return static_cast<PyStrategy&>(self).getDataHandler();
            });
            
    }
}
