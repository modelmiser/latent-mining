<!-- Archived by the organiser on 2026-11-21. Report of the second Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 2: experiment 17, partial-opening Lean files

Paths are relative to `17-cut-and-peek/`. I read every new or changed file named in the brief, plus `Spec.lean`, `b.sh`, `BUILD.log`, `PROGRESS-partial.md` and NOTE.md Sections 1, 4 and 6. I also ran my own Python check (in the scratchpad, not in the repo) against the note's strings.

## Verdicts

**Q1: CONFIRMED.**
- `seen R w j p = rot j w p = w (p - j)` (PartialSpec.lean:15-16, Spec.lean:16). This is the card at opened position p after a cut by j.
- `views` maps `seen R w` over `Finset.univ.val` of `ZMod m` (PartialSpec.lean:20-21). That gives exactly m entries, one per cut, with multiplicity kept.
- `IsPartialAnd` (PartialSpec.lean:27-31) has three parts: R non-empty; privacy for all pairs x, y with `andFn n x = false` and `andFn n y = false`, where `andFn` is "all bits true" (Spec.lean:48), so "AND false" means "not 1…1"; and correctness as "no view of 1…1 is a view of 0…0". This is the note's definition (NOTE.md:195-200).
- Words are functions on the subtype `↥R`, and equality of words is extensional, as it should be.
- I found nothing that makes the definition weaker or stronger than the note's:
  - Non-emptiness of R already follows from correctness, because with R empty every word equals the one empty word.
  - n = 0: the all-true and all-false inputs coincide, so correctness fails and the predicate is unsatisfiable. The bound 0 ≤ m is trivial anyway, and the note assumes n ≥ 1 (NOTE.md:55).
  - n = 1 and m = 1 are harmless.
  - Encoding matches Section 1 (NOTE.md:55-59): a card controlled by a bit differs for the two values via `xor` (Spec.lean:31-34), and a bit may control no card in both.

**Q2: CONFIRMED.**
- PartialMain.lean:12-14 states `2 ^ n - 1 ≤ m` for an arbitrary `R` and `e`, assuming only `[NeZero m]` and `IsPartialAnd e R`.
- 2^n ≥ 1, so natural-number subtraction is exact.
- `partial_and_card_bound_aux` (PartialBound.lean:184-185) has the same signature, so nothing extra is assumed.

**Q3: CONFIRMED.**
- My script finds that `owner36` equals the digits of `111111012301331022102230222123003012` and `u0_36` equals the digits of `000011000011011001000000110111111011` (36 each, all positions).
- The note's u1 string differs from u0 exactly at the owned positions and agrees on the 0-owner positions (no exceptions), so `xor base bit` reproduces u1.
- `enc36` maps digit 1/2/3 to `some 0/1/2` and 0 to `none`; `base` is `u0 = 1` (heart = `true`) (PartialMain.lean:22-28).
- `R36 = {0,6,12,18,24,30}` (PartialMain.lean:31).
- `p.val < 36 =` the length of both lists, so the `getD … 0` fallback is never used.
- From the definitions, my script gives the same multiset (33 distinct words) for all seven AND-0 inputs, and M(1,1,1) has 16 words disjoint from it. This agrees with the note.

**Q4: CONFIRMED.**
- `partial3_of_checks` (PartialCheck.lean:38-45) takes three hypotheses:
  - `L a b c` injective for every input;
  - for every AND-0 input, every cut j and every p ∈ R: `row(x)(p − L j) = row(0…0)(p − j)`, meaning the view of x at cut L(j) equals the view of 0…0 at cut j;
  - for every pair of cuts (j, k), some p ∈ R where the 1…1 row at j differs from the 0…0 row at k.
- This matches the README's description. `hcorr` is actually equivalent to the correctness clause, not just sufficient for it.
- `cert36` has six explicit lists, and the identity (`List.range 36`) for 000 and 111. "Six permutations … a certificate found outside Lean that Lean checks" is accurate: injectivity and matching are both checked by `decide`. The correctness half uses no certificate.

