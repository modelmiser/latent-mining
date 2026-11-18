# Experiment 14: orienting a forest as it grows

Edges are added to a forest one at a time by an adversary. Each new edge must
be given a direction, with at most two edges pointing out of any vertex, and
reversing an edge that was oriented earlier costs one flip. How many flips
can the adversary force over the whole sequence?

**Short answer: at least about n/3, and at most about n.** A simple attack
forces B(n) = max(0, ⌊n/4⌋ − 1) flips and is exactly optimal for up to 15
vertices. It stops
being optimal by n = 30, and a better attack forces
n/3 − (7/3)·log₂(n+1) + 5/6 for every n. The best known algorithm pays at
most n − log₂(n+1). The exact rate, somewhere from 1/3 to 1 flip per vertex,
is open. The chain guessed the simple attack would fail somewhere between
n = 16 and 24; it holds at least to 15, and the first failure is somewhere
from 17 to 30 (from 16, if the single-program value at n = 16 is set aside).

## The game

V = {1, …, n}. A position is a forest G on V with every edge directed so that
each vertex is the tail of at most 2 edges. The start has no edges. In each
of n − 1 rounds:

1. The Adversary names two vertices in different components.
2. The Algorithm orients the new edge and may reverse any older edges, as
   long as the result is valid. It pays the number of older edges reversed.

Both see everything. T(n) is the total the Adversary can force. A vertex is
saturated if it is the tail of 2 edges. B(n) = max(0, ⌊n/4⌋ − 1).

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): is
T(n) = B(n) for every n, and if not, what is T(n) and the limit of T(n)/n?

## Results

L denotes the limit of T(n)/n, which exists.

