# Referee report

**Solver A's headline stands: L = lim G(h)/h = 5 is proved. G(4) = 10 and G(5) = 15 hold for all lists, through solver B's Lemma Z, which I re-ran with my own program. A's "G(6) = 20" is not established: only 15 <= G(6) <= 20 by my runs (16 by B's).**

My code is in `verifier/`: `ref.py` (slow definition, witness checker), `v.cpp` (search), `strat.cpp` (Theorem 2 strategy), `lemc.py`.

## Verdicts

| Claim | Verdict | How |
|---|---|---|
| C(h) = 1, 2, 7, then 5h-10 for 4 <= h <= 14 | CONFIRMED | recomputed |
| G(1) = 1, G(2) = 2, G(3) = 5 | CONFIRMED | recomputed; also B's P2 |
| G(4) = 10, G(5) = 15, all lists | CONFIRMED | Lemma Z read, no gap; my zone search |
| G(6) = 20 (A) | UNSUPPORTED | lower bound covers windows of width <= 4 only |
| G(6) >= 16 (B) | not reproduced | my search reached M = 14 |
| "G(h) < C(h) for all h >= 3" is false | CONFIRMED | G(4) = C(4) = 10 |
| A Theorem 1, C(h) <= 5h-5 | CONFIRMED for h >= 2 | proof read; false as stated at h = 1 |
| B P0, P1, P2, P3 (C(h) <= 5h-6) | CONFIRMED | proofs read, no gap |
| A Theorem 2 and Corollary (L = 5) | CONFIRMED | proof read, no gap; program tests |
| B Lemma C, Lemma Z | CONFIRMED | proofs read, no gap; checks below |
| Problem's sqrt(24) bound | REFUTED for large h | Theorem 2, since 5 > sqrt(24) |
| G(h) = C(h) = 5h-10 for h >= 4 | open | conjecture in both reports |

## Theorem 2, step by step

No step fails.

- **Height identity:** k_x - 2T_x + T_{x-1} + T_{x+1} is the 5-site mean of k; asserted at every step by `strat.cpp`.
- **Legality:** only a ready vertex is fired, at most once per round.
- **Max lag M >= 6 is ready:** l_x <= T_x <= 3 eta*_x, and at a maximum 2l_x - l_{x-1} - l_{x+1} >= 0, so eta_x >= M/3 >= 2.
- **Potential:** the add raises Theta by at most 2 max(M-6, 0); firing a lag-M vertex removes 2(M-6)-1, or (M-6)^2 when M < 7. Net at most 1.
- **Both steps of a round:** lag <= 6.6 + sqrt(t-1) after the add and <= 6 + sqrt(t) after the firing; both give height <= t/5 + 13.2 + 2 sqrt(t).
- **Corollary arithmetic:** checked numerically for h from 50 to 10^6.

Program tests, three tie-break rules:

- Exhaustive over all lists: length 11 on 5 vertices, 13 on 4, 10 on 6, 8 on 7.
- Beam search for large lag and large height minus t/5: lengths to 3000, widths to 12.
- Largest lag found is 2.4; largest height minus t/5 is 3.2. A max-lag vertex was unready only at lag <= 1.
- The real strategy never reaches lag 6. A lazy variant that fires only at lag >= 6, covered by the same proof, reached lag 7.2; all assertions held.
- Three broken variants were rejected: wrong target coefficient, readiness claimed from lag 1, firing only from lag 12.

## Small values: Lemma C and Lemma Z

- **Lemma C:** the reach bound rho(F) and the superposition step are right; brute force over all lines for small lists shows the bound attained, never exceeded.
- **Lemma Z:** restricting Firer only helps the list, and a minimal Z-winning list has an overlap-connected interval family. The prefix test is sound.
- **Mechanical check:** every minimal Z-winning list inside a window is connected (20 lists at h = 3, 2531 at h = 4). The detector fires on non-minimal lists (480 at h = 3, length 6).
- **Independent search:** no Z-winning list for h = 3, M <= 4; h = 4, M <= 9; h = 5, M <= 14 (1202 s); h = 6, M <= 14. Runs with the pass prune off or the memo off agree where tried (h = 4; h = 5, M = 12).

So G(4) = 10 and G(5) = 15 are unconditional, up to a 128-bit hash collision.

## Disagreements between the solvers

- **Limit:** A says proved, B says open. A is right.
- **Upper bound:** 5h-5 (A), 5h-6 (B). Both valid; B's is sharper.
- **G lower bounds:** A's are windowed, B's cover all lists. B's status is correct.
- **Reading:** both test heights after each step, as written. Neither gives the exact G(h) for all h.

## Errors found

- A's Theorem 1 says "every h >= 1"; at h = 1 it gives C(1) <= 0.
- A's table lists G(6) = 20 as computed; it is a windowed value.
- A's simulation maxima (lag 2.2, height excess 3.0) are low: 2.4 and 3.2 occur. That simulation cannot test the lag >= 6 claim, as A says.

## Witnesses

- All 11 winning lists and 5 surviving Firer lines from both solvers are accepted by `ref.py`.
- Six broken witnesses were rejected. A seventh list I meant as broken (thirteen zeros, then 1, 0, at h = 5) really wins; `v.cpp` agrees.
- B's winner counts match mine: 8 at h = 3, 21 at h = 4 with radius 1.

## Not checked

- B's `g2.cpp` and `g4.cpp` line by line; B's cluster-rule runs; B's h = 6, M = 15 run.
- A's alternative-reading table; B's adaptive-Adder note.
- My unrestricted window run for h = 5, M = 14, radius 3 timed out; radius 2 found no winning list.

## Follow-up tasks

1. Prove the lag in A's strategy is bounded by a constant (observed <= 12/5). That gives G(h) >= 5h - c.
2. Prove C(h) = 5h-10 for all h >= 4, then settle G(6) by completing the zone search for M = 16 to 19.
