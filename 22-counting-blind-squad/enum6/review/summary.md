<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-154052 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q4** (split) | REFUTED | CONFIRMED | CONFIRMED |
| **Q5** (split) | UNSUPPORTED | CONFIRMED | CONFIRMED |
| **Q6** (split) | UNSUPPORTED | REFUTED | REFUTED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q4, Q5, Q6
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 2 CRITICAL, 3 MODERATE, 8 LOW
- **CRITICAL** [cursor:gpt-5.6-sol-xhigh] `enum6/enum.c:74-76,42` — `sim` accepts outputs outside 0…S−1, which can become an out-of-bounds `d[x*3+k]` access on the next tick — confidence: high — parse strictly and reject invalid digits
- **CRITICAL** [cursor:gpt-5.6-sol-xhigh] `enum6/enum.c:83-88,102-104` — unvalidated `cd` or negative chunk IDs can index `dig` outside its 14 elements — confidence: high — validate `0 <= cd <= ND`, chunk bounds, and numeric parsing
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `22-counting-blind-squad/README.md:84` — three-/four-state reproduction is claimed without retained output — confidence: high — remove it or add outputs
- **MODERATE** [cursor:grok-4.7-medium] `README.md:77-79` — "without reading the earlier programs" is stated as fact; nothing in the tree shows the author did not read `solver-a/`, `solver-b/`, or `verifier/` — high — "The task said not to read the earlier programs; the author comments that the program was written from the question."
- **MODERATE** [cursor:grok-4.7-medium] `README.md:17-18` — "an error there could only make it too small" is false for L(7): symmetry omissions can only drop rules, but the referee's searches share one simulator (`README.md:99-100`), and a simulator bug can accept a bad rule and make 11 too big — medium — "Seven states are backtracking only; a simulator bug could move L(7) either way."
- **LOW** [cursor:gpt-5.6-sol-xhigh] `enum6/aggregate.py:36` — internal N=1 is mislabeled N=0 — confidence: high — label it “fails length 2 (internal score 1)”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `enum6/aggregate.py:15-20` — histogram width is not checked, so a line truncated after trailing zero bins can pass — confidence: high — require exactly 20 bins
- **LOW** [cursor:gpt-5.6-sol-xhigh] `enum6/organiser_rerun.txt:1` — unexplained `16` makes the rerun record ambiguous — confidence: high — label or remove it
- **LOW** [cursor:grok-4.7-medium] `README.md:84` — "each chunk was run once" conflicts with the second run of chunks 39 and 77 (`organiser_rerun.txt:1-3`) — high — "one recorded pass; chunks 39 and 77 were rerun and matched."
- **LOW** [cursor:grok-4.7-medium] `README.md:86-87` — the cross-check is described as a count plus one witness, but `reports/solver-a.md:25` already prints this same full histogram (71,265,267,072 … 336), not only 336 — high — "the histogram matches solver A's published s=6 histogram; the 336 rules were not kept as a list, so the set check is one witness."
- **LOW** [cursor:grok-4.7-medium] `aggregate.py:37` — `scoreN`'s 1 (fails length 2, `enum.c:52-57`) is printed as "N=0"; N≥2 are unshifted, so L(6)=8 is unaffected — high — "score 1 (fails length 2): …"
- **LOW** [cursor:gemini-3.8-flash-high] 22-counting-blind-squad/README.md:78 — "without reading the earlier programs" is stated as an established fact rather than an instruction from BRIEF.md:6 or instance self-report, which cannot be verified from repository files — confidence: high — Change to "prompted to write a plain enumerator from the definition without reading earlier programs"
- **LOW** [cursor:gemini-3.8-flash-high] 22-counting-blind-squad/enum6/enum.c:57 — `scoreN` returns 1 when length 2 fails, conflating a rule failing length 2 (0 synchronised lengths) with \(N=1\); `enum sim` outputs `N=1` directly while `aggregate.py:37` manually relabels it to `N=0` — confidence: high — In `scoreN`, return `n == 2 ? 0 : n - 1`

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
