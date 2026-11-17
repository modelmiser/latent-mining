## 1. KNOWN

The question looks well-posed. Because the cop is deterministic and the robber adversarial, it is a finite reachability game on belief states (cop vertex, set of vertices the robber may occupy, glimpses left), so g(h) and t(h) exist and are computable in principle. The k = 0 case is the zero-visibility cops-and-robber game with one cop, which has a literature on trees (I am unsure of the exact characterisation of one-cop-win trees). By hand I believe g(0) = g(1) = g(2) = 0: on T_2 the cop sweeps one child's leaves, returning to the child between leaves, then the other side; a robber stepping onto the child is caught on the return. For h = 3 recontamination through the root seems to force glimpses. An upper bound linear in h is easy (one glimpse per level, roughly). Periodic-visibility variants ("witness" versions) exist; a budget version on trees I cannot place. The clause "robber moves onto cop" never matters.

## 2. SUCCESSOR QUESTION

Pointing versus telling: direction glimpses on complete binary trees.

Let T_h be the complete binary tree of height h >= 0: a root, every non-leaf vertex has exactly two children, all leaves at distance h from the root (2^(h+1) - 1 vertices). Let k >= 0 be an integer. Two games, D(h, k) and E(h, k), are played by a cop and a robber on T_h; both players know T_h, h, k and which game is played. The games differ only in what a glimpse reveals.

Setup. The cop chooses a start vertex c. The robber, seeing c, chooses a start vertex r different from c. The cop is not told r.

Rounds. Each round has three steps, in this order:
1. Glimpse (optional). If the cop has used fewer than k glimpses so far, he may spend one. In E(h, k) (exact glimpse) he is told the robber's current vertex. In D(h, k) (direction glimpse) he is told only the unique neighbour of his own current vertex that lies on the path from his vertex to the robber's vertex; nothing else (in particular not the distance).
2. Cop move. The cop moves to an adjacent vertex or stays. If he is now on the robber's vertex, the robber is captured and the game ends.
3. Robber move. The robber moves to an adjacent vertex or stays. If she is now on the cop's vertex, she is captured and the game ends.

Information. The robber always knows the cop's vertex, the number of glimpses spent, every glimpse answer, and the cop's whole strategy. The cop knows only his own moves, the answers to his glimpses, and that capture has not yet happened. His strategy is deterministic: a function of that history.

Winning. The cop wins if he has a strategy and a number N such that, against every robber start and every sequence of robber moves, capture occurs within N rounds.

Define d(h) as the least k such that the cop wins D(h, k), and e(h) as the least k such that the cop wins E(h, k). Both are finite (glimpsing every round and always stepping to the indicated neighbour, or toward the revealed vertex, never lets the robber pass the cop, and her region shrinks), and d(h) >= e(h), since an exact glimpse determines the direction.

Questions.
(i) Determine d(h) and e(h) for h = 0, 1, 2, 3, 4.
(ii) Is d(h) = e(h) for every h? If not, find the least h with d(h) > e(h), and determine whether d(h) - e(h) is bounded as h grows.
(iii) Give a closed formula for d(h) for all h, with proof.

## 3. WHY IT IS PROBABLY UNCHARTED

Zero-visibility and periodic-visibility pursuit are studied, and distance-query "localization" games are studied, but there the searcher does not chase under a query budget. A budgeted one-bit-ish directional oracle for a moving cop, compared head to head with exact sightings, I have not seen. Doubts: on trees a direction may carry nearly all the useful information (the cop mostly needs to know which side is contaminated), so d = e could hold for a simple reason; and the budget game itself may be folklore in the graph-searching literature under another name.

## 4. HOW TO CHECK AN ANSWER

Solve the belief-state game exactly: state = (cop vertex, set S of possible robber vertices, glimpses left); cop chooses glimpse/move, the adversary chooses the glimpse answer (a block of the partition of S by answer); S then loses the cop's vertex and expands to closed neighbourhoods minus the cop's vertex. Cop wins iff he can force S empty (least fixed point). Use tree symmetry and memoisation; h <= 4 (31 vertices) should be feasible, though reachable belief sets may need canonicalisation. A formula must match these values; proofs need a robber evasion invariant for the lower bound.
