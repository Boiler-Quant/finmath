#ifndef FINMATH_OPTIMIZATION_LINALG_HELPERS_H
#define FINMATH_OPTIMIZATION_LINALG_HELPERS_H

#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <stdexcept>

namespace finmath
{
    namespace optimization
    {
        namespace linalg
        {

            // Calculate L2 norm (Euclidean norm)
            double norm(const std::vector<double> &v);

            // In-place vector addition: a += b
            void add_vectors(std::vector<double> &a, const std::vector<double> &b);

            // In-place vector subtraction: a -= b
            void subtract_vectors(std::vector<double> &a, const std::vector<double> &b);

            // In-place scalar multiplication: v *= scalar
            void scale_vector(std::vector<double> &v, double scalar);

            // In-place addition with scaling: a += scalar * b
            void add_scaled_vector(std::vector<double> &a, const std::vector<double> &b, double scalar);

            // In-place subtraction with scaling: a -= scalar * b
            void subtract_scaled_vector(std::vector<double> &a, const std::vector<double> &b, double scalar);

            // Clip vector v in-place if its norm exceeds max_norm
            void clip_vector_norm(std::vector<double> &v, double max_norm);

            // Sample a vector uniformly from a ball of given radius and dimension
            std::vector<double> sample_uniform_ball(double radius, size_t dimension, std::mt19937 &rng_engine);

        } // namespace linalg
    } // namespace optimization
} // namespace finmath

#endif // FINMATH_OPTIMIZATION_LINALG_HELPERS_H