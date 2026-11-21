# PROGRESS (experiment 17, Lean)

Status 2026-11-21: COMPLETE. All four target theorems proved; full build clean (BUILD.log),
no sorry/admit/native_decide/axiom (SORRY.txt empty), axioms = propext, Classical.choice,
Quot.sound (AXIOMS.txt).

## Files (dependency order)
- `CutPeek/Spec.lean` — untouched.
- `CutPeek/Unit.lean` — `unit_pair` (unit complex a+b=c+d), `unit_pair_one`, `unit_triple`.
- `CutPeek/Fourier.lean` — `norm_stdAddChar`, `dft_shift`, `pair_freq`, `core` (Lemma 1 over C).
- `CutPeek/Rows.lean` — `sgn`, `sgn_injective`, `CycEq.symm'`/`trans'`, `row_identities`,
  `seven_aux`, `seven_force_eighth_aux`.
- `CutPeek/Cube.lean` — `patch_i`, `patch_other`, `patch_const`, `restrict`, `restrict_row`.
- `CutPeek/Odd.lean` — `no_scfo_of_odd_corner_aux`, `no_scfo_and_aux`, `no_scfo_eq_aux`.
- `CutPeek/Main.lean` — the four theorems, each a one-line term applying its `_aux`
  (statements unchanged; only `:= by sorry` replaced and `import CutPeek.Odd` added).

## Left to do
Nothing.

## Deviations / notes
- `unit_triple` does not run the 27-case analysis: if some u_i = 1 the identity follows from
  the three pair equations by linear arithmetic alone; if none is 1, `unit_pair_one` forces
  u1=-u2, u1=-u3, u2=-u3, so u2=0, contradiction. (`grind` on the 27 cases crashed Lean.)
- BUILD FLAKE, not caused by the proofs: under `b.sh` any file importing the Fourier stack
  fails about 1 time in 6 with `lean::exception: failed to create thread` (exit 134) after
  ~11 s, where a good build takes ~2 s. Same file content passes on rerun (seen on Rows.lean
  and Cube.lean with only a trailing comment changed). Presumably the `ulimit -v 16000000`
  cap against Mathlib's mapped oleans. Workaround: rerun `./b.sh`. b.sh was not modified.

## Added by the organiser after the agent finished (2026-11-21)
- The build flake was the virtual-memory cap: with `ulimit -v 16000000` clean builds failed two times
  out of three with "failed to create thread"; with `ulimit -v 40000000` three clean builds in a row
  passed (about 13 s each, 3.6 GB resident). `b.sh` now uses the larger cap.
- `CutPeek/FiveCard.lean` added: the five-card trick satisfies `IsSCFO` for two-bit AND (by `decide`),
  so the specification is not vacuous. `CutPeek/SixCard.lean` added likewise for three-bit equality.
- After the review panel: two comments in `CutPeek/Spec.lean` clarified (m = 0; `patch` assumes an
  injective `i`); no definition changed. A comment in `FiveCard.lean` reworded so that the search in
  `SORRY.txt` (now also covering `opaque` and `set_option`) is empty. `BUILD.log`, `AXIOMS.txt` and
  `SORRY.txt` were regenerated from a clean build of the final files.
