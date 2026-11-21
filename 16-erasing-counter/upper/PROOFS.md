# Proofs for experiment 16 (upper bounds)

Model as in `16-erasing-counter/chain/3-question.txt`. In Sections 1 to 3 the depth bound plays no role:
"tree" means any finite tree of the model, of any depth, on any finite index set. idx(T) is the set of
indices at internal nodes of T. f_T is the map of T. An *empty leaf* is a leaf with R = ∅; an *empty step*
is a step that ends at an empty leaf. W_0 = 1, W_r = W_(r-1)(W_(r-1) + 1) = 2, 6, 42, 1806, 3263442.

Shape function: L(leaf) = 1, L(node) = L(1-child) · (L(0-child) + 1). By induction a tree of depth ≤ r
has L ≤ W_r, with equality only for the complete tree of depth r.

## 0. Known facts used (Lemma 2 of solver-a, restated with proof)

Let Z = (z_1, ..., z_k) be the indices on the all-zero path of T and λ0 its leaf. For a state s let
Z(s) be the number of ones of s among z_1..z_k.

(F1) Every step erases all bits it reads and writes at most one bit. So a step that reads c ones changes
the weight by 1 − c or −c. A step raises the weight only if it reads no 1, i.e. only if it ends at λ0
and λ0 writes.

(F2) If Z(s) ≥ 1 and z_i is the first 1 of s on Z, the step from s reads z_1..z_i, then walks in S_i,
the subtree at the 1-child of the node z_i. It erases z_i and writes at most one bit, so Z(f(s)) ≤ Z(s).
If Z(s) = 0 the step ends at λ0 and Z(f(s)) ≤ 1.

## 1. Theorem E: a cycle has at most one empty step

**Theorem E.** For every tree T (any depth) and every cycle C of f_T, at most one step of C ends at an
empty leaf.

Proof. Induction on m = |idx(T)|. If m = 0, T is a leaf, f is the identity, cycles have length 1.
Let m ≥ 1, so k ≥ 1, and let C have length ≥ 2 (length 1 is trivial).

Case A: no state of C has Z = 0. Then no step of C ends at λ0, so by (F1) no step raises the weight;
the weight is constant on C. Every step reads at least one 1 (it does not follow the all-zero path), so
an empty step would lower the weight. There is none.

Case B: some state of C has Z = 0. If λ0 were empty that state would be a fixed point; so λ0 writes
some z_j0. By (F2), going once round the cycle from that state, every state of C has Z ≤ 1. Call the
states with Z = 0 *home* states; let there be H ≥ 1 of them, h_1, ..., h_H in cyclic order. All bits
outside idx(T) are constant, and a home state has zeros on Z, so h_t is determined by its restriction
x_t to the *data* indices idx(T) \ Z; the x_t are pairwise distinct.
A state of C with Z = 1, the 1 at z_i, is "in mode i"; in it every z_j with j ≠ i is 0. Its step runs
through S_i and its leaf does one of: (α) writes some z_j, and the next state is in mode j; (β) writes a
data index; (γ) writes nothing. After (β) or (γ) the next state has Z = 0. Hence the segment of C from
h_t to h_(t+1) (an *excursion*) is: the step at λ0 (not empty), then zero or more (α) steps, then exactly
one (β) or (γ) step. The empty steps of C are exactly the (γ) steps: at most one per excursion. If
H = 1 we are done. Let H ≥ 2.

