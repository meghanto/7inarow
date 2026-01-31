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
    // Generate horizontal edges (length 7 plus boundary length 4)
    static void add_horizontal_edges(std::vector<Hyperedge>& edges, int32_t num_cols);
    
    // Generate vertical edges (full column, length 4)
    static void add_vertical_edges(std::vector<Hyperedge>& edges, int32_t num_cols);
    
    // Generate diagonal edges (length 4)
    static void add_diagonal_edges(std::vector<Hyperedge>& edges, int32_t num_cols);
    
    // Generate extra diagonal edges (length 3 and 2)
    static void add_extra_edges(std::vector<Hyperedge>& edges, int32_t num_cols);
    
    // Helper to ensure edges are in canonical form (sorted)
    static void canonicalize(Hyperedge& edge);
};

} // namespace game
