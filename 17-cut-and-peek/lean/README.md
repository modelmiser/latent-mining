# Lean check of the full-opening impossibility and of two partial-opening results

Lean 4.33.1 with Mathlib (commit `0df444a360`, pinned in `lake-manifest.json`).

What is proved, as statements about the definitions in `CutPeek/Spec.lean` (53 lines):

- `seven_force_eighth`: for three input bits, any row length m ≥ 1 and any encoding in which
  each card depends on at most one bit, if the seven rows other than the one at a corner `c`
  are cyclically equal to one another, the row at `c` is cyclically equal to them too.
- `no_scfo_of_odd_corner`: if three variables and fixed values of the others give a cube on
  which `f` takes, at seven corners, a value different from its value at the eighth, then `f`
  has no single-cut full-open protocol on any number m ≥ 1 of cards.
- `no_scfo_and`: none for the AND of n ≥ 3 bits. `no_scfo_eq`: none for the equality of n ≥ 4 bits.
- `five_card_trick` (`CutPeek/FiveCard.lean`): the five-card trick IS a protocol for the AND of
  two bits under the same definitions, so the definitions do not exclude everything.
- `six_card_trick` (`CutPeek/SixCard.lean`): the six-card trick is a protocol for the equality
  of three bits, so "n ≥ 4" in `no_scfo_eq` cannot be lowered.

The statements are at the end of `CutPeek/Main.lean`. `AXIOMS.txt` shows that each depends only
on Lean's three standard axioms; `SORRY.txt` (empty) is the search for `sorry`, `admit`,
`native_decide`, `axiom`, `unsafe`, `implemented_by`, `opaque`, `set_option`; `BUILD.log` is a clean build.

What has to be trusted: Lean and Mathlib; that `CutPeek/Spec.lean` says what the note means (a
row is a function from positions modulo m to two kinds of card; `rot`, `CycEq`; an encoding gives
each position either a fixed card or a card that is `base` or its opposite according to one bit;
`IsSCFO` asks that two inputs give cyclically equal rows exactly when `f` agrees on them); that
the four statements say what the note claims; that the two layouts are the tricks they are named
after; and that the build log belongs to these files. The partial-opening results of the note
(Section 4) are not formalised, except the two in the section "Partial opening" below.

Who wrote what: the specification, the four statements and the two layouts were written first,
by the organising instance; a second instance wrote the proofs and was not allowed to change
them. Two comments in `Spec.lean` were clarified afterwards (no definition changed).

Files: `Unit.lean` (complex numbers of modulus 1), `Fourier.lean` (the lemma over the complex
numbers, by the discrete Fourier transform on `ZMod m`), `Rows.lean` (from cards to ±1 and back;
hearts are +1 there, clubs in the note, which changes nothing),
`Cube.lean` and `Odd.lean` (restriction to a cube, Theorem 2 and the two corollaries).

## Partial opening (added 21 November)

`CutPeek/PartialSpec.lean` (33 lines) defines `seen R w j` (the cards of the row `w`, cut by `j`,
at the opened positions `R`), `views R w` (the multiset of these over all `m` cuts) and
`IsPartialAnd e R`: `R` is not empty, any two inputs with AND false have the same views, and no
view of the all-true input is a view of the all-false input. `CutPeek/PartialMain.lean` states

- `partial_and_card_bound`: `IsPartialAnd e R` implies `2 ^ n - 1 ≤ m` (natural-number
  subtraction; Proposition 3 of the note);
- `thirty_six_cards`: the note's 36-card layout, retyped there as two lists of digits, with
  `R` the multiples of 6, satisfies `IsPartialAnd`.

`PartialBound.lean` proves the first along the note's proof; `PartialCheck.lean` reduces the second
to finite checks that `decide` performs, using six permutations of the cuts found by a search
outside Lean (a certificate that Lean checks). `AXIOMS-partial.txt`: the three standard axioms.
`BUILD-partial.log`: a build by the organising instance after moving the project's build directory away, so that all 14 of its modules are compiled again (Mathlib's compiled files are reused); no warnings. `MutantPartial.lean` and
`MUTANT-partial.txt`: the same proof term is rejected in two tried cases: with the first digit of
`u0_36` changed from 0 to 1, and with the opened position 30 replaced by 31. (A reviewer's own
program finds that each of the 36 single-digit changes of `u0_36` gives an invalid layout.) The search for the forbidden words listed above finds nothing in the new
files either. As before, the definitions, the two statements and the retyped layout were written
by the organising instance first (it compared the digits with the note's strings by program), and
a second instance wrote the proofs; by its log it changed nothing in the two given files except to
replace `sorry` and add imports (the organising instance reread both files afterwards; the
repository has no earlier version of them to compare). The six permutations in `PartialCheck.lean` come
from a search by the proving instance whose program is not included; Lean checks them. To be trusted in addition: that `PartialSpec.lean` says what Section 4 of
the note means, and that the digits are the note's layout.

To build: `lake exe cache get` (downloads Mathlib's compiled files), then `lake build`.
`b.sh` is the wrapper used on the machine where this was produced (two cores, a cap on virtual
memory); `PROGRESS.md` is the proving agent's log.
