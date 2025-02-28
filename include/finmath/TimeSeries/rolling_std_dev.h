#ifndef ROLLING_STD_DEV
#define ROLLING_STD_DEV
#include <vector>
//function to compute rolling_std_dev with a perfect sliding window size
std::vector<double> rolling_std_dev_fast(size_t window_size, const std::vector<double> &prices);
#endif