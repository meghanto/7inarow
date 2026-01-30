#include "core/Game.h"
#include <stdexcept>

namespace game {

Game::Game(int32_t num_cols, const std::vector<Hyperedge>& edges)
    : board_(num_cols)
    , edges_(edges)
    , current_player_(Player::Maker)
    , move_count_(0) {
}

MoveResult Game::make_move(const Cell& cell) {
    if (!board_.is_valid(cell)) {
        throw std::invalid_argument("Invalid cell coordinates");
    }
    if (!board_.is_empty(cell)) {
        throw std::invalid_argument("Cell is already occupied");
    }
    
    // Place the mark
    CellState state = (current_player_ == Player::Maker) ? CellState::Maker : CellState::Breaker;
    board_.set(cell, state);
    
    // Increment move count
    ++move_count_;
    
    // Check for Maker win
    MoveResult result;
    result.maker_wins = check_maker_win();
    if (result.maker_wins) {
        result.winning_edge = find_winning_edge();
    }
    
    // Switch player
    current_player_ = (current_player_ == Player::Maker) ? Player::Breaker : Player::Maker;
    
    return result;
}

bool Game::check_maker_win() const {
    for (const auto& edge : edges_) {
        if (is_edge_complete(edge)) {
            return true;
        }
    }
    return false;
}

std::optional<Hyperedge> Game::find_winning_edge() const {
    for (const auto& edge : edges_) {
        if (is_edge_complete(edge)) {
            return edge;
        }
    }
    return std::nullopt;
}

bool Game::is_edge_complete(const Hyperedge& edge) const {
    for (const auto& cell : edge) {
        if (board_.get(cell) != CellState::Maker) {
            return false;
        }
    }
    return true;
}

} // namespace game
