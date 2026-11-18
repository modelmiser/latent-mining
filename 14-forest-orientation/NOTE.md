# Total flips for 2-orientations of incremental forests: n/3 − O(log n)

*A short note. Draft of 18 November 2026. Source and all supporting files:
this folder; overview in [README.md](README.md).*

## Summary

Edges are inserted one at a time into a forest on n vertices. An algorithm
keeps every edge directed so that no vertex has more than two out-edges, and
may reverse (flip) edges inserted earlier. Let T(n) be the largest total
number of flips, over the whole sequence of n − 1 insertions, that an adaptive
adversary can force.

**Theorem 1.** For every n ≥ 1,

    T(n) ≥ n/3 − (7/3)·log₂(n+1) + 5/6.

**Theorem 5.** For every n ≥ 1,

    T(n) ≤ ⌊(n − 1)/3⌋.

So T(n) = n/3 − O(log n), and the limit L of T(n)/n is 1/3. Both theorems
are machine-checked in Lean 4, as statements about one and the same
definition of T: Theorem 1 in the integer form
n + 3 ≤ 3·T(n) + 7·⌊log₂(n+1)⌋ and Theorem 5 as 3·T(n) + 1 ≤ n (Section 7).
We do not know whether n/3 − T(n) is unbounded.

Theorem 5 is achieved by the algorithm of Gupta, Kumar and Stein
[7, Theorem 2.1], for which its authors prove 2n, with one of its ties
broken by a fixed rule (rule L, Section 6). Three weaker results about that
algorithm came first and are kept in Section 5, numbered in the order found:

**Theorem 2.** For every n ≥ 19, T(n) ≤ (9/16)·n − log₂ n.

**Theorem 3.** For every n ≥ 1, T(n) ≤ max(0, ⌊(n − 3)/2⌋).

**Theorem 4.** For every j ≥ 1, the algorithm of [7], with its ties broken
by the Adversary, can be made to pay (3^j − 1)/2 − j flips on 3^j vertices.

As bounds on T, Theorems 2 and 3 are superseded by Theorem 5, except that
Theorem 3 gives T(4) = 0 where Theorem 5 gives T(4) ≤ 1. Theorem 2 is
an analysis by a recurrence on tree sizes and Theorem 3 one by a potential;
both hold however the ties are broken. Theorem 4 shows that Theorem 3 cannot
be improved beyond lower-order terms without saying how ties are broken: the
worst case with ties broken against the algorithm is n/2 − O(log² n). So the
tie-breaking rule is what brings the cost from n/2 down to n/3.

A forest can always be oriented with out-degree at most 1. Keeping that
bound as edges arrive can require Ω(n log n) flips, and Gupta, Kumar and
Stein [7] showed that relaxing the bound to 2 brings the total down to O(n).
Theorems 1 and 5 say that the amortized number of flips per insertion that
an adversary can force is 1/3 − o(1), no more and no less. A constant
amortized bound in each direction is not surprising. Our search did not find
this constant stated, as a lower or as an upper bound (Section 8); we are
not specialists, and it may be known. Besides the constant, the note offers
proofs short enough to check and a machine check of both.

## 1. The game

V = {1, …, n}. A position is a forest G on V together with a direction for
each edge such that every vertex is the tail of at most two edges. The start
has no edges. In each of n − 1 rounds:

1. the Adversary names two vertices u, v in different trees;
2. the Algorithm chooses a direction for uv and may flip any older edges,
   provided the result is a position. It pays the number of older edges
   flipped.

Both players see the position. T(n) is the value of this finite game: the
total payment under optimal play by both.

A vertex is *saturated* if it has two out-edges. For a vertex v, let h(v) be
the length of a shortest directed path from v to an unsaturated vertex, so
h(v) = 0 exactly when v is unsaturated. This is finite: if every vertex
reachable from v were saturated, the reachable set R would span 2|R| edges
of a forest.

**Size fact.** If h(v) ≥ k then the tree of v has at least 2^(k+1) − 1
vertices: every vertex at directed distance less than k from v is saturated
and so has two out-neighbours, and distinct directed paths from v in a tree
end at distinct vertices, so there are 2^j vertices at distance j for each
j ≤ k.

## 2. Normal replies

Let {x, y} be the named pair. A reply is *normal at x* if it is one of:

- **free:** x is unsaturated; flip nothing and direct the new edge x → y;
- **path:** x is saturated; flip one directed path x = w₀ → w₁ → … → w_d
  (d ≥ 1) in which w₀, …, w_(d−1) are saturated and w_d is not, and direct
  the new edge x → y. The cost is d.

**Lemma 1 (simulation).** Let D_A and D_L be two orientations of the same
forest, and write d(D_L, D_A) for the number of edges on which they differ.
Suppose a pair is named and a reply takes D_A to D_A′ at cost p. Then there
is a normal reply taking D_L to some D_L′, at cost c, determined by D_L and
D_A′, with

    c + d(D_L′, D_A′) ≤ p + d(D_L, D_A).

Consequently, if a play A and a play L start from the empty position, face
the same named pairs, and L answers each round by this reply, then after
every round cost(L) + d(D_L, D_A) ≤ cost(A). The pairs may be chosen in any
way, including by looking at D_L.

*Proof.* Say D_A′ directs the new edge x → y. On the old edges,
d(D_L, D_A′) ≤ d(D_L, D_A) + p.

If x is unsaturated in D_L, take the free reply at x. Nothing is paid and
the new edge agrees, so the inequality holds.

If x is saturated in D_L, then x has two old out-edges in D_L and at most one
in D_A′, so some edge x → w₁ of D_L is directed w₁ → x in D_A′. If w₁ is
saturated in D_L, its two out-edges there do not include w₁x, while in D_A′
it has the out-edge w₁ → x and so at most one other; hence some edge
w₁ → w₂ of D_L is reversed in D_A′. Continue. The vertices are distinct
because a directed walk in an oriented forest cannot repeat a vertex, so the
walk ends at some w_d unsaturated in D_L. Flip this path and direct x → y.
The result is a position: x loses one out-edge and gains the new one, the
inner vertices lose one and gain one, and w_d, which was unsaturated, gains
one. So this is a normal reply of cost d, and all d flipped edges were
disagreements with D_A′, so the distance falls by d while the cost is d. ∎

**Corollary (shadow play).** If an Adversary strategy S forces at least c in
every play in which all replies are normal, then T(n) ≥ c.

*Proof.* Against an arbitrary Algorithm, the Adversary keeps a *shadow*
orientation D_L of the same forest. Each round it names the pair S prescribes
for the shadow position, which is legal because the two positions have the
same forest, and after the Algorithm's actual reply it updates the shadow by
the normal reply of Lemma 1. The shadow play is a play of normal replies
against S, so it costs at least c, and by Lemma 1 the real play costs at
least as much. ∎

The Corollary matters because the strategy below is adaptive: it chooses its
next pair by looking at the orientation, and after a reply that is not normal
the real orientation need not have the properties the strategy relies on. It
must be run on the shadow. From here on, all replies are normal.

In a normal reply all flipped edges lie in the tree of x, so the cost of a
round can be charged to the tree it creates.

## 3. Types

Give every vertex two *slots*. A slot holding an out-neighbour o has value
h(o); an empty slot has value −1. Write the two values of v in sorted order
as (P, Q), P ≤ Q. Then h(v) = P + 1 in every case. For integers
−1 ≤ p ≤ q, say that v *has type (p, q)* if P ≥ p and Q ≥ q. If v has type
(p, q) it has every type that is smaller in both coordinates.

**Lemma 2 (one reply).** Let x have type (p, q) and y have type (p′, q′), in
different trees, and let {x, y} be named. Every normal reply at x satisfies
one of

- (X1) cost ≥ p + 1, and afterwards x has type sort(q, p′ + 1);
- (X2) cost ≥ q + 1, and afterwards x has type sort(p, p′ + 1).

In both cases y still has type (p′, q′). The same holds with x and y
exchanged for replies at y.

