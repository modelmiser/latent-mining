## Upper-side attack on mu^F = 5/3: not proved, and the evidence now points against it

**No manager with h^F(M) ≤ 5M/3 + c was found.** The main question is NOT PROVED in either direction. What I did establish is a computer-assisted lower bound for a broad class of managers, which is evidence for the other side.

### Best manager found
First fit plus compaction (after a freed 2 or 3, move the highest 1 to the lowest free cell). It is not good:

| M | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 |
|---|---|---|---|---|---|---|---|---|---|
| exhaustive worst extent | 10 | 12 | 14 | 16 | 18 | 21 | 22 | 25 | 26 |
| optimum h^F | 9 | 11 | 13 | 14 | 16 | 18 | 20 | 21 | n/a |

- **Exhaustive reach:** M = 14 only (10.4 million reachable states, growing about 3.4× per unit of M). The M in the 20s you asked for was not reached exhaustively.
- **Scripted adversary at larger M** (lower bounds on what the rule needs): 41, 82, 167, 334, 504 at M = 24, 48, 96, 192, 288, i.e. exactly 7M/4 at M = 288.
- **Other rules:** greedy potential-based move rules reach about 1.70·M on that script but are no better (or far worse) in exhaustive play. Plain first fit with no moves reaches 11M/6.

The adversary pattern that beats them: allocate M 1s, free one residue class mod 3 ("11.11.11."), allocate 2s (forced on top), free every second 2, thin the 1s to a minimal set leaving no free 3-window, then allocate 3s.

### What optimal managers do (exact safe set at M = 13, H = 21)
- A move is needed after only 4.7% of freed 2s and 1.3% of freed 3s.
- Compaction is a safe response in just 56% of the freed-2 cases that need a move.
- When a move is needed, 55% of the live 1s work as the block to move, but 90% of free cells work as the destination. Which 1 is removed matters; where it goes mostly does not.
- A freed 3 is almost always answered with one 1-move; a 2-move works in under 8% of the cases needing a move.
- No simple geometric invariant describes the safe set, and it is mirror-symmetric.

### Proof status
- **mu^F = 5/3: NOT PROVED.** No invariant found.
- **Theorem R: COMPLETE (computer-assisted).** For M ≡ 0 mod 12, any F-manager that puts the first M 1s on cells 0..M-1 and packs 2s on top needs at least 27M/16 − 11 cells, whatever its move rule. That is slope 5/3 + 1/48, and it covers first fit and best fit.
  - Mechanism: a 1-move is worth at most 7/4 of a cell in that position, while a freed 2 in a packed run costs 2.
  - The 7/4 is certified by a no-positive-cycle check on a 144-node graph; the same search fails at 20/12, so the constant is exact.
  - Exact restricted optimum by dynamic programming: 77, 159, 320, 645, 1292, 2589, 5180 at M = 48, 96, 192, 384, 768, 1536, 3072. Against 5M/3 that is −3, −1, 0, +5, +12, +29, +60.
  - The effect is negative below M = 192 and first positive at M = 384, so no small table can show it.
  - The dynamic program matches brute-force enumeration in 7 cases (M = 24, 36, 48).
- **Theorem R': COMPLETE (same computer check, extra steps by hand).** Any manager that puts the first M 1s on cells 0..M-1 needs at least (5/3 + 1/144)M − 12 cells, wherever it puts the 2s.
- **mu^F > 5/3 for all managers: NOT PROVED.** A manager that spreads or offsets the first M 1s is not covered; I have only a heuristic argument that this costs more than it gains.

### Negative controls (all behaved as expected)
- Band one cell too small: at M = 12 with cap 21 the test reports FAIL; with cap 22 it reports PASS.
- Deliberately worse rules: last fit needs 16 and 20 at M = 8 and 10 (first fit plus compaction: 14 and 18); "move the lowest 1 into the freed hole" needs 12, 18, 24 at M = 6, 8, 10.
- The certificate search fails for slope 20/12 and succeeds for 21/12.

### Belief
mu^F > 5/3, roughly 3 to 1. The limited move is strong (worth 7/4 per freed 2, against 1 for compaction) but not strong enough. Every manager that fills the bottom contiguously provably loses a linear amount, and the only uncovered escape, leaving holes among the first M 1s, looks expensive.

### Housekeeping
- Two of my own `rt2` sweep processes are still running at nice 15. They end on their own 900-second timeouts; I was denied permission to kill them. Their output file `sweep2.log` is interleaved and should be ignored, as should the partial `sweep1.log`.
- Nothing outside the new directory was modified, and `open-f-lower` was not read.

### Files
All in `./open-f-upper/`:
- `proofs.txt` — theorems, tables and status tags
- `rt.cpp` — rule tester: exhaustive, scripted and random adversaries
- `tests.log`, `tests_greedy.log`, `tests_v10.log`, `att.log` — rule results and negative controls
- `strat.cpp`, `strat1.cpp`, `strat_F13_21.log`, `strat1_F13_21.log` — safe-set statistics at M = 13
- `dp4.cpp`, `dp4_all.log`, `dp4.log` — exact restricted optimum
- `cyc.cpp`, `cyc.log` — the 7/4 certificate
- `opt4.cpp`, `opt4_48.log`, `opt4_sa.log`, `crosscheck_dp_vs_enum.log` — brute-force and annealing cross-checks
