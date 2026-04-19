#pragma once

#include <pybind11/pybind11.h>
#include <chrono>
#include <datetime.h>

namespace pybind11 {
namespace detail {

    template <> 
    struct type_caster<std::chrono::year_month_day> {
        
        public:

            PYBIND11_TYPE_CASTER(std::chrono::year_month_day, const_name("datetime.date"));
            
            bool load(handle src, bool) {

                if (!PyDateTimeAPI) { PyDateTime_IMPORT; }
                if(!PyDate_Check(src.ptr())) return false;
                
                auto year = PyDateTime_GET_YEAR(src.ptr());
                auto month = PyDateTime_GET_MONTH(src.ptr());
                auto day = PyDateTime_GET_DAY(src.ptr());
                
                value = std::chrono::year_month_day{
                    std::chrono::year{year}, 
                    std::chrono::month{static_cast<unsigned>(month)}, 
                    std::chrono::day{static_cast<unsigned>(day)}
                };
                
                return value.ok();
            }

            static handle cast(const std::chrono::year_month_day& src, return_value_policy, handle ) {
                
                if (!PyDateTimeAPI) { PyDateTime_IMPORT; }
                return PyDate_FromDate(
                    static_cast<int>(src.year()),
                    static_cast<unsigned>(src.month()),
                    static_cast<unsigned>(src.day())
                );
            }
    };

    }

}
