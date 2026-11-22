<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Dispositions, round 1 (experiment 8)

Q1-Q5 CONFIRMED by all five seats. Q8 REFUTED by all five: logs for n = 2, 3 were not saved.

- FIXED: "three independent programs" wording (README rows and "What to distrust"); added rerun-n2-n4.txt, a saved re-run of both solvers' programs for n = 2, 3, 4.
- FIXED: 18 of the 170 n = 5 classes are paddings of the 4-state examples (Opus seat). Re-checked by my own script (panel/work/padcheck.py): 18.
- FIXED: "Why the example works": deviation goes to pair 12 first; blind game not decided by y_1 alone.
- FIXED: "both solvers' checkers" -> solver B's ran on a relabelled copy.
- FIXED: "closed under either letter" -> closed up to containment under a round.
- NOTE: solver-b/proofs.txt defects (height+1; x_t = a for t <= d+1; M_(r-1) = 13) listed in a note at the top of that file; body unchanged.
- NOTE: referee's invalid negative control ("CHECKER BROKEN" line) mentioned in README.
- REFUTED: GPT seat, solver-b/verify.py:31-54 "blind checks start from Q.a". `frozenset(A['a'])` iterates the dict's keys, i.e. all states Q. Same for line 21-24.
- NOT DONE: moving mutation by-product files out of solver-a/ (left as the solver wrote them; its report says to ignore them).
- NOT DONE: solver-a/proofs.txt:49 "I regard this as a machine proof" left; README says computed.
No CRITICAL. No real-bug MODERATE survives. Stop.
