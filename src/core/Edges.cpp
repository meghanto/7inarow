#include "core/Edges.h"
#include <algorithm>

namespace game {

std::vector<Hyperedge> EdgeGenerator::generate_edges(int32_t num_cols) {
    std::vector<Hyperedge> edges;
    
    // Add horizontal edges (length 7 plus boundary length 4)
    add_horizontal_edges(edges, num_cols);
    
    // Add vertical edges (all columns, length 4)
    add_vertical_edges(edges, num_cols);
    
    // Add diagonal edges (length 4)
    add_diagonal_edges(edges, num_cols);
    
    // Add extra diagonal edges (length 3 and 2)
    add_extra_edges(edges, num_cols);
    
    // Canonicalize all edges
    for (auto& edge : edges) {
        canonicalize(edge);
    }
    
    // Sort and remove duplicates efficiently
    std::sort(edges.begin(), edges.end());
    edges.erase(std::unique(edges.begin(), edges.end()), edges.end());
    
    return edges;
}

void EdgeGenerator::add_horizontal_edges(std::vector<Hyperedge>& edges, int32_t num_cols) {
    // For each row, generate length-7 horizontal lines and boundary length-4 edges
    for (int32_t row = 0; row < 4; ++row) {
        // Left boundary: length 4 at columns 0..3
        if (num_cols >= 4) {
            Hyperedge edge;
            for (int32_t i = 0; i < 4; ++i) {
                edge.push_back({row, i});
            }
            edges.push_back(edge);
        }
        
        // Right boundary: length 4 at columns n-4..n-1
        if (num_cols >= 4) {
            Hyperedge edge;
            for (int32_t i = 0; i < 4; ++i) {
                edge.push_back({row, num_cols - 4 + i});
            }
            edges.push_back(edge);
        }
        
        // Interior length-7 edges start at column 1 through n-7 (1-indexed: 2..n-7)
        for (int32_t start_col = 1; start_col + 7 <= num_cols - 1; ++start_col) {
            Hyperedge edge;
            for (int32_t i = 0; i < 7; ++i) {
                edge.push_back({row, start_col + i});
            }
            edges.push_back(edge);
        }
    }
}

void EdgeGenerator::add_vertical_edges(std::vector<Hyperedge>& edges, int32_t num_cols) {
    // Every column forms a vertical edge of length 4
    for (int32_t col = 0; col < num_cols; ++col) {
        Hyperedge edge;
        for (int32_t row = 0; row < 4; ++row) {
            edge.push_back({row, col});
        }
        edges.push_back(edge);
    }
}

void EdgeGenerator::add_diagonal_edges(std::vector<Hyperedge>& edges, int32_t num_cols) {
    // Diagonals of length 4 (the board is 4 rows tall)
    
    // Down-right diagonals: starting from (0, col)
    for (int32_t start_col = 0; start_col + 4 <= num_cols; ++start_col) {
        Hyperedge edge;
        for (int32_t i = 0; i < 4; ++i) {
            edge.push_back({i, start_col + i});
        }
        edges.push_back(edge);
    }
    
    // Up-right diagonals: starting from (3, col)
    for (int32_t start_col = 0; start_col + 4 <= num_cols; ++start_col) {
        Hyperedge edge;
        for (int32_t i = 0; i < 4; ++i) {
            edge.push_back({3 - i, start_col + i});
        }
        edges.push_back(edge);
    }
}

void EdgeGenerator::add_extra_edges(std::vector<Hyperedge>& edges, int32_t num_cols) {
    if (num_cols < 3) {
        return;
    }
    
    // Extra length-3 diagonals at both ends
    edges.push_back({{2, 0}, {1, 1}, {0, 2}});
    edges.push_back({{1, 0}, {2, 1}, {3, 2}});
    edges.push_back({{2, num_cols - 3}, {1, num_cols - 2}, {0, num_cols - 1}});
    edges.push_back({{1, num_cols - 3}, {2, num_cols - 2}, {3, num_cols - 1}});
    
    // Extra length-2 diagonals at both ends
    edges.push_back({{1, 0}, {0, 1}});
    edges.push_back({{1, num_cols - 2}, {0, num_cols - 1}});
}

void EdgeGenerator::canonicalize(Hyperedge& edge) {
    std::sort(edge.begin(), edge.end());
}

} // namespace game
