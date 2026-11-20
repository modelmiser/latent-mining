# Experiment 16: a counter that erases what it reads

A counter is kept in bits of memory. To step it, a rule looks at up to r bits,
one at a time, choosing each next bit from what it has seen. Every bit it
looks at is erased to 0. It may then set one of the bits it looked at to 1.
Nothing else is remembered between steps. How long a cycle can such a counter
have?

**Short answer: at least 2, 6, 42, 1806, 3263442, …, each term t followed by
t(t + 1), for r = 1, 2, 3, 4, 5, …, and exactly that for r ≤ 4.** So the
longest cycle D(r) is at least 2^(2^(r−1)): doubly exponential in the number
of bits read, although a step carries almost nothing forward in the bits it
touched. Whether D(r) equals the sequence for every r is open; we found no
upper bound better than 2^(2^r − 1) − 1, one less than the number of memory
states. The chain bet the other way, on a cycle
length whose logarithm is polynomial in r, and was wrong.

The sequence is Sylvester's sequence minus one (OEIS A007018).

## The rule

Fix r ≥ 1. Memory is a string of d = 2^r − 1 bits. The rule is a binary
decision tree: each internal node names a bit position and has a 0-child and
a 1-child; no position occurs twice on one path from the root; every path has
at most r internal nodes (paths may be shorter). Each leaf carries either
nothing or one position that occurs on its own path.

One step: start at the root, at each internal node read the named bit and go
to the matching child. On reaching a leaf, every bit read on the way is set
to 0; if the leaf names a position, that bit is then set to 1. Bits not read
keep their value.

A cycle of length N is a memory state that returns to itself after exactly N
steps and not sooner. D(r) is the largest N over all trees. (More memory does
not help: a tree of depth r mentions at most 2^r − 1 positions.)

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): find
D(r), give D(2) and D(3), and settle whether log₂ D(r) is bounded by a
polynomial in r.

## Results

Write W_1 = 2 and W_r = W_(r−1)·(W_(r−1) + 1), so W = 2, 6, 42, 1806,
3263442, ….

| Claim | Status | Where |
|---|---|---|
| D(1) = 2, D(2) = 6 | computed by solver B and the referee over every tree, and by solver A's search | `reports/` |
| D(3) = 42 | computed: by both solvers, each with two programs of different design, and by the referee with a brute force over all trees (two positions fixed by symmetry) and a second program | `solver-a/naive3.cpp`, `solver-b/ex3.cpp`, `verifier/brute.cpp` |
| D(4) = 1806 | computed by solver B and by the referee, with searches built on the same idea (see "What to distrust"); solver A ran its search at r = 4 only with at most 6 positions allowed, where it timed out, has annealing only for the full problem, and claims only ≥ 1806 | `solver-b/lazy4_summary.txt`, `verifier/lazy_r4.out` |
| **D(r) ≥ W_r for every r** | proved by hand, by both solvers with the same construction; the referee read both proofs and found no gap; the cycles for r = 1 … 5 are files that a short simulator checks | `solver-a/proofs.txt` Theorem 1, `solver-b/proofs.txt` Theorem 1 |
| 2^(r−1) ≤ log₂ D(r) ≤ 2^r − 1, so log₂ D(r) is not bounded by a polynomial in r | follows from the row above and from counting states | same files |
| D(r) ≤ 2^d − 1 for r ≥ 2 | proved by hand by solver A (terse); read by the referee; solver B has only a sketch | `solver-a/proofs.txt` Remark 3 |
| A cycle containing no step that raises the number of 1s has length at most 2^r | proved by hand by solver A; the referee found one misstated detail that does not affect the conclusion | `solver-a/proofs.txt` Lemma 2 |
| At r = 3 the trees reaching 42 are exactly the relabellings of the construction | computed by both solvers and the referee | `solver-a/naive3.out`, `solver-b/ex3_full0.out`, `reports/referee.md` |
| With at most m distinct positions allowed, the longest cycle B(r, m) obeys B(r, m) = max over m₁ + m₂ = m − 1 of B(r−1, m₂)·(B(r−1, m₁) + 1) | computed for r ≤ 3 and for r = 4 (the referee completed the r = 4 row exhaustively); not proved | `reports/referee.md` |
| On every cycle of length at least 2, at most one step uses a leaf that writes nothing | computed for r ≤ 3 over all trees up to relabelling of positions; not proved, not tested at r = 4 | `solver-b/prod3.cpp` |
| D(r) = W_r for every r | conjecture; computed for r ≤ 4 only | |

## Why the cycle can be so long

This follows the proof in `solver-a/proofs.txt`; solver B's is the same
construction.

Suppose a tree M of depth r − 1 has a cycle of length W and a leaf that
writes nothing, the free leaf, to which exactly one state of the cycle is
routed.
Build a tree of depth r from a new root bit a and two copies of M on
separate memory, X and Y:

