#include "finmath/Optimization/linalg_helpers.h"
#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <stdexcept>
#include <algorithm> // For std::transform

namespace finmath
{
    namespace optimization
    {
        namespace linalg
        {

            double norm(const std::vector<double> &v)
            {
                double sum_sq = 0.0;
                for (double val : v)
                {
                    sum_sq += val * val;
                }
                return std::sqrt(sum_sq);
                // Alternative: return std::sqrt(std::inner_product(v.begin(), v.end(), v.begin(), 0.0));
            }

            void add_vectors(std::vector<double> &a, const std::vector<double> &b)
            {
                if (a.size() != b.size())
                    throw std::invalid_argument("Vectors must have the same size for addition.");
                for (size_t i = 0; i < a.size(); ++i)
                {
                    a[i] += b[i];
                }
            }

            void subtract_vectors(std::vector<double> &a, const std::vector<double> &b)
            {
                if (a.size() != b.size())
                    throw std::invalid_argument("Vectors must have the same size for subtraction.");
                for (size_t i = 0; i < a.size(); ++i)
                {
                    a[i] -= b[i];
                }
            }

            void scale_vector(std::vector<double> &v, double scalar)
            {
                for (double &val : v)
                {
                    val *= scalar;
                }
            }

            void add_scaled_vector(std::vector<double> &a, const std::vector<double> &b, double scalar)
            {
                if (a.size() != b.size())
                    throw std::invalid_argument("Vectors must have the same size for add_scaled_vector.");
                for (size_t i = 0; i < a.size(); ++i)
                {
                    a[i] += scalar * b[i];
                }
            }

            void subtract_scaled_vector(std::vector<double> &a, const std::vector<double> &b, double scalar)
            {
                if (a.size() != b.size())
                    throw std::invalid_argument("Vectors must have the same size for subtract_scaled_vector.");
                for (size_t i = 0; i < a.size(); ++i)
                {
                    a[i] -= scalar * b[i];
                }
            }

            void clip_vector_norm(std::vector<double> &v, double max_norm)
            {
                if (max_norm <= 0)
                    return; // No clipping if max_norm is non-positive
                double current_norm = norm(v);
                if (current_norm > max_norm)
                {
                    double scale_factor = max_norm / current_norm;
                    scale_vector(v, scale_factor);
                }
            }

            // Reference for sampling uniformly from ball: Marsaglia (1972) "Choosing a Point from the Surface of a Sphere"
            // Generate Gaussian vector, then normalize and scale by U^(1/d)
            std::vector<double> sample_uniform_ball(double radius, size_t dimension, std::mt19937 &rng_engine)
            {
                if (radius < 0)
                    throw std::invalid_argument("Radius cannot be negative.");
                if (dimension == 0)
                    return {};
                if (radius == 0)
                    return std::vector<double>(dimension, 0.0);

                std::normal_distribution<double> gaussian_dist(0.0, 1.0);
                std::vector<double> v(dimension);
                for (size_t i = 0; i < dimension; ++i)
                {
                    v[i] = gaussian_dist(rng_engine);
                }

                double current_norm = norm(v);
                if (current_norm < 1e-15)
                { // Avoid division by zero for zero vector
                    // Return a vector with radius in the first dimension, others zero
                    v[0] = radius;
                    return v;
                }

                // Normalize to unit sphere
                scale_vector(v, 1.0 / current_norm);

                // Scale to be uniformly within the ball
                std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);
                double u = uniform_dist(rng_engine);
                double scale_factor = radius * std::pow(u, 1.0 / static_cast<double>(dimension));
                scale_vector(v, scale_factor);

                return v;
            }

        } // namespace linalg
    } // namespace optimization
} // namespace finmath