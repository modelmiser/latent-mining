<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Dispositions, round 1 (2026-11-16)

No CRITICAL. Q1-Q6 CONFIRMED by all five seats. Splits on Q7-Q9 are wording/provenance.

- FIXED  README.md:32 headline status (opus, grok): now "hand-proved, not machine-checked; exact odd clock value open past 13".
- FIXED  01 README C-odd cell (4 seats): hand d=1, exhaustive d=3,5, certificates d=7..13, one checker at 11, 13.
- FIXED  01 README H-odd cell (gpt): invariant machine-checked, existence on paper.
- FIXED  solver-a/proofs.txt:46 false inequality (all): errata E2 with repair; README points at solver-b P2.
- FIXED  solver-a/proofs.txt:30 "+2, +2" unproved (grok, opus, fable, gpt): errata E1; README says computed d<=12.
- FIXED  solver-a/proofs.txt:62,70,86,92 (T0 >= 2d+2, V in {P,Q}, even ticks 0, H<=C): errata E3.
- FIXED  stale NOT PROVED list (opus): errata E4.
- FIXED  certificate policy (opus, fable, gemini): cert_7_7.txt shipped (re-checked OK from the repo copy); regeneration command given and tested at d=5.
- FIXED  prior-art report absent (gpt): prior-art.md added.
- FIXED  no Lean toolchain (fable, gpt): lean-toolchain + axioms line in README.
- FIXED  checkers are same-family artefacts (fable): distrust bullet.
- FIXED  Q7 (grok REFUTED): the left inequality needs H<=C; now stated in the README headline paragraph.
- NOTE   solver-a/run_c5.txt and run_rh.txt MemoryError logs (opus, gemini): left as the solver's record; the d=5 evidence is run_clock_cpp_d5.txt.
