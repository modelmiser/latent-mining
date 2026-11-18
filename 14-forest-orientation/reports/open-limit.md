# open-limit report

Files: `open-limit/` (`proofs.txt`, scripts, logs).
All bounds are against normal-form replies.

## Proved

**Task 1 is settled: L ≥ 1/3.**

- **Theorem 1, COMPLETE.** T(n) ≥ n/3 − (7/3)·log₂(n+1) + 5/6 for every n ≥ 1.
- **State of a tree:** one vertex x and its two slot values (h of each out-neighbour, −1 for a hole).
  Types: A_k = (k−1, k−1), i.e. h(x) ≥ k, and R_k = (k−1, k).
- **Type Lemma, COMPLETE.** Naming two A_k witnesses: every reply costs ≥ k and leaves an R_k. Naming two R_k
  witnesses: every reply costs ≥ k and leaves an A_{k+1}, or costs ≥ k+1 and leaves an R_k.
- **Invariant:** paid(C) + F(type) ≥ |C|/3 for every tree, F(A_k) = k + 1/3, F(R_k) = k + 2/3. Adversary merges
  any two trees of equal type; at most 2·log₂(n+1) trees are left over.
- Inductive from every position: it reads only the current orientation. Overpaying is the lemma's second
  case and loses Algorithm 1/3. Solver B's obstacle was the fixed 4-ary schedule; pairing by current type
  removes it.

**Best constant of this family: exactly 1/3.**

- **Theorem 2, COMPLETE reduction + COMPUTED values.** The game on one pair (p, q) per tree, with any pairing
  of trees, has value V_pair(n) ≤ T(n).
- **Theorem 3, COMPLETE.** V_pair(n) ≤ (n−1)/3, by the Algorithm potential g(p,q) = q + φ(q−p),
  φ = 4/3, 2/3, 1/3, 1/3, …. So one-witness strategies give exactly 1/3, whatever the pairing or depth.
- **Corollary 7, COMPLETE.** A normal-form Algorithm that never creates h ≥ D+1 pays at least
  (1/3 + 1/(6·4^D))·n − O(D²); rate 3/8 for D = 1.

**New values (Theorem 2 + DP).** V_pair(n) = B(n) for n ≤ 29, and 7, 8, 9, 10, 12, 13, 17, 18 at
n = 30, 32, 36, 40, 46, 48, 62, 64. So T(36) ≥ 9 and T(40) ≥ 10 (uncertified in solver B's report) hold, and
**T(n) > B(n) for every n ≥ 30** (table for 30–61, monotonicity, the referee's 64-block bound beyond).

## Mechanical tests

- **Type Lemma** (`test_lemma.py`): all 5,083 oriented trees with ≤ 9 vertices, every vertex, every normal
  reply: 2,016,888 cases, 0 failures. Four false variants: 51,534 to 983,434 failures.
- **Pair Lemma** (`test_pair_lemma.py`): 371,532 cases, 0 failures. Two false variants: 169,932 and 17,178.
- **Strategy** (`sim.py`): explicit forests, n ≤ 1500, five Algorithms, 56 runs: lemma, invariant and final
  bound always hold. An Adversary naming non-witnesses gives 254 violations.
- **Bound** (`check_bound.py`): never exceeds the strategy's exact worst case, n ≤ 300. Coefficient 1
  instead of 7/3 fails at 7 values of n.
- **Theorem 2 values**: `pairdp.py` and `pairdp_noprune.py` (no pruning, other state encoding) agree for
  n ≤ 61 and never exceed the exact T(n), n ≤ 16. With a false rule (slot-1 cost p+2) the DP exceeds T(4).
- **Theorem 3** (`pair_potential3.py`): all 11,025 type pairs with entries ≤ 12, 0 violations. With the
  Adversary's F instead: 1,284 violations.

**Gaps in the checks.** I stopped the unpruned cross-check at n = 61, so the values for 62–66 (17 at 62,
18 at 64) rest on `pairdp.py` alone. A first control that removed Algorithm's two expensive options changed
no value for n ≤ 18, so it discriminates nothing.

## What failed

- **Task 2, no progress.** The bound stays T(n) ≤ n − log₂(n+1). I tested Theorem 3's potential on real trees,
  G(C) = max over vertices of g: does some normal reply always have cost + G(new) ≤ G(X) + G(Y)? True for all
  pairs of trees with ≤ 7 vertices, false at 8–9 (`test_upper.py`, `test_upper2.py`). Counterexample:
  Y = [4→3, 4→5, 3→0, 0→{1,2}, 5→6, 6→{7,8}] named at 4 against a copy of itself; every reply costs 1 and
  creates h = 2. The end of a flipped path loses a hole, and a one-hole vertex above a saturated one becomes
  deep; one witness per tree cannot see this. It is no Adversary gain (18 vertices yield 10/3 < 6).
- **Solver B's conjecture T(4^k) ≥ (4^k−1)/3 − k** is not proved for general k: my strategy pays exactly
  that against shortest-path replies (3, 18, 81), but its worst case is 17 at n = 64 and 79 at n = 256.
- **Task 3, not settled.** The pair game gives exactly B(n) for 17 ≤ n ≤ 29: evidence for least n = 30, not a
  proof. I attempted no exhaustive upper-bound search.

## Open

- L ≤ 1/3 (known: 1/3 ≤ L ≤ 1); the least n with T(n) > B(n), still in [17, 30]; the exact formula.
- My guess, not a result: L = 1/3, T(4^k) = (4^k−1)/3 − k, least n = 30.

## Confidence

- Lemmas, Theorem 1: 97%. Theorem 2 reduction: 95%; its table 95% where two programs agree, 85% beyond.
- Theorem 3: 93%. Corollary 7: 90%.
- All rests on solver-b Lemma 1 (referee-checked for n ≤ 11).
