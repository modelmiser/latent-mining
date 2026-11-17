## KNOWN

The e = 0 game is Bennett's reversible pebble game: p pebbles reach node 2^p - 1 exactly, and time-space trade-offs for it are well studied (Bennett; Li and Vitanyi; Knill gave an exact move-count recurrence; Kralovic on trade-offs). With unlimited erasure the game is ordinary pebbling of a path: two pebbles reach any n in 2n - 1 moves. Li, Tromp and Vitanyi studied reversible simulation with a bounded number of irreversible erasures, so the reach part of the seed is, I believe, at least asymptotically known; I am unsure whether exact values for all (p, e) are written down. The related "spooky pebble game" (Gidney; Kornerup, Sadun, Soloveichik) prices forgetting differently. Exact minimum move counts for all (n, p, e) are probably not tabulated, but the seed has three parameters and three questions, so it is broad rather than sharp.

## SUCCESSOR QUESTION

Tired pebbles. Nodes 1, 2, 3, ... lie on a line; node 0 is a virtual node that always counts as pebbled. You own p pebbles, all initially in your hand and all "fresh". A node holds at most one pebble. A move is one of:

(P) Place: put a pebble from your hand (fresh or tired, your choice) on an empty node i >= 1 whose predecessor i-1 is pebbled.
(R) Reversible removal: take the pebble on node i back into your hand, allowed when node i-1 is pebbled. The pebble keeps its status (fresh stays fresh, tired stays tired).
(F) Forgetful removal: take a FRESH pebble on node i back into your hand while node i-1 is empty. The pebble becomes "tired", permanently. A tired pebble can never again be removed forgetfully: it leaves the board only by move (R).

So each pebble can pay for forgetting exactly once in its life, and the price is paid by that pebble, not from a shared purse. There is no limit on the number of moves.

Let F(p) be the largest i such that some finite sequence of moves from the empty board puts a pebble on node i. (F(p) is finite, since at most p forgetful removals ever occur. By hand, F(1) = 1 and F(2) = 4.)

Also let G(p) be the same quantity in the shared-purse game: pebbles carry no status, and move (F) is allowed on any pebble whose predecessor is empty, but at most p times in total.

Questions. (1) Determine F(p) exactly for every p >= 1, as a closed formula or a recurrence with proof. (2) Is F(p) = G(p) for all p? If not, find the least p with F(p) < G(p) and determine the growth of G(p) - F(p).

## WHY IT IS PROBABLY UNCHARTED

Known work treats erasure as a global count or as ghosts to be re-pebbled; I know of no model where the right to forget is a one-shot property of an individual pebble, so that which pebble sits where matters and pebbles stop being interchangeable. That breaks the usual recursive "halve the interval" arguments. Doubts: G(p) may follow from Li-Tromp-Vitanyi-style bounds, and an exchange argument might show F = G, which would make part (2) a short lemma. My hand values F(1), F(2) are unverified beyond a quick check.

## HOW TO CHECK AN ANSWER

Breadth-first search over states (board contents with each pebble's fresh/tired status, plus the number of tired pebbles in hand); the board never needs more than p occupied nodes, and reach is bounded, so p up to 5 or 6 is a small finite search. Compute F(p) and G(p), compare with the claimed formula, and compare the two sequences. A proof needs a matching strategy and an upper-bound invariant; a single p where search disagrees with the formula refutes it.
