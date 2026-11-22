<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Dispositions, Lean specification audit (experiment 14)
Q1-Q7 CONFIRMED by all five: Spec.lean encodes the game; no loophole. Q8 (documentation): four REFUTED.
- FIXED in README: false-variants record (inputs only, lists differ, one not false); "checks that can fail" (T 0..2 trivial; conn_iff a lemma); where the theorem first has content (n = 33); core-library functions in the trusted base; pow form weaker.
- NOTE appended to lean/SPEC.md: unlisted encoding choices (edge order in state, flips on unequal lengths, totality), item 7 wording, conn_iff location, T 5 not logged.
No CRITICAL, no MODERATE affecting the theorem. Stop.
