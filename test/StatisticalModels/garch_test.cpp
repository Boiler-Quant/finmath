#include <cassert>
#include <iostream>
#include <Eigen/Dense>
#include "finmath/StatisticalModels/garch.h"

int garch_tests() {
    // Test Case 1: Simple GARCH(1,1) with known parameters
    {
        // Generate synthetic data with known parameters
        int n = 1000;
        Eigen::VectorXd returns(n);
        double omega = 0.0001;
        double alpha = 0.1;
        double beta = 0.8;
        
        // Initialize first return
        returns(0) = 0.01;
        
        // Generate returns using GARCH(1,1) process
        for (int t = 1; t < n; ++t) {
            double sigma_t = std::sqrt(omega + alpha * returns(t-1) * returns(t-1) + beta * sigma_t * sigma_t);
            returns(t) = sigma_t * (rand() / double(RAND_MAX) - 0.5);
        }
        
        // Fit GARCH model
        finmath::StatisticalModels::GARCH garch;
        garch.fit(returns);
        
        // Check parameter estimates
        double omega_est = garch.get_omega();
        double alpha_est = garch.get_alpha();
        double beta_est = garch.get_beta();
        
        // Parameters should be close to true values
        assert(std::abs(omega_est - omega) < 0.0001);
        assert(std::abs(alpha_est - alpha) < 0.1);
        assert(std::abs(beta_est - beta) < 0.1);
        
        // Check volatility prediction
        double vol_pred = garch.predict_volatility(1);
        assert(vol_pred > 0);
    }
    
    // Test Case 2: Realistic financial returns
    {
        // Generate more realistic returns with volatility clustering
        int n = 2000;
        Eigen::VectorXd returns(n);
        
        // Initialize with random values
        for (int t = 0; t < n; ++t) {
            returns(t) = 0.01 * (rand() / double(RAND_MAX) - 0.5);
        }
        
        // Add some volatility clustering
        for (int t = 500; t < 1000; ++t) {
            returns(t) *= 2.0; // Higher volatility period
        }
        
        // Fit GARCH model
        finmath::StatisticalModels::GARCH garch;
        garch.fit(returns);
        
        // Check model diagnostics
        Eigen::VectorXd residuals = garch.get_residuals();
        double aic = garch.get_aic();
        double bic = garch.get_bic();
        
        // Basic checks
        assert(residuals.size() == n);
        assert(aic < 0); // AIC should be negative for good fit
        assert(bic < aic); // BIC should be more negative than AIC
        
        // Check volatility predictions
        double vol_1 = garch.predict_volatility(1);
        double vol_5 = garch.predict_volatility(5);
        double vol_10 = garch.predict_volatility(10);
        
        // Volatility predictions should be positive and decreasing with horizon
        assert(vol_1 > 0);
        assert(vol_5 > 0);
        assert(vol_10 > 0);
        assert(vol_1 >= vol_5);
        assert(vol_5 >= vol_10);
    }
    
    std::cout << "All GARCH Tests Passed!" << std::endl;
    return 0;
}

int main() {
    return garch_tests();
} 