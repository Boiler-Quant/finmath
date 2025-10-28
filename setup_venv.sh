#!/bin/bash

# Setup virtual environment for Python testing
# This script creates a venv and installs NumPy for testing the C++ NumPy bindings

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_ROOT"

echo "Setting up Python virtual environment..."

# Create venv if it doesn't exist
if [ ! -d "venv" ]; then
    echo "Creating virtual environment..."
    python3 -m venv venv
fi

# Activate venv
echo "Activating virtual environment..."
source venv/bin/activate

# Upgrade pip
echo "Upgrading pip..."
pip install --upgrade pip --trusted-host pypi.org --trusted-host files.pythonhosted.org

# Install NumPy
echo "Installing NumPy..."
pip install numpy --trusted-host pypi.org --trusted-host files.pythonhosted.org

# Verify installation
echo ""
echo "Verifying installation..."
python3 -c "import numpy; print('✅ NumPy version:', numpy.__version__)"

echo ""
echo "✅ Virtual environment setup complete!"
echo ""
echo "To use it:"
echo "  source venv/bin/activate"
echo ""
echo "To run the NumPy tests:"
echo "  source venv/bin/activate"
echo "  python3 test/TimeSeries/RollingVolatility/Python/test_rolling_volatility_np.py"
echo ""
echo "To deactivate:"
echo "  deactivate"

