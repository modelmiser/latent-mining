# solver-a report

All files are in `solver-a/`.

## 1. Tables

**T(n), exact, COMPUTED (exhaustive game search over oriented forests up to isomorphism):**

| n | 1–7 | 8–11 | 12–15 | 16 |
|---|---|---|---|---|
| T(n) | 0 | 1 | 2 | 3 |

So **T(n) = B(n) for every n ≤ 16.** Sources:
- n ≤ 6: `slow.py`, labelled positions straight from the definition, every orientation tried.
- n ≤ 9: `fast full` (whole-forest re-orientation, no pruning) = `fast fast` = `fast3d` (`run_small_out.txt`, no disagreement).
- n = 8…13: the full value tables `table_n8.txt … table_n13.txt` are re-verified state by state by `check_dump.py`, a Python checker sharing no code with the solver (all pass; `check_n11.txt`, `check_n12.txt`, `check_n13.txt`).
- n = 14: two C++ solvers agree (`fast` exact branch-and-bound, `fast2` budget search).
- n = 15, 16: `fast2` only (`run_n15.txt`, `run_n16.txt`; 1.2 M and 4.8 M states). The lower bounds are proved (P3); the upper bounds T(15) ≤ 2, T(16) ≤ 3 rest on that one program.

Sanity model, out-degree cap 1: 0,0,0,1,1,2,3,5,5,6,7,9 for n = 1…12; three solvers agree to n = 7, two to n = 9.

**Auxiliary game W_j(m)** (same game, Adversary also receives ψ_j(final tree); ψ_j(X) = min over re-orientations X′ of dist + min(j, maxd X′); maxd = largest directed distance from a saturated vertex to the nearest unsaturated one):
- COMPUTED exact: W₁ = W₂ = W₃ = ⌊m/4⌋ for m ≤ 13; W₁(14) = 3; **W₂(14) = 4**.
- COMPUTED lower bounds only: **W₂(16) ≥ 5** (certificate); W₂(15) ≥ 4, W₂(17) ≥ 5, W₂(18) ≥ 5 (solver output only, no certificate kept).

**Restricted Adversary** (blocks of 4, then only merges of sizes divisible by 4): value 4 = B(20) at n = 20, COMPUTED. Not a bound on T(20). The same run at n = 24 timed out (`run_adv5_n24.txt`): inconclusive.

## 2. Witnesses

- `table_n8.txt … table_n13.txt`: value of every reached state; `check_dump.py FILE` verifies both directions (a reply for every Adversary move; one Adversary move beating every cheaper reply). Deliberately corrupted copies `table_n8_corruptA/B.txt` are rejected.
- `cert_W2_14_ge4.txt` (280 claims), `cert_W2_16_ge5.txt` (441 claims): Adversary certificates; `check_lb.py 2 FILE N LB` accepts them. Corrupted copies `cert_W2_14_corruptA/B.txt` are rejected, and so is the genuine file when checked with J = 1.
- No explicit certificate file exists for the upper bounds at n = 14, 15, 16 (tables too large).

## 3. Headline

**Is T(n) = B(n) for every n? NO.**
T(30) ≥ W₂(14) + W₂(16) − 2 ≥ 4 + 5 − 2 = **7 > B(30) = 6**, and T(32) ≥ 8 > 7.
This is a complete proof (P6) resting on the two machine-checked certificates. Confidence: high.

**Least n with T(n) > B(n): not determined. It lies in [17, 30].** T(17) was out of reach under the memory and time caps (estimated 13 GB, 40 min). The two-part method of P6 cannot give anything below 30, because W₂(m) = ⌊m/4⌋ for m ≤ 13.

**True formula and lim T(n)/n: not found.** Proved: the limit τ exists and **1/4 ≤ τ ≤ 3**. I have no evidence that τ > 1/4: the deviation found is an additive +1, and no computed value exceeds n/4. I offer no conjectured formula. The guess "first deviation in 16–24" is neither confirmed nor refuted, except that 16 is ruled out.

Mechanism: at 14 vertices (price 2) and at 16 (price 3) Adversary forces a saturated vertex whose out-neighbours are both saturated, or else an extra payment. Merging two such trees costs 2 in one round.

## 4. Proofs (`proofs.txt`)

- P1 COMPLETE: Algorithm never needs to touch components not being merged.
- P2 COMPLETE: desaturating u costs exactly its directed distance to an unsaturated vertex; with two components left the value is min(maxd A, maxd B).
- P3 COMPLETE: T(n) ≥ B(n) for all n (block strategy).
- P4 COMPLETE: T(a+b) ≥ T(a)+T(b); the limit exists.
- P5 COMPLETE: T(n) ≤ 3(n−1) (reverse all out-arcs of an overfull vertex; potential = disagreements with the final tree rooted).
- P6 COMPLETE given the two certificates: T(a+b) ≥ W_j(a)+W_j(b)−j.
- Not proved: any upper bound of the form T(n) ≤ B(n)+c; the exact τ.

Stated beliefs: T(n)=0 for n ≤ 7, T(8)=1 — true (computed). T ≥ B, superadditivity, T ≤ Cn (C = 3) — true (proved). "One more out-degree-0 vertex than saturated vertices" — true (out-degrees sum to m−1).

## 5. Interpretation

- "Pays the number of old edges with D′ ≠ D" is read as the net change per round; the new edge is free.
- Algorithm may re-orient other components; by P1 this never helps, and the reference solver that allows it gives the same values (n ≤ 9).
- n = 1: no rounds, T = 0. No other ambiguity found.
- Rule slip: for under a minute three of my processes ran at once (limit 2).

## 6. Code

- `slow.py` — definition-level reference (n ≤ 6 cap 2, n ≤ 7 cap 1): `slow_out_5.txt`, `slow_cap1_n7.txt`, `slow_cap2_n6.txt`.
- `fast.cpp` — `full` reference mode and `fast` exact mode; writes the `table_n*.txt` files.
- `fast2.cpp` — budget search; T(15), T(16).
- `fast3.cpp` (binary `fast3d`) — same plus W_j games (`TERMJ`), restricted Adversaries, certificate dump (`DUMPLO`). W values: `W_runs_13_14.txt` and `run_small.sh`.
- `check_dump.py`, `check_lb.py` — independent checkers.
- `run_small.sh` — regenerates the small tables and cross-checks.
