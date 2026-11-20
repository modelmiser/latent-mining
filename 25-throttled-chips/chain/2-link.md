## KNOWN

The question is well-posed: a finite perfect-information game, so F(h) is defined. I re-derived F(1) = 1, F(2) = 2, F(3) = 4 by hand and agree. Chip-firing on the path is classical (abelian sandpile; firing raises the sum of squared positions by exactly 2, the standard potential), but that potential does not bound the game because Adder may place chips anywhere. I know of no theorem that gives F(h); unsure whether "one firing per round against one chip per round" has been studied as a game. The natural Adder plan is to lay a block of 1s, then overload it so each firing makes two new ready vertices while Firer may fire only one; I expect F(h) finite and polynomial, but this is unproved. No flaw found. Part (c) (forced firing) is a side variant.

## SUCCESSOR QUESTION

The board is the two-way infinite path: vertices are the integers, and i is adjacent to i-1 and i+1. Each vertex holds a non-negative number of chips; at the start every vertex holds 0. A vertex is "ready" when it holds at least 2 chips; "firing" a ready vertex i removes 2 chips from i and adds 1 chip to i-1 and 1 chip to i+1.

Fix an integer h >= 1 (the target height). An "Adder sequence" is a finite list a_1, ..., a_m of integers. Adder is OBLIVIOUS: it must commit to the whole list before play starts and cannot change it. Firer is told the whole list before play starts and sees the whole position at all times. Then m rounds are played; round t = 1, ..., m consists of:
 (1) one new chip is put on vertex a_t;
 (2) Firer does exactly one of: pass (nothing changes), or fire exactly one ready vertex of its choice. Firer may pass even if ready vertices exist; if no vertex is ready, Firer must pass.

The sequence "wins" if, for EVERY way Firer can play, there is a moment, immediately after some step (1) or some step (2), at which some vertex holds at least h chips. (Firer wins against the sequence if it has one line of play in which no vertex ever holds h chips through the end of round m.)

Define G(h) as the least m for which a winning Adder sequence of length m exists, or infinity if none exists. For comparison, define F(h) as the least m for which an ADAPTIVE Adder (who chooses a_t at the start of round t, having seen everything so far) can force some vertex to hold at least h chips within m rounds, under the same rules. Clearly G(h) >= F(h).

Sanity values (worked by hand, unverified; reproduce them): G(1) = 1, G(2) = 2, F(3) = 4, and G(3) <= 9 via the sequence -2, -1, 0, 1, 2, 0, 0, 0, 0 (after the eighth chip, either Firer has already lost or the position is 1,2,2,2,1 on vertices -2..2, where every firing creates a 3 and passing lets the ninth chip create one). Also G(3) >= 5, since the adaptive win in 4 rounds needs a fourth move that depends on Firer's reply.

Question:
 (a) Compute G(3) exactly, and G(4), and G(h) as far beyond as exhaustive search allows; for each, exhibit one shortest winning sequence.
 (b) Is G(h) finite for every h? If so, give an exact formula, or failing that the exponent c with G(h) = Theta(h^c), or prove that the growth is faster than every polynomial.
 (c) Compare with the adaptive game: is G(h) > F(h) for every h >= 3, and is G(h)/F(h) bounded as h grows? State and prove the answer.

## WHY IT IS PROBABLY UNCHARTED

Chip-firing games in the literature are mostly solitaire, or have the adversary choose firings against a fixed configuration; a committed chip sequence that must beat every firing schedule (a "universal overloading word") is a conformant-planning twist I have not seen. Adaptive tricks (answer whichever side Firer fired) stop applying, so symmetric traps like 1,2,2,2,1 become the only tool. Doubts: G(4) may be beyond a few minutes of search; G(h) might be infinite for some h, which would be a short answer; my G(3) bound is hand-checked only.

## HOW TO CHECK AN ANSWER

For a claimed sequence, a depth-first search over Firer's replies (pass or one ready vertex per round) confirms it wins; this is quick. For minimality, enumerate all sequences of length m-1 up to translation and reflection, restricted to a window justified by the fact that a chip far from all others cannot interact within the remaining rounds; equivalently run a breadth-first search over sets of Firer-reachable safe positions. F(h) is checked by ordinary game-tree search. A formula must reproduce every computed value; growth claims need matching upper (explicit sequence family) and lower (Firer strategy) proofs.
