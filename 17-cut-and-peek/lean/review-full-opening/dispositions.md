<!-- Archived by the organiser on 2026-11-21. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Dispositions, Lean formalisation of experiment 17, round 1 (2026-11-21)
Seats: GPT, Grok, Gemini (panel.py); Claude seat 1, Claude seat 2 (Opus) on the plan.
0 CRITICAL. Q1-Q3, Q5-Q8 confirmed 5/5 (Spec.lean and the four statements say what the note means; five-card layout checked by hand by several seats). Q4: GPT REFUTED on the patch comment (injectivity) -> comment clarified. Q9: authorship sentence wrong (MODERATE, GPT, both Claude seats) -> rewritten.
Applied: authorship; trust inventory; "about 410 lines"; sign-convention note; "no encoding at all" reworded; OR/majority parenthetical; README row notes the equivalent formulation; Spec comments (m = 0, patch); FiveCard comment; SORRY search extended and now empty; six-card check added (Claude seat 1's suggestion; layout found by search, proved by decide).
Verified by me: statements in Main.lean unchanged; Spec.lean differs from the agent's input only in comments (diff saved in ../old); clean rebuild 3275 jobs, 0 warnings; axioms = the three standard ones for all six theorems; virtual-memory cap raised to 40 GB after diagnosing the thread-creation flake.
Not reviewed again after edits.
