# 7-in-a-Row Maker-Breaker Game Harness

A C++20 command-line implementation of the finite maker-breaker game (4, n, 7^tr) as described in "Towards solving the 7‑in‑a‑row game" (Czifra, Csóka, Zombori, Makay, 2021).

## Features

- **Board**: 4 rows × n columns with configurable width
- **Players**: Maker and Breaker alternate moves
- **Hyperedges**: Implements the (4,n,7^tr) game variant with:
  - Standard length-7 horizontal lines
  - Truncated edges (length 4-6) at board boundaries
  - Vertical lines (full columns, length 4)
  - Diagonal lines (length 4)
- **Potential Calculation**: Implements l-lines and pot(b) formula exactly as defined in the paper
- **Win Detection**: 
  - Maker wins by completing any hyperedge
  - Breaker winning certificate: pot(b) < 1 on Breaker's turn
- **CLI Commands**: Print edges, simulate random games, compute potential

## Building

### Requirements

- C++20 compiler:
  - **Linux**: GCC 11+ or Clang 14+
  - **macOS**: Apple Clang (Xcode Command Line Tools)
- CMake 3.16+
- Ninja (recommended) or Make

### Build Instructions

Using CMake Presets (recommended):

```bash
# Linux Debug
cmake --preset linux-debug
cmake --build build/linux-debug

# Linux Release
cmake --preset linux-release
cmake --build build/linux-release

# macOS Debug
cmake --preset macos-debug
cmake --build build/macos-debug

# macOS Release
cmake --preset macos-release
cmake --build build/macos-release
```

Traditional CMake:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Usage

The `game` executable provides several commands:

### Print Hyperedges

Print all hyperedges for a given board size in canonical form:

```bash
./build/linux-release/game print-edges -n 10
```

### Compute Potential

Calculate the potential and l-line histogram for an empty board:

```bash
./build/linux-release/game potential -n 8
```

Output:
```
L-line histogram:
  x_1 = 0
  x_2 = 0
  x_3 = 0
  x_4 = 22
  x_5 = 4
  x_6 = 4
  x_7 = 8
Potential: 3.250000
```

### Simulate Random Game

Run a random game simulation with move-by-move statistics:

```bash
./build/linux-release/game simulate -n 7 -s 42 -m 100
```

Options:
- `-n, --cols <N>`: Number of columns (default: 10)
- `-s, --seed <S>`: Random seed for reproducibility (default: 42)
- `-m, --max-moves <M>`: Maximum number of moves (default: 100)

Output shows for each move:
- Move number
- Player (Maker or Breaker)
- Cell coordinates
- Current potential pot(b)
- Whether Maker has won
- Whether Breaker has winning certificate

## Testing

Run the test suite:

```bash
./build/linux-release/game_tests
```

Tests verify:
- Edge generation correctness
- Canonical form of edges
- Edge count invariants
- Potential calculation formula
- Histogram computation
- Game logic

## Implementation Details

## Algorithms Needed to Recreate the Paper

To reproduce the end-to-end results from "Towards solving the 7‑in‑a‑row game," the implementation needs the following algorithms. This repository currently implements the finite $(4,n,7^{tr})$ game construction, l-line counts, and potential formula; the remaining items describe the solver features required by the paper.

### A) Game Construction $(4,n,7^{tr})$
- **Truncated edge generator**: Build hyperedges for first/last 4 cells in each row, all length-7 windows, all vertical 4-edges, both diagonal 4-edge families, and the extra short boundary edges from Section III.
- **Edge representation**: Store hyperedges as bitmasks over the $4n$ squares (or another constant-time membership structure) to update/kill edges quickly after moves.
- **Tiling reduction support**: Use the truncated edge set so that finite-block results can be lifted to the infinite board (Theorems III.1 and III.3).

### B) Core Solver: Proof-Number Search (PNS)
- **AND/OR game graph**: OR nodes for Maker-to-move, AND nodes for Breaker-to-move, with terminal outcomes defined by Maker completing a hyperedge vs Breaker blocking all.
- **PN/DN recurrence**: OR has $PN=\min PN(child)$, $DN=\sum DN(child)$; AND has $PN=\sum PN(child)$, $DN=\min DN(child)$.
- **Most-proving selection loop**: Iteratively select the most-proving leaf, expand it, initialize PN/DN for new leaves, and back up values until the root is solved.

### C) Search Reductions (PNS+)
- **Potential + breaker-win stop**: Compute $pot(b)=\sum_l x_l 2^{-(l-1)}$ from l-lines; if Breaker to move and $pot(b)<1$, declare Breaker win.
- **Maker-win stop**: Detect immediate Maker wins by a crossing of two 2-lines (Theorem IV.1).
- **Forced move pruning**: If a 1-line exists or a 2-line crossing exists, restrict legal moves to the forced responses.
- **Dead-square elimination**: Remove empty squares not belonging to any active line.
- **Dominated-square / partial pairing**: Remove pairs of squares that lie only in a single line; for 2-lines, bias Maker to the dominating square.
- **Simplify-to-fixpoint**: Apply the reductions repeatedly in a `simplify(state)` routine before generating children.

### D) Avoiding Repeats + Decomposition
- **Transposition table**: Canonicalize positions and memoize nodes to turn the tree into a DAG.
- **Horizontal symmetry**: Use the min of (board, reflected) as the hash key.
- **(Optional) Hypergraph isomorphy**: Canonical-form merging (often more expensive than the savings).
- **Component decomposition**: Split the residual hypergraph and solve components independently; handle articulation/cut-vertex cases using Theorems IV.2 and IV.3.

### E) Heuristic PN/DN Initialization
- **DN heuristic from potential**: Use $\alpha^{pot(b)}$ (paper uses $\alpha=1000$) with OR-node adjustments relative to parent AND-node potential.
- **PN via logistic regression**: Predict Breaker-win probability from node type, empty squares, and potential; set $PN=1+\beta\cdot P(\text{breaker win})$ (paper uses $\beta=10$).

### Potential Formula

The potential is computed as:

```
pot(b) = Σ(l=1 to k) x_l × 2^(-(l-1))
```

where:
- k = 7 (maximum edge length)
- x_l = number of l-lines
- An l-line is a hyperedge with no Breaker cells and exactly l empty cells

### Breaker Winning Certificate

When it's Breaker's turn and pot(b) < 1, Breaker has a certificate that they can prevent Maker from winning.

### Complexity Analysis

For detailed time and space complexity analysis of all operations, see [COMPLEXITY.md](COMPLEXITY.md).

**Summary**:
- Edge generation: O(n log n) time, O(n) space
- Single move: O(n) time (includes win detection)
- Potential calculation: O(n) time
- Full game: O(n²) time for up to 4n moves

Where n is the number of columns in the board.

### Cross-Platform Compatibility

The code uses:
- Standard C++20 features only
- Fixed-width integer types from `<cstdint>`
- No platform-specific APIs
- Consistent behavior across macOS and Linux

## License

See repository license.

## References

Czifra, G., Csóka, G., Zombori, B., & Makay, G. (2021). "Towards solving the 7‑in‑a‑row game"
