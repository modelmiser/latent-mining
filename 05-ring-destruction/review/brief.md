<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 5 (destroying messages on a ring)

Repository folder under review: `05-ring-destruction/` (present in the working tree; it may not be committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. Definitions are in `05-ring-destruction/README.md` ("The problem") and `chain/3-question.txt`.

Claims made in `05-ring-destruction/README.md`:

- C1. F(n,1) = F(n,2) = ⌊n/2⌋ (`solver-a/proofs.txt` §3; `solver-b/proofs.txt` Theorem K2).
- C2. F(n,k) ≥ n − ⌈n/min(k,n)⌉ by the two-comb strategy (`solver-a/proofs.txt` Theorem L).
- C3. F(n,k) ≤ n − ⌈n/k⌉ for all n > k ≥ 3 (`solver-a/proofs.txt` §0 lemmas L0–L3, Theorem U and its Corollary).
- C4. Hence F(n,k) = n − ⌈n/k⌉ for n > k ≥ 3 and c(k) = 1 − 1/k.
- C5. Solver B's weaker bound F(n,k) < n − n/k + 1 (`solver-b/proofs.txt` Lemmas E, D, Φ, Theorem U).
- C6. The README's status wording and explanations match what the files support.

The heart of this review is C3: one instance wrote it, one referee instance read it, and the other solver did not find it. Spend most of your effort there.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. Model lemmas L0–L2 of `solver-a/proofs.txt`: is K_t(i) always an arc with the stated recursion for its extents; is the survival condition (*) right, including the constant D = 2t − n + 2; is the "pieces" rewriting of the recursion right; is L2 (two events hitting the same link) correct with the indices as written?
- Q2. L3: (i) the count of pairs against destructions, (ii) closure, (iii) "a pair of value y is first created in a round ≥ 2y − 1", and the consequence that for odd t at most k pairs have the top value H.
- Q3. Theorem U, the diagonal argument: is P_A exactly the union of initial segments of diagonals; are distinct positions on one diagonal forced to have distinct f-values in [1, h_c]; is inequality (1) right; is Σ δ_c = n?
- Q4. Theorem U, case t even: derive (E) yourself. Case t odd: derive (O) yourself, including the claims E ≤ k and B ≤ E.
- Q5. The Corollary: redo the algebra for t0 = n − ⌈n/k⌉ in both parities with n = kq − r, including the hypothesis D′ ≥ H, the exceptional sub-cases (k = 3, r = 2; q = 2), and the claim that the right side of (O) is smallest at B = E = k. Is the corollary proved for every n > k ≥ 3?
- Q6. Theorem L: is the strategy legal (budget, no link twice running) and does the delay computation give survival through T = n − ⌈n/m⌉ − 1 for every n and k, including k ≥ n and n not divisible by k?
- Q7. C1: are the k ≤ 2 arguments complete for both parities of n? C5: is solver B's Theorem U correct as far as it goes?
- Q8. Does the README overstate anything? Check each Status cell against the reports (especially `reports/referee.md`), the table against the logs, the sections "What the adversary does" and "Why it cannot do better" against the proofs (flag any sentence that is hand-waving presented as fact), the k ≥ n row, and the prior-art paragraph against `prior-art.md`.

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
