# Experiment 9: the slipped comparator

A sorting network built only from comparators on neighbouring wires. One
comparator, chosen by an adversary, is wired one position too high or too low.
How many extra comparators does it take to sort anyway?

**Short answer: exactly n − 1 extra, for every n ≥ 3.** The smallest network
that survives one slip has (n − 1)(n + 2)/2 comparators, against n(n − 1)/2
with no faults. For n ≥ 3, surviving a slip turns out to be the same as
surviving the removal of any one comparator. Proved by hand, two different
ways for the lower bound; computed for n = 3 … 7.

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
| For n ≥ 3 a word is slip-proof if and only if it is deletion-proof | proved by hand by one solver; referee read it and compared the two sets of words for n = 3 (length ≤ 9), n = 4 (length ≤ 10) and n = 5 (size 14 only); I read it line by line | `solver-a/proofs.txt` T1 |
| A(n) ≥ N + n − 1 | proved by hand, by two different arguments; referee read both; I read the first line by line | `solver-a/proofs.txt` T2, `solver-b/proofs.txt` L5 |
| S(n) ≥ N + n − 1 directly, through words that survive slips from c_i to c_(i+1) only | proved by hand for n ≥ 4, resting on one computed base case (n = 4; saved output in `solver-b/runup.txt` and `verifier/out_lemmas.txt`), with a separate hand check for n = 3; referee read it and recomputed the base | `solver-b/proofs.txt` L4 |
| Insertion sort with its last pass repeated has N + n − 1 comparators and is deletion-proof | proved by hand by solver A (one trivial case, the all-ones input, is not written out); I read it line by line; checked by program for n ≤ 12. Solver B's separate proof has a false step, see Review | `solver-a/proofs.txt` T3 |
| **S(n) = A(n) = (n − 1)(n + 2)/2, so D(n) = n − 1, for every n ≥ 3** | follows from the rows above | |
| S(3 … 7) = 5, 9, 14, 20, 27, with no slip-proof word one shorter | computed by three separately written programs of similar design (search over prefixes, tested on the reversed input, with dominated states dropped at n = 7); two of them also work straight from the definition for n ≤ 5 | `reports/` |
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
exception shows the edge of this: if a comparator can only slip from c_i to
c_(i+1), a letter c_(n−1) cannot slip at all, and for n = 3 four comparators
suffice instead of five.

## Why n − 1 extra are needed

Take a word that survives any deletion and run it on the reversed input. Each
of the N pairs of values is swapped by exactly one comparator. For each v,
delete the comparator that swaps v and v + 1. The run then proceeds exactly as
before with v and v + 1 trading places, until some later comparator meets
those two values and repairs them. Such a comparator must exist, or the
damaged word would end unsorted. In the undamaged run it has nothing to do.
There is one such idle comparator for each v = 1 … n − 1, and they are all
different because each holds a different pair of values.

## What to distrust

- **Everything is by one model family.** "Proved by hand" means one instance
  wrote it and others read it. Nothing is machine-checked as a proof.
- **This may be known.** The literature search ([prior-art.md](prior-art.md))
  found that the upper bound is probably a special case of a known fact about
  subword complexes in Coxeter combinatorics (Ceballos, Labbé, Stump 2011),
  where "every letter can be deleted" has a standard meaning; the search did
  not check that identification against the paper. Removal of a comparator is
  the "passive fault" of Yao and Yao (1985), so A(n) is not a new notion, only
  its exact value for adjacent comparators. The search did not find
  the lower bound or the slip model, but it read no source in full, and the
  lower bound is short enough to be a remark somewhere. The slip fault model
  itself was not found anywhere.
- **The problem turned out easier than the chain expected.** The chain's
  doubt, that a misplaced comparator might do useful work, has a three-line
  answer.
- **n = 7** was computed with a pruning rule (dropping dominated states) that
  all three programs use; it was checked against unpruned runs only up to
  n = 6.
- **Solver B's upper-bound proof is wrong** in one step (L2(c)); see Review.
  The upper bound rests on solver A's T3 and on the program check for n ≤ 12.
- **Small errors** in the proof files are listed in a note at the top of each;
  the bodies are left as written.
- **Two of the referee's "broken" witnesses are not broken**
  (`verifier/wit_bad.txt`, noted in `reports/referee.md`), and its test of
  solver B's L2 invariant could not have caught the false step.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files. All five confirmed
the monotonicity lemma, the equivalence of slips and deletions (T1), the
lower bound (T2), solver B's lower-bound arguments (L4, L5), and that the
headline follows for n ≥ 3. What they found:

- **Solver B's proof of the upper bound has a false step** (Grok). L2(c)
  says that inserting a new value into a vector that is sorted except for one
  low-lying value x leaves it sorted or again out of place only in x. With
  wires holding 1, 4, 5, 2 and a new value 3 below a 6, the result is
  1, 2, 4, 5, 3, 6: the misplaced value is now 3, not 2. The construction
  still works and the argument looks repairable (let the misplaced value
  change), but it is not repaired here. Solver A's proof T3, which works on
  zeros and ones, does not have this problem; it omits only the all-ones
  input, which every comparator leaves alone.
- My explanation of the lower bound did not say the word must survive
  deletions, without which it is false. Fixed.
- "Three independent programs" overstated how different the programs are.
- "Upward slip" named a move to a lower wire. Reworded.

The corrected wording has not been through a second round.

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
