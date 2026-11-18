# SPEC: what `FO/Spec.lean` says, and every choice it makes

Core Lean 4 (toolchain `leanprover/lean4:v4.36.0`), no Mathlib, no other dependency.
Build: `./b.sh` (runs `lake build` on cores 4,5, niced, `ulimit -v 16000000`, `timeout 1500`;
the lakefile passes `-j 2` to lean because more worker threads do not fit under that limit).
`Axioms.lean` regenerates `AXIOMS.txt`.

## Definitions and the sentence each one encodes

| Lean (`namespace FO`) | Problem statement (`chain/3-question.txt`) |
|---|---|
| `Arc n := Fin n × Fin n`, `(t, h)` = edge `{t,h}` directed from tail `t` to head `h` | "D assigns to every edge of G one of its two directions" |
| `Pos n := List (Arc n)`, one arc per edge, newest edge first; `[]` is the start | "A position is a pair (G, D) ... The start position has no edges." |
| `outdeg D v` = number of arcs with tail `v`; `valid D` = every `v` has `outdeg D v ≤ 2` | "every vertex is the tail of at most 2 edges. Call such a D valid" |
| `conn D a b` (recursion on the edge list) | "connected components of G" |
| `moves D` = all ordered pairs `(u, v)` with `conn D u v = false` | "Adversary names a pair {u, v} of vertices in different connected components of G. Every such pair is legal." |
| `reorient D` = all `2^|D|` lists obtained by keeping or reversing each arc in place; `replies D u v = (reorient ((u,v) :: D)).filter valid` | "Algorithm chooses any assignment D′ valid for G + uv ... may change edges anywhere in the forest and more than necessary" |
| `flips`, `cost D D' = flips D D'.tail` (the head of `D'` is the new edge) | "pays the number of edges e of G (old edges only) with D′(e) ≠ D(e). The new edge uv costs nothing in either direction." |
| `value k D` = max over `moves D` of min over `replies` of `cost + value (k-1)`; `value 0 = 0` | "Adversary maximises and Algorithm minimises the total payment" |
| `T n = value (n - 1) []` | "exactly n − 1 rounds ... T(n) the value of this finite perfect-information game" |
| `MainStatement : ∀ n, 1 ≤ n → n + 3 ≤ 3 * T n + 7 * Nat.log2 (n + 1)` | Theorem 1 of `open-limit/proofs.txt` |

The theorem is proved as `FO.main : MainStatement` in `FO/Full.lean` and restated in
`FO/Main.lean` (`T_lower_bound`, and `T_lower_bound_pow` without logarithms).

## The integer form of the inequality

Paper: `T(n) ≥ n/3 − (7/3)·log₂(n+1) + 5/6`.  Lean: `n + 3 ≤ 3·T(n) + 7·⌊log₂(n+1)⌋`.
Divide by 3: `T(n) ≥ n/3 + 1 − (7/3)·⌊log₂(n+1)⌋`; since `⌊log₂(n+1)⌋ ≤ log₂(n+1)` and
`1 ≥ 5/6` this implies the paper's inequality.  Nothing is lost in the rounding; the Lean
statement is stronger by `1/6 + (7/3)·frac(log₂(n+1))`.  Why it is still true: the proof bounds
`3·T(n)` (an integer) below by `n − 7i/2 − ...` with an integer index `i ≤ 2⌊log₂(n+1)⌋ − 1`.
`T_lower_bound_pow`: `2^(2n+6) ≤ 2^(6·T(n))·(n+1)^14` uses only `+ * ^` on `Nat`; its `log₂` is
`T(n) ≥ n/3 + 1 − (7/3)·log₂(n+1)`.
`Nat.log2 m = ⌊log₂ m⌋` for `m ≠ 0` (core: `Nat.le_log2 : n ≠ 0 → (k ≤ n.log2 ↔ 2 ^ k ≤ n)`).

## Places where the encoding makes a choice

1. **Vertices** are `Fin n = {0, …, n−1}`, not `{1, …, n}`.
2. **"G is a forest" is not a field of a position.**  A position is any list of arcs.  Play
   starts from `[]` and only adds edges between different components, so every position that
   occurs is a forest (and a simple graph).  The proof uses this in the form `Forest`
   (`FO/Graph.lean`: built edge by edge, each edge joining two components).  That `Forest`
   coincides with "acyclic" is not proved and not needed.
