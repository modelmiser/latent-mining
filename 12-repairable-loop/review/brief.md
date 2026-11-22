<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 12 (the repairable loop)

Repository folder under review: `12-repairable-loop/` (present in the working tree; it may not be committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `12-repairable-loop/chain/3-question.txt` (formal) and `README.md` ("The game").

Claims made in `12-repairable-loop/README.md`:

- C1. 2^n + 2 ≤ M(n) ≤ 2^(n+1) for n ≥ 3 (`solver-a/proofs.txt` P0–P5; `solver-b/proofs.txt` Facts A, B, C, Theorems 1, 2).
- C2. Structure: the reversibility lemma (only the loss of a length-1 run cannot be undone) and solver B's Lemma 5 (`solver-b/proofs.txt` Lemmas 4, 5).
- C3. Theorem 1 of `open-growth/proofs.txt`: for every n ≥ 7 no loop of length 2^n + 2 is repairable.
- C4. Theorem 2 of `open-growth/proofs.txt`: inequality (**) for every repairable loop, and M(n) ≥ 2^n + (2^(n−3) − 19n²)/(19n); hence M(n) − 2^n is unbounded.
- C5. Computed: M(3) = 10, M(4) = 18, M(5) = 34; no repairable loop of length 66 for n = 6; a repairable loop of length 76 for n = 6.
- C6. The README's status wording and its explanatory sections match what the files support.

The heart of this review is C3 and C4: written by one instance, read by one other (`reports/open-growth-referee.md`). Theorem 2 is vacuous below n = 15, so no computation can confirm it. Spend most of your effort there.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. Counting tools C1, C2, C3 of `open-growth/proofs.txt`: is the occurrence count of k-words in a universal loop right (including the "surplus" refinement); are the edit formulas for loss and doubling exactly right for k ≥ 2, including wrapping windows; is the run-count lemma right?
- Q2. Theorem 1, Steps 0–2: do the adversary moves (E1), (E2), (E3) exist in every loop one edit away from universal, for n ≥ 7; are S(w) = σ + 2 and ρ(w) = T + 2 forced? Check each use of solver B's Facts A, B, C against their statements.
- Q3. Theorem 1, Steps 3–6: the excess vector on 3-words; the claims that t = 0 and z = 0; each "the mechanic's reply is forced" (enumerate the options yourself for at least one of them); the conclusion that w is a de Bruijn cycle plus an alternating pair.
- Q4. Theorem 1, Step 7 and consequences (a), (b) of Step 6: the 0-run lengths, why the isolated 1 inside 0001000 exists in w and is untouched, and the final contradiction in all three cases (Z neither A nor B; Z = A; Z = B). Is the theorem proved for every n ≥ 7, and for non-universal w?
- Q5. Theorem 2, G1–G3: is G2 right for non-universal w; is the key lemma G3 right as stated (definition of k-bad, the junction windows, the inequality), and does its proof handle repeated words among the windows through s?
- Q6. Theorem 2, G4 and the derivation of (**): the count of k-bad singletons (the equations for word(J_p) = word(W_u), the reduction to d ≥ 2, the factor 2^(max(m,d)−1), and the term for loops with repeated n-words); the lower bound on the number of singletons; the assembly of (**) term by term. The second reader supplied one missing sentence in G4; is its repair right, and is anything else missing?
- Q7. The closed bound: derive M(n) ≥ 2^n + (2^(n−3) − 19n²)/(19n) from (**) with k = n − 4 yourself, and recompute at least one of the quoted numbers (e ≥ 9 at n = 15; 418 at n = 20; 272775 at n = 30). Does "unbounded" follow?
- Q8. C1, C2, C5 and the README: are the lower-bound proofs of M(n) ≥ 2^n + 2 correct; is the length-76 witness for n = 6 actually in the folder and checked; do the status cells, the numbers, and the three explanatory sections ("Why 2 extra bits work", "Why that stops at n = 6", "Unbounded excess") say only what the proofs and logs support? Flag any sentence that tells a different story from the proof. Check the prior-art paragraph against `prior-art.md`.

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
