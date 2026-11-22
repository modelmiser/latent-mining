<!-- Archived by the organiser on 2026-11-21. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Dispositions, certificate for 11 bins (experiment 21), round 1 (2026-11-21)
Seats: GPT, Grok, Gemini (panel.py); Claude seat 1, Claude seat 2 (Opus) on the plan.
Q1-Q5 (the proof) confirmed 5/5. Q6: GPT REFUTED on parts=0 crash and per-part ACCEPT (rated CRITICAL by GPT): command-line edge cases, no recorded run uses parts; not a soundness defect of the single-part runs; disclosed on the page, checker file left verbatim. Q7/Q8: byte-identity had no log -> organiser checksum file added; explicit.py claim had no log -> organiser ran it at 6, 7 (bad), 8 bins, log added; damaged-certificate wording fixed; "the 12 resting on the certificate" and dropped hedge -> sentence rewritten with sources for each number; difftest.log MISMATCH lines, missing REPORT.md, NOPRUNE switch disclosed; Review section marked as predating certificate/.
Not applied: 217 vs 218 lines (wc -l gives 218); edits to PROOF.md / check.cpp (verbatim agent files; slips listed on the page).
Checked by me: read PROOF.md; recompiled and reran checker (11 bins accept, 7 reject, damaged 9 reject); explicit.py runs.
Not reviewed again after edits.
