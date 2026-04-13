#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <numeric>

#include "finmath/InterestAndAnnuities/compound_interest.h"
#include "finmath/InterestAndAnnuities/simple_interest.h"
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/OptionPricing/binomial_tree.h"
#include "finmath/TimeSeries/rolling_volatility.h"
#include "finmath/TimeSeries/rolling_volatility_simd.h"
#include "finmath/TimeSeries/rolling_std_dev.h"
#include "finmath/TimeSeries/simple_moving_average.h"
#include "finmath/TimeSeries/simple_moving_average_simd.h"
#include "finmath/TimeSeries/rsi.h"
#include "finmath/TimeSeries/rsi_simd.h"
#include "finmath/TimeSeries/ema.h"
#include "finmath/TimeSeries/ema_simd.h"
#include "finmath/TimeSeries/autocorrelation.h"
#include "finmath/TimeSeries/rolling_corr.h"
#include "finmath/TimeSeries/returns.h"
#include "finmath/TimeSeries/rolling_zscore.h"
#include "finmath/TimeSeries/vwap.h"
#include "finmath/OrderBook/features.h"
#include "finmath/Regression/OLS.h"
#include "finmath/MarkovChains/markov_chain.h"
#include "finmath/MonteCarlo/monte_carlo.h"
#include "finmath/Risk/risk_metrics.h"
#include "finmath/Helper/simd_helper.h"
#include "finmath/GraphAlgos/bellman_arbitrage.h"

namespace py = pybind11;

std::vector<double> rolling_volatility_np(py::array_t<double> prices_arr, size_t window_size);
std::vector<double> simple_moving_average_np(py::array_t<double> data_arr, size_t window_size);
std::vector<double> compute_smoothed_rsi_np(py::array_t<double> prices_arr, size_t window_size);
std::vector<double> rolling_std_dev_np(py::array_t<double> arr, size_t window_size);
std::vector<double> log_returns_np(py::array_t<double> prices_arr);
std::vector<double> pct_returns_np(py::array_t<double> prices_arr);
std::vector<double> rolling_mean_np(py::array_t<double> data_arr, size_t window_size);
std::vector<double> rolling_zscore_np(py::array_t<double> data_arr, size_t window_size);
std::vector<double> vwap_np(py::array_t<double> prices_arr, py::array_t<double> volumes_arr, size_t window_size);
std::vector<double> rolling_dollar_volume_np(py::array_t<double> prices_arr, py::array_t<double> volumes_arr,
                                             size_t window_size);

