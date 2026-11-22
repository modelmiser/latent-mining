<!-- Archived by the organiser on 2026-11-21. Report of the Gemini seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `17-cut-and-peek/lean/CutPeek/Spec.lean:11-18`, `17-cut-and-peek/lean/CutPeek/Main.lean:9,17,25,30`, `17-cut-and-peek/NOTE.md:48-50` — none
Q2: CONFIRMED — `17-cut-and-peek/lean/CutPeek/Spec.lean:24-33`, `17-cut-and-peek/NOTE.md:52-70` — none
Q3: CONFIRMED — `17-cut-and-peek/lean/CutPeek/Spec.lean:37-38`, `17-cut-and-peek/NOTE.md:58-68` — none
Q4: CONFIRMED — `17-cut-and-peek/lean/CutPeek/Spec.lean:40-49`, `17-cut-and-peek/lean/CutPeek/Cube.lean:11-23`, `17-cut-and-peek/lean/CutPeek/Main.lean:18` — none
Q5: CONFIRMED — `17-cut-and-peek/lean/CutPeek/Main.lean:9-32`, `17-cut-and-peek/lean/CutPeek/Odd.lean:12-73`, `17-cut-and-peek/NOTE.md:20-32` — none
Q6: CONFIRMED — `17-cut-and-peek/lean/CutPeek/FiveCard.lean:11-18`, `17-cut-and-peek/lean/CutPeek/Spec.lean:14,30-38` — none
Q7: CONFIRMED — `17-cut-and-peek/lean/CutPeek/Main.lean:9-32`, `17-cut-and-peek/lean/AXIOMS.txt:1-5`, `17-cut-and-peek/lean/SORRY.txt:1` — none
Q8: CONFIRMED — `17-cut-and-peek/lean/CutPeek/Fourier.lean:1-44`, `17-cut-and-peek/lean/CutPeek/Rows.lean:1-44`, `17-cut-and-peek/NOTE.md:72-88` — none
Q9: CONFIRMED — `17-cut-and-peek/NOTE.md:34-39,320-339`, `17-cut-and-peek/lean/README.md:1-27`, `17-cut-and-peek/README.md:60-63,183-185`, `README.md:48` — none

ROUNDS MODE:
F: LOW — `17-cut-and-peek/lean/CutPeek/Spec.lean:40` — `patch` docstring omits the condition that `i` must be injective to prevent index shadowing — confidence: high — Document in docstring that `i` is assumed injective.
F: LOW — `17-cut-and-peek/lean/PROGRESS.md:4` — States `(SORRY.txt empty)` when `SORRY.txt` contains one false-positive comment line matching the substring "admits" from `FiveCard.lean:16` — confidence: high — Note on line 4 that `SORRY.txt` contains only the "admits" docstring line.

### Important items not asked about (most severe first)

1. **Host-specific build wrapper:** `17-cut-and-peek/lean/b.sh` hardcodes `taskset -c 4,5` and `nice -n 15`, which fails on non-Linux OSes or environments with fewer than 6 cores, although `lean/README.md:32` documents `b.sh` as a machine-specific wrapper rather than the primary build route (`lake build`).
2. **Missing `Equivalence` instance for `CycEq`:** `CycEq` is defined as a plain relation `Prop` rather than registered with Mathlib's `Equivalence` or `Trans` typeclasses. Symmetry and transitivity are proved as explicit lemmas (`CycEq.symm'`, `CycEq.trans'`) in `17-cut-and-peek/lean/CutPeek/Rows.lean:21-29` and applied manually.
3. **Fixed subcube dimension:** `patch` and `restrict` in `17-cut-and-peek/lean/CutPeek/Cube.lean` are specialized to 3-dimensional subcubes (`Fin 3 → Fin n`) rather than arbitrary dimension `k`, matching the specific needs of Lemma 1 and Theorem 2 without generalizing.
