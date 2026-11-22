<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1: review of `17-cut-and-peek/lean/`

Read only; nothing run in the repository. Paths are relative to `17-cut-and-peek/`. No CRITICAL finding: I found no place where a Lean statement says something other than the note.

## Verdicts

**Q1. CONFIRMED.** `Spec.lean:12` `Row m := ZMod m → Bool`; for m ≥ 1 `ZMod m` is `Fin m`, the positions mod m. `Spec.lean:15` `rot s w p = w (p - s)`: the card at q goes to q + s, a rotation. `Spec.lean:19` `CycEq v w := ∃ s, v = rot s w` is NOTE.md:48-49. For m = 0, `ZMod 0 = ℤ`: rows are two-way infinite and `CycEq` is translation; meaningless for cards. All four theorems carry `[NeZero m]` (`Main.lean:9,18,25,30`) and the non-vacuity check has m = 5, so m = 0 is excluded everywhere.

**Q2. CONFIRMED.** `Spec.lean:25-33`: an owned position shows `xor (base p) (x i)`, which differs for the two values of the bit; an unowned one shows `base p`. That is NOTE.md:53-57 exactly, and the correspondence is one-to-one (owner and base are recoverable from the rows). Allowed by both: several positions per bit, a bit owning no position, all positions additional, arbitrary base. Nothing extra allowed, nothing excluded. The paraphrase of Shinagawa and Nuida (NOTE.md:62-66) is the special case of two owned positions per bit with opposite bases; I could check only the paraphrase, not the paper.

**Q3. CONFIRMED.** `Spec.lean:37-38` is NOTE.md:59-60 word for word. Equivalence with the z0, z1 form: forwards, take z_b = row of any input with f = b; backwards, use symmetry and transitivity of `CycEq` (`Rows.lean:22-29`). For constant f, `IsSCFO` says all rows are cyclically equal (satisfiable, for example with no owned position); the z-form agrees for m ≥ 1 since all-clubs and all-hearts are not cyclically equal. No theorem hypothesis holds for constant f.

**Q4. CONFIRMED.** `Spec.lean:42-43`: updates at `i 0`, then `i 1`, then `i 2`, so on a collision the latest wins; the comment is true only for injective `i`, which is the only case used (`Main.lean:19`, `Cube.lean:11`). `andFn` (`:46`) is AND (true for n = 0); `eqFn` (`:49`) is "all bits equal".

**Q5. CONFIRMED.** (a) `Main.lean:10-11`: hypothesis and conclusion are as described, for any corner; this is Lemma 1 (NOTE.md:20-23). The hypothesis is satisfiable (any encoding in which one bit owns no position), and the conclusion is not automatic in general. See finding 4. (b) `Main.lean:20`: f is Bool-valued, so "differs from f at c on the other seven" is "one value at exactly seven of eight". Conclusion `¬ ∃ e : Encoding n m, IsSCFO e f`, every m ≥ 1, every n (injectivity of `i` forces n ≥ 3). (c) `3 ≤ n`, `4 ≤ n`, `[NeZero m]`: as claimed. `Odd.lean:34-35,55-56` instantiate the cube honestly (all-true corner; all-false corner with bit 3 fixed false).

**Q6. CONFIRMED.** `FiveCard.lean:12-14`, row = [x0, ♣, x1, ¬x0, ¬x1] with true = ♥:
- (0,0) ♣♣♣♥♥
- (1,0) ♥♣♣♣♥ = (0,0) rotated by 1
- (0,1) ♣♣♥♥♣ = (0,0) rotated by 4
- (1,1) ♥♣♥♣♣: hearts not adjacent, not a rotation of the others.

So it is a valid protocol for two-bit AND. It is den Boer's layout (ā, one card, b) with colours exchanged and positions multiplied by 3 mod 5, both of which preserve cyclic equality. It shows `IsSCFO` is satisfiable by a non-constant function with shared ownership and an additional card. It does not exercise three-bit encodings; the six-card trick for `eqFn 3` would, and would also show that 4 is sharp.

