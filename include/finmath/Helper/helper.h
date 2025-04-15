#ifndef HELPER_H
#define HELPER_H

#include <xsimd/xsimd.hpp>

double normal_cdf(double x);
double normal_cdf_approx(double x) noexcept;
xsimd::batch<double> normal_cdf_approx(const xsimd::batch<double>& x) noexcept;
double normal_pdf(double x);
double combinations(int n, int k);

#endif
