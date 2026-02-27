#pragma once

#include <unordered_map>
#include <vector>
#include <utility>
#include <cmath>
#include <unordered_set>

template <typename Node>
using AdjList = std::unordered_map<Node, std::vector<std::pair<Node, double>>>;

template <typename Node>
std::vector<Node> detectArbitrageBellman(const AdjList<Node>& graph);

