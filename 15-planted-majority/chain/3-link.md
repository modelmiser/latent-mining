## KNOWN

The fair-scheduler version is classical: a 4-state protocol computes exact majority for every population size (Draief–Vojnović; Mertzios et al.), and it is silent. That protocol fails here: with both strong states present the unfair scheduler flips a weak agent back and forth forever (instance (3,1,weak)). The question as received is well posed (the cycle/terminal equivalence is correct for a finite graph) but asks four things at once, and one of them looks easy: I believe G is unbounded, by a protocol in which each agent starts with a vote of weight K and every opinion flip costs one unit of weight, so all plays terminate and the plant is worth at most one honest vote. That gives G(s) >= about s/2 (unverified, reasoned in my head only). So "bounded by a constant?" is probably "no", and the live question is the upper side. Unsure of any literature on unfair scheduling plus one planted agent.

## FINAL QUESTION

A *protocol* with s states is: a set Q, |Q| = s; two distinct states q_A, q_B in Q; an output map w: Q -> {A, B}; a rule d: Q x Q -> Q x Q (ordered pairs; d need not be symmetric).

A *configuration* is a multiset over Q (anonymous agents). A *planted instance* is (a, b, x) with integers a, b >= 0, |a - b| >= 2, x in Q. Its initial configuration has a + b + 1 agents: a in q_A, b in q_B, one (the plant) in x. Its *majority* is A if a > b, else B; the plant does not count, and after the start it is an ordinary agent.

A *move* from a configuration: choose two distinct agents in an order; if their states are (p, q) in that order, the move is *legal* iff d(p, q) != (p, q), and the first agent takes the first component of d(p, q), the second the second. A scheduler who knows everything picks moves; it must pick a legal move whenever one exists and is otherwise unrestricted (no fairness). Let R be the set of configurations reachable from the initial one by legal moves, with a directed edge for each legal move (self-loops when the multiset is unchanged, e.g. a swap).

A configuration is *good* if every agent, plant included, is in a state whose output is the instance's majority. The protocol *wins* the instance iff every configuration in R that has no legal move, and every configuration in R lying on a directed cycle (self-loops included), is good. (Equivalently: every play, finite or infinite, is good from some point on.)

G(s) = the largest N >= 2 such that one s-state protocol wins every planted instance with 2 <= a + b <= N and every x in Q; G(s) = 1 if none achieves N = 2; G(s) = infinity if no largest N exists.

HEADLINE (yes/no): Is there an absolute constant C with G(s) <= C*s for every s >= 2? (A "no" includes the case G(s) = infinity for some s.) A full answer gives the exact function G(s); the first open exact values are G(3) and G(4).

UNVERIFIED beliefs:
- G(2) = 1.
- G(3) >= 3, via states A, B, C: d(A,B) = d(B,A) = (C,C); A with C (either order) makes both A; B with C makes both B; all else identity. This protocol fails at N = 4 on (1, 3, C).
- G is non-decreasing (add a duplicate of q_A).
- G(s) >= floor((s - 6)/2), by a "budget" protocol: states are weights in {-K..-1, 1..K} plus two weak states wa, wb; q_A = +K, q_B = -K; opposite signs (i, -j) net out, the larger keeping |i - j| with its sign, the other becoming weak with the winner's letter (tie: wa and wb); a positive weight i meeting wb gives (i - 1, wa), where weight 0 means wa; symmetric for negatives; all else identity. Every move lowers total absolute weight, so plays are finite; K = N + 2 is claimed to suffice.
- So G is unbounded; whether it is ever infinite or superlinear is the question.

GUESS: yes (G(s) = Theta(s)), because the plant can impersonate any state, so no state may be worth more than about one vote, while forcing termination against an unfair scheduler seems to need a per-agent budget that grows with N.

## WHY IT IS PROBABLY UNCHARTED

Population-protocol majority is studied under fair or random schedulers, and Byzantine variants usually keep fairness. Here the scheduler is unfair but cannot idle, and one agent starts in an arbitrary state; I know no paper with that pair. Doubts: self-stabilising population protocols and "silent" protocols are close neighbours, and a standard pumping argument may settle finiteness of G(s) quickly, leaving only constants. My linear lower bound is unchecked.

## HOW TO CHECK AN ANSWER

Fixed s, N: fix q_A = 0, q_B = 1; enumerate w (2^s) and d ((s^2)^(s^2), by backtracking that assigns d entries only when a play first needs them). For each instance (a, b, x) with 2 <= a+b <= N, build R by breadth-first search over multisets, compute strongly connected components, and require every terminal configuration and every configuration in a component containing an edge to be good. G(s) is the largest N passed by some protocol; s = 3 is minutes. A "yes" needs a proof that any s-state protocol loses some instance with a + b <= C*s; a "no" needs an explicit protocol family with a termination and correctness proof, checkable by the same program at small N.
