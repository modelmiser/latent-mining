# Solver B report

## 1. Tables

| r | d | D(r) | status |
|---|---|------|--------|
| 1 | 1 | 2 | COMPUTED, all 5 trees (slow.py) |
| 2 | 3 | 6 | COMPUTED, all 1201 trees, no symmetry (slow.py); again by lazy.cpp |
| 3 | 7 | 42 | COMPUTED three ways: ex3 `canon` (27.6M trees after symmetry), ex3 `full` (root fixed only, 41336² = 1.71e9 trees), lazy.cpp |
| 4 | 15 | 1806 | COMPUTED, one method only (lazy.cpp, 1.3e10 search nodes, all 16 start weights complete) |
| 5 | 31 | >= 3263442 | lower bound only (witness checked) |

Longest cycle by minimum weight w on the cycle (lazy.cpp; the r=3 row reproduced independently by brute force, ex3w):
- r=3, w=0..7: 42, 36, 26, 16, 11, 7, 3, 1
- r=4, w=0..15: 1806, 1764, 1272, 900, 610, 400, 262, 172, 100, 64, 39, 23, 15, 7, 3, 1

Pruning used, and why it is safe:
- ex3: root index fixed to position 1; in `canon` the 0-subtree's indices are fixed up to relabelling of positions 2..7. `full` drops the second reduction and gives the same maximum (42, 720 optimal trees).
- lazy.cpp: the tree is decided node by node, only when the orbit of the start state reaches an undecided node. The start is a minimum-weight state of the cycle, relabelled to "ones on the top w positions". Positions not yet used in the tree and with equal start bit are interchangeable, so only the lowest is tried. A branch ends when the orbit returns to the start (length recorded), hits another visited state, or drops below weight w.
- An unsafe pruning in either program could only lower the reported value. The r=4 value is also met from below by a witness, so an error would mean D(4) > 1806.

## 2. Witnesses (files in solver-b/)

- `witness_r1.txt` .. `witness_r5.txt`: tree, start state, cycle length. Format: `(i T0 T1)` internal, `[]` / `[i]` leaf.
- r=2: `(1 (2 [2] [1]) (3 [3] []))`. r=3: `(1 (2 (3 [3] [2]) (4 [4] [1])) (5 (6 [6] [5]) (7 [7] [])))`. Start all-zero.
- `check.py r file` validates the tree against the definition and follows the orbit. It accepts all five (2, 6, 42, 1806, 3263442).
- Deliberate failures, all rejected: wrong length (`bad_N.txt`), altered leaf (`bad_leaf.txt`), repeated index (`bad_idx.txt`), depth-4 tree checked as depth 3.
- "Next value impossible" has no short certificate: for r <= 4 it is the exhaustive search itself (outputs `ex3_*.out`, `lazy4_*.out`, `lazy4_summary.txt`).

## 3. Headline

- **log2 D(r) is NOT polynomially bounded in r. PROVED.** 2^(r-1) <= log2 D(r) <= 2^r - 1, so log2 D(r) = Theta(2^r). The problem's GUESS is wrong.
- **D(2) = 6, D(3) = 42, D(4) = 1806** (computed).
- **Conjecture (not proved): D(r) = a_r**, where a_1 = 2 and a_r = a_(r-1)·(a_(r-1)+1), i.e. Sylvester's sequence minus 1: 2, 6, 42, 1806, 3263442, ... The lower bound D(r) >= a_r is proved for all r; equality is computed for r <= 4. Confidence that equality holds for all r: moderate (about 75%). I have no general upper bound better than 2^d - 1.

Why the guess fails: unread bits are a counter that persists. The root's 1-subtree is a full depth-(r-1) counter of period a; the root plus its 0-subtree is a counter of period a+1 whose single "overflow" leaf writes the root instead of nothing, which triggers one step of the other counter.

## 4. Proofs (proofs.txt)

- Theorem 1, COMPLETE: D(r) >= a_r by the recursive construction W_r, hence log2 D(r) >= 2^(r-1).
- Theorem 2, COMPLETE (trivial): D(r) <= 2^d.
- Remark 4, COMPLETE: the problem's weight claims are true.
- SKETCH only: D(r) <= 2^d - 1 for r >= 2.
- Observation, COMPUTED for r <= 3, not proved (prod3.cpp): on every cycle of length >= 2, at most one step uses an empty leaf. It follows that no depth-4 tree whose two root subtrees use disjoint positions beats 1806 (consistent with the exhaustive r=4 result).

## 5. Problem statement

- **The claim "for r >= 2 the all-ones state has no preimage" is false.** Paths may be shorter than r, so `(1 [1] [])` is a legal depth-r tree; it sends 011..1 to 111..1 and back, a 2-cycle through all-ones. The bound 2^d - 1 probably survives (sketch), and no reported number depends on it.
- Otherwise no ambiguity. I used d = 2^r - 1 exactly, allowed short paths and the single-leaf tree, and counted cycles of length 1.
- The claim 6 <= D(2) <= 7 resolves to 6.

## 6. Code

- `slow.py`: direct definition; D(1), D(2); also a second checker (`slow.py check`).
- `ex3.cpp`: D(3), modes `canon` and `full`. `ex3w.cpp`: r=3 table by minimum weight.
- `lazy.cpp`: D(1)..D(4) and the per-weight tables.
- `prod3.cpp`: empty-leaf observation and the disjoint-product bound.
- `mkwit.py`: witnesses. `check.py`: independent witness checker.
