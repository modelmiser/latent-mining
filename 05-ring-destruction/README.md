# Experiment 5: destroying messages on a ring

n processes on a ring tell both neighbours everything they know, every
round. An adversary destroys up to k messages per round but may not hit the
same link two rounds running. How long can it keep every process from knowing
everything?

**Short answer: for 3 ≤ k < n, the first process to know everything does so
at round n − ⌈n/k⌉ at the latest, and the adversary can hold it off until
exactly then.** As a fraction of the ring that is 1 − 1/k in the limit. With
k = 1 or 2 the adversary gains nothing: someone knows everything at round
⌊n/2⌋, as with no adversary at all. Proved by hand by one solver; the other
solver proved the limit and the formula to within one round; computation
agrees for n ≤ 12 with k ≤ 4.

## The problem

Processes are 0 … n − 1 on a ring, n ≥ 3. There are 2n directed links,
(i → i+1) and (i → i−1). In every round every process sends everything it
knows on both of its outgoing links. There is no protocol to choose.

The adversary picks, each round t, a set D_t of at most k links whose
messages are destroyed, with D_t disjoint from D_(t−1). It sees everything.

K_0(i) = {i}, and K_t(i) adds what arrives from each neighbour whose link
into i was not destroyed in round t. A process is complete when it knows all
n names. F(n, k) is the latest round, over all adversaries, at which the
first process becomes complete, and c(k) is the limit of F(n, k)/n.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): find
c(k) for k ≥ 3. The chain bet on c(4) = 5/8.

## Results

| Claim | Status | Where |
|---|---|---|
| F(n, 1) = F(n, 2) = ⌊n/2⌋ | proved by hand, twice; referee read both | `solver-a/proofs.txt` §3, `solver-b/proofs.txt` Theorem K2 |
| F(n, k) ≥ n − ⌈n/min(k, n)⌉ | proved by hand by solver A; referee read it and simulated the strategy for n ≤ 70, k ≤ 14. Solver B reached the same strategy separately, with a proof for k ≤ n, n ≥ 5 that the referee only simulated. The question both received already hinted at opposing ramps | `solver-a/proofs.txt` Theorem L |
| F(n, k) ≤ n − ⌈n/k⌉ for n > k ≥ 3 | proved by hand by solver A; referee read it line by line, redid the algebra and tested every lemma on 11,700 plays; I read it once and redid the even case | `solver-a/proofs.txt` Theorem U |
| **F(n, k) = n − ⌈n/k⌉ for all n > k ≥ 3** | follows from the two rows above | |
| F(n, k) < n − n/k + 1 for n > k ≥ 3 | proved by hand by solver B, independently and by a different count; weaker by at most one round | `solver-b/proofs.txt` Theorem U |
| **c(k) = 1 − 1/k for k ≥ 3**, and c(1) = c(2) = 1/2 | follows from either solver's bounds | |
| Table of F(n, k), n ≤ 12, k ≤ 4 (below) | computed. Every upper bound is an exhaustive search (solver A and the referee to n = 12, solver B to n = 11). The lower bounds for (11,4), (12,3), (12,4) come from the strategy, checked by program, because the searches for a surviving play timed out | `reports/` |
| F(n, k) = n − 1 for n ≤ k < 2n, and 2⌊n/2⌋ for k ≥ 2n | odd n, and even n with k ≥ 2n: proved by solver A. Even n with n ≤ k < 2n: computed for n ≤ 8, plus a short argument by the referee that one reviewer traced and found correct | `solver-a/proofs.txt` §4, `reports/referee.md` item 8 |

F(n, k):

| k \ n | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
|---|---|---|---|---|---|---|---|---|---|---|
| 1, 2 | 1 | 2 | 2 | 3 | 3 | 4 | 4 | 5 | 5 | 6 |
| 3 | 2 | 2 | 3 | 4 | 4 | 5 | 6 | 6 | 7 | 8 |
| 4 | 2 | 3 | 3 | 4 | 5 | 6 | 6 | 7 | 8 | 9 |

## What the adversary does

Information moving one way round the ring advances one step per round. A
destruction on the link it is about to cross holds it up by one round. The
adversary cannot hit that same link again next round, but it can follow the
delayed information and hit it again two rounds later. One such chain of
destructions builds a ramp of delays 1, 2, 3, … on consecutive pieces of
information.

