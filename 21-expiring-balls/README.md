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
for and comes from putting the two solvers' results together. No formula is proved; k + 2^(k−1) fits the four values. For k = 5 a follow-up of 21 November shows that 21 bins suffice (the value the fit predicts; the earlier bound was 28), so N(5) is between 12 (N(4) = 12, and N cannot decrease as k grows) and 21; in general it proves ⌊(k+1)²/4⌋ + 1 ≤ N(k) ≤ 5·2^(k−3) + 1 for k ≥ 5. The gap between quadratic and exponential is open.

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
| **g(11) = 3, so N(4) ≥ 12** | computed by solver B's program (run by solver B and rerun by the referee) and by the referee's separately written program, which find a winning set of 974,272,116 non-final states from 3,183,950 starting positions. Since 21 November there is also a certificate: a list of 3,334,554 positions that defines a safe set for the rule, and a separate checker that accepts it by examining a family of positions shown, in a two-page hand proof, to be enough. The two programs' winning sets reduce to the same certificate byte for byte | `solver-b/back2.cpp`, `verifier/vb.cpp`, `certificate/` |
| **N(4) ≤ 12** | an explicit strategy written out by solver A; replayed against every answer of the rule by solver A's program and by a program the referee wrote from the prose | `solver-a/proofs.txt` P6, `verifier/replay12.py` |
| So N(4) = 12 | from the two rows above; stated by neither solver | |
| N(k) ≥ 2k and N(k) ≤ 2^k for all k | proved by hand by both solvers; read by the referee | `solver-a/proofs.txt` P1, P2 |
| N(k) ≤ N(k−1) + 2^(k−1), hence N(k) ≤ 2^k − 1 for every k ≥ 3 | one-line argument supplied by a reviewer (Claude Opus) and checked by the organiser: force load k − 1 on N(k−1) bins, build a second bin of load k − 1 on 2^(k−1) fresh bins by merging without expiry, and offer the two against each other | this page |
| Without expiry the value is ⌊log₂ n⌋ | proved by hand by solver B; known (see below) | `solver-b/proofs.txt` P3 |
| If the adversary may remove any ball it likes, 1 + k(k+1)/2 bins suffice for load k, and the least number is 2, 4, 7, 11 for k = 1, …, 4 | upper bound proved by hand by both solvers; the values computed by solver A and, up to 10 bins, by the referee | `solver-a/proofs.txt` P4 |
| The rule "give the ball to the bin with the smaller load; on a tie, to the bin that appears earlier in the age-ordered list of balls, untouched bins last" is optimal for n ≤ 8 | computed by solver A and the referee; solver A also reports n = 9, not rerun | `solver-a/greedy.cpp` |
| **N(5) ≤ 21** | an explicit Adversary strategy as a table of 3,676 positions with one move each, accepted by a checker written from the definition that shares no code with the generator (it follows every answer of the rule and verifies that no position can repeat); also proved by hand (Theorem U). One follow-up instance, 21 November; the organiser reran the checker (accept on 21 bins; reject on 20 bins, with target 6, and with one line removed); five reviewers confirmed the checker against the definition, and two replayed the table with their own programs, reaching the same 3,676 positions (see Review) | `general/adv_k5_n21.txt`, `general/check_adv.py`, `general/organiser_check.txt` |
| **N(k) ≤ 5·2^(k−3) + 1 for every k ≥ 5** (21, 41, 81, …; the recursion above gives 28, 60, 124, …) | proved by hand by the same instance (three rounds of "feeding" a bin from bins of loads j, …, k − 1 standing in separate components, then a merge on fresh bins); for k = 6 the strategy was written out on 41 bins and accepted by the checker (a table of 2,689,236 positions, not included, so this run can be repeated only by regenerating it with `general/gen.py`) | `general/PROOFS.md` Theorem U, `general/out_check_k6_n41.txt` |
| **N(k) ≥ ⌊(k+1)²/4⌋ + 1 for every k** (13, 17, 21 for k = 6, 7, 8; better than 2k from k = 6). The same rule is safe even if the adversary may remove any ball, so the any-removal number is between about k²/4 and k²/2 | proved by hand by the same instance; the method is, according to the instance, the "Robin Hood" argument known from load balancing with departures, adapted to forests (no source was checked for this, and the exact constant was not looked for in the literature); it does not use the order of expiry; five reviewers traced it and found no gap | `general/PROOFS.md` Theorem L |
| N(k) in general | open: between ⌊(k+1)²/4⌋ + 1 and 5·2^(k−3) + 1, that is, between quadratic and about (5/8)·2^k. k + 2^(k−1) fits k ≤ 4, is attained as an upper bound at k = 5, and is not reached by the new strategy for k ≥ 6 (41 against 38); a lower bound that uses the order of expiry is missing | |

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
clean (k−1)-star on k bins plus 2^(k−1) fresh ones. For k = 5 the follow-up of 21 November forces a clean 4-star within 21 bins; for k ≥ 6 nobody has shown that a clean (k−1)-star can be forced within k + 2^(k−1) bins, nor that the adversary cannot do better.

