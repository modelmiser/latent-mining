<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Dispositions, round 1 (experiment 4)
Q1-Q4, Q6, Q7 CONFIRMED by all five.
- FIXED: run1.txt lacks T1 for k = 2, 3 (Fable, Opus, GPT, Grok, Gemini). Re-ran `ref t1 2`, `ref t1 3`: 960 states, 0 lost each. Saved as open-upper-referee/rerun-t1-k2-k3.txt.
- REAL BUG, DISCLOSED: ref.cpp key() casts counts to char (GPT). Verified in source. Affects the root sweep above n = 255 only; T1 unaffected (counts <= 25). README "What to distrust".
- FIXED: T2 row added; Lemma S hypotheses in the explanation; "at least as many"; k = 1 sentence; "such states"; empty Where cell; literal-definition coverage.
- NOTE: proofs.txt blemishes in a note at top (ell >= 1; 13 >= questions left after the lie; state counts; which second program).
- NOT DONE: unfiltered table_out.txt (said so in README); literal Q_2(5) in run2.txt (T2 has four other sources).
No CRITICAL. The real bug is outside the proof's table. Stop.
