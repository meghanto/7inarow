#pragma once

#include "core/Board.h"
#include <vector>
#include <set>

namespace game {

// Hyperedge is a set of cells that form a winning line
using Hyperedge = std::vector<Cell>;

class EdgeGenerator {
public:
    // Generate all hyperedges for a (4, n, 7^tr) game
    static std::vector<Hyperedge> generate_edges(int32_t num_cols);
    
private:
    // Generate standard horizontal edges (length 7)
    static void add_horizontal_edges(std::vector<Hyperedge>& edges, int32_t num_cols);
    
    // Generate truncated horizontal edges at board boundaries
    static void add_truncated_horizontal_edges(std::vector<Hyperedge>& edges, int32_t num_cols);
    
    // Generate vertical edges (full column, length 4)
    static void add_vertical_edges(std::vector<Hyperedge>& edges, int32_t num_cols);
    
    // Generate diagonal edges (length 4)
    static void add_diagonal_edges(std::vector<Hyperedge>& edges, int32_t num_cols);
    
    // Helper to ensure edges are in canonical form (sorted)
    static void canonicalize(Hyperedge& edge);
};

} // namespace game
