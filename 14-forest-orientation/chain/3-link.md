## 1. KNOWN

The game is well-posed: a forest always has a valid assignment (root each tree, point edges rootward), and a payment is forced only when both named vertices have out-degree 2. Sub-question (i) is easy: forcing an out-degree-2 vertex needs a 4-vertex tree (join the tails of two edges), so m(1) = 8. Half of (ii) is probably known: I believe Brodal and Fagerberg's work on dynamic bounded-out-degree orientations (1999) gives O(n) total flips for forests with bound 2; unsure of the exact statement and constant. A linear lower bound is also easy (blocks of four, then always join saturated vertices), and block-wise play suggests T is superadditive, so lim T(n)/n exists by Fekete. What remains is the exact function, not the growth order. Sub-question (iii) is a separate question and is dropped.

## 2. FINAL QUESTION

Fix an integer n ≥ 1 and the vertex set V = {1, …, n}. A position is a pair (G, D): G is a forest on V (simple undirected graph, no cycle) and D assigns to every edge of G one of its two directions, so that every vertex is the tail of at most 2 edges. Call such a D valid for G. The start position has no edges.

Two deterministic players, Adversary and Algorithm, both see the whole position and history. The game has exactly n − 1 rounds. In each round:

1. Adversary names a pair {u, v} of vertices in different connected components of G. Every such pair is legal.
2. Algorithm chooses any assignment D′ valid for G + uv. It pays the number of edges e of G (old edges only) with D′(e) ≠ D(e). The new edge uv costs nothing in either direction. Algorithm may change edges anywhere in the forest and more than necessary. The position becomes (G + uv, D′).

Adversary maximises and Algorithm minimises the total payment over the n − 1 rounds. Let T(n) be the value of this finite perfect-information game.

Call a vertex saturated if it is the tail of exactly 2 edges. Let B(n) = max(0, ⌊n/4⌋ − 1).

HEADLINE: find the exact function T(n) for all n ≥ 1, as a closed formula or an exact finite recursion. The first decisive step is yes/no: is T(n) = B(n) for every n? If not, give the least n with T(n) > B(n) and the true formula, including the exact rational value of lim T(n)/n.

UNVERIFIED beliefs:
- T(n) = 0 for n ≤ 7 and T(8) = 1.
- T(n) ≥ B(n). Adversary splits V into blocks of 4. In each block it joins two pairs and then joins the two tails, forcing a saturated vertex or a payment. It then merges components, always naming saturated vertices when both components have one. Invariant: a component built from k blocks has cost at least k − 1, and at least k if it has no saturated vertex.
- T(a + b) ≥ T(a) + T(b).
- T(n) ≤ Cn for an absolute constant C.
- Algorithm can remove one saturated vertex for 1 unit whenever it has an out-neighbour of out-degree 0, and each tree has exactly one more out-degree-0 vertex than saturated vertices.

GUESS (not a result): I would bet T(n) ≠ B(n), with first deviation at some n between 16 and 24, because Adversary should be able to build two trees in which a saturated vertex has only saturated out-neighbours, forcing a payment of 2 in one round.

## 3. WHY IT IS PROBABLY UNCHARTED

The literature on low-out-degree orientations is amortised and asymptotic: it proves O(1) flips per insertion for some algorithm and does not ask for the exact game value against an adaptive adversary. An exact value for forests with bound 2 would be a combinatorial-game result with no algorithmic payoff, so nobody had reason to compute it. Doubts: the answer may be the dull B(n), and a tight potential argument might be folklore. If the first deviation lies beyond n ≈ 14, exhaustive computation cannot see it.

## 4. HOW TO CHECK AN ANSWER

For fixed n, enumerate positions (G, D) up to vertex relabelling (canonical form of the directed forest). Run memoised minimax from the empty position: Adversary's moves are all cross-component pairs up to symmetry, Algorithm's replies are all valid assignments of G + uv, scored by Hamming distance on old edges. Compare T(n) with B(n) for n ≤ 12 or as far as the machine reaches. A general formula needs two proofs: an Adversary strategy with an invariant forcing the lower bound against every reply, non-lazy ones included, and an Algorithm strategy with a potential function proving the matching upper bound for every n, plus agreement with the computed table.
