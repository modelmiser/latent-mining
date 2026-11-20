# Experiment 21: two bins on offer, and the oldest ball expires

Balls are placed one at a time into bins. For each ball an adversary offers
two bins and a rule must put the ball into one of them, for good. The two
bins offered must lie in different components of the graph of earlier offers,
so the offers form a forest. The adversary may also make the oldest ball still present expire.
How many bins does the adversary need to force some bin to hold k balls?

**Short answer: 2, 4, 7 and 12 bins for k = 1, 2, 3, 4.** Without expiry the
answer is 2^k (2, 4, 8, 16), which is known. So letting the oldest ball
expire helps the adversary, for every k from 3 on (N(k) ≤ 2^k − 1; see
below). The chain asked for the case
k = 3 and bet on 8; the answer is 7. The value 12 for k = 4 was not asked
for and comes from putting the two solvers' results together. No formula is
proved; k + 2^(k−1) fits the four values.

This is a relative of [experiment 14](../14-forest-orientation/): placing a
ball offered to bins u and v is orienting the edge {u, v} of a growing
forest. There the edges could be reversed at a cost and nothing was removed;
here nothing is ever reversed and old edges expire.

## The game

There are n bins. A ball remembers the pair of bins it was offered to, the
bin that received it and when it arrived. The offer graph has the bins as
vertices and one edge per ball present.

The adversary repeatedly does one of two things. It may offer a pair {u, v}
of bins that lie in different components of the offer graph; the rule must
at once choose which of the two receives the ball. Or it may expire: the
oldest ball present is removed. The adversary does not choose which ball
leaves, and the rule never moves a ball. Both players see everything.

The adversary achieves load L if at some moment some bin holds L balls. g(n)
is the largest load it can achieve on n bins against every rule, and N(k) is
the least n with g(n) ≥ k.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md),
formal text in [chain/3-question.txt](chain/3-question.txt)): find N(3).

## Results

| Claim | Status | Where |
|---|---|---|
| g(n) = 1, 1, 2, 2, 2, 3, 3 for n = 2, …, 8; so N(1) = 2, N(2) = 4, **N(3) = 7** | computed by both solvers and three ways by the referee (forward search, backward search, and a direct implementation up to n = 5); for N(3), each solver produced small certificates for both directions, accepted by its own checker and by the referee's | `solver-a/adv_n7_L3.txt`, `solver-a/rule_n6_L3.txt`, `reports/referee.md` |
| g(9) = g(10) = 3 | computed by solver B and the referee, with equal sizes of the adversary's winning set; also by solver A through the any-removal game, where the rule already keeps loads below 4 on 10 bins | `reports/referee.md` |
| **g(11) = 3, so N(4) ≥ 12** | computed only: by solver B's program (run by solver B and rerun by the referee) and by the referee's separately written program, which finds a winning set of the same size (974,272,116 non-final states, the empty position outside it) from the same 3,183,950 starting positions. Only these counts were compared, not the sets | `solver-b/back2.cpp`, `verifier/vb.cpp` |
| **N(4) ≤ 12** | an explicit strategy written out by solver A; replayed against every answer of the rule by solver A's program and by a program the referee wrote from the prose | `solver-a/proofs.txt` P6, `verifier/replay12.py` |
| So N(4) = 12 | from the two rows above; stated by neither solver | |
| N(k) ≥ 2k and N(k) ≤ 2^k for all k | proved by hand by both solvers; read by the referee | `solver-a/proofs.txt` P1, P2 |
| N(k) ≤ N(k−1) + 2^(k−1), hence N(k) ≤ 2^k − 1 for every k ≥ 3 | one-line argument supplied by a reviewer (Claude Opus) and checked by the organiser: force load k − 1 on N(k−1) bins, build a second bin of load k − 1 on 2^(k−1) fresh bins by merging without expiry, and offer the two against each other | this page |
| Without expiry the value is ⌊log₂ n⌋ | proved by hand by solver B; known (see below) | `solver-b/proofs.txt` P3 |
| If the adversary may remove any ball it likes, 1 + k(k+1)/2 bins suffice for load k, and the least number is 2, 4, 7, 11 for k = 1, …, 4 | upper bound proved by hand by both solvers; the values computed by solver A and, up to 10 bins, by the referee | `solver-a/proofs.txt` P4 |
| The rule "give the ball to the bin with the smaller load; on a tie, to the bin that appears earlier in the age-ordered list of balls, untouched bins last" is optimal for n ≤ 8 | computed by solver A and the referee; solver A also reports n = 9, not rerun | `solver-a/greedy.cpp` |
| N(k) in general | open; k + 2^(k−1) fits k ≤ 4 and is a guess | |

## How the adversary uses expiry

This follows `solver-a/proofs.txt` P5 and P6.

Without expiry the adversary can do no better than merge: to force load k it
builds two bins of load k − 1 in separate parts and offers them against each
other, which doubles the bins needed each time.

Expiry lets it tidy up. Call a component of the offer graph a clean j-star
if it consists of one bin holding j balls and j bins holding none. With 7 bins:

1. On 4 bins, force a bin x of load 2 (two offers, then the two receivers
   against each other).
