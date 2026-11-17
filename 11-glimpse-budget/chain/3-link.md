## 1. KNOWN

With k = 0 both games are the one-cop zero-visibility cops-and-robber game. I believe (unsure of the exact citation; Dereniowski, Dyer, Tifenbach and Yang studied zero-visibility cop numbers of trees) that one blind cop wins on a tree exactly when it is a caterpillar. T_0, T_1, T_2 are caterpillars and T_3 is not, so d = e = 0 for h <= 2 and both are >= 1 for h >= 3. Because the robber knows the deterministic strategy, each game is a finite perfect-information game on (cop vertex, set of possible robber vertices, glimpses left), so every instance is decidable. Flaws in the question as given: h = 0 leaves the robber no legal start, and it asks three things at once. A hand sketch suggests an exact glimpse of a robber two steps ahead buys two levels where a direction glimpse buys one, so the answer to (ii) is probably "not equal", which makes the threshold the sharp target.

## 2. FINAL QUESTION

Let T_h be the complete binary tree of height h >= 0: a root, every non-leaf vertex has exactly two children, all leaves at distance h from the root. Let k >= 0 be an integer. Two games, D(h, k) and E(h, k), are played on T_h by a cop and a robber; both know h, k and which game is played.

Setup. The cop chooses a start vertex c. The robber, seeing c, chooses a start vertex r != c. The cop is not told r. (If h = 0 the robber has no legal start and the cop wins.)

Rounds. Each round has three steps, in this order.
1. Glimpse (optional). If the cop has spent fewer than k glimpses, he may spend one. In E (exact) he is told the robber's current vertex. In D (direction) he is told only the neighbour of his own vertex that lies on the path from his vertex to the robber's; nothing else.
2. Cop move. The cop stays or moves to an adjacent vertex. If he is now on the robber's vertex, he has captured her and the game ends.
3. Robber move. The robber stays or moves to an adjacent vertex. If she is now on the cop's vertex, she is captured and the game ends.

Information. The robber knows the cop's vertex at all times, every glimpse answer, and the cop's whole strategy in advance. The cop knows only his own moves, his glimpse answers, and that capture has not yet occurred. The cop's strategy is deterministic: his start vertex, and in each round whether to glimpse and where to move, are functions of that history (the move may depend on the glimpse just received).

Winning. The cop wins if some strategy and some number N guarantee capture within N rounds against every robber start and every sequence of robber moves.

Let d(h) be the least k for which the cop wins D(h, k), and e(h) the least k for which he wins E(h, k). Both are finite, and e(h) <= d(h), since an exact answer determines the direction answer.

HEADLINE. Find the exact value of h* = the least h with d(h) > e(h); the answer is "none" if d(h) = e(h) for every h. The answer must come with the values d(h) and e(h) for all h <= h* (or, if "none", a proof for all h).

General form. Closed formulas for d(h) and e(h) for all h, with proof; in particular whether d(h) - e(h) is bounded.

UNVERIFIED beliefs.
- d(h) = e(h) = 0 for h = 0, 1, 2 (T_2 is a caterpillar; a blind cop sweeps it by stepping to each leaf and straight back).
- d(3) >= 1 and e(3) >= 1.
- d(h) <= h - 1 for h >= 1: start at the root, glimpse and step down each round until at a parent of leaves, then try both leaves.
- e(h) is roughly h/2 for large h.

Guess (a bet, not a result): h* exists and is 4 or 5, with d(h) - e(h) unbounded, because an exact glimpse of a robber two steps below the cop fixes two levels of descent and a direction glimpse fixes one.

## 3. WHY IT IS PROBABLY UNCHARTED

Limited-visibility pursuit is studied through the number of cops, the visibility radius, or periodic sightings ("witness" variants), and the localization game studies distance probes. I do not know of work that counts a budget of sightings for a single cop, or that compares a one-neighbour direction answer with a full position answer. Doubts: the hunter-and-rabbit and graph-searching literature is large, and a budgeted-probe result on trees could exist under another name. The blind caterpillar fact may also be misremembered, which would change the small values but not the question.

## 4. HOW TO CHECK AN ANSWER

For fixed h and k, build the game on states (cop vertex, set S of possible robber vertices, glimpses left). A cop turn picks glimpse or not, then a move; a glimpse branches adversarially over the answers consistent with S (at most 3 in D, |S| in E). Then S loses the cop's vertex and expands to closed neighbourhoods minus the cop's vertex. The cop wins iff some start forces S empty, computed as a least fixed point (attractor) over reachable states. Take the least winning k. T_3 has 15 vertices and is immediate; T_4 and T_5 need reachable-state enumeration with root-fixing symmetry. A general claim needs an explicit cop strategy with its glimpse count, and a robber strategy defeating every deterministic cop with one fewer, for both games.
