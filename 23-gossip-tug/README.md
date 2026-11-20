# Experiment 23: gossip with one player stalling and one hurrying

Each of n people knows one secret. When two of them talk, each tells the
other every secret they know. A call is allowed only if at least one of the
two does not yet know the other's own secret. Two players steer the calls:
the Delayer names a person who must make the next call, the Hastener picks
that person's partner. The Delayer wants as many calls as possible before
everyone knows everything, the Hastener as few. How many calls are made
under best play?

**Short answer (computed; proved by hand only up to four people): 1, 3, 6,
9, 12, 16, 19, 23, 27 calls for n = 2, …, 10.** The last of these has the
least behind it: its upper side comes from searches that rely on hand-proved
bounds and has no strategy file. The
chain asked whether the answer is always 2n − 3 (for n ≥ 4) and bet yes. It
is not: with four people the Delayer can force all six possible calls, not
five, and the computed values keep pulling away from 2n − 3. What is proved
in general is only that at least 2n − 3 calls are made. The exact function
is not known, and neither is whether the number of calls per person stays
bounded.

The rule for which calls are allowed is a known one ("Learn New Secrets" in
the literature on gossip protocols, with either party allowed to be the one
who learns). The game between two players was not found there.

## The game

Agents are numbered 1 to n; agent i starts knowing only secret i. A call
{a, b} is legal if a does not know b's secret or b does not know a's. After
a call both know the union of what they knew. An agent is active if some
legal call includes it. Play ends when everyone knows every secret.

In each round the Delayer names any active agent a, and then the Hastener
chooses any b for which {a, b} is legal. Both see everything. A(n) is the
number of calls under best play by both.

Without the players, the shortest possible run has 2n − 4 calls for n ≥ 4
(the classical gossip number; 1 and 3 calls for n = 2 and 3) and the longest
n(n − 1)/2 (no pair can call twice).

The question, as the chain left it ([chain/3-link.md](chain/3-link.md),
formal text in [chain/3-question.txt](chain/3-question.txt)): is
A(n) = 2n − 3 for every n ≥ 4?

## Results