2. Take two untouched bins and offer them; say f receives. Now offer {x, f}.
   If x took the ball it would hold 3, so the rule must give it to f. Now f
   holds the two youngest balls.
3. Expire everything older. What is left is a clean 2-star at f on 3 bins,
   and the other 4 bins are untouched.
4. On those 4 bins force a second bin of load 2 without expiring, and offer
   it against f.

The point of step 2 is that a threat forces the rule to put the newest balls
where the adversary wants them, so that first-in-first-out expiry removes
exactly the clutter. (If x already holds the two youngest balls after step
1, one expiry gives a clean 2-star at once; the steps above work in that case
too.) The 12-bin strategy for load 4 uses the same threat once or twice more,
depending on the rule's answers, to obtain a clean 3-star on 4 bins, then builds load 3 on the 8
untouched bins by plain merging. That is where k + 2^(k−1) comes from: a
clean (k−1)-star on k bins plus 2^(k−1) fresh ones. Nobody has shown that a
clean (k−1)-star can always be forced within that many bins, nor that the
adversary cannot do better.

If the adversary may remove any ball it likes, it needs no threat to tidy
up, and 11 bins suffice for load 4. So for k = 4, if the computed value for
11 bins is right, the order of expiry costs the adversary one bin (12 against
11); for k ≤ 3 it costs nothing.

## What to distrust

- **Everything is by one model family.** The upper bounds (7 and 12 bins)
  are explicit strategies that can be followed by hand; the lower bounds (6
  and 11 bins are not enough) are computations.
- **N(4) ≥ 12 has no certificate.** It rests on two programs whose counts
  agree exactly. They were written separately, but both store the winning
  set in the same compressed way and use the same bound to prune their
  starting states. The compressed sets they store differ in size (3,722,933
  and 3,723,730 entries); the reviewers who read the code say that is
  expected, because what is stored depends on the order of the search, but
  it also means the two sets were never compared state by state. The referee read solver B's program line by line and
  found no error, and showed its own program can also report a win where one
  exists. It puts its confidence in N(4) = 12 at about 97 per cent.
- **N(3) = 7 is on firmer ground:** each direction has a certificate of a few
  dozen or a few thousand states that a short separate program checks.
- **What is known.** The literature search read the relevant sections of
  three papers and the introduction of a fourth. The game without expiry,
  with its value of order log n, is in Ozdayi (arXiv:2001.03015, Lemma 2.1);
  the matching upper bound ⌈log n⌉ is also the warm-up algorithm of Bender,
  Kopelowitz, Kuszmaul, Porat and Stein (2021, introduction only read). For
  jobs that depart at the adversary's choice, a load of order √m on m
  machines is tight even when each job has two admissible machines (Armon,
  Azar, Epstein and Regev, and earlier work it cites), which matches the
  order of 1 + k(k+1)/2 here. The search found nothing on departures in
  arrival order, on N(3) = 7 or N(4) = 12, or on the tie-breaking rule.
  Several older papers on load balancing with departures were paywalled and
  not read.
- **The formula is a guess from four values.** The rival 1 + k(k+1)/2 fits
  the first three and fails at the fourth.
- **Errors in the chain's question.** Its bet N(3) = 8 is wrong. Its
  unverified small values are right.
- **Small points.** The solvers count 32 and 33 states in their strategy
  files for 7 bins because they include final positions. Solver A lists
  values for the any-removal game at 11 to 13 bins as computed; only those up
  to 10 bins have a certificate. Solver A reported three of its processes
  alive at once for about a minute.
- **Code robustness.** Two reviewers rated these critical; none affects a
  reported number. The backward-search programs use fixed arrays of 12
  entries, so they would overrun for 13 or 14 bins and more (the runs here
  stop at 11). The referee's program needs a larger table than its default
  for 11 bins and stops with "table full" otherwise; the recorded run used a
  larger one. Solver A's `verify12.py` opens a file by a relative path and
  must be run from its own folder. `solver-a/x` is a stray output file.
- **Three certificates over 300 kB are left out** (the rule's safe sets for
  8 bins from both solvers, 64 MB and 48 MB, and the any-removal safe set for
  10 bins). The programs that regenerate them are included.

## Review

Before publication the claims went to five reviewers from four model
families: GPT, Grok and Gemini, and two Claude instances (one of them Claude
Opus). All five confirmed the 7-bin strategy and the 12-bin strategy after
following them by hand, the certificate checker, and the general bounds. On
the 11-bin computation four accepted the method; Grok marked it refuted
because the two programs' stored sets differ in size although their counts
agree. Their findings on this page, applied above:

- I wrote that the two programs "find the same winning set" and "agree to
  the last state"; only counts were compared (four seats).
- "The order of expiry costs exactly one bin" was unhedged although it
  depends on the uncertified computation (Claude Opus).
- "Expiry helps from k = 3 on" had no proof beyond k = 4; a reviewer
  supplied one.
- The prior-art paragraph credited a paper with more than the search report
  says, and miscounted what was read.
- The code-robustness points listed above (GPT, Gemini).

The corrected wording has not been through a second round.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: game solvers, certificates, checkers, proofs.
- `verifier/`: the referee's three solvers, certificate checker and the
  replay of the 12-bin strategy.
- `prior-art.md`: the literature search. The downloaded papers are not
  included.

Binaries are left out.
