<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261118-144657 — (repository) — round 8

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $6.144; this month $173.71 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $4.655, input 70 + cache write 157,575 + cache read 2,714,695 tokens — cache hit 94%
- claude:claude-opus-5-5: $1.489, input 62 + cache write 105,646 + cache read 1,657,036 tokens — cache hit 94%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q1** (split) | CONFIRMED | CONFIRMED | REFUTED | REFUTED | CONFIRMED |
| **Q3** (split) | CONFIRMED | CONFIRMED | REFUTED | REFUTED | REFUTED |
| **Q4** (split) | CONFIRMED | REFUTED | REFUTED | REFUTED | REFUTED |
| **Q5** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q1, Q3, Q4, Q5
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 8 MODERATE, 24 LOW
- **MODERATE** [claude:claude-fable-5-1] 14-forest-orientation/README.md:186 — "At n = 1000 it gives T ≥ 314, against an upper bound of 549" contradicts the new result (upper bound is 333) — high — replace 549 by 333.
- **MODERATE** [claude:claude-fable-5-1] 14-forest-orientation/prior-art.md:20 — "(The bounds are now 1/3 ≤ L ≤ 1/2; see NOTE.md.)" stale, calls L open — high — "(L = 1/3 by NOTE.md Theorems 1 and 5.)".
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `prior-art.md:20` — stale `L≤1/2` contradicts the proved `L=1/3` — confidence: high — update the bound
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `upper/third_pool.cpp:65-68` — endpoint signature reduction skips distinct real rounds while the header says all replies are generated — confidence: high — enumerate every endpoint or narrow the claim
- **MODERATE** [cursor:grok-4.7-medium] `NOTE.md:772-778` — case (d) is called a finite check, but only the both-heights-≥1 region is reduced; one height 0 is infinite, so the hand proof does not close (Lean `free_some` and `third_check.py` are separate) — high — if one height is 0 and the other is ≥2, the new weight is `2+ε` of the height-0 neighbour and independent of the larger height, so heights in `{0,1,2}` suffice
- **MODERATE** [cursor:grok-4.7-medium] `README.md:186` — “upper bound of 549” at n=1000; `3T+1≤n` gives 333 — high — write 333
- **MODERATE** [cursor:grok-4.7-medium] `prior-art.md:20` — still states the current bounds as `1/3 ≤ L ≤ 1/2` — high — state `L = 1/3`
- **MODERATE** [cursor:gemini-3.8-flash-high] `14-forest-orientation/README.md:186` — Stale upper bound 549 for \(n = 1000\) in lower-bound section contradicts Theorem 5 and `NOTE.md:894` (333) — high — Replace "against an upper bound of 549" with "against an upper bound of 333"
- **LOW** [claude:claude-fable-5-1] lean/SPEC.md (upper part, "Source" line), lean/FO/UpperSpec.lean:7, UpperCore.lean:2, UpperLemma6.lean:2 — cite "NOTE.md, Section 5" for Theorem 5/Lemmas 6-8, which are in Section 6 — high — change to Section 6.
- **LOW** [claude:claude-fable-5-1] lean/SPEC.md "Mutation tests … (in tmp/, not in the build)" — lean/tmp/ is empty; PROGRESS.md says the scratch files are not in the repository — high — say "not kept" or add them.
- **LOW** [claude:claude-fable-5-1] NOTE.md:819 — third_paths.py also limits the height of z to SPREAD (third_paths.py:32); the sentence lists only α_i, β — high — add "height of z up to 4".
- **LOW** [claude:claude-fable-5-1] NOTE.md:832-842, README.md:276-279 — the tie-rule search, the LP fits and the 43-vertex counterexample are not in the repository — medium — mark "not in the repository" or add them.
- **LOW** [claude:claude-fable-5-1] NOTE.md:902-904, lean/PROGRESS.md — "rebuilt from clean by another instance" is provenance I cannot verify from files — low — none.
- **LOW** [claude:claude-opus-5-5] README.md:186 — the lower-bound bullet still says "against an upper bound of 549" at n = 1000; it is now 333 — high — fix the number
- **LOW** [claude:claude-opus-5-5] prior-art.md:20 — still says "The bounds are now 1/3 ≤ L ≤ 1/2", which contradicts L = 1/3 — high — update it as prior-art-deep.md was
- **LOW** [claude:claude-opus-5-5] NOTE.md:40, README.md:58-59 — "superseded": at n = 4 Theorem 3 gives T(4) ≤ 0, but Theorem 5 only ≤ 1 — high — add "except at n = 4"
- **LOW** [claude:claude-opus-5-5] lean/SPEC.md:141, UpperSpec.lean:7, UpperLemma6.lean:2, UpperCore.lean:2 — they cite "NOTE.md Section 5" for Theorem 5; it is Section 6 — high — renumber
- **LOW** [claude:claude-opus-5-5] third_pool.cpp:4-5, 66-69; NOTE.md:832 — the header says all replies are generated, but endpoints are reduced to one per class. Off-path weight changes depend on in-neighbours, so rounds are missed — high — say so in the header and the note
- **LOW** [claude:claude-opus-5-5] README.md:265-270 — "rests on the normal form … the n/3 bound" is now ambiguous between the two n/3 bounds, and neither machine-checked bound uses it — medium — write "the hand proof of the lower bound"
- **LOW** [claude:claude-opus-5-5] README.md:294-301 — "That is now explained" overclaims: (n−1)/3 caps the probes but does not explain why they equal the pair-game value — medium — "is consistent with"
- **LOW** [claude:claude-opus-5-5] NOTE.md:836-845, README.md:277 — the tie-break search, the linear programs, the 43-vertex example and the "67 million rounds" are still not in the repository (open since round 7) — high — attach them or drop the claims
- **LOW** [claude:claude-opus-5-5] lean/b.sh — output is piped through grep, so the exit status is grep's; BUILD.log is filtered; AXIOMS.txt lost its "exit 0" line — medium — record `${PIPESTATUS[0]}`
- **LOW** [claude:claude-opus-5-5] README.md:193-194 — "an empty min … can only lower T" points the unsafe way for an upper bound. It is harmless here because a reply is exhibited — medium — add that remark
- **LOW** [claude:claude-opus-5-5] NOTE.md:1032 — "reviewed by five models" does not cover the integrated Theorem 5 (code form, Lean) until this round — low — date the review
- **LOW** [cursor:gpt-5.6-sol-xhigh] `lean/SPEC.md:141`, `lean/FO/UpperSpec.lean:7`, `UpperCore.lean:2`, `UpperLemma6.lean:2` — wrong NOTE section — confidence: high — change 5 to 6
- **LOW** [cursor:gpt-5.6-sol-xhigh] `lean/PROGRESS.md:2,80-82` — still says the specification has had no independent audit, contradicting the recorded panel audit — confidence: high — mark these as historical or update them
- **LOW** [cursor:grok-4.7-medium] `SPEC.md:141` — Theorem 5 is cited as Section 5; the proof is Section 6 — high — “Section 6”
- **LOW** [cursor:grok-4.7-medium] `SPEC.md:205` — “the draft does not say” weights can be negative, but `NOTE.md:609-610` does — medium — drop UD5 or point it at the old draft only
- **LOW** [cursor:gemini-3.8-flash-high] `14-forest-orientation/lean/SPEC.md:141` — Cites `NOTE.md` Section 5 for Theorem 5 and Lemmas 6–8, but they are in Section 6 — high — Replace "Section 5" with "Section 6"
- **LOW** [cursor:gemini-3.8-flash-high] `14-forest-orientation/lean/FO/UpperSpec.lean:6` — Docstring cites "Theorem 5 of ../NOTE.md, Section 5" instead of Section 6 — high — Replace "Section 5" with "Section 6"
- **LOW** [cursor:gemini-3.8-flash-high] `14-forest-orientation/lean/FO/UpperCore.lean:2` — Header comment refers to "NOTE.md, Section 5" instead of Section 6 — high — Replace "Section 5" with "Section 6"
- **LOW** [cursor:gemini-3.8-flash-high] `14-forest-orientation/lean/FO/UpperLemma6.lean:2` — Header comment refers to "NOTE.md (Section 5)" instead of Section 6 — high — Replace "Section 5" with "Section 6"

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
