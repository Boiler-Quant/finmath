#include <pybind11/pybind11.h>
#include <pybind11/stl.h>        // Automatic conversion between Python lists and std::vector
#include <pybind11/numpy.h>      // Add numpy include
#include <pybind11/functional.h> // Add functional include for std::function conversion

#include "finmath/InterestAndAnnuities/compound_interest.h"
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/OptionPricing/binomial_tree.h"
#include "finmath/TimeSeries/rolling_volatility.h"
#include "finmath/TimeSeries/simple_moving_average.h"
#include "finmath/TimeSeries/rsi.h"
#include "finmath/TimeSeries/ema.h"
#include "finmath/Optimization/psgd.h" // Include PSGD header

namespace py = pybind11;

PYBIND11_MODULE(finmath, m)
{
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
      m.def("rolling_volatility", &rolling_volatility, "Rolling Volatility (List input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("rolling_volatility", &rolling_volatility_np, "Rolling Volatility (NumPy/Pandas input)",
            py::arg("prices"), py::arg("window_size"));

      // Bind simple moving average
      m.def("simple_moving_average", &simple_moving_average, "Simple Moving Average (List input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("simple_moving_average", &simple_moving_average_np, "Simple Moving Average (NumPy/Pandas input)",
            py::arg("prices"), py::arg("window_size"));

      // Bind RSI
      m.def("smoothed_rsi", &compute_smoothed_rsi, "Relative Strength Index(RSI) (List input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("smoothed_rsi", &compute_smoothed_rsi_np, "Relative Strength Index(RSI) (NumPy/Pandas input)",
            py::arg("prices"), py::arg("window_size"));

      // Bind EMA (window)
      m.def("ema_window", &compute_ema, "Exponential Moving Average - Window (List input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("ema_window", &compute_ema_np, "Exponential Moving Average - Window (NumPy/Pandas input)",
            py::arg("prices"), py::arg("window_size"));

      // Bind EMA (smoothing factor)
      m.def("ema_smoothing", &compute_ema_with_smoothing, "Exponential Moving Average - Smoothing Factor (List input)",
            py::arg("prices"), py::arg("smoothing_factor"));
      m.def("ema_smoothing", &compute_ema_with_smoothing_np, "Exponential Moving Average - Smoothing Factor (NumPy/Pandas input)",
            py::arg("prices"), py::arg("smoothing_factor"));

      // --- Optimization Module ---
      // Could also be a submodule: auto m_opt = m.def_submodule("optimization", ...);
      m.def("perturbed_sgd", &finmath::optimization::perturbed_sgd,
            "Enhanced Perturbed Stochastic Gradient Descent (PSGD-C)",
            py::arg("stochastic_grad"),
            py::arg("objective_f"),
            py::arg("x0"),
            // Problem params
            py::arg("ell"),
            py::arg("rho"),
            py::arg("eps") = 1e-3,
            py::arg("sigma") = 0.1,
            py::arg("delta") = 0.1,
            // Algo params
            py::arg("batch_size") = 32,
            py::arg("step_size_coeff") = 0.5,
            py::arg("ema_beta") = 0.9,
            py::arg("max_iters") = 100000,
            py::arg("grad_clip_norm") = 10.0,
            py::arg("param_clip_norm") = 100.0);
}