3. **Different components.**  `conn` is a recursion on the edge list ("connected in G+uv iff
   connected in G, or through uv").  `FO.conn_iff` proves it equal to the reflexive-transitive
   closure of adjacency.
4. **Named pairs are ordered**: both `(u, v)` and `(v, u)` are moves.  They have the same
   replies up to the order of the head arc and the same costs.
5. **Algorithm may flip edges in other components** and more than necessary: `replies`
   contains every valid orientation of all edges.
6. **Edges are identified by position in the list**, so `flips` compares arc by arc.
7. **Empty maximum and minimum are 0** (`maxL [] = 0`, `minL [] = 0`).  If Adversary had no
   legal move, or Algorithm no legal reply, the rest of the game would be worth 0.  Both
   conventions can only make `T` smaller, so they cannot help a lower bound.  (Neither happens
   in play; that Algorithm always has a reply is proved, `exists_npath` + `valid_nreply`.)
8. **n = 1**: `T 1 = value 0 [] = 0`.  **n = 0**: `T 0 = 0` because `0 − 1 = 0` in `Nat`; the
   theorem assumes `1 ≤ n` (it is false at `n = 0`).
9. **The value is defined by backward induction on positions** (`value`), not as a sup/inf over
   strategies that see the history.  For a finite perfect-information game these agree; that
   equivalence is not formalised.
10. **Determinism of the players** plays no role in a minimax value.

## Sanity checks (`FO/SpecCheck.lean`, part of the build)

`T 0 = T 1 = T 2 = 0` by `decide`; `T 3 = T 4 = 0` by `#guard` (compiled evaluation);
`T 5 = 0` was evaluated once with `#eval` (3 minutes, not in the build).  Move lists, reply
counts, costs, and a position (two cherries on 6 vertices) whose one-round value is exactly 1.
One of my own expected values was wrong (I wrote 6 valid orientations where there are 7) and
the `#guard` caught it.  Deliberately false variants and Lean's refusals: `FalseVariants.lean.txt`
(inputs) and the comment at the end of `SpecCheck.lean`.

## DEVIATIONS

- **D1. No `sorry` placeholder.**  The brief allowed the final theorem to sit in `Spec.lean`
  with `sorry`.  Instead `Spec.lean` defines the proposition `MainStatement` and `Full.lean`
  proves it.
- **D2. The statement proved is slightly stronger than the paper's** (see above).  To get the
  integer form, the abstract core carries a slack of 2: Adversary forces every `g` with
  `3g + 7⌊log₂(n+1)⌋ ≤ n + 5`, which is the same as "the total paid `p` has
  `3p + 7⌊log₂(n+1)⌋ ≥ n + 3`".
- **D3. `h` is not defined as a number.**  `Hge D k v` is "`h(v) ≥ k`" (every directed path
  from `v` with fewer than `k` arcs ends in a saturated vertex).  Slots and types are
  `TypeGe D v p q` with all slot values shifted by one (hole = 0, out-neighbour `o` =
  `h(o)+1`): "smaller slot ≥ p" is `Hge D p v`, "larger slot ≥ q" is `q = 0 ∨ ∃ out-neighbour
  o, Hge D (q−1) o`.  `HasIdx D i v = TypeGe D v (i/2) ((i+1)/2)`.
- **D4. Lemma B** is stated for a reply at `t` with only `h(hd) ≥ p'` assumed of the other
  endpoint (that is all its proof uses), without assuming `p ≤ q`; "the other endpoint keeps its
  type" is the separate `type_keep`, which also covers every other tree.
- **D5. Lemma A(b)** is proved by a different argument from the paper's: instead of "the
  unique tree path from o′ to wᵢ starts with the edge o′x", it uses `nb` (two neighbours of
  `x` are not connected in `G − x`), from which `dag` and `disjoint` follow.
- **D6. The free reply is the path reply with a path of length 0** (`NPath D t []` means `t`
  unsaturated).
- **D7. Size bound** is `Hge D k v → 2^(k+1) ≤ n + 1` (the reachable vertices are counted
  inside `Fin n`, not inside the tree of `v`); Theorem 1 uses nothing more.
- **D8. M4 follows the shadow construction** of `reports/open-limit-referee.md` §0, not the
  "by value" reading of Lemma 1: `shadow_path` is one round of Lemma 1, `transfer` is the
  induction with the invariant `forced-on-shadow ≤ value(real) + d(L, A)`.
- **D9. `Core.Forces k s g`** has the clause `g = 0 ∨ …` (nothing to force), so it does not
  assert that Adversary has a legal move when it no longer needs one.

## Files

`Spec.lean` (M0) · `SpecCheck.lean` · `Core.lean` (M1) · `Graph.lean`, `Normal.lean`,
`Reply.lean` (M2) · `Strategy.lean` (M3) · `Full.lean`, `Main.lean` (M4) · `Axioms.lean` →
`AXIOMS.txt` · `SORRY.txt` · `FalseVariants.lean.txt` · `PROGRESS.md`.


## NOTES ADDED AFTER REVIEW (not by the author of this file)

A five-seat panel (four model families) read `FO/Spec.lean` against the problem statement and
confirmed that the definitions encode the game and that the theorem has no loophole. Points they
raised about this document and the files around it:

- Unlisted encoding choices, all harmless: (11) a position is an ordered list, so it also records
  the order in which edges were added, and values of positions with the same (G, D) are never
  identified; the tree rooted at `[]` is nevertheless the game tree. (12) `flips` on lists of
  unequal length ignores the excess; every reply produced by `replies` has the new edge at the
  head and is aligned with the old position, so `cost` compares edge by edge. (13) `value`,
  `replies` and `valid` are total on arbitrary arc lists, including non-forests and self-loops;
  only positions reachable from `[]` matter for `T`.
- Item 7: for `maxL [] = 0` the value 0 is the natural one, not a lowering; `minL [] = 0` scores
  that one move as 0, not "the rest of the game". That the Algorithm always has a reply is proved;
  that the Adversary always has a move in rounds 1..n-1 is by counting components and is not
  formalised.
- `Spec.lean` line 27 says `conn_iff` is in SpecCheck.lean; it is proved in Graph.lean.
- Sanity checks: `T 0 = T 1 = T 2 = 0` test almost nothing; the `T 5 = 0` evaluation is not in
  the build and no log of it was kept. `FalseVariants.lean.txt` holds inputs only, its list
  differs from the comment at the end of SpecCheck.lean, its last comment should read
  "n + 6 in place of n + 3", and that variant is "not derivable by that proof" rather than false.
- `Main.lean`: `T_lower_bound_pow` is a weaker corollary of `T_lower_bound`, not equivalent.
- The theorem first forces T(n) >= 1 at n = 33.
