#include "core/Edges.h"
#include <algorithm>

namespace game {

std::vector<Hyperedge> EdgeGenerator::generate_edges(int32_t num_cols) {
    std::vector<Hyperedge> edges;
    
    // Add standard horizontal edges (length 7)
    add_horizontal_edges(edges, num_cols);
    
    // Add truncated edges (length 4-6 at boundaries)
    add_truncated_horizontal_edges(edges, num_cols);
    
    // Add vertical edges (all columns, length 4)
    add_vertical_edges(edges, num_cols);
    
    // Add diagonal edges (length 4)
    add_diagonal_edges(edges, num_cols);
    
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
    // For each row, generate all length-7 horizontal lines
    for (int32_t row = 0; row < 4; ++row) {
        for (int32_t start_col = 0; start_col + 7 <= num_cols; ++start_col) {
            Hyperedge edge;
            for (int32_t i = 0; i < 7; ++i) {
                edge.push_back({row, start_col + i});
            }
            edges.push_back(edge);
        }
    }
}

void EdgeGenerator::add_truncated_horizontal_edges(std::vector<Hyperedge>& edges, int32_t num_cols) {
    // According to the (4,n,7^tr) definition, we include truncated edges at boundaries
    // These are lines of length 4-6 at the left and right boundaries
    
    for (int32_t row = 0; row < 4; ++row) {
        // Left boundary: length 4-6 starting from column 0
        for (int32_t len = 4; len <= 6 && len <= num_cols; ++len) {
            Hyperedge edge;
            for (int32_t i = 0; i < len; ++i) {
                edge.push_back({row, i});
            }
            edges.push_back(edge);
        }
        
        // Right boundary: length 4-6 ending at last column
        for (int32_t len = 4; len <= 6 && len <= num_cols; ++len) {
            Hyperedge edge;
            for (int32_t i = 0; i < len; ++i) {
                edge.push_back({row, num_cols - len + i});
            }
            // Only add if it doesn't overlap with full length-7 edges or left boundary
            if (num_cols - len >= 7 || num_cols < 7) {
                edges.push_back(edge);
            }
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

void EdgeGenerator::canonicalize(Hyperedge& edge) {
    std::sort(edge.begin(), edge.end());
}

} // namespace game
