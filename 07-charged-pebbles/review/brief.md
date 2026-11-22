<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 7 (charged pebbles)

Repository folder under review: `07-charged-pebbles/` (present in the working tree; it may not be committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `07-charged-pebbles/README.md` ("The game") and `chain/3-question.txt`.

Claims made in `07-charged-pebbles/README.md`:

- C1. C(p,k) ≥ (pk+2)·2^(p−2) for all p ≥ 2, k ≥ 0 (`solver-a/proofs.txt` P1; `solver-b/proofs.txt` Theorem 1).
- C2. C(2,k) = 2k+2 (`solver-a/proofs.txt` P2; `solver-b/proofs.txt` Theorem 2).
- C3. Relaxed game (q uncharged pebbles, f forgetful removals): a clean board has n ≤ (f+2)·2^(q−2) and no pebble is above (f+4)·2^(q−2) − 1 (`open-upper/proofs.txt`, Main Theorem).
- C4. Hence C(p,k) = (pk+2)·2^(p−2) for all p ≥ 2, k ≥ 0.
- C5. The README's status wording and its two explanatory sections match what the files support.

The heart of this review is C3, a new proof written by one instance and read by one other. Spend most of your effort there.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. `open-upper/proofs.txt` Part 1 (L1–L6 and the imported Lemma W with W(iii)): is each proof complete? In particular L1's Reach(r) induction and the pebble count; L2 (confluence) and the claim that red(S) is unique; L3 in both directions; L4; L5; L6 (lifting from the (q−1)-pebble game, including moves "at node z" being skipped).
- Q2. Lemma N (normal form of a deletion): is the case analysis exhaustive and each case correct? Check the step "e reducible in A'' in the (q−1)-game: contradiction" and the construction of V in the last case.
- Q3. Lemma Step: check all branches (1), (2), (3a), (3b) for general q. In (3b), verify u' > u, the existence of a reducible e ≠ s in V, and that Q − s has the claimed one-shift form with the shift bound allowed at size k−1. Does anything silently need A = empty or {a} beyond where it is stated?
- Q4. Shape Lemma and Theorem I: is the invariant J(f) inductive under deletion of ANY element, in all three cases m = 0, m = 1, m ≥ 2? Recheck every inequality (U = 2^(q−2), r = q − m). Is "J(f) implies J(f+1)" true? What about m = q (r = 0)?
- Q5. Does the Main Theorem (a), (b) follow from J(f), including f = 0, boards that are not reduced, and n ≤ 2^(q−1)? Is the passage from the charged game to the relaxed game (f ≤ pk) correct?
- Q6. Are the lower-bound proofs (C1) complete: does each solver's rotation rule guarantee that the pebble to be forgotten always has charge left, for every p and k, and is the reversible "advance by 2^(p−2) with p−1 free pebbles above a base" step justified?
- Q7. Are the two p = 2 upper-bound proofs (C2) correct?
- Q8. Does the README overstate anything? Check each Status cell against the reports, the sentence "Each forgetting buys 2^(p−2) more nodes and no more, however the forgettings are spent", the sections "How the strategy works" and "Why nothing does better", and the prior-art paragraph against `prior-art.md`.

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