*Proof.* Let (P, Q) be the true slot values of x. Three facts about a normal
reply at x:

(a) A path reply whose first step goes to the out-neighbour o costs at least
1 + h(o), since the rest of the path is a directed path from o to an
unsaturated vertex.

(b) The value of the other slot of x does not change. If that slot is empty
this is clear. If it holds o′, let R be the set of vertices reachable from o′
by directed paths. Neither x nor any vertex of the flipped path is in R: the
unique tree path from o′ to any of them begins with the edge between o′ and
x, which is directed x → o′. Every flipped edge has both ends on the path, and the
new edge leaves x. So no out-edge of a vertex of R changes, and h(o′) is
determined by R and those out-edges.

(c) h(y) and the slot values of y do not change, by the same argument:
nothing in the tree of y is flipped and the new edge points into y. And
h(y) ≥ p′ + 1.

After the reply the slots of x hold the new out-neighbour y, of value
h(y) ≥ p′ + 1, and whatever was in the slot not used. A free reply needs an
empty slot, so P = −1 and hence p = −1; it costs 0 = p + 1 and leaves values {Q, h(y)}:
this is (X1). A path reply through the slot of value P costs at least
P + 1 ≥ p + 1 and leaves {Q, h(y)}: (X1). A path reply through the slot of
value Q costs at least Q + 1 ≥ q + 1 and leaves {P, h(y)}: (X2). Sorting is
monotone in both arguments, which gives the stated types. ∎

Now define, for k ≥ 0,

    A_k = type (k−1, k−1),   index 2k,     F(A_k) = k + 1/3,
    R_k = type (k−1, k),     index 2k+1,   F(R_k) = k + 2/3.

Every vertex has type A_0. The types are nested: a vertex whose type has
index i + 1 also has the type of index i. A vertex of index 2k or 2k + 1 has
h ≥ k. Write F(i) for the value of F at the type of index i; then
F(i) ≤ i/2 + 1/3 and F(i + 1) − F(i) ≤ 2/3.

**Lemma 3 (types).** Let x and y, in different trees, both have the type of
index i, and let {x, y} be named. After any normal reply, the tail of the new
edge has the type of some index j ∈ {i, i + 1} with

    cost + F(j) ≥ 2·F(i).                                            (∗)

*Proof.* Apply Lemma 2 with (p, q) = (p′, q′). For A_k both alternatives give
cost ≥ k and type sort(k − 1, k) = R_k, and k + (k + 2/3) = 2(k + 1/3). For
R_k, (X1) gives cost ≥ k and type sort(k, k) = A_(k+1), and
k + (k + 1 + 1/3) = 2(k + 2/3); (X2) gives cost ≥ k + 1 and type R_k, and
(k + 1) + (k + 2/3) > 2(k + 2/3). ∎

In words: joining two trees at depth k costs k, and the Algorithm can avoid
the result becoming deeper only by paying one more.

## 4. Proof of Theorem 1

The Adversary assigns to every tree C an index a(C) and remembers a vertex of
C, its *witness*, that has the type of that index. Singletons get index 0.
Trees not involved in a round keep their witnesses, since nothing in them
changes. Let paid(C) be the total cost of the rounds that built C, and

    Φ(C) = paid(C) + F(a(C)) − |C|/3.

For a singleton, Φ = 0 + 1/3 − 1/3 = 0.

**Main phase.** While two trees X, Y have the same index i, name their
witnesses. By Lemma 3 the new tree has a witness of an index j with (∗);
assign it j. Then

    Φ(new) = paid(X) + paid(Y) + cost + F(j) − (|X| + |Y|)/3 ≥ Φ(X) + Φ(Y).

So Φ ≥ 0 for every tree throughout the main phase, whatever the Algorithm
does.

**Cleanup.** The remaining trees X₁, …, X_m have distinct indices
i₁ < … < i_m. If m = 1 there is nothing to do and the total paid is at
least n/3 − F(i₁). Otherwise put Y_m = X_m and b_m = i_m. For r = m − 1, …, 1, name the
witnesses of Y_(r+1) and X_r. Both have the type of index i_r, the first by
nesting since b_(r+1) ≥ i_(r+1) > i_r. Lemma 3 gives the new tree Y_r a
witness of some index b_r ∈ {i_r, i_r + 1} with cost + F(b_r) ≥ 2·F(i_r),
hence

    Φ(Y_r) ≥ Φ(Y_(r+1)) + Φ(X_r) − (F(b_(r+1)) − F(i_r)).

Since Φ(X_r) ≥ 0 and Φ(Y_m) ≥ 0, summing gives
Φ(Y₁) ≥ − Σ_(r<m) (F(b_(r+1)) − F(i_r)). Y₁ is the whole vertex set, so the
total paid is

    paid(Y₁) = n/3 − F(b₁) + Φ(Y₁)
             ≥ n/3 − F(b₁) − Σ_(r<m) (F(b_(r+1)) − F(i_r))
             = n/3 − F(b_m) − Σ_(r<m) (F(b_r) − F(i_r))
             ≥ n/3 − F(i_m) − (2/3)(m − 1),

where the equality collects the terms F(b₁), …, F(b_m), and the last step
uses b_m = i_m and F(b_r) − F(i_r) ≤ 2/3.

**Counting.** The tree X_m has a vertex with h ≥ ⌊i_m/2⌋, so by the size
fact i_m ≤ 2·log₂(n+1) − 1. The indices are distinct and non-negative, so
m ≤ i_m + 1. With F(i_m) ≤ i_m/2 + 1/3,

    total ≥ n/3 − (7/6)·i_m − 1/3 ≥ n/3 − (7/3)·log₂(n+1) + 5/6.

This is a strategy against normal replies; the Corollary of Section 2 gives
the bound for T(n). ∎

The bound is weak for small n. It is positive from n = 34 on.

## 5. The algorithm of [7] with arbitrary ties, and other facts

**The algorithm.** If an endpoint of the named pair is unsaturated, direct
the new edge out of it and flip nothing. Otherwise let u be the endpoint in
the smaller of the two trees, of size a; flip a shortest directed path from u
to an unsaturated vertex and direct the edge out of u. The cost is 0 in the
first case and h(u) in the second, all flipped edges lie in the tree of u,
and by the size fact h(u) ≤ c(a), where

    c(a) = ⌊log₂(a + 1)⌋ − 1 ≥ 0.

Define f(1) = 0 and, for n ≥ 2, f(n) as the maximum over a ≤ b with
a + b = n of f(a) + f(b) + c(a). By induction on the rounds, the rounds that
built a tree with m vertices cost at most f(m) in total, so T(n) ≤ f(n).
The same holds for the rule of [7], which flips the shorter of the two
shortest paths: that path is no longer than the one from u.

**Proof of Theorem 2.** Let ψ(n) = log₂ n if n is a power of 2 and
ψ(n) = log₂(n+1) otherwise. We show

    f(n) ≤ (9/16)·n − ψ(n)   for every n ≥ 64.                        (∗∗)

Three finite facts are used, all checked in integer arithmetic by
`upper/greedy_recurrence.py`, which prints the table of f(1), …, f(126):

- (i) (∗∗) holds for 64 ≤ n ≤ 126;
- (ii) f(a) + c(a) + log₂(1 + (a+1)/64) ≤ (9/16)·a for 1 ≤ a ≤ 63;
- (iii) f(n) ≤ (9/16)·n − log₂ n for 19 ≤ n ≤ 63.

One more fact: ψ(a) − c(a) ≥ 1 for every a ≥ 2. If a is not a power of 2
this is log₂(a+1) ≥ ⌊log₂(a+1)⌋; if a = 2^k with k ≥ 1 then c(a) = k − 1.

Let n ≥ 127 and suppose (∗∗) holds for all m with 64 ≤ m < n. Take any
a ≤ b with a + b = n; then 64 ≤ b < n.

