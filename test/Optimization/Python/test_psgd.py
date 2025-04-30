import pytest
import numpy as np
import finmath

# --- Test Setup ---

# Define a simple quadratic objective function: f(x) = 0.5 * ||x - target||^2
DIM = 5
TARGET_VECTOR = np.arange(DIM, dtype=np.float64)
NOISE_SIGMA = 0.2 # Std dev of noise added to *individual* gradient samples
BATCH_SIZE = 16

def objective_quadratic(x):
    diff = np.array(x) - TARGET_VECTOR
    return 0.5 * np.dot(diff, diff)

def stochastic_grad_quadratic(x):
    true_grad = np.array(x) - TARGET_VECTOR
    # Simulate noise averaged over a batch
    noise = np.random.normal(scale=NOISE_SIGMA, size=DIM) / np.sqrt(BATCH_SIZE)
    return (true_grad + noise).tolist() # Return as list, as expected by C++ std::vector

# Problem parameters for the quadratic function
# f(x) = 0.5 * ||x||^2 => grad = x, H = I. ell=1, rho=0.
# f(x) = 0.5 * ||x-t||^2 => grad = x-t, H = I. ell=1, rho=0.
ELL = 1.0
RHO = 1e-6 # Using a small proxy for rho as the true value is 0, which breaks the formula

# --- Tests ---

def test_psgd_quadratic_convergence():
    """Test if PSGD converges to the minimum of a simple quadratic."""
    x0 = np.zeros(DIM, dtype=np.float64)
    
    # Need to provide sigma used in *threshold calculation* (passed to C++ func)
    # This is based on Var[g_i(x)] = sigma^2, so Var[batch_grad] = sigma^2 / batch_size.
    # The parameter sigma in the function is sqrt(Var[g_i(x)]).
    sigma_param = NOISE_SIGMA 

    final_x = finmath.perturbed_sgd(
        stochastic_grad=stochastic_grad_quadratic,
        objective_f=objective_quadratic,
        x0=x0.tolist(), # Pass as list
        ell=ELL,
        rho=RHO,
        eps=1e-4,      # Target accuracy for grad norm
        sigma=sigma_param, # Std dev estimate for individual gradients
        delta=0.1,
        batch_size=BATCH_SIZE,
        step_size_coeff=0.05, # Reduced step size coeff
        ema_beta=0.9,
        max_iters=50000, # Increased iterations
        grad_clip_norm=100.0,
        param_clip_norm=100.0 
    )

    final_x_np = np.array(final_x)
    print(f"Final x: {final_x_np}")
    print(f"Target:  {TARGET_VECTOR}")
    np.testing.assert_allclose(final_x_np, TARGET_VECTOR, rtol=1e-2, atol=5e-2)

def test_psgd_parameter_validation():
    """Test if PSGD raises errors for invalid input parameters."""
    x0 = np.zeros(DIM).tolist()
    with pytest.raises(ValueError): # Changed from std::invalid_argument in pybind11
        finmath.perturbed_sgd(stochastic_grad_quadratic, objective_quadratic, x0, ell=0, rho=RHO)
    with pytest.raises(ValueError):
        finmath.perturbed_sgd(stochastic_grad_quadratic, objective_quadratic, x0, ell=ELL, rho=0)
    with pytest.raises(ValueError):
        finmath.perturbed_sgd(stochastic_grad_quadratic, objective_quadratic, x0, ell=ELL, rho=RHO, eps=0)
    with pytest.raises(ValueError):
        finmath.perturbed_sgd(stochastic_grad_quadratic, objective_quadratic, x0, ell=ELL, rho=RHO, sigma=-0.1)
    with pytest.raises(ValueError):
        finmath.perturbed_sgd(stochastic_grad_quadratic, objective_quadratic, x0, ell=ELL, rho=RHO, delta=0)
    with pytest.raises(ValueError):
        finmath.perturbed_sgd(stochastic_grad_quadratic, objective_quadratic, x0, ell=ELL, rho=RHO, batch_size=0)
    with pytest.raises(ValueError):
        finmath.perturbed_sgd(stochastic_grad_quadratic, objective_quadratic, x0, ell=ELL, rho=RHO, step_size_coeff=0)
    with pytest.raises(ValueError):
        finmath.perturbed_sgd(stochastic_grad_quadratic, objective_quadratic, x0, ell=ELL, rho=RHO, ema_beta=1.0)
    with pytest.raises(ValueError):
        finmath.perturbed_sgd(stochastic_grad_quadratic, objective_quadratic, x0, ell=ELL, rho=RHO, max_iters=0) 