<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261118-131106 — (repository) — round 7

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $4.015; this month $167.56 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.970, input 28 + cache write 59,736 + cache read 472,884 tokens — cache hit 89%
- claude:claude-opus-5-5: $1.044, input 26 + cache write 57,751 + cache read 391,578 tokens — cache hit 87%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q2** (split) | CONFIRMED | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED |
| **Q4** (split) | CONFIRMED | CONFIRMED | UNSUPPORTED | REFUTED | CONFIRMED |
| **Q5** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q2, Q4, Q5
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 8 MODERATE, 21 LOW
- **MODERATE** [claude:claude-fable-5-1] Attachment A, Theorem 5 statement and title — "T(n) = n/3 − Θ(log n)" is not proved; only O(log n) (lower side of the gap is 1/3). The "finer statement" cannot help since the Adversary controls H of the final tree — high — replace with O(log n) as above.
- **MODERATE** [claude:claude-fable-5-1] Attachment A "Checks run"/"Status" — phiU_c3.py, gpoolC3.cpp, pathcheck.py, cex1_c3.py, ruleL_emit.py, runC3.sh and their counts (634,854 rounds; 10,222,212 profiles; 43-vertex example; exhaustive ≤11) are neither attached nor in the repository — high — attach or drop.
- **MODERATE** [claude:claude-opus-5-5] Attachment A, Theorem 5 statement — "T(n) = n/3 − Θ(log n)" is not proved. The upper bound is (n−1)/3 − H(T), and H(T) can be small at the end, so only O(log n) follows — high — Write "n/3 − O(log n)".
- **MODERATE** [claude:claude-opus-5-5] Attachment A, "Checks run" and "How it was found" — phiU_c3.py, gpoolC3.cpp, pathcheck.py, cex1_c3.py, ruleL_emit.py and runC3.sh are not in the repository. So the claims about the 43-vertex example, play up to 11 vertices and the pool search cannot be checked — high — Add the programs or drop the claims.
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] THEOREM5-draft.md:146-156 — “\(n/3-\Theta(\log n)\)” is not implied; an \(\Omega(\log n)\) deficit is missing — confidence: high — use \(n/3-O(\log n)\)
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] third_check.py:28,67-100 — non-positive `R` can print `ALL OK` after checking no path cases — confidence: high — reject `R<4`
- **MODERATE** [cursor:grok-4.7-medium] draft Theorem 5 — “T(n)=n/3−Θ(log n)” is not implied; only L=1/3 and a deficit of at least 1/3 and at most O(log n) — high — replace Θ by O and state the deficit gap
- **MODERATE** [cursor:gemini-3.8-flash-high] Attachment A:158-164 — Cited verification artefacts (`phiU_c3.py`, `pathcheck.py`, `gpoolC3.cpp`, `runC3.sh`, `cex1_c3.py`, `ruleL_emit.py`) are not present in the repository — confidence: high — Commit the supporting programs to the repository alongside `third_check.py`.
- **LOW** [claude:claude-fable-5-1] third_check.py line `R = int(sys.argv[1])` — R≤0 prints ALL OK with 0 cases — high — assert R≥14 and expected counts.
- **LOW** [claude:claude-fable-5-1] Lemma 7 → Theorem 5 — composition of unit slot rises into a round's rise is implicit — high — one sentence.
- **LOW** [claude:claude-fable-5-1] Lemma 8 (a) — "rule L is used here and only here" understates the rule (β=0, ε_y=1 also constrains α); harmless — high — say "only this consequence is used".
- **LOW** [claude:claude-fable-5-1] Theorem 5 — Φ can be negative (Φ(A₁)=−1); "Φ starts at 0" plus per-round gain ≥3k is the whole argument, worth saying that Lemma 6 is only an upper bound — medium — wording.
- **LOW** [claude:claude-fable-5-1] Lemma 6 — T−r being one tree / two trees unstated — high — add.
- **LOW** [claude:claude-opus-5-5] third_check.py, the R argument — R ≤ 3 makes the run vacuous; I ran R = 0 and got ALL OK with exit 0 — high — Refuse R < 4.
- **LOW** [claude:claude-opus-5-5] Attachment A, Lemma 7 and "Checks run" — 27,225 is the count for R = 14, but the text says the run was `third_check.py 30`, which gives 477,617 for part (M) and 21,544 for part (P). I reproduced all three counts — high — Name both runs.
- **LOW** [claude:claude-opus-5-5] Attachment A, Theorem 5 proof — the chaining of single-slot increases (several steps, both out-neighbours) is not written down — high — Add it, as in Q2.
- **LOW** [claude:claude-opus-5-5] Attachment A, Lemma 8 (a) — "Rule L is used here and only here" is inexact. Theorem 5 also needs h(x) ≤ h(y), and rule L gives more than the stated consequence (a_0 ≤ Q_y) — medium — "the only consequence used in Lemma 8".
- **LOW** [claude:claude-opus-5-5] Attachment A, Theorem 5 proof — Step 1 of Lemma 4 also uses that y is in the other tree — medium — Say so.
- **LOW** [claude:claude-opus-5-5] Attachment A, "In words" — when h = 0, "other out-neighbour shallower" means the endpoint with fewer out-neighbours wins. That is the tie that blocks A_1 — low — Spell it out.
- **LOW** [cursor:gpt-5.6-sol-xhigh] THEOREM5-draft.md:125 — “Rule L is used here and only here” overlooks its earlier use to obtain \(h(y)\ge h(x)\) — confidence: high — say “the lexicographic tie-break is used here”
- **LOW** [cursor:gpt-5.6-sol-xhigh] THEOREM5-draft.md:168-175 — five cited testing programs are absent from both repository and attachments, so those run claims are unauditable — confidence: high — include the programs/logs or remove the claims
- **LOW** [cursor:grok-4.7-medium] draft Lemma 7 — “(S1) raises it from 0 to 1” fails when the only out-neighbour already has h≥1; weight stays 1 — high — state the h≥1 subcase
- **LOW** [cursor:grok-4.7-medium] draft Theorem 5 citing Lemma 7 — the proof treats +1 on one slot, while a round can raise several slots by more than 1; iteration closes it and no counterexample turned up — high — add the iteration sentence
- **LOW** [cursor:grok-4.7-medium] draft potential paragraph — “three times the g of Section 5” does not match NOTE.md Section 5, which defines F, U, and W — high — delete the citation
- **LOW** [cursor:grok-4.7-medium] draft Lemma 8(d) — the leaf exclusion is not the whole lex rule when h(y)=0 and ε_y=1 — high — state the Q_y comparison; the extra cases still gain ≥0
- **LOW** [cursor:grok-4.7-medium] `third_check.py` default-argument path — R<4 prints ALL OK while missing the finite boundary the text reduces to — high — reject R<4
- **LOW** [cursor:grok-4.7-medium] draft “Checks run” — `pathcheck.py`, `phiU_c3.py`, `gpoolC3.cpp`, `cex1_c3.py`, and `ruleL_emit.py` are not in the repository, so the 10,222,212 profiles, the 634,854 rounds, and the 43-vertex rerun cannot be audited here — high — attach them or drop those sentences
- **LOW** [cursor:gemini-3.8-flash-high] Attachment B:14 — `third_check.py` allows command-line argument R ≤ 0, executing 0 assertions and exiting 0 with `ALL OK` — confidence: high — Validate argument with `if R < 4: sys.exit("R must be >= 4")`.
- **LOW** [cursor:gemini-3.8-flash-high] Attachment A:141-143 — Text states slots of path vertices are non-decreasing without explicitly highlighting that w_k gaining a second out-neighbour raises its missing slot from −1 — confidence: high — Explicitly state that gaining an out-neighbour corresponds to increasing the missing slot from −1 to its non-negative height.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
