# Experiment 15: majority among anonymous agents, with one planted agent and no fairness

A population of identical agents starts with two opinions, A and B, plus one
extra agent planted in an arbitrary state. Agents meet in pairs and change
state by one fixed rule. Whoever chooses the meetings is an adversary with no
duty to be fair, except that it must let some pair interact as long as some
pair can change. The population must end up, and stay, with every agent
showing the opinion that was in the majority by at least two. With s states
per agent, how large a population can a rule handle?

**Short answer: more than linearly many, but never all sizes.** If G(s) is
the largest number a + b of ordinary agents up to which some s-state rule
handles every case, then G(s) is at
least of order s³ (precisely G(8M − 5) ≥ M³ − 1), and G(s) is finite for
every s. The chain asked whether G(s) ≤ C·s for some constant C and bet yes;
the answer is no. The exact function is not known, and neither is whether it
is bounded by a polynomial. The small values are G(2) = 1, G(3) = 3,
G(4) = 3.

## The game

A protocol has a set Q of s states, two distinct starting states q_A and q_B,
an output A or B for each state, and a rule that maps each ordered pair of
states to an ordered pair of states.

A planted instance (a, b, x) starts with a agents in q_A, b in q_B and one
more agent in state x, where a and b differ by at least 2 and x is any state.
Its majority is A if a > b and B otherwise; the planted agent does not count.

A move picks two agents in an order; it is legal if the rule changes their
pair of states. The scheduler knows everything and picks any legal move while
one exists. The protocol wins the instance if on every play, finite or
infinite, from some point on every agent (the planted one included) shows the
majority. Formally: every reachable configuration with no legal move, and
every reachable configuration on a cycle, must be good. A move that swaps two
different states leaves the collection of states unchanged and counts as a
cycle of length one.

G(s) is the largest N such that one s-state protocol wins every planted
instance with 2 ≤ a + b ≤ N; it is set to 1 if no protocol manages N = 2.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md),
formal text in [chain/3-question.txt](chain/3-question.txt)): is there a
constant C with G(s) ≤ C·s for all s?

## Results

A set S of positive integers is called *closed* here if every element from 2
up is a sum of two elements of S, and every difference u − v of two elements
u > v is an element of S or a sum of two elements of S. (Solver A's definition allows one
more way to resolve a difference; the referee found that no set used here
needs it.)

