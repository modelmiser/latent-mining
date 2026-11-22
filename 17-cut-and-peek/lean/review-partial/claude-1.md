<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1: experiment 17, Lean partial opening

Method: read the four new `.lean` files, `Spec.lean`, NOTE.md Sections 1, 4, 6, both READMEs and the evidence files. Nothing was built. I wrote my own Python check (outside the repository) that recomputes M(x) from the definition, using the digits parsed from `PartialMain.lean`.

## Verdicts

**Q1: CONFIRMED.** `seen R w j p = rot j w p.1 = w (p − j)` (PartialSpec.lean:15-16, Spec.lean:16). `views` maps `seen` over `Finset.univ.val`, so one entry per cut, with multiplicity (:20-21). `IsPartialAnd` (:27-31) is non-empty R, privacy over all pairs with `andFn = false` (the same as "all x ≠ 1…1"), and correctness as in the note.
- Not weaker. `R.Nonempty` is redundant: for empty R the correctness clause already fails, and the bound proof discards it (PartialBound.lean:130). Bits that own no card are allowed, where `17-cut-and-peek/README.md:30` requires at least one per player, but such an encoding can never satisfy privacy and correctness together, so nothing extra is accepted.
- Not stronger. `Encoding` is NOTE Section 1's definition.
- Edge cases: for n = 0 the all-true and all-false inputs coincide, so `IsPartialAnd` is false and the bound is vacuous. For n = 1 and m = 1 the bound reads 1 ≤ m. `ZMod 0` is excluded by `[NeZero m]`. All harmless.

**Q2: CONFIRMED.** PartialMain.lean:12-13. Since 2^n ≥ 1, the truncated subtraction is exact. The only hypotheses are `[NeZero m]` and `h`. It covers n = 0, which the note excludes (trivial).

**Q3: CONFIRMED.**
- Digits: my script finds `owner36` and `u0_36` equal to the NOTE.md:254-255 strings, 36 digits each, and also equal to `solver-a/w_n3_m36.txt`.
- u1: the note's u1 string is u0 flipped exactly on the owned positions and equal to it on the additional ones.
- Reading: `enc36` (:22-28) maps digit i to `some (i−1)`, 0 to `none`, and base to `digit = 1`.
- `R36`: all six multiples of 6 below 36.
- Fallback: `p.val < 36` and both lists have length 36, so the `getD` fallback is never used for a position.
- Validity recomputed from the definition: privacy and correctness both hold, with 33 distinct words for AND 0 and 16 for 1,1,1, matching NOTE.md:264-265.

**Q4: CONFIRMED.**
- Hypotheses of `partial3_of_checks` (PartialCheck.lean:38-45): `hinj` for all eight inputs, `hpriv` for inputs with AND false against the all-false row, `hcorr` for every pair of cuts. This matches the README and the docstring.
- Certificate: my script finds each of the six lists to be a permutation of 0..35 that satisfies `hpriv`. Input 000 (and the unused 111) take the identity through `List.range 36` (:74). "A certificate found outside Lean that Lean checks" is accurate.
- Fallback: `perm36`'s `getD` fallback is also never reached.

**Q5: CONFIRMED.**
- The same term is used with one base digit changed (position 0) and with one opened position changed (30 to 31), at MutantPartial.lean:4-13.
- The line and column numbers in MUTANT-partial.txt match that file: the second and third `decide` (`hpriv`, `hcorr`) fail, and `hinj` passes.
- No sentence claims the mutants are invalid layouts. In fact both are: my script finds privacy and correctness false for each, and the log's "`decide` proved … is false" for `hcorr` is exactly the failure of correctness. The prose claims less than the evidence shows.
- All 36 single-digit changes to u0 give an invalid layout (my script).

**Q6: REFUTED in one place (F1); otherwise CONFIRMED or UNSUPPORTED as listed.**
- "33 lines": confirmed (`wc -l`), as is 53 for `Spec.lean`.
- Three standard axioms: AXIOMS-partial.txt:1-2.
- Forbidden words: none in `Partial*.lean`.
- Unformalised parts: stated at NOTE.md:355-356.
- "Clean full build" and who wrote what: UNSUPPORTED by the files themselves (F3, F5).

## Findings

**F1. MODERATE, confidence high.** `lean/README.md:28-29` still says "The partial-opening results of the note (Section 4) are not formalised", which lines 40-62 of the same file contradict. The title (:1) and the "What is proved" list (:5) also cover full opening only.
Replacement for :28-29: "Of the partial-opening results of the note (Section 4), only the two in the section below are formalised."
Replacement title: "Lean check of the full-opening impossibility and of two partial-opening results".

**F2. LOW, confidence high.** MUTANT-partial.txt:1,6,9,14 cite `Mutant.lean`; the file shipped is `MutantPartial.lean`. The contents agree by line and column, but the log was produced from a file of another name.
Replacement: regenerate the log from `MutantPartial.lean`, or add to lean/README.md:56 "(the log names the file `Mutant.lean`, its name when the test was run)".

**F3. LOW, confidence medium.** `BUILD-partial.log` is the single line "Build completed successfully (3279 jobs)."
- It names no module, unlike `BUILD.log`.
- 3279 = 3275 + 4 is consistent with the four new modules, but the log does not show they were compiled in that run, nor that it was a from-scratch build.
- `b.sh:7` filters some output lines.
Replacement for lean/README.md:55: "`BUILD-partial.log`: the last line of a build of the whole library by the organising instance (3279 jobs, four more than before; modules already compiled are not listed)."

**F4. LOW, confidence high.** Top-level `README.md` row 17, evidence column: "the bound 2^n − 1 on the number of cards is also machine-checked". The result column never states this bound, nor which model it is for.
Replacement: "that a protocol for n players with some cards turned up needs at least 2^n − 1 cards is also machine-checked in Lean".

**F5. LOW, confidence medium.** Two authorship points.
- NOTE.md:357-361 ("The specification, the four statements and the two layouts were written by the organising AI instance…") now follows the partial-opening sentences but covers only the old files.
  Replacement: add "; the same division holds for the partial-opening files".
- lean/README.md:60-61 ("it changed nothing in the two given files except to replace `sorry` and add imports") cannot be checked from the tree: the files are untracked and no earlier version is included. `PROGRESS-partial.md` agrees with it.
  Replacement: prefix "According to the proving instance's log,".

**F6. LOW, confidence high.** NOTE.md:345-346, "Of Section 4, two results are (added on 21 November)", is hard to parse.
Replacement: "Two results of Section 4 are formalised (added on 21 November):".

No CRITICAL finding: the definitions and both statements say what Section 4 says.
