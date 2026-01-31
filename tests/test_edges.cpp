#include "test_framework.h"
#include "core/Edges.h"
#include "core/Board.h"
#include <algorithm>

void test_edge_generation();

namespace {

bool has_edge(const std::vector<game::Hyperedge>& edges, const game::Hyperedge& target) {
    game::Hyperedge sorted_target = target;
    std::sort(sorted_target.begin(), sorted_target.end());
    return std::any_of(edges.begin(), edges.end(), [&](const auto& edge) {
        return edge == sorted_target;
    });
}

void test_edge_count_n10() {
    auto edges = game::EdgeGenerator::generate_edges(10);
    
    // For n=10, we expect:
    // - Horizontal length-7: 4 rows * 3 positions = 12
    // - Horizontal length-4 boundaries: 4 rows * 2 = 8
    // - Vertical: 10 columns = 10
    // - Diagonals: (10-4+1)*2 = 14 down-right and up-right
    // - Extra edges: 6
    
    // At minimum we should have > 45 edges
    if (edges.size() < 45) {
        test::TestRunner::instance().add_result({
            "test_edge_count_n10", false, 
            "Expected at least 45 edges, got " + std::to_string(edges.size())
        });
        return;
    }
    
    test::TestRunner::instance().add_result({"test_edge_count_n10", true, ""});
}

void test_edges_are_canonical() {
    auto edges = game::EdgeGenerator::generate_edges(8);
    
    for (const auto& edge : edges) {
        // Check if edge is sorted
        for (size_t i = 1; i < edge.size(); ++i) {
            if (edge[i] < edge[i-1]) {
                test::TestRunner::instance().add_result({
                    "test_edges_are_canonical", false, 
                    "Edge not in canonical form"
                });
                return;
            }
        }
    }
    
    test::TestRunner::instance().add_result({"test_edges_are_canonical", true, ""});
}

void test_vertical_edges_n7() {
    auto edges = game::EdgeGenerator::generate_edges(7);
    
    // Count vertical edges (length 4)
    int32_t vertical_count = 0;
    for (const auto& edge : edges) {
        if (edge.size() == 4) {
            // Check if it's vertical (same column)
            bool is_vertical = true;
            int32_t col = edge[0].col;
            for (size_t i = 1; i < edge.size(); ++i) {
                if (edge[i].col != col) {
                    is_vertical = false;
                    break;
                }
            }
            if (is_vertical) ++vertical_count;
        }
    }
    
    // Should have exactly 7 vertical edges for n=7
    if (vertical_count != 7) {
        test::TestRunner::instance().add_result({
            "test_vertical_edges_n7", false, 
            "Expected 7 vertical edges, got " + std::to_string(vertical_count)
        });
        return;
    }
    
    test::TestRunner::instance().add_result({"test_vertical_edges_n7", true, ""});
}

void test_edge_cells_in_bounds() {
    auto edges = game::EdgeGenerator::generate_edges(12);
    
    for (const auto& edge : edges) {
        for (const auto& cell : edge) {
            if (cell.row < 0 || cell.row >= 4 || cell.col < 0 || cell.col >= 12) {
                test::TestRunner::instance().add_result({
                    "test_edge_cells_in_bounds", false, 
                    "Cell out of bounds: (" + std::to_string(cell.row) + "," + std::to_string(cell.col) + ")"
                });
                return;
            }
        }
    }
    
    test::TestRunner::instance().add_result({"test_edge_cells_in_bounds", true, ""});
}

void test_extra_edges() {
    auto edges = game::EdgeGenerator::generate_edges(8);
    
    if (!has_edge(edges, {{2, 0}, {1, 1}, {0, 2}})) {
        test::TestRunner::instance().add_result({
            "test_extra_edges", false,
            "Missing extra edge {2,0}-{1,1}-{0,2}"
        });
        return;
    }
    
    if (!has_edge(edges, {{1, 0}, {0, 1}})) {
        test::TestRunner::instance().add_result({
            "test_extra_edges", false,
            "Missing extra edge {1,0}-{0,1}"
        });
        return;
    }
    
    if (!has_edge(edges, {{2, 5}, {1, 6}, {0, 7}})) {
        test::TestRunner::instance().add_result({
            "test_extra_edges", false,
            "Missing extra edge {2,5}-{1,6}-{0,7}"
        });
        return;
    }
    
    if (!has_edge(edges, {{1, 6}, {0, 7}})) {
        test::TestRunner::instance().add_result({
            "test_extra_edges", false,
            "Missing extra edge {1,6}-{0,7}"
        });
        return;
    }
    
    test::TestRunner::instance().add_result({"test_extra_edges", true, ""});
}

} // namespace

void test_edge_generation() {
    test_edge_count_n10();
    test_edges_are_canonical();
    test_vertical_edges_n7();
    test_edge_cells_in_bounds();
    test_extra_edges();
}
