#include <pybind11/pybind11.h>
#include <pybind11/stl.h>   // Automatic conversion between Python lists and std::vector
#include <pybind11/numpy.h> // Add numpy include

#include "finmath/InterestAndAnnuities/compound_interest.h"
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/OptionPricing/binomial_tree.h"
#include "finmath/TimeSeries/rolling_volatility.h"
#include "finmath/TimeSeries/rolling_volatility_simd.h"
#include "finmath/TimeSeries/simple_moving_average.h"
#include "finmath/TimeSeries/simple_moving_average_simd.h"
#include "finmath/TimeSeries/rsi.h"
#include "finmath/TimeSeries/rsi_simd.h"
#include "finmath/TimeSeries/ema.h"
#include "finmath/TimeSeries/ema_simd.h"
#include "finmath/Helper/simd_helper.h"
#include "finmath/GraphAlgos/bellman_arbitrage.h"

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
      m.def("rolling_volatility_simd", &rolling_volatility_simd, "Rolling Volatility (SIMD-optimized, zero-copy NumPy)",
            py::arg("prices"), py::arg("window_size"));

      // Binomial greeks (from main)
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

      // Black-Scholes greeks (from main)
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

      // Bind simple moving average
      m.def("simple_moving_average", &simple_moving_average, "Simple Moving Average (List input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("simple_moving_average", &simple_moving_average_np, "Simple Moving Average (NumPy/Pandas input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("simple_moving_average_simd", &simple_moving_average_simd, "Simple Moving Average (SIMD-optimized, zero-copy NumPy)",
            py::arg("prices"), py::arg("window_size"));

      // Bind RSI
      m.def("smoothed_rsi", &compute_smoothed_rsi, "Relative Strength Index(RSI) (List input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("smoothed_rsi", &compute_smoothed_rsi_np, "Relative Strength Index(RSI) (NumPy/Pandas input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("smoothed_rsi_simd", &compute_smoothed_rsi_simd, "Relative Strength Index (SIMD-optimized, zero-copy NumPy)",
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

      // Bind SIMD-optimized EMA
      m.def("ema_window_simd", &compute_ema_simd, "Exponential Moving Average - Window (SIMD-optimized, zero-copy NumPy)",
            py::arg("prices"), py::arg("window_size"));
      m.def("ema_smoothing_simd", &compute_ema_with_smoothing_simd, "Exponential Moving Average - Smoothing Factor (SIMD-optimized, zero-copy NumPy)",
            py::arg("prices"), py::arg("smoothing_factor"));

      // Utility function to get SIMD backend
      m.def("get_simd_backend", &finmath::simd::get_simd_backend, "Get the active SIMD backend (AVX, SSE, NEON, or Scalar)");

      // Bellman-based arbitrage detection (from main)
      m.def("detect_arbitrage", &detectArbitrageBellman<std::string>,
            "Detect arbitrage opportunities in a currency graph",
            py::arg("graph"));
}
