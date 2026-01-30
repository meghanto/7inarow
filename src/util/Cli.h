#pragma once

#include <string>
#include <vector>

namespace game {

enum class CliCommand {
    PrintEdges,
    SimulateRandom,
    ComputePotential,
    Help
};

struct CliArgs {
    CliCommand command;
    int32_t num_cols = 10;
    int32_t seed = 42;
    int32_t max_moves = 100;
};

class CliParser {
public:
    static CliArgs parse(int argc, char* argv[]);
    static void print_help();
    
private:
    static CliCommand parse_command(const std::string& cmd);
};

} // namespace game