*Case a ≥ 64.* By (∗∗) for a and b,

    f(a) + f(b) + c(a) ≤ (9/16)·n − ψ(b) − (ψ(a) − c(a)) ≤ (9/16)·n − ψ(b) − 1,

and ψ(b) + 1 ≥ ψ(n): if b is not a power of 2, ψ(b) + 1 = log₂(2b + 2) ≥
log₂(n+1) ≥ ψ(n). If b = 2^m and a < b, then n + 1 ≤ 2^(m+1), so
ψ(n) ≤ log₂(n+1) ≤ m + 1. If a = b = 2^m, then ψ(n) = m + 1.

*Case a ≤ 63.* By (∗∗) for b,

    f(a) + f(b) + c(a) ≤ (9/16)·n − ψ(n) + [f(a) + c(a) + ψ(n) − ψ(b) − (9/16)·a],

and the bracket is at most 0 by (ii), since ψ(n) − ψ(b) ≤ log₂(n+1) − log₂ b
= log₂(1 + (a+1)/b) ≤ log₂(1 + (a+1)/64).

So (∗∗) holds for all n ≥ 64. Since ψ(n) ≥ log₂ n, this gives Theorem 2 for
n ≥ 64, and (iii) gives it for 19 ≤ n ≤ 63. ∎

Among n ≤ 18, f(n) exceeds (9/16)·n − log₂ n at n = 6, 9 and 18. For n ≤ 16
the exact values of T are in the small cases below; T(18) is not known,
which is why the theorem starts at 19.

**The constant 9/16 is exact for this recurrence.** For every k ≥ 4,
f(2^k) = 9·2^k/16 − k. The table gives f(16) = 5 and f(32) = 13. Taking
a = b = 2^k in the definition gives f(2^(k+1)) ≥ 2·f(2^k) + k − 1, so
f(2^k) ≥ 9·2^k/16 − k by induction, and (∗∗) gives the reverse for k ≥ 6.
Also f(n) ≤ (9/16)·n for every n: for n ≥ 64 by (∗∗) and for n ≤ 63 by the
table. Since c ≥ 0, f(a + b) ≥ f(a) + f(b), so by Fekete's lemma f(n)/n
converges to its supremum, which is 9/16: at most 9/16 by the bound just
stated, and at least that because f(2^k)/2^k tends to 9/16. This is a
statement about f, not about T. An upper bound below 9/16 needs a different
algorithm, or an analysis that does not charge each merge by c of the
smaller size alone. Theorem 3 below is such an analysis.

**Comparison with [7].** The algorithm is theirs, as just noted. Gupta,
Kumar and Stein bound the flipped path by log₂ of the size of the smaller
tree and get the recurrence with log₂ min(a, b) in place of c(a). They state
the bound 2n, noting 2n − log₂ n − 2 by induction. The same induction gives
n − 1 − log₂ n for their recurrence, with equality at powers of 2, so that
recurrence cannot give a constant below 1. This sharper reading of their
recurrence is not new: Ozdayi [13, Theorem 2.4] proves n − log₂ n − 1 for n
edges from it (we found [13] only after writing this section). The improvement to 9/16 comes
from two things in the path bound: the size fact, which gives
log₂(a + 1) − 1 in place of log₂ a, and rounding down, since a path length
is an integer. Without the rounding the recurrence gives about 0.69·n at
n = 1000 (computed).

**A potential for the rule of [7].** From here to the end of the proof of
Theorem 4 the algorithm is the rule of [7] itself, which we call rule R: for
the named pair, let p be an endpoint with h(p) ≤ h(q), q being the other;
flip a shortest directed path from p to an unsaturated vertex and direct the
new edge from p to q. The cost is k = h(p); if k = 0 nothing is flipped. Ties
(h(p) = h(q), or several shortest paths) may be broken in any way. This is
not the smaller-tree rule at the start of this section, and Lemma 4 below
fails for that rule: take for u the root of a complete binary out-tree on 7
vertices and for v a vertex with two out-neighbours of out-degree 0 and five
in-neighbours of out-degree 1; the smaller-tree rule flips from u at cost 2
and Φ rises by 3/2.

For a position, let

    Φ = (1/2) · Σ over edges x → c of min( h(c), h(x) + 1 ).

Φ is 0 when there are no edges, and Φ of a position is the sum of Φ over its
trees. For a saturated vertex whose out-neighbours have h equal to m ≤ M
(so the vertex has h = m + 1), its two out-edges contribute
F(m, M) = (m + min(M, m + 2))/2. A vertex with one out-neighbour, of h equal
to M, contributes (1/2)·min(M, 1).

**Lemma 4 (one round).** A round of rule R of cost k raises Φ by at least k.

*Proof.* Let the flipped path be p = w_0 → w_1 → … → w_k, so h(w_i) = k − i.
For i < k, w_i is saturated; let c_i be its out-neighbour other than
w_(i+1), and a_i = h(c_i) ≥ k − i − 1. Let b = h(q) ≥ k. The vertex w_k has
no out-neighbour or one, which we call z. After the round the out-neighbours
are: w_0 → {c_0, q}; w_i → {c_i, w_(i−1)} for 1 ≤ i ≤ k − 1; and w_k →
{w_(k−1)} or {z, w_(k−1)}. Every other vertex keeps its out-neighbours. A
directed path from c_i, from z or from q cannot reach a vertex of the flipped
path, since with the path it would close a cycle (q is in another tree). So
the out-trees of c_i, z and q, and hence their values of h, are unchanged.

Let k ≥ 1 and write g_i for the new value of h at w_i.

*No h decreases.* We show g_i ≥ k − i = h(w_i) for every i < k by induction
on i. First, g_0 = 1 + min(a_0, b) ≥ k. For 1 ≤ i < k the induction
hypothesis is g_(i−1) ≥ k − i + 1, so g_i = 1 + min(a_i, g_(i−1)) ≥ k − i.
Also g_k ≥ 0 = h(w_k). Any other vertex keeps its
out-neighbours, and h of a vertex is non-decreasing in the h of its
out-neighbours, so by induction along its out-tree its h does not decrease.

*Edges out of vertices off the path.* These edges are unchanged, and
min(h(c), h(x) + 1) is non-decreasing in both arguments. So their terms do
not decrease. This covers every edge into a path vertex from outside the
path, every edge into q, and every edge of the other tree.

*Edges out of w_i, 1 ≤ i ≤ k − 1.* Put d = k − i − 1, a = a_i ≥ d and
g = g_(i−1) ≥ d + 2. The old contribution is F(d, a) and the new one is F on
the pair {a, g}.
- a = d: old d; new (d + min(g, d + 2))/2 = d + 1.
- a = d + 1: old d + 1/2; new (d + 1 + min(g, d + 3))/2 ≥ d + 3/2.
- a ≥ d + 2 and a ≤ g: old d + 1; new (a + min(g, a + 2))/2 ≥ a ≥ d + 2.
- a > g: old d + 1; new (g + min(a, g + 2))/2 ≥ g + 1/2 ≥ d + 5/2.

The gain is at least 1 in every case.

*Edges out of w_0.* Put d = k − 1, a = a_0 ≥ d and b ≥ d + 1. Old F(d, a),
new F on {a, b}.
- a = d: old d; new (d + min(b, d + 2))/2 ≥ d + 1/2.
- a = d + 1: old d + 1/2; both values are at least d + 1, so new ≥ d + 1.
- a ≥ d + 2 and b ≤ a: old d + 1; the smaller value is b ≥ d + 1 and
  min(a, b + 2) ≥ d + 2, so new ≥ d + 3/2.
- a ≥ d + 2 and b > a: old d + 1; new (a + min(b, a + 2))/2 ≥ a + 1/2 ≥ d + 5/2.

The gain is at least 1/2.

*Edges out of w_k.* Here g_(k−1) ≥ 1.
- No old out-neighbour: old 0; new (1/2)·min(g_(k−1), 1) = 1/2.
- Old out-neighbour z with h(z) = 0: old 0; new (0 + min(g_(k−1), 2))/2 ≥ 1/2.
- h(z) ≥ 1: old 1/2; both new values are at least 1, so new ≥ 1.

