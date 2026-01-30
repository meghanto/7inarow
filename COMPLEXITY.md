# Complexity Analysis: 7-in-a-Row Maker-Breaker Game

This document provides a detailed time and space complexity analysis of all major operations in the (4,n,7^tr) game implementation.

## Notation

- `n` = number of columns in the board (variable)
- `m` = number of cells = `4n` (always 4 rows)
- `E` = number of hyperedges ≈ `O(n)` (see analysis below)
- `k` = maximum edge length = 7 (constant)

## 1. Edge Generation (`EdgeGenerator::generate_edges`)

### Time Complexity: **O(n log n)**

The edge generation process consists of several steps:

#### 1.1 Horizontal Edges (Length 7)
```cpp
for (int32_t row = 0; row < 4; ++row) {
    for (int32_t start_col = 0; start_col + 7 <= num_cols; ++start_col) {
        // Create edge of length 7
    }
}
```
- **Iterations**: 4 rows × max(0, n-6) positions = `O(n)`
- **Work per iteration**: Creating 7-cell edge = `O(1)` (constant)
- **Total**: `O(n)`

#### 1.2 Truncated Horizontal Edges (Length 4-6)
```cpp
for (int32_t row = 0; row < 4; ++row) {
    for (int32_t len = 4; len <= 6; ++len) {
        // Left boundary edges
        // Right boundary edges (conditional)
    }
}
```
- **Iterations**: 4 rows × 3 lengths × 2 boundaries = `O(1)` (constant)
- **Total**: `O(1)`

#### 1.3 Vertical Edges
```cpp
for (int32_t col = 0; col < num_cols; ++col) {
    // Create edge of length 4
}
```
- **Iterations**: n columns
- **Work per iteration**: Creating 4-cell edge = `O(1)`
- **Total**: `O(n)`

#### 1.4 Diagonal Edges
```cpp
for (int32_t start_col = 0; start_col + 4 <= num_cols; ++start_col) {
    // Down-right diagonal
}
for (int32_t start_col = 0; start_col + 4 <= num_cols; ++start_col) {
    // Up-right diagonal
}
```
- **Iterations**: 2 × max(0, n-3) = `O(n)`
- **Work per iteration**: Creating 4-cell edge = `O(1)`
- **Total**: `O(n)`

#### 1.5 Canonicalization
```cpp
for (auto& edge : edges) {
    canonicalize(edge);  // std::sort on edge
}
```
- **Number of edges**: E ≈ `4(n-6) + ~12 + n + 2(n-3)` = `O(n)`
- **Sort per edge**: `O(k log k)` where k ≤ 7, so `O(1)`
- **Total**: `O(n)`

#### 1.6 Deduplication
```cpp
std::sort(edges.begin(), edges.end());
edges.erase(std::unique(edges.begin(), edges.end()), edges.end());
```
- **Sort**: `O(E log E)` = `O(n log n)` since E = `O(n)`
- **Unique**: `O(E)` = `O(n)`
- **Total**: `O(n log n)`

**Overall Edge Generation**: `O(n) + O(1) + O(n) + O(n) + O(n) + O(n log n)` = **O(n log n)**

### Space Complexity: **O(n)**
- Stores E edges, each with at most 7 cells
- Total: `O(E × k)` = `O(n × 1)` = `O(n)`

### Edge Count Formula
For board size n:
- Horizontal (length 7): `4(n-6)` for n ≥ 7, else 0
- Truncated horizontal: approximately 12-16 edges (constant)
- Vertical: `n`
- Diagonals: `2(n-3)` for n ≥ 4, else 0

**Total edges E ≈ 4(n-6) + 12 + n + 2(n-3) = 7n - 30** for large n, so **E = Θ(n)**

## 2. Board Operations

### 2.1 Board Construction: **O(n)**
```cpp
Board::Board(int32_t num_cols) 
    : cells_(4 * num_cols, CellState::Empty)
```
- Allocates and initializes `4n` cells
- **Time**: `O(n)`
- **Space**: `O(n)`

