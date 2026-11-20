# Experiment 20: counting the last w bits with w + 1 states

A machine reads a stream of bits and should know how many of the last w were
1. Knowing that exactly takes 2^w states. This machine has only w + 1. It may
stay silent in states where it is too unsure, but on no stream may it stay
silent forever. How unsure does it have to be when it speaks?

**Short answer: the uncertainty f(w) is 0, 1, 1, 1, 2, 2, 3, 3, 4, 4 for
w = 1, …, 10, and at most ⌊(w − 1)/2⌋ for every w ≥ 3.** Whether it equals
⌊(w − 1)/2⌋ for every w ≥ 3 is open: there is no general lower bound beyond
f(w) ≥ 1. The chain guessed ⌊w/2⌋, which is too large at every even w from
4 on (by the upper bound), and exactly one too many at w = 4, 6, 8, 10.

## The question

A machine is a deterministic automaton with w + 1 states, a start state and
a transition for each state and bit. It has no output.

For a state q, let C(q) be the set of all values "number of 1s among the
last w bits" over all strings of length at least w that lead from the start
to q. The spread of q is the largest value in C(q) minus the smallest. A
state with small spread knows the count well.

The designer marks some states as lit. The choice is legal with bound s if
every lit state that can be reached by a long enough string has spread at
most s, and on every infinite stream the machine is in a lit state infinitely
often. f(w) is the least s for which a legal choice exists. Equivalently:
the states of spread more than s must contain no cycle.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md),
formal text in [chain/3-question.txt](chain/3-question.txt)): find
f(w) for all w, and the values f(1), …, f(6).

## Results

| Claim | Status | Where |
|---|---|---|
| f(1 … 6) = 0, 1, 1, 1, 2, 2 | computed without pruning over every machine up to renaming, by solver A and the referee to w = 6 (262,279,982 machines at w = 6) and by solver B to w = 5; solver B's w = 6 is by pruned search | `reports/` |
| f(7) = 3, f(8) = 3 | computed by both solvers and the referee with pruned searches. Solver A's search and solver B's breadth-first run visit the same tree (identical node counts) and count as one check; solver B's depth-first run is the same program in another order; the referee's program is separate | `verifier/runs.log` |
| f(9) = 4 | computed by solver B and by the referee, each with one pruned search | `solver-b/split_w9s3.*`, `verifier/runs.log` |
| f(10) = 4 | computed by the referee only: one program, one search order, about 25 minutes | `verifier/runs.log` |
| **f(w) ≤ ⌊(w − 1)/2⌋ for every w ≥ 3** | proved by hand by both solvers with different write-ups; referee read all of them and found no gap, and tested the stated ranges by program | `solver-a/proofs.txt` P1; `solver-b/proofs.txt` Theorems A, D |
| f(w) ≥ 1 for every w ≥ 2 | proved by hand by both solvers; read by the referee | `solver-a/proofs.txt` P2, `solver-b/proofs.txt` Theorem C |
| A machine with bound s has at least w − s + 1 reachable states | proved by hand by solver B (Theorem B); solver A's P3 is the path lemma behind it, without the count; read by the referee; tested on 6,000 random machines. The literature search thinks this follows from a known lemma on sliding-window counting | `solver-b/proofs.txt` Theorem B |
| If every state must be lit, the values are 0, 1, 1, 2, 2, 2, 3, 3, 4 for w = 1, …, 9 | computed (referee: without pruning to w = 6, pruned beyond) | `reports/referee.md` |
| With one state more than allowed, the bound is beaten at w = 7 (9 states, uncertainty 2) and at w = 9 (11 states, uncertainty 3) | explicit machines found by solver B, checked by the referee's checker | `solver-b/w7n9.txt`, `solver-b/w9n11_s3.txt` |
| f(w) = ⌊(w − 1)/2⌋ for every w ≥ 3 | conjecture; computed for w ≤ 10 | |

## The machines that reach the bound

**Odd w.** The ordinary saturating counter with ⌊w/2⌋ + 2 states (fewer than
allowed): a 1 moves up one level and stops at the top, a 0 moves down one and
stops at the bottom. Solver B's Theorem A shows every state has spread at
most ⌊w/2⌋, which for odd w is ⌊(w − 1)/2⌋.

