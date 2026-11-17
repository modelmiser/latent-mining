# solver-a report

All files are in `solver-a/`.

## 1. Tables

| n | S(n) | D(n) | A(n) (deletion-proof) | slip-proof words of size S(n) | status |
|---|------|------|------|------|--------|
| 3 | 5  | 2 | 5  | 2 | COMPUTED twice (brute + BFS), and PROVED |
| 4 | 9  | 3 | 9  | 24 | COMPUTED twice, and PROVED |
| 5 | 14 | 4 | 14 | 1912 | COMPUTED twice, and PROVED |
| 6 | 20 | 5 | 20 | 1210080 | COMPUTED (BFS only), and PROVED |
| 7 | 27 | 6 | 27 | 7144675968 | COMPUTED (BFS only), and PROVED |
| all n >= 3 | (n-1)(n+2)/2 | n-1 | same | not known | PROVED (`proofs.txt`) |

The word counts are computed only; the proof does not give them. They are identical in slip mode and deletion mode.

Methods:
- **`brute.cpp`** (slow, direct): depth-first over all words up to a size limit, no pruning and no deduplication. It carries the image of the reversed input under the word and under every slip. Each word that passes is re-tested on all n! inputs straight from the definition.
  - Reached n=3 to size 7, n=4 to size 10, and n=5 to size 14 in slip mode (30 s); deletion mode was run for n=3 and n=4 only.
  - Slip-proof word counts by size: n=3 gives 0, 2, 16, 58 at sizes 4 to 7; n=4 gives 0, 24, 444 at sizes 8 to 10; n=5 gives 0, 0, 1912 at sizes 12 to 14.
  - The reversed-input count equalled the all-inputs count in every case.
- **`bfs.cpp`** (fast): breadth-first over states (P, U), exact for n <= 7.
  - P is the set of threshold 0-1 vectors of the unslipped image of the reversed input; U is the set of threshold vectors reachable with exactly one slip.
  - Pruning 1: U is reduced to elements not dominated by another element of U or by the same-weight vector of P. This is safe by monotonicity (L1 in `proofs.txt`), which the program also self-tests.
  - Pruning 2: mirror symmetry (c_i to c_{n-i}).
  - Pruning 3: states with level + lower bound > T are dropped, where the lower bound is the larger of the inversions of P and the largest 0-1 inversion count in U. This is safe whenever the answer is <= T; an unsafe prune could only bias the answer upward.
  - Cross-checks (`out_cross.txt`): with the dominance reduction off (n <= 6), with T raised by 2, and with T = S-1 ("no word"), the answers and counts are unchanged, and they agree with `brute.cpp` for n = 3, 4, 5.
  - n=7 peaks at 1.33 million states and takes under 2 minutes (`out_n7_m0.txt`, `out_n7_m1.txt`).

## 2. Witnesses

- **`witnesses.txt`**: W_n = D_1 D_2 ... D_{n-1} D_{n-1} with D_k = c_k c_{k-1} ... c_1, which is insertion sort with the last pass repeated; size (n-1)(n+2)/2, for n = 3..12. Example for n=5: `1 21 321 4321 4321`.
- **`check.py`** (plain Python, from the definition; all n! inputs for n <= 7, all 0-1 inputs above) confirms every one is sorting, slip-proof and deletion-proof (`out_check.txt`).
- **`oddeven.txt`**: odd-even transposition sort with n+2 rounds, both starting parities, n = 3..12. All are slip-proof and deletion-proof. This is computed, not proved.
- **`bad.txt`**: deliberately wrong objects, n = 3..7 — W_n minus its last letter, plain insertion sort, and odd-even with n+1 and with n rounds. All are rejected (`slip_proof=False`).
- **Why nothing smaller exists**: theorem T2 in `proofs.txt`, plus the exhaustive "no word of size <= S-1" runs for n <= 6 in `out_cross.txt`. For n=7, the size-27 run in `out_n7_m0.txt` reaches the goal first at level 27.

## 3. Headline

**Yes: D(n) = n-1 and S(n) = (n-1)(n+2)/2 for every n >= 3.** This is proved, and independently computed for n = 3..7. Confidence is about 98%; the remaining doubt is an unnoticed error in a short proof, which the n <= 7 computations do not contradict.

Two further results:
- For n >= 3 a word is slip-proof if and only if it is deletion-proof, so S(n) = A(n) and the sets of words coincide.
- The number of optimal words runs 2, 24, 1912, 1210080, 7144675968 for n = 3..7. I have no formula for it.

## 4. Proofs (full text in `proofs.txt`; all marked COMPLETE there)

- **L0**: the 0-1 principle.
- **L1**: comparators are monotone for the prefix-sum dominance order on 0-1 vectors. This gives beliefs (a) and (d).
- **T1** (slip-proof equals deletion-proof, n >= 3):
  - Deletion-proof implies slip-proof, because a slip is a deletion plus an inserted comparator, which is harmless by (d).
  - Conversely, on a 0-1 input a letter c_i that is active sees (1,0). The slipped comparator then sees (0,·) on wires i+1, i+2 or (·,1) on wires i-1, i, so it is idle. The slipped word therefore acts as the deleted word wherever the deletion matters.
  - So every deletion sorts all 0-1 inputs, hence all inputs.
- **T2** (lower bound):
  - On the reversed input each of the n(n-1)/2 pairs is exchanged at exactly one letter.
  - Delete the letter that exchanges v+1 and v. The run then equals the original with v and v+1 relabelled, until some later letter compares exactly those two values.
  - That letter must exist, and it is idle in the original run. The n-1 such letters are distinct, so the word has at least n-1 idle letters.
- **T3** (upper bound): W_n is deletion-proof. On 0-1 inputs the class "sorted with at most one 0 displaced downward" is preserved by intact passes, produced by a damaged pass, and repaired by the final extra pass.
- **Sketch only / unproved**: odd-even transposition with n+2 rounds is slip-proof for all n (verified for n <= 12).

## 5. Interpretation

- I read the statement literally: a slip changes one letter by ±1 within the alphabet, and w and all its slips must sort all inputs. The adversary phrasing is equivalent.
- n=2 is degenerate (no slip exists, so S(2)=1 vacuously), which is why n >= 3 is needed; T1 uses n >= 3 exactly here.
- The alternative "deletion" reading gives the same values and the same words (A(2)=2 also fits the formula).
- Belief (c) is true but not tight: the true bound is the sum n(n-1)/2 + (n-1), not the max.
- Beliefs (a), (b), (d) are true; (e) is true for n <= 12 by computation.
- The stated doubt, that a slipped comparator might do useful work, does not materialise, because of T1.

## 6. Code and outputs

- `bfs.cpp` (`./bfs n mode T [nored]`, mode 0 = slip, 1 = deletion): S and A for n = 3..7 and the optimal-word counts. Outputs: `out_n6_m0.txt`, `out_n6_m1.txt`, `out_n7_m0.txt`, `out_n7_m1.txt`, `out_cross.txt`.
- `brute.cpp` (`./brute n mode L`): n = 3, 4, 5. Output: `out_brute.txt`.
- `check.py` (`python3 check.py FILE`): `witnesses.txt`, `oddeven.txt`, `bad.txt`. Output: `out_check.txt`.
- `proofs.txt`: the proofs.

All runs were niced, single-threaded, memory-capped and under timeout.
