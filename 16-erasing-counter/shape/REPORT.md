<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the second follow-up instance (counterexample to the shape bound, Theorem Z), verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

# Report: experiment 16, shape bound, attempt A

(The harness refused the write of REPORT.md, so this text is the only copy of the report. PROOFS.md and all programs and outputs are in the folder.)

## Result

**Conjecture P1 is false.** A depth-5 tree on 7 indices has a cycle of length 52 with L(T) = 50:

    (1 (2 (3 [2] (4 [4] (5 [5] []))) [1]) (3 (6 [6] (7 [7] [3])) []))     start 0000000

The headline conjecture D(r) = W_r is not touched: 52 is far below W_5. I also proved the W_depth bound for a class that contains this tree and all read-once trees.

| Statement | Status | Where |
|---|---|---|
| P1 fails at depth 5: cycles 52 > 50 (7 indices), 114 > 98 (9), 762 > 602 (13) | COMPUTED by two programs sharing no code; the 52 case also traced by hand | `check.py` with `cex_small.txt`, `cex_mid.txt`, `cex_big.txt`; `allcycles.py`; `out_cex.txt`, `out_checks.txt`; PROOFS.md §1 |
| Lemma A: in any tree, a cycle of length ≥ 2 with no home state (all bits on the all-zero path 0) has length ≤ k, the length of that path; so it is below L(T) and at most the depth | PROVED | PROOFS.md §2 |
| Theorem Z1: if the subtrees S_i hanging off the all-zero path have pairwise disjoint data (indices off that path), a cycle through a home state has length ≤ 2C_1 + C_2 + … + C_l, with C_t = c_t…c_l, for a chain of distinct modes with data-cycle lengths c_u | PROVED, using Theorem E of `upper/PROOFS.md` | PROOFS.md §3 |
| Theorem Z: every tree of class 𝒞 (that hypothesis, applied recursively to the pruned S_i) of depth ≤ r has all cycles ≤ W_r | PROVED | PROOFS.md §3 |
| 𝒞 strictly contains the read-once trees: it includes examples (a) and (b) of `upper/PROOFS.md` §5 and the counterexample itself | PROVED | PROOFS.md §3 |
| Lemma A, Z1 and the W_depth bound on 6,003 trees satisfying the hypothesis (462,422 cycles, 53,760 through home, Z1 bound attained on 51,427) | COMPUTED, no violation | `test_Z.py`, `out_testZ.txt` |
| D(r) = W_r for all trees | FAILED: not attempted beyond the class | |

The random test is a check of the bookkeeping, not strong evidence: the random cycles are short, and the longest (52) is the fixed counterexample.

## How the counterexample works

The all-zero path reads 1, 2, 3. Its leaf writes 2, and the 1-child of node 2 is the single leaf [1]. So every return to the all-zero path costs three states (home, "bit 2", "bit 1") before the low counter under the root's 1-child moves. That counter reads index 3 once so that it may write it on overflow, which hands the carry to the counter under node 3.

- **Length:** c_3(3c_1 + 1), against L = (c_1 + 1)(2c_3 + 2), where c_1 and c_3 are the two counters' cycle lengths.
- **Why L undercounts:** the relay leaf [1] has L = 1 but is used once per home state. Its real cost is one level of depth for everything further down the path, which L does not measure and W_depth does.
- **Depth 4:** there is no room (c_3 ≤ 2), consistent with the repository's exhaustive depth-4 computation.

## What I tried that failed

- **Repairing map (iv), or any injection into a set of size L(T):** impossible, since P1 is false. Before seeing that, I tried replacing each bit by "direction at the next visit of the node"; it agrees with (iv) on example (b) and fails the same way.
- **A "used part, one-child nodes contracted" refinement of L:** it is ≤ L, so the same tree refutes it.
- **A correct shape function:** not found. Any such function must charge a leaf on a 1-edge of the all-zero path a full factor, as W_depth does.
- **Depth-5/6 search:** none was run. The counterexample came from the proof attempt: the inequality between the Z1 bound and L fails for the chain of modes (2, 1, 3).

## What a reader must still trust

- Theorem E, F1, F2 and the Case A/B split of `upper/PROOFS.md`; Theorem Z1 uses Theorem E.
- My proofs are about an hour old, single author, not machine-checked and not reviewed. The counterexample does not depend on them: it is a 128-state computation anyone can redo.
- The "more generally" clause of Theorem Z (subtrees of depth ≤ 4 with arbitrary sharing) rests on the repository's depth-4 computation.
- For all trees, D(r) = W_r stays open. Sharing of data between different S_i is not covered.

## Checks watched failing

- **`check.py`:** rejected wrong lengths 51 and 104, a wrong L, an index twice on a path, and a leaf writing off its path (`out_checks.txt`). It accepts example (b) (length 10, L = 15).
- **`test_Z.py`:**
  - mut 1 (data shared between the S_i): 161 failures of "one jump state per mode".
  - mut 2 (Lemma A with k − 1): 70 failures.
  - mut 3 (C_1 in place of 2C_1): 25,177 failures.
- **No power:** under mut 1 the Z1 bound itself did not fail in the random sample, so that test says nothing about shared data.

## Files (all in (staging)/)

- PROOFS.md
- check.py
- allcycles.py
- test_Z.py
- cex_small.txt
- cex_mid.txt
- cex_big.txt
- known_b.txt
- bad_len51.txt, bad_len104.txt, bad_L.txt, bad_repeat.txt, bad_leaf.txt
- out_cex.txt
- out_checks.txt
- out_testZ.txt

Nothing was written elsewhere, there was no network use, and compute ran one process at a time on core 6 under the stated caps.