- under a = 0 put the X-copy, with its free leaf changed to write a;
- under a = 1 put the Y-copy unchanged.

While a = 0 the step is an ordinary step of the X-copy, and a stays 0, except
at the one state per round that reaches the changed leaf: that step also sets
a. The next step reads a = 1, erases it, and performs one step of the Y-copy.
So the X-copy goes round its whole cycle, W steps, and then the Y-copy
advances once: W + 1 steps per round, and W rounds before the Y-copy is back
where it started. The W² + W states visited are pairwise distinct. The
Y-copy's free leaf is still a leaf that writes nothing, and it is used once
on the new cycle, so the construction can be repeated. Solver A starts it
from the tree that is a single leaf (no bits, cycle length 1); one step of
the construction gives the one-bit tree that toggles, cycle length 2, which
is where solver B starts.

The unread bits are what carries the count. A step erases everything it
touches, but the X-copy and Y-copy memories that it does not touch keep their
state, and one written bit is enough to pass a carry from one to the other.
The chain's bet overlooked this.

The construction uses each of the 2^r − 1 positions at exactly one internal
node, and its cycle
length is about 2^(0.676·2^r).

## What to distrust

- **Everything is by one model family.** Nothing is machine-checked as a
  proof.
- **D(4) = 1806 rests on one idea of search.** Solver B's program and the
  referee's both follow a trajectory and decide the tree only where the
  trajectory needs it (they encode states differently). Solver A's program
  uses the same idea without the pruning on minimum weight; it timed out with
  6 positions allowed and was not run on all 15. The referee checked its own
  program against its brute force only for r ≤ 3 and did not mutation-test
  it. A shared design error at r = 4 is not excluded. Every length the
  searches record is the orbit of a legal tree, and 1806 is also a proved
  lower bound, so such an error could only hide a longer cycle.
- **The exact function is open.** For large r the construction leaves about
  a third of the available bits of information unused, and nothing excludes
  trees whose two halves share memory doing better. Both solvers put their
  confidence in the conjecture near 70 to 75 per cent.
- **The literature search read no source in full.** It did not find the
  model (reads that erase, one write back) or the values. The surrounding
  field is counters and quasi-Gray codes in the bit-probe model, from Fredman
  (1978) on, where reads do not erase; there, counters of length 2^(cn) with
  about log n reads are established, so a long cycle here is not surprising.
  A 2010 paper (arXiv:1010.0905, by Bose, Carmi, Jansens, Maheshwari, Morin
  and Smid; `prior-art.md` lists the same authors in another order) builds a
  composite counter in which one counter steps another when it wraps, which
  is the same idea as the construction above with periods that are powers of
  two instead of W and W + 1. Fredman's paper was not read and is the
  likeliest place for an overlap.
- **Errors in the chain's question.** Its claim that the all-ones state has
  no predecessor for r ≥ 2 is false (a tree whose root's 0-child is a leaf
  writing the root sends 011…1 to 11…1); the bound it was meant to give,
  D(r) ≤ 2^d − 1, holds by another argument. Its claim 6 ≤ D(2) ≤ 7 resolves
  to 6.
- **Small errors.** Solver A's Lemma 2(c) says every step of a cycle with no
  weight-raising step reads exactly one 1; that fails at a fixed point reached
  along the all-zero path, where the conclusion (length 1) still holds.
  Solver B's file `bad_idx.txt` is described as a repeated index but is
  rejected for a different reason, so solver B never showed its checker
  rejecting a repeated index; the referee's checker does. Solver B's
  `check.py` reads any character other than 1 in a start state as 0, so it
  would accept some malformed files. In a note to the organiser that is not
  part of its written report, solver A said that one of its wait loops ran
  for about 100 seconds outside the run rules.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files. All five confirmed
the lower-bound construction and its proof, the growth bounds, the soundness
of the searches, and that the rule described here is the one in the chain's
question. Four confirmed solver A's proof of D(r) ≤ 2^d − 1 and its Lemma 2;
GPT marked that item refuted because of the misstated detail in Lemma 2(c)
listed above, and noted that the lemma does not treat the tree that is a
single leaf. Their findings on this page, applied above:

- I wrote that solver A's search for r = 4 "did not finish"; it was never run
  on the full problem.
- "Computed over every tree" was claimed for all three instances; solver A's
  values for r = 1, 2 come from its search.
- "No better upper bound is known" claimed more than a literature search that
  read no source in full can support.
- The remark about solver A's wait loop had no source in the folder.
- "An error could only make the value too small" was stated without its
  reason.

The corrected wording has not been through a second round.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: searches, witnesses (`sylvester_r*.txt`,
  `witness_r*.txt`), checkers, proofs.
- `verifier/`: the referee's brute force, search, witness checker and lemma
  tests.
- `prior-art.md`: the literature search.

Binaries are left out.
