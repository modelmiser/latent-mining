# solver-b report

All files are in `solver-b/` (index: `README.txt`).

## 1. Tables

| n | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
|---|---|---|---|---|---|---|---|---|----|----|
| A(n) | 1 | 3 | **6** | **9** | **12** | **16** | 19 | 23 | 27 | ≥ 30 |

All entries COMPUTED by exhaustive minimax:

- n ≤ 5: direct definition, no symmetry, no pruning (`slow.py`).
- n ≤ 8: full value table over positions up to relabeling (`fast.cpp`); n ≤ 6 also without symmetry reduction; n ≤ 8 with two different canonical forms.
- n ≤ 7: tables re-verified by a separate program matching children over all n! relabelings (`check_table.cpp`).
- n = 8, 9: threshold search "can Delayer force ≥ t?" with transposition table (`ab.cpp`, `ab2.cpp`); n = 9: YES at 23, NO at 24 in four runs (two engines, with/without static cut-offs).
- n = 10: YES at 27, NO at 28; two runs of `ab2.cpp` with different move orders (`n10d.log`, `n10e.log`). No independent engine for the NO side.
- n = 11: YES at 30 only (`n11c.log`); upper side not reached. Not exact.

Pruning: transposition bounds plus static bounds proved in `proofs.txt` (Lemmas 1–3) and "value ≤ number of live pairs". A wrong lower-bound lemma would bias up, a wrong upper-bound lemma down. Values for n ≤ 9 are the same with the lemmas off.

Sanity: 2n−4 ≤ A(n) ≤ n(n−1)/2; A nondecreasing; restricting Hastener's replies never lowered a value.

Side family (COMPUTED, exact unless marked): V(m,r) = value of the position "m agents who each know exactly those m secrets, plus r untouched agents".

| m \ r | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|
| 2 | 2 | 5 | 8 | 11 | 15 | 18 |
| 3 | 3 | 7 | 10 | 14 | 17 | 21 |
| 4 | 4 | 9 | 13 | 16 | 20 | 24 |
| 5 | 5 | 11 | 15 | 19 | 23 | ≥27 |
| 6 | 6 | 13 | 17 | 22 | 26 | ≥29 |
| 7 | 7 | 15 | 20 | 24 | | |
| 8 | 8 | 17 | 22 | 26 | | |
| 9 | 9 | 19 | 24 | | | |

V(m,1)=m is proved (Lemma 1; only m live pairs), so the m = 8, 9 entries of that column were not run. V(m,2)=2m+1 and V(m,3)=⌊(7m+11)/3⌋ hold on the computed range (observations).

## 2. Witnesses

- `table4.txt` … `table7.txt`: complete value tables (both players' sides); `./check_table n tableN.txt` prints OK and A(n).
- `cert_n4_ge6.txt`, `cert_n5_ge9.txt`, `cert_n6_ge12.txt`, `cert_n7_ge16.txt`, `cert_n8_ge19.txt` (6 MB): explicit Delayer strategies; format in each file's header. Checked by `check_cert.py` (pure Python, no search).
- Deliberate failures: corrupted tables and certificates are rejected (`failtests.log`).
- No compact witness for n = 9, 10 (certificates would be ≥ 100 MB) nor for the upper bounds at n ≥ 8.

## 3. Headline

**NO.** It fails at the smallest possible n: **A(4) = 6**, not 5 (certain: hand proof plus three programs). A(n) > 2n−3 for every computed n from 4 to 11. A(4)=6, A(5)=9, A(6)=12, A(7)=16.

Exact function: **not found.** Not settled: whether A(n)/n is bounded.

- Increments A(n)−A(n−1) for n = 3..10: 2, 3, 3, 3, 4, 3, 4, 4. A(n)/n rises 1.5 → 2.7.
- A(n) = 3n−6 for 4 ≤ n ≤ 6 only; 3n−5 at 7, 8; 3n−4 at 9; 3n−3 at 10.
- Fit, not a claim: A(2k) = (k²+7k−6)/2 holds for k = 1..5; odd n (3, 9, 16, 23) do not follow a matching quadratic.
- Observed, unproved: A(n) = (m−1) + V(m, n−m) in every exact case with n−m ≥ max(2, m−2), n ≤ 10. If it persists, A(11) = 31. GUESS.
- GUESS, low confidence: A(n)/n is unbounded. The data cannot separate that from a line of slope about 4.

The brief's beliefs: A(2)=1, A(3)=3 correct; A(n) ≥ 2n−4 holds for n ≤ 11 (not re-proved in general); "A(4)=5 by hand" wrong; the closing guess wrong; the fixed-agent and no-linear-bound remarks not tested.

## 4. Proofs (`proofs.txt`)

- Lemma 1 COMPLETE: a call adds at most one new knower of a given secret, so value ≥ n − #knowers(s).
- Lemma 2 COMPLETE: value ≥ ⌈(non-full agents)/2⌉.
- Lemma 3 COMPLETE: if a full agent exists and every non-full agent misses some full agent's secret, value ≤ number of non-full agents.
- Theorem A COMPLETE: A(4) = 6. Delayer: after {1,2} name 1 (forced {1,3} up to symmetry), then name 2. Reply 3 leaves agent 4 untouched: three more calls by Lemma 1. Reply 4: name 2 (forced {2,3}), then name the full agent 3 (forced {3,4}), and agent 1 still needs a call. Upper bound: only 6 pairs.
- No general-n statement is proved. No sketches offered.

## 5. Interpretation

No ambiguity found; rules taken literally. No alternative reading computed.

## 6. Code per number

- A(2..5): `slow.py`. A(2..8), tables, optimal lines: `fast.cpp`. Table checks: `check_table.cpp`.
- A(8), A(9): `ab.cpp`; A(4..10), A(11) ≥ 30, V(m,r): `ab2.cpp` (`vmr.py`, `vmr2.py` drive it; binaries `ab2_v3/v4/v5` are earlier option settings of the same source, mapped in `README.txt`).
- Certificates: `ab2.cpp` with `CERT=`, checked by `check_cert.py`.

Process note: a few sub-second runs (n ≤ 8 certificates) overlapped two long runs, briefly exceeding the two-process limit.
