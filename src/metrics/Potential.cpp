#include "metrics/Potential.h"
#include <cmath>

namespace game {

PotentialCalculator::PotentialCalculator(const Board& board, const std::vector<Hyperedge>& edges)
    : board_(board)
    , edges_(edges) {
}

double PotentialCalculator::compute_potential() const {
    LLineHistogram hist = compute_histogram();
    double pot = 0.0;
    
    // pot(b) = sum_{l=1..k} x_l * 2^{-(l-1)}
    for (int32_t l = 1; l <= 7; ++l) {
        int32_t x_l = hist[static_cast<size_t>(l - 1)];
        double weight = std::pow(2.0, -static_cast<double>(l - 1));
        pot += static_cast<double>(x_l) * weight;
    }
    
    return pot;
}

LLineHistogram PotentialCalculator::compute_histogram() const {
    LLineHistogram hist = {0, 0, 0, 0, 0, 0, 0};
    
    for (const auto& edge : edges_) {
        // An l-line has no Breaker cells and exactly l empty cells
        if (!has_breaker_cell(edge)) {
            int32_t empty_count = count_empty_cells(edge);
            if (empty_count >= 1 && empty_count <= 7) {
                ++hist[static_cast<size_t>(empty_count - 1)];
            }
        }
    }
    
    return hist;
}

bool PotentialCalculator::has_breaker_certificate() const {
    return compute_potential() < 1.0;
}

int32_t PotentialCalculator::count_empty_cells(const Hyperedge& edge) const {
    int32_t count = 0;
    for (const auto& cell : edge) {
        if (board_.is_empty(cell)) {
            ++count;
        }
    }
    return count;
}

bool PotentialCalculator::has_breaker_cell(const Hyperedge& edge) const {
    for (const auto& cell : edge) {
        if (board_.get(cell) == CellState::Breaker) {
            return true;
        }
    }
    return false;
}

} // namespace game
