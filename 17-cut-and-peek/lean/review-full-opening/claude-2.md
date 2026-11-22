<!-- Archived by the organiser on 2026-11-21. Report of the second Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 2: the Lean formalisation of experiment 17

I read Spec.lean, Main.lean, FiveCard.lean, the first 40-50 lines of Fourier.lean and Rows.lean, every declaration header in the other proof files, AXIOMS.txt, BUILD.log, SORRY.txt, PROGRESS.md, the lakefile, manifest and toolchain, NOTE.md, both READMEs, and the git diff. I ran no Lean.

## Verdicts

**Q1. CONFIRMED.** For m ≥ 1, a row is a function from the positions modulo m to two cards (`Row m := ZMod m → Bool`, Spec.lean:12). `rot s w p = w (p - s)` moves the card at q to q + s (Spec.lean:15). `CycEq` (Spec.lean:19) is NOTE.md:48-49. For m = 0, `ZMod 0` is ℤ, so rows become infinite sequences and `CycEq` means equality up to an integer shift. That is not the note's notion. All four theorems assume `[NeZero m]` (Main.lean:9,18,25,30), which excludes m = 0.

**Q2. CONFIRMED.** At Spec.lean:25-33, a position owned by `none` gets `base p`. A position owned by `some i` gets `xor (base p) (x i)`. This is exactly NOTE.md:55-57. The Lean definition also allows several positions per bit, a bit that controls no position, every position additional, and any base word. The note allows all of these too, and a larger class of encodings only strengthens the impossibility. The Shinagawa-Nuida definition is covered: their pair x_i, x̄_i is two positions owned by i with opposite base cards, and their permutation and inserted words only relabel positions or add `none` positions.

**Q3. CONFIRMED.** Spec.lean:37-38 is NOTE.md:59-60. When f takes both values, IsSCFO is equivalent to the words z_0, z_1 version:
- From IsSCFO, take z_b to be the row of any input with value b.
- In the other direction, use that `CycEq` is symmetric and transitive (Rows.lean:22-29): row x ~ row y ⇔ z_{f x} ~ z_{f y} ⇔ f x = f y.

When f is constant, both versions accept any encoding whose rows are all cyclically equal. That is harmless here, because every theorem's hypothesis makes f non-constant on some cube.

**Q4. CONFIRMED.** At Spec.lean:42-43, `patch` updates the positions in the order i 0, i 1, i 2, so a later update wins on a collision. Theorem 2 assumes `i` is injective (Main.lean:19), so the order is irrelevant there. `andFn` (Spec.lean:46) and `eqFn` (Spec.lean:49) are correct.

**Q5. CONFIRMED.**
- **(a)** At Main.lean:10-11, the hypothesis is that the seven rows other than the one at c are pairwise cyclically equal. The conclusion is that the row at c is cyclically equal to each of them, for any corner c and `Encoding 3 m`. This is Lemma 1 (NOTE.md:20-23).
  - The hypothesis is satisfiable, for example by an encoding where every owner is `none`.
  - The conclusion is not trivial: for m = 2, ♣♣ and ♣♥ are not cyclically equal.
- **(b)** Main.lean:20 says f(patch z i x) ≠ f(patch z i c) for every x ≠ c. Because f is Bool-valued, this means the other seven corners all take the value ¬f(c), which is "one value at exactly seven of the eight". The conclusion holds for every n and every m ≥ 1. An injective i forces n ≥ 3.
- **(c)** The ranges are 3 ≤ n (Main.lean:25) and 4 ≤ n (Main.lean:30), with any m ≥ 1 in both.

**Q6. CONFIRMED.** At FiveCard.lean:13-14, bit 0 owns positions 0 and 3, bit 1 owns positions 2 and 4, and position 1 is an additional card. The base word is ♣♣♣♥♥. Writing 1 for ♥, the row for (a, b) is a 0 b ¬a ¬b:
- (0,0) gives 00011, (0,1) gives 00110, (1,0) gives 10001. In each, the two hearts are cyclically adjacent, so all three are rotations of one word.
- (1,1) gives 10100, where the hearts are separated, so it is a different class.

