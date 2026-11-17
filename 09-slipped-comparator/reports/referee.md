# Referee report

Headline: **CONFIRMED.** D(n) = n-1 and S(n) = (n-1)(n+2)/2 for every n >= 3. Both lower-bound proofs hold, by different routes, and my own code reproduces every table entry for n = 3..7. The errors found are small.

Code and outputs: `verifier/`.

## Verdicts

- **CONFIRMED, recomputed:**
  - S(3..7) = 5, 9, 14, 20, 27, with zero words at size S-1, and A(n) = S(n).
  - Minimum word counts 2, 24, 1912, 1210080, 7144675968 (slip and deletion alike).
  - B's up-slip column: u = 4, 9, 14, 20, 27; counts 1, 64, 3860, 2611264, 16896927944.
  - A's counts by size (n=3: 0, 2, 16, 58; n=4: 0, 24, 444).
- **CONFIRMED, witness checked and proofs read:** W_n is slip-proof and deletion-proof (n = 3..12).
- **CONFIRMED, proof read with no gap, lemma tested:** slip-proof iff deletion-proof (A's T1); both lower bounds (A's T2; B's L4 with base u(4) = 9 recomputed).
- **UNSUPPORTED:** odd-even transposition with n+2 rounds is slip-proof for all n. True for n = 3..12; neither solver claims a proof.

Recomputation: `brute.cpp` (n = 3..5, definition-level, slip-proof words re-tested on all n! inputs) and `dp.cpp` (n = 3..7, layered count on the reversed input). They agree for n <= 5; n = 7 ran with dominance reduction only (validated against no reduction for n <= 6).

## Proofs, checked separately

- **A's T1**: for n >= 3 a word is slip-proof exactly when it is deletion-proof. No gap found.
  - Pointwise test (an active letter's slip acts as its deletion on 0-1 inputs): 0 failures in 82,870 exhaustive n=4 cases and 1.46 million random cases for n = 5..8.
  - Word by word, the two sets are identical for n=3 (length <= 9), n=4 (length <= 10) and n=5 (size 14: 1912 words each, symmetric difference 0).
  - n=2 fails as stated: `c_1` is slip-proof vacuously, not deletion-proof.
- **A's T2**: a deletion-proof word has n(n-1)/2 active and at least n-1 idle letters on the reversed input. No gap found.
  - Tested: deleting the letter that exchanges v+1 and v still sorts exactly when a later letter compares v and v+1. 0 failures over all sorting words for n=3 (length <= 9), n=4 (length <= 10), and random ones for n = 5..7.
- **B's L4**: u(n) >= u(n-1) + n for up-slip-proof words, giving S(n) >= n(n+1)/2 - 1 from u(4) = 9. No gap found.
  - Base recomputed from the definition: no up-slip-proof word of size <= 8 for n=4, 64 at size 9; u(3) = 4, unique word `2 1 1 2`.
  - Decomposition tested on every U-word for n=3 (length <= 9) and n=4 (length <= 11), and 120 each for n = 5..7: 0 failures.
- **B's L5**: A(n) >= A(n-1) + n without computation. No gap found; 0 test failures.
- **B's L3** (S(3) >= 5 by hand): no gap found.
- **Upper bound** (A's T3, B's L2): W_n is deletion-proof. No gap found in either. Invariants tested after every block: A's Z_m on all 0-1 inputs for n = 3..10, B's "x-low" on all permutations for n = 3..7, 0 failures.
- **L0, L1**: no gap.

Four lemma tests were re-run in deliberately false versions; each failed as it should (`out_mutants.txt`).

## Disagreements between the solvers

No numerical disagreements; three differences:

1. **Do the minimum word sets coincide?** B calls it a conjecture; A proves it (T1), for every size. A is right.
2. **Proof route.** B's L4 needs a computed base case; A's T1 + T2 needs none.
3. **Does a slipped comparator ever help?** A says no; B says "partly" (u(3) = 4). Both are correct: B's case arises only because c_{n-1} has no up-slip.

## Errors found

- A's T3(i): "x sorted is the case c=0" does not cover x = 1^m. Trivial.
- B's `proofs.txt` says count equality for "n = 3..6"; B's report and data say 3..7.
- B's belief (a) check for n=5 cannot fail; B says so.
- A's n=7 lower bound has no size-26 run; B and I ran it: 0 words.
- Mine: two intended broken witnesses (n descending passes, n = 3, 4) are genuinely slip-proof, not minimal; not counted as rejections.

## Witness checker

`checkwit.py`: all n! inputs for n <= 7, all 0-1 inputs above.
- Accepted: all 35 solver witness lines (W_n for n = 3..12; odd-even, n+2 rounds, both parities) and my 30 rebuilt copies.
- Rejected: 61 broken words (W_n with a letter missing, plain insertion sort, last letter slipped, odd-even with n+1 or n rounds), plus two refused as invalid.

## Problem as written

Both solvers took the literal reading; I found no ambiguity. Beliefs (a), (b), (d) are true; (c) true but weak; (e) unproved.

## Not checked

- The solvers' C++ was not audited; only outputs were compared.
- n = 8 was not computed.
- The proofs were read by one model of the solvers' family.

## Follow-up tasks

1. Prove or refute: for every n >= 3, odd-even transposition sort with n+2 rounds (either parity) is deletion-proof, and with n+1 rounds it is not.
2. Two faults: let S_2(n) be the least size of a word that sorts after any two slips at distinct positions. Is S_2(n) = n(n-1)/2 + 2(n-1), and is two-slip-proof equivalent to two-deletion-proof? Compute n = 3..6 first.
