#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Automatic conversion between Python lists and std::vector

#include "finmath/InterestAndAnnuities/compound_interest.h"
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/OptionPricing/binomial_tree.h"
#include "finmath/TimeSeries/rolling_volatility.h"
#include "finmath/TimeSeries/simple_moving_average.h"
#include "finmath/TimeSeries/rsi.h"
#include "finmath/TimeSeries/ema.h"

#include "finmath/InterestAndAnnuities/discount_factor.h"
#include "finmath/InterestAndAnnuities/present_future_value.h"
#include "finmath/InterestAndAnnuities/annuity.h"
#include "finmath/InterestAndAnnuities/cash_flow.h"
#include "finmath/FixedIncome/bond_pricing.h"

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
//     m.def("rsi", &compute_rsi, "Relative Strength Index",
          py::arg("prices"), py::arg("window_size"));

    m.def("ema_window", &compute_ema, "Exponential Moving Average - Window",
          py::arg("prices"), py::arg("window_size"));

    m.def("ema_smoothing", &compute_ema_with_smoothing, "Exponential Moving Average - Smoothing Factor",
            py::arg("prices"), py::arg("smoothing_factor"));

    // Discount factors
    m.def("discount_factor", &discount_factor,
          "Discount factor (discrete compounding)",
          py::arg("rate"), py::arg("time"));

    m.def("discount_factor_continuous", &discount_factor_continuous,
          "Discount factor (continuous compounding)",
          py::arg("rate"), py::arg("time"));

    m.def("future_value_factor", &future_value_factor,
          "Future value factor",
          py::arg("rate"), py::arg("time"));

    //Present and future value
    m.def("present_value", &present_value,
          "Present value with discrete compounding",
           py::arg("future_value"), py::arg("rate"), py::arg("time"));

    m.def("future_value", &future_value,
          "Future value with discrete compounding",
           py::arg("present_value"), py::arg("rate"), py::arg("time"));

    m.def("present_value_continuous", &present_value_continuous,
          "Present value with continuous compounding",
           py::arg("future_value"), py::arg("rate"), py::arg("time"));

    m.def("future_value_continuous", &future_value_continuous,
          "Future value with continuous compounding",
           py::arg("present_value"), py::arg("rate"), py::arg("time"));
}