| Claim | Status | Where |
|---|---|---|
| **For a closed set S with largest element K, G(2\|S\| + 1) ≥ K − 1** | proved by hand by solver A; solver B proves the same with one state more; referee read both and found no gap; the resulting protocols for several small sets were checked exhaustively and win exactly as far as stated or further | `solver-a/proofs.txt` Theorems 1, 1′; `solver-b/proofs.txt` T1 |
| G(4M − 1) ≥ M² − 1, so no constant C has G(s) ≤ C·s | from the row above with an explicit closed set (solver A); solver B proves G(4M) ≥ M² − 1, which gives the same conclusion | `solver-a/proofs.txt` Corollary 1a, `solver-b/proofs.txt` T1 |
| **G(8M − 5) ≥ M³ − 1** | proved by hand by solver A with an explicit closed set of 4M − 3 elements; referee read it and checked by program that the sets are closed for M = 2, …, 12. One identity in the hand proof is misprinted (see "What to distrust"); the sets are closed all the same. The first protocol where this beats the quadratic bound (19 states, N = 26) was enumerated by the referee only up to N = 13 | `solver-a/proofs.txt` Corollary 1b |
| **G(s) is finite for every s** | proved by hand by solver A only; referee read it line by line, tried to break three of its steps and found no gap. No explicit bound comes out of the proof | `solver-a/proofs.txt` Theorem 2 |
| G(2) = 1, G(3) = 3 | computed by both solvers and the referee in the reduced model; in the model as stated, G(2) by all three and G(3) by solver B and the referee (solver A's search there timed out) | `reports/` |
| G(4) = 3 | computed in the reduced model only (one outcome per unordered pair of states, no swaps), which a hand-proved lemma shows is enough; the solvers' searches and the referee's first one are the same search (node counts agree to the unit); the referee's second, with a different branching rule, also finds nothing | `solver-a/proofs.txt` Lemma R, `reports/referee.md` |
| G(6) ≥ 5, G(8) ≥ 5, G(10) ≥ 7, G(11) ≥ 9, G(15) ≥ 15 | explicit protocols, checked by the referee's checker | `solver-b/witness_s6_N5.txt`, `solver-a/fam*.txt` |
| G is non-decreasing | proved by hand by both solvers | `solver-a/proofs.txt` Lemma M |
| The largest possible maximum of a closed set with m elements is 1, 2, 4, 6, 10, 14, 18, 24, 32, 42 for m = 1, …, 10 | computed by solver A and recomputed by the referee; solver A's values 50, 62, 75 for m = 11, 12, 13 were checked only as closed sets, not as maxima | `solver-a/closed.py` |
| The exact G(s); G(5); whether G is polynomially bounded | open | |

## How a protocol can handle a population much larger than its state count

This follows `solver-a/proofs.txt`, Theorems 1 and 1′.

States are tokens +u and −u for u in a closed set S, which show A and B
respectively, and two weightless states, one showing each opinion. (Theorem
1′ merges the two weightless states into one, which saves a state.) Agents
start as +K or −K, where K is the largest element. The weight
of a configuration is the sum of its tokens' signed values, and every rule
preserves it.

- A positive and a negative token cancel as far as they can: +u and −v with
  u > v leave the difference u − v, with its sign, on one agent if u − v is in
  S, and otherwise spread over both as a sum of two elements. This is where
  the second closure condition is used. An agent left with nothing becomes
  weightless, showing the opinion of the token that survived (in Theorem 1′
the single weightless state always shows A; an exact tie leaves one
weightless agent of each kind in Theorem 1).
- A token of value at least 2 that meets a weightless agent showing the
  wrong opinion splits its value between the two of them. This is where the
  first closure condition is used.

Every move lowers the total of absolute values, or keeps it and removes a
weightless agent, so every play ends, whatever the scheduler does. The total
weight favours the true majority by at least K even if the planted agent is
the strongest token of the wrong sign. A stuck configuration therefore has
tokens of the majority's sign only. If it also had a weightless agent showing
the wrong opinion, no token could be splittable, so every token would have
value 1, and then the weight would be at most the number of tokens. That is
at most a + b, since at least one of the a + b + 1 agents is weightless, and
a + b ≤ K − 1 is less than K.

The chain's own protocol spends a unit of weight to convert a wrong agent, so
its weights have to run through every value up to K: about K states. Here a
conversion splits weight and loses none, so S only needs to be closed. The
set {1, …, M} together with the multiples of M up to M² is closed, with
2M − 1 elements and largest element M². Adding the numbers M² − j and the
multiples of M² up to M³ keeps it closed: 4M − 3 elements, largest element
M³. Solver A reports that the next step of this recursion is not closed for
M ≥ 4.

## Why no protocol handles every size

This follows `solver-a/proofs.txt`, Theorem 2. Suppose one protocol won every
instance.

- No play could ever go round a cycle. Add enough agents in the minority's
  starting state to flip the majority; the scheduler can ignore them and keep the old population
  cycling, and the cycle would have to be good for both majorities.
- A combination of moves with zero net effect would give a cycle in a large
  enough population (run each move on its own private group of agents). So
  none exists, and by a standard theorem of linear algebra (Gordan's) there
  is a potential, linear in the numbers of agents per state, that every move
  lowers by at least a fixed amount. Hence any sequence of legal moves on n
  agents that all start in q_A or q_B has at most c·n moves, with c depending
  only on the protocol.
- The scheduler now feeds agents in one at a time in the order B, A, A, B, B,
  A, A, …, each time letting the agents already active get stuck. (It is
  free to do so: it only ever has to choose some legal move, and it chooses
  among the active agents while they have one.) Whenever
  the active counts differ by one, the stuck configuration is a terminal
  configuration of a planted instance (majority by two, the planted agent in
  the minority's starting state), so all active agents show one opinion; two
  agents later they all show the other. Starting from k agents of each kind
  and feeding in 2m + 1 more forces at least k·m moves on 2k + 2m + 1 agents.
  With k = m large this exceeds c·n.

## What to distrust

- **Everything is by one model family.** Nothing is machine-checked as a
  proof.
- **The finiteness theorem has one author, and one reviewer rejected it.**
  Solver B left the question open. The referee found no gap. Of the four
  panel reports, three accepted the staged scheduling argument and Grok
  rated it a critical gap; the Review section below says why I think the
  objection fails. It is the least-settled general claim here.
- **Slips in solver A's proof file.** In the proof that the cubic sets are
  closed, the identity for (M² − j) − j′ when j + j′ > M has M − 1 where
  M − 2 is meant (found by two reviewers; with the correction the difference
  is again a sum of two elements, and the referee's program confirms the sets
  are closed for M up to 12). Some other identities there silently allow a
  zero summand or need M ≥ 2. In Theorem 2, the case of a protocol with no
  legal move at all is not treated separately (it is excluded by two small
  instances), the last stage names an agent that does not exist, and the
  count "at least k moves" can be k + 1.
- **The cubic bound is checked by proof and by a rule-table test, not by
  playing the game.** For the first size where it matters, exhaustive
  enumeration reached only N = 13 of the claimed 26; the rest rests on the
  hand proof and on a mechanical check that the generated rules conserve
  weight, descend, and exist wherever the proof needs them.
- **G(4) = 3 depends on a reduction lemma and on searches that are not
  independent.** Three programs turned out to be the same lazy search. The
  referee's second search differs in its branching rule only. Nobody
  completed a search in the model exactly as stated for 4 states.
- **The solvers disagree on how far the construction goes.** Solver B's
  report says quadratic growth is the limit of this construction; solver A's
  cubic sets satisfy solver B's own conditions, so that statement is wrong
  (solver B's proof file marks it as a sketch). Solver B's report and proof
  file also give different largest closed sets of one size.
- **The literature search read one source in full.** It found no result on
  this combination (one corrupted agent, no fairness, exact majority, exact
  state count). Nearest: self-stabilising exact majority (Kanaya, Eguchi,
  Sasada, Ooshita, Inoue, 2025), where every agent starts corrupted and the
  scheduler is random; robust protocols under crashes with fairness (Lossin,
  Czerner, Esparza, Guttenberg, Prehn, 2024; and Czerner, Esparza, Fischer,
  Guttenberg, Pins, Reilich, 2026, which counts states exactly for threshold
  predicates but not for majority); and
  state-complexity bounds for fault-free protocols (Czerner, Esparza, Leroux,
  2021). Cancelling signed weights with a conserved sum is a standard device
  in exact-majority protocols; the closure condition and the planted-agent
  argument were not found. The sequence 1, 2, 4, 6, 10, 14, 18, 24, 32, 42 is
  not in the OEIS by direct lookup.
- **The chain's claims.** Its guess of linear growth is wrong. Its own
  "budget" protocol does work, and by the solvers' computations wins somewhat
  further than the chain claimed.
- **Process.** In a note to the organiser that is not part of its written
  report, solver A said that three of its processes ran at once for about a
  minute and that its checker crashed four times on a comment line before a
  parser fix. Solver B reported three early runs
  without the memory cap and short periods with three processes. The referee
  reported about a second with three processes.

## Review

Before publication the claims went to five reviewers from four model
families. One of the two Claude seats returned nothing (the month's review
budget ran out during its run), so there are four reports: Claude Opus, GPT,
Grok and Gemini. All four confirmed the weight protocol and its proof
(Theorems 1 and 1′) and the reduction lemma.

**The finiteness proof.** Claude Opus and Gemini confirmed all of it. GPT
confirmed the reasoning and marked two steps refuted on details now listed
under "What to distrust" (the empty case, the index of the last stage). Grok
rated the staged scheduler a critical gap: once the active agents are stuck,
it says, the scheduler "must move if any legal pair remains", so it cannot
keep inactive agents untouched. I do not think this holds. The rule of the
game is that the scheduler must pick some legal move whenever one exists; it
is never told which. In the staged play it always picks a move between two
active agents; when they have none, it enlarges the active set by one agent
and continues, and every move it makes is legal in the whole population. The
argument needs only that this is a sequence of legal moves, because the bound
of c·n moves applies to every such sequence, complete or not. The same point
was put to the other reviewers in the brief, and they accepted it. A reader
should still treat this theorem as contested until someone outside this
family of models has read it.

**Other findings, applied above.**

- The misprinted identity in the cubic proof (Claude Opus, Grok), which the
  referee and I had both missed.
- My retelling of the counting step compared the weight with the number of
  agents, which is off by one at the boundary; the proof's count is the
  number of tokens.
- G(3) in the model as stated was credited to solver A, whose search there
  timed out; the bound G(4M − 1) ≥ M² − 1 was credited to both solvers.
- G was called a population size without saying that it counts a + b; the
  convention G = 1 and the status of swap moves were missing from the
  definition.
- The 2026 paper was called a sequel under the wrong byline, and solver A's
  process notes had no source in the folder.

The corrected wording has not been through a second round.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: checkers, searches, protocol generators,
  protocols, proofs.
- `verifier/`: the referee's checker, reference implementation, search,
  closed-set programs and protocol generator.
- `prior-art.md`: the literature search. The downloaded papers are not
  included.

Binaries are left out.
