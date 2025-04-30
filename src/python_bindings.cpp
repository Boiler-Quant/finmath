#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Automatic conversion between Python lists and std::vector

#include "finmath/InterestAndAnnuities/compound_interest.h"
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/OptionPricing/binomial_tree.h"
#include "finmath/TimeSeries/rolling_volatility.h"
#include "finmath/TimeSeries/simple_moving_average.h"
#include "finmath/TimeSeries/rsi.h"

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

    m.def("black_scholes_multiple_calls", &black_scholes_multiple_calls, "Black-Scholes Pricing for Multiple Calls",
          py::arg("strikes"), py::arg("prices"), py::arg("times"), py::arg("rates"), py::arg("volatilities"));

    m.def("black_scholes_multiple_calls_basic", &black_scholes_multiple_calls_basic,
          "Black-Scholes multiple call pricing without SIMD",
          py::arg("strikes"),
          py::arg("prices"),
          py::arg("times"),
          py::arg("rates"),
          py::arg("volatilities"));


    // Bind binomial greeks
    m.def("binom_delta", &Binom::compute_delta, "Calculate the Delta of a Binomial Option",
            py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("T"), py::arg("r"), py::arg("sigma"), py::arg("N"), py::arg("delta_S") = -1);

    m.def("binom_gamma", &Binom::compute_gamma, "Calculate the Gamma of a Binomial Option",
            py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("T"), py::arg("r"), py::arg("sigma"), py::arg("N"), py::arg("delta_S") = -1);
    
    m.def("binom_vega", &Binom::compute_vega, "Calculate the Vega of a Binomial Option",
            py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("T"), py::arg("r"), py::arg("sigma"), py::arg("N"), py::arg("delta_sig") = -1);

    m.def("binom_theta", &Binom::compute_theta, "Calculate the Theta of a Binomial Option",
            py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("T"), py::arg("r"), py::arg("sigma"), py::arg("N"), py::arg("delta_T") = -1);

    m.def("binom_rho", &Binom::compute_rho, "Calculate the Rho of a Binomial Option",
            py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("T"), py::arg("r"), py::arg("sigma"), py::arg("N"), py::arg("delta_r") = -1);
    

    // Bind black scholes greeks
    m.def("black_scholes_delta", &BlackScholes::compute_delta, "Calculate the Delta of a Black Scholes Option",
            py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("t"), py::arg("r"), py::arg("q"), py::arg("sigma"));

    m.def("black_scholes_gamma", &BlackScholes::compute_gamma, "Calculate the Gamma of a Black Scholes Option",
            py::arg("S0"), py::arg("K"), py::arg("t"), py::arg("r"), py::arg("q"), py::arg("sigma"));

    m.def("black_scholes_vega", &BlackScholes::compute_vega, "Calculate the Vega of a Black Scholes Option",
            py::arg("S0"), py::arg("K"), py::arg("t"), py::arg("r"), py::arg("q"), py::arg("sigma"));
    
    m.def("black_scholes_theta", &BlackScholes::compute_theta, "Calculate the Theta of a Black Scholes Option",
            py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("t"), py::arg("T"), py::arg("r"), py::arg("q"), py::arg("sigma"));

    m.def("black_scholes_rho", &BlackScholes::compute_rho, "Calculate the Rho of a Black Scholes Option",
            py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("t"), py::arg("r"), py::arg("q"), py::arg("sigma"));

    // Bind rolling volatility
    m.def("rolling_volatility", &rolling_volatility, "Rolling Volatility",
          py::arg("prices"), py::arg("window_size"));

    m.def("simple_moving_average", &simple_moving_average, "Simple Moving Average",
          py::arg("prices"), py::arg("window_size"));

    m.def("smoothed_rsi", &compute_smoothed_rsi, "Relative Strength Index(RSI)",
          py::arg("prices"), py::arg("window_size"));
}
