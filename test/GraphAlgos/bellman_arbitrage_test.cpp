#include <algorithm>
#include <cassert>
#include "finmath/GraphAlgos/bellman_arbitrage.h"

static bool contains(const std::vector<std::string>& v, const std::string& x) {
    return std::find(v.begin(), v.end(), x) != v.end();
}

int bellman_arbitrage_tests() {

    {
        AdjList<std::string> adjList = {
            {"USD", {{"EUR", 0.9}}},
            {"EUR", {{"USD", 1.2}}}
        };
        auto cycle = detectArbitrageBellman(adjList);
        std::vector<std::string> expected = {"USD", "EUR", "USD"};
        
        assert(cycle.size() == expected.size());
        for (const auto& n : expected) {
            assert(contains(cycle, n));
        }
    }

    {
        AdjList<std::string> adjList = {
            {"A", {{"B", 1.1}, {"D", 0.5}}},
            {"B", {{"C", 1.05}, {"A", 0.7}}},
            {"C", {{"A", 0.9}, {"E", 0.3}}},
            {"D", {{"C", 1.0}}},
            {"E", {{"B", 0.2}}}
        };
    
        auto cycle = detectArbitrageBellman(adjList);
        std::vector<std::string> expected = {"A", "B", "C", "A"};
    
        assert(cycle.size() == expected.size());
        for (const auto& n : expected) {
            assert(contains(cycle, n));
        }
    }

    {
        AdjList<std::string> adjList = {
            {"USD", {{"EUR", 0.9}}},
            {"EUR", {{"JPY", 130}}},
            {"JPY", {{"USD", 0.006}}}
        };
        
        assert(detectArbitrageBellman(adjList).empty());
    }

    {
        AdjList<std::string> adjList = {
            {"A", {{"B", -1.0}, {"C", 2.0}}},
            {"C", {{"A", 0.4}}}
        };
        
        assert(detectArbitrageBellman(adjList).empty());
    }

    return 0;
}

int main() {
    return bellman_arbitrage_tests();
}
