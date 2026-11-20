## KNOWN

The question is well-posed; its "equivalently" clause is right, since a configuration on no cycle is visited at most once. Under global fairness, 4 states give exact majority for every size (strong/weak cancellation). Without fairness that protocol loses on (3, 2): after one cancellation the adversary alternates the two weak-flipping rules forever. I believe N(2) = 2 by a short case analysis. Finiteness of N(s) (pumping) and linear growth (counting states) both look plausible; unsure.

## SUCCESSOR QUESTION

A *protocol* with s states consists of: a set Q with |Q| = s; two distinct states q_A, q_B in Q; an output map w: Q -> {A, B}; and a rule d: Q x Q -> Q x Q.

A *configuration* is a multiset of states (one per agent; agents are anonymous). A *planted instance* is a triple (a, b, x) with integers a, b >= 0, |a - b| >= 2, and x in Q. Its initial configuration has a + b + 1 agents: a in q_A, b in q_B, and one extra agent (the *plant*) in state x. Its *majority* is A if a > b, else B (the plant does not count). After the start the plant is an ordinary agent following d.

Play is a one-player game for an adversary who knows the protocol and sees the whole configuration at all times. A *move*: the adversary picks two distinct agents and names one of them first; if their states are (p, q) in that order, the move is *legal* exactly when d(p, q) != (p, q), and the two agents then take the states d(p, q) (the first-named agent gets the first component). The adversary must make a legal move whenever one exists; there is no other restriction (no fairness). If no legal move exists, play stops; otherwise it continues forever.

A configuration is *good* if every one of the a + b + 1 agents, the plant included, is in a state whose output equals the instance's majority. The protocol *wins* the instance if in every possible play from its initial configuration, finite or infinite, there is a point after which every configuration is good (for a finite play: the final configuration is good). Equivalently: every reachable configuration that has no legal move, or that lies on a cycle of legal moves (a legal move leaving the multiset unchanged is a cycle), is good.

Define G(s) as the largest integer N >= 2 such that some single s-state protocol wins every planted instance (a, b, x) with 2 <= a + b <= N and every x in Q; put G(s) = 1 if no s-state protocol achieves even N = 2, and G(s) = infinity if there is no largest such N.

Determine G(s) for s = 2, 3, 4, and the behaviour of G(s) as s grows: is it finite for every s; is it bounded by a constant independent of s; if not, what is it as a function of s (exactly, or up to a constant factor)?

## WHY IT IS PROBABLY UNCHARTED

Self-stabilising protocols corrupt every agent and assume fairness; Byzantine models let the bad agent misbehave forever. One honest agent in a forged state, an unfair scheduler and an exact state count is a combination I have not seen. Every state storing a large count becomes a forgery to survive, so states are liabilities too. Doubt: G(s) may collapse to a constant by a short argument.

## HOW TO CHECK AN ANSWER

For fixed s and N, enumerate protocols by backtracking over the entries of d actually used. For each, explore all configurations reachable from every initial configuration; reject if a terminal configuration, or one on a cycle, is not good. G(s) = N needs a survivor at N and none at N + 1. A growth law needs a protocol family plus an impossibility proof.
