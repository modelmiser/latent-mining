# Experiment 25: chips arrive every round, but only one vertex may fire

Chips are dropped one per round onto points of a line, following a list fixed
in advance. A point holding two chips may fire, sending one chip to each
neighbour. An opponent who knows the whole list decides the firing, but may
fire at most one point per round. How short a list forces some point to hold
h chips, whatever the opponent does?

**Short answer: 5h rounds minus a constant, and no list beats dropping every
chip on the same point by more than a fixed number of rounds.** If G(h) is
the length of the shortest winning list, then
5h − 176 ≤ G(h) ≤ 5h − 6 for every h ≥ 2 (proved by hand, the lower bound
by one instance on 21 November; not machine-checked). For moderate h a second lower bound from the same day is stronger: a list of length m loses if m/5 + 5.2 + 2·H_3m < h (H the harmonic numbers), which gives for example G(100) ≥ 398 and is the best of the three bounds from h = 28 to somewhere near 10⁵; below h = 28 the simple bound 2h − 1 is the best. So no list beats the single-point list by more than 170 rounds, whatever h is; whether any list beats it at all for h ≥ 6 is open.
The lower bound with a constant was found on 21 November by a follow-up
instance; the first version of this page had 5h − 10√(5h) − 72. The small values are G(1), …, G(5) = 1, 2, 5, 10, 15. The chain guessed
that the limit would be below √24 ≈ 4.9 and that mixing two neighbouring
points would keep a constant-factor advantage over a single point; both
guesses are wrong. Mixing helps at h = 3 and not at h = 4 or 5.

## The game

The board is the integers; every point starts with no chips. A point is
ready when it holds at least 2 chips; firing it removes 2 chips and adds one
to each neighbour.

Fix h and m. The adder commits to a list a₁, …, a_m of points. The firer is
told the whole list. In round t, one chip is added at a_t, and then the firer
either passes or fires one ready point of its choice. The list wins if,
however the firer plays, some point holds at least h chips after some step of
some round. G(h) is the least m for which a winning list exists, and C(h)
the least m for which the list of m zeros wins.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md),
formal text in [chain/3-question.txt](chain/3-question.txt)): find G(h),
with G(3), G(4), G(5) and the limit of G(h)/h.

## Results

