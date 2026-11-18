# A lower bound of n/3 − O(log n) on total flips for 2-orientations of incremental forests

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

**Theorem 2.** For every n ≥ 16, T(n) ≤ (4/7)·n − log₂(n+1).

So the limit L of T(n)/n, which exists, satisfies 1/3 ≤ L ≤ 4/7. Theorem 1 is
machine-checked in Lean 4 in the integer form n + 3 ≤ 3·T(n) + 7·⌊log₂(n+1)⌋
(Section 6). Theorem 2 is proved by hand with a finite check by program. It
holds for the algorithm of Gupta, Kumar and Stein [7, Theorem 2.1], who
prove 2n; what is added is the analysis (Section 5). We do not
know the value of L.

A forest can always be oriented with out-degree at most 1. Keeping that
bound as edges arrive can require Ω(n log n) flips, and Gupta, Kumar and
Stein [7] showed that relaxing the bound to 2 brings the total down to O(n).
Theorem 1 says that this is the right order and that the amortized number of
flips per insertion cannot be below 1/3 − o(1). A constant amortized lower
bound is not surprising. Our search did not find one stated, nor this
constant (Section 7). Besides the constant, the note offers a proof short
enough to check and a statement of what the method cannot do (Section 5).

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

## 5. The upper bound, and what else is known

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

**Proof of Theorem 2.** Put N = 16 and α = 4/7. We show
f(n) ≤ αn − log₂(n+1) for every n ≥ N. Two finite facts are needed, both
checked in integer arithmetic by `upper/greedy_recurrence.py` from the table
of f(1), …, f(30), which is 0,0,0,0,0,1,1,1,2,2,2,3,3,4,4,5,5,6,6,6,7,7,8,8,9,9,10,10,10,11:

- (i) f(n) ≤ αn − log₂(n+1) for 16 ≤ n ≤ 30;
- (ii) f(a) + c(a) + log₂(1 + a/17) ≤ αa for 1 ≤ a ≤ 15.

Let n ≥ 31 and suppose the claim holds for all m with 16 ≤ m < n. Take any
a ≤ b with a + b = n; then 16 ≤ b < n. If a ≥ 16, then since
c(a) ≤ log₂(a+1) − 1,

    f(a) + f(b) + c(a) ≤ αn − log₂(b+1) − 1 ≤ αn − log₂(n+1),

because 2(b + 1) ≥ n + 1. If a ≤ 15, then

    f(a) + f(b) + c(a) ≤ αn − log₂(n+1) + [f(a) + c(a) + log₂((n+1)/(b+1)) − αa],

and the bracket is at most 0 by (ii), since (n+1)/(b+1) = 1 + a/(b+1) ≤
1 + a/17. ∎

Below 16 the inequality of Theorem 2 fails at n = 1, 2, 3, 4, 6 and 9; for
n ≤ 15 the exact values are in the small cases below. The constant 4/7 is
not the best this argument gives. The maximum of f(n)/n is 0.5579 for
n ≤ 3000 (the program above) and 0.5621 for n ≤ 40000 (a separate run of
the same recurrence, not in the repository), and f(n)/n appears to tend to
9/16. We have not proved that, and it would bound only this analysis.

**Comparison with [7].** The algorithm is theirs, as just noted. Gupta,
Kumar and Stein bound the flipped path by log₂ of the size of the smaller
tree and get the recurrence with log₂ min(a, b) in place of c(a). They state
the bound 2n, noting 2n − log₂ n − 2 by induction. The same induction gives
n − 1 − log₂ n for their recurrence, with equality at powers of 2, so that
recurrence cannot give a constant below 1. The improvement to 4/7 comes
from two things in the path bound: the size fact, which gives
log₂(a + 1) − 1 in place of log₂ a, and rounding down, since a path length
is an integer. Without the rounding the recurrence gives about 0.69·n at
n = 1000 (computed).

**The limit exists.** T(a + b) ≥ T(a) + T(b): the Adversary plays an optimal
strategy on a set of a vertices, then on the other b looking only at those,
then joins the two trees. Flips the Algorithm makes outside the part being
played only add to what it pays, so its payments within each part are at
least the value of that part. Since T(n)/n is bounded (Theorem 2), Fekete's
lemma gives L = lim T(n)/n = sup T(n)/n.

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

