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
touched. Whether D(r) equals the sequence for every r is open. A follow-up
on 21 November proved that it does for every r when no position may be read
at two places in the tree, so a longer cycle would have to come from reusing
a position in different branches; for all trees the best upper bound is
still within a factor 2^r/(r + 1) of the number of memory states (see
"Towards an upper bound"). The chain bet the other way, on a cycle
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
| On every cycle, at most one step uses a leaf that writes nothing (Theorem E) | at first computed for r ≤ 3 (solver B); since 21 November proved by hand for every tree by one follow-up instance, tested on all trees of depth ≤ 4; five reviewers traced the proof and found no gap, and two of them tested the theorem with their own programs, one of those also the construction of the auxiliary tree (see Review) | `upper/PROOFS.md` §1, `solver-b/prod3.cpp` |
| If the two subtrees below the root share no position, the longest cycle is at most M₁·(M₀ + 1), where M₀, M₁ are the longest cycles of the two subtrees with the leaves that write the root's position made empty (Theorem RD) | proved by hand by the follow-up instance, using Theorem E; tested on 383,161 trees of depth ≤ 3 | `upper/PROOFS.md` §2 |
| **For trees in which no position labels two nodes, every cycle has length at most L of the tree, and every shape has such a labelling that attains it; so the longest cycle over these trees of depth at most r is exactly W_r, for every r** (Theorem RO; L(leaf) = 1, L(node) = L(1-child)·(L(0-child) + 1)) | proved by hand by the follow-up instance, from Theorem RD and the construction of the lower bound | `upper/PROOFS.md` §3 |
| For r ≥ 5, a tree of depth r with a cycle longer than W_r must reuse a position at two nodes whose lowest common ancestor has depth at most r − 5 (the root has depth 0); at depth 5 that ancestor is the root | follows from Theorem RD and the computed D(ρ) = W_ρ for ρ ≤ 4 | `upper/PROOFS.md` §2 |
| D(r) ≤ (r + 1)·2^(2^r − 1 − r) for every r ≥ 1: 6, 64, 10240, about 4.03 × 10⁸ for r = 2, …, 5 (Theorem G) | proved by hand by the follow-up instance | `upper/PROOFS.md` §4 |
| ~~Every cycle of every tree has length at most L(T)~~ **False.** It holds for every tree of depth ≤ 4 (computed), but the depth-5 tree `(1 (2 (3 [2] (4 [4] (5 [5] []))) [1]) (3 (6 [6] (7 [7] [3])) []))` has L = 50 and a cycle of length 52; two larger trees of the same family give 114 against 98 and 762 against 602. All are far below W_5, so D(r) = W_r is not affected | this page carried the statement as a conjecture (from `upper/PROOFS.md`) for about an hour on 21 November (commit `70f0c0d`); a further instance, asked to prove it, refuted it. The three cycles are checked by its two programs, by the organiser's own simulator, and by the separate simulators of two reviewers (see Review) | `shape/PROOFS.md` §1, `shape/cex_*.txt`, `shape/organiser_check_out.txt` |
| A cycle that never visits a state with zeros on the whole all-zero path has length at most the length of that path (Lemma A) | proved by hand by that instance | `shape/PROOFS.md` §2 |
| If the subtrees hanging off the all-zero path share no position other than those on that path, and the same holds recursively inside each of them once the nodes reading path positions are removed, then every cycle has length at most W_r at depth r (Theorem Z). This class contains the trees that read each position once, the two small examples of `upper/PROOFS.md` §5 (the first is shown below), and the counterexample above | proved by hand by that instance, using Theorem E; its counting bound, Lemma A and the bound by W_depth were tested on 6,000 random trees satisfying the top-level condition (not necessarily in the whole recursive class) and on three fixed examples, which checks the bookkeeping and is not strong evidence; five reviewers traced the proofs and found no gap (see Review) | `shape/PROOFS.md` §3, `shape/out_testZ.txt` |
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

## Towards an upper bound

This section follows `upper/PROOFS.md`, the work of one follow-up instance on 21 November; its last paragraph follows `shape/PROOFS.md`.

*One empty step per cycle.* Call a leaf that writes nothing empty. Look at
the positions on the all-zero path of the tree. A step that reads a 1 never
raises the number of 1s; only the all-zero leaf can. If a cycle never visits
a state with zeros on that whole path, its weight is constant and it has no
empty step. Otherwise every state of the cycle has at most one 1 on the
path, and the cycle splits into excursions between consecutive states that
are zero on it. Each excursion ends with exactly one step that writes off
the path or writes nothing. The proof builds a tree on fewer positions (those off the path only; it may
be deeper than the original), one step of which reproduces a whole excursion
and is empty exactly when the excursion ends with an empty step; induction
on the number of positions finishes.

*Disjoint subtrees multiply.* If the subtrees T₀ and T₁ below the root share
no position, then while the root bit is 0 only T₀'s memory moves, and while
it is 1 only T₁'s. A change of the root bit from 0 to 1 is an empty step of
T₀ (with its leaves that write the root made empty), and T₀'s cycle has only
one of those. So every state with root bit 1 sees the same memory of T₀: at
most M₁ such states, and at most M₀·M₁ with root bit 0.

