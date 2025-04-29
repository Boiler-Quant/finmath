#ifndef GARCH_H
#define GARCH_H

#include <vector>
#include <Eigen/Dense>

namespace finmath {
namespace StatisticalModels {

class GARCH {
public:
    // Constructor with default parameters (GARCH(1,1))
    GARCH(double omega = 0.0001, double alpha = 0.1, double beta = 0.8);
    
    // Fit the model to data
    void fit(const Eigen::VectorXd& returns);
    
    // Predict volatility
    double predict_volatility(int steps_ahead = 1) const;
    
    // Get model parameters
    double get_omega() const;
    double get_alpha() const;
    double get_beta() const;
    
    // Get fitted values
    Eigen::VectorXd get_fitted_values() const;
    
    // Get residuals
    Eigen::VectorXd get_residuals() const;
    
    // Get AIC (Akaike Information Criterion)
    double get_aic() const;
    
    // Get BIC (Bayesian Information Criterion)
    double get_bic() const;

private:
    double omega_;  // Constant term
    double alpha_;  // ARCH parameter
    double beta_;   // GARCH parameter
    
    Eigen::VectorXd fitted_values_;
    Eigen::VectorXd residuals_;
    Eigen::VectorXd conditional_variances_;
    
    // Helper functions
    void update_parameters(const Eigen::VectorXd& returns);
    double calculate_likelihood() const;
};

} // namespace StatisticalModels
} // namespace finmath

#endif // GARCH_H 