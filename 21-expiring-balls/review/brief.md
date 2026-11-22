<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 21 (two bins on offer, the oldest ball expires)

Repository folder under review: `21-expiring-balls/` (present in the working tree; it is not committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `21-expiring-balls/chain/3-question.txt` (formal) and `README.md` ("The game").

Claims made in `21-expiring-balls/README.md`:

- C1. N(3) = 7: g(7) >= 3 by the strategy in `solver-a/adv_n7_L3.txt` (and the prose strategy in `solver-a/proofs.txt` P5 and the README), g(6) <= 2 by the safe set `solver-a/rule_n6_L3.txt`; checkers `solver-a/check.py`, `solver-b/check.py`, `verifier/wcheck.py`.
- C2. N(4) <= 12 by the explicit strategy of `solver-a/proofs.txt` P6, replayed by `solver-a/verify12.py` and `verifier/replay12.py`.
- C3. g(11) = 3, hence N(4) >= 12, by the backward computations `solver-b/back2.cpp` and `verifier/vb.cpp`.
- C4. The general bounds 2k <= N(k) <= 2^k, the no-expiry value floor(log2 n), and the any-removal bound 1 + k(k+1)/2 (`solver-a/proofs.txt` P1-P4, `solver-b/proofs.txt` P1-P4).
- C5. The README's status wording, its section "How the adversary uses expiry" and "What to distrust" match what the files support.

Questions. Verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1, the 7-bin strategy as written in the README (four steps): play it by hand against every answer of the Rule. Check that every offer is legal (the two bins are in different components at that moment), that in step 2 the Rule really is forced, that after "expire everything older" exactly the two balls at f remain and 4 bins are untouched (count the bins used: is it 7 in every branch, including the branch of step 1 where the two youngest balls are already x's?), and that step 4 does not need Expire. Does the README's strategy differ from P5 in `solver-a/proofs.txt` in any way that matters?
- Q2. C1, the other direction: read `verifier/wcheck.py` (or one of the solvers' checkers) and say whether a Rule certificate is checked correctly: contains the empty state; closed under Expire; for every state and every legal offer (all pairs of bins in different components, over all n bins, including untouched ones) some answer stays inside; no state has load 3. Is working up to relabelling of bins while keeping the age order sound?
- Q3. C2: follow P6 (stages Ia, Ib, I', II) by hand. Count bins in each branch (is 12 enough everywhere?), check each "the Rule must give the ball to ..." claim, check that the balls to be expired really are older than the ones to be kept, and check the final merge on 8 untouched bins needs no Expire. State the first step that does not follow, if any.
- Q4. C3: read `verifier/vb.cpp` or `solver-b/back2.cpp` far enough to judge the method: is a backward attractor from "two bins of load L-1 in different components" a correct way to decide whether the adversary can force load L; is "closed under adding an older ball" true of the adversary's winning set (monotonicity), so that storing minimal cores is exact; is the seed pruning bound justified? Is the README right about what the two programs share and about the absence of a certificate?
- Q5. C4: are the proofs of N(k) >= 2k (smaller-load rule: why does each of the two components have at least k vertices, and does Expire break the argument?), N(k) <= 2^k, and the any-removal bound complete?
- Q6. Does the README's explanation, including "for k = 4 the order of expiry costs the adversary exactly one bin" and "that is where k + 2^(k-1) comes from", claim anything the files do not support? Is the relation to experiment 14 described correctly?
- Q7. Check each Status cell and each bullet of "What to distrust" against `reports/referee.md`, the solver reports and `prior-art.md`; flag any number, count or attribution that differs from its source.
- Q8. Is the top-level `README.md` row for experiment 21 consistent with the folder, and is the README's "The game" the same game as `chain/3-question.txt`?

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
