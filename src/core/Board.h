#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace game {

enum class CellState : uint8_t {
    Empty = 0,
    Maker = 1,
    Breaker = 2
};

enum class Player : uint8_t {
    Maker = 0,
    Breaker = 1
};

struct Cell {
    int32_t row;
    int32_t col;
    
    bool operator==(const Cell& other) const {
        return row == other.row && col == other.col;
    }
    
    bool operator<(const Cell& other) const {
        if (row != other.row) return row < other.row;
        return col < other.col;
    }
};

class Board {
public:
    explicit Board(int32_t num_cols);
    
    int32_t rows() const { return 4; }
    int32_t cols() const { return num_cols_; }
    
    CellState get(int32_t row, int32_t col) const;
    CellState get(const Cell& cell) const;
    
    bool is_empty(int32_t row, int32_t col) const;
    bool is_empty(const Cell& cell) const;
    
    bool is_valid(int32_t row, int32_t col) const;
    bool is_valid(const Cell& cell) const;
    
    void set(int32_t row, int32_t col, CellState state);
    void set(const Cell& cell, CellState state);
    
    std::vector<Cell> get_empty_cells() const;
    
    std::string to_string() const;
    
private:
    int32_t num_cols_;
    std::vector<CellState> cells_;
};

} // namespace game
