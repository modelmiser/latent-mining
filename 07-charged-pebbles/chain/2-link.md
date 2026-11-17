## KNOWN

With no forgetting at all (moves P and R only), this is Bennett's reversible pebble game on a line: p pebbles reach exactly node 2^p - 1. With unlimited forgetting, two pebbles walk forever. Trading erasures against pebbles in reversible simulation was studied by Li, Tromp and Vitanyi (shared budget of erasures, close to the G(p) variant); I am unsure of their exact statements. The per-pebble "tired" rule I do not recognise.

Flaw found: the stated hand value F(2) = 4 looks wrong under the rules as written. Place 1, place 2, remove 1 reversibly, place 3, forget 2, place 4, forget 3, place 5 reaches node 5. The value 4 is what you get if the board must end with a single pebble, which the question never required. So the question silently mixes two goals; the successor separates them.

## SUCCESSOR QUESTION

Tired pebbles, clean delivery. Nodes 1, 2, 3, ... lie on a line; node 0 is a virtual node that always counts as pebbled and never holds one of your pebbles. You own p pebbles, all initially in your hand and all "fresh". Each pebble is permanently either fresh or tired; pebbles of the same status are interchangeable. A node holds at most one pebble. A move is exactly one of:

(P) Place: put a pebble from your hand (fresh or tired, your choice) on an empty node i >= 1 whose predecessor i-1 is pebbled.
(R) Reversible removal: take the pebble on node i >= 1 back into your hand, legal exactly when node i-1 is pebbled. The pebble keeps its status.
(F) Forgetful removal: take a FRESH pebble on node i >= 2 back into your hand, legal exactly when node i-1 is empty. That pebble becomes tired, permanently. A tired pebble whose predecessor is empty cannot be removed at all.

So each pebble can pay for forgetting once in its life. There is no limit on the number of moves.

A configuration is CLEAN AT n if node n holds a pebble, every other node is empty, and the other p-1 pebbles are in your hand (their statuses, and the status of the pebble on n, do not matter).

Define
  C(p) = the largest n such that some finite sequence of legal moves from the empty board reaches a configuration clean at n;
  F(p) = the largest n such that some finite sequence of legal moves from the empty board ever puts a pebble on node n (stranded tired pebbles may be left behind).

Both are finite because at most p forgetful removals ever occur. Unverified hand values: C(1) = F(1) = 1, C(2) = 4, F(2) = 5 (for p = 2: place on 1, place on 2, remove 1 reversibly, place on 3, forget 2, place on 4, forget 3 -- now the board is clean at 4 with both pebbles tired -- then place on 5, which strands the tired pebble on 4).

Questions. (1) Determine C(p) exactly for every p >= 1, as a closed formula or a recurrence, with proof. (2) Determine the stranding gap F(p) - C(p) for every p: is it bounded, linear in p, or exponential in p? (3) In an optimal clean delivery (one reaching a configuration clean at C(p)), is it always possible to have used all p forgetful removals, and is it ever necessary to use fewer than p?

## WHY IT IS PROBABLY UNCHARTED

Limited-erasure pebbling in the literature uses a shared erasure budget and usually asks about space-time tradeoffs, not a per-pebble one-shot fatigue with a clean final board. Here a tired pebble stranded far out must be rescued by rebuilding the chain behind it with the remaining pebbles, so the greedy "forget as late as possible" intuition may fail. Doubts: the answer might collapse to a simple recurrence such as a sum of reversible reaches 2^k - 1 over shrinking fresh supplies, and someone may have analysed an equivalent "write-once ancilla" model that I do not know.

## HOW TO CHECK AN ANSWER

Breadth-first search over states (set of occupied nodes with fresh/tired status, plus the fresh and tired counts in hand) from the empty board; the reachable set is finite, so the search terminates without an a priori board bound. Read off C(p) and F(p) for p = 1..5 or 6 and compare with the claimed formula; record the number of forgetful removals along shortest clean paths for question (3). A proof needs a matching strategy and an upper-bound invariant, likely by induction on the number of fresh pebbles.
