#include "finmath/OrderBook/features.h"

#include <algorithm>
#include <stdexcept>

double orderbook_midprice(double best_bid, double best_ask)
{
    if (best_ask < best_bid) {
        throw std::runtime_error("best_ask must be >= best_bid");
    }
    return 0.5 * (best_bid + best_ask);
}

double orderbook_spread(double best_bid, double best_ask)
{
    if (best_ask < best_bid) {
        throw std::runtime_error("best_ask must be >= best_bid");
    }
    return best_ask - best_bid;
}

double orderbook_microprice(double best_bid, double bid_size, double best_ask, double ask_size)
{
    if (best_ask < best_bid) {
        throw std::runtime_error("best_ask must be >= best_bid");
    }
    const double denom = bid_size + ask_size;
    if (denom == 0.0) {
        return orderbook_midprice(best_bid, best_ask);
    }

    return (best_bid * ask_size + best_ask * bid_size) / denom;
}

static void validate_book_inputs(const std::vector<double>& bid_prices, const std::vector<double>& bid_sizes,
                                 const std::vector<double>& ask_prices, const std::vector<double>& ask_sizes,
                                 size_t depth)
{
    if (depth == 0) {
        throw std::runtime_error("depth must be > 0");
    }
    if (bid_prices.size() != bid_sizes.size()) {
        throw std::runtime_error("bid_prices and bid_sizes must have the same length");
    }
    if (ask_prices.size() != ask_sizes.size()) {
        throw std::runtime_error("ask_prices and ask_sizes must have the same length");
    }
    if (bid_prices.empty() || ask_prices.empty()) {
        throw std::runtime_error("book sides cannot be empty");
    }
}

double orderbook_imbalance(const std::vector<double>& bid_sizes, const std::vector<double>& ask_sizes, size_t depth)
{
    if (depth == 0) {
        throw std::runtime_error("depth must be > 0");
    }

    const size_t d = std::min({depth, bid_sizes.size(), ask_sizes.size()});
    double bid_sum = 0.0;
    double ask_sum = 0.0;
    for (size_t i = 0; i < d; ++i) {
        bid_sum += bid_sizes[i];
        ask_sum += ask_sizes[i];
    }

    const double total = bid_sum + ask_sum;
    if (total == 0.0) {
        return 0.0;
    }
    return (bid_sum - ask_sum) / total;
}

double orderbook_weighted_mid(const std::vector<double>& bid_prices, const std::vector<double>& bid_sizes,
                              const std::vector<double>& ask_prices, const std::vector<double>& ask_sizes, size_t depth)
{
    validate_book_inputs(bid_prices, bid_sizes, ask_prices, ask_sizes, depth);

    const size_t d = std::min({depth, bid_prices.size(), ask_prices.size()});
    double num = 0.0;
    double den = 0.0;
    for (size_t i = 0; i < d; ++i) {
        num += bid_prices[i] * bid_sizes[i];
        den += bid_sizes[i];
        num += ask_prices[i] * ask_sizes[i];
        den += ask_sizes[i];
    }

    if (den == 0.0) {
        return orderbook_midprice(bid_prices[0], ask_prices[0]);
    }
    return num / den;
}
