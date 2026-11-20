# solver-b report

All files are in `solver-b/`.

## 1. Tables

NO / YES are COMPUTED by exhaustive enumeration unless tagged PROVED: all automata with at most s states, up to renaming of states. Pruning uses necessary solo-run conditions only (listed in `enum2.cpp`). Unsafe pruning would bias towards NO, so the pruned enumerator was compared with the weaker-pruned one on a YES case: both give 19300.

**n = 2** (solution with at most s states per process?)

| b | s=3 | s=4 | s=5 | s=6 |
|---|---|---|---|---|
| 2 | NO | NO | NO | YES (19300 unordered pairs; smallest shape 3 + 6 states) |
| 3 | NO | NO | NO (3.3·10⁹ model checks) | not computed |
| 4 | NO | YES (116680 pairs) | YES | YES |
| 5–8 | NO | not computed | – | – |

So g_3(2) ≥ 9 (possibly infinite), g_4(2) = g_5(2) = 4, g_s(2) = 2 for s ≥ 6.

**n = 3**

| b | s=3 | s=4 | any s |
|---|---|---|---|
| 2 | NO | NO | NO (PROVED, T2) |
| 3 | NO | NO | NO (PROVED, T3) |
| 4 | NO | NO (116680 two-process solutions, no pairwise-compatible triple) | YES at s = 7 |
| 5–8 | NO (no two-process solution at s = 3) | not computed | b = 5: YES at s = 7 |

Instances asked for: n = 2, s ≤ 4, b ≤ 3: no solution in any. n = 3, s = 3, b ≤ 4: no solution.

## 2. Witnesses

Format: `n b`, then per process the state count and one line per state (`R q0 … q_{b-1}` or `A c q`; state 0 = rem, 1 = crit). All accepted by `check` (C++) and `slowcheck.py` (independent Python).

- `w_n2_b2_s6.txt` — g(2) = 2. P1: add 1; wait for R = 1; crit; add 1. P2: wait for 0; add 1; read: 1 → crit, 0 → add 1 and wait again.
- `w_n2_b4_s4.txt` — 4 states suffice with b = 4 (found by search).
- `w_n3_b4_s7.txt` — g(3) ≤ 4: Burns' algorithm, flags are the summands 1, 1, 2 mod 4.
- `w_n4_b8_s7.txt` (weights 1,1,2,4), `w_n5_b14_s7.txt` (1,1,2,5,5).
- "Next better value impossible" for n = 3 is the proof T3, not a computation.
- Rejections seen: `bad_n2_b2.txt` (SAFETY); `gen_burns.py 3 1 1 1` refused as ambiguous; `crosstest.py` ran 2138 mutated/random systems (930 SAFETY, 921 PROGRESS, 172 ILLEGAL, 115 OK) with zero disagreements between the two checkers.

## 3. Headline

**g(3) = 4.** Lower bound proved for every s (T3); upper bound by a model-checked 7-state witness. Confidence high; the remaining risk is an error in T3, which no machine has checked.

General n: **not settled.** Proved: g(n) ≥ 4 for n ≥ 3. Upper bounds: g(4) ≤ 8, g(5) ≤ 14 (model-checked), g(n) ≤ 2^(n−1) (sketch). No conjectured formula. The proposer's guess 4 is right, but its reason ("identities are not needed") is backwards: b = 3 fails because a process cannot tell whom to yield to.

## 4. Proofs (`proofs.txt`)

- L0 COMPLETE: g_s(n) is monotone in n and s.
- T2 COMPLETE: b = 2 fails for n ≥ 3 (the statement's argument, made exact).
- T3 COMPLETE: b = 3 fails for n ≥ 3. Outline: indistinguishability forces that a process alone enters only from R = 0, every first add is 1, R = 1 in a solo crit, exit = add 2. Each process then has a chain of levels: raised and reading 2, it either never changes R (hold) or its first change is −1 (yield), after which, reading 0, it raises by 1 again. The third process, as a helper that leaves through crit, makes every level reachable jointly with any level of another process. Two processes whose chains end in a hold level deadlock; two with infinite chains livelock in lockstep. Three processes, two types: contradiction.
- T6 SKETCH: g(n) ≤ 2^(n−1), weights 1,1,2,…,2^(n−2); decoding proved, correctness of the single-read Burns variant for all n only cited. Checked for n ≤ 5.

## 5. Interpretation

- "At most s states" allows unreachable states; crit's action may be a read.
- Progress read literally: after T only processes that move forever may be active; the rest sit in rem and never step. Implemented per set M of movers: no strongly connected set of crit-free configurations, others in rem, in which every member of M moves.
- No ambiguity changed an answer. "1,1,2 mod 5" works and so does mod 4.
- Not done: n = 2 with b = 3, s = 6; n = 3 with s = 5; any lower bound above 4 for n ≥ 4.

## 6. Code

- `mc.h`, `check.cpp` — model checker; `slowcheck.py` — reference checker; `crosstest.py` — comparison.
- `enum.cpp` — first enumerator (n = 3, s = 3, b ≤ 6; n = 2 small cases; count 19300 in `log_old_enum_b2_s6.txt`).
- `enum2.cpp` — stronger pruning, all other entries; pair lists `pairs_*.txt`, logs `log_*.txt`.
- `gen_burns.py` — weighted Burns generator and least-modulus search (2, 4, 8, 14 for n = 2..5).

Housekeeping: `pairs_b4_s4_p0/p1.txt` are about 650 kB each, over the 300 kB guideline. `log_b3_s5_p0/p1.txt` come from a run that hit the time limit and carry no result.
