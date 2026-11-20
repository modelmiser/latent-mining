## KNOWN

The classical firing squad synchronisation problem (Moore, Myhill) is solvable for all n with a fixed state set; minimal time is 2n - 2 (Goto, Waksman, Balzer); Mazoyer gave a 6-state minimal-time solution; Balzer showed 4 states do not suffice for minimal time. Solutions with reduced communication (1-bit channels, Mazoyer; Umeo) and with left/right-symmetric rules (Szwerinski, I believe) exist. For the question as given I found no ill-posedness: L(s) is well defined, nondecreasing, and L(s) >= 2 trivially. I expect, without proof, that for large s the equality-blind model can simulate full neighbour visibility by time-multiplexed probing (a cell cycles through guesses while its neighbour holds still, with roles assigned by the general's first wave), giving L(s) = infinity at linear slowdown. Whether 2n - 2 survives: unsure. The search space for s = 5 (about 5^36 raw rules) is uncomfortable for exhaustive work.

## SUCCESSOR QUESTION

Counting-blind firing squad. Fix an integer s >= 3 and a state set S of size s containing three distinct named states: Q (asleep), G (general), F (fire). Cells 1..n stand in a line, n >= 2; cell i has neighbours i-1 and i+1 where those exist (cells 1 and n have one neighbour each). A cell never sees a neighbour's state, cannot tell left from right, and gets no end-of-line marker. At each tick a cell sees only its own state x and one number k in {0, 1, 2}: the number of its existing neighbours whose current state is different from x. (So an end cell always has k in {0, 1}, and an end cell with k = 1 sees exactly what an interior cell with one differing neighbour sees.) A rule is a function d: (S \ {F}) x {0, 1, 2} -> S taking (own state, k) to the next state, subject to the sleep constraint d(Q, 0) = Q. All cells use the same d and update simultaneously at each tick. At time 0 cell 1 is in G and every other cell is in Q. The rule d synchronises length n if there is a time t >= 1 such that every cell is in F at time t and no cell is in F at any earlier time (behaviour after time t is irrelevant; if some cell enters F while another does not, d fails on that n). The rule has no access to n other than through the numbers k it sees.

Define L(s) as the largest N such that some single rule d on s states synchronises every length n with 2 <= n <= N; put L(s) = infinity if some rule on s states synchronises every n >= 2. (L(s) >= 2 always: take d(G,1) = d(Q,1) = F.)

Questions.
(1) Determine L(3), L(4), L(5) and L(6) exactly, and for each of these s the number of rules d attaining L(s).
(2) Decide whether L(s) = infinity for some s. If so, determine the least such s and the least function T(n) such that some counting-blind rule (any finite number of states) synchronises every n >= 2 and fires on length n at time T(n); in particular decide whether T(n) = 2n - 2 is attainable. If L(s) is finite for every s, determine L(s) as a function of s, or at least decide whether L(s) is bounded.

## WHY IT IS PROBABLY UNCHARTED

Symmetric rules and low-bandwidth channels have each been studied, but I know no work where a cell learns only how many neighbours differ from itself: no direction, no end marker, no neighbour content. The probing simulation that presumably settles the original needs to tell which side answered; here that is gone, and an end cell is indistinguishable from an interior cell with one agreeing neighbour. Doubts: a clever encoding may still restore direction (moving waves with refractory trails), making part (2) a routine simulation; or L(s) may be tiny for all small s, making part (1) dull.

## HOW TO CHECK AN ANSWER

A rule is a table of 3(s-1) entries, so there are at most s^(3s-3) rules: 4^9, 5^12, 6^15. For s <= 5 enumerate all; for s = 6 use backtracking that fixes entries only when a simulation first needs them. For each rule simulate n = 2, 3, ... until failure (premature F, or a repeated configuration without firing). Sanity anchor, hand-derived and unverified: L(3) = 2. A claimed L(s) = infinity needs an explicit rule plus an invariant proof; a claimed bound needs an indistinguishability argument. Claimed T(n) is checked by simulation for n up to a few hundred.
