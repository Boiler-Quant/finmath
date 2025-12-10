#include "finmath/StatisticalModels/garch.h"
#include <cmath>

namespace finmath {
namespace StatisticalModels {

GARCH::GARCH(double omega, double alpha, double beta)
    : omega_(omega), alpha_(alpha), beta_(beta) {}

void GARCH::fit(const Eigen::VectorXd& returns) {
    int n = returns.size();
    
    // Initialize vectors
    fitted_values_ = Eigen::VectorXd::Zero(n);
    residuals_ = Eigen::VectorXd::Zero(n);
    conditional_variances_ = Eigen::VectorXd::Zero(n);
    
    // Initialize first conditional variance
    conditional_variances_(0) = returns.array().square().mean();
    
    // Update parameters using maximum likelihood
    update_parameters(returns);
}

void GARCH::update_parameters(const Eigen::VectorXd& returns) {
    int n = returns.size();
    
    // Initialize parameters for optimization
    Eigen::Vector3d params(omega_, alpha_, beta_);
    
    // Simple gradient descent optimization
    double learning_rate = 0.0001;
    int max_iterations = 1000;
    
    for (int iter = 0; iter < max_iterations; ++iter) {
        // Compute conditional variances
        for (int t = 1; t < n; ++t) {
            conditional_variances_(t) = omega_ + 
                alpha_ * returns(t-1) * returns(t-1) + 
                beta_ * conditional_variances_(t-1);
        }
        
        // Compute residuals
        residuals_ = returns.array() / conditional_variances_.array().sqrt();
        
        // Compute gradients
        double grad_omega = 0.0;
        double grad_alpha = 0.0;
        double grad_beta = 0.0;
        
        for (int t = 1; t < n; ++t) {
            double var_t = conditional_variances_(t);
            double r_t = returns(t);
            double r_t_prev = returns(t-1);
            
            grad_omega += (1.0 / var_t) * (r_t * r_t / var_t - 1.0);
            grad_alpha += (r_t_prev * r_t_prev / var_t) * (r_t * r_t / var_t - 1.0);
            grad_beta += (conditional_variances_(t-1) / var_t) * (r_t * r_t / var_t - 1.0);
        }
        
        // Update parameters
        omega_ += learning_rate * grad_omega;
        alpha_ += learning_rate * grad_alpha;
        beta_ += learning_rate * grad_beta;
        
        // Ensure parameters are positive and sum to less than 1
        omega_ = std::max(0.0, omega_);
        alpha_ = std::max(0.0, std::min(1.0, alpha_));
        beta_ = std::max(0.0, std::min(1.0, beta_));
        
        if (alpha_ + beta_ > 0.99) {
            double sum = alpha_ + beta_;
            alpha_ = (alpha_ / sum) * 0.99;
            beta_ = (beta_ / sum) * 0.99;
        }
    }
}

double GARCH::predict_volatility(int steps_ahead) const {
    if (steps_ahead <= 0) return std::sqrt(conditional_variances_.tail(1)(0));
    
    // Compute long-run variance
    double long_run_var = omega_ / (1.0 - alpha_ - beta_);
    
    // Compute predicted variance
    double predicted_var = long_run_var + 
        std::pow(alpha_ + beta_, steps_ahead) * 
        (conditional_variances_.tail(1)(0) - long_run_var);
    
    return std::sqrt(predicted_var);
}

double GARCH::get_omega() const { return omega_; }
double GARCH::get_alpha() const { return alpha_; }
double GARCH::get_beta() const { return beta_; }

Eigen::VectorXd GARCH::get_fitted_values() const {
    return fitted_values_;
}

Eigen::VectorXd GARCH::get_residuals() const {
    return residuals_;
}

double GARCH::calculate_likelihood() const {
    int n = residuals_.size();
    double log_likelihood = -0.5 * n * std::log(2 * M_PI);
    
    for (int t = 0; t < n; ++t) {
        log_likelihood -= 0.5 * (std::log(conditional_variances_(t)) + 
                                residuals_(t) * residuals_(t));
    }
    
    return log_likelihood;
}

double GARCH::get_aic() const {
    return -2 * calculate_likelihood() + 6; // 3 parameters
}

double GARCH::get_bic() const {
    int n = residuals_.size();
    return -2 * calculate_likelihood() + 3 * std::log(n);
}

} // namespace StatisticalModels
} // namespace finmath 