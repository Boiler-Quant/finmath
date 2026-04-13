#ifndef FINMATH_ORDERBOOK_FEATURES_H
#define FINMATH_ORDERBOOK_FEATURES_H

#include <vector>

double orderbook_midprice(double best_bid, double best_ask);
double orderbook_spread(double best_bid, double best_ask);
double orderbook_microprice(double best_bid, double bid_size, double best_ask, double ask_size);

double orderbook_imbalance(const std::vector<double>& bid_sizes, const std::vector<double>& ask_sizes, size_t depth);

double orderbook_weighted_mid(const std::vector<double>& bid_prices, const std::vector<double>& bid_sizes,
                              const std::vector<double>& ask_prices, const std::vector<double>& ask_sizes, size_t depth);

#endif  // FINMATH_ORDERBOOK_FEATURES_H
