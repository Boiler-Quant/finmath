#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Automatic conversion between Python lists and std::vector

#include "finmath/InterestAndAnnuities/compound_interest.h"
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/OptionPricing/binomial_tree.h"
#include "finmath/TimeSeries/rolling_volatility.h"
#include "finmath/Regression/OLS.h"

namespace py = pybind11;

PYBIND11_MODULE(finmath, m) {
    m.doc() = "Financial Math Library";

    // Expose the OptionType enum class
    py::enum_<OptionType>(m, "OptionType")
        .value("CALL", OptionType::CALL)
        .value("PUT", OptionType::PUT)
        .export_values();

    // Bind compound interest function
    m.def("compound_interest", &compound_interest, "Calculate compound interest",
          py::arg("principal"), py::arg("rate"), py::arg("time"), py::arg("frequency"));

    // Bind Black-Scholes function
    m.def("black_scholes", &black_scholes, "Black Scholes Option Pricing",
          py::arg("type"), py::arg("strike"), py::arg("price"), py::arg("time"), py::arg("rate"), py::arg("volatility"));

    // Bind binomial option pricing function
    m.def("binomial_option_pricing", &binomial_option_pricing, "Binomial Option Pricing",
          py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("T"), py::arg("r"), py::arg("sigma"), py::arg("N"));

    // Bind rolling volatility
    m.def("rolling_volatility", &rolling_volatility, "Rolling Volatility",
          py::arg("prices"), py::arg("window_size"));

    // Bind OLS
    py::class_<OLS>(m, "OLS")
        .def(py::init<const std::vector<std::vector<double>>&, const std::vector<double>&>())
        .def("get_coefficients", &OLS::get_coefficients)
        .def("get_intercept", &OLS::get_intercept)
        .def("predict", &OLS::predict);
}
