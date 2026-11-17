## Report (solver-b)

All files are in `./solver-b/`.

### 1. Table — every entry COMPUTED (exhaustive greatest-fixpoint solve over all layouts; H−1 LOSE and H WIN both checked)

| M | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 |
|---|---|---|---|---|---|---|---|---|---|---|---|
| h^N | 4 | 6 | 7 | 9 | 11 | 13 | 14 | 16 | 18 | 20 | **22** |
| h^F | 4 | 6 | 7 | 9 | 11 | 13 | 14 | 16 | 18 | 20 | **21** |
| h^O | 4 | 6 | 7 | 9 | 11 | 13 | 14 | 16 | 18 | 20 | **21** |

- **M = 3, 4** were done by hand first (`hand.txt`) and the code reproduces them.
- **Cross-check:** the naive Python solver (explicit block lists, forward search, no symmetry) agrees with the C++ solver on 57 (G, M, H) cases, 27 LOSE and 30 WIN, with no mismatch.
- **Sanity checks:** sizes {1} give h = M; sizes {1,2} give 2,4,5,7,8,10,11 for M = 2..8, matching the hand values; h is non-decreasing and h^O ≤ h^F ≤ h^N throughout.
- **Stopped at M = 13.** M = 14 needs H = 23, about 4·10^8 layouts, which exceeds the 8 GB cap with this code.
- **Caveat:** N(13) WIN at 22 and F(13) WIN at 21 come only from the two-thread solver `solve2p`, which matches the single-thread solvers for M ≤ 9 on all three games and matches the N and O values through M = 12. N(13) LOSE at 21 was confirmed by two programs.

### 2. Strictness (B) and witnesses (C)

- **h^F < h^N:** first strict at M = 13 (21 vs 22).
- **h^O < h^F:** never strict for M ≤ 13.
- **H−1 impossible:** the solver's LOSE verdict for every entry. `pv_O.txt` has explicit adversary lines for M = 3..11 that free only 1s, so they are valid in all three games. The typical shape is: allocate M 1s, thin them to a pattern with no free pair or triple, allocate 2s or 3s, thin again, then a 3 does not fit.
- **N at M = 13, H = 21:** `pvN13.txt` has one sample line. It happens to free only 1s, because its simulated manager is not optimal; against the O-optimal manager the adversary must free a 2 or 3, since O wins at 21.
- **H suffices:** the solver's safe set ("always step to a safe layout"). I found no simple rule that hits the optimum.
  - For M = 3, 4 any manager works (`hand.txt`).
  - First fit in O needs 4,6,8,10,12,14,15,17,19 for M = 3..11, which is optimum + 1 from M = 5.
  - First fit, best fit and four other fixed rules in N are all above the optimum from M = 5 on (`rules_out.txt`).
- **Failing checks seen on purpose:** LOSE verdicts at H−1, and a "middle-most" rule that fails at the optimal H.

### 3. Conjectures and headline (D)

- **Proved:** mu^O = 5/3 exactly, and mu^N ≥ 7/4. So mu^O < mu^N, and (a) and (b) cannot both hold.
- **Open:** mu^F, which lies in [5/3, mu^N].
- **My answer:** (b) or (c), not (a), leaning (b) with mu^F = 5/3. Confidence is low: roughly (b) 40%, (c) 40%, (a) 20%.
- **For F tracking O:**
  - At M = 13, the only M where the games separate, F equals O.
  - The one all-manager lower-bound technique I have (residue-class markers, P1/P6) gives nothing above 5/3 in F. Each freed 2 lets the manager move a marker 1, which cancels the gain exactly.
- **Against (b):** for one natural F-manager (first fit, plus moving the rightmost low 1 into the leftmost hole after each freed 2), I have a sketched adversary reaching about 7M/4. So (b) would need a cleverer manager than I found.
- **No closed form offered.** floor(7M/4) − 1 fits all three games for M = 3..12, but it is provably wrong for O (it exceeds the O upper bound at M = 24) and N leaves it at M = 13. Data at M ≤ 13 do not settle any slope.

### 4. Proofs (E) — full text in `proofs.txt`

- **P1, COMPLETE.** h^O(M) ≥ 3(K + t) − 2 ≥ 5M/3 − 4, with K = ceil(M/3) and t = floor((M − K)/3). Allocate M 1s, keep K on one residue class mod 3, allocate t 3s; each 3 needs a fresh cell of that class. Only 1s are freed, so it holds in O, F and N.
- **P5, COMPLETE.** h^O(M) ≤ floor((5M+2)/3), by first fit. Invariants: every 1 sits below cell M, and cells M..top are solid permanent blocks. When a block must extend the top, cells 0..M−1 hold at least ceil((M−2)/3) occupied cells. Together with P1 this gives mu^O = 5/3.
- **P6, COMPLETE.** h^N(M) ≥ 7M/4 − 17/4. Allocate M 1s, drop the smallest class mod 3, allocate floor((M − X)/2) 2s. Each 2 covers two classes, so one of the two kept classes r has 2·(1s on r) + (2s covering r) ≥ X + y/2. Free everything not covering class r, including 2s, then allocate 3s. This is N only.
- **P2, COMPLETE but weak.** h^N(M) ≤ 3M − 6 for any manager.
- **P3, COMPLETE.** Exact values at M = 3, 4 (`hand.txt`).
- **P4, COMPLETE.** In F a freed 2 with no live 1, or a freed 3 with no live 1 or 2, grants nothing.
- **S1, SKETCH only.** First fit is poor in N (adversary reaches about 11M/6) and first fit plus compaction is poor in F (about 7M/4). These are arguments against single managers, not bounds.
- **Not proved:** any upper bound on mu^N below 3, any F-manager bound, and any F lower bound above 5/3.

### 5. Interpretations

- A position is the layout between requests with live ≤ M; h is the least H at which the empty heap is safe.
- In F, moves are optional and happen only right after a free. After a freed 3 the manager may do nothing, move one 2 (possibly overlapping its old place), or make one or two sequential 1-moves. Moving the same 1 twice is equivalent to one move.
- Freed cells count as free for the moves.
- In O, 2s and 3s are never freed, so they are permanent and count toward M forever.
- mu is read as limsup; for O the limit exists.

### 6. Files

- **Solvers:** `solve.cpp` (plain, binary-search index), `solve2.cpp` (hash index), `solve2p.cpp` (two threads), `solve_pv.cpp` (adversary lines), `naive.py` (independent check), `rules.cpp` (fixed-rule managers against every adversary), `checks.sh`.
- **Outputs:**
  - `out_small.txt`: M = 3..9 and the restricted size sets.
  - `outN.txt`, `outO.txt`: N and O for M = 8..13.
  - `outN13.txt`, `confirmN13_21.txt`: N at M = 13.
  - `outF10_12.txt`, `outF13.txt`: F.
  - `checks_out.txt`, `rules_out.txt`, `pv_O.txt`, `pvN13.txt`.
- **Write-ups:** `results.txt` (table with provenance), `hand.txt`, `proofs.txt`.
