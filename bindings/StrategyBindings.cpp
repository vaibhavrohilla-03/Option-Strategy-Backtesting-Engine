#include "Bindings.h"
#include "Strategy.h"

class PyStrategy : public Strategy {
public:
    using Strategy::Strategy; 
    
    using Strategy::pushSignal;

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
            .def("on_event", &Strategy::onEvent)
            

            .def("push_signal", &PyStrategy::pushSignal, py::arg("signal"))
            
            .def_readonly("Strategyname", &Strategy::Strategyname);
            
    }
}