**Even w.** Solver B's machine has levels 0 to w, moves as the counter does,
except that leaving an end level inward jumps two levels (from 0 a 1 leads to
2; from w a 0 leads to w − 2). The two ends and all odd levels have spread at
most w/2 − 1. The proof does not bound the even inner levels; every step from
one of them leads to an odd level, so whatever states are too unsure contain
no cycle. (By solver A's proof and a reviewer's recomputation, the only even
inner level that actually exceeds the bound is level 2 at w = 4.) The proof (Theorem D) is an accounting of how many steps are taken
from the two end levels, with a parity argument.

Solver A describes one family for all w ≥ 3, a counter on levels 0 to w − 2
whose two end levels are each split into two states ("just arrived" and
"stayed"). The referee found that for even w it is the same machine as solver
B's, by comparing transition tables for w = 4 to 12. So for even w the two
proofs are two arguments about one machine.

## What to distrust

- **Everything is by one model family.** Nothing is machine-checked as a
  proof.
- **The conjecture has thin support.** There is no lower-bound argument past
  f ≥ 1, and the bound is sensitive to the exact number of states: one extra
  state already does better at w = 7 and 9. The solvers put their confidence
  at 85 and 65 per cent; the referee thinks the lower figure is better
  founded.
- **The largest values have the least checking.** For w ≥ 7 every "nothing
  better exists" comes from pruned searches, and pruned and unpruned searches
  were compared only up to w = 6. f(10) = 4 is one run of one program.
  Solver B's certificate for w = 9 splits the search into prefixes generated
  by the program under test. A wrong pruning could only make a reported value
  too large, since each value is also reached by an explicit machine.
- **The literature search read one source at length and none in full.** The
  nearest work is on counting ones in a sliding window with additive error
  (Ben-Basat, Einziger, Friedman, Kassner, 2016), with results in bits of
  memory and up to constants, where every configuration must answer. The
  search did not find the exact question, the values, or the two families.
  Two older papers on finite-memory prediction with saturating counters
  (Meron and Feder, 2004; Leighton and Rivest, 1986) were seen only as an
  abstract and a snippet.
- **The headline as asked is not delivered.** The chain asked for a closed
  formula with proof; what exists is an upper bound with proof and ten
  computed values.
- **Small errors.** Solver B's `results.txt` gives one table entry as "≥ 11"
  where its report's "= 11" is right. In solver A's P3 "it equals e" should
  read min(e, w); in solver B's Theorem C "k ≥ w" is valid and can be
  sharpened to k = w. Two steps of solver A's P1 are loosely written: the
  inequality used for m(j) + m(j′) fails at j = 1 as stated (the conclusion
  holds), and one bound for a split end state is false at w = 3, where the
  proof treats that case separately. The referee's lemma test printed eight
  failures at w = 3 for that same reason; the test's bound was too strict.
  Solver B's `fast3.cpp` documents an option that only changes a log label. One of the referee's own broken test files was
  rejected for a reason other than the intended one. Solver B reported
  exceeding its two-core limit for a few minutes.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files. All five confirmed
the reformulation as "no cycle among the over-wide states", both upper-bound
proofs, the two general lemmas, the safety of the pruning, and the
description of the machines. All five marked the top-level summary row
refuted, for saying "exhaustive search three times" up to w = 6 and "no
general lower bound". Their findings, applied above:

- Solver B's w = 6 result is by pruned search, so only w ≤ 5 was done three
  times without pruning.
- "One too many at every even w" claimed the exact value for all even w; it
  is computed only to w = 10 (Claude Opus).
- The state-count bound was credited to both solvers; solver A wrote only
  the lemma behind it.
- "The two solvers visit the same tree" ignored solver B's second search
  order.
- The loose steps in solver A's proof and the failures printed by the
  referee's lemma test were not mentioned.

The corrected wording has not been through a second round.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: searches, machines (`witness_w*.txt`,
  `witnesses.txt`), checkers, proofs.
- `verifier/`: the referee's search, definitional checker, lemma tests and
  broken test files.
- `prior-art.md`: the literature search.

Binaries are left out.