### 2.2 Cell Access (get/set): **O(1)**
```cpp
CellState Board::get(int32_t row, int32_t col) const {
    return cells_[row * num_cols_ + col];
}
```
- Direct array indexing
- **Time**: `O(1)`

### 2.3 Get Empty Cells: **O(n)**
```cpp
std::vector<Cell> Board::get_empty_cells() const {
    for (int32_t r = 0; r < 4; ++r) {
        for (int32_t c = 0; c < num_cols_; ++c) {
            if (is_empty(r, c)) {
                empty.push_back({r, c});
            }
        }
    }
}
```
- Iterates through all `4n` cells
- **Time**: `O(n)`
- **Space**: `O(n)` for result vector

### 2.4 Board to String: **O(n)**
```cpp
std::string Board::to_string() const {
    for (int32_t r = 0; r < 4; ++r) {
        for (int32_t c = 0; c < num_cols_; ++c) {
            // Add character to string
        }
    }
}
```
- Iterates through all `4n` cells
- **Time**: `O(n)`
- **Space**: `O(n)` for result string

## 3. Potential Calculation

### 3.1 Compute Histogram: **O(En) = O(n²)**
```cpp
LLineHistogram PotentialCalculator::compute_histogram() const {
    for (const auto& edge : edges_) {
        if (!has_breaker_cell(edge)) {
            int32_t empty_count = count_empty_cells(edge);
            // Update histogram
        }
    }
}
```
- **Outer loop**: E edges = `O(n)`
- **Per edge**:
  - `has_breaker_cell`: Checks up to k=7 cells = `O(1)`
  - `count_empty_cells`: Checks up to k=7 cells = `O(1)`
- **Total**: `O(E × 1)` = `O(n)`

**Wait, let me reconsider**: Each edge has up to 7 cells (constant), so checking each cell is O(1). With E edges, total is `O(E)` = `O(n)`.

### 3.2 Compute Potential: **O(n)**
```cpp
double PotentialCalculator::compute_potential() const {
    LLineHistogram hist = compute_histogram();  // O(n)
    for (int32_t l = 1; l <= 7; ++l) {          // O(1)
        pot += hist[l-1] * weight;
    }
}
```
- Histogram computation: `O(n)`
- Sum over k=7 values: `O(1)`
- **Total**: `O(n)`

### 3.3 Breaker Certificate Check: **O(n)**
- Calls `compute_potential()` which is `O(n)`

## 4. Game Operations

### 4.1 Game Construction: **O(n log n)**
```cpp
Game::Game(int32_t num_cols, const std::vector<Hyperedge>& edges)
    : board_(num_cols)      // O(n)
    , edges_(edges)         // O(n) copy
```
- Board construction: `O(n)`
- Edge copy: `O(n)` (E edges, each with constant size)
- **Total**: `O(n)`

**Note**: If edges are generated during construction, add `O(n log n)` for generation.

### 4.2 Make Move: **O(n)** without win check, **O(n)** with win check
```cpp
MoveResult Game::make_move(const Cell& cell) {
    board_.set(cell, state);           // O(1)
    result.maker_wins = check_maker_win(); // O(n)
}
```
- Cell validation and update: `O(1)`
- Win check: `O(n)` (see below)
- **Total**: `O(n)`

### 4.3 Check Maker Win: **O(n)**
```cpp
bool Game::check_maker_win() const {
    for (const auto& edge : edges_) {
        if (is_edge_complete(edge)) {  // O(1) per edge
            return true;
        }
    }
}
```
- Iterates through E edges
- Per edge: checks up to k=7 cells = `O(1)`
- **Total**: `O(E)` = `O(n)`

### 4.4 Find Winning Edge: **O(n)**
- Same analysis as `check_maker_win`
- **Total**: `O(n)`

## 5. Complete Game Simulation

### Single Move in Simulation: **O(n)**
```cpp
// From main.cpp simulate_random_game
auto empty = g.board().get_empty_cells();              // O(n)
game::Cell cell = empty[dist(rng)];                    // O(1)
auto result = g.make_move(cell);                       // O(n)
game::PotentialCalculator calc(g.board(), edges);      // O(1)
double pot = calc.compute_potential();                 // O(n)
```
- Get empty cells: `O(n)`
- Make move (with win check): `O(n)`
- Compute potential: `O(n)`
- **Total per move**: `O(n)`

