## 1. KNOWN

The question as given is degenerate. Let B be the bound on balls present. The adversary offers the same pair of bins every time: two bins then hold all the balls, so some bin reaches ceil(B/2), for every n >= 2, with no deletions needed. Conversely "take the emptier bin" never exceeds ceil(B/2): a bin reaches load k only when the other offered bin holds at least k-1, so 2k-1 <= B. So the answer is ceil(B/2), greedy is optimal, n is irrelevant, and moving one old ball per new one changes nothing (the pigeonhole on two bins still applies).

The interesting versions forbid repeating pairs. Viewed as online edge orientation (bin = vertex, ball = edge, load = in-degree), insert-only adversaries on forests force about log2 n by a merging argument. For deletions, online load balancing of temporary tasks with restricted assignment has square-root-of-n competitive bounds (authors: unsure).

## 2. SUCCESSOR QUESTION

There are n bins, labelled 1..n, initially empty. A ball, once placed, is a record ({u,v}, w): the unordered pair of distinct bins that was offered, and the bin w in {u,v} that received it. The state is the set of balls currently present. The offer graph G of a state has the bins as vertices and one edge {u,v} for each present ball. The load of a bin is the number of present balls it received.

Two players, Adversary and Rule, alternate with perfect information: both see the whole state (every present ball, its pair and its bin) at all times. Repeatedly, Adversary makes one of two moves:

- Delete: remove any one present ball from the state (its edge leaves G, and the load of its bin drops by one). Rule does nothing.
- Offer: name two distinct bins u, v. This is legal only if u and v lie in different connected components of the current G (so G is always a forest, and at most n-1 balls are ever present). Rule must then answer immediately by choosing w in {u,v}; the ball ({u,v}, w) is added to the state. Rule can never move or remove a ball.

Play may continue for any finite number of moves; there is no bound on the total number of balls offered over time. Adversary achieves L if at some moment some bin has load L. Define f(n) as the largest L such that Adversary has a strategy that achieves L against every Rule. (The game is a finite-state reachability game, so f(n) is well defined and does not depend on whether Rule is fixed in advance or adaptive.)

Determine f(n) for every n >= 2: give the exact values of f(n) for n = 2,...,8, and a closed formula (or exact characterisation) of the least n for which f(n) = k, for every k, together with an optimal Rule.

## 3. WHY IT IS PROBABLY UNCHARTED

The forest condition makes the offline optimum always 1, so the question isolates the pure cost of being online with deletions. Without deletions the answer is logarithmic; deletions let the adversary recycle bins, and I expect polynomial growth, but I do not know the exact threshold sequence. Doubts: the dynamic-orientation literature (bounded arboricity, with and without flips) and temporary-task load balancing may contain matching asymptotic bounds, possibly Theta(sqrt n). I believe exact values, and the exact constant, are not recorded, but I am not sure.

## 4. HOW TO CHECK AN ANSWER

For fixed n and target L, states are oriented labelled forests on n vertices with all in-degrees below L; compute the Adversary's winning set by backward induction (attractor computation) over this finite graph, up to relabelling of bins. f(n) is the largest L for which the empty state is winning. This is feasible for n up to about 8. Sanity values checkable by hand: f(2) = 1, f(3) = 1, f(4) = 2. A claimed formula must match all computed values; a proof needs an explicit adversary strategy and an explicit Rule with a potential-function invariant.
