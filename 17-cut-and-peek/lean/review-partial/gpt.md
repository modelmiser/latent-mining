<!-- Archived by the organiser on 2026-11-21. Report of the GPT seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — `seen` uses `rot` (`CutPeek/PartialSpec.lean:13-14`; `CutPeek/Spec.lean:15`), `views` maps every cut into a multiset (`PartialSpec.lean:20-21`), and validity exactly states nonempty/privacy/disjointness (`PartialSpec.lean:26-31`); edge cases introduce no weaker protocol — none

Q2: CONFIRMED — the theorem assumes only `[NeZero m]`, an encoding, an opened set, and `IsPartialAnd`, concluding `2 ^ n - 1 ≤ m` (`CutPeek/PartialMain.lean:13-15`); since `2^n ≥ 1`, natural subtraction does not truncate unexpectedly — none

Q3: CONFIRMED — both 36-digit lists match NOTE.md (`PartialMain.lean:18-20`; `NOTE.md:254-256`); `enc36` maps owners 1–3 to `Fin 3` values 0–2 and flips owned base cards (`PartialMain.lean:22-27`; `Spec.lean:31-34`); `R36` is exactly `{0,6,12,18,24,30}` (`PartialMain.lean:30-31`), and `p.val < 36`, so 36-element-list fallback is unreachable — none

Q4: UNSUPPORTED — the reduction’s hypotheses are exactly injection, matching to all-false, and pairwise-cut separation (`PartialCheck.lean:38-45`), and six explicit lists are checked in `thirty_six_cards` (`PartialCheck.lean:66-78`; `PartialMain.lean:35`), but no generator or search transcript establishes that those lists were “found outside Lean” — “six explicit permutations supplied as a certificate and checked by Lean”

Q5: CONFIRMED — position 0’s digit is changed at `MutantPartial.lean:4`, opened position 30 to 31 at `MutantPartial.lean:11`, the same certificate-based construction is attempted at lines 9 and 13, and both attempts are rejected (`MUTANT-partial.txt:1-17`); the prose claims only rejection, not mutant invalidity — none

Q6: REFUTED — `lean/README.md:29` says partial-opening results are not formalised, contradicting its own formalisation account at lines 40-57; moreover the one-line build log proves success only, not “clean,” provenance, or source identity (`BUILD-partial.log:1`) — “Two Section 4 results are formalised below; the recorded build completed successfully”

F: MODERATE — `17-cut-and-peek/lean/README.md:29` — falsely says Section 4 is not formalised — confidence: high — delete or replace as above

F: MODERATE — `17-cut-and-peek/README.md:188-189` — falsely says Lean does not cover partial-opening results, despite `PartialMain.lean:13-35` — confidence: high — say it now covers Proposition 3 and the 36-card protocol only

F: MODERATE — `17-cut-and-peek/lean/README.md:55` — “clean full build run by the organising instance” is not established by the one-line log — confidence: high — say only “the recorded build completed successfully”

F: LOW — `17-cut-and-peek/lean/README.md:59-61` — authorship and unchanged-file history lack an auditable repository record — confidence: high — label these as reported provenance

F: LOW — `17-cut-and-peek/lean/README.md:54` — outside-Lean origin of the certificate lacks a generator or transcript — confidence: high — use the Q4 replacement

F: LOW — `17-cut-and-peek/lean/MUTANT-partial.txt:1,9` — diagnostics name `Mutant.lean`, not the supplied `MutantPartial.lean`, weakening artifact traceability — confidence: high — regenerate with command and source name recorded

Unasked: the stale broad disclaimer at `17-cut-and-peek/README.md:188-189` is the most consequential prose defect outside the specifically named rows.