| Claim | Status | Where |
|---|---|---|
| **C(h) ≤ 5h − 6 for h ≥ 2**, so G(h) ≤ 5h − 6 | proved by hand by solver B; solver A proves 5h − 5 the same way; referee read both | `solver-b/proofs.txt` P3, `solver-a/proofs.txt` Theorem 1 |
| **Every list of length m loses whenever m/5 + 2√m + 13.2 ≤ h − 1**, so G(h) ≥ 5h − 10√(5h) − 72 | proved by hand by solver A only; referee read it step by step, found no gap, and ran the strategy against every short list confined to 4 to 7 neighbouring points and against lists searched for to make it fail | `solver-a/proofs.txt` Theorem 2, `verifier/strat.cpp` |
| So G(h)/h and C(h)/h both tend to 5 | from the two rows above | |
| **Under the same firing rule every height stays at most t/5 + 35.2 after t rounds, so every list of length at most 5h − 177 loses and G(h) ≥ 5h − 176** | proved by hand by one follow-up instance; the organiser checked every line; the lemmas were tested by program on variants of the rule where they actually come into play; five reviewers then traced the proof and found no gap (see Review) | `constant/proofs.txt` Lemmas 1, 2, Theorem A |
| **The worst-case shortfall (lag) of that rule grows like the logarithm of the list length.** Upper bound, for every list and every way of breaking ties: after a round every lag is at most 2 + H_n, where n ≤ 3t is the number of points within distance 1 of the list and H_n = 1 + 1/2 + … + 1/n (Theorem U). Lower bound, for each deterministic tie-breaking that depends only on the play so far: for every j there is a list, built against that tie-breaking, of length at most 24·34^j + 84 after which some lag is (8 + j)/5 (Theorem L). A firer that breaks ties knowing the rest of the list, or at random, is not covered by the lower bound | both proved by hand by a second follow-up instance on 21 November; the organiser checked Theorem U line by line; the construction of Theorem L was run against the game with the claims of its proof asserted, for 3 stages (lag 2.2; the proof's "flush" step never arose in these runs), and a greedier variant for 12 stages (lag 4.0); the lists from N = 1156 up were replayed by a separate program by the instance, and the three smallest by the organiser; five reviewers traced both proofs and found no gap, and one rebuilt the construction from the proof's text in its own simulator against five tie rules (see Review) | `lag/PROOFS.md`, `lag/gen.py`, `lag/replay.cpp`, `lag/replay_out_34_organiser.txt` |
| G(h) ≥ 5h − 26 − 10·H_15h; the exact condition behind it (a list of length m loses if m/5 + 5.2 + 2·H_3m < h) gives G(59) ≥ 200, G(100) ≥ 398, G(1000) ≥ 4873; better than 5h − 176 for h up to somewhere near 10⁵ (not computed exactly) and, in its exact form, better than 2h − 1 from h = 28 (the closed form from h = 31) (Corollary U2) | proved by hand from Theorem U by the same instance; values computed | `lag/PROOFS.md`, `lag/corollary_out.txt` |
| Earlier search evidence for the same thing | lists found by a search over n neighbouring points on which the largest lag rises from 2.4 at n = 16 to 6.6 at n = 32,768, by 0 to 0.8 per doubling of n and with differences of up to 0.6 between runs with the same printed settings (the search's seed is not printed); the lists for n = 512 (lag 3.8) and n = 16,384 (lag 6.4) were replayed by a separate exact program, the one for 6.6 was not | `constant/lagadv2_out.txt`, `constant/lagadv2_big.txt`, `constant/replay_n512.txt`, `constant/replay_n16384.txt` |
| G(h) ≥ 2h − 1 for h ≥ 3 | proved by hand by solver B; read by the referee | `solver-b/proofs.txt` P2 |
| G(1), G(2), G(3) = 1, 2, 5 | computed by all three instances; G(3) also follows from the row above and the list 0, 1, 0, 1, 0 | `reports/` |
| **G(4) = 10 and G(5) = 15, over all lists** | upper bounds by explicit lists; lower bounds by exhaustive search made finite by a lemma of solver B, which the referee read and then re-ran with its own program | `solver-b/proofs.txt` Lemma Z, `verifier/v.cpp` |
| 15 ≤ G(6) ≤ 20 (16 by solver B's run) | the upper bound is twenty zeros; 15 is from the referee's completed search through length 14 and 16 from solver B's through length 15; solver B's search at length 16 ran out of time. Solver A's "G(6) = 20" covers only lists confined to 4 neighbouring points | `reports/referee.md` |
| C(h) = 1, 2, 7 for h = 1, 2, 3 and 5h − 10 for 4 ≤ h ≤ 14 | computed by all three instances (solver B to h = 12) | `reports/` |
| G(h) = C(h) = 5h − 10 for all h ≥ 4 | conjecture of both solvers; open | |

## Why 5h is enough

This follows `solver-a/proofs.txt` Theorem 1 (solver B's P3 is the sharper
version).

Drop every chip at 0 and let Q be the sum of x² over all chips, x being a
chip's position. Adding a chip at 0 leaves Q unchanged, and each firing
raises Q by exactly 2. After the m-th chip has been added at most m − 1
firings have happened, so Q ≤ 2(m − 1). But if every point holds at most
h − 1 chips, the chips cannot all sit near 0: once there are at least
5(h − 1) of them, the cheapest arrangement puts h − 1 on each of 0, ±1, ±2
and the rest further out, where each chip costs at least 9. Counting this
way gives Q ≥ 9m − 35(h − 1) for every m. The two bounds are
incompatible once m reaches 5(h − 1).

The potential Q with its gain of 2 per firing is a standard tool for
chip-firing on the line (Anderson, Lovász, Shor, Spencer, Tardos and
Winograd, 1989, as described in a survey; the paper itself was not
obtained).

## Why no list is much shorter

This follows `solver-a/proofs.txt` Theorem 2.

The firer needs a rule that works against every list without looking ahead.
Let k_x be the number of chips added at x so far. The rule assigns each point
a target number of firings,

    T_x = (3·k_x + k_(x−1) + k_(x+1)) / 5,

and calls the shortfall the lag of x: T_x minus the number of times x has
fired. In its turn the firer fires the ready point with the largest lag,
provided that lag is at least 1, and otherwise passes.

If every point had fired exactly its target number of times, the height at x
would be the average of k over the five points x − 2, …, x + 2, which is at
most t/5 after t rounds. That is where the 5 comes from: the targets spread
each chip evenly over five points. In general the height at x is that average
plus 2·lag(x) − lag(x − 1) − lag(x + 1), so the firer survives as long as the
lags stay small.

A point whose lag is the largest of all, and at least 6, is always ready
(its height is at least a third of its lag). So once lags are large the rule
is never stuck, and a potential (the sum of squares of the excess of each lag
over 6) grows by at most 1 per round. That bounds every lag by 6.6 + √t, and the height by t/5 + 2√t + 13.2. (Both are superseded below: the lag is at most 2 + H_3t.)

## From √h to a constant

This follows `constant/proofs.txt`. The argument above bounds every lag by
the same quantity, which grows with t. The sharper argument notices where
the cost of a large lag falls.

Fix a level a ≥ 5 and add up, over all points, the amount by which the lag
exceeds a. An add raises this sum by at most 1. In a round where the largest
lag is at least a + 1, the point fired has that largest lag (this is the
readiness fact above, which needs a lag of at least 6), and firing it lowers
the sum by exactly 1. So the sum never grows during a stretch of such
rounds, and just before the stretch began every lag was below a + 1, so each
point contributed less than 1.

Now suppose the lag at a point y is L, much larger than a. Then the sum is at
least L − a, so just before the stretch more than L − a points had lag above
a. Leave out the seven points nearest y. Each of the others has a target
above a, and its target counts only chips added at distance 3 or more from
y; one chip counts towards at most three targets, with total weight 1. So
more than a·(L − a − 7) chips were added at distance 3 or more from y.
Writing e for the number of chips added at distance 3 or more from y, this
says L < a + 7 + e/a.

Those chips are exactly what the height at y has to spare: the average of k
over the five points around y is (t − e)/5, not t/5. With a = 10 the height
at y is at most (t − e)/5 + 2·(17 + e/10) = t/5 + 34 after a round, and at
most t/5 + 35.2 just after an add. The two terms in e cancel exactly.

In the referee's and two reviewers' experiments the lag never exceeded 2.4.
The follow-up instance searched harder and found lists, for one tie-breaking
rule, on which the lag reaches 6.6 and grows slowly with the length of the
list, while the heights stay within 0.8 of t/5. So the lag itself looked
unbounded, and bounding the heights by a constant needed an
argument that does not go through a uniform bound on the lag. A second
follow-up then proved both halves (`lag/PROOFS.md`). Upper: the sum of the
k largest lags never exceeds k(3 + H_n − H_k), by the argument known for
the greedy player in the "cup game" (no reference was looked up), so no lag
exceeds 2 + H_n after a round. Lower: an explicit adaptive construction. Small groups of
seven points are prepared so that one add at the centre lifts both
neighbours and makes the centre the single largest lag, which the firer is
forced to relieve; a second add lifts both neighbours above everything
else and the firer can relieve only one. Groups whose neighbours were both
fired are then refilled to one level higher, paid for by spending
surviving top points as decoys. That costs a factor of 34 per level in the
proof (about 2.5 on average in the greedy runs; single steps range from 1 to 5, and one last step of a run is 11), which is
where the logarithm comes from. The constant
35.2 is not optimised, and the conjecture G(h) = 5h − 10 remains open.

## The small values

With h = 3 the list 0, 1, 0, 1, 0 wins in 5 rounds where zeros alone need 7.
For h = 4 and h = 5 the computed values of G(h) equal those for zeros alone;
beyond that it is open.

A search over all lists is infinite on its face, since a list may place
chips anywhere. Solver B's Lemma Z makes it finite: it restricts the firer
to a class of plays (which can only help the list) and shows that a shortest
list winning against that restricted firer has its chips in overlapping
zones. If no such list of length up to M wins, then G(h) > M. The referee
read the lemma, checked mechanically on small cases that minimal lists
winning against the restricted firer do have that shape, and repeated the search with its own program for
h = 4 and h = 5.

## What to distrust

- **Everything is by one model family.** Nothing is machine-checked as a
  proof.
- **The limit rests on one solver's proof.** Solver B did not find it and
  reported the limit as unsettled. The referee's reading and tests support
  solver A's proof, but its program tests cannot reach the regime the proof
  is about (lags of 6 and more never occurred in any test); it tested that regime
  only through a deliberately lazy variant of the rule covered by the same argument. Since 21 November the limit also follows from Corollary U2 in `lag/PROOFS.md`, which uses the same rule and three of solver A's facts and needs readiness only at lags above 3, a regime the tests did reach.
- **The constant bound has one author and is new.** It was written on 21
  November by a single follow-up instance. The organiser checked each step of
  its two lemmas and the theorem by hand, and it builds only on three facts
  of solver A's proof that the first review round confirmed. Its tests
  exercise the lemmas on lazy variants of the rule (fire only at lag 6 or
  11), not on the rule itself, where a lag of 6 was reached only by the
  searched lists and never in the lemma tests. The tests of the final height
  bound are far from binding (largest excess 15.2 against 34). It is not
  machine-checked. The first evidence that the lag is unbounded was a search
  result for one tie-breaking rule that cannot be regenerated exactly because
  the seed was not recorded; it is superseded by the proof below.
- **The logarithmic bounds on the lag are hours old and have one author.**
  Theorem U is a ten-line induction that the organiser checked. Theorem L is
  a two-page construction; its claims are asserted in runs of the proof's
  own construction (3 stages; its flush step and one sub-case of its
  rebuilding step never arose there) and of a greedier variant (12 stages),
  which tests the bookkeeping and not the proof. `lag/PROOFS.md` is left as
  the instance wrote it; reviewers noted that its summary sentence drops the
  additive constant of the upper bound (it is about ln t + 4.1, not ln t),
  that the lower-bound formula needs t ≥ 108, and that one step uses without
  saying so that a number of chips is an integer. `lag/PROOFS.md` and `lag/REPORT.md` also still carry "2 to 4", "0.2 to 0.8", "every list replayed" and "unreviewed", which this page corrects. It does not cover a
  firer whose tie-breaking looks at the rest of the list. The gap between
  the two bounds is a factor of about 18 in the coefficient of ln t. Two
  checks the instance expected to fail did not (a list built against one
  tie rule still reached its level when replayed under another), so the
  replays do not show that a list depends on its tie rule. Four lists
  (6 MB to 73 MB) are left out; `lag/gen.py` regenerates them.
- **Slips in the follow-up's files.** `constant/proofs.txt` states the chain
  5h − 176 ≤ G(h) ≤ C(h) ≤ 5h − 6 "for all h"; the upper bound needs h ≥ 2.
  One line of Lemma 2 reads "Φ < |Z|" before noting that Z is not empty. It
  refers to a `REPORT.md` that is not in the folder (the numbers are in
  `lagadv2_out.txt` and `lagadv2_big.txt`) and quotes lag maxima of "7.2 and
  12" where the output has 7.2 and 12.2. `test_out_superseded.txt` is the
  output of an earlier version of the test script, in which one deliberately
  wrong variant still passed; `test_out2.txt` is the corrected run. Two
  assertions in `test_lemmas.py` are strict where the lemma has "at most".
- **G(4) and G(5) depend on Lemma Z and on searches that key their memory by
  a hash.** A collision or a wrong pruning could only hide a winning list,
  which would make the reported value too large; the upper bounds are
  explicit lists.
- **G(6) is not known.** Solver A's table lists 20 as computed; it is
  computed only for lists confined to a narrow window.
- **The literature search read sections of five downloads and none in full.**
  It did not find this game, the constant 5, or the values. The two classical
  chip-firing papers were not obtained (one was read through a survey). The
  nearest games it found are a toppling game in which both players add chips
  and firing is automatic (Bonato, Kinnersley and Prałat, 2013) and
  buffer-size results for adversarial packet injection on a line (Miller and
  Patt-Shamir, 2017), where packets flow one way and leave.
- **Errors in the chain's question.** Its bound C(h) ≤ √24·(h − 1) + O(1) is
  false for large h (it used a continuum estimate where only about five
  points matter), and its guess about the limit follows it. Its small values
  are right.
- **Small errors.** Solver A's Theorem 1 is stated for every h ≥ 1 and is
  false at h = 1. Its simulation's reported maxima are slightly low (the
  referee found lag 2.4 and height excess 3.2). Under the other natural
  reading (heights tested only at the end of a round) solver A computes
  values for zeros alone that fit 5h − 8 for 3 ≤ h ≤ 8; nobody else checked
  that. Solver B's report says that G = C at h = 4 and 5 contradicts the
  chain's guess about the limits; two finite values cannot do that, and it is
  solver A's theorem that does. The referee's strategy program checks the
  numerical height bound after the firing step only, although the proof
  claims it at both steps.
- **Process.** Solver B reported a third process of its own overlapping
  briefly. Two further items are from the organiser's session and are in none
  of the reports: early in its run solver A started a few commands from the
  wrong directory, leaving four one-line error files in the root of this
  repository's working copy, which were moved out and never committed; and
  the referee said in a note that a wait loop of its own overlapped briefly
  as a third process.

## Review

Before publication the claims went to five reviewers from four model
families: GPT, Grok and Gemini, and two Claude instances (one of them Claude
Opus). All five confirmed the upper bound, every step of the survival
strategy, the consequences drawn from it, and solver B's reduction lemma;
none found a gap, and two reviewers' own simulations against adversarial
lists did not break the strategy. Their findings on this page, applied above:

- My summary said that for large h no list does better than a single point.
  Only the first-order statement is proved; a gap of order √h is not excluded
  (four seats).
- "Mixing helps only at h = 3" is established only for h ≤ 5 (three seats).
- The page linked a question file that was missing from the folder. A copying
  script had taken it for a binary; it is restored (all five seats).
- "Every short list" overstated what the referee's exhaustive runs covered.
- Two process remarks had no source in the folder.
- The lower bound holds for every h, not only large h (both Claude seats).

The corrected wording has not been through a second round.

The constant bound of 21 November went to the same five reviewers. All five
traced Lemma 1, Lemma 2, Theorem A and the corollary line by line and found
no gap; none found a list that breaks the accounting, and one replayed a
search list with its own script. Their findings, applied above:

- A status cell said the review panel had read the proof before it had (both
  Claude seats, Grok).
- "For all h" on the two-sided bound is false at h = 1, where the upper bound
  does not hold (all five).
- The search evidence was described too neatly: one replay output was in the
  folder where the page said two (the second has been added), the trend per
  doubling is irregular, and identically labelled runs differ.
- A stale test output showed a wrong variant passing; it is now labelled.
- The headline stated the constant with no status.

These edits have not been reviewed again.

The lag results of 21 November went to the same five reviewers. All five
confirmed Theorem U (base, the three cases and the algebra), the two rules
on forced firings, the base and the stage of Theorem L with its counting,
and the corollary, whose sample values two of them recomputed. None found a
move by the firer that the lower-bound proof misses; one Claude seat rebuilt
the construction from the proof's text in its own simulator and ran it
against five tie rules, two chosen to be awkward. Their findings, applied
above:

- The headline and the top-level row stated "grows like log t" without the
  quantifiers: the upper bound is for every list and tie rule, the lower
  bound is one list per deterministic, history-only tie rule (GPT, Grok,
  both Claude seats).
- "2 to 4" per level in the greedy runs was wrong (four seats); my sentence
  about "rebuilding the survivors" misdescribed the construction (Gemini,
  both Claude seats).
- "Every claim asserted in runs that reach 12 stages" mixed up the proof's
  construction with the greedy variant, and one step of the proof was never
  exercised (a Claude seat).
- "Each list replayed" was untrue of the three smallest; the organiser
  replayed them.
- An older row said 0.2 to 0.8 per doubling where one step is 0 (GPT).

A second-round reviewer (one Claude instance) then checked the edited page
against the files. It found the Short answer stale (it did not mention the
stronger bound for moderate h), one more wrong range for the greedy factor,
and older sentences not marked as superseded; all applied.

## Files

- `lag/`: the second follow-up of 21 November (the lag is logarithmic):
  `PROOFS.md`, the generator of the lists, a separate replay program, tests
  and outputs.

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: searches, witnesses, checkers, proofs.
- `verifier/`: the referee's reference implementation, search, and the
  program that plays the survival rule.
- `constant/`: the follow-up that proves the constant bound: its brief,
  `proofs.txt`, the lemma tests with their outputs, and the search for lists
  with large lag (one list of 150,161 entries is left out; `lagadv2.cpp`
  produces lists of this kind but its seed was not recorded).
- `prior-art.md`: the literature search. The downloaded papers are not
  included.

Binaries are left out.
