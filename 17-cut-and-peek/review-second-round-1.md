<!-- Archived by the organiser on 2026-11-21. Report of a second-round reviewer (one Claude instance) on this experiment's page, taken from the session transcript. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

Second-round review, experiment 17 (Lean sentences in `17-cut-and-peek/README.md`, `NOTE.md` Section 6 "Machine check", `lean/README.md`, top-level row 17). I found no CRITICAL defect, one MODERATE and four LOW. Nothing was built; everything was checked by reading the Lean files and logs, plus one small script.

**MODERATE**

1. `lean/README.md:31-33` — "the specification, the four statements and the two layouts were written first, by the organising instance; a second instance wrote the proofs and was not allowed to change them."
   - Why: `lean/PROGRESS.md:31-36` says `FiveCard.lean` and `SixCard.lean` were "Added by the organiser after the agent finished", and `17-cut-and-peek/README.md:278` says the six-card check was added on a reviewer's suggestion, after the panel. So the layouts were not written first. Their proofs (`by decide`) are also the organiser's, not the second instance's.
   - Replacement: "The specification and the four statements were written first, by the organising instance; a second instance wrote the proofs and was not allowed to change them. The two layouts and their proofs (by `decide`) were added afterwards by the organising instance, the six-card one after the review."

**LOW**

2. `NOTE.md:361-364` — "The specification, the four statements and the two layouts were written by the organising AI instance; the proofs by a second instance, which was given the specification and statements …"
   - Why: same issue in milder form. It does not say "first", but it reads as if the second instance proved the layouts.
   - Replacement: after "second instance" add "(the two layout checks, added later, are entirely the organising instance's)".

3. `lean/README.md:58-59` — "The search for the forbidden words listed above finds nothing in the new files either."
   - Why: true for `CutPeek/Partial*.lean` and `MutantPartial.lean` (my grep). But the new `AxiomsPartial.lean` contains `#print axioms`, which matches `axiom`, as `Axioms.lean` does. There is also no log of this second search; `SORRY.txt` is dated 06:25 and predates the partial files.
   - Replacement: "… finds nothing in the new files under `CutPeek/` or in `MutantPartial.lean` either (not logged; `AxiomsPartial.lean` contains only `#print axioms`)."

4. `17-cut-and-peek/README.md:291-294` — "Not changed: two inaccurate remarks in the proving instance's own files …"
   - Why: there is a third. `lean/PROGRESS-partial.md:3-4` says "full `./b.sh` clean (BUILD-partial.log)", but `lean/README.md:55` says `BUILD-partial.log` is the organiser's later rebuild.
   - Replacement: say "three", and add "and its log's reference to `BUILD-partial.log`, which is now the organiser's rebuild".

5. `lean/README.md:69` — "`PROGRESS.md` is the proving agent's log."
   - Why: lines 31-40 of that file are the organiser's additions, and `PROGRESS-partial.md` is not named anywhere in the file list.
   - Replacement: "`PROGRESS.md` and `PROGRESS-partial.md` are the proving agents' logs (the former with a section added by the organiser)."

**Confirmed clean**

- **Line counts:** `Spec.lean` is 53 lines and `PartialSpec.lean` 33. "About 410 lines of proof" matches Unit + Fourier + Rows + Cube + Odd = 414. `PROGRESS-partial.md`'s 196 and 80 are right.
- **Versions:** Lean 4.33.1; Mathlib `0df444a360…` in both `lakefile.toml` and the manifest.
- **Axioms:** all six full-opening names and both partial names show only the three standard axioms.
- **Build logs:** `BUILD-partial.log` shows exactly 14 modules built and no warnings.
- **Statements:** the descriptions of `seven_force_eighth`, `no_scfo_of_odd_corner`, `no_scfo_and` (n ≥ 3), `no_scfo_eq` (n ≥ 4), `partial_and_card_bound` (`2 ^ n - 1 ≤ m`, natural subtraction) and `thirty_six_cards` match `Main.lean` and `PartialMain.lean`.
- **Definitions:** `IsPartialAnd` matches the README's and the note's Section 4 definition (R non-empty, privacy, correctness as 1…1 against 0…0).
- **Certificate:** six explicit permutations plus identity; four uses of `decide`, where the proving log says three, as `README.md` states.
- **Sign conventions:** `Rows.lean` has hearts as +1, as both documents say.
- **Layout digits (recomputed):** `owner36` and `u0_36` equal the note's strings, and the note's `u1` equals `u0` flipped exactly on owned positions.
- **Layout validity (recomputed):** valid, with 33 and 16 distinct words, as `NOTE.md` says.
- **Failure tests:** `MutantPartial.lean` and `MUTANT-partial.txt` agree with the description (digit 0 changed 0→1; position 30→31; file name matches). I recomputed the reviewer's claim: all 36 single-digit changes of `u0_36` give an invalid layout, and the 31-for-30 opened set is invalid too.
- **Cross-document consistency:** the three documents agree on what is and is not formalised (OR, majority, the m ≤ 15 search and the other Section 4 facts are not). I found no remaining sentence saying the partial-opening results are unformalised. "What to distrust", Results rows 60, 62, 63 and 67, and the Files entry are consistent with the Lean folder.
- **Top-level row 17:** consistent with the page.

**Not checkable from the folder:** what the five reviewers said (their reports are not included).

Script: `(scratch)/chk.py`. Nothing was written under `(workspace)`.
