# Experiment 25: chips arrive every round, but only one vertex may fire

Chips are dropped one per round onto points of a line, following a list fixed
in advance. A point holding two chips may fire, sending one chip to each
neighbour. An opponent who knows the whole list decides the firing, but may
fire at most one point per round. How short a list forces some point to hold
h chips, whatever the opponent does?

**Short answer: about 5h rounds, and to first order nothing beats dropping
every chip on the same point.** If G(h) is the length of the shortest winning
list, then G(h)/h tends to 5; more exactly
5h − 10√(5h) − 72 ≤ G(h) ≤ 5h − 6 for every h ≥ 2 (the lower bound says
nothing until h is about 45). So no list beats the single-point list by more
than about 10√(5h) rounds; whether any list beats it at all for h ≥ 6 is
open. The small values are G(1), …, G(5) = 1, 2, 5, 10, 15. The chain guessed
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
over 6) grows by at most 1 per round. That bounds every lag by 6.6 + √t, and
the height by t/5 + 2√t + 13.2.

In the referee's and two reviewers' experiments the lag never exceeded 2.4, against the proved
bound of 6.6 + √t. So the √h term in the lower bound is probably an artefact
of the proof, and nobody has proved a bound of the form 5h minus a constant.

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
  only through a deliberately lazy variant of the rule covered by the same
  argument.
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

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: searches, witnesses, checkers, proofs.
- `verifier/`: the referee's reference implementation, search, and the
  program that plays the survival rule.
- `prior-art.md`: the literature search. The downloaded papers are not
  included.

Binaries are left out.
