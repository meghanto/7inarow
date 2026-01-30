#include "core/Board.h"
#include <stdexcept>
#include <sstream>

namespace game {

Board::Board(int32_t num_cols) 
    : num_cols_(num_cols)
    , cells_(4 * num_cols, CellState::Empty) {
    if (num_cols <= 0) {
        throw std::invalid_argument("Number of columns must be positive");
    }
}

CellState Board::get(int32_t row, int32_t col) const {
    if (!is_valid(row, col)) {
        throw std::out_of_range("Cell coordinates out of bounds");
    }
    return cells_[static_cast<size_t>(row * num_cols_ + col)];
}

CellState Board::get(const Cell& cell) const {
    return get(cell.row, cell.col);
}

bool Board::is_empty(int32_t row, int32_t col) const {
    return is_valid(row, col) && get(row, col) == CellState::Empty;
}

bool Board::is_empty(const Cell& cell) const {
    return is_empty(cell.row, cell.col);
}

bool Board::is_valid(int32_t row, int32_t col) const {
    return row >= 0 && row < 4 && col >= 0 && col < num_cols_;
}

bool Board::is_valid(const Cell& cell) const {
    return is_valid(cell.row, cell.col);
}

void Board::set(int32_t row, int32_t col, CellState state) {
    if (!is_valid(row, col)) {
        throw std::out_of_range("Cell coordinates out of bounds");
    }
    cells_[static_cast<size_t>(row * num_cols_ + col)] = state;
}

void Board::set(const Cell& cell, CellState state) {
    set(cell.row, cell.col, state);
}

std::vector<Cell> Board::get_empty_cells() const {
    std::vector<Cell> empty;
    for (int32_t r = 0; r < 4; ++r) {
        for (int32_t c = 0; c < num_cols_; ++c) {
            if (is_empty(r, c)) {
                empty.push_back({r, c});
            }
        }
    }
    return empty;
}

std::string Board::to_string() const {
    std::ostringstream oss;
    for (int32_t r = 0; r < 4; ++r) {
        for (int32_t c = 0; c < num_cols_; ++c) {
            CellState state = get(r, c);
            char ch = '.';
            if (state == CellState::Maker) ch = 'M';
            else if (state == CellState::Breaker) ch = 'B';
            oss << ch;
            if (c < num_cols_ - 1) oss << ' ';
        }
        oss << '\n';
    }
    return oss.str();
}

} // namespace game