The gain is at least 1/2.

Adding up, Φ rises by at least 1/2 + (k − 1) + 1/2 = k.

Let k = 0. Nothing is flipped and p gains the out-neighbour q. No h
decreases, so the terms of all edges out of other vertices do not decrease.
If p had no out-neighbour, the new edge contributes at least 0. If p had one
out-neighbour z, the old contribution of p was (1/2)·min(h(z), 1) and the new
one is F on {h(z), h(q)}: if h(z) = 0 the old one is 0; if h(z) ≥ 1 and
h(q) = 0 the new one is min(h(z), 2)/2 ≥ 1/2; if both are at least 1 the new
one is at least 1. ∎

**Lemma 5 (counting).** Every tree T on n vertices, directed with all
out-degrees at most 2, has Φ(T) ≤ (n − 1)/2 − H(T), where H(T) is the largest
h in T. No assumption is made on how T arose.

*Proof.* Let S(T) = 2Φ(T); we show S(T) ≤ n − 2H(T) − 1 by induction on n.
For n = 1 both sides are 0. Let n ≥ 2. T has n − 1 edges, so some vertex r
has in-degree 0, and r has an out-neighbour since T is connected. No out-tree
of another vertex contains r, so deleting r changes no other h.

If r has one out-neighbour c, then T − r is a tree T_1 on n − 1 vertices,
H(T) = H(T_1), and S(T) = S(T_1) + min(h(c), 1) ≤ (n − 1) − 2H(T_1) − 1 + 1.

If r has two out-neighbours, T − r consists of two trees T_1 and T_2, with
n_1 + n_2 = n − 1, containing the out-neighbours c_1 and c_2. Name them so
that m = h(c_1) ≤ M = h(c_2), and write H_1, H_2 for H(T_1), H(T_2); then
H_1 ≥ m, H_2 ≥ M and H(T) = max(H_1, H_2, m + 1). By induction

    S(T) ≤ (n − 1) − 2H_1 − 2H_2 − 2 + m + min(M, m + 2),

and it suffices that E = m + min(M, m + 2) − 2H_1 − 2H_2 − 2 + 2H(T) ≤ 0.
- H(T) = H_2: E = m + min(M, m + 2) − 2H_1 − 2 ≤ 2m + 2 − 2m − 2 = 0.
- H(T) = H_1: E = m + min(M, m + 2) − 2H_2 − 2 ≤ m + M − 2M − 2 < 0.
- H(T) = m + 1 > H_1, H_2: then H_1 = H_2 = M = m and
  E = 2m − 4m − 2 + 2m + 2 = 0. ∎

**Proof of Theorem 3.** Play rule R against any Adversary. Φ starts at 0, and
by Lemma 4 each round costs at most the rise of Φ in that round, so the total
cost is at most Φ of the final tree, which by Lemma 5 is at most
(n − 1)/2 − H. If the total cost is positive then Φ > 0 at the end, so some
edge x → c has h(c) ≥ 1 and H ≥ 1; the total is then at most (n − 3)/2, and
it is an integer. So rule R never pays more than max(0, ⌊(n − 3)/2⌋), and
T(n) is at most that. ∎

Lemma 4 on all pairs of directed trees with at most 10 vertices each and at
most 15 together, and Lemma 5 on all directed trees with at most 10 vertices,
were also checked by enumeration (`upper/lemma4_check.py`,
`upper/lemma5_check.py`). A vertex with h = 3 needs 15 vertices, so these
runs only contain flipped paths of length at most 2; longer paths are
exercised by `upper/pool_search.cpp`, which builds trees by rule R and tests
Lemma 4 on every round it generates, but not exhaustively: run as
`pool_search 160 10` it tested 972,225,750 rounds with no violation, of
which 94,104,638 had a flipped path of length 3, 4 or 5. The potential was found by a linear program fitted to such
rounds; the proof above does not depend on that.

**The two bounds compared.** Write U(n) = max(0, ⌊(n − 3)/2⌋). Computed from
the table of f (`upper/compare_bounds.py`, for n ≤ 4000 only): f(n) ≤ U(n)
for every n ≤ 95; U(n) ≤ f(n) for every n from 94 to 4000; and U(n) < f(n)
at n = 96 and for every n from 126 to 4000. We do not prove the comparison
beyond 4000. For n ≤ 15 the exact values below are no larger than either.

**Proof of Theorem 4.** Define directed trees A_j with a marked vertex, the
root. A_1 is a vertex, the root, with two out-neighbours of out-degree 0; it
is built by two rounds of cost 0, naming the root with each of the other two
vertices, with the ties (all h are 0) broken so that both edges are directed
out of the root. To build A_j, the Adversary builds three
copies X, Y, Z of A_(j−1) with roots x, y, z, then names {x, y}, then {x, z}.
We show by induction the invariant I(j): the out-tree of the root of A_j is a
complete binary out-tree of depth j, with saturated inner vertices and with
its 2^j deepest vertices of out-degree 0. I(1) holds. Assume I(j − 1).

When {x, y} is named, h(x) = h(y) = j − 1. The two choices of p give the same
position up to renaming; say p = x. Every shortest directed path from x to an
unsaturated vertex ends at a deepest vertex of the out-tree of x and has
length j − 1, so the round costs j − 1. Afterwards the out-neighbours of x are
y and the out-neighbour c of x that was not on the flipped path. The
out-tree of c is unchanged, a complete binary out-tree of depth j − 2, so
h(c) = j − 2, h(y) = j − 1 and h(x) = j − 1.

When {x, z} is named, h(x) = h(z) = j − 1; the Adversary breaks the tie so
that p = x. Every shortest path from x now goes through c, and the round
costs j − 1. Afterwards the out-neighbours of x are y and z, whose out-trees
are unchanged, and I(j) holds with root x.

So the cost c_j of building A_j satisfies c_1 = 0 and
c_j = 3·c_(j−1) + 2(j − 1), which gives c_j = (3^j − 1)/2 − j. ∎

A_j has 3^j vertices and its root has h = j. Its cost is at most
(n − 1)/2 − H by the proof of Theorem 3, which gives H ≤ j, so H = j and the
cost equals that bound: the proof of Theorem 3 is exact on this family.
`upper/ternary_family.py` plays these rounds for j ≤ 9 (19,683 vertices) and
checks each cost.

**The worst case of rule R.** Let W(n) be the most that rule R can be made to
pay on n vertices when the Adversary also breaks its ties. Then
W(n) ≤ U(n) by the proof of Theorem 3, and W(a + b) ≥ W(a) + W(b), since
rule R looks only at the two trees named. Write n in base 3 with digits d_i,
let J = ⌊log₃ n⌋ and c_0 = 0; then W(n) ≥ Σ d_i·c_i ≥ n/2 − (J + 1)². So
W(n) = n/2 − O(log² n). The construction relies on the Adversary breaking
ties in two places: when A_1 is built, and in the second of the two rounds
at every level. The tie in the first of the two rounds is symmetric, and the
choice among shortest paths does not matter. We have not determined the
worst case of rule R under every fixed tie-breaking rule; Section 6 gives
one that pays at most (n − 1)/3. For n ≤ 15, exhaustive
search gives W(n) = f(n) except at n = 14, where W(14) = 3 < f(14) = 4 (one program,
`upper/greedy_exact.cpp`; its output is `upper/greedy_exact_log.txt`).

**The limit exists.** T(a + b) ≥ T(a) + T(b): the Adversary plays an optimal
strategy on a set of a vertices, then on the other b looking only at those,
then joins the two trees. Flips the Algorithm makes outside the part being
played only add to what it pays, so its payments within each part are at
least the value of that part. Since T(n)/n is bounded (Theorem 2), Fekete's
lemma gives L = lim T(n)/n = sup T(n)/n. By Theorems 1 and 5, L = 1/3.

