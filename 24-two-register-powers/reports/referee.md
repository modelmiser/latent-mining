# Referee report

Code and outputs: `verifier/`.

## Verdicts

**Headline, L > 1: CONFIRMED** (both proofs read line by line, no gap; lemmas tested mechanically in exact arithmetic). Best proved constant: L >= 1/0.956 = 1.0460 (solver B). Solver A's L >= 1.0383 is also proved, and weaker.

**Solver A, gamma = 0.963096: PROVED as stated.**
- P4 (values <= 2^k F_{j+2}), P5 (at most 2^j C(k+2j+1, 2j+1) classes), P6 (N(t) <= U(t)): no gap. Tested on all words of length <= 15: no violation (P4 tight in 136 cases, so not vacuous).
- Entropy optimisation recomputed by bisection: crossing at d* = 0.1206975, gamma = 0.9630958, 2^gamma = 1.949489, 1/gamma = 1.038318. It is the global max-min (f1 increasing on [0, d*], f2 decreasing).
- U(t) recomputed at t = 100..1600: matches A's five values.

**Solver B, 0.956: PROVED, with two misstatements.**
- Four-case lemma: every case re-derived by hand, no gap. Exact integer test (2^(4 Phi) = y^3 max(4x, y)/4) on all 1 <= x <= y <= 600: the per-type suprema are exactly 16, 8, 8, 8. Inequality (*) checked exactly on all type-words of length <= 16.
- Word count: H(0.176) + 0.176 log2 3 = 0.95020 < 0.956. Correct.
- Misstatement 1: the header of P2 claims N(t) <= 2^(0.956 t); the proof gives 2 * 2^(0.956 t) (the report has the factor right). The header is true with eta = 0.0442 in place of 0.044.
- Misstatement 2: "for large t" is very large. The explicit bound falls under 2^(0.956 t + 1) only from t of about 9500, so the report's unqualified "D(m) >= (log2 m - 1)/0.956" is proved only for m with D(m) of that size. The liminf is unaffected.

**N(t), t = 0..22: CONFIRMED** (recomputed). All 23 values match A; the first 22 match B.

**D(m), 2 <= m <= 4100: CONFIRMED**, recomputed two independent ways: BFS on Z_m x Z_m, and directly from the integer definition (least t at which {n : G(n) <= t} meets every residue). All three of A's columns (D, number of worst residues, first worst residue) and all of B's table match. No entry differs from either solver.

**G tables: CONFIRMED.** B's `g_15.txt`: all 32768 entries. A's `nt_G.txt`: the 14342 entries with G <= 17.

**G(n) = ceil(log2 n) iff popcount <= 2: "if" CONFIRMED** (proofs read, no gap). "Only if" confirmed by recomputation up to n <= 2^22 (254 numbers, none with three or more 1-bits); unproved beyond.

**Mod-m equals integer formulation, easy bounds: CONFIRMED** (proofs read; no violation for m <= 4100).

**Witnesses: CONFIRMED** with my checker, which uses my own g_m(r) table. A's `paths_m400.txt` (80199 lines) and B's `dm128w.out` (8255 lines) are accepted, and every word has exactly minimal length. A's `worst_residue_m400.txt`: 399 of 399 correct. Rejected: the solvers' three broken files and six of mine (flipped letter, dropped residue, valid but over-long word, swapped residues, illegal letter, duplicated line).

## Disagreements between the solvers

No numerical disagreement.
- Different constants (0.963096 vs 0.956) come from different arguments; both valid.
- Gap histogram 358/3738/3 (A, m <= 4100) vs 358/3734/3 (B, m <= 4096): both right for their ranges. Gap 2 occurs at m = 511, 512, 520.
- A calls the minimum ratio "still drifting down". Not supported: octave minima of D/log2 m from 2^6 to 2^14 are 1.292, 1.280, 1.279, 1.307, 1.292, 1.291, 1.286, 1.284, 1.279.

## Errors and weak checks

- B's two misstatements above.
- B's `potential_check.py` has an assertion that cannot fail (`assert abs(...) < 1e-6 or True`, with P never updated). Its test of (*) uses floats with a tolerance, and its finite check of the final bound cannot fail at the small t it samples (the bound exceeds 2^t there).
- Neither solver checked the statement's claim that N(t) grows at least like phi^t. It is true: the binary method gives N(t) >= F_{t+3} - 1, with equality for t <= 5.
- Both lean on "D(m) stays within 1 of T(m)" for the guess L = 1/log2(lambda). I extended D(m) to m <= 40000 (integer-definition method only): the gap is 0 for 558 values, 1 for 37268, 2 for 2173, never 3. Gap 2 becomes common from m = 9069.

## Problem as written

Both answered the stated problem, with the same reading: either register counts, residue 0 means positive multiples of m, liminf over all m. I found no ambiguity. Both correctly flag "only for powers of 2" as false.

## Not checked

- The solvers' programs were not rerun.
- D(m) for 4101..40000 rests on one method.
- The "no shorter path" side of D(m) rests on my two exhaustive computations, not on a certificate.
- Nothing in `chain/` was read; no web search.

## Follow-up tasks

1. Sharpen the lower bound: prove N(t) <= 2^(gamma t + o(t)) with gamma <= 0.94, by optimising (c, rho) in B's potential and counting words modulo A's A/B commutation. My unchecked hand estimate: (c, rho) near (0.29, 0.5) alone gives about 0.949.
2. Upper bound: prove L < 2, that is, find c < 2 and an infinite family (m = 2^k is the natural candidate, where D/k is about 1.33 for k <= 15) with D(m) <= c log2 m + O(1). Tractability uncertain: two registers rule out window methods.
