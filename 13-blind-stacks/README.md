# Experiment 13: sorting blind through small stacks

Numbers arrive in an unknown order and must leave in increasing order,
passing through a row of stacks that hold two items each. The sorter never
sees a value. It can only try a move and be told whether it happened. How
many arrival orders can one strategy get right?

**Short answer, for two stacks: N(n) = 5·N(n−1) − 5·N(n−2) for n ≥ 5**,
starting 1, 2, 6, 21, 75, 270, 975, …, so the count grows by a factor of
(5 + √5)/2 ≈ 3.618 per extra item. With k stacks every order can be handled
exactly when n ≤ k + 1. The recurrence is proved with the help of a finite
computation, two different ways, on top of a hand-proved reduction that was
tested against the literal rules up to 10 items. The chain guessed
exponential growth with a rational generating function, which is right.

## The game

There are k stacks S_1 … S_k in a row, each holding at most 2 items, all
empty. The numbers 1 … n arrive in an order fixed in advance. The sorter
knows k and n and nothing else. It attempts moves:

- **IN**: push the next arrival onto S_1 (needs room, and something still to
  arrive).
- **SHIFT i**: pop the top of S_i and push it onto S_(i+1) (needs room).
- **OUT i**: pop the top of S_i and send it out. Legal only if that item is
  the smallest number not yet output.

A legal attempt is carried out and answered "done"; an illegal one changes
nothing and is answered "refused". Items never move backward. A strategy
beats an arrival order if it gets all n numbers out. N(k, n) is the largest
number of arrival orders a single strategy beats.

The only way to learn anything about a value is to try to output it.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): find
N(2, n) for all n.

## Results

| Claim | Status | Where |
|---|---|---|
| The sorter's knowledge is captured by which items in the stacks have been refused since the last successful output, and how many items are still to arrive; N(k, n) is the value of a recursion over those "flag states" | proved by hand (solver B's version is the fuller one); referee read both and compared every state, not only the answer, with a solver working from the literal rules: 1.9 million states for k = 2, n = 10, no mismatch (that solver keys its memo on a 128-bit hash of each set, not the set itself) | `solver-b/proofs.txt` Lemma 0, `solver-a/proofs.txt` Lemma 1 |
| N(k, n) = n! exactly when n ≤ k + 1 | proved by hand, twice, with the same adversary; referee read both | `solver-b/proofs.txt` Theorems 1, 2; `solver-a/proofs.txt` Theorem 2 |
| **N(2, n) = 5·N(2, n−1) − 5·N(2, n−2) for all n ≥ 5, with N(2, 1 … 4) = 1, 2, 6, 21** | proved with a finite computation, twice by different finite checks, both conditional on the first row; referee read both arguments and re-ran both checks with its own code | `solver-a/proofs.txt` Theorem 3, `solver-b/proofs.txt` Theorem 3 |
| Closed form for n ≥ 3: N(2, 2j+1) = 3·5^(j−1)·F_(2j+1) for j ≥ 1 and N(2, 2j) = 3·5^(j−2)·L_(2j) for j ≥ 2 (Fibonacci and Lucas numbers), with N(2, 1) = 1 and N(2, 2) = 2 apart; generating function x(1 − 3x + x² + x³)/(1 − 5x + 5x²) | follows from the recurrence; checked to n = 300 by the referee | `solver-b/proofs.txt`, `verifier/flag_out.txt` |
| N(1, n) = 2^(n−1) | solver A's script runs the same finite check for one stack, but the induction is not written out; solver B calls it a sketch; computed to n = 10 from the literal rules | `solver-a/proofs.txt` |
| Table below | computed by both solvers' recursions and the referee's; from the literal rules by the referee for k = 1, 2 up to n = 10, k = 3 up to n = 8, k = 4 for n = 5, 6, 7 and k = 5 for n = 6, 7 | `reports/` |
| N(k, k+2) = (k+2)! − (k+1) | computed for k ≤ 8; not proved | |
| N(3, n) satisfies a(n) = 11a(n−1) − 40a(n−2) + 58a(n−3) − 30a(n−4) for n ≥ 9 | fitted and checked to n = 70; not proved | |
| N(4, n) satisfies a recurrence of order 10 for n ≥ 17 | fitted and checked to n = 90; not proved | `solver-b/proofs.txt` |

| k \ n | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256 | 512 |
| 2 | 1 | 2 | 6 | 21 | 75 | 270 | 975 | 3525 | 12750 | 46125 |
| 3 | 1 | 2 | 6 | 24 | 116 | 604 | 3188 | 16948 | 90460 | 483924 |
| 4 | 1 | 2 | 6 | 24 | 120 | 715 | 4781 | 33419 | 235814 | 1673633 |
| 5 | 1 | 2 | 6 | 24 | 120 | 720 | 5034 | 39808 | 340774 | 3004976 |

## Why n ≤ k + 1 is the limit for getting everything right

**Enough room.** Keep one item in each stack, shifting items to the right so
that the first stack is free before each arrival. Then every item is on top
of its stack and the sorter can try them all; the one that goes out was the
smallest. With n = k + 1, bring k items in and try them all. If all are
refused, the smallest is the one still to arrive, and it can be brought in
and sent straight out.

