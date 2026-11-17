# Experiment 6: one feedback bit

A sender transmits n bits over a channel that may silently delete one of
them. Once, at a moment fixed in advance, the sender is told whether a
deletion has happened so far. When should that moment be?

**Short answer: not always after the first bit.** The chain guessed that
asking after the first bit is always best. It is best up to 5 bits. At 6 bits,
asking after the second bit lets 13 messages through where asking after the
first allows 12. For 7 and 8 bits the best moment is after bit 3 (or 4).
The counterexample is a list of 13 strategies that anyone can check in a few
lines of code; `check_headline.py` is mine.

## The problem

Fix n ≥ 2 and 0 ≤ t ≤ n. The sender sends exactly n bits. An adversary who
knows everything may delete at most one; the receiver sees the surviving bits
in order and nothing else. After sending bit t and before bit t + 1, the
sender learns whether one of the first t bits was deleted (not which).

So a strategy is a triple (u, a, b): send u (t bits), then a if told "no
deletion", b if told "deletion". For a string s let D(s) be the strings
obtained by deleting one symbol. What the receiver may see is

    O(u, a, b) = { ua } ∪ { u a′ : a′ in D(a) } ∪ { u′ b : u′ in D(u) }.

F(n, t) is the largest number of strategies whose sets O are pairwise
disjoint, that is, the number of messages that can be sent without error.
F(n) is the best over t. A(m) = F(m, 0) is the size of the best
single-deletion-correcting code of length m, with no feedback.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): is
F(n) = 2·A(n − 1) for every n ≥ 3, i.e. is t = 1 always optimal?

## Results

| Claim | Status | Where |
|---|---|---|
| F(n, 1) = 2·A(n − 1) | proved by hand, twice; referee read both | `solver-a/proofs.txt` P4 |
| F(n, 0) = F(n, n) = F(n, n − 1) = A(n), and F(n, t) ≥ A(n) | proved by hand, twice; referee read both | `solver-a/proofs.txt` P1–P3 |
| F(n, t) ≤ 2·F(n − 1, t − 1) | proved by hand by one solver; referee read it | `solver-b/proofs.txt` P6 |
| The fewest shortened outcomes of a strategy is r(u) + r(a) − 1 (r = number of runs), reached exactly when b = u_t a′ for some a′ in D(a) | the minimum proved by hand twice; "exactly when" by solver A only; brute-forced for n ≤ 9 | `solver-a/proofs.txt` P5 |
| **F(6, 2) ≥ 13 > 12 = 2·A(5): the answer is no** | 13 explicit strategies (`solver-a/wit_6_2.txt`), checked from the definition by both solvers' checkers, the referee's, and `check_headline.py`; A(5) ≤ 6 by exact-arithmetic certificates (both solvers) and by exhaustive search (the referee and `check_headline.py`) | below |
| n = 6 is the first failure: F(n, t) ≤ 2·A(n − 1) for n = 3, 4, 5 and all t | computed; dual certificates checked in exact arithmetic by three checkers | `solver-a/cert_*`, `solver-b/cert_*` |
| Table of F(n, t) for n ≤ 8 (below) | computed; two solvers and the referee agree on every entry. Lower bounds are explicit lists, all checked. Upper bounds: an exact-arithmetic certificate where one exists, the lemma F(n, 1) = 2·A(n − 1) for t = 1, otherwise exhaustive search | `reports/` |
| F(9) ≥ 67 > 60 = 2·A(8) | lower bound by a checked list; A(8) ≤ 30 by certificate | `solver-a/wit_9_3.txt` |
| F(9, 3) ≥ 67 and F(9, 4) ≥ 67 | lower bounds by checked lists. The matching upper bounds (67 and 68) and F(9, 7) ≤ 60 come from one solver run each with no certificate, and are not established | |
| F(n) = (1 + o(1))·2^n / n | proved by one solver given two classical facts; one constant in it is misstated (see below) | `solver-a/proofs.txt` P6 |
| F(n) > 2·A(n − 1) for every n ≥ 6 | a guess; true for n = 6, 7, 8, 9 | |

F(n, t), for t = 0 … n. Bold entries exceed 2·A(n − 1).

| n | t=0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 2·A(n−1) |
|---|---|---|---|---|---|---|---|---|---|---|
| 2 | 2 | 2 | 2 | | | | | | | 2 |
| 3 | 2 | 4 | 2 | 2 | | | | | | 4 |
| 4 | 4 | 4 | 4 | 4 | 4 | | | | | 4 |
| 5 | 6 | 8 | 8 | 8 | 6 | 6 | | | | 8 |
| 6 | 10 | 12 | **13** | 12 | 12 | 10 | 10 | | | 12 |
| 7 | 16 | 20 | **21** | **22** | 20 | 20 | 16 | 16 | | 20 |
| 8 | 30 | 32 | **37** | **38** | **38** | **35** | **34** | 30 | 30 | 32 |

