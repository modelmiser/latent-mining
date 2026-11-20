## KNOWN

Chip-firing is abelian: for a fixed multiset of added chips, whether the process stops, the final position and the number of times each vertex fires do not depend on firing order, and also not on whether chips are added before or between firings (a legal firing stays legal when chips are added). So the seed's "many firings" aim is not a game: whenever the process ends, the total depends only on where the Adder put chips, a one-player optimisation. Likewise, if the Firer may fully stabilise between additions on a path, heights stay tiny (I believe at most 2). On a cycle, fewer than n chips always stops and more than n never does (standard; Björner–Lovász–Shor for the general theory). The seed is therefore under-specified: only a transient aim (peak height) with a rate limit on firing gives a real game. No timing rule, budget or information rule was stated.

## SUCCESSOR QUESTION

The board is the two-way infinite path: vertices are the integers, and i is adjacent to i-1 and i+1. Each vertex holds a non-negative number of chips; at the start every vertex holds 0. A vertex is "ready" when it holds at least 2 chips; "firing" a ready vertex i removes 2 chips from i and adds 1 chip to i-1 and 1 chip to i+1.

Fix integers h >= 1 (the target height) and m >= 1 (the budget). Two players, Adder and Firer, play m rounds with perfect information: both see the whole position and the whole history at all times, and both know h and m. Round t = 1, ..., m consists of:
 (1) Adder puts exactly one new chip on a vertex of its choice (any integer; there is no passing).
 (2) Firer then does exactly one of: pass (nothing changes), or fire exactly one ready vertex of its choice. Firer may pass even if ready vertices exist; if no vertex is ready, Firer must pass.

Adder wins if, immediately after some step (1) or some step (2), some vertex holds at least h chips. If all m rounds finish without this, Firer wins. No randomness; the game is finite, so exactly one player has a winning strategy.

Define F(h) as the least m for which Adder has a winning strategy, or infinity if there is none for any m.

Sanity values to reproduce (worked by hand, verify them): F(1) = 1, F(2) = 2, F(3) = 4. (For h = 3: Adder plays 0, then 1, then 0; the position is 2 chips on 0 and 1 chip on 1; if Firer passes Adder adds to 0, if Firer fires 0 then vertex 1 holds 2 and Adder adds to 1.)

Question:
 (a) Compute F(h) exactly for h = 4, 5, 6 and as far beyond as exhaustive search allows.
 (b) Is F(h) finite for every h? If so, give an exact formula for F(h), or failing that determine its order of growth: find the exponent c with F(h) = Theta(h^c), with proof, and describe an optimal strategy for each player.
 (c) Let F*(h) be the same quantity when Firer is not allowed to pass while any vertex is ready (it must fire exactly one ready vertex whenever one exists). For which h is F*(h) different from F(h), and in which direction?

## WHY IT IS PROBABLY UNCHARTED

Chip-firing games in the literature I know are solitaire (abelian sandpile), or two-player in the Baker–Norine "dollar game" sense, or Spencer-style balancing/pusher games; none that I recall pits a one-chip-per-turn adder against a one-firing-per-turn firer with a peak-height aim. The rate limit destroys the abelian shortcut, so no invariant I can name settles it. Doubts: it may resemble Spencer's chip "tenure"/liar games or load-balancing adversary bounds closely enough that the growth exponent follows from a known potential argument; and F(h) might turn out to be a dull simple polynomial.

## HOW TO CHECK AN ANSWER

Minimax with memoisation over positions modulo translation and reflection, state = (position, rounds left). With m rounds left, Adder placements farther than m from every chip are interchangeable, so the search is finite and exact. Reproduce F(1..3) = 1, 2, 4, then compare claimed F(4), F(5), F(6) and F*; a claimed formula must match every computed value. A growth claim needs an explicit Adder strategy (upper bound) and a Firer potential-function argument (lower bound), each testable against the computed optimal play.
