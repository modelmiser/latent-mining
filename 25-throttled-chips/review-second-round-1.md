<!-- Archived by the organiser on 2026-11-21. Report of a second-round reviewer (one Claude instance) on this experiment's page, taken from the session transcript. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

Second-round review, experiment 25 (`25-throttled-chips/README.md`): no CRITICAL defects; 2 MODERATE, 5 LOW. Nothing written under `~/Claude`.

**MODERATE**

1. **README.md:12-15 (Short answer) — stale: it no longer states the best known lower bound.**
   - Sentence: "5h − 176 ≤ G(h) ≤ 5h − 6 … it is weaker than the simple bound 2h − 1 below until h reaches 59". The arithmetic is right (114 < 115 at h = 58, 119 > 117 at h = 59).
   - Why: it implies 2h − 1 is the best bound below h = 59. `lag/corollary_out.txt` has Corollary U2 beating 2h − 1 from h = 28 and beating 5h − 176 up to h of order 10⁵ (G(59) ≥ 200 against 119).
   - "By more than 170 rounds" is likewise not the best gap for moderate h: at h = 100 it is 494 − 398 = 96.
   - Replacement for the parenthesis: "(proved by hand, the lower bound by one instance on 21 November; not machine-checked). For h from 28 up to about 10⁵ a second bound from the same day is stronger: G(h) ≥ 5h − 26 − 10·H_15h, e.g. G(100) ≥ 398; below h = 28 the simple bound 2h − 1 is the best."

2. **README.md:148 — "(between 1 and 5, about 2.5 on average, in the greedy runs)" is contradicted by the outputs.**
   - `gen_out_39304.txt` greedy L: S(17,22) → S(18,2), a factor of 11.
   - `gen_out_1156.txt` greedy: 57 → 11, a factor of 5.2.
   - The N = 300000 run does stay within 1.0 to 5.0, and "about 2.5" holds for all three (geometric means 2.5, 2.7, 2.5).
   - Replacement: "(about 2.5 on average in the greedy runs; single steps range from 1 to 5, and up to 11 at the last step of a run)".

**LOW**

3. **README.md:48 (Results row) — "better than 2h − 1 from h = 28" reads as a property of the closed form.**
   - It is true only of the exact condition (57 against 55 at h = 28; a tie at 53 at h = 27).
   - The closed form 5h − 26 − 10·H_15h gives 47.8 at h = 28 and first exceeds 2h − 1 at h = 31 (61.8 against 61), by my recomputation.
   - Replacement: "…; the exact condition is better than 2h − 1 from h = 28 (the closed form from h = 31)". The same care is needed in any new Short answer wording.

4. **Top-level README.md:56 (row 25) — "(lower bound for tie-breaking that does not look ahead)" drops "deterministic".**
   - README.md:47 and 296-298 both exclude random tie-breaking.
   - Replacement: "(lower bound for deterministic tie-breaking that does not look ahead)".
   - The row also omits U2. That is acceptable, but if the Short answer changes, add "(and at least 5h − 26 − 10·H_15h, stronger for 28 ≤ h ≲ 10⁵)".

5. **README.md:193-197 ("What to distrust", lag bullet) — the list of slips in the instance's files is incomplete.**
   - `lag/PROOFS.md:155` still says "a factor 2 to 4", and `:162` still says "0.2 to 0.8 in the earlier search". The Review section (299, 307) calls both wrong.
   - `lag/REPORT.md` is not mentioned on the page and still says "every list replayed by a separate C++ program", "unreviewed", "2 to 4", and "asserted in the runs, which reach 12 stages".
   - Add: "`PROOFS.md` and `REPORT.md` also still carry '2 to 4', '0.2 to 0.8', 'every list replayed' and 'unreviewed', corrected on this page."

6. **README.md:171-176 ("The limit rests on one solver's proof") — stale.**
   - The limit 5 now also follows from Theorem U and Corollary U2. That route needs readiness only at lag above 3, a regime the tests did reach (`check_U_out.txt`: "fact (d) checked in 13 rounds", lag 3.8).
   - It still rests on solver A's rule and facts (a) to (c).
   - Add: "Since 21 November the limit also follows from `lag/PROOFS.md` Corollary U2, which uses the same rule and three of solver A's facts."

7. **README.md:101-102 ("Why no list is much shorter") — correct for Theorem 2, but nothing marks it as superseded.**
   - Add after "13.2": "(Both are superseded below: the lag is at most 2 + H_3t.)"

**Clean (checked against the files)**

- Row 46, and the "What to distrust" bullet on the constant bound.
- Row 47:
  - 2 + H_n with n ≤ 3t, the length 24·34^j + 84 and the lag (8 + j)/5 match `PROOFS.md`.
  - 3 stages give lag 2.2 and 12 greedy stages give lag 4.0, matching `gen_out_39304.txt` and `gen_out_300000.txt`.
  - Replays exist for N = 1156, 39304 and 300000, plus the organiser's for N = 34.
  - "Flush never arose": I confirmed it with an instrumented scratch copy of `gen.py` (0 flushes for N = 34, 1156 under L, R and H, and 39304 under L). No file in the folder records this.
- Row 48 values: 200, 398 and 4873 recomputed; the crossover near 10⁵ is consistent (closed form about 1.2·10⁵).
- Row 49: 6.6 and the 0.8 height excess match `constant/lagadv2_big.txt`.
- Lag bullet numbers: "ln t + 4.1", "t ≥ 108" and "factor about 18" (1/0.0567) are all correct.
- "From √h to a constant", apart from item 2.
- All three Review paragraphs: each listed correction is in fact applied on the page.
- Files section.
