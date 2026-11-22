<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — NOTE.md:43–62 matches the pair, known permutation, inserted words, and z_0, z_1 condition, and that condition is the same as “cyclically equal exactly when f(x)=f(x')” for a Boolean f — none

Q2: CONFIRMED — NOTE.md:70–136: (∗), (∗∗), the pair split, m odd (no (X)), θ=0, at most one exceptional pair, r·t, both cases, vanishing frequencies, non-unique shifts, and the corner flip all follow; NOTE.md:72–74 is true — none

Q3: CONFIRMED — NOTE.md:140–145 follows from Lemma 1; AND/OR NOTE.md:150–151, equality NOTE.md:152–154, majority NOTE.md:155–156 are the 7–1 pattern; NOTE.md:147 is that pattern; NOTE.md:168–175 matches prior-art-deep.md:16–17 — “the AND of three literals, or the negation of that AND”

Q4: CONFIRMED — NOTE.md:194–214 matches proofs.txt:22–34; T4, T5, C3, L1 match NOTE.md:217–220; the three strings match solver-a/w_n3_m36.txt:4–6 and check.py:3–9 (0 = extra card, 1 = heart) — none

Q5: REFUTED — Summary and §5 stay inside Theorem 2, the examples, Proposition 3, and the §1 model match; open-status hedges are NOTE.md:3–5 and 303–307 — none

Q6: REFUTED — NOTE.md:329–330 says every instance’s verbatim report is in this folder; reports/ has only solver-a.md, solver-b.md, and referee.md (README.md:225–230 only summarises the panel). The checks those three files do record agree with them. AI, unread by a mathematician, and not machine-checked are stated at NOTE.md:3–5, 32–33, and 289–290 — “Solvers and referee: reports/. The five reviewers’ verbatim reports are not in the folder.”

Q7: CONFIRMED — NOTE.md:25 “either colour”; NOTE.md:147 “negation of one”; NOTE.md:210 “[v=0]” undefined; NOTE.md:218 “player” undefined — “both colours, in any numbers”; “or the negation of that AND”; “c when v=0 and 0 otherwise”; “each input bit”

F: MODERATE — NOTE.md:329–330 — the five reviewers’ verbatim reports are not in the folder, so “the full record … is in this folder” is false — high — state that only the solver and referee reports are present
F: LOW — NOTE.md:25 — “either colour” can be read as monochrome helpers, the case [3] already closed — medium — “both colours, in any numbers”
F: LOW — NOTE.md:147 — “the negation of one” does not say one what — high — “or the negation of that AND”
F: LOW — NOTE.md:89–123 — θ_i is mod m, but r·t equalities are written in Z; they are only congruences (harmless for ω^{r·t}; README.md:113 says “modulo m”, the note does not) — high — write ≡ (mod m)
F: LOW — NOTE.md:210 — the bracket [v=0] is never defined — high — “c when v=0, and 0 otherwise”
F: LOW — NOTE.md:218 — “player” is used before any definition — high — “each input bit”
F: LOW — NOTE.md:288 — “adds … m even” is already in proofs.txt:51 — high — say only the corner reduction is new relative to T2
F: LOW — NOTE.md:298 — 1,011,693 occurs nowhere else in the folder — high — cite the script or drop the count
F: LOW — NOTE.md:313–314 — the 6-card agreement is cited as a check, but README.md:196–197 says both sides are zero — high — say both searches find none, so this does not test a positive find
F: LOW — NOTE.md:161–164 — the Table 2 balance count is not in the repo — high — include the 16-row table
F: LOW — NOTE.md:66 versus NOTE.md:231 — ♣=+1, ♥=−1 in the proof, then 1=♥, 0=♣ in the witness — high — restate the witness convention at the strings

The lemma, theorem, examples, and Proposition 3 hold as written. The summary’s “between 16 and 36” is still only as strong as two pruned searches: referee.md:17 did not rerun m=14 or 15.
