## KNOWN

Reversible pebbling of a line with p pebbles reaches node 2^p - 1 (Bennett's strategy; tightness usually credited to Li and Vitanyi), and I believe the "only node n pebbled" variant reaches 2^(p-1). Reversible simulation with a limited number of irreversible erasures has been studied (Li, Tromp, Vitanyi; exact statement: unsure), but with a global erasure budget, not a per-pebble one.

Flaws in the question as received: it asks three things, not one. Part (2) looks easy at the coarse level: from a configuration clean at C(p), the other p-1 pebbles play reversibly above it, so F(p) >= C(p) + 2^(p-1) - 1, an exponential gap. Part (3) is vague. For part (1) a simple strategy gives C(p) >= (p+2) * 2^(p-2) (so C(3) >= 10); the only real content is the matching upper bound. All of this is unverified.

## FINAL QUESTION

Charged pebbles, clean delivery. Parameters: integers p >= 2 (pebbles) and k >= 0 (charge).

Board: nodes 1, 2, 3, ... on a line. Node 0 is virtual: it always counts as pebbled and never holds one of your pebbles. Each node holds at most one pebble. You own p pebbles, all initially in your hand. Each pebble carries an integer charge, initially k; pebbles with equal charge are interchangeable.

A move is exactly one of:
(P) Place: put any pebble from your hand on an empty node i >= 1 whose predecessor i-1 is pebbled. Charge unchanged.
(R) Reversible removal: take the pebble on node i >= 1 back into your hand; legal exactly when node i-1 is pebbled. Charge unchanged.
(F) Forgetful removal: take the pebble on node i >= 2 back into your hand; legal exactly when node i-1 is empty AND that pebble's charge is >= 1. Its charge drops by 1. A pebble with charge 0 whose predecessor is empty cannot be removed.

There is no limit on the number of moves. A configuration is CLEAN AT n if node n holds a pebble and every other node is empty (the other p-1 pebbles are in hand; charges are irrelevant). Define

  C(p,k) = the largest n such that some finite sequence of legal moves from the empty board reaches a configuration clean at n.

(Finite: at most pk F-moves ever occur, and between F-moves play is reversible pebbling with p pebbles above a bounded frontier.)

HEADLINE (yes/no): Is C(p,k) = (pk + 2) * 2^(p-2) for all p >= 2 and all k >= 0? If no, give the smallest counterexample (p,k) (ordered by p, then k) and the exact function C(p,k).

UNVERIFIED beliefs:
- k = 0 is pure reversible pebbling, where the clean reach is 2^(p-1); the formula agrees.
- Lower bound C(p,k) >= (pk+2) * 2^(p-2): reach clean at 2^(p-1) with no F-move; then repeat pk times: from clean at m, use the p-1 hand pebbles reversibly to reach {m, m + 2^(p-2)}, then forget the pebble on m. Rotate which pebble ends on top so the pebble left at the new base still has charge.
- Hand values: C(2,1) = 4, C(2,k) >= 2k+2, C(3,1) >= 10.
- The Bennett-style alternative (go to the full reversible frontier, forget p-1 pebbles) gains less per F-move.

Guess (marked as a guess): I would bet YES, because every strategy I tried gains at most 2^(p-2) nodes per F-move; the risk is an end-game trick that wastes no charge on the final pebbles.

## WHY IT IS PROBABLY UNCHARTED

Erasure-limited reversible pebbling is normally stated with one global erasure budget and asymptotic space-time tradeoffs. A per-pebble charge, an exact extremal value, and the "clean" end condition together are a combination I have not seen. Doubts: the per-pebble budget may reduce to the global one (the lower-bound strategy uses all pk charges, so the answer may coincide with a global budget of pk), in which case an existing bound could settle it. I may also simply not know the right paper. The conjectured formula is suspiciously simple, which cuts both ways.

## HOW TO CHECK AN ANSWER

For fixed (p,k), pick N larger than the claimed value (say 4 * (pk+2) * 2^(p-2)). Breadth-first search over states: a state is the map from occupied nodes in 1..N to charges, plus the multiset of hand charges. Apply moves P, R, F, forbidding placement above N. If no reachable state pebbles node N, the truncation is exact; otherwise double N. C(p,k) is the largest n with a reachable state clean at n. Run p = 2, 3 for k up to about 6, and p = 4, 5 for k = 0, 1, 2.

A general YES needs: the lower-bound strategy checked for legality (charge rotation), and an upper-bound proof that covers arbitrary interleavings, including re-pebbling from node 0. A NO needs one explicit move sequence.
