# Solver B report — cut-and-peek AND

## 1. Tables

Notation: P_i′ = positions of P_i where u_i^0 ≠ u_i^1; "weak" = private and M(1ⁿ) ≠ M(0ⁿ) but supports overlap (not valid under the text).

| quantity | value | status |
|---|---|---|
| E(2,m), m = 2,3,4 | 0 | COMPUTED, exhaustive over the raw definition (`slow.cpp`: all owners, all word pairs, all non-empty R, no pruning) |
| E(2,m), m = 5..12 | 1 (also with R = Z_m) | COMPUTED (`slow` for 5–7, `fast` for 5–12), witnesses re-checked by `check.py` |
| r(2) = f(2) | 5 | COMPUTED |
| min \|R\| for n=2, m = 5..12 | 4,3,4,4,5,4,4,3 | COMPUTED (`fast`; `slow` agrees for m ≤ 7) |
| E(3,m), m ≤ 6 | 0 | COMPUTED raw (`slow`); m ≤ 5 also by proof |
| E(3,m), m = 7..15 | 0 | COMPUTED, pruned exhaustive (`fast` ≤ 12, `fast2` 13–14, `fast3` ≤ 15) |
| f(3), f(n) for n ≥ 3 | ∞ | PROVED (T2) |
| E(4,m), m ≤ 14 | 0 | PROVED (T1: m ≥ 2ⁿ−1); m ≤ 12 also COMPUTED |
| weak E(3,m) | 0 for m ≤ 11; 1 for m = 12,13,14,15 | COMPUTED (`fast3`): 216, 1152, 7596, 34152 canonical (layout,R) pairs |

n=3 search: 1.58·10¹⁰ layouts at m=15 (8 chunks). Leaf totals for m = 6..15 equal an independent closed-form count (`leafcount.py`). The weak non-existence for m ≤ 11 rests on `fast3` alone (no slow cross-check).

## 2. Witnesses (`check.py` reads them; plain text)

- `witnesses_n2.txt`: 23 valid n=2 protocols, m = 5..12 (first found, min-|R|, full-R). E.g. m=5: `owner=0H101 c0=00011 c1=10100 R=11111`.
- `weak_n3.txt`: five n=3 weak examples (m = 12..15), e.g. m=12 `owner=0H0H121H0H02 c0=000010001011 c1=101001100000 R=110000000100`.
- `bad.txt`: three deliberately wrong objects, all rejected. `t5test.py` also fails on purpose on the weak examples.
- Non-existence for n=2, m ≤ 4 and n=3, m ≤ 6: `slow.out`. For n=3, m = 7..15: output logs only (`fast*.out`, `m15.part*.out`); there is no compact certificate.

## 3. Headline

**Not settled.** r(3) ≥ 16 (COMPUTED); finiteness is open.

GUESS: r(3) = ∞, confidence about 55%. The problem's stated reason for that guess is wrong: privacy is *not* the obstruction, since perfectly private three-player protocols with M(111) ≠ M(000) exist from m = 12 on. What fails is zero-error correctness. Proved constraints on any valid n=3 protocol: 5 ≤ |R| ≤ m−2, every translate of R contains a flipping card of every player, the seven AND-0 rows are not all rotations of one word. No conjectured formula.

General form: r(2) = 5; r(n) ≥ max(2n, 2ⁿ−1) PROVED; the set {n : r(n) < ∞} contains 2 and I cannot say more.

## 4. Proofs (all in `proofs.txt`, each COMPLETE unless stated)

- **L0, L1** (belief (a)): true. WLOG complement words; |P_i′| even, half hearts; all 2ⁿ rows have equal heart count; m ≥ 2n.
- **T1** (belief (b), plus new bound): for some S ⊆ R every non-zero parity vector in F₂ⁿ occurs among the translates of S. Hence m ≥ 2ⁿ−1, some translate of R meets every P_i, |R| ≥ n, and w(1ⁿ) shares all k-point counts in R for k < n.
- **T2**: for n ≥ 3, if the seven AND-0 rows of some three players (others fixed to 1) are rotations of one word W, then w(1ⁿ) is the rotation of W by s₁₂+s₁₃+s₂₃−s₁−s₂−s₃. So f(n) = ∞ for all n ≥ 3. Machine test `fullR.cpp` (m ≤ 18) is vacuous for n=3: no shift triple with seven rotations exists at all.
- **C3**: for n ≥ 3, |R| ≤ m−2.
- **T4** (sign lemma): |R| ≥ n+1 for all n; |R| ≥ 5 for n=3. Not machine-tested for n=3 (nothing to test on).
- **T5** (covering): every translate of R meets every P_i′. Machine-checked on all valid n=2 pairs, m ≤ 11 (`fast4.cpp`, 0 violations).
- **Not proved**: E(3,m) = 0 for all m. No sketch offered.

Status of the problem's beliefs: (a) true; (b) true; (c) true (computed); (d) conclusion true, superseded by T2; (e) conclusion true for all n ≥ 3 without the recalled theorem; (f) false as worded for n=2 (the five-card trick is of that form), true for n ≥ 3 by T2.

## 5. Interpretation

- Took the text literally: M(x) is a multiset of m words; correctness = disjoint supports of M(1ⁿ) and M(0ⁿ). The other natural reading (weak) is reported separately above and gives a different answer for n=3: weak r(3) = 12.
- E(n,m) is not obviously monotone in m, so I report E per m rather than only r.
- `token.py` is an abandoned exploration; nothing is claimed from it.

## 6. Code

All in `solver-b/`. `slow.cpp` → `slow.out`. `fast.cpp` → `fast.out` (n=2 and n=3, m ≤ 12). `fast2.cpp` (adds clique pruning on R, all-players filter, chunking) → `fast2.out` (n=3, m = 13, 14). `fast3.cpp` (adds weak count) → `fast3.out`, `m14w.part*.out`, `m15.part*.out`. `fast4.cpp` → `fast4.out`. `fullR.cpp` → `fullR.out`. Pruning safety arguments are in `proofs.txt`. A wrong pruning would bias towards E = 0, the reported answer, so on n=2, where positives exist, `fast2` (m ≤ 12) and `fast3` (m = 5,6,8,10,12) had to reproduce `fast`'s valid-pair counts exactly, and `fast` had to match `slow` (m ≤ 7) on E, min/max |R| and the full-R flag.
