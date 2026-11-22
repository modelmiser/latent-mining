<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `17-cut-and-peek/README.md:73-84`, `17-cut-and-peek/solver-b/proofs.txt:38-44`, `17-cut-and-peek/chain/3-question.txt:4-7` — none
Q2: CONFIRMED — `17-cut-and-peek/solver-b/proofs.txt:42-63`, `17-cut-and-peek/README.md:86-120` — none
Q3: CONFIRMED — `17-cut-and-peek/solver-b/proofs.txt:42-64`, `17-cut-and-peek/solver-b/fullR.cpp:1-14`, `17-cut-and-peek/solver-b/fullR.out:1-14` — none
Q4: CONFIRMED — `17-cut-and-peek/README.md:121-142`, `17-cut-and-peek/prior-art-deep.md:33-35` — none
Q5: CONFIRMED — `17-cut-and-peek/solver-a/w_n3_m36.txt:1-7`, `17-cut-and-peek/verifier/vcheck.py:10-44`, `17-cut-and-peek/verifier/tests.out:1-24`, `17-cut-and-peek/README.md:152-159` — none
Q6: CONFIRMED — `17-cut-and-peek/solver-b/proofs.txt:22-35,88-111`, `17-cut-and-peek/reports/referee.md:15-16,63`, `17-cut-and-peek/README.md:60-63,181-185` — none
Q7: CONFIRMED — `17-cut-and-peek/README.md:53-69,169-201`, `17-cut-and-peek/reports/referee.md:10-24`, `17-cut-and-peek/prior-art-deep.md:7-50` — none
Q8: CONFIRMED — `README.md:47`, `17-cut-and-peek/README.md:23-40`, `17-cut-and-peek/chain/3-question.txt:1-7` — none

F: none

### Important items not asked about (most severe first)

1. **Possible vacuity of the seven-rotation hypothesis (T2/C1).**
   T2 proves the conditional implication: *if* seven rows of a 3-cube are cyclic rotations of one word \(W\), *then* the eighth row is also a rotation of \(W\). It does not prove whether such a word \(W\) and disjoint flipping vectors \(a_1, a_2, a_3\) can ever produce seven rotations for any \(m\). In fact, `solver-b/fullR.cpp` tested all words and shift triples for \(m \le 18\) and found exactly zero triples satisfying the hypothesis (`solver-b/fullR.out:1-14`). Shinagawa and Nuida (arXiv:2507.04916, Conclusion) specifically asked whether seven words can be cyclically equalized by helper cards; T2 proves full-open AND is impossible without settling whether seven-word cyclic equalizability itself is achievable.

2. **The 20-card gap for partial opening (\(16 \le r(3) \le 36\)).**
   Pruned exhaustive search stops at \(m = 15\), and the interleaved strand construction finds a witness at \(m = 36\). For general reveal sets \(R\), the entire range \(16 \le m \le 35\) is completely unsearched (`reports/solver-a.md:49`, `17-cut-and-peek/README.md:186`).

3. **Shared search normalisation between solvers at \(m = 14, 15\).**
   The lower bound \(r(3) \ge 16\) relies on solver A's and solver B's partitioned runs at \(m = 14, 15\), which the referee explicitly did not rerun (`reports/referee.md:16,68`). Both solvers' algorithms share the exact same structural prunings (complement form, balance, placing a player-0 club at index 0, difference filtering on \(R - R\)). While brute-force cross-checks match at \(m \le 6\), an error in the shared normalisation would not be caught by matching leaf counts (`reports/referee.md:63`).

4. **Lower bound for four players (\(r(4) \ge 15\)).**
   Solver A reported \(r(4) \ge 14\) based purely on its exhaustive run up to \(m = 13\) (`reports/solver-a.md:20,50`). Solver B's Theorem T1 proves \(m \ge 2^n - 1\) for any valid protocol, which immediately establishes \(r(4) \ge 15\) even under partial opening (`solver-b/proofs.txt:26`, `reports/referee.md:35`).

5. **Unchecked minimality in the strand family.**
   Solver A's claim that 36 is the minimal card count within interleaved strand protocols is unverified by the referee, depends on floating-point LP solvers (`solver-a/zscanR.py`), and explicitly left open several branch cases, including \((a, b) = (7, 5)\) (\(m = 35\)) and \(a \ge 11\) with partial \(R\) (`reports/solver-a.md:26`, `reports/referee.md:24,72`).
