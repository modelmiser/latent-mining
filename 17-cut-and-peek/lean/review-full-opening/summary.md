<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-061341 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q4** (split) | REFUTED | CONFIRMED | CONFIRMED |
| **Q9** (split) | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q8 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q4, Q9
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 1 MODERATE, 9 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `NOTE.md:338-339` — singular authorship is contradicted internally and build-runner provenance is unsupported — confidence: high — use the replacement wording above
- **LOW** [cursor:gpt-5.6-sol-xhigh] `lean/CutPeek/Spec.lean:40-43` — `patch`’s comment is false without injectivity — confidence: high — add the injectivity qualification and overwrite order
- **LOW** [cursor:gpt-5.6-sol-xhigh] `NOTE.md:72-73` — the prose’s sign convention is opposite to Lean’s — confidence: high — note that Lean uses the globally negated convention
- **LOW** [cursor:gpt-5.6-sol-xhigh] `NOTE.md:334-335` — the trust inventory omits auditing `FiveCard.lean` when identifying the witness as the five-card trick — confidence: high — include its statement and layout in the semantic trust boundary
- **LOW** [cursor:grok-4.7-medium] `17-cut-and-peek/lean/CutPeek/Rows.lean:12` — `sgn true = 1` maps hearts to `+1`, while `NOTE.md:73` maps clubs to `+1`; a reader matching the write-up to the proof sees the opposite sign. Statements are unaffected — high — say the formalization swaps the two suits relative to Section 2
- **LOW** [cursor:grok-4.7-medium] `17-cut-and-peek/lean/CutPeek/Spec.lean:40-43` — the `patch` comment states the three-coordinate update with no injectivity hypothesis; if `i 0 = i 2`, the outer `Function.update` keeps `x 2` and drops `x 0` — high — add “when `i` is injective; otherwise the last write wins”
- **LOW** [cursor:grok-4.7-medium] `17-cut-and-peek/lean/CutPeek/Spec.lean:10` — “positions modulo `m`” is false for `m = 0`, where `ZMod 0 = ℤ`; only the theorems exclude that case — high — restrict the comment to `m ≥ 1`
- **LOW** [cursor:grok-4.7-medium] `17-cut-and-peek/lean/PROGRESS.md:4` — it says `SORRY.txt` is empty, but `SORRY.txt:1` is the “admits” comment; `lean/README.md:18-20` already says that — high — replace “empty” with “one comment hit”
- **LOW** [cursor:gemini-3.8-flash-high] `17-cut-and-peek/lean/CutPeek/Spec.lean:40` — `patch` docstring omits the condition that `i` must be injective to prevent index shadowing — confidence: high — Document in docstring that `i` is assumed injective.
- **LOW** [cursor:gemini-3.8-flash-high] `17-cut-and-peek/lean/PROGRESS.md:4` — States `(SORRY.txt empty)` when `SORRY.txt` contains one false-positive comment line matching the substring "admits" from `FiveCard.lean:16` — confidence: high — Note on line 4 that `SORRY.txt` contains only the "admits" docstring line.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
