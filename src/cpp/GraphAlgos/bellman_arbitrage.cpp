#include "finmath/GraphAlgos/bellman_arbitrage.h"

#include <limits>
#include <algorithm>

template <typename Node>
std::vector<Node> detectArbitrageBellman(const AdjList<Node>& graph) {
    std::vector<Node> cycle;

    // contruct graph with negative log of edges (edge is transformation ratio)
    AdjList<Node> logGraph;
    for (const auto& [from, neighbors] : graph) {
        for (const auto& [to, rate] : neighbors) {
            if (rate <= 0.0) continue; // Avoid invalid exchange rates
            double logWeight = -std::log(rate);  // Apply -log(rate) to each edge weight
            logGraph[from].emplace_back(to, logWeight);
        }
    }

    std::unordered_map<Node, double> dist;
    std::unordered_map<Node, Node> parent;

    if (logGraph.empty()) {
        return cycle;
    }

    for (const auto& [node, _] : logGraph) {
        dist[node] = std::numeric_limits<double>::infinity();
        parent[node] = node;
    }

    Node start = logGraph.begin()->first;
    dist[start] = 0.0;

    int num_vertices = static_cast<int>(logGraph.size());

    for (int i = 0; i < num_vertices - 1; ++i) {
        for (const auto& [node, neighbors] : logGraph) {
            for (const auto& [next_node, weight] : neighbors) {
                if (dist[node] + weight < dist[next_node]) {
                    dist[next_node] = dist[node] + weight;
                    parent[next_node] = node;
                }
            }
        }
    }

    // Check for negative cycle
    for (const auto& [node, neighbors] : logGraph) {
        for (const auto& [next_node, weight] : neighbors) {
            if (dist[node] + weight < dist[next_node]) {
                // Found cycle
                Node curr_node = next_node;

                std::unordered_set<Node> seenNodes;

                while (seenNodes.find(curr_node) == seenNodes.end()) {
                    cycle.push_back(curr_node);
                    seenNodes.insert(curr_node);
                    curr_node = parent[curr_node];
                }


                cycle.push_back(curr_node);

                std::reverse(cycle.begin(), cycle.end());
                return cycle;
            }
        }
    }

    return cycle;
}

template std::vector<std::string> detectArbitrageBellman<std::string>(const AdjList<std::string>& graph);
