#include "finmath/Optimization/psgd.h"
#include "finmath/Optimization/linalg_helpers.h"

#include <vector>
#include <functional>
#include <cmath>
#include <random>
#include <stdexcept>
#include <limits>
#include <iostream>  // For potential debug/verbose output
#include <algorithm> // For std::max

namespace finmath
{
    namespace optimization
    {

        std::vector<double> perturbed_sgd(
            const std::function<std::vector<double>(const std::vector<double> &)> &stochastic_grad,
            const std::function<double(const std::vector<double> &)> &objective_f,
            const std::vector<double> &x0,
            // Problem params
            double ell,
            double rho,
            double eps,
            double sigma,
            double delta,
            // Algo params
            int batch_size,
            double step_size_coeff,
            double ema_beta,
            int max_iters,
            double grad_clip_norm,
            double param_clip_norm)
        {
            // --- Input Validation ---
            if (ell <= 0)
                throw std::invalid_argument("Smoothness parameter ell must be positive.");
            if (rho <= 0)
                throw std::invalid_argument("Hessian Lipschitz parameter rho must be positive.");
            if (eps <= 0)
                throw std::invalid_argument("Target accuracy eps must be positive.");
            if (sigma < 0)
                throw std::invalid_argument("Gradient noise std dev sigma cannot be negative.");
            if (delta <= 0 || delta >= 1)
                throw std::invalid_argument("Confidence delta must be between 0 and 1.");
            if (batch_size <= 0)
                throw std::invalid_argument("Batch size must be positive.");
            if (step_size_coeff <= 0)
                throw std::invalid_argument("Step size coefficient c must be positive.");
            if (ema_beta < 0 || ema_beta >= 1)
                throw std::invalid_argument("EMA beta must be in [0, 1).");
            if (max_iters <= 0)
                throw std::invalid_argument("Max iterations must be positive.");
            if (x0.empty())
                throw std::invalid_argument("Initial point x0 cannot be empty.");

            // --- Initialization ---
            size_t d = x0.size();
            std::vector<double> x = x0;
            std::vector<double> g_ema(d, 0.0);
            std::mt19937 rng_engine(std::random_device{}()); // Random number generator

            // Calculate f(x0) - needed for chi
            double f0 = objective_f(x0);
            // Note: Using f0 directly in log assumes f_star is negligible or zero relative to f0.
            // A more robust approach might require a user-provided f_star estimate.
            const double f0_minus_fstar_proxy = std::max(f0, 1e-9); // Use f0 (or small positive if f0=0)

            // Derived thresholds (matching LaTeX formulae)
            const double chi = 3.0 * std::max(
                                         std::log(static_cast<double>(d) * ell * f0_minus_fstar_proxy / (step_size_coeff * eps * eps * delta)),
                                         4.0);
            const double g_thresh = std::sqrt(step_size_coeff) / (chi * chi) * eps + sigma / std::sqrt(static_cast<double>(batch_size));
            const double radius = std::sqrt(step_size_coeff) / (chi * chi) * eps / ell;
            // Note: Original t_thresh had chi/c^2. Assuming typo and it should be chi^2/c based on similar literature.
            // If chi/c^2 is correct, the calculation needs update.
            // Let's use chi / (c^2) as per the provided formula for now.
            const double t_thresh_val = chi / (step_size_coeff * step_size_coeff) * ell / std::sqrt(rho * eps);
            const int t_thresh = static_cast<int>(std::ceil(t_thresh_val)); // Use ceil to be safe?
            // const double f_thresh = step_size_coeff / std::pow(chi, 3) * std::sqrt(eps * eps * eps / rho); // Not used as early exit is omitted

            const double step_size = step_size_coeff / ell;
            int t_noise = -t_thresh - 1; // Initialize so perturbation is possible early on

            // --- Main Loop ---
            for (int t = 0; t < max_iters; ++t)
            {
                // Get stochastic gradient
                std::vector<double> g = stochastic_grad(x);
                if (g.size() != d)
                {
                    throw std::runtime_error("Stochastic gradient dimension mismatch.");
                }

                // Update EMA
                linalg::scale_vector(g_ema, ema_beta);
                linalg::add_scaled_vector(g_ema, g, (1.0 - ema_beta));

                // Perturbation step
                if (linalg::norm(g_ema) <= g_thresh && (t - t_noise) > t_thresh)
                {
                    std::vector<double> noise = linalg::sample_uniform_ball(radius, d, rng_engine);
                    linalg::add_vectors(x, noise); // x = x + noise
                    t_noise = t;
                    // Early exit check omitted here (as in Python code)
                }

                // Gradient clipping
                linalg::clip_vector_norm(g, grad_clip_norm);

                // SGD update step
                linalg::subtract_scaled_vector(x, g, step_size); // x = x - step_size * g

                // Parameter clipping
                linalg::clip_vector_norm(x, param_clip_norm);

                // Termination condition
                if (linalg::norm(g_ema) <= eps && (t - t_noise) > t_thresh)
                {
                    // std::cout << "Termination condition met at iteration " << t << std::endl;
                    break;
                }
            }

            return x;
        }

    } // namespace optimization
} // namespace finmath