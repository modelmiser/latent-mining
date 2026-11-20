## KNOWN

Under global fairness, population protocols stably compute exactly the semilinear predicates (Angluin, Aspnes, Eisenstat and co-authors). Exact majority has a 4-state protocol (strong A/B cancel to weak a/b; strong converts opposite weak), and 3 states are known not to suffice for all n. State complexity of counting predicates is an active field; exact bounds: unsure.

The seed is ill-posed in three ways. "Restricted only mildly" is undefined. Bounding how long an *agent* may be ignored is degenerate: the adversary can keep two specific agents apart forever (e.g. fix a perfect matching), so nothing global is computable. And "which questions can be answered" has no single answer. Also, against any adversary who may schedule meetings that change nothing, time is unbounded. The 4-state protocol itself fails against an adversary who must only make progress: it can flip weak agents back and forth forever while never letting the strong A and strong B meet.

## SUCCESSOR QUESTION

A *protocol* with s states consists of: a set Q with |Q| = s; two distinct states q_A, q_B in Q; an output map w: Q -> {A, B}; and a rule d: Q x Q -> Q x Q.

A *configuration* is a multiset of states (one per agent; agents are anonymous). The *input* (a, b), with integers a, b >= 0 and a != b, is the configuration with a agents in q_A and b agents in q_B. Its *majority* is A if a > b, else B.

Play is a one-player game for an adversary who knows the protocol and sees the whole configuration at all times. A *move*: the adversary picks two distinct agents and names one of them first; if their states are (p, q) in that order, the move is *legal* exactly when d(p, q) != (p, q), and the two agents then take the states d(p, q) (first agent gets the first component). The adversary must make a legal move whenever one exists; there is no other restriction. If no legal move exists, play stops. Otherwise play continues forever.

A configuration is *good* if every agent's state has output equal to the input's majority. The protocol *wins* on input (a, b) if in every possible play from that input, finite or infinite, there is a point after which every configuration is good (for a finite play: the final configuration is good). Equivalently: every configuration reachable from the input that has no legal move, or that lies on a cycle of legal moves, is good.

Define N(s) as the largest integer N such that some single s-state protocol wins on every input (a, b) with a != b and 1 <= a + b <= N; put N(s) = infinity if there is no largest such N.

Determine N(s) for s = 2, 3, 4, 5, and determine the growth of N(s): is it finite for every s, and if so what is it as a function of s (exactly, or up to a constant factor)?

## WHY IT IS PROBABLY UNCHARTED

The usual theory assumes global or probabilistic fairness, where four states do majority for every n. Here the adversary is constrained only to make progress, so every useful protocol must make correctness survive all cycles in its configuration graph; the standard cancel-and-convert trick dies. I know constructions with states growing with n (merge signed counts, track weights), suggesting N(s) is finite and growing, but I know no theorem fixing it. Doubts: work on "silent" protocols, weak fairness, or termination of Petri nets may imply bounds I am unaware of; N(s) = infinity for some s is not excluded by anything I can prove.

## HOW TO CHECK AN ANSWER

For a given protocol and input, build the reachable configuration graph (multisets, at most C(n+s-1, s-1) nodes), find terminal nodes and strongly connected components containing an edge, and test that all are good. For N(s): enumerate protocols by backtracking, assigning d(p, q) only when first needed, up to relabelling, over inputs of increasing size; s <= 3 is immediate, s = 4 is 16^16 raw but prunes heavily. Hand anchors: N(2) = 2; the rule A+B -> c+c, A+c -> A+A, B+c -> B+B wins up to size 4 and fails on (3, 2). A lower bound needs one protocol; an upper bound needs the exhaustive search or a proof.
