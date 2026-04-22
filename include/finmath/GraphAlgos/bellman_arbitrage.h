#pragma once

#include <unordered_map>
#include <vector>
#include <utility>
#include <cmath>
#include <unordered_set>

template <typename Node>
using AdjList = std::unordered_map<Node, std::vector<std::pair<Node, double>>>;

template <typename Node>
std::vector<Node> detectArbitrageBellman(const AdjList<Node>& graph) {
    using Edge = std::tuple<int,int,double>;

    std::unordered_map<Node,int> id;
    std::vector<Node> nodes;

    for (const auto& [u, edges] : graph) {
        if (!id.count(u)) {
            id[u] = nodes.size();
            nodes.push_back(u);
        }
        for (const auto& [v, w] : edges) {
            if (!id.count(v)) {
                id[v] = nodes.size();
                nodes.push_back(v);
            }
        }
    }

    int n = nodes.size();
    std::vector<Edge> edges;

    for (const auto& [u, adj] : graph) {
        for (const auto& [v, rate] : adj) {
            edges.emplace_back(id[u], id[v], -std::log(rate));
        }
    }

    std::vector<double> dist(n, 0.0);
    std::vector<int> parent(n, -1);
    int x = -1;

    for (int i = 0; i < n; i++) {
        x = -1;
        for (auto [u, v, w] : edges) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                x = v;
            }
        }
    }

    if (x == -1) return {};

    int y = x;
    for (int i = 0; i < n; i++) {
        y = parent[y];
    }

    std::vector<Node> cycle;
    for (int cur = y;; cur = parent[cur]) {
        cycle.push_back(nodes[cur]);
        if (cur == y && cycle.size() > 1) break;
    }

    std::reverse(cycle.begin(), cycle.end());
    return cycle;
}

