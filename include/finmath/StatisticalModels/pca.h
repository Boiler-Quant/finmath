#ifndef PCA_H
#define PCA_H

#include <vector>
#include <Eigen/Dense>

namespace finmath {
namespace StatisticalModels {

class PCA {
public:
    PCA(const Eigen::MatrixXd& data, int num_components = 0);
    
    // Get principal components
    Eigen::MatrixXd get_components() const;
    
    // Get explained variance ratio
    Eigen::VectorXd get_explained_variance_ratio() const;
    
    // Transform data to principal component space
    Eigen::MatrixXd transform(const Eigen::MatrixXd& data) const;
    
    // Inverse transform from principal component space
    Eigen::MatrixXd inverse_transform(const Eigen::MatrixXd& transformed_data) const;
    
    // Get number of components
    int get_n_components() const;
    
    // Get mean of original data
    Eigen::VectorXd get_mean() const;
    
    // Get standard deviation of original data
    Eigen::VectorXd get_std() const;

private:
    Eigen::MatrixXd components_;
    Eigen::VectorXd explained_variance_ratio_;
    Eigen::VectorXd mean_;
    Eigen::VectorXd std_;
    int n_components_;
    
    void fit(const Eigen::MatrixXd& data);
};

} // namespace StatisticalModels
} // namespace finmath

#endif // PCA_H 