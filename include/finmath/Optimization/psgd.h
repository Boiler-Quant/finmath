#ifndef FINMATH_OPTIMIZATION_PSGD_H
#define FINMATH_OPTIMIZATION_PSGD_H

#include <vector>
#include <functional>
#include <stdexcept> // For potential exceptions

namespace finmath
{
    namespace optimization
    {

        /**
         * @brief Implements the Enhanced Perturbed Stochastic Gradient Descent (PSGD-C) algorithm.
         *
         * Based on the algorithm described in the user-provided LaTeX source and Python implementation.
         * Designed for non-convex optimization, incorporating EMA smoothing, gradient/parameter clipping,
         * and noise injection to escape saddle points.
         *
         * @param stochastic_grad Function providing a stochastic gradient estimate for a given point x.
         *                        Signature: std::vector<double>(const std::vector<double>& x)
         * @param objective_f Function providing the objective function value f(x). Needed for threshold calculation.
         *                     Signature: double(const std::vector<double>& x)
         * @param x0 Initial starting point (vector).
         * @param ell Smoothness parameter (Lipschitz constant of the gradient).
         * @param rho Hessian Lipschitz parameter.
         * @param eps Target accuracy for the norm of the smoothed gradient (termination criterion).
         * @param sigma Standard deviation estimate of the noise in the stochastic gradient samples (||grad_f_i(x) - grad_f(x)||).
         * @param delta Confidence parameter (probability of failure).
         * @param batch_size Mini-batch size used by stochastic_grad (influences g_thresh).
         * @param step_size_coeff Coefficient 'c' to calculate step size eta = c / ell.
         * @param ema_beta Decay factor for the exponential moving average of the gradient (typically 0.8-0.95).
         * @param max_iters Maximum number of iterations to perform.
         * @param grad_clip_norm Maximum L2 norm for the raw stochastic gradient (g_max). Set <= 0 to disable.
         * @param param_clip_norm Maximum L2 norm for the parameter vector x (x_max). Set <= 0 to disable.
         * @return The final optimized parameter vector x.
         *
         * @throws std::invalid_argument if input parameters are inconsistent (e.g., ell <= 0, rho <= 0, eps <= 0, sigma < 0, 0 < delta < 1, batch_size <= 0, c <= 0, 0 <= ema_beta < 1).
         */
        std::vector<double> perturbed_sgd(
            const std::function<std::vector<double>(const std::vector<double> &)> &stochastic_grad,
            const std::function<double(const std::vector<double> &)> &objective_f,
            const std::vector<double> &x0,
            // Problem params
            double ell,
            double rho,
            double eps = 1e-3,
            double sigma = 0.1,
            double delta = 0.1,
            // Algo params
            int batch_size = 32,
            double step_size_coeff = 0.5,
            double ema_beta = 0.9,
            int max_iters = 100000,
            double grad_clip_norm = 10.0,  // Default g_max (or disable if <= 0)
            double param_clip_norm = 100.0 // Default x_max (or disable if <= 0)
        );

    } // namespace optimization
} // namespace finmath

#endif // FINMATH_OPTIMIZATION_PSGD_H