<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Dispositions, round 1 (experiment 6)
Grok seat produced no answer (agent looping). Four seats, three families. Q1-Q7 CONFIRMED by all four.
- REFUTED as CRITICAL: GPT, verifier/ref.cpp n=7 DP out of memory. The log is a failed attempt; referee.md claims ref dp only for t = 0, 6, 7 at n = 7 and uses bb / certificates elsewhere. Disclosed in README.
- FIXED: "exactly when" proved twice (Opus, Gemini, Fable) -> solver A only.
- FIXED: counterexample explanation (prefix facts wrong; mechanism speculative).
- FIXED: my script now in the folder (check_headline.py); A(5) provenance wording; F(8,1) via P4; F(9,.) row; "drifts".
- NOTE: Hoeffding constant; A(0) convention; B's P7 scope: notes at top of proofs files.
- NOT DONE: rename dualbound field in wit header (explained in README); dp.cpp printf order; solver A's mislabel of bad2 file (in referee.md).
No CRITICAL survives. Stop.