If the adversary may remove any ball it likes, it needs no threat to tidy
up, and 11 bins suffice for load 4. So for k = 4 the order of expiry costs
the adversary one bin: 12 against 11. The 12 rests on solver A's explicit
strategy (12 suffice) and on the certificate described below (11 do not).
The 11 rests on the hand-proved bound 1 + k(k+1)/2 (11 suffice when any
ball may be removed) and on a safe set for 10 bins in that game, produced by
solver A and accepted by the referee's checker (10 do not). For k ≤ 3 the
order of expiry costs nothing.

## What to distrust

- **Everything is by one model family.** The upper bounds (7 and 12 bins, and since 21 November 21 bins for k = 5, as a table) are explicit strategies, the first two short enough to follow by hand; the lower bounds (6
  and 11 bins are not enough) are computations.
- **The general bounds are hours old and have one author.** N(5) ≤ 21
  does not depend on the hand proof: the table and the checker of about 150 lines suffice, if the checker is right. It treats positions that differ by
  renaming bins as the same and untouched bins as interchangeable, which
  its header justifies; it was tested on damaged tables (for the 12-bin table every one of its 101 lines: every deletion and every swapped move rejected, and the 21 accepted altered offers all only rename an untouched bin; for the 21-bin table a random sample of 150 lines, with every deletion and swap rejected, and a sample of 400 altered offers, of which 297 were rejected and the 103 accepted all rename an untouched bin) and reproduces N(1), N(2), N(3) through a small
  exhaustive solver that shares its conventions. It was not compared with
  the repository's other checkers, which use another file format. For
  k ≥ 7 the upper bound is the hand proof alone. The lower bound says
  nothing specific to first-in-first-out expiry, so it cannot explain the fast growth of the first four values. `general/PROOFS.md` refers to a `REPORT.md` that was not in the folder at the time of review (the instance's report, archived since as `general/REPORT.md`), and two output files in `general/` (`out_check_small.txt`, `out_check_k6_n41.txt`) contain REJECT lines, and `out_rh_test.txt` two assertion failures, without the commands that produced them.
- **N(4) ≥ 12 now has a certificate, of a compressed kind.** At first it
  rested on two programs whose counts agreed; they store the winning set in
  the same compressed way, and what they stored differed in size (3,722,933
  and 3,723,730 entries), so the sets had never been compared. A follow-up
  instance reduced both to their minimal positions, which do not depend on
  the order of the search: the two files are identical (3,334,554 lines
  each, with the same sha256; the checksums are in
  `certificate/out/organiser_sha256_and_lines.txt`, recorded by the
  organiser, and the same holds at 10 bins). Reviewers reduced the small
  cases themselves and got the same. The follow-up instance then wrote a checker, sharing no code with the
  two programs, that takes such a list as untrusted input and accepts only
  if the positions with none of the listed positions among their newest
  balls form a safe set for the rule. The full set is far too large to walk,
  so the checker examines three finite families of positions, and
  `certificate/PROOF.md` proves that these are enough. On 11 bins it accepts
  in 40 seconds; a second mode that walks all 974,272,116 winning positions,
  and needs only the short argument of the proof's section 5 in place of the
  reduction to three families, also accepts (in 9.5 minutes). It rejects every certificate tried for
  7 bins and load 3 (where the adversary wins), and every damaged 11-bin
  certificate tried except one in which a redundant line was added, which it
  rightly accepts. What is still trusted: the two-page proof, which so far
  only models have read; that the 218-line checker implements it; and two
  pruning bounds proved in the same file, one of them shared with the two
  programs, switched off for comparison only up to 9 bins. The checker was
  tested against literal checks of the definition: its own literal mode,
  which shares its state-handling code, on 1,560 certificates (intact,
  damaged and random) up to 8 bins by the follow-up's account (the log does
  not record which modes ran on each batch) and on the intact one at 9; and a separate Python program sharing
  nothing with it, which the organiser ran on 6 bins (accept), on a bad
  7-bin certificate (reject) and on 8 bins (accept, all 4,358,193 safe
  positions checked directly); see
  `certificate/out/organiser_explicit.log`. None of this is a proof that the
  checker is right, and no literal check reaches 10 or 11 bins. By the
  organiser's own account, with the output in
  `certificate/out/organiser_rerun.log`, it recompiled the checker and reran
  it on the 11-bin certificate (accepted) and on a 9-bin certificate it
  damaged itself (rejected).
