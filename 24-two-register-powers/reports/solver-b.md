# solver-b report

All files are in `solver-b/`. Moves: A=(2a,b), B=(a,2b), C=(a+b,b), D=(a,a+b).

## 1. Tables

**N(t), t = 0..21 — COMPUTED** (exhaustive level BFS over unordered integer pairs, `gbfs.cpp`; t <= 12 also by a no-symmetry Python BFS `slow.py`; t <= 15 also from the bounded BFS `gbound.cpp`):

1, 2, 4, 7, 12, 20, 34, 57, 98, 169, 295, 521, 912, 1614, 2864, 5093, 9115, 16337, 29401, 52912, 95363, 171995

Asked range was t <= 12; N(12) = 912. Ratios N(t+1)/N(t) for t = 16..20: 1.792, 1.800, 1.800, 1.802, 1.804 (still creeping up).

**G(n) for all n <= 32768 — COMPUTED** (`gbound.cpp`, BFS restricted to registers <= 2^15; the restriction is safe because registers never decrease; file `g_15.txt`).

**D(m) for 2 <= m <= 4096 — COMPUTED** (BFS on all of Z_m x Z_m, no pruning, `dm.cpp`; m <= 400 reproduced exactly by `slow.py`). Full table: `D_table.txt`. D(2..40) = 1,2,2,3,3,4,4,4,4,5,5,5,5,6,6,5,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,8,8. D(400) = 12. D is not monotone (D(16)=6, D(17)=5).

| d | first m with D=d | last m <= 4096 with D=d |
|---|---|---|
| 5 | 11 | 17 |
| 6 | 15 | 26 |
| 7 | 27 | 41 |
| 8 | 39 | 73 |
| 9 | 71 | 131 |
| 10 | 119 | 209 |
| 11 | 191 | 389 |
| 12 | 369 | 561 |
| 13 | 511 | 1066 |
| 14 | 987 | 1829 |
| 15 | 1647 | 3137 |
| 16 | 2877 | (4096) |

Observations (COMPUTED, m <= 4096, not proved): with T(m) = min{t : N(t) >= m}, D(m) - T(m) is 0 (358 cases), 1 (3734) or 2 (3). D(m)/log2 m lies in [1.29, 1.39] for 2048 <= m < 4096; the octave minimum is 1.29, 1.28, 1.29, 1.29 for the octaves starting at 64, 256, 1024, 2048.

All problem-statement values checked: D(2..8) = 1,2,2,3,3,4,4 and N(0..3) = 1,2,4,7 are correct; ceil(log2 m) <= D(m) <= 2 floor(log2 m) holds for every m <= 4096 (and is proved, P1).

## 2. Witnesses

- `dm128w.out`: for every m <= 128 and every residue r, a move string of length g_m(r). `check_w.py` (independent, replays on integers) ACCEPTS it, and REJECTS two deliberately corrupted copies (`bad_w.txt` wrong path, `bad_w2.txt` wrong D).
- Lower bounds (no shorter path exists) are exhaustive-search facts; the evidence is agreement of two independent programs (`slow.py` vs `dm.cpp` for m <= 400; `slow.py`, `gbfs.cpp`, `gbound.cpp` for N).
- `g_15.txt`: n, G(n) for n <= 32768; N(t), t <= 15, follows by counting.

## 3. Headline

**YES, L > 1. PROVED (COMPLETE proof, `proofs.txt` P2): L >= 1/0.956 > 1.046.** Confidence in the yes/no: very high (short proof, lemma also checked numerically by `potential_check.py`).

Value of L: NOT determined. Proved: 1.046 <= L <= 2. Guess only: L = 1/log2(lambda) with lambda = lim N(t)^(1/t); the data (ratios ~1.80 and rising, D within 2 of the counting bound T(m)) suggest L somewhere around 1.1-1.2. I have no proof of any upper bound below 2, nor that the limit lambda exists.

## 4. Proofs (full text in `proofs.txt`)

- P0 COMPLETE: mod-m BFS equals the integer definition (moves commute with reduction; paths lift).
- P1 COMPLETE: N(t) <= 2^t; D(m) >= T(m) >= ceil(log2 m); D(m) <= max_{n<=m} G(n) <= 2 floor(log2 m) (binary method).
- P2 COMPLETE (main): for an unordered state x <= y put M = y, r = x/y, Phi = log2 M + (1/4) log2 max(r, 1/4). Doubling the larger register raises Phi by at most 1; each of the other three move types (double the smaller, add smaller to larger, add larger to smaller) raises Phi by at most 3/4 (four-case calculation). So a path of j steps with k non-"double the max" steps ends with log2 M <= j - k/4 + 1/2. Any n > 2^(0.956 t) with G(n) <= t therefore comes from a word with k <= 0.176 t + 2, and there are at most 2^((0.9502+o(1)) t) such words. Hence N(t) <= 2^(0.956 t + 1) for large t, lambda <= 1.9401, D(m) >= (log2 m - 1)/0.956. Constants not optimised.
- P3 COMPLETE in one direction: popcount(n) <= 2 implies G(n) = ceil(log2 n). Converse COMPUTED for n <= 32768 only.
- No sketches are presented as proofs.

## 5. Problem-statement issues

- **Wrong claim:** "G(n) = ceil(log2 n) only for powers of 2" is false: G(3) = 2, G(5) = G(6) = 3 (the statement's own cost list shows this). Computed for n <= 32768: G(n) = ceil(log2 n) exactly when n has at most two 1-bits.
- The guess "non-forced steps lose a constant fraction of a bit" holds only amortised: one such step can gain almost a full bit (ratio near 1), hence the potential term.
- Interpretation: states are ordered pairs, but the move set is swap-symmetric, so unordered pairs give the same G; mod m, 0 is an ordinary value and g_m(0) = min G over multiples of m. No other ambiguity found; no alternative reading needed.

## 6. Code -> numbers

- `slow.py` (reference): N(0..12), D(2..400) -> `slow400.out`
- `gbfs.cpp`: N(0..21) -> `gbfs21.out`
- `gbound.cpp`: G(n), n <= 2^15 -> `g_15.txt`, `gbound15.out`
- `dm.cpp`: D(m), m <= 4096 -> `dm1100.out`, `dm4096.out`, `D_table.txt`; witnesses `dm128w.out`
- `check_w.py`: witness checker; `analyse.py`: bounds, D - T(m), ratios; `potential_check.py`: lemma, inequality (*) on all 5,592,405 words of length <= 11, exponent arithmetic, and a deliberately wrong parameter (c = 0.4) that fails as it should.
