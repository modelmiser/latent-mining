<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the instance that proved the four full-opening theorems in Lean, verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

All four theorems are proved: `seven_force_eighth`, `no_scfo_of_odd_corner`, `no_scfo_and`, `no_scfo_eq`. The final `BUILD.log` shows `Build completed successfully (3273 jobs)` with no warnings and no `sorry`, and `SORRY.txt` is empty. `Spec.lean` is untouched, and the four statements are unchanged — only `:= by sorry` became a one-line term, plus one import in `Main.lean`.

**AXIOMS.txt**
```
'CutPeek.seven_force_eighth' depends on axioms: [propext, Classical.choice, Quot.sound]
'CutPeek.no_scfo_of_odd_corner' depends on axioms: [propext, Classical.choice, Quot.sound]
'CutPeek.no_scfo_and' depends on axioms: [propext, Classical.choice, Quot.sound]
'CutPeek.no_scfo_eq' depends on axioms: [propext, Classical.choice, Quot.sound]
```

**Files and line counts** (under `(staging)/lean/`)

| File | Lines | Content |
|---|---|---|
| `CutPeek/Spec.lean` | 51 | unchanged |
| `CutPeek/Unit.lean` | 97 | unit-modulus algebra: `unit_pair`, `unit_pair_one`, `unit_triple` |
| `CutPeek/Fourier.lean` | 76 | `dft_shift`, `pair_freq`, `core` (Lemma 1 over ℂ) |
| `CutPeek/Rows.lean` | 104 | Bool→±1 transport, `CycEq` symmetry and transitivity, `row_identities`, `seven_force_eighth_aux` |
| `CutPeek/Cube.lean` | 61 | `patch` lemmas, `restrict`, `restrict_row` |
| `CutPeek/Odd.lean` | 76 | Theorem 2 and the AND and equality corollaries |
| `CutPeek/Main.lean` | 34 | the four theorems, each applying its `_aux` lemma |
| `CutPeek.lean` | 2 | |
| `Axioms.lean` | 6 | |

Also present: `PROGRESS.md`, `BUILD.log`, `AXIOMS.txt`, `SORRY.txt`, and `../REPORT.md` (411 words).

**Hand proof: repairs or extra hypotheses**

None were needed, and no statement turned out false. I took two shorter routes than the hand proof:

- **27-case analysis replaced.** If some u_i = 1, the identity (u1+u2+u3−2)·u1u2u3 = v12·v13·v23 follows from the three pair equations by linear arithmetic alone. If no u_i = 1, the key fact forces u1 = −u2, u1 = −u3, u2 = −u3, so u2 = 0, a contradiction. The unit-modulus lemma is only used in that second branch.
- **No explicit a_i.** The three pair equations and the triple formula are proved pointwise by cases on owner and cards (`row_identities`). The odd corner is written `![c0,c1,c2]` and the base corner `![!c0,!c1,!c2]`.

**Not finished:** nothing.

**Build flake under `b.sh`**

Files importing the Fourier stack fail intermittently (about 7 of the ~22 builds of `Rows.lean`/`Cube.lean`) with `lean::exception: failed to create thread` (exit 134). A failing run takes about 11 s; a good one about 2 s. Identical content passes on rerun; I saw this on both `Rows.lean` and `Cube.lean`. My guess, unconfirmed, is the `ulimit -v 16000000` cap against Mathlib's mapped oleans. I did not modify `b.sh`; the workaround is to rerun. The final `BUILD.log` is a single passing run that rebuilt Rows, Cube, Odd, Main and CutPeek.

Separately, `grind` on the 27-case goal crashed Lean with a deep stack trace, so it is not used.

**Rules**

Builds went only through `./b.sh` plus the one permitted Axioms command, one at a time, with no network access. Nothing was written outside the staging directory and no files were deleted. There are no `unsafe` or `implemented_by` uses.
