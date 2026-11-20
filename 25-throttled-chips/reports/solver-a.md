# Solver A report

**L = lim G(h)/h = 5, proved. Computed: G(3)=5, G(4)=10, G(5)=15, G(6)=20, and G(h)=C(h) for h=4,5,6, so "G(h)<C(h) for every h>=3" is false (the G lower bounds are window-restricted, see 1). The problem's sqrt(24) bound is wrong.**

## 1. Tables

| h | 1 | 2 | 3 | 4 | 5 | 6 | 7..14 |
|---|---|---|---|---|---|---|---|
| C(h) | 1 | 2 | 7 | 10 | 15 | 20 | 25,30,...,60 (=5h-10) |
| G(h) | 1 | 2 | 5 | 10 | 15 | 20 | not computed |
| largest window W searched for G | 5 | 5 | 7 | 7 | 6 | 4 | |

- **C(h), h<=14: COMPUTED**, exact and unrestricted: memoised search over all Firer lines of the all-zero list (`slow.py C h`).
- **G(h): COMPUTED over all lists whose entries lie in W consecutive vertices** (`gsearch.cpp`: iterative deepening on list length; state = exact set of Firer-reachable positions after a prefix).
  - The upper bounds (winning lists) are unconditional.
  - The lower bounds are exhaustive only for lists of span <= W. Wider lists are not covered, and I have no proof they cannot help. Every W tried (see table) gave the same value.
- Prunings in `gsearch`, both exact for the windowed problem:
  - memo on the position set, canonical under reflection, keyed by a 128-bit hash; a collision could only prune wrongly and bias G upward;
  - remaining length < h minus (min over positions of the max height) cannot win, since each round adds one chip. With this off (`NOLB`) the values are unchanged for h=4 W=3, h=5 W=3,4, h=6 W=3 (`out_nolb.txt`).
- Cross-check against the slow program enumerating every list: h<=3 with W<=4, h=4 with W=2,3, h=5 with W=2 all agree (`out_slowcross.txt`).
- The problem's stated small values are all reproduced; h <= G <= C holds in every entry.

## 2. Witnesses (`witnesses.txt`; checker `slow.py check h list`)
- Winning lists: 0,1,0,1,0 (h=3); ten zeros (h=4); fifteen zeros (h=5); twenty zeros (h=6); seven zeros for C(3).
- C lower bounds: explicit surviving Firer lines for 6, 9, 14, 19 zeros at h=3,4,5,6.
- Rejected on purpose: 0,1,0,1 at h=3 and the one-shorter zero lists (checker prints a surviving line).
- G lower bounds have no compact certificate; they are the exhaustive runs (`out_h*_W*.txt`), reproducible with `gsearch h W`. For h=3, `wide3.py` also checks all 13^4 lists of length 4 on 13 vertices: none wins.

## 3. Headline answer
- **L = 5 exactly, and the limit exists** (proved, section 4). lim C(h)/h = 5 as well. Confidence: high.
- Conjecture, not proved: **G(h) = C(h) = 5h-10 for all h>=4**, with G = 1,2,5 for h = 1,2,3 and C(3)=7. Evidence: h<=14 for C; h=4,5,6 for G, windowed.
- The problem's GUESS is false: L is not below sqrt(24), and not below lim C/h.
- The problem's claim C(h) <= sqrt(24)(h-1)+O(1) is false for large h, since Theorem 2 gives C(h) >= 5h - O(sqrt h). Its error: "Q >= n^3/(12(h-1)^2)" is a continuum estimate, but the game lives on about 5 vertices, where the minimum for n=5(h-1) chips is 10(h-1), not 10.42(h-1).

## 4. Proofs (full text in `solver-a/proofs.txt`)
- **Theorem 1, COMPLETE: C(h) <= 5h-5.** Let Q = sum of x^2 over chips. After the m-th add, Q <= 2(m-1). Since x^2 >= 9-w_x with w = 9,8,8,5,5 on 0,±1,±2, Q >= 9m-35(h-1). So a surviving Firer needs m <= 5(h-1)-1.
- **Theorem 2, COMPLETE: if m/5 + 2sqrt(m) + 13.2 <= h-1, every list of length m loses.** Hence G(h) >= 5h - 10sqrt(5h) - 72.
  - Strategy: with k_x = adds at x so far, target T_x = (3k_x+k_{x-1}+k_{x+1})/5 and lag_x = T_x minus firings at x. Fire the ready vertex of largest lag >= 1; otherwise pass.
  - Then height_x = (mean of k over x-2..x+2) + 2lag_x - lag_{x-1} - lag_{x+1}, and lag >= 0.
  - A vertex of maximal lag M >= 6 is always ready, so the potential sum of max(lag-6,0)^2 grows by at most 1 per round, giving lag <= 6.6+sqrt(t).
- **Corollary, COMPLETE:** 5h - O(sqrt h) <= G(h) <= C(h) <= 5h-5, so L = 5.
- Not proved: C(h) <= 5h-10 (Theorem 1 misses by 5); G(h) >= 5h-10 with no error term; that wide lists cannot beat windowed ones.
- `strategy_sim.py` runs the strategy on random, constant, cyclic and bursty lists of length 300: largest lag 2.2, largest height minus t/5 was 3.0. Lag never reached 6, so the proof's key step was not exercised; sanity check only.

## 5. Interpretation
- Literal reading used: the height test applies after step (1) and after step (2) of every round.
- Other reading (test only at the end of a round; `slow.py ALT`, `out_ALT.txt`): C = 1,4,7,12,17,22,27,32 for h=1..8, i.e. 5h-8 from h=4. G with W=3 is 7 at h=3 and 12 at h=4, equal to C. L = 5 is unaffected.

## 6. Files (all in `solver-a/`)
- `slow.py`: C values (`out_C.txt`), list checker (`witnesses.txt`), brute-force G (`out_slowcross.txt`), other reading (`out_ALT.txt`).
- `gsearch.cpp`: G table (`out_h3_W7.txt`, `out_h4_W6.txt`, `out_h4_W7.txt`, `out_h5_W6.txt`, `out_h6_W2.txt`..`W4`, `out_nolb.txt`). Runs with W<=5 for h<=5 went to the terminal only (under a minute each).
- `wide3.py` (`out_wide3.txt`), `strategy_sim.py`, `proofs.txt`.
