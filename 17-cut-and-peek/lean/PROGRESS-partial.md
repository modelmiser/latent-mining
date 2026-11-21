# PROGRESS (experiment 17, Lean, partial opening)

Status 2026-11-21: COMPLETE. `partial_and_card_bound` and `thirty_six_cards` proved; full
`./b.sh` clean (BUILD-partial.log); axioms propext, Classical.choice, Quot.sound
(AXIOMS-partial.txt). No sorry, no set_option, no native_decide.

## Files
- `CutPeek/PartialSpec.lean` — untouched.
- `CutPeek/PartialBound.lean` (196 lines) — `sg`, `corr` (the note's C_S), `corr_eq_views`,
  `count_views`, `views_eq_of_corr`, `cnt`, `prod_row`, `sum_char`, `exists_shift`,
  `partial_and_card_bound_aux`. Follows Proposition 3 of the note.
- `CutPeek/PartialCheck.lean` (80 lines) — `bits`, `views_eq_of_perm`, `partial3_of_checks`,
  and the certificate `cert36`/`perm36` (one permutation of the 36 shifts per input with AND
  false, found by a Python search, checked by Lean).
- `CutPeek/PartialMain.lean` — statements and definitions unchanged; two imports added and the
  two `by sorry` replaced by one-line terms (three plain `decide`s in `thirty_six_cards`).
- `CutPeek.lean` — four imports added. `AxiomsPartial.lean` — new.

## Log
1. PartialBound.lean built on the first attempt (only linter warnings, then fixed).
2. PartialCheck.lean + PartialMain.lean built on the first attempt; plain `decide` sufficed
   (PartialMain takes about 9 s).

## Left to do
Nothing.
