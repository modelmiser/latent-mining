<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 8 (patient automaton)

Repository folder under review: `08-patient-automaton/`. Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `08-patient-automaton/README.md` ("The game") and in `chain/3-question.txt`.

Claims made in `08-patient-automaton/README.md`:

- C1. The 4-state automaton a = (1→2, 2→2, 3→1, 4→3), b = (1→4, 2→3, 3→1, 4→1) is d-winnable for every d ≥ 0 (`solver-a/proofs.txt` Theorem 1, strategy in `solver-a/witnesses.txt`).
- C2. The same automaton is not blind-winnable (`solver-a/proofs.txt` Theorem 2).
- C3. The automaton a = (1,1,2,3), b = (4,1,3,4) is patient (`solver-b/proofs.txt` Theorem 1, Lemmas A and B), with the defects listed in `reports/referee.md`.
- C4. Patient automata exist for every n ≥ 4 by padding (`solver-a/proofs.txt` Corollary; `solver-b/proofs.txt` Theorem 3).
- C5. No patient automaton has ≤ 3 states; this is stated as computed only.
- C6. The README's status wording (what is proved, computed, by how many programs, what is open) matches what the files support.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. Recompute the pair tables at the top of `solver-a/proofs.txt` from the two letter maps. Are all entries right?
- Q2. Is Theorem 1 of `solver-a/proofs.txt` a complete proof of C1? Check legality of the strategy (x_t uses only y_1..y_(t-1-d)), both cases y_1 = a and y_1 = b, the mod-3 invariant, and the claim that play ends with a singleton.
- Q3. Is Theorem 2 of `solver-a/proofs.txt` a complete proof of C2? Check that each family maps as stated, that the Lemma's induction is sound (note the Spoiler knows the whole word), and the first-round step.
- Q4. Is the padding argument (C4) correct for every n ≥ 5, for d-winnability and for blind non-winnability alike?
- Q5. Does the game solved in the proofs match the game defined in the README: who moves first, what "delay d" lets the Synchronizer see, when the image must be a singleton, uniform T? Is the reduction paragraph at the end of `solver-a/proofs.txt` (position = image plus pending letters) correct?
- Q6. Is solver B's Theorem 1 (C3) sound apart from the two defects the referee lists, and are those defects harmless as claimed?
- Q7. Does the README overstate anything? In particular: "checked by three programs", "three independent programs agree" for the n ≤ 5 table, the n = 6 column, "exactly 96", and the "Why the example works" section (is it true that b collapses the pair 34 and no other pair, and that the phase is fixed by the Spoiler's first letter?).
- Q8. Is C5 fairly described as computation, and do the logs in the folder actually show W_0 = W_inf for n = 2, 3 from more than one program?

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
