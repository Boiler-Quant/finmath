#include "finmath/GraphAlgos/bellman_arbitrage.h"

#include <limits>
#include <algorithm>

// https://cp-algorithms.com/graph/finding-negative-cycle-in-graph.html

template <typename Node>
std::vector<Node> detectArbitrageBellman(const AdjList<Node>& graph) {
    std::vector<Node> cycle;

    // get modified graph which has edge weights of negative log of original graph's edge weights
    AdjList<Node> logGraph;
    for (const auto& [from, neighbors] : graph) {
        for (const auto& [to, rate] : neighbors) {
            if (rate <= 0.0) {
                continue;
            }
            double weight = -std::log(rate);
            logGraph[from].emplace_back(to, weight);
        }
    }

    if (logGraph.empty()) {
        return cycle;
    }

    std::unordered_map<Node, double> dist;
    std::unordered_map<Node, Node> parent;

    for (const auto& [node, _] : logGraph) {
        dist[node] = 0.0;  // Start all at 0 to catch any negative cycle
        parent[node] = node;
    }

    int numVertices = static_cast<int>(logGraph.size());

    // relax edges
    Node lastUpdated;
    for (int i = 0; i < numVertices; ++i) {
        lastUpdated = Node();  // reset
        bool anyUpdate = false;

        for (const auto& [node, neighbors] : logGraph) {
            for (const auto& [next_node, weight] : neighbors) {
                if (dist[node] + weight < dist[next_node]) {
                    dist[next_node] = dist[node] + weight;
                    parent[next_node] = node;
                    lastUpdated = next_node;
                    anyUpdate = true;
                }
            }
        }

        if (!anyUpdate) {
            return cycle;
        }
    }

    // negative weight cycle if we can still relax an edge
    if (lastUpdated != Node()) {
        Node cycleStart = lastUpdated;

        for (int i = 0; i < numVertices; ++i) {
            cycleStart = parent[cycleStart];
        }

        Node curr = cycleStart;

        do {
            cycle.push_back(curr);
            curr = parent[curr];
        } while (curr != cycleStart);

        cycle.push_back(cycleStart);
        std::reverse(cycle.begin(), cycle.end());

        return cycle;
    }

    return cycle;
}

template std::vector<std::string> detectArbitrageBellman<std::string>(const AdjList<std::string>& graph);
