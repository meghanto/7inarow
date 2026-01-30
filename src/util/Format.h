#pragma once

#include "core/Board.h"
#include "core/Edges.h"
#include "metrics/Potential.h"
#include <string>

namespace game {

class Formatter {
public:
    // Format a cell as "(row,col)"
    static std::string format_cell(const Cell& cell);
    
    // Format a hyperedge as a list of cells
    static std::string format_edge(const Hyperedge& edge);
    
    // Format all edges in canonical form
    static std::string format_all_edges(const std::vector<Hyperedge>& edges);
    
    // Format histogram
    static std::string format_histogram(const LLineHistogram& hist);
    
    // Format potential value
    static std::string format_potential(double pot);
    
    // Format move statistics
    static std::string format_move_stats(int32_t move_num, Player player, const Cell& cell,
                                         double pot, bool maker_win, bool breaker_cert);
};

} // namespace game