PYBIND11_MODULE(_finmath, m)
{
    m.doc() = "Financial Math Library";

    py::enum_<OptionType>(m, "OptionType")
        .value("CALL", OptionType::CALL)
        .value("PUT", OptionType::PUT)
        .export_values();

    m.def("compound_interest", &compound_interest, "Compound interest",
          py::arg("principal"), py::arg("rate"), py::arg("time"), py::arg("frequency"));

    m.def("simple_interest", &simple_interest, "Simple interest I = P*r*t",
          py::arg("principal"), py::arg("rate"), py::arg("time"));

    m.def("black_scholes", &black_scholes, "Black-Scholes European price (strike, spot, ...).",
          py::arg("type"), py::arg("strike"), py::arg("spot"), py::arg("time"), py::arg("rate"), py::arg("volatility"),
          py::arg("dividend_yield") = 0.0);

    m.def("black_scholes_delta", &BlackScholes::compute_delta, py::arg("type"), py::arg("S0"), py::arg("K"),
          py::arg("t"), py::arg("r"), py::arg("q"), py::arg("sigma"));
    m.def("black_scholes_gamma", &BlackScholes::compute_gamma, py::arg("S0"), py::arg("K"), py::arg("t"), py::arg("r"),
          py::arg("q"), py::arg("sigma"));
    m.def("black_scholes_vega", &BlackScholes::compute_vega, py::arg("S0"), py::arg("K"), py::arg("t"), py::arg("r"),
          py::arg("q"), py::arg("sigma"));
    m.def("black_scholes_theta", &BlackScholes::compute_theta, py::arg("type"), py::arg("S0"), py::arg("K"),
          py::arg("t"), py::arg("T"), py::arg("r"), py::arg("q"), py::arg("sigma"));
    m.def("black_scholes_rho", &BlackScholes::compute_rho, py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("t"),
          py::arg("r"), py::arg("q"), py::arg("sigma"));

    m.def("binomial_option_pricing", &binomial_option_pricing, "Binomial option pricing",
          py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("T"), py::arg("r"), py::arg("sigma"), py::arg("N"));

    m.def("rolling_volatility", &rolling_volatility, py::arg("prices"), py::arg("window_size"));
    m.def("rolling_volatility", &rolling_volatility_np, py::arg("prices"), py::arg("window_size"));
    m.def("rolling_volatility_simd", &rolling_volatility_simd, py::arg("prices"), py::arg("window_size"));

    m.def("rolling_std_dev", static_cast<std::vector<double> (*)(size_t, const std::vector<double>&)>(&rolling_std_dev),
          py::arg("window_size"), py::arg("prices"));
    m.def("rolling_std_dev", &rolling_std_dev_np, py::arg("prices"), py::arg("window_size"));

    m.def("simple_moving_average", &simple_moving_average, py::arg("prices"), py::arg("window_size"));
    m.def("simple_moving_average", &simple_moving_average_np, py::arg("prices"), py::arg("window_size"));
    m.def("simple_moving_average_simd", &simple_moving_average_simd, py::arg("prices"), py::arg("window_size"));

    m.def("smoothed_rsi", &compute_smoothed_rsi, py::arg("prices"), py::arg("window_size"));
    m.def("smoothed_rsi", &compute_smoothed_rsi_np, py::arg("prices"), py::arg("window_size"));
    m.def("smoothed_rsi_simd", &compute_smoothed_rsi_simd, py::arg("prices"), py::arg("window_size"));

    m.def("ema_window", &compute_ema, py::arg("prices"), py::arg("window_size"));
    m.def("ema_smoothing", &compute_ema_with_smoothing, py::arg("prices"), py::arg("smoothing_factor"));
    m.def("ema_window_simd", &compute_ema_simd, py::arg("prices"), py::arg("window_size"));
    m.def("ema_smoothing_simd", &compute_ema_with_smoothing_simd, py::arg("prices"), py::arg("smoothing_factor"));

    m.def("get_simd_backend", &finmath::simd::get_simd_backend);

    m.def("detect_arbitrage", &detectArbitrageBellman<std::string>, py::arg("graph"));

    m.def("markov_is_row_stochastic", &markov_is_row_stochastic, py::arg("P"), py::arg("tol") = 1e-9);
    m.def("markov_matrix_multiply", &markov_matrix_multiply, py::arg("A"), py::arg("B"));
    m.def("markov_matrix_power", &markov_matrix_power, py::arg("P"), py::arg("n"));
    m.def("markov_steady_state", &markov_steady_state, py::arg("P"), py::arg("tol") = 1e-12,
          py::arg("max_iter") = 1000000);

    py::class_<OLS>(m, "OLS")
        .def(py::init<const std::vector<std::vector<double>>&, const std::vector<double>&>())
        .def("coefficients", &OLS::coefficients, py::return_value_policy::copy)
        .def("intercept", &OLS::intercept)
        .def("predict", &OLS::predict);

    m.def("autocorrelation", &autocorrelation, py::arg("values"), py::arg("max_lag"));

    m.def("rolling_cov", &rolling_cov, py::arg("window_size"), py::arg("x"), py::arg("y"));
    m.def("rolling_corr", &rolling_corr, py::arg("window_size"), py::arg("x"), py::arg("y"));
    m.def("rolling_beta", &rolling_beta, py::arg("window_size"), py::arg("y"), py::arg("x"));

    m.def("log_returns", &log_returns, py::arg("prices"));
    m.def("log_returns", &log_returns_np, py::arg("prices"));
    m.def("pct_returns", &pct_returns, py::arg("prices"));
    m.def("pct_returns", &pct_returns_np, py::arg("prices"));

    m.def("rolling_mean", &rolling_mean, py::arg("window_size"), py::arg("data"));
    m.def("rolling_mean", &rolling_mean_np, py::arg("data"), py::arg("window_size"));
    m.def("rolling_zscore", &rolling_zscore, py::arg("window_size"), py::arg("data"));
    m.def("rolling_zscore", &rolling_zscore_np, py::arg("data"), py::arg("window_size"));

    m.def("vwap", &vwap, py::arg("prices"), py::arg("volumes"), py::arg("window_size"));
    m.def("vwap", &vwap_np, py::arg("prices"), py::arg("volumes"), py::arg("window_size"));
    m.def("rolling_dollar_volume", &rolling_dollar_volume, py::arg("prices"), py::arg("volumes"),
          py::arg("window_size"));
    m.def("rolling_dollar_volume", &rolling_dollar_volume_np, py::arg("prices"), py::arg("volumes"),
          py::arg("window_size"));

    m.def("orderbook_midprice", &orderbook_midprice, py::arg("best_bid"), py::arg("best_ask"));
    m.def("orderbook_spread", &orderbook_spread, py::arg("best_bid"), py::arg("best_ask"));
    m.def("orderbook_microprice", &orderbook_microprice, py::arg("best_bid"), py::arg("bid_size"),
          py::arg("best_ask"), py::arg("ask_size"));
    m.def("orderbook_imbalance", &orderbook_imbalance, py::arg("bid_sizes"), py::arg("ask_sizes"), py::arg("depth"));
    m.def("orderbook_weighted_mid", &orderbook_weighted_mid, py::arg("bid_prices"), py::arg("bid_sizes"),
          py::arg("ask_prices"), py::arg("ask_sizes"), py::arg("depth"));

    m.def("monte_carlo_european", &monte_carlo_european, py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("T"),
          py::arg("r"), py::arg("q"), py::arg("sigma"), py::arg("n_paths"), py::arg("seed"));

    m.def("historical_var", &historical_var, py::arg("returns"), py::arg("alpha"));
    m.def("historical_cvar", &historical_cvar, py::arg("returns"), py::arg("alpha"));
    m.def("parametric_var", &parametric_var, py::arg("mean"), py::arg("std_dev"), py::arg("alpha"));
}

