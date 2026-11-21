# Report: upper bounds for experiment 16 (erasing counter)

W_r = 2, 6, 42, 1806, 3263442 (Sylvester minus 1). L(T) is the shape function L(leaf) = 1,
L(node) = L(1-child)·(L(0-child) + 1); a tree of depth ≤ r has L ≤ W_r.

## Strongest statement

Goal (A), D(r) ≤ W_r for all trees, is **not** proved. What is proved (all in `PROOFS.md`):

1. **PROVED — Theorem E (all trees, any depth): every cycle contains at most one step that uses an empty
   leaf.** Solver B's observation (computed for r ≤ 3); the referee's follow-up 1.
   Proof idea: cut the cycle where the all-zero path holds no 1; between cuts the steps only erase and
   then write once, so the return map is again a tree of the model, on fewer indices, with the same
   empty steps; induct.
2. **PROVED — Theorem RD: if the two root subtrees use disjoint index sets, the longest cycle is at most
   M(T1°)(M(T0°) + 1)**, hence ≤ D(r−1)(D(r−1) + 1). (Referee's follow-up 2; uses Theorem E.) With
   D(4) = 1806 from the repository: a depth-5 tree beating 3263442 must reuse an index across the root.
   Iterated: any tree of depth r beating W_r has two equal-index nodes in different branches whose
   lowest common ancestor is at depth ≤ r − 5.
3. **PROVED — Theorem RO (exact value under a restriction): for trees in which no index labels two
   nodes, the longest cycle is exactly L(T) for every shape, so exactly W_r at depth r, for every r.**
   The construction in the repository is read-once, so the conjecture D(r) = W_r is now equivalent to
   "reusing an index in different branches never helps".
4. **PROVED — Theorem G (all trees): D(r) ≤ (r + 1)·2^(2^r − 1 − r)** (6, 64, 10240, 4.03e8 for
   r = 2..5). A small gain over 2^d − 1.
5. **COMPUTED — Conjecture P1: every cycle of every tree has length ≤ L(T).** Holds for all 5,478,539
   trees of depth ≤ 3 up to relabelling (`brute3.cpp`, `out_brute3_r3.txt`) and for all 604,143,675
   closed cycles of trees of depth ≤ 4 (`lazy2.cpp`, `out_lazy2_r4.txt`). At depth ≤ 3 equality occurs 26 times,
   once per shape, always read-once; at depth ≤ 4, 677 times (the number of shapes). P1 implies
   D(r) = W_r. Both programs also confirm Theorem E (maximum 1).
6. **Goal (C) — where the induction breaks, smallest configurations (COMPUTED, checked by hand):**
   - product bound of RD fails with sharing at depth 2, two indices: (1 (2 [1] []) (2 [2] [2])) has a
     3-cycle, bound 2. The 1-subtree writes a bit the 0-subtree reads, so the 0-subtree's memory is no
     longer on one of its own cycles.
   - "root = 1 states ≤ L(T1)" fails at depth 3, four indices: (1 (2 [1] (3 [3] [])) (2 (4 [4] [2]) [])),
     10-cycle with four root = 1 states, L(T1) = 3. The 1-subtree reads an index only to write it and
     thereby borrows a subtree of the other side as a second digit.
   The missing idea is a count that survives this borrowing; P1 is the candidate statement.

Why the values match for r ≤ 4: the optimum keeps (i) one empty step per cycle and (ii) while a
node's bit is 1, its 0-subtree's memory frozen in one fixed state; for read-once trees (i) forces
(ii), which gives t → t(t + 1).

## Tried and failed

- Induction for P1 through the reduction of Theorem E with L as measure: the reduced tree can contain
  several copies of one subtree and its L is far too large.
- Strengthened hypotheses, all FALSE at depth 3 (counts in `out_brute3_r3.txt`): per-leaf usage caps;
  #(root = 1 states) ≤ L(T1); orbit segments without empty steps ≤ W (43 occurs); injectivity of the
  natural map from cycle states to the L(T) "configurations".
- A brute-force run for depth 4 on four indices timed out; nothing is claimed from it.
- Goal (D): nothing found that suggests the formula fails.

## What a reader must still trust

- The proofs are by one model and unreviewed. Theorem E's construction was tested on 40,000 trees
  (`check_F.py`), Theorem RD on 383,161 trees (evidence, not proof-checking).
- Statement 2's consequence for depth 5 uses D(4) = 1806 from the repository.
- `lazy2.cpp` follows the referee's search design (I had read it); same node count as the referee's. `brute.cpp` is an independent full enumeration, r ≤ 3 only.
- Every check was watched failing on a wrong input: wrong L, wrong notion of empty leaf, wrong product
  bound, wrong signature, two wrong constructions of the reduced tree (`out_*mut*.txt`,
  `out_lazy2_small.txt`, `out_checkF.txt`).

## Files (all in .)

`PROOFS.md` (theorems, examples, list of computations); `brute.cpp`, `brute2.cpp`, `brute3.cpp`,
`lazy2.cpp`, `check_F.py`; outputs `out_*.txt`, `err_*.txt`. Nothing was written outside this folder.