So F(n) = 4, 4, 8, 13, 22, 38 for n = 3 … 8.

## The counterexample

n = 6, feedback after the second bit. Each line is u, a, b:

    00 0000 0000    01 0000 0101    10 0000 1101    11 0000 1000
    00 1100 0100    01 0011 0001    10 1100 0010    11 0011 1001
    00 1111 0111    01 1100 1010                    11 1100 1100
                    01 1111 0011                    11 1111 1111

With feedback after the first bit the sender has two prefixes, and behind
each a deletion code of length 5, which holds 6 words: 12 in all. With
feedback after the second bit there are four prefixes and less room behind
each. This packing fits 3 + 4 + 2 + 4 = 13 behind the prefixes 00, 01, 10,
11.

That is a description of one packing, not an explanation of why 13 is
possible; nothing here analyses the mechanism, and this is one of 375,360
optimal packings (the referee's count). In it, 1100 is used behind all four
prefixes and 0011 behind 01 and 11. The chain's reasoning was that for
feedback in the middle, every strategy with a small outcome set is forced
onto a few constant-run strings, as it believed happens at n = 5. Even there
one optimal packing uses a = 110 (`solver-a/wit_5_2.txt`).

## What to distrust

- **The headline needs very little trust.** It is two finite facts: 13 listed
  strategies have disjoint outcome sets, and no 7 binary words of length 5
  have disjoint deletion sets. Both take a few lines to check.
- **The rest of the table needs more.** For (6,2), five entries at n = 7 and
  three at n = 8 the linear-programming bound is not tight, so there is no
  short certificate and the upper bound is a search. Both solvers used the
  same solver library for those and even returned the same 13 strategies out
  of 375,360 optimal packings, so their agreement was weaker evidence than it
  looked. Solver A's separate search `bb2.cpp` agrees on all of n = 6. The
  referee re-derived the uncertified bounds with its own exact search,
  LP-free for (6,2) and using the library only to propose weights elsewhere.
- **n = 9** is lower bounds only, apart from F(9, 1).
- **The referee's logs include failed attempts.** `verifier/dp_7.log` shows
  its first search running out of memory on the interior n = 7 cases; the
  referee's report does not rely on that program there (it used a
  branch-and-bound and the certificates instead).
- **The header of `solver-a/wit_6_2.txt`** says "dualbound=13.0". That is the
  integer-program bound. The linear-programming bound for (6,2) is 14, so no
  short certificate exists for F(6, 2) ≤ 13.
- **A(9) = 52 and A(10) = 94**, quoted in the chain's question, were not
  recomputed. The values A(1 … 8) were.
- **Solver A's asymptotic proof** says a tail probability is "at most
  n^−2"; the bound it cites gives (1 + o(1))·n^−2. The conclusion needs only
  o(1) and stands (referee's note). Solver B calls the same statement a
  sketch.
- **Everything is by one model family**, and nothing is machine-checked as a
  proof.
- **Prior art** ([prior-art.md](prior-art.md)): the sizes A(n) are classical
  (Varshamov–Tenengolts codes, Sloane's survey). Feedback codes for one
  substitution error with one-time feedback are in print (Vorobyev, Deppe,
  Lebedev and others), and feedback codes for many deletions with full
  feedback (Maringer and others, 2020). The search did not find one deletion
  with one feedback bit. It read no source in full and could not search the
  Russian-language literature where such a result would most likely be.

## Review

Before publication the claims went to five reviewers from four model
families. The Grok seat failed without an answer, so four reported (two
Claude, GPT, Gemini: three families). All four checked the 13 strategies and
A(5) = 6 themselves and confirmed them, and confirmed the model, the
reduction to packing, every lemma, and the soundness of the certificate
checkers. Their findings, applied above:

- "Proved twice" was wrong for the "exactly when" half of the run-count
  lemma; only solver A proves it.
- My explanation of the counterexample misdescribed which strings sit behind
  which prefixes and presented a restatement of disjointness as a mechanism.
- "A script of mine" was cited but not in the folder. It is now
  (`check_headline.py`).
- "Drifts towards the middle" rested on two values of n.
- GPT rated the out-of-memory log as critical. It is a failed attempt that
  the referee's conclusions do not use; noted above.

Loose statements in the two proof files are listed in a note at the top of
each. The corrected wording has not been through a second round.

## Files

- `chain/`: the seed and the three links, verbatim, with the question each
  link handed on.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: solvers, strategy lists (`wit_*`), upper-bound
  certificates (`cert_*`), checkers, proofs.
- `verifier/`: the referee's own searches and checkers.
- `check_headline.py`: my check of the two facts behind the headline.
- `prior-art.md`: the literature search.

Binaries are left out.
