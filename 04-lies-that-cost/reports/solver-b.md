# solver-b report

All files are in `solver-b/`.

## 1. Tables

P(n) is the formula quoted in the problem (least q with n(q+1) ≤ 2^q for even n, n(q+1)+(q−1) ≤ 2^q for odd n).

| n | 2 | 3 | 4 | 5 | 6–8 | 9–16 | 17–28 | 29–50 | 51–92 |
|---|---|---|---|---|---|---|---|---|---|
| U1(n) = P(n) | 3 | 5 | 5 | 6 | 6 | 7 | 8 | 9 | 10 |

- **U1(n) = P(n), n = 2..70**: COMPUTED, exhaustive search on one-lie states (`u1.py`, `u1_table.txt`). Table entries beyond n = 70 are formula only.
- **Q_2(n) = U1(n) = P(n) for every n in 2..1100, and for n = 2047..2050**: COMPUTED, exhaustive memoised search (`fast.cpp`; `fast_k2_2_600.txt`, `fast_k2_601_1100.txt`, `fast_k2_2047_2050.txt`).
- **Q_k(n) = U1(n) for k = 3, 4, 5, 8 and n = 2..300**: COMPUTED (`fast_k{3,4,5,8}_2_300.txt`).
- **k = 1, n = 2..317**: COMPUTED. Q_1(n) = U1(n) for n = 8 and every n in 10..317. Q_1(n) = ∞ for n ∈ {2, 3, 4, 5, 6, 7, 9}, by fixed-point iteration on the full uncapped state space (`fix.py`, `fix_out.txt`).

Cross-checks, all in agreement:
- Literal definition, no memo, no abstraction (`slow.cpp`, `slow_out.txt`): k = 2, 3 and U1 for n = 2..5 at both q = value and q = value − 1; k = 1, n = 2, 3, 4 not winnable in 6.
- Uncapped fixed point (`fix.py`): k = 1 n ≤ 11, k = 2 n ≤ 9, k = 3 n ≤ 8.
- `fast` with and without pruning: k = 1, 2, 3, n ≤ 12 (`fast_noprune_small.txt`, `fast_prune_small.txt`).

Pruning in `fast.cpp`:
- Horizon cap (obligation ≥ questions left is treated as equal to it): exact.
- Volume bound: safe by Lemma 4 in `proofs.txt`. An unsafe version would bias Q upward.
- Questions with more than half the alive set in S are skipped (complement symmetry).

## 2. Witnesses (`certs/`)

- `strat_k2_n{2..64}.txt`, `strat_k1_n{8,10,11,12,16}.txt`, `strat_k3_n{2..8,16}.txt`: explicit questioner trees (answer path → set S) that win within P(n).
- `check_strategy.py` replays each tree against every legal responder using the literal definition, not the abstraction. All 76 are ACCEPTed (`check_strategy_out.txt`).
- Lower bounds: Q_k(n) ≥ U1(n) ≥ P(n) for all n and k is proved (Lemmas 2 and 3), so no responder certificate is needed.
- `inf_k1_n{2,3,4,5,6,7,9}.txt`: closed trap sets of states showing Q_1 = ∞. `check_inf.py` enumerates every question itself; all ACCEPTed (`check_inf_out.txt`). This checker relies on Lemma 1.
- Deliberate failures (`neg/`, `neg_out.txt`), all REJECTed: a corrupted tree, a 6-question tree declared as 5, the k = 2 tree for n = 3 declared as k = 1, a trap set with a state removed, and a k = 1 trap set declared as k = 2.
- No tree files for n > 64 (size); rerun `fast`.

## 3. Headline

**Answer: yes, as far as computed; not proved.** No n with Q_2(n) > U1(n) exists in 2..1100 or at 2047..2050; that covers every step of P(n) up to and including 15→16.

Confidence that it holds for all n: about 90%. This is a guess beyond the computed range.

Conjectures (computed ranges above, nothing more):
- E_k = {n ≥ 2} for every k ≥ 2.
- E_1 = {8} ∪ {n ≥ 10}, with Q_1(n) = ∞ for the seven excluded n.

## 4. Proofs (`proofs.txt`)

- **Lemma 1, COMPLETE**: the position is the multiset of remaining obligations of alive candidates. This confirms the problem's last UNVERIFIED item.
- **Lemma 2, COMPLETE**: Q_k(n) ≥ U1(n).
- **Lemma 3, COMPLETE**: U1(n) ≥ P(n) for all n (volume bound plus the odd-n first-question imbalance). The converse for all n is Pelc's theorem, which I did not reprove; inside my tables it follows from the computation and Lemma 2.
- **Lemma 4, COMPLETE**: the volume pruning is safe.
- **Proposition 5, COMPLETE for n = 2 only**: Q_1(2) = ∞ by hand. n = 3, 4, 5, 6, 7, 9 are machine results.
- **Headline for all n: no proof, and no sketch offered as one.** `proofs.txt` ends with a heuristic, labelled as such: a lie can be repeated only if 2·ell < questions left, and volume-tight states have too many candidates for that except in the last four or so questions.

The problem's other UNVERIFIED items: Q_1(2) = Q_1(3) = ∞ confirmed (n = 4, 5, 6, 7, 9 are infinite too); Q_2(2) = 3 and Q_2(3) = 5 confirmed by three independent programs.

## 5. Interpretation

- By (i), each x has exactly one possible L, so "valid pair" just means "x alive".
- **Degenerate case**: a question that does not split C has ell = 0, so the "wrong" answer is a lie with an empty honesty window. I kept this, as the text says. Under the other reading (such questions forbidden), the values are identical for k = 1 n ≤ 10 and k = 2 n ≤ 8 (`fix_noell0.py`, `fix_noell0_out.txt`).
- "Q = ∞" for k = 1 is established by fixed point, not by a depth cutoff. The `Q>24` lines in `fast_k1_*.txt` are only depth cutoffs.

## 6. Code → numbers

- `fast.cpp`: all Q_k tables and the strategy trees (`DUMP` mode). `cmp.py` compares its output with P(n).
- `slow.cpp`: literal cross-check, n ≤ 5.
- `fix.py`: uncapped fixed point, ∞ results, trap sets. `fix_noell0.py`: the ell = 0 variant.
- `u1.py`: U1 table and P(n).
- `check_strategy.py`, `check_inf.py`: certificate checkers.