**Small cases** (details, proofs and programs in [README.md](README.md)).

- A simpler strategy forces B(n) = max(0, ⌊n/4⌋ − 1) for every n (proved by
  hand, `solver-b/proofs.txt` Lemma 2). T(n) = B(n) for every n ≤ 15, by
  exhaustive search with three separate programs; for n = 14 and 15 the
  search was over normal replies and relies on Lemma 1.
- T(30) ≥ 7 > B(30) = 6, T(32) ≥ 8 and T(64) ≥ 18, by certificates against
  normal replies (valid by Lemma 1) verified with a separate checker. The
  certificate for n = 64 is 10 MB and is not in the repository; the program
  that generates it is. T(n) > B(n) for every n ≥ 30; for n between 36 and
  61 this rests on two programs for the abstract game below and no
  certificate.
- The least n with T(n) > B(n) is not known. It lies between 17 and 30, or
  between 16 and 30 if the value T(16) = 3, whose upper half rests on one
  program, is set aside.

**A limit of the method** (proved in `open-limit/proofs.txt`, Theorem 3 of
that file, not here). Abstract the strategy of Section 4 to a *pair game* on multisets of
pairs (p, q), one per tree, starting from n copies of (−1, −1): the
Adversary picks two pairs, the Algorithm picks one of the four alternatives
of Lemma 2 and pays its cost bound, and the Adversary keeps either the new
pair or the untouched one. By Lemma 2 the value of the pair game is a lower
bound on T(n), and the strategy of Section 4 is a strategy of it. Its value
is at most (n − 1)/3: with g(p, q) = q + φ(q − p), φ(0) = 4/3, φ(1) = 2/3,
φ(d) = 1/3 for d ≥ 2, the Algorithm can always choose an alternative that
keeps paid(C) + g(pair of C) ≤ |C|/3. So the pair game is worth exactly
n/3 up to lower-order terms. (This was proved before Theorem 5, which now
gives the same ceiling for the real game.)

In the other direction, the natural attempt to turn g into an Algorithm on
real forests fails. Let G(C) be the maximum of g over the slot values of the
vertices of C; the Algorithm would need, for every named pair, a normal reply
with cost + G(new tree) ≤ G(X) + G(Y). This holds for all pairs of trees
with at most 7 vertices each and fails for a 9-vertex oriented tree named
against a copy of itself (computed; same file, Section 8). Section 6 uses
a different potential, a sum over all vertices of weights built from the
same g.

## 6. Proof of Theorem 5

Recall the slots of a vertex v (Section 3): the values h(c) of its
out-neighbours c, and −1 for each missing one. Write them sorted, (P, Q)
with P ≤ Q; then h(v) = P + 1. Let ε(v) = 0 if P = Q and ε(v) = 1 if P < Q.
So ε(v) = 0 exactly when v has no out-neighbour or two out-neighbours of
equal h.

**Rule L.** For the named pair, let x be an endpoint with

    2·h(x) + ε(x) ≤ 2·h(y) + ε(y),

y being the other endpoint (if both qualify, either). Flip a shortest
directed path x = w_0 → w_1 → … → w_k from x to an unsaturated vertex, any
such path, and direct the new edge from x to y. The cost is k = h(x).

In words: flip from the endpoint of smaller h; if the two h are equal and
exactly one endpoint has its two slots equal, flip from that one; otherwise
flip from either. Since h(x) ≤ h(y), rule L is rule R of Section 5 with some
of its ties decided, and so it is the algorithm of [7] with a tie-breaking
rule. The choice among shortest paths stays free.

**The potential.** In this section every quantity is multiplied by 3, so
that all of them are integers: a round of cost k has to raise the potential
by 3k. For a vertex c put

    γ(c) = 3·h(c) + 1 + ε(c).

The weight w(v) of a vertex v depends on its out-neighbours:

    no out-neighbour:                             w = 0
    one out-neighbour c:                          w = ε(c) if h(c) = 0;   w = 1 if h(c) ≥ 1
    two, of heights p ≤ q, with flags ε_p, ε_q:   w = 3p − 1 + ε_p + ε_q    if q = p
                                                  w = 3p + 1 + ε_p + ε_q    if q = p + 1
                                                  w = 3p + 2 + ε_p          if q ≥ p + 2

Here "height" means h and "flag" means ε. Φ is the sum of w over all
vertices; it is 0 when there are no edges, and Φ of a position is the sum of
Φ over its trees. A weight can be negative: it is −1 for a vertex whose two
out-neighbours both have no out-neighbour.

Equivalently, with (P, Q) the slots of v,

    w(v) = 1 + Σ_c γ(c) − max( γ(c) over the out-neighbours c of v,  g(v) ),

where g(v) = 3Q + 4, 3Q + 2 or 3Q + 1 according as Q − P is 0, 1 or at least
2, for a vertex v with at least one out-neighbour. This g is three times the
g(p, q) of the paragraph "A limit of the method" in Section 5. Note that
g(v) ≥ γ(v), with equality unless Q − P ≥ 2. That the table and the formula
agree is a direct computation in each line of the table;
`upper/third_check.py`, part (0), also checks it for all heights below 30.

**Lemma 6 (every tree).** For every directed tree T on n vertices with all
out-degrees at most 2,

    Φ(T) ≤ n − max over v of γ(v) ≤ n − 1.

No assumption is made on how T arose.

*Proof.* Induction on n. For n = 1, Φ = 0 and γ = 1. Let n ≥ 2 and let r be
a vertex of in-degree 0, which exists since T has n − 1 edges. No out-tree
of another vertex contains r, so deleting r changes no other h, slot, γ or
w. Write Θ(T) for the maximum of γ over T; then Θ(T) is the maximum of γ(r)
and of Θ over the trees of T − r.

If r has one out-neighbour c, then T − r is a tree T_1. From the formula
with the maximum and g(r) ≥ γ(r), w(r) ≤ 1 + γ(c) − max(γ(c), γ(r)), so by
induction

    Φ(T) ≤ (n − 1) − Θ(T_1) + 1 + γ(c) − max(γ(c), γ(r)).

For fixed s the function t ↦ max(t, s) − t is non-increasing, and
Θ(T_1) ≥ γ(c), so max(Θ(T_1), γ(r)) − Θ(T_1) ≤ max(γ(c), γ(r)) − γ(c). Hence
Φ(T) ≤ n − max(Θ(T_1), γ(r)) = n − Θ(T).

If r has two out-neighbours c_1, c_2, in the trees T_1, T_2 of T − r, of
sizes n_1 + n_2 = n − 1, then
w(r) ≤ 1 + γ(c_1) + γ(c_2) − max(γ(c_1), γ(c_2), γ(r)). For fixed t the
function (a, b) ↦ a + b − max(a, b, t) is non-decreasing in a and in b.
With Θ_i = Θ(T_i) ≥ γ(c_i),

    Φ(T) ≤ n_1 − Θ_1 + n_2 − Θ_2 + 1 + Θ_1 + Θ_2 − max(Θ_1, Θ_2, γ(r)) = n − Θ(T). ∎

**Lemma 7 (monotone).** The weight of a vertex does not decrease when one
slot of one of its out-neighbours is raised by one, the out-neighbours
themselves staying the same. (The slots stay sorted: the larger slot may
always be raised, the smaller one only if it is below the larger.)

*Proof.* Raising one slot of an out-neighbour c changes the pair
(h(c), ε(c)) in one of four ways: (S1) (h, 0) → (h, 1), when the slots were
equal; (S2) no change, when the larger of two different slots is raised;
(S3) (h, 1) → (h + 1, 0) and (S4) (h, 1) → (h + 1, 1), when the smaller slot
is raised.

*One out-neighbour.* The weight is ε(c) if h(c) = 0 and 1 if h(c) ≥ 1. (S1)
raises it from 0 to 1 if h(c) = 0 and leaves it at 1 otherwise. (S3) and
(S4) start from flag 1, so from weight 1, and end at height at least 1, so
at weight 1.