- **Rough edges in the follow-up's files.** `certificate/out/difftest.log`
  contains 30 lines marked MISMATCH from a first comparison script that
  compared differently formatted lines; the valid comparison is
  `count_check.txt`, which reviewers recomputed. `PROOF.md` refers to a
  `REPORT.md` that was not in the folder at the time of review (archived since as `certificate/REPORT.md`) and says the separate Python program
  took part in the damaged-certificate tests, which it did not. The checker
  can be run in parts; each part prints ACCEPT on its own and only the first
  checks the starting positions, so a split run is valid only if every part
  accepts (the runs recorded here use a single part). It crashes if asked
  for zero parts, and any value of its `NOPRUNE` switch turns pruning off. The 90 MB certificate is not in the repository; it can be
  regenerated from `certificate/gen/` (run with `DUMP=file`, then
  `certtool.py minimize`), though no command line is recorded.
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

This section's first part predates `certificate/`; "the certificate checker"
in it is the referee's checker for the small explicit certificates.

Before publication the claims went to five reviewers from four model
families: GPT, Grok and Gemini, and two Claude instances (one of them Claude
Opus). All five confirmed the 7-bin strategy and the 12-bin strategy after
following them by hand, the certificate checker, and the bounds 2k ≤ N(k) ≤ 2^k. On
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

The certificate for 11 bins went to the same five reviewers, with most of
the brief on the two-page proof that the reduced check is enough. All five
confirmed every step of it (the three cases, their exhaustiveness, the two
pruning bounds, the edge cases); the four who read the checker closely
found no place where it departs from the proof in substance. One Claude
seat tested the theorem itself on about 7,500 random lists for small sizes
and found no case where the three families pass and the literal properties
fail; the other reduced the programs' small outputs with its own tool and
checked a certificate with its own literal checker. Their findings, applied
above:

- I wrote that "the 12" rests on the certificate and dropped a hedge; the
  certificate covers one half of the 12, and each number in that sentence
  now has its sources (both Claude seats, Grok, Gemini).
- The byte-for-byte identity and the checksum had no log in the folder
  (four seats); added.
- "Rejects damaged 11-bin certificates" hid one rightly accepted case.
- A log with spurious MISMATCH lines, a reference to a missing file, and a
  claim about the separate literal checker that no log supported (all
  five); the first two are now explained and the third replaced by actual
  runs.
- GPT rated a crash on a zero "parts" argument critical; it is a
  command-line edge case that no recorded run uses, listed above with the
  split-run caveat.

A second-round reviewer (one Claude instance) then checked the edited page
against the files, recomputing the figures and reducing the small outputs
itself. It found no false claim; its corrections (what the second mode
still relies on, the make-up of the 1,560 test certificates, which
certificate the organiser damaged, the regeneration recipe) are applied.

The general bounds went to the same five reviewers. All five confirmed
the strategy checker against the definition (legality of offers, expiry of
the oldest ball, both answers followed, the test that no position repeats
and why it is needed, the identification of positions up to renaming),
Theorem U with its bin counts (21 at k = 5, 41 at k = 6) and Theorem L.
Both Claude seats replayed the 21-bin table with their own programs, with
different state representations: 3,676 positions, always ending in load 5,
failing on 20 bins. Their findings, applied above: my "line removed" rerun
was not in the log (added); the damage tests were exhaustive only for the
12-bin table (all five); a sentence about clean stars was stale; where the
lower end 12 comes from; "known argument" claimed more than was checked
(GPT); the size of the omitted table could not be verified. A second-round reviewer (one
Claude instance) then rechecked the edited sections against the files, rerunning the
checker and recomputing the numbers; it found no false or unsupported claim, and its
three wording points are applied.

## Files

- `certificate/REPORT.md`, `general/REPORT.md`: the two follow-up instances' final reports, archived from the session transcript on 21 November, verbatim with paths shortened.
- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: game solvers, certificates, checkers, proofs.
- `verifier/`: the referee's three solvers, certificate checker and the
  replay of the 12-bin strategy.
- `general/`: the second follow-up of 21 November (bounds for general k and
  N(5) ≤ 21): proofs, the strategy generator, the checker, tables for k ≤ 5,
  tests.
- `certificate/`: the follow-up for 11 bins: `PROOF.md`, the checker
  `check.cpp`, the tools that reduce a program's output to a certificate and
  test the checker, small certificates for 6, 8 and 9 bins, two 7-bin files
  that must be rejected, the follow-up's brief, and logs. The
  11-bin certificate (90 MB) and the 10-bin one (1.3 MB) are left out;
  `gen/` holds copies of the two search programs with a dump statement
  added, from which `certtool.py minimize` regenerates them.
- `prior-art.md`: the literature search. The downloaded papers are not
  included.

Binaries are left out.