Excursion tree. For a mode i and a set K of data indices define the tree U(i, K) from S_i, top down:
- a node whose index is some z_j, or a data index in K: replace it by its (processed) 0-child;
- a node whose index u is a data index not in K: keep it, and process its children with K ∪ {u};
- a leaf writing a data index, or nothing: keep it;
- a leaf writing some z_j: replace it by U(j, K'), K' = K ∪ (data indices kept on the way);
  except that if K' = K and mode j has already been entered since K last grew, put an empty leaf
  there instead (a *dummy*).
Let F = U(j0, ∅). Every graft either enlarges K or lengthens a chain of pairwise distinct modes, so F is
finite. On every path of F the indices are distinct (a node is kept only if its index is not in K, and
K is the set of indices already on the path), and a leaf writes nothing or an index of its own path
(the leaf of S_i wrote a data index on its path in S_i; that index is either kept there or lies in K).
So F is a tree of the model with idx(F) ⊆ idx(T) \ Z, at most m − k < m indices.

Claim: f_F(x_t) = x_(t+1), and the step of F from x_t ends at an empty leaf iff excursion t ends with (γ).
Follow excursion t and the walk of F on x_t together. Invariant at the start of a stage: the state of T
is in mode i, its data bits are x_t with the indices of K set to 0, and the walk of F stands at the root
of a copy of U(i, K), K being the set of indices on the path of F so far. It holds after the λ0 step
(mode j0, K = ∅). In mode i the step of T reads, inside S_i, every z_j as 0 (j ≠ i, and z_i cannot
recur below itself), every index of K as 0, and any other data index u as (x_t)_u: exactly the walk of F
through U(i, K). The step erases what it read. If its leaf is of type (α) the invariant holds for the
next stage (a dummy is never reached from a state of C: there the same stages would repeat for ever
without changing the data, and the orbit would never return to a home state). If the leaf is of type
(β) or (γ), the data bits of the next home state are x_t with all indices on the F-path erased and, in
case (β), the written index set: that is f_F(x_t), and the F-leaf is empty exactly in case (γ).

So x_1, ..., x_H is a cycle of f_F of length H ≥ 2, and by induction at most one of its steps is empty,
i.e. at most one excursion ends with (γ). ∎

Remarks. (i) This is solver B's observation (computed for r ≤ 3) and the referee's follow-up task 1,
for all trees. (ii) Mechanical cross-checks: `brute.cpp` (all trees of depth ≤ 3) and `lazy2.cpp`
(all trees of depth ≤ 4) both report maximum 1; `check_F.py` tests the construction of F itself.

## 2. Theorem RD: root subtrees on disjoint indices

For T = (a T0 T1) let T0°, T1° be T0, T1 with every leaf that writes a turned into an empty leaf.
They are trees of the model on idx(T0), idx(T1). Let M(G) be the longest cycle of a tree G.

**Theorem RD.** If idx(T0) ∩ idx(T1) = ∅ then M(T) ≤ M(T1°) · (M(T0°) + 1).
Moreover on any cycle of T that contains both a state with a = 0 and a state with a = 1, all states
with a = 1 agree on idx(T0).

Proof. Write a state as (a, x, y), x on idx(T0), y on idx(T1); other bits never change. Let g0, g1
be the maps of T0°, T1°. By the definition of a step:
 (0, x, y) → (e, g0(x), y), e = 1 iff x is routed in T0 to a leaf writing a (an *exit*);
 (1, x, y) → (e', x, g1(y)), e' = 1 iff y is routed in T1 to a leaf writing a.
(A leaf writing a erases what it read in its own subtree exactly like the empty leaf that replaces it.)
Let C be a cycle of T.
I. All states have a = 0: y is constant and x runs through a cycle of g0; |C| ≤ M(T0°).
II. All states have a = 1: x is constant, so the y are pairwise distinct and form a cycle of g1 all of
whose steps are empty (in T1°). By Theorem E, |C| = 1.
III. Both occur. x changes only at a = 0 steps, by g0, and C is periodic, so x always lies on one cycle
C0 of g0; likewise y lies on one cycle C1 of g1. C contains a step from a = 0 to a = 1, an exit,
which is an empty step of C0. By Theorem E, C0 has exactly one state x_e whose step is empty. Take a
state (1, x, y) of C and go back along C to the last a = 0 state before it: its step was an exit, so it
was (0, x_e, ·), and x has not changed since: x = g0(x_e) =: x*. So the a = 1 states lie in
{1} × {x*} × C1 and the a = 0 states in {0} × C0 × C1:
 |C| ≤ |C1| + |C0||C1| ≤ M(T1°)(M(T0°) + 1). ∎

**Corollary RD-r.** If T has depth ≤ r and idx(T0) ∩ idx(T1) = ∅ then every cycle of T has length
≤ D(r−1)(D(r−1) + 1). With D(4) = 1806 (computed in the repository): a tree of depth 5 with a cycle
longer than 3263442 must have an index that occurs in both root subtrees.

**Corollary (sharing must reach high up).** Say T is *disjoint above level j* if for every internal
node at depth < j (root = depth 0) its two subtrees have disjoint index sets. Applying RD at every such
node (T0°, T1° are again disjoint above level j − 1): a tree of depth ≤ r that is disjoint above level j
has M(T) ≤ U_j, U_0 = D(r − j), U_i = U_(i−1)(U_(i−1) + 1). If D(r − j) = W_(r−j) this is W_r.
Since D(ρ) = W_ρ for ρ ≤ 4, every tree of depth r with a cycle longer than W_r contains two nodes with
the same index in different branches whose lowest common ancestor has depth ≤ r − 5.

## 3. Theorem RO: read-once trees, exact value for every shape

A tree is *read-once* if no index occurs at two nodes (equivalently: disjoint above every level).

**Theorem RO.** For a read-once tree T every cycle has length ≤ L(T). Every shape has a read-once
labelling with a cycle of length exactly L(T). Hence the longest cycle over read-once trees of depth
≤ r is exactly W_r = 2, 6, 42, 1806, 3263442, ... for every r.

Proof. Upper bound by induction on the tree: a leaf has L = 1 and only fixed points. For
T = (a T0 T1), T0° and T1° are read-once with the same shapes, so M(T0°) ≤ L(T0), M(T1°) ≤ L(T1), and
Theorem RD gives M(T) ≤ L(T1)(L(T0) + 1) = L(T).
Lower bound: claim Q(T): the shape of T has a read-once labelling with a cycle of length L(T) on which
the all-ones leaf is empty and is used exactly once. Leaf: clear. Node: label T1 by Q(T1); label T0 by
Q(T0) and change its all-ones leaf to write the new root index a. This is the construction of
Theorem 1 of solver-a/solver-b with W replaced by p = L(T0), q = L(T1): p steps with a = 0, then one
step of T1, repeated q times; length q(p + 1); the all-ones leaf of T is that of T1, used once. ∎

So the conjecture D(r) = W_r is equivalent to: reusing an index in different branches never gives a
longer cycle than the best read-once tree of the same depth.

## 4. An explicit bound for all trees

**Theorem G.** D(r) ≤ (r + 1) · 2^(2^r − 1 − r) for every r ≥ 1 (r = 2: 6, exact; r = 3: 64; r = 4: 10240;
r = 5: about 4.03e8). So log2 D(r) ≤ 2^r − 1 − r + log2(r + 1).

Proof. Let T have depth ≤ r, m = |idx(T)|, k the length of the all-zero path; k = 0 gives length 1.
A cycle of length ≥ 2 either has no state with Z = 0 — then (Case A above) the weight is constant, each
step reads exactly one 1, which is on Z, and writes one bit; Z(s) cannot drop, so the written bit is on
Z, only bits of Z ever change and the length is ≤ 2^k ≤ 2^r — or it has one, and then (Case B) every
state has at most one 1 on Z: at most (k + 1) 2^(m−k) states. If k = r this is ≤ (r + 1) 2^(d−r). If
k < r, the all-zero leaf sits at depth k, so at least 2^(r−k) − 1 of the 2^r − 1 possible nodes are
missing, m ≤ 2^r − 2^(r−k), and with j = r − k ≥ 1:
 (k + 1) 2^(m−k) ≤ (k + 1) 2^(2^r − 2^j − r + j) = (k + 1) 2^(j + 1 − 2^j) · 2^(2^r − 1 − r) ≤ (r + 1) 2^(2^r − 1 − r).
Finally 2^r ≤ (r + 1) 2^(2^r − 1 − r) for r ≥ 1. ∎

## 5. What fails when the root subtrees share an index (smallest examples)

(a) Smallest failure of the product bound of Theorem RD: depth 2, two indices.
T = (1 (2 [1] []) (2 [2] [2])). Bits (1,2): 01 → 00 → 10 → 01, length 3, but M(T1°) = M(T0°) = 1, so
M(T1°)(M(T0°) + 1) = 2. The 1-subtree writes a bit that the 0-subtree reads: the memory seen by T0
is not moved by T0 alone, so it does not lie on a cycle of T0°. (L(T) = 6.)

(b) Smallest failure of "the root = 1 states number at most L(T1)": depth 3, four indices.
T = (1 (2 [1] (3 [3] [])) (2 (4 [4] [2]) [])). Orbit of 0000 (bits 1234):
0000 1000 0001 1001 0100 0010 1010 0011 1011 0110 → 0000, length 10; four states have bit 1 set,
L(T1) = M(T1°) = 3, M(T0°) = 1 (product bound 6). The 1-subtree reads index 2 only to be allowed to
write it; that write hands control to the 1-subtree of node 2 inside T0, which the 1-subtree thereby
borrows as a second digit. (L(T) = 15.) No such failure exists at depth ≤ 2, nor at depth ≤ 3 with three indices (`out_brute_r2.txt`, `out_brute3_r3_m3.txt`).

**Conjecture P1 (shape bound).** For every tree, every cycle has length ≤ L(T). It implies D(r) = W_r
for all r, is Theorem RO for read-once trees, and is computed for all trees of depth ≤ 4 (Section 6).
The induction for it is missing. Natural strengthenings that would carry one are FALSE at depth 3
(`out_brute3_r3.txt`): (i) #(root = 1 states) ≤ L(T1); (ii) uses of a leaf ≤ product of L(1-sibling)
over the 0-edges of its path; (iii) an orbit segment with no empty step has ≤ W_depth states (43 at
depth 3); (iv) the map "state ↦ set of nodes reached by always taking the 1-child and taking the
0-child iff the bit is 0" (which has exactly L(T) values) is injective on every cycle; (v) the product bound of Theorem RD. Also, applying the
excursion-tree reduction of Section 1 with L as the measure fails: F can contain several copies of one
subtree of T (one per leaf that jumps to it), and L(F) then exceeds what is needed by a large factor.

## 6. Computations (all by programs in this folder, written from the definition)

- `brute.cpp` / `brute3.cpp` (`out_brute_r3.txt`, `out_brute3_r3.txt`): all 5,478,539 trees of depth ≤ 3
  up to relabelling, full functional graph of each. Empty steps per cycle ≤ 1; no cycle longer than
  L(T); exactly 26 cycles with length = L(T), one for each of the 26 shapes, all read-once;
  383,161 trees with index-disjoint root subtrees, none violating Theorem RD or its "moreover";
  counts of the failures (i)–(v) above.
- `lazy2.cpp` (`out_lazy2_r4.txt`): every cycle of every tree of depth ≤ 4 (trajectory search,
  1,890,054,694 decision nodes — the same count as the referee's `lazy_r4.out` — 604,143,675 closed
  cycles): longest 1806; empty steps ≤ 1; no cycle longer than L of the part of the tree it uses;
  677 with equality (677 = number of shapes of depth ≤ 4).
- `check_F.py` (`out_checkF.txt`): the construction of F in Section 1, on 40,000 random and edited
  trees, about 20,000 cycles with ≥ 2 home states.
- Checks watched failing on purpose: `out_brute_mut1.txt` (wrong L), `out_brute_mut2.txt` (wrong notion
  of empty), `out_brute3_r3_mut4.txt` (wrong product bound), `out_brute3_mut3.txt` (wrong signature),
  `out_lazy2_small.txt` (both mutations at r = 3), `out_checkF.txt` (two wrong constructions).