### Complete Game (up to 4n moves): **O(n²)**
- Maximum moves: 4n (all cells filled)
- Per move: `O(n)`
- **Total**: `O(n × n)` = `O(n²)`

## 6. Overall Space Complexity

### Memory Usage Summary
| Component | Space Complexity | Description |
|-----------|-----------------|-------------|
| Board | `O(n)` | 4n cells |
| Edge list | `O(n)` | E ≈ 7n edges, each ≤ 7 cells |
| Game state | `O(n)` | Board + edges |
| Potential histogram | `O(1)` | Fixed 7 counters |
| Empty cells list | `O(n)` | Up to 4n cells |

**Total Space**: `O(n)`

## 7. Practical Performance Characteristics

### For typical board sizes:

| n | Edges (E) | Cells (m) | Edge Gen | Single Move | Full Game |
|---|-----------|-----------|----------|-------------|-----------|
| 5 | ~21 | 20 | < 1ms | < 1ms | ~20ms |
| 10 | ~52 | 40 | < 1ms | < 1ms | ~40ms |
| 20 | ~112 | 80 | < 1ms | < 1ms | ~160ms |
| 50 | ~322 | 200 | ~1ms | ~1ms | ~1s |
| 100 | ~672 | 400 | ~3ms | ~2ms | ~4s |

**Note**: These are rough estimates. Actual performance depends on:
- Compiler optimizations
- Hardware (CPU speed, cache)
- Early termination (game ends before filling board)

## 8. Optimization Opportunities

### Current Optimizations Already Implemented
1. **Edge deduplication**: Changed from `O(n²)` set-based to `O(n log n)` sort+unique
2. **Constant-size edges**: All edges have ≤ 7 cells, making per-edge operations O(1)
3. **Early termination**: Win check stops on first complete edge
4. **Direct array access**: Board uses flat array with O(1) indexing

### Potential Future Optimizations
1. **Incremental potential**: Update potential incrementally instead of full recalculation
   - Current: `O(n)` per potential check
   - Optimized: `O(1)` average case with bookkeeping
   
2. **Affected edges tracking**: Only check edges containing the last move
   - Current: `O(n)` to check all edges for win
   - Optimized: `O(1)` expected (only ~7-10 edges per cell)
   
3. **Bitset representation**: Use bitsets for edge states
   - Current: `O(k)` to check edge completion
   - Optimized: `O(1)` with bitwise AND

4. **Precomputed edge-cell mapping**: Build inverse index
   - Space: `O(n)` additional
   - Benefit: Faster affected edge lookup

## 9. Asymptotic Bounds Summary

| Operation | Time | Space | Notes |
|-----------|------|-------|-------|
| **Edge Generation** | `O(n log n)` | `O(n)` | Dominated by sorting E = Θ(n) edges |
| **Board Construction** | `O(n)` | `O(n)` | Allocate 4n cells |
| **Single Move** | `O(n)` | `O(1)` | Win check iterates E edges |
| **Potential Calculation** | `O(n)` | `O(1)` | Iterate E edges, constant work each |
| **Full Game Simulation** | `O(n²)` | `O(n)` | Up to 4n moves, each O(n) |
| **Get Empty Cells** | `O(n)` | `O(n)` | Scan all 4n cells |

## 10. Conclusion

The implementation achieves efficient asymptotic complexity:
- **Edge generation**: `O(n log n)` one-time cost
- **Per-move cost**: `O(n)` for win detection and potential calculation
- **Space usage**: `O(n)` linear in board width

The linear dependence on board width n is optimal for this problem, as we must consider O(n) edges and O(n) cells. The implementation uses efficient algorithms (sort+unique for deduplication) and constant-time per-edge operations due to the fixed maximum edge length of 7.

For practical board sizes (n ≤ 100), all operations complete in milliseconds, making the implementation suitable for interactive use and batch simulations.