std::vector<double> rolling_volatility_np(py::array_t<double> prices_arr, size_t window_size)
{
    py::buffer_info buf_info = prices_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }
    const size_t n = static_cast<size_t>(buf_info.size);
    const double* ptr = static_cast<const double*>(buf_info.ptr);
    std::vector<double> prices(ptr, ptr + n);
    return rolling_volatility(prices, window_size);
}

std::vector<double> simple_moving_average_np(py::array_t<double> data_arr, size_t window_size)
{
    py::buffer_info buf_info = data_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }
    const size_t n = static_cast<size_t>(buf_info.size);
    const double* ptr = static_cast<const double*>(buf_info.ptr);
    std::vector<double> data(ptr, ptr + n);
    return simple_moving_average(data, window_size);
}

std::vector<double> compute_smoothed_rsi_np(py::array_t<double> prices_arr, size_t window_size)
{
    py::buffer_info buf_info = prices_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }
    const size_t n = static_cast<size_t>(buf_info.size);
    const double* ptr = static_cast<const double*>(buf_info.ptr);
    std::vector<double> prices(ptr, ptr + n);
    return compute_smoothed_rsi(prices, window_size);
}

std::vector<double> rolling_std_dev_np(py::array_t<double> prices_arr, size_t window_size)
{
    py::buffer_info buf_info = prices_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }
    const size_t n = static_cast<size_t>(buf_info.size);
    const double* ptr = static_cast<const double*>(buf_info.ptr);
    std::vector<double> prices(ptr, ptr + n);
    return rolling_std_dev(window_size, prices);
}

std::vector<double> log_returns_np(py::array_t<double> prices_arr)
{
    py::buffer_info buf_info = prices_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }
    const size_t n = static_cast<size_t>(buf_info.size);
    const double* ptr = static_cast<const double*>(buf_info.ptr);
    std::vector<double> prices(ptr, ptr + n);
    return log_returns(prices);
}

std::vector<double> pct_returns_np(py::array_t<double> prices_arr)
{
    py::buffer_info buf_info = prices_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }
    const size_t n = static_cast<size_t>(buf_info.size);
    const double* ptr = static_cast<const double*>(buf_info.ptr);
    std::vector<double> prices(ptr, ptr + n);
    return pct_returns(prices);
}

std::vector<double> rolling_mean_np(py::array_t<double> data_arr, size_t window_size)
{
    py::buffer_info buf_info = data_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }
    const size_t n = static_cast<size_t>(buf_info.size);
    const double* ptr = static_cast<const double*>(buf_info.ptr);
    std::vector<double> data(ptr, ptr + n);
    return rolling_mean(window_size, data);
}

std::vector<double> rolling_zscore_np(py::array_t<double> data_arr, size_t window_size)
{
    py::buffer_info buf_info = data_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }
    const size_t n = static_cast<size_t>(buf_info.size);
    const double* ptr = static_cast<const double*>(buf_info.ptr);
    std::vector<double> data(ptr, ptr + n);
    return rolling_zscore(window_size, data);
}

std::vector<double> vwap_np(py::array_t<double> prices_arr, py::array_t<double> volumes_arr, size_t window_size)
{
    py::buffer_info pinfo = prices_arr.request();
    py::buffer_info vinfo = volumes_arr.request();
    if (pinfo.ndim != 1 || vinfo.ndim != 1) {
        throw std::runtime_error("Input arrays must be 1-dimensional.");
    }
    if (pinfo.size != vinfo.size) {
        throw std::runtime_error("Prices and volumes must have the same length.");
    }
    const double* p = static_cast<const double*>(pinfo.ptr);
    const double* v = static_cast<const double*>(vinfo.ptr);
    const size_t n = static_cast<size_t>(pinfo.size);
    std::vector<double> pv(p, p + n);
    std::vector<double> vv(v, v + n);
    return vwap(pv, vv, window_size);
}

std::vector<double> rolling_dollar_volume_np(py::array_t<double> prices_arr, py::array_t<double> volumes_arr,
                                             size_t window_size)
{
    py::buffer_info pinfo = prices_arr.request();
    py::buffer_info vinfo = volumes_arr.request();
    if (pinfo.ndim != 1 || vinfo.ndim != 1) {
        throw std::runtime_error("Input arrays must be 1-dimensional.");
    }
    if (pinfo.size != vinfo.size) {
        throw std::runtime_error("Prices and volumes must have the same length.");
    }
    const double* p = static_cast<const double*>(pinfo.ptr);
    const double* v = static_cast<const double*>(vinfo.ptr);
    const size_t n = static_cast<size_t>(pinfo.size);
    std::vector<double> pv(p, p + n);
    std::vector<double> vv(v, v + n);
    return rolling_dollar_volume(pv, vv, window_size);
}
