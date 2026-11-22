<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 9 (slipped comparator)

Repository folder under review: `09-slipped-comparator/`. Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. Definitions are in `09-slipped-comparator/README.md` ("The problem") and `chain/3-question.txt`.

Claims made in `09-slipped-comparator/README.md` (N = n(n-1)/2):

- C1. Inserting an adjacent comparator anywhere in a sorting word leaves it sorting (`solver-a/proofs.txt` L0, L1).
- C2. For n ≥ 3, a word is slip-proof iff it is deletion-proof (`solver-a/proofs.txt` T1).
- C3. Every deletion-proof word has at least N + n − 1 letters (`solver-a/proofs.txt` T2; independently `solver-b/proofs.txt` L5).
- C4. Every slip-proof word has at least N + n − 1 letters via up-slip-proof words and a computed base u(4) = 9 (`solver-b/proofs.txt` L4).
- C5. W_n = D_1 D_2 … D_(n−1) D_(n−1), D_k = c_k c_(k−1) … c_1, is deletion-proof (`solver-a/proofs.txt` T3; `solver-b/proofs.txt` L2).
- C6. Hence S(n) = A(n) = (n−1)(n+2)/2 for every n ≥ 3.
- C7. The README's status wording matches what the files support.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. Is L1 in `solver-a/proofs.txt` correct: the formula for the new prefix sum s_i after c_i, monotonicity, and both stated consequences (beliefs (a) and (d))?
- Q2. Is T1 a complete proof of C2 in both directions? In (=>), check the claim that on a 0-1 input a slipped comparator is idle whenever the original letter was active, for both slip directions and at the ends of the alphabet, and that every letter has at least one slip when n ≥ 3.
- Q3. Is T2 a complete proof of C3? Check: each pair exchanged at exactly one letter on the reversed input; the relabelling relation between the two runs and why it persists; existence of q_v; that the q_v are distinct and are not among the active letters.
- Q4. Is T3 a complete proof of C5 for every n, including the cases (i), (ii), (iii) and the case analysis on where the deleted letter lies? The referee reports one uncovered trivial case in (i); is that the only gap?
- Q5. Is solver B's L5 an independent, correct proof of C3? Is L4 correct given its computed base case, and is the base case actually in the logs?
- Q6. Does C6 follow from C2, C3, C5, with the hypotheses (n ≥ 3) used correctly? What happens at n = 2, and does the README say anything false about it?
- Q7. Do the README sections "Why a slip is a deletion" and "Why n − 1 extra are needed" state the arguments correctly, including the remark about upward slips only and n = 3?
- Q8. Does the README overstate anything: "three independent programs", the n = 7 pruning remark, the word counts, the prior-art paragraph compared with `prior-art.md`?

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