The strategy runs k chains against clockwise information on odd rounds and k
against counter-clockwise information on even rounds, spaced at most ⌈n/k⌉
apart. That uses the full budget every round and never hits a link in two
consecutive rounds. The clockwise ramps slope one way and the
counter-clockwise ramps the other, and they are aligned so that at every
process the two delays add up to at least what is needed to keep it
incomplete, through round n − ⌈n/k⌉ − 1.

With k ≤ 2 the adversary has made too few destructions by round ⌊n/2⌋ to give
every process a delay at all; the proof is a count (`solver-a/proofs.txt`
§3).

## Why it cannot do better

A destruction can raise a piece's delay by several units at once, so delay
itself cannot be counted against the budget. What can be counted is pairs: a
pair is a piece of information together with a delay value it has held at
some time. Each destruction creates at most one new pair, so after t rounds
there are at most kt. A piece can only reach delay y if its neighbour reached
y − 1 at least two rounds earlier, which is the no-repeat rule again; so
delays are at most about t/2 and the pairs form ramps. For every process to
be incomplete, the two delays at each process must add up to at least
2t − n + 2. Counting the pairs this needs along each ramp, and comparing with
kt, gives the bound. For odd t solver A's count gains the last round over
solver B's because pairs of the largest possible value can only be created in
round t itself, so there are at most k of them.

## What to distrust

- **Everything is by one model family.** The exact upper bound (solver A's
  Theorem U) was written by one instance and read by one referee instance.
  Solver B did not reach it; B's own bound confirms the limit but not the
  exact formula. Nothing is machine-checked as a proof.
- **The two solvers' search witnesses are the same schedules** where the
  referee compared them, presumably because both searches try links in the same
  order. They are valid but not independent evidence. The table itself was
  recomputed by the referee with a different search.
- **Solver B's report is wrong about what its proof leaves open.** It names
  F(10, 4) as the first undecided entry; the referee found its bound already
  fails to decide F(5, 4). This does not affect solver A's proof.
- **The range k ≥ n for even n** rests on computation to n = 8 and one
  unreviewed argument.
- **The problem is not the seed.** The seed asked which functions identical
  processes can compute under message loss. The chain turned that into a
  one-player question about how long flooding can be delayed, with no
  protocol to design.
- **Prior art** ([prior-art.md](prior-art.md)): broadcast under a bounded
  number of faulty links per step is an established topic (Chlebus, Diks,
  Pelc 1996 and later work on message adversaries). The search found nothing
  that combines a per-round budget with a rule against repeating a link, and
  did not find this formula in that setting (the k = 3 and k = 4 rows are
  ⌊2n/3⌋ and ⌊3n/4⌋, which are in the OEIS with no mention of networks). It
  read no paper in full, and three full texts could not be opened, one of
  which reportedly treats rings.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files, with most of the
effort asked for on solver A's upper bound. All five confirmed the model
lemmas, the pair-counting lemmas, the diagonal argument, both parity cases
and the algebra of the corollary, and the k ≤ 2 proofs. Four confirmed the
lower-bound proof as written; GPT marked it refuted because the construction
is not defined for k = 1, where the bound is trivial.

Most of what they found was in my own explanation, not in the proofs:

- I wrote that each destruction creates at most one unit of delay. That is
  false, and it is the very argument solver A's report calls invalid. The
  count is of (piece, value) pairs. Four reviewers caught it.
- I wrote that the two delays add up to the same total at every process.
  They add up to at least the amount needed; on a 7-ring with k = 3 the sums
  differ.
- "Never repeats a link" should have been "never in consecutive rounds".
- The statement that top-value pairs appear only in the last round holds for
  odd t only.
- "Proved twice" for the strategy, and "exhaustive search" for the whole
  table, both claimed more than the reports support.

Loose statements in the two proof files are listed in a note at the top of
each. The corrected wording has not been through a second round.

## Files

- `chain/`: the seed and the three links, verbatim, with the question each
  link handed on.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: search programs, adversary sequences, strategy
  generators, checkers, proofs.
- `verifier/`: the referee's own search, checker and lemma tests.
- `prior-art.md`: the literature search.

Binaries are left out.
