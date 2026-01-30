#include "util/Format.h"
#include <sstream>
#include <iomanip>

namespace game {

std::string Formatter::format_cell(const Cell& cell) {
    std::ostringstream oss;
    oss << "(" << cell.row << "," << cell.col << ")";
    return oss.str();
}

std::string Formatter::format_edge(const Hyperedge& edge) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < edge.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << format_cell(edge[i]);
    }
    oss << "]";
    return oss.str();
}

std::string Formatter::format_all_edges(const std::vector<Hyperedge>& edges) {
    std::ostringstream oss;
    oss << "Total edges: " << edges.size() << "\n";
    for (size_t i = 0; i < edges.size(); ++i) {
        oss << i + 1 << ". " << format_edge(edges[i]) << "\n";
    }
    return oss.str();
}

std::string Formatter::format_histogram(const LLineHistogram& hist) {
    std::ostringstream oss;
    oss << "L-line histogram:\n";
    for (int32_t l = 1; l <= 7; ++l) {
        oss << "  x_" << l << " = " << hist[static_cast<size_t>(l - 1)] << "\n";
    }
    return oss.str();
}

std::string Formatter::format_potential(double pot) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << pot;
    return oss.str();
}

std::string Formatter::format_move_stats(int32_t move_num, Player player, const Cell& cell,
                                         double pot, bool maker_win, bool breaker_cert) {
    std::ostringstream oss;
    oss << "Move " << move_num << ": ";
    oss << (player == Player::Maker ? "Maker" : "Breaker");
    oss << " -> " << format_cell(cell);
    oss << " | pot=" << format_potential(pot);
    oss << " | maker_win=" << (maker_win ? "true" : "false");
    oss << " | breaker_cert=" << (breaker_cert ? "true" : "false");
    return oss.str();
}

} // namespace game
