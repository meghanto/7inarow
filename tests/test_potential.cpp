#include "test_framework.h"
#include "core/Board.h"
#include "core/Edges.h"
#include "core/Game.h"
#include "metrics/Potential.h"
#include <cmath>

void test_potential_calculation();

namespace {

void test_empty_board_potential() {
    game::Board board(10);
    auto edges = game::EdgeGenerator::generate_edges(10);
    game::PotentialCalculator calc(board, edges);
    
    double pot = calc.compute_potential();
    
    // Empty board should have high potential
    if (pot <= 0.0) {
        test::TestRunner::instance().add_result({
            "test_empty_board_potential", false, 
            "Empty board potential should be > 0"
        });
        return;
    }
    
    test::TestRunner::instance().add_result({"test_empty_board_potential", true, ""});
}

void test_histogram_counts() {
    game::Board board(8);
    auto edges = game::EdgeGenerator::generate_edges(8);
    game::PotentialCalculator calc(board, edges);
    
    auto hist = calc.compute_histogram();
    
    // On empty board, all edges should be counted in their respective l-line category
    int32_t total = 0;
    for (int32_t i = 0; i < 7; ++i) {
        total += hist[static_cast<size_t>(i)];
    }
    
    // Total should equal number of edges
    if (total != static_cast<int32_t>(edges.size())) {
        test::TestRunner::instance().add_result({
            "test_histogram_counts", false, 
            "Histogram total " + std::to_string(total) + " != edges count " + std::to_string(edges.size())
        });
        return;
    }
    
    test::TestRunner::instance().add_result({"test_histogram_counts", true, ""});
}

void test_breaker_reduces_potential() {
    auto edges = game::EdgeGenerator::generate_edges(7);
    game::Game g(7, edges);
    
    game::PotentialCalculator calc_initial(g.board(), edges);
    double pot_initial = calc_initial.compute_potential();
    
    // First move: Maker
    g.make_move({0, 3});
    
    game::PotentialCalculator calc_after_maker(g.board(), edges);
    double pot_after_maker = calc_after_maker.compute_potential();
    
    // Second move: Breaker
    g.make_move({1, 3});
    
    game::PotentialCalculator calc_after_breaker(g.board(), edges);
    double pot_after_breaker = calc_after_breaker.compute_potential();
    
    // Breaker's move should reduce potential more than or equal to Maker's
    // (In general, Breaker blocks edges while Maker advances them)
    if (pot_after_breaker >= pot_initial) {
        test::TestRunner::instance().add_result({
            "test_breaker_reduces_potential", false, 
            "After moves, potential should change from initial"
        });
        return;
    }
    
    test::TestRunner::instance().add_result({"test_breaker_reduces_potential", true, ""});
}

void test_potential_formula() {
    game::Board board(5);
    auto edges = game::EdgeGenerator::generate_edges(5);
    game::PotentialCalculator calc(board, edges);
    
    auto hist = calc.compute_histogram();
    double pot = calc.compute_potential();
    
    // Manually compute potential
    double expected = 0.0;
    for (int32_t l = 1; l <= 7; ++l) {
        int32_t x_l = hist[static_cast<size_t>(l - 1)];
        double weight = std::pow(2.0, -static_cast<double>(l - 1));
        expected += static_cast<double>(x_l) * weight;
    }
    
    // Should match within floating point precision
    if (std::abs(pot - expected) > 1e-9) {
        test::TestRunner::instance().add_result({
            "test_potential_formula", false, 
            "Potential mismatch: " + std::to_string(pot) + " != " + std::to_string(expected)
        });
        return;
    }
    
    test::TestRunner::instance().add_result({"test_potential_formula", true, ""});
}

} // namespace

void test_potential_calculation() {
    test_empty_board_potential();
    test_histogram_counts();
    test_breaker_reduces_potential();
    test_potential_formula();
}