**Q7. CONFIRMED by reading; the logs taken on trust.** My search of all `.lean` files for `sorry|admit|axiom|native_decide|unsafe|implemented_by|opaque|notation|instance|set_option|macro|attribute|local|syntax|elab` finds only `#print axioms` lines and comment words. Only `open ZMod` (`Fourier.lean:9`) and `open ComplexConjugate` (`Unit.lean:8`), neither in Spec or Main. `AXIOMS.txt:1-5` lists the three standard axioms for all five. Each Main theorem is a one-line application of an `_aux` with the identical statement (`Rows.lean:94-96`, `Odd.lean:11-14,32-33,52-53`). I cannot verify that `BUILD.log` and `AXIOMS.txt` came from these sources.

**Q8. CONFIRMED.** `Fourier.lean:4,11,17-18,38-43`: Mathlib's transform on `ZMod m`, the shift rule, the core for arbitrary complex W, the shift `s12+s13+s23−s1−s2−s3` of NOTE.md Section 2. Only extra hypothesis: `[NeZero m]`. `Rows.lean:12` maps ♥ to +1 and ♣ to −1, the opposite of NOTE.md:73; immaterial.

**Q9. CONFIRMED with LOW defects.** Formalised and not formalised are stated correctly (NOTE.md:320-337, `lean/README.md:26-27`). `Spec.lean` is 51 lines. Toolchain `v4.33.1`. Mathlib `0df444a360` matches `lake-manifest.json:8`. README.md:60 ("f(n) = ∞ … machine-checked") is supported: with every card revealed, the README's validity (README.md:40-42) is equivalent to `IsSCFO e (andFn n)`, a translation done by hand.

## Findings

1. **LOW, confidence medium.** NOTE.md:338-339 "The Lean files were written by an AI model instance". `lean/PROGRESS.md:8,15-16,35` shows the prover received `Spec.lean` and the statements already written, and that the organiser added `FiveCard.lean`. Replacement: "The specification, the four statements and the five-card check were written by the organising instance; the proofs by a second instance, which was not allowed to change the statements; builds were run by AI model instances."

2. **LOW, confidence high.** NOTE.md:334-336 omits what `lean/README.md:22` includes. Replacement: "What still has to be trusted is Lean's kernel, the Mathlib definitions the specification uses (`ZMod`, `Function.update`), that the build log belongs to these files, and that those 51 lines and the four statements say what this note means."

3. **LOW, confidence high.** NOTE.md:322 "about 450 lines of proof": the five proof files total 414 lines (448 with `Main.lean`). Replacement: "about 410 lines of proof".

4. **LOW, confidence high (pre-existing text, bears on Q5a).** NOTE.md:340-342 "no encoding at all in which seven rows … are rotations of one word" contradicts Section 1, which allows a bit to own no card; such encodings satisfy the hypothesis trivially. Replacement: "no encoding in which each of the three bits controls a card (state the search's actual restriction)". Consequence worth stating: the only known witnesses of Lemma 1's hypothesis are degenerate, so its content is carried by Theorem 2.

5. **LOW, confidence high.** `lean/PROGRESS.md:4` "SORRY.txt empty"; it has one line (`lean/SORRY.txt:1`). Replacement: "SORRY.txt has one line, a comment containing 'admits'".

6. **LOW, confidence high.** `lean/CutPeek/FiveCard.lean:2-5`. Replacement: add "This is den Boer's layout with the colours exchanged and positions multiplied by 3 modulo 5; rows: 00 ♣♣♣♥♥, 10 ♥♣♣♣♥, 01 ♣♣♥♥♣, 11 ♥♣♥♣♣."

7. **LOW, confidence high.** `lean/CutPeek/Spec.lean:40-41`: add "(for injective `i`; on a collision the later update wins)".
