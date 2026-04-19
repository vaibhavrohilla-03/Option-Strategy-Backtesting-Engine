#include "Bindings.h"

PYBIND11_MODULE(py_backtester, m) {
    
    PyDateTime_IMPORT;

    m.doc() = "Backtesting Engine Python";

    bindings::init_enum_bindings(m);
    bindings::init_data_structure_bindings(m);
    bindings::init_event_bindings(m);
    bindings::init_strategy_bindings(m);
    bindings::init_engine_bindings(m);
    bindings::init_pricing_bindings(m);

}