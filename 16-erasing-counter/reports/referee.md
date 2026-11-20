# Referee report

W_1 = 2, W_r = W_{r-1}(W_{r-1}+1) = 2, 6, 42, 1806, 3263442.
My code, in `verifier/`: `brute.cpp` (all trees, full functional graphs, r ≤ 3), `lazy.cpp` (trajectory search, r ≤ 4), `chk.py` (witness checker), `lemma.py` (lemma tests).

## Verdicts

| Claim | Verdict | How |
|---|---|---|
| D(1) = 2, D(2) = 6 | CONFIRMED | recomputed, all 5 and all 1201 trees, no symmetry |
| D(3) = 42 | CONFIRMED | recomputed twice: brute force (2.85e8 trees after fixing two indices) and lazy |
| D(4) = 1806 (B; A claims only ≥) | CONFIRMED | recomputed: `lazy 4` complete, 1.89e9 decision nodes, 248 s, one run |
| D(5) ≥ 3263442 | CONFIRMED | both witnesses checked |
| D(r) ≥ W_r, all r (Theorem 1 of A and of B) | CONFIRMED | both proofs read, no gap found; witnesses r = 1..5 checked |
| 2^(r-1) ≤ log2 D(r) ≤ 2^r − 1: not polynomial | CONFIRMED | proof read, no gap |
| D(r) ≤ 2^d − 1, r ≥ 2 (A Remark 3) | CONFIRMED | proof read, no gap (terse); its bound 2^(2^(r-1)) on cycles through all-ones held on all r ≤ 2 trees and 20000 random r=3 trees |
| A Lemma 2 | CONFIRMED, one wrong detail | see Errors; same tree sets |
| Problem's weight claims are true | CONFIRMED | proof read; asserted per step in `lemma.py` |
| Problem's "all-ones has no preimage" is false | CONFIRMED | both counterexamples are valid |
| B's weight tables, r=3 and r=4 | CONFIRMED | recomputed, all 8 + 16 entries equal; B's 16 r=4 trees accepted with the stated minimum weight |
| A's B(r,m), exhaustive part (r ≤ 3; r=4, m ≤ 5) | CONFIRMED | recomputed |
| A's r=4, m=6..11: 40, 72, 108, 180, 270, 420 (annealing) | CONFIRMED, now exhaustive | recomputed; also m=12..14: 630, 930, 1302, as A's composition rule predicts |
| A: r=3 optimum unique up to relabelling | CONFIRMED | 720 = 6! trees reach 42 with the root fixed |
| A: r=3 mixed cycles: ≤ 6 root=1 states, root=0 runs ≤ 6, ≤ 36 root=0 states | CONFIRMED | recomputed: 6, 6, 36 |
| B: at most one empty-leaf step per cycle of length ≥ 2, r ≤ 3 | CONFIRMED | recomputed over all trees |
| D(r) = W_r for all r | UNSUPPORTED | computed for r ≤ 4; neither folder has an upper-bound proof |

My r=3 histogram of trees by longest cycle (root fixed) equals B's `ex3_full` in every bin.

## Disagreements between the solvers

1. D(4): A "≥ 1806, not exhaustive"; B "= 1806, one method". My search completes at 1806. B is right.
2. D(r) ≤ 2^d − 1: A says COMPLETE, B says SKETCH. A's argument is complete; B's is a sketch and is labelled so.
3. Counterexample to "no preimage": A uses any tree whose root 0-child is a leaf writing the root; B uses `(1 [1] [])`. Both valid; B's is a special case.
4. Optimal trees at r=3: A counts 24, B 6 and 720. The difference is only the symmetry reduction.

## Errors found

- A, Lemma 2(c): "on a cycle with no creation every step reads exactly one 1" fails for a fixed point reached by the all-zero path with an empty leaf (no 1 is read). The conclusion still holds there (length 1).
- B, report section 2: `bad_idx.txt` is called "repeated index". Its index 14 sits in two different branches, which is legal; B's checker rejects the file because leaf `[15]` is off its path. B therefore never showed its checker rejecting an index repeated on one path. Mine does (`myb_repeat.txt`, A's `bad_repeat.txt`).
- No wrong numerical value in either report.

## Witness checker

`chk.py` accepts every witness: A's 5 Sylvester files, 5 search witnesses and 32 annealing files (with the stated index counts); B's r = 1..5 and 16 per-weight trees. It rejects all 8 of the solvers' bad files and 7 of mine: wrong N (5), N a multiple of the period (12), leaf writing off its path, index repeated on a path, index out of range, path longer than r, start not on the cycle.

## Problem as written

Both answered the stated problem: d = 2^r − 1, paths of length 0..r, single-leaf tree allowed, a leaf may write any one index it read, cycles of length 1 count. I saw no other reasonable reading, and both solvers took this one. The headline is answered in part: D(2), D(3) and the polynomial question (no) are settled; the exact function is known only for r ≤ 4.

## Not checked

- `sa.cpp`, `prod3.cpp`, `naive3b.cpp`, `ex3w.cpp` were neither run nor read line by line; I recomputed their outputs. A's `naive3` histogram was not reproduced bin by bin.
- My r=4 result is one run of one program, validated against my brute force only for r ≤ 3. It is a trajectory search of the same family as both solvers' `lazy.cpp` (A's turns out to use the same XOR-with-start encoding), so a shared design error at r=4 is not excluded. I did not mutation-test it.
- B's empty-leaf observation at r=4.
- Lemma tests at r=3 are a random sample, not exhaustive.

## Follow-up tasks

1. Prove or refute for all r: on every cycle of length ≥ 2, at most one step uses a leaf with R = ∅. Test it exhaustively at r=4 first (a counter in the lazy search, about 5 minutes).
2. Prove, for trees whose two root subtrees use disjoint index sets, that the longest cycle is ≤ D(r−1)·(D(r−1)+1); then state exactly which step fails when the subtrees share indices.
