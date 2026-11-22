<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-064609 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q4** (split) | REFUTED | CONFIRMED | REFUTED |
| **Q8** (split) | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q4, Q8
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 7 MODERATE, 12 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/constant/proofs.txt:56-58` — “for all h” makes the upper bound false at h=1 — confidence: high — restrict the upper-bound chain to h≥2
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:56` — the unqualified top-level bound is false at h=1 — confidence: high — add “for h≥2”
- **MODERATE** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:45` — the status says the review panel read the constant bound, but `25-throttled-chips/README.md:205-223` never mentions `35.2` or `5h-176` — high — delete “read by the review panel (see Review)”
- **MODERATE** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:46` — “two of them replayed” is not in the tree: the only transcript is `constant/replay_n16384.txt:5` (lag 6.4); `constant/list_n512.txt` has no replay output, and lag 6.6 at `constant/lagadv2_big.txt:3` was not replayed — medium — say one list was replayed and 6.6 is from `lagadv2` only
- **MODERATE** [cursor:grok-4.7-medium] `constant/proofs.txt:76` — Observation B cites `REPORT.md`, which is not in the repository — high — cite `constant/lagadv2_big.txt`
- **MODERATE** [cursor:gemini-3.8-flash-high] 25-throttled-chips/constant/proofs.txt:56 — The corollary claims "5h - 176 <= G(h) <= C(h) <= 5h - 6 for all h", which fails at h=1 where G(1)=C(1)=1 but 5h-6=-1 — confidence: high — Restrict the upper bound chain to h >= 2 (with G(1)=C(1)=1).
- **MODERATE** [cursor:gemini-3.8-flash-high] README.md:56 — Experiment 25 summary row states G(h) is between 5h - 176 and 5h - 6 without restricting to h >= 2, failing at h=1 — confidence: high — Qualify the range with "for h >= 2".
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/constant/test_lemmas.py:19,87` — the test can reject a valid equality case not excluded by the proof — confidence: high — test `<=`
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/README.md:46` — “two … replayed” has only one recorded exact replay result — confidence: medium — say “one replayed” or include the second output
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/constant/proofs.txt:76` — it references absent `REPORT.md` — confidence: high — reference `lagadv2_big.txt` and `replay_n16384.txt`
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/constant/test_out.txt:5-9` — stale output says wrong variant 3 passed, although the corrected test fails — confidence: high — label it obsolete or retain only the corrected output
- **LOW** [cursor:grok-4.7-medium] `constant/proofs.txt:56` — the displayed chain is quantified “for all h”, but `C(1) = 1 ≰ 5·1-6`; the parenthetical `h ≥ 2` is the range P3 actually has — high — write “for every h ≥ 2”
- **LOW** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:46` — “about 0.3 each time their length doubles” treats vertex-count `n` as list length; the K=8 series moves by 0.2–0.8 per doubling, and a second `n = 128` run is 2.8 (`constant/lagadv2_big.txt:7`) rather than 3.4 (`:4`) — medium — quote the `n` column and the two endpoints
- **LOW** [cursor:grok-4.7-medium] `constant/test_out.txt:9` — this log says `ALL CHECKS PASSED (wrong=3)`, while `constant/test_out2.txt:1-3` shows the corrected negative test failing — high — remove or mark `test_out.txt` as superseded
- **LOW** [cursor:grok-4.7-medium] `constant/proofs.txt:65` — “lag reaches 7.2 and 12”, but `constant/test_out2.txt:6-7` records 7.2 and 12.2 — high — write 12.2
- **LOW** [cursor:grok-4.7-medium] `constant/test_lemmas.py:86` — the lag check is strict (`10*v < 850+5*e`) while Lemma 2 is `≤` — high — use `<=`
- **LOW** [cursor:grok-4.7-medium] `constant/test_lemmas.py:59-60` — Lemma 1 is checked only as a net `Phi(after) ≤ Phi(before)`; the add’s “at most +1” and the fire’s “exactly −1” are not asserted — high — assert those two increments separately
- **LOW** [cursor:gemini-3.8-flash-high] 25-throttled-chips/constant/proofs.txt:75 — Cites "reaches the lags listed in REPORT.md", but REPORT.md does not exist in the repository; the run outputs are in lagadv2_big.txt — confidence: high — Change reference to lagadv2_big.txt.
- **LOW** [cursor:gemini-3.8-flash-high] 25-throttled-chips/constant/test_out.txt:7-9 — Contains stale run output where deliberately broken test --wrong 3 passed ("ALL CHECKS PASSED (wrong=3)"), whereas test_out2.txt contains the corrected failing run — confidence: high — Overwrite test_out.txt with test_out2.txt or remove the stale output file.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
