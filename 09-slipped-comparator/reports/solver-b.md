# solver-b report: slip-proof adjacent-comparator words

N = n(n-1)/2. Files: `solver-b/`.

## 1. Tables

| n | S(n) | D(n) | A(n) (deletion-proof) | u(n) (up-slips only, see §4) | # min slip-proof words | # min deletion-proof | # min up-slip-proof |
|---|---|---|---|---|---|---|---|
| 3 | 5 | 2 | 5 | **4** | 2 | 2 | 1 (word 2112) |
| 4 | 9 | 3 | 9 | 9 | 24 | 24 | 64 |
| 5 | 14 | 4 | 14 | 14 | 1912 | 1912 | 3860 |
| 6 | 20 | 5 | 20 | 20 | 1210080 | 1210080 | 2611264 |
| 7 | 27 | 6 | 27 | 27 | 7144675968 | 7144675968 | 16896927944 |

All entries COMPUTED, exhaustive.
- n=3,4 (all three columns): `slow.py`, the definition itself: every word of each length up to the answer, word and every variant tested on all n! inputs.
- n=3..6: `fast.cpp`, layered search over states (p, Q): p = image of the reversed input under the prefix, Q = set of images after exactly one slip. Run at L = S-1 (0 accepted) and L = S. Only the reversed input is tested, which is a necessary condition, so "0 accepted at S-1" is a rigorous lower bound without belief (a); upper bounds come from witnesses re-checked on all inputs. Pruning: drop a state when p or a member of Q has more inversions than letters left (a comparator removes at most one inversion). n=4 was also run unpruned: same counts.
- n=7: same program with `DOM=1`, which removes from Q every element dominated (0-1 dominance = Bruhat order) by p or another element; safe by proofs.txt L1(2). With and without DOM the counts agree for n=4..6. Without DOM n=7 exceeded the 8 GB cap.
- Outputs: `run6.txt`, `runup.txt`, `rundom.txt`, `run7dom.txt`.

## 2. Witnesses

`witnesses.txt`: W_n = I_2 I_3 ... I_n I_n with I_k = c_{k-1}...c_1 (insertion sort plus a repeated last pass), size N+n-1; e.g. n=5: `1 2 1 3 2 1 4 3 2 1 4 3 2 1`. Verified slip-proof and deletion-proof on all inputs for n=3..7 by `verify.py` (output `verify.out`). Deliberately wrong object (W_n minus its last letter) is rejected. "Next better impossible": the L = S-1 runs above, plus the proof in §4.

Also computed in `verify.py`: odd-even transposition sort is deletion-proof and slip-proof with n+2 rounds, and neither with n or n+1 rounds (n=3..7): belief (e) holds for these n, unproved in general. Belief (a): no counterexample among all words up to length 9 for n=3,4 (the n=5 line in verify.out is vacuous: no word that short sorts). Belief (d): proved (L1) and checked for n=4.

## 3. Headline

**Yes: D(n) = n-1, S(n) = (n-1)(n+2)/2 for every n >= 3.** Also A(n) = S(n). Confidence: high (about 97%); the residual doubt is an unnoticed error in my own proof text; all numerics for n=3..7 agree.

The problem's doubt ("the misplaced comparator might do useful work") is partly real: with up-slips only, n=3 needs just 4 letters (2112), one fewer than N+n-1.

## 4. Proofs (`proofs.txt`)

- L0 0-1 principle, L1 belief (d) (insertion never breaks a sorting word; so deletion-proof implies slip-proof, S <= A): COMPLETE.
- L2 upper bound, W_n is deletion-proof for all n: COMPLETE (invariant: after one deletion the vector is sorted except for one entry x that sits too low and is smaller than all entries below it; later insertion passes keep this; the final I_n repairs it).
- L3 S(3) >= 5 by hand: COMPLETE.
- L4 lower bound S(n) >= N+n-1 for n >= 4: COMPLETE **given one computed base case, u(4) = 9**. U(n) = words that sort and whose every slip c_i -> c_{i+1} sorts; slip-proof words are in U(n). Put the maximum on wire 1; its path uses n-1 letters; letters that compare with it from above are wasted; the remaining letters form a word on n-1 wires which is again in U(n-1) (an up-slip either maps to an up-slip or to a deletion, and a deletion is stronger by L1); up-slipping the path letter c_{n-2} forces at least one wasted letter. So u(n) >= u(n-1) + n. This recursion is not tight at n=4 (u(3)=4, u(4)=9), hence the computed base (3^L words, L<=8, all 24 inputs; `slow.py 4 9 up`, `fast 4 8 up`).
- L5 A(n) >= N+n-1 for n >= 2: COMPLETE, no computation (same decomposition, base A(2)=2).
- Not proved: OET with n+2 rounds for general n; equality of the numbers of minimum slip-proof and deletion-proof words (observed n=3..7; I conjecture the two sets coincide at minimum size, not checked word by word).

## 5. Interpretation

No real ambiguity found. Reading: both slips of every position must sort (an end letter has one slip); "sorts" means all n! inputs. n=2 is degenerate (no slip exists, S(2)=1, D=0), consistent with the statement's n>=3. Other reading reported: deletions instead of slips give the same values (column A).

## 6. Code

- `slow.py` (definition, brute force; modes slip/del/up): n=3,4 rows, base case u(4)=9.
- `fast.cpp` (`fast n L mode`, env `DOM=1`, `NOKEEP=1`): n=3..7 rows, counts, layer sizes.
- `check.py`: independent all-inputs checker for one word.
- `verify.py`: witnesses n=3..7, OET, beliefs (a),(d), deliberate failure.
- `proofs.txt`: proofs.
