#include "util/Cli.h"
#include <iostream>
#include <stdexcept>

namespace game {

CliArgs CliParser::parse(int argc, char* argv[]) {
    CliArgs args;
    
    if (argc < 2) {
        args.command = CliCommand::Help;
        return args;
    }
    
    std::string cmd(argv[1]);
    args.command = parse_command(cmd);
    
    // Parse additional arguments
    for (int i = 2; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-n" || arg == "--cols") {
            if (i + 1 < argc) {
                try {
                    args.num_cols = std::stoi(argv[++i]);
                } catch (const std::exception&) {
                    throw std::invalid_argument("Invalid value for " + arg + ": expected integer");
                }
            }
        } else if (arg == "-s" || arg == "--seed") {
            if (i + 1 < argc) {
                try {
                    args.seed = std::stoi(argv[++i]);
                } catch (const std::exception&) {
                    throw std::invalid_argument("Invalid value for " + arg + ": expected integer");
                }
            }
        } else if (arg == "-m" || arg == "--max-moves") {
            if (i + 1 < argc) {
                try {
                    args.max_moves = std::stoi(argv[++i]);
                } catch (const std::exception&) {
                    throw std::invalid_argument("Invalid value for " + arg + ": expected integer");
                }
            }
        }
    }
    
    return args;
}

CliCommand CliParser::parse_command(const std::string& cmd) {
    if (cmd == "print-edges") return CliCommand::PrintEdges;
    if (cmd == "simulate") return CliCommand::SimulateRandom;
    if (cmd == "potential") return CliCommand::ComputePotential;
    if (cmd == "help") return CliCommand::Help;
    
    throw std::invalid_argument("Unknown command: " + cmd);
}

void CliParser::print_help() {
    std::cout << "7-in-a-Row Maker-Breaker Game Harness\n\n";
    std::cout << "Usage: game <command> [options]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  print-edges   Print all hyperedges for the given board size\n";
    std::cout << "  simulate      Simulate a random game\n";
    std::cout << "  potential     Compute potential for an empty board\n";
    std::cout << "  help          Show this help message\n\n";
    std::cout << "Options:\n";
    std::cout << "  -n, --cols <N>        Number of columns (default: 10)\n";
    std::cout << "  -s, --seed <S>        Random seed (default: 42)\n";
    std::cout << "  -m, --max-moves <M>   Maximum moves (default: 100)\n";
}

} // namespace game