**Q5: CONFIRMED, with a LOW point.**
- MUTANT-partial.txt shows `decide` refuting `hpriv` and `hcorr` for the one-digit mutant (u0 at position 0, an owned position) and for `RBad`. The columns 70 and 82 match the last two `(by decide)` on lines 9 and 13 of MutantPartial.lean.
- But the log names the file `Mutant.lean`, so it was produced from a file of another name.
- The prose ("the same proof term is rejected") does not claim the mutants are invalid. Strictly, the `hcorr` refutation does imply that correctness fails for both mutants, but nothing claims that.
- "when one digit … or one opened position is changed" can be read as "any one", while only one instance of each was tested (see F4).

**Q6: REFUTED, for two stale sentences (F1, F2).** The rest checks out:
- PartialSpec.lean has 33 lines.
- AXIOMS-partial.txt lists propext, Classical.choice and Quot.sound for both theorems.
- Grepping all four new `.lean` files and MutantPartial.lean for `sorry|admit|native_decide|axiom|unsafe|implemented_by|opaque|set_option` finds nothing.
- NOTE.md:356-357 and lean/README.md say that the m ≤ 15 search and the other facts of Section 4 are not formalised.
- "Build" is only partly supported (F3). The claim that the prover "changed nothing except sorry/imports" cannot be checked from the repository, because both files are untracked and have no earlier version.

## Findings

**F1: MODERATE, confidence high.** lean/README.md:35-36 still says: "The partial-opening results of the note (Section 4) are not formalised." The new section at :40-62 of the same file contradicts this. The title at :1, "Lean check of the full-opening impossibility", is also out of date.
- Replacement for :35-36: "Of Section 4, Proposition 3 and the validity of the 36-card layout are formalised (section 'Partial opening' below); the search for m ≤ 15 and the other facts are not."
- Replacement title: "Lean check of experiment 17".

**F2: MODERATE, confidence high.** README.md (experiment 17), lines 183-189: "…it does not cover the partial-opening results." This is now false for the bound and for the 36-card layout.
- Replacement: "…it covers, of the partial-opening results, only the bound 2^n − 1 and the validity of the 36-card protocol (as statements about a further 33-line specification); the search for m ≤ 15 is not machine-checked."

**F3: MODERATE, confidence medium.** lean/README.md:52 says "`BUILD-partial.log`: a clean full build". The log is the single line "Build completed successfully (3279 jobs)."
- With `b.sh`'s filters, a build that compiles modules prints `✔ … Built …` lines, as BUILD.log does. A log with only the final line is what an up-to-date (no-op) build gives.
- So the log shows that all 3279 targets were current and succeeded. It does not show a full rebuild, or that the build ran without warnings.
- Replacement: "`BUILD-partial.log`: the result of `./b.sh` on the finished files (all 3279 jobs up to date and successful; the modules had been compiled by earlier runs)". Alternatively, rerun after `lake clean` of the CutPeek modules and keep the full log.

**F4: LOW, confidence high.** NOTE.md:353-355 and lean/README.md:55-56 say "rejected when one digit of the layout or one opened position is changed".
- Replacement: "rejected for a layout with one digit changed (position 0 of u0) and for one with one opened position changed (30 → 31) (`lean/MutantPartial.lean`)".
- Also note that MUTANT-partial.txt was produced under the file name `Mutant.lean`, or regenerate it from `MutantPartial.lean`.

**F5: LOW, confidence medium.** NOTE.md:347 says "a further 33 lines of definitions". The 33 lines include the header comment, the imports and the docstrings; there are three definitions.
- Replacement: "a further 33-line file of definitions".

**F6: LOW, confidence medium.** lean/README.md:59-60 says the prover "changed nothing in the two given files except to replace `sorry` and add imports". The repository does not support this, because no pre-proof version of `PartialSpec.lean` or `PartialMain.lean` exists in git.
- Replacement: either commit the pre-proof statements, or add "(not verifiable from the repository: the files given to the prover were not committed)".

No CRITICAL defects: the definitions and both statements say what Section 4 means.