| Claim | Status | Where |
|---|---|---|
| The Algorithm loses nothing by a normal form: reverse nothing and orient the new edge out of an endpoint with a free slot, or reverse one directed path from a saturated endpoint through saturated vertices to the first vertex with a free slot | proved by hand by solver B; referee read it and checked by program, for every position with n ≤ 11 and every Adversary move, that the best normal reply is as good as the best re-orientation of the merged tree (a one-round check); known as a repair move in the literature, where its optimality for the whole game was not found stated | `solver-b/proofs.txt` Lemma 1 |
| T(n) ≥ B(n) for all n; T is monotone and superadditive, so L exists | proved by hand, twice; referee read both | `solver-a/proofs.txt` P3, P4; `solver-b/proofs.txt` Lemmas 2, 3, 5 |
| T(n) ≤ n − log₂(n+1), by always reversing a shortest path in the smaller tree | proved by hand by solver B; referee read it | `solver-b/proofs.txt` Lemma 4 |
| T(n) = B(n) for n ≤ 15 | computed exhaustively by both solvers and the referee, three programs | `reports/` |
| T(16) = 3 = B(16) | the upper half rests on one solver's program | `solver-a/run_n16.txt` |
| T(30) ≥ 7 > 6 = B(30) | two routes with different lemmas and certificates (both use a simulation argument of the same kind), each certificate checked by the referee's own checker | `solver-a/cert_W2_*.txt`, `solver-b/cert_n30_k7.txt` |
| **T(n) ≥ n/3 − (7/3)·log₂(n+1) + 5/6 for every n, so L ≥ 1/3** | proved by hand by one follow-up instance; read line by line by a second, which found one missing step and supplied it (see below); I followed the lemmas and the main argument | `open-limit/proofs.txt` Lemmas A, B, Type Lemma, Theorem 1; `reports/open-limit-referee.md` |
| T(n) > B(n) for every n ≥ 30 | for 30 ≤ n ≤ 35 from the certificates for T(30) ≥ 7 and T(32) ≥ 8; for 36 ≤ n ≤ 63 only from a reduced game computed by the follow-up's program and the second reader's (the follow-up's own unpruned cross-check stops at 61); from 64 on from the certificate for T(64) ≥ 18 and superadditivity | `open-limit/proofs.txt` Theorem 2 |
| This family of attacks (one tracked vertex per tree) cannot force more than (n − 1)/3 in its reduced game | proved by hand; read by the second instance | `open-limit/proofs.txt` Theorem 3 |
| The least n with T(n) > B(n) | open; in 17 … 30 (16 … 30 without the single-program value at 16) | |
| The exact value of L | open; 1/3 ≤ L ≤ 1 | |

The general bound of the seventh row is weak for small n: it is positive only
from n = 34, first exceeds B(n) at n = 179, and exceeds it for every n from
189 on. Below that the certified and computed values do the work.

## The simple attack

Work in blocks of four vertices. In each block join two pairs, then join the
two tails: the Algorithm either pays a flip there or is left with a saturated
vertex. Then join trees, naming saturated vertices when both have one:
neither endpoint has a free slot, so the Algorithm must reverse something.
The count that comes out is that k blocks force k − 1 flips.

## The better attack

For a vertex v let h(v) be the number of steps along directed edges from v to
the nearest vertex with a free slot (0 if v has one). Reversing that path is
the cheapest way to free a slot at v, and costs h(v).

The Adversary tracks one vertex per tree and sorts trees into types by that
vertex's two out-slots, where a slot holding an out-neighbour is worth that
neighbour's h and an empty slot is worth −1: type A_k if both are worth at
least k − 1, type R_k if one is worth at least k − 1 and the other at least k. It always joins the
tracked vertices of two trees of the same type. The Type Lemma says what the
Algorithm can then do:

- two A_k trees: every reply costs at least k and leaves an R_k tree;
- two R_k trees: every reply either costs at least k and leaves an A_(k+1)
  tree, or costs at least k + 1 and leaves an R_k tree.

So depth k costs k per merge, and the Algorithm can avoid going deeper only
by paying one more now. (These statements are about normal-form replies.)
The bookkeeping is a potential: paid so far, plus k + 1/3 for an A_k tree or
k + 2/3 for an R_k tree, is at least a third of the tree's size, and every
merge of two trees of the same type preserves that. Trees left without a
partner of the same type are merged at the end; each of those merges can
lower the potential by at most 2/3, and there are at most about 2·log₂(n+1)
of them, which is the logarithmic term.

Both solvers had found the mechanism. Solver B tried to turn it into an
induction and reported that it breaks when the Algorithm overpays by one in a
subtree. The follow-up's diagnosis is that the schedule of merges had been
fixed in advance; pairing trees by their current type, whatever the Algorithm
did to them, is what makes the invariant hold from every normal-form
position.

## From normal-form replies to all replies

The better attack is adaptive: it looks at the position to choose its next
move, and after a reply that is not in normal form the position need not have
the types the attack expects. Solver B's normal-form lemma is proved by
comparing, on one and the same sequence of Adversary moves, an arbitrary play
with a normal-form play that costs no more. As worded ("against every
Adversary strategy") it claims more than that, and read pointwise against an
adaptive Adversary it is false.

What is needed is true, and can be seen in two ways. By value: a
deterministic Algorithm faces sequences of moves, the lemma turns it into a
normal-form Algorithm that is no worse on every sequence, so the game against
normal-form Algorithms has the same value. Constructively, as the second
reader wrote it: the Adversary keeps a shadow orientation of the same forest,
updated each round by the normal-form reply the lemma associates with the
Algorithm's actual reply, and chooses its moves from the shadow; the lemma's
inequality holds round by round whatever is named, so the real cost is at
least what the attack forces on the shadow. The second reader tested the
construction against algorithms making random extra flips, and tested that
reading types off the real position instead does fail.

The reviewers split on whether this is a gap in the n/3 proof or only a
statement that needed tightening. Either way the bound stands.

## What to distrust

- **Everything is by one model family.** The n/3 bound was written by one
  instance and read by one other. Nothing is machine-checked as a proof.
- **Much of this rests on the normal form** (row one), proved by hand and
  checked by program for n ≤ 11 only: the n/3 bound, the reduced game, solver
  B's certificates for T(30), T(32) and T(64), and the table entries at
  n = 14 and 15, which were searched with normal-form replies. Solver A's
  route to T(30) ≥ 7 and T(32) ≥ 8, the upper bound and the simple attack do
  not use it.
- **T(16)** has one program behind its upper bound; the referee could not
  fit the search in memory.
- **The values for 36 ≤ n ≤ 63** come only from a reduced game solved by two
  programs, not from certificates.
- **Nothing is known from above beyond the greedy bound.** The follow-up
  tried to turn its own potential into an Algorithm and found a 9-vertex
  tree where that fails. One solver reports the greedy recursion evaluates to
  about 0.558 n for n ≤ 3000, which is numerical only.
- **Prior art.** Two searches ([prior-art.md](prior-art.md),
  [prior-art-deep.md](prior-art-deep.md)). Incremental edge orientation in
  forests is an active topic (Bender, Kopelowitz, Kuszmaul, Porat, Stein
  2021; Marques Domingues, Nguyen, Solomon 2026; Hiken, Wein 2026), studied
  for update time and for flips per insertion, worst-case and amortized. The
  path reversal is described there as folklore, and a total of at most
  3(n − 1) flips follows from a lemma of Brodal and Fagerberg (1999). The
  deeper search, which read the relevant sections of eleven papers and
  keyword-scanned six more, found no lower bound on total flips for forests
  at out-degree 2 and none of the values here; one of the 2026 papers says
  that for update time no non-trivial lower bound is known even for dynamic
  forests. Two cautions from that search: a constant
  number of flips per insertion is what this literature would call a trivial
  bound, so the interest is in the constant; and the paper most likely to
  contain an earlier result (Gupta, Kumar, Stein, SODA 2014) was paywalled
  and not read.
- **Large certificates are left out** of this folder: the 10 MB certificate
  for T(64) ≥ 18 and several position tables for n = 11, 12, 13. The programs
  that generate them are included.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files, with most of the
effort asked for on the normal form, the n/3 bound and the transfer to all
replies. All five confirmed the normal-form lemma's inequality, Lemmas A and
B, the Type Lemma, the main and cleanup phases of Theorem 1 and its
constants, the transfer argument, the simple bound, the upper bound, solver
A's route to T(30) ≥ 7, and the reduced game with its ceiling of 1/3. Their
findings, applied above:

- I wrote that the general bound exceeds B(n) "only from n = 189". It first
  exceeds it at n = 179; 189 is where it does so for good. I copied the
  figure from a report without recomputing it. GPT marked the constants
  question refuted on this.
- I wrote "each merge preserves" the potential. Only merges of equal type do;
  the cleanup merges lose up to 2/3 each.
- I described the simple attack as one flip per block and its block join as
  free. It is k − 1 flips for k blocks, and the join is free only if the
  Algorithm accepts a saturated vertex.
- I wrote "nearest" in the normal form, where the lemma says "first along the
  path".
- I presented the shadow construction as a repair the proof needed. Three
  reviewers consider it one way of making explicit something the lemma
  already gives; one considers it necessary; all agree the lemma's wording
  overstates. The section above now says so.
- Several scope statements were too broad or too narrow (what rests on the
  normal form, which values rest on which programs, what the cited papers'
  "no lower bound" refers to).

Loose statements in `solver-b/proofs.txt` and `open-limit/proofs.txt` are
listed in a note at the top of each. The corrected wording has not been
through a second round.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`,
  `brief-open-limit.txt`: what each instance was told. The reader of the
  limit proofs was told to assume them wrong, follow every step, test the
  Type Lemma against every legal reply and not only normal ones, and replay
  the attack against several algorithms; that brief was not saved as a file.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: game solvers, certificates, checkers, proofs.
- `verifier/`: the referee's solvers and certificate checker.
- `open-limit/`: the n/3 bound, the reduced game and their tests.
- `open-limit-referee/`: the second reader's programs and logs.
- `prior-art.md`, `prior-art-deep.md`: the two literature searches.

Binaries are left out.
