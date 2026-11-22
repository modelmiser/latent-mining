<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Dispositions, experiment 19, round 1 (2026-11-19)
Q1-Q6 confirmed 5/5 (L1, T3 all steps, attempts to break, checkers). Q7 refuted 5/5: "decoding not written by either solver" false -> replaced with solver A's explanation. Q8 refuted 4/5: status-cell provenance -> fixed.
CRITICAL (GPT): solver-b/mc.h MAXB=8 vs b=14 witness. Verified in source. Does not affect g(3)=4 (b=4). Disclosed; g(5)<=14 rests on verifier/vcheck.py (rerun by me: OK, 7070 configs) and solver B's slowcheck.py.
Applied: trial counts split; n<=5 scope; 4-state search provenance and half log; wording of the proof retelling; "nothing settled" -> "no exact value"; prior-art wording; least moduli attribution.
Not applied: solver/verifier code and proof-file edits (verbatim).
No second round.
