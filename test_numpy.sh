#!/bin/bash

# Quick script to run NumPy tests with the virtual environment

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_ROOT"

# Check if venv exists
if [ ! -d "venv" ]; then
    echo "Virtual environment not found. Running setup..."
    ./setup_venv.sh
fi

# Activate venv
source venv/bin/activate

# Run the NumPy tests
echo "Running rolling_volatility_np tests..."
python3 test/TimeSeries/RollingVolatility/Python/test_rolling_volatility_np.py

echo ""
echo "✅ Tests complete!"

