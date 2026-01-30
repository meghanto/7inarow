#include "core/Board.h"
#include "core/Edges.h"
#include "core/Game.h"
#include "metrics/Potential.h"
#include "util/Cli.h"
#include "util/Format.h"
#include <iostream>
#include <random>

void print_edges_command(int32_t num_cols) {
    auto edges = game::EdgeGenerator::generate_edges(num_cols);
    std::cout << game::Formatter::format_all_edges(edges);
}

void compute_potential_command(int32_t num_cols) {
    game::Board board(num_cols);
    auto edges = game::EdgeGenerator::generate_edges(num_cols);
    game::PotentialCalculator calc(board, edges);
    
    auto hist = calc.compute_histogram();
    double pot = calc.compute_potential();
    
    std::cout << game::Formatter::format_histogram(hist);
    std::cout << "Potential: " << game::Formatter::format_potential(pot) << "\n";
}

void simulate_random_game(int32_t num_cols, int32_t seed, int32_t max_moves) {
    std::mt19937 rng(static_cast<uint32_t>(seed));
    
    auto edges = game::EdgeGenerator::generate_edges(num_cols);
    game::Game g(num_cols, edges);
    
    std::cout << "Starting random simulation with n=" << num_cols 
              << ", seed=" << seed << "\n\n";
    
    for (int32_t move = 1; move <= max_moves; ++move) {
        auto empty = g.board().get_empty_cells();
        if (empty.empty()) {
            std::cout << "Board full, game ends in draw.\n";
            break;
        }
        
        // Pick random empty cell
        std::uniform_int_distribution<size_t> dist(0, empty.size() - 1);
        game::Cell cell = empty[dist(rng)];
        
        game::Player current = g.current_player();
        
        // Make move
        auto result = g.make_move(cell);
        
        // Compute potential after move
        game::PotentialCalculator calc(g.board(), edges);
        double pot = calc.compute_potential();
        bool breaker_cert = (g.current_player() == game::Player::Breaker) && calc.has_breaker_certificate();
        
        // Print stats
        std::cout << game::Formatter::format_move_stats(move, current, cell, pot, 
                                                        result.maker_wins, breaker_cert) << "\n";
        
        if (result.maker_wins) {
            std::cout << "\nMaker wins! Winning edge: ";
            if (result.winning_edge) {
                std::cout << game::Formatter::format_edge(*result.winning_edge);
            }
            std::cout << "\n";
            break;
        }
        
        if (breaker_cert) {
            std::cout << "\nBreaker has winning certificate (pot < 1)!\n";
        }
    }
    
    std::cout << "\nFinal board:\n";
    std::cout << g.board().to_string();
}

int main(int argc, char* argv[]) {
    try {
        game::CliArgs args = game::CliParser::parse(argc, argv);
        
        switch (args.command) {
            case game::CliCommand::PrintEdges:
                print_edges_command(args.num_cols);
                break;
            case game::CliCommand::SimulateRandom:
                simulate_random_game(args.num_cols, args.seed, args.max_moves);
                break;
            case game::CliCommand::ComputePotential:
                compute_potential_command(args.num_cols);
                break;
            case game::CliCommand::Help:
                game::CliParser::print_help();
                break;
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
