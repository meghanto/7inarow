#pragma once

#include "core/Board.h"
#include "core/Edges.h"
#include <optional>
#include <vector>

namespace game {

struct MoveResult {
    bool maker_wins;
    std::optional<Hyperedge> winning_edge;
};

class Game {
public:
    Game(int32_t num_cols, const std::vector<Hyperedge>& edges);
    
    const Board& board() const { return board_; }
    Board& board() { return board_; }
    
    Player current_player() const { return current_player_; }
    int32_t move_count() const { return move_count_; }
    
    // Make a move and return result
    MoveResult make_move(const Cell& cell);
    
    // Check if Maker has won
    bool check_maker_win() const;
    
    // Find the winning edge if Maker has won
    std::optional<Hyperedge> find_winning_edge() const;
    
    const std::vector<Hyperedge>& edges() const { return edges_; }
    
private:
    Board board_;
    std::vector<Hyperedge> edges_;
    Player current_player_;
    int32_t move_count_;
    
    // Check if an edge is fully occupied by Maker
    bool is_edge_complete(const Hyperedge& edge) const;
};

} // namespace game
