# Report: solver-a

All files are in `solver-a/`.

## Headline

**YES, L > 1. Proved (COMPLETE, proofs.txt P4–P7): L >= 1/gamma = 1.0383**, gamma = 0.963096. Confidence in the yes: about 97%. The exact value of L is not settled; proved range 1.0383 <= L <= 2.

## 1. Tables

**N(t), t = 0..22** — COMPUTED, exhaustive (nt.cpp: every state reachable in exactly t steps, unordered pairs, dedup within a level only; t = 22 streamed from level 21). The slow direct program (slow.py) agrees for t <= 12, including every G(n) with G <= 12.

1, 2, 4, 7, 12, 20, 34, 57, 98, 169, 295, 521, 912 (t = 12), 1614, 2864, 5093, 9115, 16337, 29401, 52912, 95363, 171995, 310856 (t = 22).

Last four ratios N(t+1)/N(t): 1.7997, 1.8023, 1.8036, 1.8074.

**D(m), 2 <= m <= 4100** — COMPUTED, exhaustive BFS on Z_m x Z_m with a visited array (dm.cpp); file D_all.txt, lines `m D #worst first-worst-residue`. slow.py (level sets, no visited pruning) agrees on every m <= 400, all four columns. D(2..8) = 1, 2, 2, 3, 3, 4, 4 as stated.

| d | first m with D = d | last m <= 4100 with D = d |
|---|---|---|
| 11 | 191 | 389 |
| 12 | 369 | 561 |
| 13 | 511 | 1066 |
| 14 | 987 | 1829 |
| 15 | 1647 | 3137 |
| 16 | 2877 | (beyond range) |

D is not monotone in m. D(400) = 12. Smallest D(m)/log2 m for m >= 400 is 1.2914 (m = 3137); still drifting down.

Checks on the stated easy facts, all m <= 4100:
- ceil(log2 m) <= D(m) <= 2 floor(log2 m): no violation (also proved, P1–P2).
- D(m) >= min{t : N(t) >= m}: no violation (proved, P1). It is nearly tight: gap 0 for 358 values of m, 1 for 3738, 2 for 3.
- N(0..3) = 1, 2, 4, 7 and the listed costs: correct.

## 2. Witnesses

- **paths_m400.txt** (1.35 MB certificate): for every m <= 400 and every residue r, a move word over A (a=2a), B (b=2b), C (a=a+b), D (b=a+b) of length <= D(m) reaching r. **check_paths.py** replays them over the integers; all pass. Seen failing on purpose: a doubled first move on one line is rejected, and lowering the claimed D(389) by one gives 107 rejections.
- **worst_residue_m400.txt**: `m D r` with g_m(r) = D(m). That r cannot be reached in D-1 steps is only checkable by exhaustive search (slow.py D 400 re-derives it in about a minute).
- For the headline: the exact integer bound U(t) of P6 (analyze.py) satisfies U(t) >= N(t) for all t <= 22, and log2 U(t) / t = 0.9979, 0.9803, 0.9717, 0.9674, 0.9652 at t = 100, 200, 400, 800, 1600.

## 3. Headline argument and conjecture

Proof idea (full text in proofs.txt):
- A word with k doublings and j additions gives values <= 2^k F_{j+2} (P4), so values near 2^t need few additions.
- A and B commute, so the state depends only on the addition sequence and the (#A, #B) counts in each gap: at most 2^j C(k+2j+1, 2j+1) classes (P5).
- Hence N(t) <= sum over s <= t, j of min(2^j C(s+j+1, 2j+1), 2^(s-j) F_{j+2}) = 2^(gamma t + o(t)), with gamma = max over d of min(d + (1+d) H(2d/(1+d)), 1 - d(1 - log2 phi)) = 0.963096 at d = 0.1207.
- So limsup N(t)^(1/t) <= 1.9495 < 2, and m <= N(D(m)) gives D(m) >= (1/gamma - o(1)) log2 m for all m.

GUESS only: lambda = lim N(t)^(1/t) exists and is roughly 1.81–1.87, and L = 1/log2(lambda), roughly 1.11–1.17. Evidence: ratios near 1.807 and rising, and D(m) sits within 1 of the N-bound for almost all m <= 4100. Neither the limit nor the equality is proved.

## 4. Proofs (proofs.txt)

- P1 COMPLETE: N(t) <= 2^t; D(m) >= min{t : N(t) >= m}; D(m) >= ceil(log2 m).
- P2 COMPLETE: D(m) <= 2 floor(log2 m) (binary method).
- P3 COMPLETE: mod-m and integer formulations agree.
- P4–P7 COMPLETE: the headline, L >= 1.0383.
- P8 COMPLETE: G(2^x + 2^y) = x + 1 = ceil(log2 n).
- No sketches. Not proved: exact L, any upper bound on L below 2.

## 5. Problem statement issues

- **"G(n) = ceil(log2 n) only for powers of 2" is FALSE**: G(3) = 2, G(5) = G(6) = 3. Corrected statement: G(n) = ceil(log2 n) iff n has at most two 1-bits. "If" is proved (P8); "only if" is COMPUTED for all n <= 2^22 (254 such n, exactly the powers and two-bit numbers; nt_excess.txt).
- No ambiguity otherwise. Reading used: a state "holds r" if either register is r; g_m(1) = 0; residue 0 is represented by positive multiples of m.

## 6. Code

- slow.py — reference N(t), G(n) (t <= 12) and D(m) (m <= 400): slow_G.txt, slow_D.txt.
- nt.cpp — N(t) to 22, G table for n <= 2^14, excess histogram: nt22.txt, nt_G.txt, nt_excess.txt.
- dm.cpp — D(m) and paths: D_400.txt … D_3401_4100.txt, D_all.txt, paths_m400.txt.
- analyze.py — all cross-checks, U(t), gamma: analysis.txt.
- check_paths.py — certificate checker; bad_paths_test.txt, bad_D_test.txt are the deliberate-failure inputs.

Process note: one command of mine contained a stray `rm -f /dev/null`; it failed with "Permission denied" and deleted nothing.