So the encoding computes AND. It is den Boer's trick with the colours swapped and the positions permuted. This rules out the vacuity that matters: if `CycEq` were always true, or plain equality, or if nothing satisfied `Encoding` or `IsSCFO`, this theorem would fail.

**Q7. CONFIRMED.** I grepped every .lean file for sorry, admit, axiom, native_decide, unsafe, implemented_by, opaque, notation, instance, set_option, macro, elab and attribute. The only hit is the word "admits" inside a comment (FiveCard.lean:16). The lakefile sets only `autoImplicit = false` and `relaxedAutoImplicit = false`, which make checking stricter. AXIOMS.txt lists propext, Classical.choice and Quot.sound for all five theorems. Each theorem in Main.lean is one term that applies an `_aux` lemma, and Lean checks the declared statement against it. No proof file redefines a name from Spec.lean. The only new definitions, `sgn` and `restrict`, do not appear in the statements.

**Q8. CONFIRMED.** Fourier.lean uses Mathlib's `ZMod.dft` and `stdAddChar` on `ZMod m`. The only hypothesis on m is `[NeZero m]` (Fourier.lean:11). `core` (Fourier.lean:38-43) is the note's step from (∗) to (∗∗) over the complex numbers. Rows.lean:12 maps ♥ to +1, while the note maps ♣ to +1 (NOTE.md:73). This is a global sign flip and harmless.

**Q9. Mostly CONFIRMED; the authorship sentence is REFUTED (F1).**
- The formalised and unformalised results are listed correctly: OR, majority of five and Section 4 are marked as not formalised (NOTE.md:336-337, lean/README.md:26-27).
- The specification has 51 lines.
- Lean 4.33.1 matches lean-toolchain.
- "About 450 lines of proof" is fair: the five proof files plus Main.lean total 448 lines.

## Findings

**F1. MODERATE, confidence high. NOTE.md:338-339** says "The Lean files were written by an AI model instance". PROGRESS.md says otherwise:
- `Spec.lean` was "untouched" by the proving agent.
- The statements in Main.lean were left "unchanged; only `:= by sorry` replaced".
- FiveCard.lean was "Added by the organiser after the agent finished".

So the specification and the statements came from a different instance than the proofs. That is evidence in the formalisation's favour, and the note hides it.
Replacement: "The specification and the four statements were written by the organising AI instance before the proofs; a separate AI instance wrote the proofs without changing them; the organiser added `FiveCard.lean`. The builds were run by AI instances."

**F2. LOW, confidence high. NOTE.md:31-32.** The edit removed the hedge "if Lemma 1 is right" from a sentence that also states the OR and majority results, and those are not formalised.
Replacement: "...and none for the majority of five bits (AND and equality are machine-checked; OR and majority follow from Theorem 2 by hand)."

**F3. LOW, confidence medium. README.md:60.** "f(n) = ∞ ... Machine-checked" uses the README's own definition, which compares the multisets M(x) with R = everything. Lean proves it for `IsSCFO`. The two are equivalent (with full opening, M(x) = M(y) exactly when the rows are cyclically equal), but only by a hand argument.
Replacement: append "(for the equivalent formulation of NOTE.md Section 1)".

**F4. LOW, confidence high. lean/PROGRESS.md:4** says "SORRY.txt empty", but the regenerated file has one line. The appendix already explains this. The search in SORRY.txt (lean/README.md:18-20) also omits `opaque`, `set_option`, `notation`, `instance` and `macro`. I grepped for those and found none.
Replacement: add "(superseded: see the organiser's notes below)" after "SORRY.txt empty", and widen the search.

I found no CRITICAL defect. The definitions and the four statements say what NOTE.md Sections 1-3 claim.
