#include "finmath/StatisticalModels/pca.h"
#include <cmath>

namespace finmath {
namespace StatisticalModels {

PCA::PCA(const Eigen::MatrixXd& data, int num_components) : n_components_(num_components) {
    fit(data);
}

void PCA::fit(const Eigen::MatrixXd& data) {
    // Center the data
    mean_ = data.colwise().mean();
    Eigen::MatrixXd centered = data.rowwise() - mean_.transpose();
    
    // Scale the data
    std_ = ((centered.adjoint() * centered) / double(data.rows() - 1)).diagonal().array().sqrt();
    Eigen::MatrixXd scaled = centered.array().rowwise() / std_.transpose().array();
    
    // Compute covariance matrix
    Eigen::MatrixXd cov = (scaled.adjoint() * scaled) / double(data.rows() - 1);
    
    // Compute eigenvalues and eigenvectors
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigen_solver(cov);
    
    // Sort eigenvalues and eigenvectors in descending order
    Eigen::VectorXd eigenvalues = eigen_solver.eigenvalues().reverse();
    Eigen::MatrixXd eigenvectors = eigen_solver.eigenvectors().rowwise().reverse();
    
    // Set number of components if not specified
    if (n_components_ == 0) {
        n_components_ = data.cols();
    }
    
    // Store components and explained variance ratio
    components_ = eigenvectors.leftCols(n_components_);
    explained_variance_ratio_ = eigenvalues.head(n_components_) / eigenvalues.sum();
}

Eigen::MatrixXd PCA::get_components() const {
    return components_;
}

Eigen::VectorXd PCA::get_explained_variance_ratio() const {
    return explained_variance_ratio_;
}

Eigen::MatrixXd PCA::transform(const Eigen::MatrixXd& data) const {
    // Center and scale the data
    Eigen::MatrixXd centered = data.rowwise() - mean_.transpose();
    Eigen::MatrixXd scaled = centered.array().rowwise() / std_.transpose().array();
    
    // Project onto principal components
    return scaled * components_;
}

Eigen::MatrixXd PCA::inverse_transform(const Eigen::MatrixXd& transformed_data) const {
    // Reconstruct original data
    Eigen::MatrixXd reconstructed = transformed_data * components_.transpose();
    
    // Unscale and uncenter
    reconstructed = reconstructed.array().rowwise() * std_.transpose().array();
    reconstructed = reconstructed.rowwise() + mean_.transpose();
    
    return reconstructed;
}

int PCA::get_n_components() const {
    return n_components_;
}

Eigen::VectorXd PCA::get_mean() const {
    return mean_;
}

Eigen::VectorXd PCA::get_std() const {
    return std_;
}

} // namespace StatisticalModels
} // namespace finmath 