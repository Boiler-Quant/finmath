#include "finmath/TimeSeries/rsi.h"

#include<numeric>
#include<cmath>
#include<algorithm>

double compute_avg_gain(const std::vector<double>& price_changes, size_t start, size_t window_size)
{
    double total_gain = 0.0;

    for (size_t i = start; i < start + window_size; i++)
    {
		double price_change = price_changes[i];

        if (price_change > 0)
        {
            total_gain += price_change;
        }
    }
    return total_gain / window_size;
}

double compute_avg_loss(const std::vector<double>& price_changes, size_t start, size_t window_size)
{
    double total_loss = 0.0;

    for (size_t i = start; i < start + window_size; i++)
    {
		double price_change = price_changes[i];

        if (price_change < 0)
        {
            total_loss += (-1 * price_change);
        }
    }
    return total_loss / window_size;
}

std::vector<double> compute_smoothed_rsi(const std::vector<double>& prices, size_t window_size)
{
    if (prices.size() < window_size) {
		return {};
    }

    std::vector<double> rsi_values; 
    std::vector<double> price_changes;

    for(size_t i = 1; i < prices.size(); i++)
    {
        price_changes.push_back(prices[i] - prices[i-1]);
    }

	size_t price_ch_window = window_size - 1;

    double avg_gain = compute_avg_gain(price_changes, 0, window_size);
    double avg_loss = compute_avg_loss(price_changes, 0, window_size);

    double rsi = 100;
	double rs;

	if (avg_loss != 0)
	{
		rs = avg_gain / avg_loss;
		rsi = 100.0 - (100.0 / (1.0 + rs));
	}
    
	rsi_values.push_back(rsi);

    for(size_t i = window_size - 1; i < price_changes.size(); i++)
    {
        double change = price_changes[i];
		
		avg_gain = (avg_gain * (window_size - 1) + (change > 0 ? change : 0)) / window_size;
		avg_loss = (avg_loss * (window_size - 1) - (change < 0 ? change : 0)) / window_size;

		if (avg_loss == 0)
		{
		    rsi_values.push_back(100.0);
			continue;
		}

		rs = avg_gain / avg_loss;
        rsi = 100.0 - (100.0 / (1.0 + rs));
        rsi_values.push_back(rsi);
    }

    return rsi_values;
}
