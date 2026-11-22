<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Dispositions, round 1 (2026-11-17)
Q1-Q3, Q6 CONFIRMED by all. Q4: 4 CONFIRMED, GPT REFUTED on one point.
- Zero-length filler job in CASH-IN (gpt MODERATE; fable, opus, grok LOW): VERIFIED (open-s/proofs.txt CASH-IN; play.py:157 omits it). Statement bug, argument unaffected. NOTE N1 in file + README Review.
- Headline lacks horizon qualifier (opus): VERIFIED, FIXED.
- P=3 node counts 3850 vs 1064 (opus, fable): VERIFIED in results.txt, NOTE N3.
- S_9(4)=2 solver-B-only via Lemma D run; S_7(4) not Lemma-D-dependent (grok, opus, fable, gpt): VERIFIED (solver-a/proofs.txt), FIXED.
- Blank cells S_9(2), F_10(2) computed (fable, opus, grok): VERIFIED in solver-b/results.txt, FIXED.
- "I re-derived it" unrecorded (opus): FIXED to "read it line by line".
- "needs ~4P^2 / ~2P^2" wording (opus): FIXED.
- Tight unit deadline <= Delta+1; monotonicity uncited; Delta used in P=5,6 runs; a_j undefined; Corollary A' domain: NOTES in the proof files.
- referee.md predates proofs (grok): NOTE in README Files and Review.
- bad2.txt identical to valid tree; bad_alloc line in tables.txt; mis-built control: VERIFIED, already disclosed by the referee / the agent; left as is.
Stop: no CRITICAL; the one real-bug MODERATE is repaired by omission of an empty job. No second round.
