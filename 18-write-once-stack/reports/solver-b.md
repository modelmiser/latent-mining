# Solver-b report

All files are in `solver-b/`.

## 1. Tables

Two readings (§5): **L** = literal text (nothing checked before the first operation, so Q(0^n) is free); **S** = strict (Q(0^n) = EMPTY also required).

| n | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|
| R_L(n) | **1** | 1 | 2 | 3 | 4 | 5 | 6 | 7 | not reached |
| R_S(n) | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | not reached |

- Every entry is COMPUTED. Existence: a scheme found by search, re-checked by an independent checker over every legal sequence. Non-existence at the next t: proof P1, plus exhaustive search.
- Exhaustive methods, agreeing where they overlap:
  - `slow.py`: plain backtracking, no pruning; all t ≤ n+1 for n ≤ 4, both readings.
  - `fast.cpp`: DFS with compatibility pruning and fresh-cell symmetry breaking; all t ≤ n+1 for n ≤ 5 (both readings), plus (6,4), (6,5), (6,6).
  - `sat.cpp`: SAT encoding with a self-written CDCL solver; t = n−1 and t = n for 2 ≤ n ≤ 7, plus (8,7).
- n = 9, t = 8: `sat` hit the 1500 s limit. Undetermined.
- Auxiliary M(w, r) = least number of cells surviving r operations from a known stack w, Q(start) = top(w). COMPUTED (`mdriver.py` + `sat`) for every w with |w| ≤ r+1, r ≤ 6 (`Mtable.txt`, 501 rows):
  - r = 0: M = 0.
  - r = 1: M = 2 for |w| ≤ 1, M = 1 for |w| = 2.
  - r = 2: M = 2 for w = 000, 111; M = 3 for the other 13.
  - r = 3, 4, 5, 6: M = r+1 for all 31, 63, 127, 255 stacks.

## 2. Witnesses

- R(n) ≥ n−1: `wit_n*_t*_{L,S}.txt` (fast, n ≤ 6) and `s_n*_t*_{L,S}.txt` (sat), including `s_n7_t6_*` and `s_n8_t7_*`.
  - Plain `Q`/`E` lines, unlisted E = identity; check with `python3 check.py <file> <L|S>`.
  - The S files for n = 7, 8 are the L witnesses with the start-config Q line removed (justified by P2), re-checked.
- Sub-game witnesses for r ≤ 5: `mwit/w_<w>_r<r>.txt`, 246 files, re-checked by `allsub.py`. The r = 6 witnesses were not stored.
- Pair scheme: `pairscheme.py`, checked for n = 2..10.
- Hybrid scheme: `hyb_p{0..3}.txt` (n = 6, 8, 10, 12; t = 5, 6, 7, 8), checked.
- Checker rejects broken inputs: `bad_t7.txt`, `bad_q.txt`, `bad_mono.txt`.

## 3. Headline

**Not settled.** R(n) = n−1 for 2 ≤ n ≤ 8 (computed), so any counterexample has n0 ≥ 9.

- Proved for all n ≥ 6: floor(n/2)+2 ≤ R(n) ≤ n−1.
- Conjecture: YES for all n ≥ 2, confidence about 75–80%. This is a guess; I found no general construction.
  - Evidence: the stronger statement G(r), "from any known stack, r+1 cells suffice for r operations", holds for every stack for r ≤ 6.
- Structural fact (computed): for r ≥ 3 every stack type needs exactly r+1 cells. A scheme for t = n−1 therefore spends exactly one cell on every forced step, with no slack until the last two operations.

## 4. Proofs (`proofs.txt`)

- **P0, COMPLETE.** The reduction the programs use.
- **P1, COMPLETE.** R_S(n) ≤ n−1 for all n; R_L(n) ≤ n−1 for n ≥ 2; R_L(1) = 1.
- **P2, COMPLETE.** The two readings coincide for t ≥ 3.
- **P3, COMPLETE.** R(n) ≥ floor(n/2) by the pair scheme (invariant proof).
- **P4, COMPLETE given a finite computed table** (the 246 checked files in `mwit/`). R(n) ≥ floor(n/2)+2 for n ≥ 6: pair scheme on p pairs; once all are touched they reveal the stack and the remaining budget r ≤ 5, and six extra cells run the computed scheme for that (stack, r).
- **P5, COMPLETE (counterexample).** "Every non-last operation must change the configuration" is false. In `wit_n5_t4_L.txt`, the third PUSH0 of PUSH0, PUSH0, PUSH0 leaves 11000 unchanged, at step 3 of 4. A push of b from (S, r) may stay iff |S| ≥ r and the top r entries are all b.
- **S1, NOT PROVED.** The headline. The natural induction on G(r) fails because the three sub-cubes above the first-step configurations overlap pairwise.

## 5. Problem-statement issues

- "Nothing is checked before the first operation" leaves Q(0^n) unconstrained. Under this literal reading (the one I chose) R(1) = 1, not 0.
- The stated proof of R(n) ≤ n−1 assumes the first push changes the configuration. That is a gap; the bound is false at n = 1 and true for n ≥ 2 (P1).
- Strict reading: R(1) = 0, everything else identical.
- Confirmed: R(2), R(3), R(4), the n = 3 example, the floor(n/2) bound.

## 6. Code

- `slow.py`, `fast.cpp`, `sat.cpp`: the R table. `check.py`: independent checker.
- `mdriver.py`: `Mtable.txt`. `allsub.py`: `mwit/`. `pairscheme.py`: P3. `hybrid.py`: P4.
- `fast2.cpp`, `fast3.cpp`, `fast4.cpp` are abandoned experiments; no reported number depends on them.

Symmetry breaking could only bias toward "no scheme". Every "scheme exists" answer is independently checked; every "no scheme" in the R table is at t ≥ n, covered by P1. The M-table lower halves (no scheme on r cells) are search-only.

Process note: for a few minutes three compute processes overlapped, exceeding the two-core limit.
