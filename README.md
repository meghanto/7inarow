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
