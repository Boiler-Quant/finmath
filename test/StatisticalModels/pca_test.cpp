#include <cassert>
#include <iostream>
#include <Eigen/Dense>
#include "finmath/StatisticalModels/pca.h"

int pca_tests() {
    // Test Case 1: Simple 2D data
    {
        Eigen::MatrixXd data(4, 2);
        data << 1, 2,
                2, 4,
                3, 6,
                4, 8;
        
        finmath::StatisticalModels::PCA pca(data, 1);
        
        // Check explained variance ratio
        Eigen::VectorXd var_ratio = pca.get_explained_variance_ratio();
        assert(var_ratio(0) > 0.99); // Should explain almost all variance
        
        // Check transformation and inverse transformation
        Eigen::MatrixXd transformed = pca.transform(data);
        Eigen::MatrixXd reconstructed = pca.inverse_transform(transformed);
        
        // Check reconstruction error
        double error = (data - reconstructed).norm() / data.norm();
        assert(error < 0.01);
    }
    
    // Test Case 2: Random 3D data
    {
        Eigen::MatrixXd data = Eigen::MatrixXd::Random(100, 3);
        finmath::StatisticalModels::PCA pca(data, 2);
        
        // Check number of components
        assert(pca.get_n_components() == 2);
        
        // Check explained variance ratio sums to less than 1
        Eigen::VectorXd var_ratio = pca.get_explained_variance_ratio();
        assert(var_ratio.sum() < 1.0);
        
        // Check transformation preserves dimensionality
        Eigen::MatrixXd transformed = pca.transform(data);
        assert(transformed.cols() == 2);
        assert(transformed.rows() == 100);
    }
    
    std::cout << "All PCA Tests Passed!" << std::endl;
    return 0;
}

int main() {
    return pca_tests();
} 