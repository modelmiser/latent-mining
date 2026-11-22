<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-165629 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q3** (split) | REFUTED | CONFIRMED | CONFIRMED |
| **Q6** (split) | REFUTED | CONFIRMED | CONFIRMED |
| **Q7** (split) | REFUTED | REFUTED | CONFIRMED |
| **Q8** (split) | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q3, Q6, Q7, Q8
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 4 MODERATE, 15 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/tight/PROOFS.md:127-128` — numerical sharpness claim is false; 11/10 works — confidence: high — state the sharp cutoff as M>1
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/tight/run_all.sh:3-15` — no working directory satisfies both local script paths and `test_rule.py`’s repository-relative paths; it also requires an absent binary and omits recorded search runs — confidence: high — anchor paths, compile solve, and include all canonical commands
- **MODERATE** [cursor:grok-4.7-medium] `README.md:50` — claims the organiser checked Part A, while `tight/PROOFS.md:5` and `tight/REPORT.md:58` say the proofs are unread and `@@TREVIEW@@` is still a placeholder — high — delete the organiser-check sentence
- **MODERATE** [cursor:grok-4.7-medium] `README.md:51` and `:188` — say the height bound was not exercised; `tight/test_rule.py:47` and `:70` assert 5η≤t+28 after adds and ≤t+24 after rounds on all 89,243 plays (observed maxima 16 and 12) — high — say the inequality was asserted and was not tight
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/README.md:15` — “better” includes an equality at h=9 — confidence: high — use “strictly better through h=8; equal at h=9”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/README.md:51` — says the height bound was not exercised although tests assert it — confidence: high — distinguish tested inequality from untested B3/B4 derivation
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/tight/REPORT.md:14` — “B2 asserted” overstates coverage: only t=t0 with Wset recomputed was checked — confidence: high — say “B2 endpoint consequence”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/tight/test_rule.py:2` — stale claim G(h)≥5h−32 — confidence: high — change to 5h−28
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/README.md:53,55,57` — older lower bounds remain unmarked as superseded — confidence: high — label them historical/superseded
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/tight/PROOFS.md:213-215` — “the constant c” suggests one fixed c, which is unproved — confidence: high — define deficit d(h)=5h−G(h)∈[10,28]
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/tight/solve.cpp:83-91` — search score claims post-fire states but skips final-round fires — confidence: high — describe it as safe states immediately after the final add
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/tight/PROOFS.md:3-4` — “nobody else has read it” conflicts with the later review status — confidence: medium — qualify “at time of writing”
- **LOW** [cursor:grok-4.7-medium] `README.md:15` — “2h−1 is better for h≤9” is false as a strict comparison at h=9, where both bounds are 17 — high — “strictly better for h≤8, equal at h=9”
- **LOW** [cursor:grok-4.7-medium] `tight/test_rule.py:2` — docstring says the tests target G(h)≥5h−32; the code and `REPORT.md:42` show that constant was abandoned for 5h−28 — high — change the docstring to 5h−28
- **LOW** [cursor:grok-4.7-medium] `tight/PROOFS.md:41-42` — “≤1, so it is 0” does not say why the left side of (A) cannot be 1; it is even or at least 3 — high — add that one line
- **LOW** [cursor:grok-4.7-medium] `README.md:53` and `:55` — the √h and 5h−176 rows are still the right theorems but, unlike the 5h−6 row, are not marked superseded, which the short answer (`:23`) says they are for h≥4 — medium — mark them superseded for h≥4
- **LOW** [cursor:grok-4.7-medium] `README.md:179` — the limit bullet still leads with solver A’s proof and never says 5h−28 and 5h−10 already force the limit 5 — medium — add that sentence
- **LOW** [cursor:gemini-3.8-flash-high] `25-throttled-chips/README.md:55` — Results table rows for \(5h - 176\) (line 55) and Corollary U2 (line 57) are superseded for \(h \ge 4\) by row 2 (\(G(h) \ge 5h - 28\)), but unlike row 3 (\(5h - 6\)) they are not marked superseded and line 55 remains in boldface — confidence: high — mark both rows superseded for \(h \ge 4\) and remove bolding from line 55
- **LOW** [cursor:gemini-3.8-flash-high] `25-throttled-chips/tight/test_rule.py:2` — Module docstring states `lower bound G(h) >= 5h-32`, which is an outdated draft constant from before the readiness threshold was lowered from 2 to 6/5 (noted in `REPORT.md:43`) — confidence: high — update docstring to `5h-28`

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