*Consequences.* For trees that read every position at one node only, the
recursion t → t(t + 1) is therefore forced, shape by shape, and the
construction of the lower bound attains it. The conjecture D(r) = W_r is
now equivalent to: reusing a position in different branches never helps.
Sharing does break the product bound: the tree
`(1 (2 [1] []) (2 [2] [2]))` of depth 2 (root reads position 1, both
children read position 2; brackets are what a leaf writes) has a cycle of
length 3, where the product bound would give 2. It is still below
L = 6. The follow-up lists five natural strengthenings (the product bound for all trees among them) of "length ≤ L(T)" that would allow an induction and are false at depth 3.

*The shape bound itself is false.* The first follow-up conjectured that
every cycle of every tree is at most L(T). A later instance found the
depth-5 tree in the Results table: its all-zero path reads positions 1, 2,
3, the leaf at its end writes 2, and the 1-child of the node reading 2 is a
single leaf that writes 1. Every return to the all-zero path therefore
costs three states before the counter under the root's 1-child moves. That
relay leaf counts 1 in L, but its real cost is one level of depth for
everything further down, which L does not see and the Sylvester value for
the depth does. So the right target is the bound by depth, W_r, not by
shape. The same instance proved that bound for trees whose subtrees off the
all-zero path share no position except those on the path (recursively);
positions shared between two of those subtrees, at any level of that recursion, are the case still open (at depth 5, only at the top level).

## What to distrust

- **A conjecture printed here was false.** "Length ≤ L(T)" was computed for
  depth ≤ 4 and stated as a conjecture; it fails at depth 5. Agreement up to depth 4 said nothing about depth 5; in this family of trees, depth 4 leaves no room for the relay to pay. `upper/PROOFS.md` still states it as Conjecture P1. The new theorems in `shape/PROOFS.md` are by one instance and hours old; reviewers noted that its formula for the family of counterexamples leaves one hypothesis unstated (the counter under the root's 1-child must use its empty exit once per cycle), which the three printed trees satisfy, and that its output file prints the witness lines of a deliberately broken run directly under the summary of the clean run.
- **The upper-bound results date from 21 November and have one author.** Theorem
  E's proof builds an auxiliary tree by a recursive rule with a special case
  for repeated modes; that construction was tested by program on 40,000
  trees and the theorem on all trees of depth ≤ 4, which is evidence and not
  a check of the proof. Theorems RD and RO depend on it. By its own account the organiser
  read the proofs and traced the depth-2 example by hand. Reviewers asked
  for three clarifications inside `upper/PROOFS.md`, which is left as the
  instance wrote it: the rule that puts a dummy leaf should say "if this
  mode has already been entered with the same set K on the way from the root
  of F (the first mode counts)"; the reason a dummy is never reached is that
  T's state at a stage is determined by the mode and K, so a repeat means
  the orbit never returns home; and in case II of Theorem RD every step is
  empty in T₁° because a step that keeps the root bit at 1 must rewrite it. The depth-4 program
  of the follow-up follows the referee's search design, so it is not an
  independent confirmation of D(4) = 1806. Nothing is machine-checked.
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

The upper-bound results of 21 November went to the same five reviewers.
All five confirmed every proof: the two facts about weights, both cases of
Theorem E including the construction of the auxiliary tree and its
finiteness, Theorem RD and its corollaries, Theorem RO, Theorem G and its
values, and both small examples, which they replayed. Both Claude seats
also wrote their own programs: over 8,000 and 18,500 random trees with
heavy reuse of positions they found no cycle with two empty steps and no violation of the product bound for disjoint subtrees; the seat with 8,000 trees also found no cycle longer than L; the other built the auxiliary tree from the proof's text for 1,934 cycles
and found it to behave as claimed each time. Their findings, applied above:

- I stated Theorem RO as "the longest cycle is exactly L", which is false
  for an individual tree (GPT, a Claude seat).
- "Separate at depth r − 5 or higher" read the wrong way round (Grok, both
  Claude seats).
- "Four" strengthenings for five; "a smaller tree" for one that can be
  deeper; "far below" for 3 against 6; "close to the number of states".
- I wrote that I had traced both small examples; I traced one.
- Three places where the proof file is terse, now listed under "What to
  distrust".

A second-round reviewer (one Claude instance) then checked the edited page against the files and the reviewers' reports; its corrections, to how much of the evidence came from which reviewer, are applied.

The counterexample and Theorem Z went to the same five reviewers. All five
confirmed the counterexample from the definition (legal, depth 5, L = 50,
cycle 52), Lemma A, Theorem Z1 and Theorem Z, and that the new class is
strictly larger than the read-once trees. Both Claude seats wrote their own
simulators and found the same three cycles; one tested every step of
Theorem Z1's proof on 485,059 cycles of 147,992 trees satisfying its
hypothesis, with no violation, and saw the checks fail when the hypothesis
is dropped. Their findings, applied above: the test corpus was misdescribed
(four seats); "the two small examples below" where one is on the page; the
recursion is on pruned subtrees; "a few hours" for about one. These edits
have not been reviewed again.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: searches, witnesses (`sylvester_r*.txt`,
  `witness_r*.txt`), checkers, proofs.
- `shape/`: a second follow-up of 21 November: the counterexample to the
  shape bound, Lemma A and Theorem Z, programs and outputs.
- `upper/`: the follow-up of 21 November: `PROOFS.md`, its programs and
  outputs, its brief and report.
- `verifier/`: the referee's brute force, search, witness checker and lemma
  tests.
- `prior-art.md`: the literature search.

Binaries are left out.
