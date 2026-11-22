<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Dispositions, experiment 15, round 1 (2026-11-19)
Seats reporting: Opus, GPT, Grok, Gemini (Fable seat empty: per-seat budget exhausted; month at $198.21).
Q1 confirmed 4/4. 
CRITICAL (Grok, Theorem 2 Step 4 "staged scheduler illegal"): REFUTED by me with reasoning (scheduler must pick SOME legal move; it picks active-active moves, enlarges the active set when none; only a sequence of legal moves is needed since the c*n bound holds for every such sequence; stuck active set = terminal configuration of the standalone planted instance). Opus, GPT, Gemini accept the step. Disclosed prominently in README (What to distrust + Review); top-level row says one reviewer rejected a step.
MODERATE real: Corollary 1b identity misprint (M-1 -> M-2) (Opus, Grok) -> disclosed; closure machine-checked by referee M <= 12, bound stands.
Applied: counting-step wording; G(3) and G(4M-1) attributions; definition (a+b, G=1 convention, swaps as cycles, u>v); potential wording; minority wording; 2026 paper byline; process-note source; proof-file slips listed.
Not applied: edits to solver files (verbatim).
No second round (budget exhausted).

# Follow-up 2026-11-21: Step 4 adjudication
Two fresh readers (session model; Opus) given both positions unattributed: both found Theorem 2 sound (reports returned as text; harness refused their report files). Second reader's rewrite -> 15-planted-majority/step4-rewritten.md.
Panel on the rewrite (GPT, Grok, Gemini; ~/staging/2026-11-21-latent-mining-15-step4/panel/round1): Q1-Q6 confirmed 3/3, Grok included (Q6: the rewrite answers the objection). Applied their LOW corrections (S_j recurrence, m(k+1), wording on outside pairs, "cannot be tested", small gaps in Steps 2-3 listed, adjudication reports labelled as not archived). README bullet, Review section and top-level row updated. The earlier CRITICAL is closed.
