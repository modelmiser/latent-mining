# Open-growth report

Proofs: `open-growth/proofs.txt`. Tests and outputs: `open-growth/t_*.py`, `t_*.out`, `bound.py`, `bound.out`.

## What is now proved

**Theorem 1 (COMPLETE).** For every n >= 7 no loop of length 2^n + 2 is repairable. With the computed case n = 6 this gives M(n) >= 2^n + 3 for all n >= 6. The proof does not assume w universal.

**Theorem 2 (COMPLETE).** M(n) - 2^n is unbounded, with rate at least 2^n/n:

    M(n) >= 2^n + (2^(n-3) - 19 n^2) / (19 n)     for all n.

Precisely, for a repairable loop of length 2^n + e, n >= 4, any 2 <= k <= n, m = n - k:

    2^(n-2) - 2 <= k(2^m+1)(e+k-1) + 2 sum_{d=2}^{k-1} (k-d) 2^(max(m,d)-1) + 2(k-1)(e+n-1).

Best k (`bound.out`): trivial up to n = 13, e >= 9 at n = 15, e >= 418 at n = 20, e >= 272775 at n = 30. So the excess is not bounded.

## The two ideas

Both count windows of length k < n: in a universal loop every k-word occurs at least 2^(n-k) times, and one edit creates at most k-1 new k-windows (k >= 2).

*Theorem 1* uses k = 3. Four adversary moves (delete the middle of three consecutive singletons; a cell of a 2-run; an isolated 1; an isolated 0) force the excess of w on 3-words to be exactly 1 on 010 and 101, 0 elsewhere. Then the reply to deleting the middle of 10101 is forced (delete a 0 of a run 00 between 1s), so w is a de Bruijn cycle plus one alternating pair. The reply to deleting an isolated 1 is also forced (double a 0 in 101), and the result must contain a 0-run of length exactly n+2 and no other 0-run of length >= n-1. That can only happen if the deleted 1 sat between the 0^n run and a run 00. For n >= 7 the word 0001000 gives an isolated 1 between two 0-runs of length >= 3: contradiction.

*Theorem 2* uses k = n-4. Key lemma: for a singleton s none of whose junction k-words equals a k-word through s, some k-window through s carries a word with more than 2^(n-k) occurrences. Such windows number at most 17(e+k-1), each holding at most k singletons. The solvers' sketches stalled at k = n because junction coincidences cannot be bounded there; at k = n-4 they number less than 2^(n-3), against 2^(n-2) singletons.

## Mechanical tests (a deliberately false variant was run and failed for each, except the n = 4, 5 structure check)

- Window-count edit formulas: 8000 random edits, k = 2..6, 0 mismatches. It caught an error of mine (a doubling destroys k-2 windows, not k-1); the proofs use the corrected form.
- Theorem 1, steps 4-6: all 41 local mechanic moves enumerated on 3-window counts; forced excess vector and forced replies as stated.
- Theorem 1, step 7: 199 random loops "de Bruijn + pair", n = 4..9, literal brute-force replies: of 756 isolated singletons every answerable one has neighbour runs {n, 2}; the 172 with both runs >= 3 are never answerable.
- All 216 + 20 repairable loops of length 2^n + 2 for n = 4, 5 are "de Bruijn + pair" with the forced excess vector.
- Key lemma of Theorem 2: every singleton of 1991 known repairable loops (n = 3..6), every k: 58364 cases, 0 violations; violated by 80 of 80 random de Bruijn cycles; variants "right side 2" and "ignore junction" fail.
- Coincidence count: exhaustive over all loops of length 9..15 for n = 3, 4 (160256 cases) plus 2492 cases up to n = 10; 0 violations, bound attained.

## What failed or was not done

- **Linear rate, M(n) >= (1+d) 2^n: not proved** (Sketch A). Missing: that one mechanic edit serves only O(1) singletons.
- **Upper bound (task 3): nothing.** The one construction analysed on paper is longer than 2^(n+1); none was run.
- **67 <= M(6) <= 76 (task 4): not attempted.**
- Theorem 1 does not cover n = 6 by hand (0001000 does not fit).
- Theorem 2 cannot be tested end to end (first non-trivial at n = 14); only its lemmas are tested.

## Still open

Whether M(n) - 2^n is Theta(2^n/n), Theta(2^n), or in between; any upper bound below 2^(n+1); M(6); M(n) >= 2^n + 4 for 7 <= n <= 14.

## Confidence

- Theorem 1: high (about 90%). Forced steps are machine-enumerated; the existence arguments of step 0 are by hand.
- Theorem 2: high (about 85-90%). Each lemma tested at small sizes; the hand part is the counting in G4.
- Both rely on the solvers' Facts A, B, C and P2, which the referee confirmed.