| Claim | Status | Where |
|---|---|---|
| **A(4) = 6**, so the answer is no | proved by hand by solver B (a five-line Delayer strategy; six is also the number of pairs); computed by all three instances | `solver-b/proofs.txt` Theorem A |
| A(2), …, A(7) = 1, 3, 6, 9, 12, 16 | computed by all three instances by full minimax; for n = 4 to 7 both solvers produced strategy files that the referee's separate checker accepts (solver A for both players, solver B for the Delayer and as full value tables) | `solver-a/cert_n*`, `solver-b/table*.txt` |
| A(8) = 19 | computed by all three instances by full minimax; Delayer strategy files from both solvers accepted by the referee's checker; no file for the Hastener's side | `reports/referee.md` |
| A(9) = 23 | computed by all three instances by threshold searches ("can the Delayer force t?"), yes at 23 and no at 24; no strategy files | `reports/referee.md` |
| A(10) = 27 | computed by solver B and the referee by threshold searches; every "no at 28" run uses a hand-proved upper bound (solver B's Lemma 3) to cut the search, alongside two lower bounds, and skips moves that differ only by swapping two interchangeable agents. Solver A has only A(10) ≥ 27 | `reports/referee.md` |
| A(11) ≥ 29 (≥ 30 by solver B's run) | the referee's program confirms 29; its attempts at 30 ran out of time | |
| **A(n) ≥ 2n − 3 for every n ≥ 2** | proved by hand by solver A only; read by the referee | `solver-a/proofs.txt` P1 |
| The exact A(n); whether A(n)/n is bounded | open | |

## Why four people need all six calls

This follows `solver-b/proofs.txt` Theorem A.

After the first call, say between 1 and 2, the Delayer names 1. Agent 1 can
only call someone new; say 3. Now 1 and 3 know secrets 1, 2, 3 and agent 2
knows 1, 2. The Delayer names 2.

- If the Hastener answers with 3, agent 4 has still not taken part. Its
  secret has to reach three more people, and a call adds at most one new
  knower of a given secret: three more calls, six in all.
- If the Hastener answers with 4, then 2 and 4 know 1, 2, 4. The Delayer
  names 2 again; its only legal partner is 3, after which 2 and 3 know
  everything. The Delayer then names 3, who knows everything but is still
  active; its only legal partner is 4. That is five calls, and agent 1 still
  lacks secret 4.

The last step uses a feature of the rules as written: the Delayer may name
someone who already knows everything, as long as that person still has a
legal call.

## Why at least 2n − 3 in general

This follows `solver-a/proofs.txt` P1.

The Delayer keeps naming agent 1 for as long as it is active. Every such
call is with someone new, so this phase lasts exactly n − 1 calls, and
afterwards the agents' knowledge forms a chain: the i-th partner knows
exactly what agent 1 knew after i calls. Two agents know everything (agent 1
and its last partner). From then on, whatever anyone does, every agent's
knowledge is a member of that chain, and the union of two members is the
larger one. So a call makes someone fully informed only if the other party
already was, and each call adds at most one fully informed agent. Getting
from 2 to n takes at least n − 2 more calls.

Solver A reports that seven simple Delayer rules of this kind each gave
exactly 2n − 3 for n = 4 to 8 (an exploration with no saved output); the
larger computed values need cleverer play.

## What to distrust

- **Everything is by one model family, and the values beyond n = 4 are
  computations.**
- **The larger values have less behind them.** For n = 4 to 7 there are
  strategy files for both players that a short separate program checks. At
  n = 8 only the Delayer's side has one. At n = 9 and 10 there are no files,
  only searches, and the upper bound at n = 10 depends on a hand-proved bound
  used to cut the search (the three bounds were read by the referee and by
  the reviewers, and hold at every reachable position for n ≤ 8). The
  Delayer files of solver B, and the referee's checker for them, score some
  end positions by one of those bounds instead of playing on, and nobody
  checked the test for interchangeable agents that the n = 10 searches use.
- **The answer depends on a detail of the rules.** If the Delayer may name
  only agents who do not yet know everything, the referee computes 5, 8, 10,
  14, 16 for n = 4, …, 8. That reproduces the chain's belief that four
  people need five calls, and the answer to its question is still no, first
  failing at n = 5 (8 against 7). Only the referee computed this variant.
- **No formula is supported.** The differences A(n) − A(n − 1) for n = 3 to
  10 are 2, 3, 3, 3, 4, 3, 4, 4. Solver A's fit ⌈7n/2⌉ − 9 matches n = 5 to 9
  only (it gives 26 at n = 10 and 5 at n = 4). Solver B's observed identity
  relating A(n) to a family of side positions is, by the referee's analysis,
  a numerical coincidence (those positions cannot be reached in the number of
  calls the identity assumes), and solver B calls it a guess. The values
  for n = 4 to 10 happen to match seven consecutive terms of an unrelated
  sequence in the OEIS (A053096).
- **The literature search read sections of two papers and none in full.**
  The call rule and the shortest and longest runs are in Apt, Grossi and van
  der Hoek (2016) and van Ditmarsch, van Eijck, Pardo, Ramezanian and
  Schwarzentruber (2015). A scheduler that maximises the number of calls
  appears in Livesey and Wojtczak (2022). The search found no game with
  opposed players, none of the values, and not the bound 2n − 3 in this
  form; it did not search theses or puzzle forums.
- **Errors in the chain's question.** Its claim A(4) = 5 and its guess are
  wrong under the rules as it wrote them.
- **Small errors.** Three things in solver B's files are weaker than they
  look: `bad5.txt` is identical to the correct table, `badcert5_root.txt`
  contains a valid strategy with only its claimed number changed, and its
  strategy files are indexed by position and target, so reading them as
  "position to move" needs the highest-target line for each position (read
  that way, the referee's checker accepts all five). Solver B reported a few
  sub-second runs overlapping two long ones.
- **Two large files are left out** (solver B's value table for n = 7,
  1.1 MB, and its Delayer strategy for n = 8, 6 MB); the programs that
  regenerate them are included. Solver A's Hastener file for n = 7 and
  Delayer file for n = 8 (0.7 and 0.6 MB) are included, after reviewers
  pointed out that the page relied on them. Solver B's `check_table.cpp`
  uses arrays of 8 entries and must not be run for more than 8 agents.

## Review

Before publication the claims went to five reviewers from four model
families: GPT, Grok and Gemini, and two Claude instances (one of them Claude
Opus). All five confirmed the hand proof that four people need six calls
(each replayed it from the rules), the general bound 2n − 3, and the three
bounds used by the larger searches. Both Claude seats recomputed the first
few values with their own short programs. Their findings on this page,
applied above:

- The page relied on two strategy files that a size filter had left out of
  the folder, and listed two omitted files where there were four (four
  seats). The two are now included.
- The top-level summary said "more than 2n − 3 from four people on", which
  reads as a theorem; it is proved at four and computed up to ten (all five
  seats).
- The classical 2n − 4 lacked "for n ≥ 4" (all five seats).
- "Those positions cannot arise in play" was false as worded (GPT, Grok).
- The headline listed A(10) = 27 without its caveat (Grok, a Claude seat).
- What the larger searches and solver B's files rely on was understated.

The corrected wording has not been through a second round.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: minimax and threshold-search programs, strategy
  files, checkers, proofs.
- `verifier/`: the referee's programs, including the variant in which fully
  informed agents cannot be named.
- `prior-art.md`: the literature search. The downloaded papers are not
  included.

Binaries are left out.
