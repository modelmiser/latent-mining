<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 6 (one feedback bit)

Repository folder under review: `06-one-feedback-bit/` (present in the working tree; it may not be committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. Definitions are in `06-one-feedback-bit/README.md` ("The problem") and `chain/3-question.txt`.

Claims made in `06-one-feedback-bit/README.md`:

- C1. F(6,2) ≥ 13 by the 13 strategies in `solver-a/wit_6_2.txt` (also printed in the README), and A(5) = 6, so F(6) > 2·A(5) and the headline answer is NO.
- C2. n = 6 is the least failure: F(n,t) ≤ 2·A(n−1) for n = 3, 4, 5 and all t.
- C3. The lemmas: F(n,1) = 2·A(n−1); F(n,0) = F(n,n) = F(n,n−1) = A(n); F(n,t) ≥ A(n); F(n,t) ≤ 2·F(n−1,t−1); the minimum number of length-(n−1) outcomes is r(u)+r(a)−1 (`solver-a/proofs.txt` L1, P1–P5; `solver-b/proofs.txt` P1–P7).
- C4. The table of F(n,t) for n ≤ 8.
- C5. The README's status wording and its explanation of the counterexample match what the files support.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. Check C1 yourself from the definition of O(u,a,b): are the 13 outcome sets pairwise disjoint (do it by hand or by reasoning over the listed strings; say how you checked)? Is A(5) = 6, i.e. is there no set of 7 binary words of length 5 with pairwise disjoint single-deletion sets, and is that supported by a certificate in the folder (`solver-a/cert_5_0.txt` or `solver-b/cert_n5_t0.txt`) whose checking logic is sound?
- Q2. Does the formal definition (triples, outcome sets) match the prose problem (sender, one deletion, one yes/no feedback after bit t)? In particular: is disjointness of outcome sets exactly the condition for zero-error decoding; what if the deletion happens after the feedback moment; is "at most one deletion" handled; are t = 0 and t = n treated correctly?
- Q3. Is L1 (reduction to packing the length-(n−1) parts S = uD(a) ∪ D(u)b, and the dominance pruning) correct, including the case of two strategies with the same (u,a)?
- Q4. Are P4 (F(n,1) = 2A(n−1)) and P2/P5 (F(n,n−1) = A(n)) correct?
- Q5. Is the run-count lemma (P5 in solver A, P7 in solver B) correct, including "exactly when"?
- Q6. Is solver B's P6 (F(n,t) ≤ 2F(n−1,t−1)) correct? Is solver A's P6 (asymptotics) correct apart from the constant the referee flags?
- Q7. Upper-bound certificates: read one checker (`solver-a/checkcert.py` or `solver-b/certcheck.py`) and say whether a passing certificate really proves F(n,t) ≤ the stated value (weak LP duality over all strategies, exact arithmetic, floor). Does C2 follow from the certificates present?
- Q8. Does the README overstate anything? Check each Status cell against the reports (especially `reports/referee.md`), the sentence that entries without certificates rest on search, the n = 9 row, the paragraph explaining why 13 is possible (is "3 + 4 + 2 + 4" right; is the explanation of the mechanism supported or speculative?), and the prior-art paragraph against `prior-art.md`.

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
