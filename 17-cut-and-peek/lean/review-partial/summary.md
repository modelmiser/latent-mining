<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-151019 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q4** (split) | UNSUPPORTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q4
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 5 MODERATE, 11 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `17-cut-and-peek/lean/README.md:29` — falsely says Section 4 is not formalised — confidence: high — delete or replace as above
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `17-cut-and-peek/README.md:188-189` — falsely says Lean does not cover partial-opening results, despite `PartialMain.lean:13-35` — confidence: high — say it now covers Proposition 3 and the 36-card protocol only
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `17-cut-and-peek/lean/README.md:55` — “clean full build run by the organising instance” is not established by the one-line log — confidence: high — say only “the recorded build completed successfully”
- **MODERATE** [cursor:grok-4.7-medium] `lean/README.md:28-29` — the trust paragraph says Section 4 is not formalised, which is false for Proposition 3 and the 36-card protocol — high — use the replacement in Q6.
- **MODERATE** [cursor:gemini-3.8-flash-high] 17-cut-and-peek/lean/README.md:28-29 — Section 1 states "The partial-opening results of the note (Section 4) are not formalised", contradicting the "Partial opening (added 21 November)" section added at line 40 of the same file. — confidence: high — Replace with "Of the partial-opening results of the note (Section 4), two are formalised (see below); the search for m ≤ 15 and other facts are not."
- **LOW** [cursor:gpt-5.6-sol-xhigh] `17-cut-and-peek/lean/README.md:59-61` — authorship and unchanged-file history lack an auditable repository record — confidence: high — label these as reported provenance
- **LOW** [cursor:gpt-5.6-sol-xhigh] `17-cut-and-peek/lean/README.md:54` — outside-Lean origin of the certificate lacks a generator or transcript — confidence: high — use the Q4 replacement
- **LOW** [cursor:gpt-5.6-sol-xhigh] `17-cut-and-peek/lean/MUTANT-partial.txt:1,9` — diagnostics name `Mutant.lean`, not the supplied `MutantPartial.lean`, weakening artifact traceability — confidence: high — regenerate with command and source name recorded
- **LOW** [cursor:grok-4.7-medium] `lean/README.md:55` — “a clean full build run by the organising instance” is not identifiable from the one-line `BUILD-partial.log:1`; `PROGRESS-partial.md:2` presents that log as the proving agent’s `./b.sh` run, while `BUILD.log` keeps per-module lines that `b.sh` does not strip — medium — cite only “Build completed successfully (3279 jobs).”
- **LOW** [cursor:grok-4.7-medium] `NOTE.md:351-353` — the machine-check account mentions only the privacy permutations, not the `decide` that every pair of cuts differs on `R` (`PartialCheck.lean:43-44`, `PartialMain.lean:35`) — high — add that sentence.
- **LOW** [cursor:grok-4.7-medium] `PartialCheck.lean:63-65` — the comment says each input’s list entry matches the all-false view, but the default `List.range 36` also covers all-true, where that match is false and is not used — high — restrict the comment to inputs with AND false.
- **LOW** [cursor:grok-4.7-medium] `PROGRESS-partial.md:16` — “three plain `decide`s”, but `PartialMain.lean:35` has four (`⟨0, by decide⟩` plus three) — high — say four.
- **LOW** [cursor:grok-4.7-medium] `MUTANT-partial.txt:1` — the errors name `Mutant.lean`, which is not in the tree; the line numbers match `MutantPartial.lean` — high — name `MutantPartial.lean` in the log.
- **LOW** [cursor:gemini-3.8-flash-high] 17-cut-and-peek/NOTE.md:357-360 — Author attribution was not updated when partial opening was added ("The specification, the four statements and the two layouts were written by the organising AI instance"), leaving authorship of `PartialSpec.lean`, `PartialMain.lean` statements, and `enc36` unstated in NOTE.md. — confidence: high — Update sentence to: "The specifications, the six statements and the three layouts were written by the organising AI instance; the proofs by a second instance..."
- **LOW** [cursor:gemini-3.8-flash-high] 17-cut-and-peek/NOTE.md:341-344 — The "What still has to be trusted" list was not updated for partial opening, omitting the trust assumptions stated in `17-cut-and-peek/lean/README.md:61-62` (`PartialSpec.lean` alignment and digit list transcription). — confidence: high — Add the partial-opening trust assumptions to `17-cut-and-peek/NOTE.md:341-344`.
- **LOW** [cursor:gemini-3.8-flash-high] 17-cut-and-peek/lean/MUTANT-partial.txt:1,9 — Compiler error trace records the filename as `Mutant.lean:9:70` and `Mutant.lean:13:70` rather than the committed file name `MutantPartial.lean`. — confidence: high — Re-run Lean against `MutantPartial.lean` and update `MUTANT-partial.txt`.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
