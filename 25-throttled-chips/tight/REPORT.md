# Report: the constant in G(h) = 5h - c (experiment 25), 2026-11-21

## Strongest statement

**For every h >= 4:  max(2h - 1, 5h - 28) <= G(h) <= C(h) <= 5h - 10.**
So c lies in [10, 28]; the page had [6, 176]. No exact value of G(h) for h >= 6 was obtained.

| Part | Status | Where |
|---|---|---|
| C(h) <= 5h - 10 for all h >= 4, hence G(h) <= 5h - 10 (was 5h - 6) | PROVED | PROOFS.md Part A |
| C(h) = 5h - 10 for 4 <= h <= 12, C(3) = 7, by a solver written from the definition | COMPUTED (agrees with the page) | solve.cpp, out_C.txt |
| Under the page's lag rule every height is <= t/5 + 5.6 at every step; every list of length <= 5h - 29 loses; G(h) >= 5h - 28 (was 5h - 176) | PROVED | PROOFS.md Part B |
| Arithmetic of the table in B4 | COMPUTED, exact fractions | arith.py, out_arith.txt |
| Lemmas B1, B2 and the height bound asserted along 89,243 plays | COMPUTED | test_rule.py, out_rule_main.txt |
| Exact G(6), G(7), G(8) | FAILED, not attempted exhaustively | |
| No winning list of length 19 at h = 6 or 24 at h = 7 found by random hill-climbing on 3 to 6 neighbouring points | COMPUTED, weak evidence only | out_search_h6.txt |
| An upper bound below 5h - 10 | FAILED, nothing found | |

## How

Upper bound. For 5h - 10 zeros, Q = sum of x^2 is twice the number of firings. Comparing with
its minimum under heights <= h - 1 gives an exact slack equation
2p + 4d_0 + 3(d_1 + d_-1) + E = 6 (p passes in rounds 2..m-1, d shortfalls at 0, 1, -1, E the
cost of chips beyond distance 2). It has three solutions. One has no pass, but rounds 2 and 3
cannot both fire. One forces equal firing counts at 1 and -1 with an odd sum. One dies on
tracing rounds m-1 and m-2 backwards.

Lower bound. Same rule, three changes to the analysis. (1) If the largest lag is >= 6/5, SOME
vertex of that lag is ready (the page: every such vertex, from 6, later from above 3); 6/5 is
least possible. (2) Theorem U's top-k induction on W = {vertices with target >= 6/5}, base
level 1 instead of 3: every lag <= H_|W|. (3) W has at most 7 vertices within distance 3 of y;
each further one costs chips outside [y-2, y+2], which lower the five-point average at y by
1/5 each but raise the lag bound only logarithmically. Lags are multiples of 1/5. Result:
2 l_y - e/5 <= 4.8.

## Tried and failed, or not done

- Exact G(6..8): length 19 is needed at h = 6 and the page's search stopped at 15 or 16. I had
  no pruning idea beyond Lemma Z and did not rerun it. My solver takes one list.
- The discrete Laplacian: top-k bounds bound sums of lags from above, never the neighbours
  from below, so I only dropped the negative terms.
- Earlier versions gave 5h - 32 and 5h - 30. The test's record of the largest top lag with no
  ready vertex (1) showed the threshold could go down to 6/5.
- The random search scores a list by the number of surviving firer positions. It finds
  winners where they exist, but its silence at length 19 means little.

## Controls, each seen to fail

- test_rule.py with readiness threshold 1, base level 4/5, height cap t/5 + 2.2, and a lazy
  rule (fire from lag 3): each ends in VIOLATION (out_rule_controls.txt).
- arith.py with cap 4.6, and without the two hand-counted cases: FAIL.
- solve: 19 zeros at h = 6 and 0,0,0,0,0 at h = 3 are reported losing, with a surviving line;
  C(3) = 7, not 5 (Part A needs h >= 4). The search finds winners at h = 3 length 5, h = 4
  length 10, h = 6 length 20, and none at h = 4 length 9 or h = 5 length 14.

## What a reader must still trust

- Both proofs are by one instance in about two hours, read by nobody else, not machine-checked.
  Part A is one page. Part B has two case analyses (B1; B3 for j = 2 and j = 4) that need a
  second reader.
- The tests exercise B1 and B2 (the top-k bound is met with equality) but not B3, B4 or the
  height bound: the largest heights seen were t/5 + 2.4 after a round and t/5 + 3.2 after an
  add, against the proved 4.8 and 5.6.
- Process: one compute process at a time on core 9, niced, 6 GB cap. One run was stopped by
  killing my own PIDs (it replayed two 26,000-entry lists too slowly; they are not in the final
  test). Nothing written outside the folder; 112 KB on disk.

## Files (in .)

PROOFS.md, REPORT.md, solve.cpp, test_rule.py, arith.py, run_all.sh, out_C.txt, out_L.txt,
out_arith.txt, out_rule_main.txt, out_rule_controls.txt, out_search_h6.txt.
