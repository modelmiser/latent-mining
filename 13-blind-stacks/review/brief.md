<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 13 (sorting blind through small stacks)

Repository folder under review: `13-blind-stacks/` (present in the working tree; it may not be committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `13-blind-stacks/chain/3-question.txt` (formal) and `README.md` ("The game").

Claims made in `13-blind-stacks/README.md`:

- C1. Reduction: the sorter's information state is the flag state, and N(k,n) is the value of the stated recursion (`solver-b/proofs.txt` Lemma 0; `solver-a/proofs.txt` Lemma 1).
- C2. N(k,n) = n! exactly when n ≤ k+1 (`solver-b/proofs.txt` Theorems 1, 2; `solver-a/proofs.txt` Theorem 2).
- C3. N(2,n) = 5N(2,n−1) − 5N(2,n−2) for all n ≥ 5 with N(2,1..4) = 1, 2, 6, 21 (`solver-b/proofs.txt` Theorem 3 with `solver-b/prove_k2.py`; `solver-a/proofs.txt` Theorem 3 with `solver-a/prove.py`), conditional on C1.
- C4. The closed forms and generating function.
- C5. The README's status wording and its two explanatory sections match what the files support.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1, Lemma 0: is the set of arrival orders consistent with a history exactly as described (pinned values for departed items; the minimum not on a flagged item; everything else free)? Check: what a refused OUT says; why flags can be cleared after a successful output; whether a refused IN or SHIFT can ever carry information; whether items buried under others can be flagged; whether "N = best sum over the decision tree" is the right meaning of "largest number of orders one strategy beats"; and that the recursion is well founded. Is anything about OUT being allowed from any stack mishandled?
- Q2. C2, Theorem 1 (n ≤ k+1): is the "spread" strategy correct, including the step for n = k+1 when all k tops are refused?
- Q3. C2, Theorem 2 (n ≥ k+2): is the adaptive adversary legitimate (may it designate values late, given a deterministic sorter)? Check Step A, claim C(1), and the induction C(j−1) ⇒ C(j), including the count of items in the last j−1 stacks when the buried item is finally output, and the case where the sorter never tries to reach it.
- Q4. C3, solver B's Theorem 3: is the argument sound that a fixed policy gives W_r = A·W_(r−1); that each slack sequence obeys a monic recurrence of order ≤ 49 so that 121 consecutive zeros of u(r) force u ≡ 0 from there on; that (i)–(iii) imply slack ≥ 0 for all r; and that this proves the policy optimal for every r (induction on r and on the order inside each r)? Read `prove_k2.py` enough to say whether it checks what the proof says it checks, in exact arithmetic.
- Q5. C3, solver A's Theorem 3: state what finite computation `prove.py 2 5 8` runs and whether the induction (margin d(u) ≥ λ2·d(u−1) ≥ 0 carried forward) is valid for all u. The referee flags that the closing remark claims the recurrence for all 49 states at u = 5 without checking d(3) ≥ 0; does the theorem itself survive that?
- Q6. C4: verify the closed forms and generating function against the recurrence and initial values (do the algebra or check several terms).
- Q7. Is the table in the README consistent with the logs and reports, and is the claim "from the literal rules by the referee for k = 2 up to n = 10" supported by files in `verifier/`?
- Q8. Does the README overstate anything? Check each Status cell, and every sentence of "Why n ≤ k + 1 is the limit" and "How the recurrence is proved" against the proofs: flag any sentence that says more than, or something different from, the proof. Check the OEIS and prior-art statements against `prior-art.md`.

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
