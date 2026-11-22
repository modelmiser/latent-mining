<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-153924 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q7** (split) | UNSUPPORTED | CONFIRMED | CONFIRMED |
| **Q8** (split) | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q7, Q8
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 3 MODERATE, 10 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:47`, `README.md:56` — summaries blur ∀rule∃rule-dependent-list into an unconditional logarithmic-lag claim, unsupported for legal future-aware ties — confidence: high — state the causal quantifier explicitly
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:49` — claimed 0.2–0.8 increase per doubling has concrete zero-increase counterexamples — confidence: high — change to 0–0.8
- **MODERATE** [cursor:grok-4.7-medium] `25-throttled-chips/lag/PROOFS.md:60` — the same sentence that proves the upper bound 2+H_{3t} then says the lag lies between 0.056 ln t and ln t; 2+H_{3t} ∼ ln t+3.7, so “≤ ln t” is false — high — replace with “about 0.056 ln t, up to ln t + 4.1”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `lag/PROOFS.md:58-60` — logarithmic lower-envelope formula is undefined for t≤84 and not derived for 85≤t<108 — confidence: high — add “for t≥108”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `lag/REPORT.md:21` — “every point,” “every lag 1,” and “every later top-lag point is ready” overstate the finite-interior reservoir and R2 readiness condition — confidence: high — say “each point of a finite interval; interior lag 1; an interior maximum with a lower neighbour is ready”
- **LOW** [cursor:grok-4.7-medium] `25-throttled-chips/lag/PROOFS.md:59` — log_34((t−84)/24) is undefined for t ≤ 84, but the two-sided bound is stated for every t — high — restrict the sentence to t ≥ 108
- **LOW** [cursor:grok-4.7-medium] `25-throttled-chips/lag/PROOFS.md:79` — “η ≥ 6/5, so η ≥ 2” skips integrality of the chip count (used explicitly only at line 18) — high — add “η is an integer, so η ≥ 2”
- **LOW** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:141` — “no lag exceeds 2+H_n” omits “after a round”; right after an add the proved bound is 2.6+H_n — high — insert “after a round”
- **LOW** [cursor:grok-4.7-medium] `README.md:56` — “the firing rule’s shortfall grows like log t” drops the history-only tie-break limit that Theorem L actually has — medium — add “against tie-breaks that ignore the rest of the list”
- **LOW** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:146` — “2 to 4 in the greedy runs” is only a typical ratio; the N=300000 log has steps near 1 and near 5 — medium — say “typically 2 to 4 per level in the greedy runs”
- **LOW** [cursor:grok-4.7-medium] `25-throttled-chips/lag/PROOFS.md:148` — “stages … under 1.1N in total” never writes the sum; it does hold, since each stage costs ≤ (17/16)m and m shrinks by 32, total < (34/31)N — high — add that one-line sum
- **LOW** [cursor:gemini-3.8-flash-high] 25-throttled-chips/README.md:143-144 — Plain-language summary says "one add at the centre makes the centre the single largest lag, which the firer must then fire, lifting both neighbours for free", which incorrectly attributes the lag increase of the flanks to the firing rather than the adder's C-add — confidence: high — rephrase to "one add at the centre lifts both neighbours while making the centre the single largest lag, which the firer is forced to relieve".
- **LOW** [cursor:gemini-3.8-flash-high] lag/replay.cpp:56 — `excRound5` tracks only `5 * max(eta[x - 1], eta[x + 1]) - t` for the fired site's neighbours rather than the global board height excess after firing, producing large negative diagnostic numbers — confidence: medium — track global board maximum after firing or label as local neighbour excess.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
