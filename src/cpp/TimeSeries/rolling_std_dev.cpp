#include "finmath/TimeSeries/rolling_std_dev.h"
#include <cmath>
#include <algorithm>

std::vector<double> rolling_std_dev_fast(size_t window, const std::vector<double> &prices)
{
    if (window == 0) {
        throw std::invalid_argument("Window size cannot be zero.");
    }

    std::vector<double> result(prices.size(),0.0);
    if(window > prices.size()) return result;

    double mean = 0.0;
    double squared_sum = 0.0;
    for(size_t i = 0; i < window; i++)
    {
        double old_mean = mean;
        mean = mean + (prices[i] - mean)/(i+1);
        squared_sum = squared_sum + (prices[i] - old_mean)*(prices[i] - mean);
    }
    result[window - 1] = std::sqrt(squared_sum/window);
    double price_out = prices[0];
    for(size_t i = window; i < prices.size() ; i++)
    {
        double price_in = prices[i];
        double old_mean = mean;
        mean += (price_in - price_out)/window;
        squared_sum += (price_in - price_out)* (price_in + price_out - 2 * mean);
        result[i] = std::sqrt(squared_sum/window);
        price_out = prices[i - window + 1];
    }
    
    return result;


}
