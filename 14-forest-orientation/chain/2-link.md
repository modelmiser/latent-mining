## KNOWN

Graphs orientable with out-degree at most one are exactly pseudoforests, so the legality rule is right and the game is finite and well posed. In the forest game an orientation is a choice of root per tree; joining trees at u and v costs at least min(depth(u), depth(v)), and Algorithm may pay more to place the root well. Small-into-large style arguments give F(n) = O(n log n), and adversaries building binomial-like trees give Ω(n log n); this matches the general theory of low out-degree orientations of sparse dynamic graphs (Brodal and Fagerberg; later work by Kowalik and others), which is asymptotic, not exact. I checked by hand that the conjectured formula gives 0, 0, 1 for n = 2, 3, 4, which agrees with the game. Whether it holds beyond that, and the value of P(n) − F(n), I do not know. No flaw found; the question is tight but lives entirely in the "one root per tree" picture.

## SUCCESSOR QUESTION

Fix an integer n ≥ 1 and the vertex set {1, …, n}. A position is a forest G on these vertices (a simple undirected graph with no cycle) together with a direction for every edge such that each vertex has at most TWO edges pointing out of it; call such a direction assignment valid. The game starts with no edges. Two players, Adversary and Algorithm, both deterministic, both seeing the whole position and the whole history, play rounds:

1. Adversary either stops the game, or names a pair {u, v} of vertices lying in different connected components of G. If G is connected the game stops.
2. Algorithm replaces the current direction assignment by any valid direction assignment of G + uv. It pays 1 for each edge of G (the old edges only) whose direction now differs from its direction before this round. The new edge is free, in either direction. Algorithm may redirect edges anywhere in the forest, not only near u and v, and may redirect more edges than the new edge forces.

The game lasts at most n − 1 rounds. Let T(n) be its value: the least total payment Algorithm can guarantee, equal to the most Adversary can force.

Determine T(n) exactly: give T(n) for all n ≤ 12 and a closed formula or exact recursive characterisation valid for all n. In particular:
(i) for each k ≥ 1 find m(k), the least n with T(n) ≥ k, and decide whether m(1) = 8;
(ii) decide whether T(n) = O(n), and if so determine the limit of T(n)/n (prove that it exists and give it as an exact rational, or prove it does not exist);
(iii) decide whether Algorithm loses anything by being lazy, i.e. whether the value changes when Algorithm is forbidden to reverse any edge in a round where some valid direction assignment of G + uv agrees with the current one on all old edges.

Orientation: with bound one instead of two, every tree has a single root and joining two trees at non-root vertices forces a re-rooting, giving a total cost that grows faster than linearly. With bound two a tree on m vertices has m − 1 units of out-degree spread over capacity 2m, so a payment is forced only when Adversary joins two vertices that both already have two out-edges; Algorithm then must reverse a directed path leading from one of them to a vertex with at most one out-edge. Algorithm controls where the saturated vertices sit and how far they are from spare capacity; Adversary controls which trees meet and where.

## WHY IT IS PROBABLY UNCHARTED

The literature on bounded out-degree orientations is amortised and asymptotic, usually with out-degree a constant multiple of arboricity; I know of no exact game values, and none for the tightest slack (bound two, arboricity one). The root picture that makes the bound-one case tractable disappears. Doubts: linearity may follow from a known potential argument (I am unsure it reaches bound two), in which case only the constant and exact sequence are new; the sequence may also turn out dull (for example one payment per eight vertices). My m(1) = 8 is a hand argument, unverified.

## HOW TO CHECK AN ANSWER

Solve the game by memoised minimax over positions up to isomorphism (oriented forests with out-degree at most two, plus rounds played is implied by edge count). For each Adversary pair, enumerate every valid orientation of G + uv and its Hamming cost on old edges. This should be feasible for n up to about 10, perhaps 12, and yields T(n), m(k) for small k, and the lazy variant by restricting moves. A claimed formula must match these values; a claimed limit needs a matching Algorithm potential argument and an explicit Adversary strategy.