**A limit of the method** (proved in `open-limit/proofs.txt`, Theorem 3, not
here). Abstract the strategy of Section 4 to a *pair game* on multisets of
pairs (p, q), one per tree, starting from n copies of (−1, −1): the
Adversary picks two pairs, the Algorithm picks one of the four alternatives
of Lemma 2 and pays its cost bound, and the Adversary keeps either the new
pair or the untouched one. By Lemma 2 the value of the pair game is a lower
bound on T(n), and the strategy of Section 4 is a strategy of it. Its value
is at most (n − 1)/3: with g(p, q) = q + φ(q − p), φ(0) = 4/3, φ(1) = 2/3,
φ(d) = 1/3 for d ≥ 2, the Algorithm can always choose an alternative that
keeps paid(C) + g(pair of C) ≤ |C|/3. So the pair game is worth exactly
n/3 up to lower-order terms, and a proof of L > 1/3, if that is true, cannot
be a strategy of this game. This does not rule out other arguments that
track a single vertex per tree.

In the other direction, the natural attempt to turn g into an Algorithm on
real forests fails. Let G(C) be the maximum of g over the slot values of the
vertices of C; the Algorithm would need, for every named pair, a normal reply
with cost + G(new tree) ≤ G(X) + G(Y). This holds for all pairs of trees
with at most 7 vertices each and fails for a 9-vertex oriented tree named
against a copy of itself (computed; same file, Section 8). Only this one
candidate is refuted.

## 6. Machine check

`lean/` contains a Lean 4 development (toolchain 4.36.0, core library only,
about 1,970 lines) proving

    theorem T_lower_bound (n : Nat) (hn : 1 ≤ n) :
        n + 3 ≤ 3 * T n + 7 * Nat.log2 (n + 1)

that is, T(n) ≥ n/3 + 1 − (7/3)·⌊log₂(n+1)⌋, which implies Theorem 1. The
statement is about the full game, with every legal reply, not the game
restricted to normal replies. It depends on an 89-line specification,
`lean/FO/Spec.lean`, in which a position is a list of directed edges on
`Fin n`, the Algorithm's replies are all valid re-orientations, and `T n` is
the max-min value over n − 1 rounds computed by backward induction. The
proof follows Sections 2 to 4: Lemma 1 and the shadow play, the lemmas on
slots and types, the potential. It uses only the axioms `propext`,
`Classical.choice` and `Quot.sound`.

What this does and does not give:

- It removes the need to trust the hand proof of Theorem 1, including the
  step from normal replies to all replies.
- It does not check that the 89 lines say what Section 1 says. Five
  reviewing models read the specification against the game for that purpose
  and found no discrepancy; a human reader should still read it.
- The inequality first forces T(n) ≥ 1 at n = 26 (it does so for n = 26 to
  30, says nothing at 31 and 32, and does so again for every n from 33 on).
  That is beyond any exhaustive computation of T, which stops at n = 16, so
  there is no numerical test of the theorem itself. At n = 1000 it gives
  T ≥ 314; Section 5 gives T ≤ f(1000) = 549.
- That the backward-induction value equals the value under history-dependent
  strategies is standard for finite games of perfect information and is not
  formalised.
- Nothing else in this note is formalised: not Theorem 2, the limit, the
  small cases or the abstract game.

## 7. Related work

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
  of the two shortest directed paths to an unsaturated vertex. Theorem 2
  above is a tighter analysis of that algorithm (Section 5). They also
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
insertion-only forests at out-degree 2, and none of the values of T(n). The
nearest statements:

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

## 8. Open questions

1. The value of L. We have 1/3 ≤ L ≤ 4/7. The recurrence of Section 5
   appears to give 9/16 and no less, so an upper bound below 9/16 needs a
   different algorithm or a finer analysis of this one.
2. Is L = 1/3? By Section 5 the pair game cannot force more than 1/3, and
   the one candidate potential for an Algorithm does not transfer to real
   trees. Neither settles the question.
3. The least n with T(n) > B(n), and whether T(4^k) = (4^k − 1)/3 − k.
4. The same question for out-degree bound c ≥ 3 on forests, and for graphs
   of arboricity α with bound α + 1.

## How this was produced

All of the mathematics here was produced by instances of one AI model (Claude,
Anthropic) in an experiment on whether such models can pose and settle small
checkable questions: one chain of instances posed the problem, two solved it
independently, others refereed, and a further one wrote the Lean
development. The claims were then reviewed by five models from four vendors
reading the files; what each review covered is in the README. The record of that process, including the errors found on
the way, is in [README.md](README.md). No human mathematician has yet checked
the proofs; the Lean check of Theorem 1 is offered in place of that for the
main result. Corrections are welcome as issues on the repository.