*Two out-neighbours, c the one of smaller height p,* the other of height
q ≥ p and flag ε_q. (S1) adds 1 in each of the three lines of the table. For
(S3) and (S4) the old flag of c is 1 and its new height is p + 1, with a new
flag ε′:
- q = p: old 3p + ε_q; new (heights p and p + 1) 3p + 1 + ε_q + ε′.
- q = p + 1: old 3p + 2 + ε_q; new (heights equal) 3p + 2 + ε′ + ε_q.
- q = p + 2: old 3p + 3; new 3p + 4 + ε′ + ε_q.
- q ≥ p + 3: old 3p + 3; new 3p + 5 + ε′.

*Two out-neighbours, c the one of larger height q > p,* the other with flag
ε_p. (S1) adds 1 if q = p + 1 and nothing if q ≥ p + 2. For (S3) and (S4):
if q = p + 1 the old weight is 3p + 2 + ε_p and the new one, with heights p
and p + 2, is 3p + 2 + ε_p; if q ≥ p + 2 nothing changes. ∎

`upper/third_check.py`, part (M), also checks every single-slot increase
with slots below 30.

**Lemma 8 (the path pays).** In a round of rule L of cost k, the vertices
w_0, …, w_k of the flipped path together gain at least 3k in weight. (For
k = 0: the vertex x does not lose weight.)

*Notation.* As in Lemma 4: h(w_i) = k − i; for i < k, c_i is the
out-neighbour of w_i other than w_(i+1), and a_i = h(c_i) ≥ k − i − 1; put
α_i = a_i − (k − i − 1) ≥ 0. Let b = h(y) ≥ k and β = b − k. The end w_k has
no out-neighbour or one, z. As shown in the proof of Lemma 4, the out-trees
of the c_i, of z and of y contain no path vertex, so their h and their ε do
not change. After the round the out-neighbours are: of w_0, c_0 and y; of
w_i (1 ≤ i < k), c_i and w_(i−1); of w_k, w_(k−1) and z if there is one.
Let h′ be the new h. Then h′(w_0) = 1 + min(a_0, b) and
h′(w_i) = 1 + min(a_i, h′(w_(i−1))) for 1 ≤ i < k. Put
o_i = h′(w_i) − h(w_i). Then

    o_0 = min(α_0, β + 1),      o_i = min(α_i, o_(i−1) + 2)  for 1 ≤ i < k,

and in particular o_i ≥ 0. The new slots of w_0 are equal exactly when
α_0 = β + 1, and those of w_i (1 ≤ i < k) exactly when α_i = o_(i−1) + 2.
The old slots of w_i (i < k) are equal exactly when α_i = 0, and those of
w_k exactly when it has no out-neighbour.

*What rule L gives.* Besides h(x) ≤ h(y), the proof uses one consequence of
rule L: if β = 0 and ε(y) = 0, then ε(x) = 0. For k ≥ 1 this says α_0 = 0;
for k = 0 it says that if y has no out-neighbour then x has none.

*The carry.* For i < k define the state s_i = (o_i, e_i, f_i), where e_i
says whether the new slots of w_i are equal and f_i whether the old slots of
w_(i+1) are equal. Let

    π(o, e, f) = −1  if f is false and (o, e) is one of (0, false), (1, true), (1, false), (2, true);
    π(o, e, f) = 0   otherwise.

*Claim.* Writing gain(v) for the new weight of v minus the old one:
(a) gain(w_0) ≥ 2 + π(s_0);
(b) gain(w_i) ≥ 3 + π(s_i) − π(s_(i−1)) for 1 ≤ i < k;
(c) gain(w_k) ≥ 1 − π(s_(k−1));
(d) if k = 0, gain(x) ≥ 0.

For k ≥ 1, adding (a), (b) for each i and (c), the carries cancel and the
total gain is at least 2 + 3(k − 1) + 1 = 3k.

Every weight involved is given by the table. By the table, a gain is
unchanged if all heights are shifted by the same amount, provided the vertex
has two out-neighbours before and after; that is so for w_0, …, w_(k−1).
For i < k measure heights from ℓ = h(w_(i+1)) = k − i − 1, and write
ε_n = ε(w_(i+1)) before the round, ε_c = ε(c_i) and α = α_i. The old weight
of w_i, minus 3ℓ, is

    −1 + ε_n + ε_c  (α = 0);     1 + ε_n + ε_c  (α = 1);     2 + ε_n  (α ≥ 2).

(b) *Interior.* The new out-neighbours are c_i, of height ℓ + α, and
w_(i−1), of height ℓ + 2 + o with o = o_(i−1) and with new flag ε′. Put
d = α − o − 2. The new weight, minus 3ℓ, is

    3α + 2 + ε_c (d ≤ −2);    3α + 1 + ε_c + ε′ (d = −1);    3α − 1 + ε_c + ε′ (d = 0);
    3o + 7 + ε_c + ε′ (d = 1);    3o + 8 + ε′ (d ≥ 2).

The right side of (b) is at most 4. If α ≥ 3, the old weight is at most 3
and the new one at least 7. If α ≤ 2 and o ≥ 3, then d ≤ −2, so the new
weight and the new state (α, false, f_i) do not depend on o, and
π(s_(i−1)) = 0; so it is enough to take o ∈ {0, 1, 2, 3}. That leaves
α ∈ {0, 1, 2}, o ∈ {0, 1, 2, 3} and the flags: finitely many cases.

(a) *Start.* The new out-neighbours are c_0, of height ℓ + α, and y, of
height ℓ + 1 + β and flag ε_y. Put d = α − β − 1. The new weight, minus 3ℓ,
is

    3α + 2 + ε_c (d ≤ −2);    3α + 1 + ε_c + ε_y (d = −1);    3α − 1 + ε_c + ε_y (d = 0);
    3β + 4 + ε_c + ε_y (d = 1);    3β + 5 + ε_y (d ≥ 2).

The right side of (a) is at most 2. If α ≥ 3, the old weight is at most 3
and the new one at least 5. If α ≤ 2 and β ≥ 3, then d ≤ −2 and nothing
depends on β. That leaves α ∈ {0, 1, 2}, β ∈ {0, 1, 2, 3} and the flags,
without the cases excluded by rule L (β = 0, ε_y = 0, α ≥ 1).

(c) *End.* The new out-neighbour w_(k−1) of w_k has height 1 + o, with
o = o_(k−1), and its flag is given by e_(k−1). If w_k had no out-neighbour,
its weight goes from 0 to 1, and π(s_(k−1)) = 0 because f_(k−1) is true. If
it had z, of height ζ and flag ε_z, the old weight is ε_z (ζ = 0) or 1
(ζ ≥ 1), and the new one is given by the table for heights ζ and 1 + o. The
right side of (c) is at most 2. If ζ ≥ 3, or ζ = 2 and o ≥ 1, the new weight
is at least 5 and the old one at most 1. If o ≥ ζ + 1, the new weight
3ζ + 2 + ε_z does not depend on o. That leaves ζ ≤ 2, o ≤ 3 and the flags.

(d) *k = 0.* Here x is unsaturated and gains the out-neighbour y. If x had
no out-neighbour, its weight goes from 0 to ε(y) or 1, which is at least 0.
If x had an out-neighbour z, the case that y has no out-neighbour is
excluded by rule L. Let z have height ζ and flag ε_z, and y height b and
flag ε_y; the old weight is ε_z if ζ = 0 and 1 if ζ ≥ 1, and the new one is
given by the table for the heights ζ and b. If both heights are at least 1,
the new weight is at least 2. If ζ = 0 and b ≥ 2, it is 2 + ε_z. If b = 0 and
ζ ≥ 2, it is 2 + ε_y. If ζ = 0 and b = 1, or ζ = 1 and b = 0, it is
1 + ε_z + ε_y. If ζ = b = 0, it is −1 + ε_z + ε_y, and ε_y = 1 because y has
an out-neighbour, so it is ε_z, the old weight.

