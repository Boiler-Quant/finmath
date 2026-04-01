#include "finmath/TimeSeries/simple_moving_average.h"

#include <numeric>
#include <vector>

std::vector<double> simple_moving_average(const std::vector<double> &data, size_t window_size)
{
    std::vector<double> averages;

    // Check for valid window size
    if (window_size == 0)
    {
        throw std::runtime_error("Window size must be greater than 0.");
    }

    if (data.size() < window_size)
    {
        return {};
    }

    // Compute moving averages using a sliding window
    for (size_t i = 0; i <= data.size() - window_size; ++i)
    {
        // Calculate the sum of the current window
        double sum = std::accumulate(data.begin() + i, data.begin() + i + window_size, 0.0);

        // Compute the average and store it
        double avg = sum / static_cast<double>(window_size);
        averages.push_back(avg);
    }

    return averages;
}
