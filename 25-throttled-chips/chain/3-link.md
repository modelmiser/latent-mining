## KNOWN

The question as received is partly easy and one stated value is weak. Part (b) is easy: the constant list (all chips on vertex 0) wins in O(h) rounds, by the quadratic potential Q = sum of x^2 over chips (adds at 0 cost nothing, each firing adds exactly 2, Firer gets one firing per round, and a flat profile of n chips needs Q of order n^3/h^2). With the trivial G(h) >= h this gives G(h) = Theta(h), so "finite?" and "exponent?" are settled (c = 1), and G/F is bounded. The claimed bound G(3) <= 9 is far from tight: by hand, the constant list wins at length 7, and 0,1,0,1,0 appears to win at length 5, matching the stated lower bound 5. All of this is hand-worked and unverified. I know of no literature on this exact game; unsure.

## FINAL QUESTION

BOARD. Vertices are the integers; i is adjacent to i-1 and i+1. Each vertex holds a non-negative number of chips, initially 0 everywhere. A vertex is "ready" when it holds at least 2 chips. "Firing" a ready vertex i removes 2 chips from i and adds 1 chip to i-1 and 1 chip to i+1.

GAME. Fix integers h >= 1 and m >= 1. Adder chooses a list a_1, ..., a_m of integers and commits to it before play. Firer is then told the whole list, and sees the whole position at all times. Rounds t = 1, ..., m are played in order; round t is:
 (1) one chip is added to vertex a_t;
 (2) Firer does exactly one of: pass (nothing changes), or fire exactly one ready vertex of its choice. Passing is always legal; if no vertex is ready, Firer must pass.
The list WINS if, for every legal line of Firer play, some vertex holds at least h chips immediately after some step (1) or some step (2) of some round t <= m. Otherwise (Firer has a line of play in which every vertex holds at most h-1 chips after every step through the end of round m) the list loses.

DEFINITIONS. G(h) = the least m for which a winning list of length m exists. C(h) = the least m for which the constant list 0, 0, ..., 0 (m zeros) wins.

HEADLINE. Find the exact function G(h) for all h >= 1. As concrete deliverables: compute G(3), G(4), G(5) with one shortest winning list each, and determine the constant L = lim G(h)/h (prove the limit exists and give its exact value).

SECONDARY (same computation). Compute C(3), C(4), C(5) and decide whether G(h) < C(h) for every h >= 3.

UNVERIFIED beliefs, to be reproduced, not trusted:
 - G(1) = 1, G(2) = 2, C(1) = 1, C(2) = 2.
 - C(3) = 7.
 - G(3) <= 5 via the list 0, 1, 0, 1, 0; probably G(3) = 5.
 - h <= G(h) <= C(h) for all h.
 - C(h) <= sqrt(24)*(h-1) + O(1), by the potential Q = sum over vertices x of x^2 * (chips on x): adding at 0 leaves Q unchanged, each firing raises Q by exactly 2, there are at most m firings, yet n chips with every height <= h-1 force Q >= about n^3 / (12 (h-1)^2). Hence G(h) is finite and Theta(h).

GUESS (not a claim): G(h) = L*h + O(1) with 1 < L < sqrt(24) and L strictly below lim C(h)/h, because mixing two adjacent sites already beats the constant list at h = 3 and I expect that advantage to persist as a constant factor.

## WHY IT IS PROBABLY UNCHARTED

Chip-firing with an adversarial, rate-limited firer and an oblivious adder is not a standard model; sandpile literature fixes the firing rule (stabilise fully) and studies shapes, not races. The order of growth is easy, so the open content is the exact function and the constant L, which need the optimal list shape. Doubts: the game may reduce to a known one-dimensional "chip games"/balancing result I do not recall; G(h) may have no clean closed form, leaving only L; and L might equal the constant-list constant after all for large h.

## HOW TO CHECK AN ANSWER

For fixed h, m: enumerate lists up to translation and reflection; for each, compute the set of Firer-reachable positions with all heights <= h-1 after each step (breadth-first over pass/fire choices); the list wins iff this set is empty by round m. G(h) is the least m with a winner. Finiteness of the enumeration must be justified: Firer's one firing per round couples far-apart groups, so gaps cannot simply be dropped; argue instead that gaps above 2m+2 can be shrunk to 2m+2 without changing the outcome. A general formula needs both an explicit list family (upper bound) and a Firer strategy against every list (lower bound); L needs existence of the limit.
