<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 16 (a counter that erases what it reads)

Repository folder under review: `16-erasing-counter/` (present in the working tree; it is not committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The object is defined in `16-erasing-counter/chain/3-question.txt` (formal) and `README.md` ("The rule").

Claims made in `16-erasing-counter/README.md`:

- C1. D(r) >= W_r for every r >= 1, W_1 = 2, W_r = W_(r-1)(W_(r-1)+1) (`solver-a/proofs.txt` Theorem 1; `solver-b/proofs.txt` Theorem 1).
- C2. 2^(r-1) <= log2 D(r) <= 2^r - 1.
- C3. D(r) <= 2^d - 1 for r >= 2 (`solver-a/proofs.txt` Remark 3) and Lemma 2 of the same file.
- C4. D(1) = 2, D(2) = 6, D(3) = 42, D(4) = 1806 by exhaustive search (the README says how many programs and of what kind for each).
- C5. The README's status wording, its section "Why the cycle can be so long", and its "What to distrust" section match what the files support.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1: check the induction step by step. Is the tree legal (depth, no index twice on a path, the changed leaf writes an index on its own path, 2^r - 1 positions)? Are the three transition rules for (a, x, y) right under the erase-then-write semantics (in particular: when the changed leaf is reached, the X-part still becomes g(x); when a = 1 the root bit is erased and the Y-copy never writes it)? Are the W^2 + W states distinct and is the return time exactly W(W+1), not a divisor? Is the invariant "exactly one state of the cycle uses a leaf with empty R" really carried to the next level? Is the base case sound (solver A starts at r = 0, solver B at r = 1)? If you can, simulate `solver-a/sylvester_r3.txt` or write the r = 3 tree out yourself and count.
- Q2. C2: does W_r >= 2^(2^(r-1)) follow, and is the upper bound just state counting?
- Q3. C3: read Remark 3 and Lemma 2 of `solver-a/proofs.txt` line by line. The referee says Remark 3 is complete but terse and Lemma 2(c) has one wrong detail. Is Remark 3 actually a proof? State the first step that does not follow, if any.
- Q4. C4: read enough of `verifier/lazy.cpp` and `solver-b/lazy.cpp` to say whether the trajectory search is exhaustive: is every tree with a cycle of length N found from some start state, are the symmetry reductions (naming positions in order of first use; start state normalised) safe, and could a pruning error make the reported maximum too LARGE or only too small? Is the README right that the r = 4 value rests on one design of search?
- Q5. Does the README's explanation "Why the cycle can be so long" say anything more than, or different from, the proof in `solver-a/proofs.txt`? Check every sentence, including "W + 1 steps per round, and W rounds" and "uses every one of the 2^r - 1 positions once" and "about 2^(0.676·2^r)".
- Q6. Check each Status cell of the Results table and each bullet of "What to distrust" against `reports/referee.md`, `reports/solver-a.md`, `reports/solver-b.md` and `prior-art.md`. Flag any number, count or attribution that differs from its source (for example which instance computed what, how many programs, what was exhaustive).
- Q7. Is the top-level `README.md` row for experiment 16 consistent with the folder?
- Q8. Is the definition in the README's "The rule" section the same object as `chain/3-question.txt`?

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