The finite cases of (a) to (d) are checked by `upper/third_check.py`,
part (P), which runs over all values of α, β, o and ζ below 30 and so
contains the lists above. ∎

**Proof of Theorem 5.** Play rule L against any Adversary and consider one
round, of cost k. By the first step of the proof of Lemma 4, which uses only
that the flipped path is a shortest path from x, that y is in another tree
and that h(y) ≥ h(x), no h decreases. Hence the sorted slots of every vertex do not decrease in either
coordinate. For a vertex off the path this is because it keeps its
out-neighbours. For the path vertices it is the computation in Lemma 8: the
old slots of w_i (1 ≤ i < k) are (k − i − 1, a_i) and the new ones are a_i
and h′(w_(i−1)) ≥ k − i + 1; the old slots of w_0 are (k − 1, a_0) and the
new ones a_0 and b ≥ k; and w_k (or x, if k = 0) keeps its out-neighbour, if
any, and gains one, which is a slot rising from −1.

A vertex off the path keeps its out-neighbours, some of which may be path
vertices. A rise of an out-neighbour's sorted slots in both coordinates is a
chain of single-slot rises that keep the slots sorted (raise the larger slot
step by step, then the smaller), and the out-neighbours can be treated one
after the other. So by Lemma 7 the weight of a vertex off the path does not
decrease. By Lemma 8 the path vertices together gain at least 3k. So Φ rises
by at least 3k in a round of cost k.

Φ starts at 0. So three times the total cost is at most Φ of the final
tree, which by Lemma 6 is at most n − 1. The total cost is an integer, so
rule L never pays more than ⌊(n − 1)/3⌋, and T(n) is at most that. ∎

**A finer statement.** The same proof gives: the rounds of rule L that built
a tree C cost at most (|C| − max_v γ(v))/3 ≤ (|C| − 1)/3 − H(C) in total,
where H(C) is the largest h in C.

**Checks by program.** None of these is needed for the proof, except that
the finite cases of Lemma 8 were checked by program and not by hand; the
Lean development (Section 7) proves (a) to (d) for all values without that
reduction. The output of the runs below is in `upper/third_log.txt`.

- `upper/third_check.py 30`: the table against the formula with the maximum;
  Lemma 7; the inequalities (a) to (d).
- `upper/third_paths.py 5 4`: for every flipped path of length at most 5 and
  every combination of the α_i up to 4, of β up to 4, of the flags and of
  the end vertex (with the height of its out-neighbour up to 4), the total gain of the path, computed from the definitions
  without the carry, is at least 3k (11,244,432 profiles).
- `upper/third_trees.py 8 14`: Lemma 6 on every directed tree with at most 8
  vertices, and the inequality of a round for every pair of such trees with
  at most 14 vertices together, every pair of endpoints allowed by rule L
  and every shortest path (10,721,542 rounds). These trees are too small for a
  flipped path longer than 2.
- `upper/third_pool.cpp`, run as `third_pool 128 6`: trees of up to 128
  vertices built by rule L, kept selectively and with one endpoint tried
  per class of vertices with equal h and slots; the inequality of a round and
  the finer statement on each of 136,196,918 rounds, of which 5,277,311 had a
  flipped path of length 3 or more.

**How the potential was found.** A search over tie-breaking rules for
rule R found one (on a tie, flip in the smaller tree) for which the largest
cost found equalled the pair-game lower bound on T(n) at every n from 26 to
66. Linear programs then fitted vertex weights to
recorded rounds. A first closed form passed every test and was false: a
vertex above the flipped path lost weight, and there can be any number of
such vertices (the example has 43 vertices and was found while trying to
prove the inequality, not by the tests). Requiring the weights to be
monotone (Lemma 7) and only the path to pay (Lemma 8) gave a feasible
program whose values suggested the formula with γ; a second linear program
gave the carry π. The proof above does not depend on any of that, and the
programs of that search, including the 43-vertex example, are not in the
repository.

**The tie-break is needed for this potential.** If x is chosen with
h(x) = h(y) but against rule L, a round can raise Φ by less than three times
its cost: the Lean development contains an 8-vertex position where a round
of cost 1 raises Φ by 2 (`lean/SPEC.md`). With the ties left to the
Adversary the cost itself can be about n/2 (Theorem 4).

## 7. Machine check

`lean/` contains a Lean 4 development (toolchain 4.36.0, core library only,
about 4,000 lines) proving

    theorem T_lower_bound (n : Nat) (hn : 1 ≤ n) :
        n + 3 ≤ 3 * T n + 7 * Nat.log2 (n + 1)

    theorem upper_third : ∀ n : Nat, 1 ≤ n → 3 * FO.T n + 1 ≤ n

The first says T(n) ≥ n/3 + 1 − (7/3)·⌊log₂(n+1)⌋, which implies Theorem 1.
The second is Theorem 5: for n ≥ 1 it says 3·T(n) ≤ n − 1, and T(n) is an
integer. Both are about the full game, with every legal reply, not the game
restricted to normal replies. Both depend on the same 89-line specification,
`lean/FO/Spec.lean`, in which a position is a list of directed edges on
`Fin n`, the Algorithm's replies are all valid re-orientations, and `T n` is
the max-min value over n − 1 rounds computed by backward induction. Both use
only the axioms `propext`, `Classical.choice` and `Quot.sound`.

The proof of the first follows Sections 2 to 4: Lemma 1 and the shadow play,
the lemmas on slots and types, the potential. The proof of the second
(`lean/FO/Upper*.lean`, about 2,040 lines) follows Section 6, with these
differences, listed in full in `lean/SPEC.md`:

- The inequality of a round is proved for every endpoint x allowed by
  rule L and every shortest path from x (`round_gain`). The reply that the
  final induction plays is one of these, with a fixed choice of endpoint
  when both qualify and a fixed path (`ruleL`).
- Lemma 6 is proved in the form Φ ≤ number of edges, for forests, which is
  what the final induction needs.
- Inequalities (a) to (d) of Lemma 8 are proved for all values of the
  parameters, not by reduction to finitely many cases.
- The finer statement at the end of Section 6 is not formalised.

What this does and does not give:

- It removes the need to trust the hand proofs of Theorems 1 and 5,
  including the step from normal replies to all replies in Theorem 1.
- It does not check that the 89 lines say what Section 1 says. Five
  reviewing models read the specification against the game for that purpose
  and found no discrepancy; a human reader should still read it.
- The two statements are easy to compare with small cases: T(1) = T(2) =
  T(3) = 0 = ⌊(n − 1)/3⌋, and the exact values for n ≤ 15 lie between the
  two bounds. The lower bound first forces T(n) ≥ 1 at n = 26 (it does so
  for n = 26 to 30, says nothing at 31 and 32, and does so again for every n
  from 33 on), which is beyond any exhaustive computation of T. At n = 1000
  the two theorems give 314 ≤ T ≤ 333.
- That the backward-induction value equals the value under history-dependent
  strategies is standard for finite games of perfect information and is not
  formalised.
- Nothing else in this note is formalised: not Theorems 2 to 4, the limit as
  such, the small cases or the abstract game.
- The files for Theorem 5 were written by one instance of the model and
  rebuilt from clean by another; what the reviewing models read of them is
  stated in the README.

## 8. Related work

This section reports a literature search done by instances of the same AI
model, not the judgement of someone who works in the area. Each item says
how far the source was read. The search is recorded in
[prior-art.md](prior-art.md) and [prior-art-deep.md](prior-art-deep.md);
sources [7] and [8] and the statements about [2] and [9] below were read
afterwards, for this note.

**Upper bounds, known.**

