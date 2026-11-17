# Experiment 9: the slipped comparator

A sorting network built only from comparators on neighbouring wires. One
comparator, chosen by an adversary, is wired one position too high or too low.
How many extra comparators does it take to sort anyway?

**Short answer: exactly n − 1 extra, for every n ≥ 3.** The smallest network
that survives one slip has (n − 1)(n + 2)/2 comparators, against n(n − 1)/2
with no faults. Surviving a slip turns out to be the same as surviving the
removal of any one comparator. Proved by hand, two different ways for the
lower bound; computed exhaustively for n = 3 … 7.

## The problem

Wires are 1 … n from top to bottom. The comparator c_i puts the smaller of the
values on wires i and i + 1 on wire i. A word is a sequence of such
comparators, applied left to right. It sorts if it sorts every input.

A slip of a word replaces one letter c_i by c_(i−1) or c_(i+1) (staying inside
c_1 … c_(n−1)). A word is slip-proof if it sorts and every slip of it sorts.
S(n) is the least size of a slip-proof word, and D(n) = S(n) − n(n − 1)/2.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): is
D(n) = n − 1 for every n ≥ 3?

## Results

N = n(n − 1)/2. "Deletion-proof" means the word sorts and still sorts with any
one letter removed; A(n) is the least size of such a word.

| Claim | Status | Where |
|---|---|---|
| Inserting a comparator into a sorting word leaves it sorting | proved by hand; referee read it | `solver-a/proofs.txt` L1 |
| For n ≥ 3 a word is slip-proof if and only if it is deletion-proof | proved by hand by one solver; referee read it and tested it word by word for n ≤ 5; I read it line by line | `solver-a/proofs.txt` T1 |
| A(n) ≥ N + n − 1 | proved by hand, two independent proofs; referee read both; I read the first line by line | `solver-a/proofs.txt` T2, `solver-b/proofs.txt` L5 |
| S(n) ≥ N + n − 1 directly, through words that survive upward slips only | proved by hand, resting on one computed base case (n = 4); referee read it and recomputed the base | `solver-b/proofs.txt` L4 |
| Insertion sort with its last pass repeated has N + n − 1 comparators and is deletion-proof | proved by hand, twice; referee read both; checked by program for n ≤ 12 | `solver-a/proofs.txt` T3, `solver-b/proofs.txt` L2 |
| **S(n) = A(n) = (n − 1)(n + 2)/2, so D(n) = n − 1, for every n ≥ 3** | follows from the rows above | |
| S(3 … 7) = 5, 9, 14, 20, 27, with no slip-proof word one shorter | computed exhaustively by three independent programs | `reports/` |
| Number of smallest slip-proof words for n = 3 … 7: 2, 24, 1912, 1210080, 7144675968 | computed by three programs; no formula | |
| Odd-even transposition sort with n + 2 rounds is slip-proof | computed for n ≤ 12 only; not proved | `solver-a/oddeven.txt` |

## Why a slip is a deletion

By the 0-1 principle it is enough to look at inputs of zeros and ones.

- A comparator that does something sees a 1 above a 0. Move it one wire down
  and it sees that 0 on top: it does nothing. Move it one wire up and it sees
  that 1 at the bottom: it does nothing. So a slipped comparator that mattered
  is simply gone.
- A comparator that did nothing can only help when moved, because an extra
  comparator on neighbouring wires never unsorts anything.

So the adversary gains nothing from choosing where the comparator goes. One
exception shows the edge of this: if only upward slips are allowed, the last
comparator c_(n−1) cannot slip at all, and for n = 3 four comparators suffice
instead of five.

## Why n − 1 extra are needed

Run the word on the reversed input. Each of the N pairs of values is swapped
by exactly one comparator. For each v, delete the comparator that swaps v and
v + 1. The run then proceeds exactly as before with v and v + 1 trading
places, until some later comparator meets those two values and repairs them.
In the undamaged run that later comparator has nothing to do. There is one
such idle comparator for each v = 1 … n − 1, and they are all different.

## What to distrust

- **Everything is by one model family.** "Proved by hand" means one instance
  wrote it and others read it. Nothing is machine-checked as a proof.
- **This may be known.** The literature search ([prior-art.md](prior-art.md))
  found that the upper bound is probably a special case of a known fact about
  subword complexes in Coxeter combinatorics (Ceballos, Labbé, Stump 2011),
  where "every letter can be deleted" has a standard meaning. It did not find
  the lower bound or the slip model, but it read no source in full, and the
  lower bound is short enough to be a remark somewhere. The slip fault model
  itself was not found anywhere.
- **The problem turned out easier than the chain expected.** The chain's
  doubt, that a misplaced comparator might do useful work, has a three-line
  answer.
- **n = 7** was computed with a pruning rule (dropping dominated states) that
  all three programs use; it was checked against unpruned runs only up to
  n = 6.
- **Small errors** in the proof files, listed in `reports/referee.md`: one
  uncovered trivial case in `solver-a/proofs.txt` T3(i), and a stale range in
  `solver-b/proofs.txt`. The files are left as written.

## Files

- `chain/`: the seed and the three links, verbatim, with the question each
  link handed on.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: search programs, witnesses, checkers, proofs.
- `verifier/`: the referee's own programs and lemma tests.
- `prior-art.md`: the literature search.

Binaries are left out.
