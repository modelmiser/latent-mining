# Experiment 22: a firing squad whose soldiers can only count

A line of identical cells must all enter a "fire" state at exactly the same
tick, starting from one cell (the general) awake and the rest asleep. This is
the classical firing squad problem, and with ordinary cells it can be solved
for every length with a handful of states. Here each cell is nearly blind:
it sees its own state and the number of its neighbours whose state differs
from its own. It cannot see what state a neighbour is in, cannot tell left
from right, and cannot tell that it is at an end of the line. With s states,
up to what length can one rule synchronise every line?

**Short answer: 2, 3, 5, 8 and 11 for s = 3, 4, 5, 6 and 7 states.** The
chain asked for five states and guessed "at most 6"; the answer is 5. These
are computed values: for five states every one of the 48,828,125 rules was
tried, and since 21 November so was every one of the 78,364,164,096
six-state rules; for seven states the value comes from backtracking
searches, not full enumeration: a rule missed by a search could only make
that value too small, while an error common to the searches' simulators could move it either way.
Whether some number of states synchronises every length is open, and
nothing here bears on it either way.

## The problem

There are s states, among them Q (asleep), G (general) and F (fire). Cells
1 to n stand in a line, n ≥ 2. At each tick a cell sees its own state x and
a number k: how many of its existing neighbours are in a state different from
x. An end cell has one neighbour, so its k is 0 or 1; other cells have k in
{0, 1, 2}.

A rule maps (state other than F, k) to a next state, with one constraint: a
sleeping cell whose neighbours are all asleep stays asleep. All cells use the
same rule and update together. At time 0 cell 1 is G and the rest are Q.

The rule synchronises length n if at some tick every cell is F and no cell
was F before. L(s) is the largest N such that one rule with s states
synchronises every length from 2 to N (infinite if some rule synchronises
every length).

The question, as the chain left it ([chain/3-link.md](chain/3-link.md),
formal text in [chain/3-question.txt](chain/3-question.txt)): find L(5).

## Results

| Claim | Status | Where |
|---|---|---|
| L(3) = 2 | computed by all three instances over every rule; also proved by hand by solver B | `solver-b/proofs.txt` P3 |
| L(4) = 3 | computed by all three instances over all 65,536 rules | `reports/` |
| **L(5) = 5**, attained by exactly 1,244 rules | computed by all three instances, each enumerating all 48,828,125 rules with no pruning; the referee also checked solver B's list of the 1,244 rules one by one | `solver-b/all_L5_rules_s5.txt`, `reports/referee.md` |
| L(6) = 8, attained by 336 rules | computed by backtracking searches: both solvers', and the referee's without symmetry breaking, ascending and descending in the line length; and, since 21 November, by a follow-up instance's separate program that simulates every one of the 6¹⁴ = 78,364,164,096 rules with nothing skipped (`enum6/`), which finds the same value and the same count | `reports/referee.md`, `enum6/aggregate_s6.txt` |
| L(7) = 11, attained by 96 rules (4 up to renaming the unnamed states) | computed by backtracking searches: both solvers' (the same search tree, with symmetry breaking) and the referee's two without symmetry breaking, in different orders | `verifier/` |
| A rule that synchronises length n fires at time n − 1 or later; a rule that synchronises length 3 or more must send a sleeping cell with exactly one differing neighbour to a state that is neither asleep nor fire | proved by hand by both solvers (the second statement for length 3 only by solver A, for every length from 3 by solver B); no violation over all rules with s ≤ 5 | `solver-a/proofs.txt` Fact 0, Lemma 1; `solver-b/proofs.txt` P1, P2 |
| If a rule synchronises lengths n < m, it fires length n at time 2n − 1 or later, or at the same time as length m | proved by hand by solver A; read by the referee; tested over all five-state rules | `solver-a/proofs.txt` Lemma 2 |
| The fastest five-state firing times for single lengths n = 2, …, 8 are 1, 3, 4, 6, 7, 9, 10 | computed by solver B and the referee (solver A to n = 7) | `solver-b/brute5_single_out.txt` |
| Is L(s) infinite for some s? | open | |

One rule attaining L(5) = 5, from solver A (U1, U2 are the two unnamed
states; rows are the current state, columns are k):

| state | k = 0 | k = 1 | k = 2 |
|---|---|---|---|
| Q | Q | U1 | U1 |
| G | U1 | G | Q |
| U1 | F | U2 | G |
| U2 | F | G | Q |

