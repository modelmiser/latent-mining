## 1. KNOWN

For m = 1 the one-cop graphs are the dismantlable graphs (Nowakowski–Winkler; Quilliot). The periodic-sighting variant is not new: I believe Clarke studied a "witness" version and Chalopin, Chepoi, Nisse and Vaxès a robber visible every k moves (attributions: fairly sure, not certain). The never-seen extreme, zero-visibility cops and robber, has been analysed on trees (authors: unsure). Easy: on a tree one cop wins for every m, since the robber can never pass him; on cycles of length at least 4 one cop already loses at m = 1. Flaws: four questions in one, and "worst-case time" is undefined without start rules.

## 2. SUCCESSOR QUESTION

Glimpse budget on complete binary trees.

Let T_h be the complete binary tree of height h >= 0: a root, every non-leaf vertex has exactly two children, all leaves at distance h from the root (2^(h+1) - 1 vertices). Let k >= 0 be an integer. The game G(h, k) is played by a cop and a robber on T_h; both know T_h, h and k.

Setup. The cop chooses a start vertex c. The robber, seeing c, chooses a start vertex r different from c. The cop is not told r.

Rounds. Each round has three steps, in this order:
1. Glimpse (optional). If the cop has used fewer than k glimpses so far, he may spend one; he is then told the robber's current vertex exactly.
2. Cop move. The cop moves to an adjacent vertex or stays. If he is now on the robber's vertex, the robber is captured and the game ends.
3. Robber move. The robber moves to an adjacent vertex or stays. If she is now on the cop's vertex, she is captured and the game ends.

Information. The robber always knows the cop's vertex, the number of glimpses spent, and the cop's whole strategy. The cop knows only his own moves, the answers to his glimpses, and that capture has not yet happened. His strategy is deterministic: a function of that history.

Winning. The cop wins G(h, k) if he has a strategy and a number N such that, against every robber start and every sequence of robber moves, capture occurs within N rounds.

Define g(h) as the least k such that the cop wins G(h, k). It is finite: glimpsing every round gives the full-information game, which one cop wins on any tree.

Questions.
(i) Determine g(h) for h = 0, 1, 2, 3, 4, 5.
(ii) Give a closed formula for g(h) for all h, with proof.
(iii) For each h <= 5, let t(h) be the least N for which some cop strategy wins G(h, g(h)) within N rounds. Determine t(h).

## 3. WHY IT IS PROBABLY UNCHARTED

Known variants fix the sighting schedule or remove sight entirely; here the cop chooses when to look, and the number of looks becomes a tree parameter I have not seen. Doubts: k = 0 is exactly the zero-visibility game, so small values may follow from that literature; a one-level-per-glimpse recursion might make g(h) just h minus a constant, true but dull (part (iii) less so); the localization game or other budgeted-query pursuit work may contain it under another name.

## 4. HOW TO CHECK AN ANSWER

Solve the belief-set game exactly. A state is (cop vertex, set S of possible robber vertices, glimpses left). A cop move deletes his new vertex from S, replaces S by its closed neighbourhood, then deletes his vertex again; a glimpse branches over every vertex of S and every branch must win. Backward induction over reachable states gives g(h) and t(h): immediate for h <= 3, needing symmetry reduction for h = 4, 5. A formula additionally needs an explicit robber evasion strategy at k = g(h) - 1.
