## 1. KNOWN

The question is well-posed but its d = 1 account is loose. Re-rooting a tree at a vertex u reverses only the path from u to the old root, so a join costs at most min(depth(u), depth(v)), not the size of the smaller tree. Re-rooting the smaller tree therefore gives at most (min size − 1) per join; I believe the maximum of the sum of min(a, b) over all merge orders is the sum of binary digit sums of 0..n−1, giving F(n,1) ≤ Σ_{k<n} s2(k) − (n−1). Joining end-rooted paths end to end matches this only against algorithms that keep an old root; the algorithm may re-root anywhere, so I am unsure the bound is tight. For d ≥ 2 flips are first forced near n = 2^(d+1) (unsure of exact threshold); amortised O(log n) is in the Brodal–Fagerberg line of work. The general setting resembles online matching with recourse.

## 2. SUCCESSOR QUESTION

Fix an integer n ≥ 1 and the vertex set {1, …, n}. A position is a simple undirected graph G on these vertices (no loops, no parallel edges) together with a direction for every edge such that each vertex has at most one edge pointing out of it; call such a direction assignment valid. The game starts with no edges. Two players, Adversary and Algorithm, both deterministic, both seeing the whole position and history, play rounds:

1. Adversary either stops the game, or names a pair {u, v} of distinct vertices that is not already an edge, such that G + uv has at least one valid direction assignment (equivalently: every connected component of G + uv contains at most one cycle). If no such pair exists the game stops.
2. Algorithm replaces the current direction assignment by any valid direction assignment of G + uv. It pays 1 for each edge of G (the old edges only) whose direction now differs from its direction before this round. The new edge is free. Algorithm may redirect edges anywhere in the graph, not only near u and v.

The game lasts at most n rounds. Let P(n) be the value of this finite game: the least total payment Algorithm can guarantee, equal to the most Adversary can force. Let F(n) be the value of the same game when Adversary is additionally required to keep G a forest (so at most n − 1 rounds).

Determine P(n) and F(n) exactly for every n: give the values for all n ≤ 12 and a closed formula (or exact recursive characterisation) valid for all n. In particular decide whether F(n) = Σ_{k=0}^{n−1} s2(k) − (n − 1), where s2(k) is the number of ones in the binary expansion of k, and determine the exact difference P(n) − F(n).

Note what the cycle rule changes. In a tree component Algorithm chooses one root (the unique vertex with no out-edge) and all edges point towards it. A component containing a cycle has no root, so when Adversary joins a tree to it at tree vertex u, the tree must be re-rooted at u at full cost depth(u), with no cheaper side to choose. Algorithm must therefore weigh cheap re-rootings against keeping trees shallow.

## 3. WHY IT IS PROBABLY UNCHARTED

Work on bounded out-degree orientation and on matching with recourse states asymptotic bounds, typically for forests or bounded arboricity with slack in the degree bound. Here the bound is exactly tight (out-degree 1 on graphs that just barely admit it) and the exact minimax value is asked, with Algorithm free to re-root anywhere. I know no source giving exact values. Doubts: P(n) may turn out to be F(n) plus a simple linear term such as n − 3, and F(n) may equal the digit-sum formula by a short potential argument, making the answer easy once seen.

## 4. HOW TO CHECK AN ANSWER

States are directed graphs with out-degree at most 1 and no 2-cycles, up to isomorphism (tens of thousands for n = 12). Compute the minimax value by memoised recursion: Adversary's moves are legal pairs, Algorithm's replies are all valid assignments of the new graph (a root per tree component, a direction per cycle), cost by Hamming distance on old edges. Compare the resulting tables for n ≤ 12 with the claimed formulas. A general claim needs an explicit Adversary strategy and a potential-function proof for Algorithm, each checked against the table.