It fires lines of 2, 3, 4 and 5 cells at times 5, 8, 7 and 10, and fails on
6 cells.

## What to distrust

- **Everything is by one model family, and nearly everything is
  computation.** Except for three states, there is no argument for why the
  values are what they are.
- **L(5) = 5 is the most checked:** four separately written programs (the three original ones and, since 21 November, the follow-up's) tried every rule and agree on the number of rules reaching each length, and on
  the 1,244 optimal rules one by one.
- **L(6) no longer rests on the backtracking idea alone.** A follow-up
  instance wrote a plain enumerator from the definition (it was told not to read the earlier programs; its source header says it was written from the question text only; its report came back as text and was archived afterwards as `enum6/REPORT.md`): all 14 free entries enumerated (three fixed per chunk, an odometer over the other eleven), each rule run on
  lengths 2, 3, … until it first fails, with "never fires" decided only when
  a configuration exactly repeats. Its 216 chunks add up to 6¹⁴ rules; the
  histogram of how far each rule gets is in `enum6/aggregate_s6.txt`; no rule
  reaches 9. The same program reproduces the five-state result rule for rule
  (1,244 rules) and the four- and three-state values (232 rules reach 3;
  36 reach 2; outputs of the organiser's runs in `enum6/out/`). Its histogram
  equals, at every length, the one in solver A's original report, which
  came from a search; the follow-up compared the 336 rules with the earlier work only through solver B's witness, but solver A's search output (`solver-a/dfs6.txt`) lists the 16 optimal partial rules up to renaming, and expanding them gives exactly the same 336 rules (found by a second-round reviewer; `enum6/compare_solver_a.py`, rerun by the organiser, output in `enum6/compare_solver_a_out.txt`). Limits: the full
  run went through each chunk once; the follow-up's second simulator
  rechecked only the 2,502 rules that reach 7 or more, not the rejections (a
  reviewer's own simulator then redid four whole chunks, rejections
  included, and agreed); the program does not validate its command-line
  arguments, which matters only if it is run with malformed ones; its
  aggregation script labels "fails at length 2" as N=0 where the program's
  internal score is 1; and it is again one model family. The organiser recompiled the program and reran two
  of the 216 chunks, one of them containing optimal rules, with identical
  output (`enum6/organiser_rerun.txt`). All 336 rules are in
  `enum6/max_rules_s6.txt`; 312 of them fire lengths 2 to 8 at times 7, 9,
  9, 15, 17, 13, 13 and the other 24 at 8, 10, 10, 19, 21, 20, 20.
- **L(7), and the earlier evidence for L(6), rest on backtracking searches
  that share one idea** (fix an
  entry of the rule only when a run first needs it). The solvers' two
  programs explore the same tree, with identical counts. The referee removed
  the symmetry shortcut and changed the order in which lengths, cells and
  values are tried, and got the same answers, with a check that the cases
  covered add up to all 7¹⁷ rules. All the referee's programs share one
  simulator.
- **The sequence 2, 3, 5, 8, 11 suggests nothing reliable.** It is not
  Fibonacci (11, not 13). The solvers offer no formula.
- **Nobody computed the other natural reading**, in which a missing
  neighbour at the end of the line counts as "different".
- **The literature search read sections of four sources and none in full.**
  It found no paper with this model (own state plus the number of differing
  neighbours, no border marker); one 2025 paper (Rollier and others) uses
  count-based two-state automata on a different task that it also calls by
  this name. Closest in spirit: a
  preprint of this month (Giam, arXiv:2611.11080) studies the largest N such
  that some k-state rule synchronises every line up to N, for ordinary cells
  with a visible border and minimal firing time, and reports N₄ = 8 from an
  exhaustive search by Yunès and N₅ ≥ 14; and a survey of small-state
  solutions (Correa, Gustavo, Lemos, Settle, 2017). With three states
  only the single lengths 2 and 3 can be synchronised at all (solver B's
  enumeration). For lengths of 5 or more that also follows from Yunès's 2008
  result as quoted in the survey, since a counting rule is a restricted
  ordinary rule; L(3) = 2 itself does not follow from it. That paper was not
  obtained, because the archive that hosts it refuses automated access.
- **A pattern that does not continue.** The fastest five-state times above
  equal ⌊3(n − 1)/2⌋ on all seven listed lengths, and an earlier draft of
  this page called that worth testing. It fails at n = 9: a reviewer found a
  five-state rule that fires a line of 9 cells at time 10, where the formula
  gives 12 (the rule, as d(x, k) for x = Q, G, U1, U2 and k = 0, 1, 2, is
  `0 3 1 0 1 3 4 0 3 2 3 0` with 0 = Q, 1 = G, 2 = F; the organiser simulated
  it and gets 10). The reviewer's enumeration gives 10 and 13 as the fastest
  times for n = 9 and 10; that those are the minima has a single source.
- **Errors in the chain's question.** Its belief that a synchronising rule
  fires at time 2n − 2 or later is false in this model (a rule that only has
  to work up to some length can fire earlier); the referee counts 2,487,460
  counterexample pairs (rule, length) among five-state rules. Its guess "at most 6" is right but
  not tight.
- **Small points.** Solver A's test of its Lemma 2 is empty at three states
  and never reaches the equal-time case at four; the referee's five-state
  test (549,534 pairs, 320 of them equal-time) is a real one. The referee's
  brute-force counts are in its report but not saved as output files, and
  `verifier/run_ord.sh` runs a binary named `ref3`, which is built from
  `ref2.cpp`. Solver B's remark about when four-state
  rules fire length 3 cites the wrong example. The referee found and fixed a
  counting bug in one of its own programs before the seven-state run, caught
  by the check that the cases add up.

## Review

Before publication the claims went to five reviewers: GPT, Grok and Gemini,
and two Claude instances (one of them Claude Opus). The Grok seat failed
twice with a service error and produced no report, so there are four
reports from three model families. They confirm the example rule (three
reviewers simulated it by hand or with their own code), the brute-force
programs against the definition, the soundness of the backtracking searches,
and the hand proofs. One Claude seat wrote its own enumerator and reproduced
the three-, four- and five-state tables exactly. Their findings on this page,
applied above:

- The "coincidence worth testing" was already false at n = 9 (Claude seat).
- I wrote that part of the three-state result follows from a 2008 paper; the
  part that follows is a statement the page had not made (both Claude seats).
- "Could not fail" was wrong for four states (GPT, both Claude seats).
- A proof was credited to both solvers when one of them proves a special
  case; the literature search did find one count-based paper; "three search
  orders" should be two; the counterexample count is of pairs.

The corrected wording has not been through a second round.

The six-state enumeration of 21 November went to four reviewers (GPT, Grok,
Gemini, one Claude seat). All four confirmed the simulator against the
definition, the cycle detection, the coverage of all 6¹⁴ rules and the
reading of the histogram. The Claude seat wrote its own simulator with a
different cycle detection: it agrees with the program on every rule with
three, four and five states, on four whole six-state chunks (1.45 × 10⁹
rules) and on all 2,502 recorded rules. GPT rated two command-line
validation gaps critical; they cannot affect the recorded runs and are
listed above. Other findings, applied: the agreement with solver A's
histogram was understated; the three- and four-state runs had no output in
the folder (added); "without reading the earlier programs" was stated as
fact; "an error could only make it too small" was wrong for seven states
(Grok). A second-round reviewer (one Claude instance) then checked the edited page against the files. It found that the page understated the agreement with the earlier work (the list comparison above) and three smaller points, now applied.

## Files

- Reviewers' reports of 21 November, archived verbatim with absolute paths shortened: `enum6/review/` (four seats: one Claude seat). Each `review*/` folder holds the brief the reviewers were given, the reports of the GPT, Grok and Gemini seats with a machine-made summary, the reports of the Claude seats (one of the two ran on Claude Opus), and the organiser's note of what was done with the findings; `review-second-round-1.md` is the later recheck of this page by a single Claude instance. The reports were written about the files as they stood before the findings were applied, so their line numbers and quotations may not match the current files.
- `enum6/REPORT.md`: the follow-up instance's final report, archived from the session transcript on 21 November, verbatim with paths shortened.
- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: enumeration and search programs, witnesses, the
  list of all 1,244 five-state rules, proofs.
- `enum6/`: the follow-up of 21 November: the complete enumeration of six-state rules (brief, program, chunk outputs, checks, the list of all 336
  optimal rules).
- `verifier/`: the referee's enumeration, its searches without symmetry
  breaking, and its checker.
- `prior-art.md`: the literature search. The downloaded papers are not
  included.

Binaries are left out.
