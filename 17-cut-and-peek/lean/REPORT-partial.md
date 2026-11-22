<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the instance that proved the two partial-opening theorems in Lean, verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

# Report: partial opening in Lean (experiment 17)

Both theorems are proved: `CutPeek.partial_and_card_bound` and `CutPeek.thirty_six_cards`. The final full `./b.sh` exited 0 (3279 jobs, no warnings) and both theorems depend only on the three standard axioms.

REPORT.md was not written: the harness refused the write, so this text is the report.

## Axioms output (`AXIOMS-partial.txt`)
```
'CutPeek.partial_and_card_bound' depends on axioms: [propext, Classical.choice, Quot.sound]
'CutPeek.thirty_six_cards' depends on axioms: [propext, Classical.choice, Quot.sound]
```

## Changes to the two given files
- **`PartialSpec.lean`**: none.
- **`PartialMain.lean`**: no elaboration fix was needed; statements and definitions are as given.
  - Added `import CutPeek.PartialBound` and `import CutPeek.PartialCheck`.
  - `partial_and_card_bound`: `:= by sorry` became `:= partial_and_card_bound_aux e R h`.
  - `thirty_six_cards`: `:= by sorry` became `:= partial3_of_checks enc36 R36 ⟨0, by decide⟩ perm36 (by decide) (by decide) (by decide)`.
- **`CutPeek.lean`**: four imports added (PartialSpec, PartialBound, PartialCheck, PartialMain).
- None of the earlier finished files was touched.

## `set_option` and forbidden constructs
- There is no `set_option` anywhere; plain `decide` sufficed, with no raised `maxRecDepth` and no `decide +kernel`.
- A grep of `CutPeek/Partial*.lean` for `sorry`, `admit`, `axiom`, `native_decide`, `unsafe`, `implemented_by`, `opaque` and `set_option` finds nothing.

## How each is proved
- **`partial_and_card_bound`** (`PartialBound.lean`) follows Proposition 3 of the note.
  - Cards are signs (`true` = −1); `corr R S w` is the note's C_S, with S a `Finset ↥R`.
  - `corr` is a function of `views`, and equal `corr` for every S gives equal `views` (via 2^|R| · count t = Σ_S χ_S(t) · C_S).
  - Correctness then gives an S with C_S(all-true) ≠ C_S(all-false).
  - For a non-zero v, the sum of C_S(x) against the character of v is 0 if no shift has parity vector v, while privacy makes it ±δ ≠ 0. So some shift has parity vector v.
  - The non-zero v therefore lie in the image of the shifts, giving 2^n − 1 ≤ m.
  - n = 0 needs no separate case.
- **`thirty_six_cards`** (`PartialCheck.lean`) reduces to finite checks through a general lemma `partial3_of_checks`.
  - Privacy: for each of the six inputs with AND false other than all-false, an explicit permutation of the 36 shifts (`cert36`) matches its views with those of all-false. Lean checks injectivity and the match by `decide`.
  - Correctness: `decide` checks that for all 36 × 36 pairs of shifts some opened position distinguishes the all-true row from the all-false row.
  - The six permutations were found by a Python search outside Lean. They are only a certificate; Lean checks them.

## Line counts
| File | Lines |
|---|---|
| `CutPeek/PartialBound.lean` (new) | 196 |
| `CutPeek/PartialCheck.lean` (new) | 80 |
| `CutPeek/PartialMain.lean` | 37 (was 35) |
| `CutPeek/PartialSpec.lean` | 33 (unchanged) |

## What failed
Nothing failed. Both new files compiled on the first build apart from linter warnings (deprecated `push_neg`, unused `[NeZero m]`), which I fixed. `PartialMain` takes about 9 s to build. The thread-creation flake noted in the earlier `PROGRESS.md` did not occur.

## Not done
I did not run a deliberate-failure check on the `decide`s (for example a corrupted certificate entry or a changed layout digit) to watch them reject.

## Files
All under `(staging)/lean/`:
- `CutPeek/PartialBound.lean`
- `CutPeek/PartialCheck.lean`
- `CutPeek/PartialMain.lean`
- `CutPeek.lean`
- `AxiomsPartial.lean`
- `AXIOMS-partial.txt`
- `BUILD-partial.log` (lists only the three modules rebuilt on the final run; the rest was cached)
- `PROGRESS-partial.md`