- Gupta, Kumar and Stein [7] (Section 2 read in full; the rest scanned for
  lower bounds). Their Section 2 is this problem, stated for in-degree: edges
  of a forest arrive online, load at most 2 is maintained, and Theorem 2.1
  gives an algorithm with at most 2n flips in total, by flipping the shorter
  of the two shortest directed paths to an unsaturated vertex. Ozdayi [13]
  (Sections 2 and 3 read, in the HTML version) sharpens their analysis to
  n − log₂ n − 1 flips for n edges, from the same recurrence. Theorems 2
  and 3 above are tighter analyses of that algorithm, Theorem 4 shows that
  n/2 is its worst case up to lower-order terms when its ties are broken
  against it (Section 5), and Theorem 5 shows that with the ties broken by
  rule L it pays at most (n − 1)/3 (Section 6). They also
  recall that keeping load 1 can require Ω(n log n) flips, citing Grove et
  al. [12]. Section 2 states no lower bound on
  flips at load 2, and our scan of the rest of the paper found none.
- Brodal and Fagerberg [1] (Sections 3 to 5 read). A different rule; for
  trees it gives bound 2 with an amortized 3 flips per insertion. We read
  that in survey slides by Gupta [8] and derived a total of 3(n − 1) from
  Lemma 1 of [1]; we did not find it stated for forests in [1].
- Bender et al. [2] (introduction read, rest scanned): with out-degree 3, at
  most O(log log n) flips per insertion and O(1) amortized. Their footnote 1
  notes that with out-degree 2, path flipping gives O(log n) flips per
  insertion in the worst case. Efron, Patel and Stein [5] (Lemma 5 and its
  proof read) credit that result for trees to [7].
- Marques Domingues, Nguyen and Solomon [3] (Sections 1, 6.3 and 7 read)
  describe a random-walk rule for out-degree 2 on forests as folklore.

**Lower bounds.** We found no lower bound on the total number of flips for
insertion-only forests at out-degree 2 that holds for every algorithm, and
none of the values of T(n). We
also found no upper bound on the total better than the n − log₂ n − 1 of
[13]. A separate search for upper bounds and for the constant 1/3 (18
November 2026, one instance) found [13] and nothing closer. The nearest statements:

- Ozdayi [13], Lemma 3.2 (read): every variant of the shortest-path
  algorithm of [7], with ties broken in any way, can be forced to flip
  (n − 3)/4 edges in n insertions, for suitable n. This is a lower bound on the total at
  out-degree 2, but for that class of algorithms only; the bound B(n) of
  Section 5, of the same order, and Theorem 1 hold for every algorithm. The
  same source shows that such an algorithm can be forced to flip 2 edges in
  one step. It states no tie-breaking rule and no bound of the form n/3.

- [1], Theorem 4 (read): with deletions allowed and out-degree equal to the
  arboricity c, Ω(n/c²) flips can be forced even amortized. This needs
  deletions and has no slack.
- [3], Theorem 1.4 (read): for out-degree 2 on outerplanar graphs, a
  sequence of updates starting from no edges forces Ω(n) amortized flips
  even offline. The same paper remarks that its example on trees does not
  imply an Ω(log n) worst-case lower bound for an algorithm starting from
  the empty graph.
- Hiken and Wein [4] (Section 1.3 read), on update time for fully dynamic
  orientation: "there is no non-trivial lower bound … Moreover, there has
  been no progress on closing this gap even for dynamic forests" (they add
  α = 1). This is about time, not flips, and with deletions.
- Online bipartite matching with replacements. Bernstein, Holm and
  Rotenberg [6] (Sections 1 and 6.1 read) report upper and lower bounds of
  order n log n on total replacements when the graph stays a forest, citing
  Chaudhuri, Daskalakis, Kleinberg and Lin [10] (introduction and related
  work read) and Grove et al. [12]. Our reading is that this is
  the case without slack, capacity 1, corresponding to out-degree 1.
- Kaplan, Naori and Sadeh [9] (abstract read, rest scanned) prove an Ω(1)
  amortized recourse lower bound for edge colouring of incremental forests,
  a neighbouring problem.

**Two older sources, read last** (18 November 2026, by one instance, from
open copies; whole text of each).

- He, Tang and Zeh [11] contains no lower bound on reorientations. Its
  bounds for insertions need out-degree at least 10 on forests (our
  arithmetic from its Theorem 1).
- Grove et al. [12] is an extended abstract on competitive ratios, with
  most proofs sketched; the extraction lost the Ω and √ signs, which we
  restore from context. Its only lower bound at capacity 2 on a tree is
  Theorem 11, Ω(√n) on the competitive ratio, and its proof uses
  disconnections. Without disconnections its Theorem 3 gives a
  2-competitive algorithm on a tree of stations fixed in advance, with each
  customer switched at most once.

A reader who knows this literature may well know a reference we missed, and
we would like to hear of it.

**References**

1. G. S. Brodal, R. Fagerberg. Dynamic Representations of Sparse Graphs.
   WADS 1999, LNCS 1663, pp. 342–351. doi:10.1007/3-540-48447-7_34
2. M. A. Bender, T. Kopelowitz, W. Kuszmaul, E. Porat, C. Stein. Incremental
   Edge Orientation in Forests. 2021. arXiv:2107.02318
3. G. Marques Domingues, M. H. Nguyen, S. Solomon. Dynamic Edge Orientation
   via Random Walks: From Trees to Outerplanar Graphs and Beyond. 2026.
   arXiv:2608.24776
4. S. Hiken, N. Wein. Dynamic Edge Orientation on Low-Arboricity Graphs:
   Breaking the log n Barrier. 2026. arXiv:2611.10161
5. Y. Efron, S. Patel, C. Stein. A Simple Algorithm for Dynamic Carpooling
   with Recourse. 2024. arXiv:2411.07553
6. A. Bernstein, J. Holm, E. Rotenberg. Online Bipartite Matching with
   Amortized O(log² n) Replacements. 2017. arXiv:1707.06063
7. A. Gupta, A. Kumar, C. Stein. Maintaining Assignments Online: Matching,
   Scheduling, and Flows. SODA 2014, pp. 468–479.
   doi:10.1137/1.9781611973402.35
8. A. Gupta. A Little Change will do you Good. Survey talk slides,
   https://cs.nyu.edu/~anupamg/talks/shonan-recourse.pptx
9. H. Kaplan, D. Naori, Y. Sadeh. Dynamic Edge Coloring of Forests. 2026.
   arXiv:2605.09711
10. K. Chaudhuri, C. Daskalakis, R. D. Kleinberg, H. Lin. Online Bipartite
    Perfect Matching With Augmentations. INFOCOM 2009.
11. M. He, G. Tang, N. Zeh. Orienting Dynamic Graphs, with Applications to
    Maximal Matchings and Adjacency Queries. ISAAC 2014, LNCS 8889.
12. E. F. Grove, M.-Y. Kao, P. Krishnan, J. S. Vitter. Online Perfect
    Matching and Mobile Computing. WADS 1995, LNCS 955, pp. 194–205.
13. M. S. Ozdayi. Achieving Competitiveness in Online Problems. Semester
    project, EPFL, supervised by O. Svensson and S. Huang. 2020.
    arXiv:2001.03015

## 9. Open questions

1. Is n/3 − T(n) unbounded? Theorems 1 and 5 give
   1/3 ≤ n/3 − T(n) ≤ (7/3)·log₂(n+1) − 5/6.
2. The least n with T(n) > B(n), and whether T(4^k) = (4^k − 1)/3 − k.
3. The same question for out-degree bound c ≥ 3 on forests, and for graphs
   of arboricity α with bound α + 1.
4. Whether Theorem 1 or Theorem 5 is already known (Section 8).

## How this was produced

All of the mathematics here was produced by instances of one AI model (Claude,
Anthropic) in an experiment on whether such models can pose and settle small
checkable questions: one chain of instances posed the problem, two solved it
independently, others refereed, and further ones found the upper bounds and
wrote the Lean development. The claims were then reviewed by five models
from four vendors reading the files; what each review covered is in the
README. The record of that process, including the errors found on the way,
is in [README.md](README.md). No human mathematician has yet checked the
proofs; the Lean checks of Theorems 1 and 5 are offered in place of that for
the two main results. Corrections are welcome as issues on the repository.
