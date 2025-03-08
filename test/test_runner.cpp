#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

// This file serves as a coordinator to run all individual test executables
int main()
{
    std::cout << "Running all tests...\n";

    std::vector<std::string> testExecutables = {
        "./compound_interest_test",
        "./black_scholes_test",
        "./binomial_option_pricing_test",
        "./rsi_test"};

    int failedTests = 0;

    for (const auto &testExe : testExecutables)
    {
        std::cout << "Running " << testExe << "...\n";
        int result = std::system(testExe.c_str());
        if (result != 0)
        {
            std::cout << "Test failed: " << testExe << std::endl;
            failedTests++;
        }
    }

    if (failedTests == 0)
    {
        std::cout << "All tests passed successfully!\n";
        return 0;
    }
    else
    {
        std::cout << failedTests << " test(s) failed.\n";
        return 1;
    }
}
