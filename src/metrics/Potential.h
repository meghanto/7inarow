#pragma once

#include "core/Board.h"
#include "core/Edges.h"
#include <array>
#include <cstdint>

namespace game {

// Histogram of l-lines for l = 1..7
using LLineHistogram = std::array<int32_t, 7>;

class PotentialCalculator {
public:
    PotentialCalculator(const Board& board, const std::vector<Hyperedge>& edges);
    
    // Compute potential: pot(b) = sum_{l=1..k} x_l * 2^{-(l-1)}
    double compute_potential() const;
    
    // Get histogram of l-lines
    LLineHistogram compute_histogram() const;
    
    // Check if Breaker has a winning certificate (pot < 1 on Breaker's turn)
    bool has_breaker_certificate() const;
    
private:
    const Board& board_;
    const std::vector<Hyperedge>& edges_;
    
    // Check if an edge is an l-line (no Breaker cells, exactly l empty cells)
    int32_t count_empty_cells(const Hyperedge& edge) const;
    bool has_breaker_cell(const Hyperedge& edge) const;
};

} // namespace game