**One item too many.** The adversary makes 1 the last arrival. If it is ever
output, at least k + 1 items then sit in k stacks, so some stack holds two.
(If there is no room to bring it in, the sorter has already failed.) Take the shortest stretch of stacks at the end of the row that together
hold more items than the stretch has stacks. Its first stack holds two items; the
adversary declares the buried one to be the next smallest. To reach it the
sorter must shift its cover forward, which leaves the remaining, shorter
stretch holding at least one item more than it has stacks. Repeating this ends at the last stack with
two items, the buried one the smallest and its cover with nowhere to go. The
adversary may decide values this late because the sorter is deterministic:
what it does while a given value is the smallest depends only on which item
carries that value, and every refusal it collected is forgotten as soon as
that value goes out.

## How the recurrence is proved

For two stacks there are 49 flag states. With u items still to arrive, each
state has a value: the number of consistent arrival orders the best play
beats from there.

Both solvers fix one rule ("in this state, attempt this move") that does not
depend on u. What has to be shown is that the fixed rule stays the best one
for every u, and neither solver argues "it was best in all the cases we
looked at".

- Solver A's induction carries two things forward together: every move's
  value obeys the recurrence, and the margin by which the fixed move beats
  each other move is non-negative and at least (5 − √5)/2 times the previous
  margin. The recurrence then gives the same at the next u. The base is a
  window of four values of u, checked exactly over all 49 states and all
  moves that the reduction leaves (it has the sorter probe every top first).
- Solver B notes that following a fixed rule makes the 49 values at u a
  fixed linear function of those at u − 1, so each of the 36 margins is a
  sequence obeying a linear recurrence of order at most 49. It checks in
  exact integers that x² − 5x + 5 annihilates each over 121 consecutive terms
  (49 would do), that each margin is non-negative for the first nine values,
  and, exactly in Q(√5), the one inequality between the eighth and ninth that
  keeps them non-negative from there on.

## What to distrust

- **Everything is by one model family.** Nothing is machine-checked as a
  proof, though the finite checks are small exact computations that three
  programs ran.
- **The headline is conditional on the reduction to flag states.** That
  lemma is proved by hand and has been compared with the literal rules up to
  n = 10 for two stacks. Solver A's write-up of it is thin: it proves that
  probing costs nothing and asserts the description of the consistent orders
  in one sentence; solver B's Lemma 0 is the one to read.
- **The number sequence is known as a sequence.** For n ≥ 3, N(2, n) is three
  times the term of OEIS A052936 two places earlier (N(2, n) = 3·A052936(n − 2));
  that sequence is the expansion of (1 − x)(1 − 2x)/(1 − 5x + 5x²), which
  has the same recurrence and Fibonacci/Lucas formulas. That page gives no
  sorting interpretation. The match was found by the literature search
  comparing terms.
- **The model was not found in print.** Sorting through bounded stacks in
  series is an established topic with full information (Atkinson, Livesey,
  Tulley 1997 and later). A sorter that sees no values and learns only from
  refused outputs was not found in the search, which read one source in full
  and could not open the paper most likely to tabulate this network.
- **Small errors.** A printed numerator in solver A's `table_out.txt` uses a
  different convention from its report; solver B's strategy files carry no
  expected count; the referee's literal-rule programs have fixed-size arrays
  with no bounds check (its runs stay inside them). Loose statements in the
  two proof files are listed in a note at the top of each.
- **One certificate over 300 kB** (`cert_k5_n7.txt`) is left out.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files. All five confirmed
the reduction to flag states, the strategy for n ≤ k + 1, both
computer-assisted proofs of the recurrence, and the table. Four confirmed the
adversary for n ≥ k + 2 as written; Grok marked it refuted on one word
("exactly j items" where more can arrive; "at least j" is what the induction
needs and what holds). Their findings, applied above:

- The closed forms were printed without their ranges and are false at
  n = 1, 2.
- "Three times A052936" dropped the shift by two.
- My account of the recurrence proof attributed solver B's matrix view to
  both solvers and understated what each finite check covers.
- A closing remark in solver A's Theorem 3 claims the recurrence for all 49
  states from u = 5; it is checked from u = 6, and at u = 5 for the starting
  state, which is all the theorem needs.
- GPT rated the missing bounds checks in the referee's program as critical;
  they are a code-quality point, since no run leaves the bounds. Gemini
  reported the `reports/` folder missing; it is present.

The corrected wording has not been through a second round.

## Files

- `review/`: the review of this experiment before publication (16 to 20 November), archived on 21 November verbatim with absolute paths shortened: the brief given to the reviewers, each seat's report, the machine-made summary, and the organiser's note of what was done with the findings. The reports were written about the files as they stood before the findings were applied, so their line numbers and quotations may not match the current files.
- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: recursions, literal-rule solvers, strategy
  tables, certificates, the finite checks, proofs.
- `verifier/`: the referee's literal-rule solver, state-by-state comparison
  and re-runs of both finite checks.
- `prior-art.md`: the literature search.

Binaries are left out.
