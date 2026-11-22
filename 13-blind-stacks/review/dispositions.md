<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Dispositions, round 1 (experiment 13)
Q1, Q2, Q4, Q5, Q7 CONFIRMED by all five. Q3: Grok REFUTED on "exactly j" (at least j suffices). Q6: GPT REFUTED on missing ranges of the closed forms.
- REFUTED as CRITICAL: GPT, verifier/defsolve.cpp bounds. Code quality; claimed runs within k <= 5, n <= 10. Mentioned in README.
- REFUTED: Gemini, reports/ missing. Present (referee.md, solver-a.md, solver-b.md).
- FIXED: closed-form ranges; A052936 shift; recurrence-proof description (A vs B; what each check covers); "has learned nothing"; "if ever"; literal-rule ranges; hash caveat; N(1,n) status; Where cells.
- NOTE: solver A Theorem 3 closing remark (u >= 6 / empty state at u = 5); solver B "exactly j": notes at top of proofs files.
No CRITICAL survives; no real-bug MODERATE. Stop.
