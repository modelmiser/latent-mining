<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 24 (two registers, doubling and adding)

Repository folder under review: `24-two-register-powers/` (present in the working tree; it is not committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The objects are defined in `24-two-register-powers/chain/3-question.txt` (formal) and `README.md` ("The question").

Claims made in `24-two-register-powers/README.md`:

- C1. N(t) <= 2 * 2^(0.956 t) for all large t, hence liminf D(m)/log2 m >= 1/0.956 (`solver-b/proofs.txt` P2).
- C2. N(t) <= 2^(0.9631 t + o(t)) by a different argument (`solver-a/proofs.txt` P4-P7).
- C3. The easy bounds, the mod-m/integer equivalence, and "popcount <= 2 implies G(n) = ceil(log2 n)".
- C4. The computed tables N(0..22) and D(m), m <= 4100.
- C5. The README's status wording, its explanatory section and "What to distrust" match what the files support.

Questions. Verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1, the lemma of P2: re-derive each of the cases A, B (r <= 1/2 and r > 1/2), C, D for Phi = log2 M + (1/4) log2 max(r, 1/4) on unordered states x <= y. Check in particular what the new ratio r' and the new maximum M' are in each case, the claim that a move from {x <= y} is always one of the four types (including x = y), and the bounds 1 and 3/4. Is inequality (*) (log2 M <= j - k/4 + 1/2) a correct consequence, including the start state?
- Q2. C1, the counting: is "if n > X and G(n) <= t then k < 4(j - log2 X) + 2" right; is the bound on the number of type-words with at most K non-A letters right (binomial sum, the entropy estimate, the factor 3^K, K/t < 1/2); does a type-word determine the state (is it legitimate to count states by type-words when types are defined relative to the current order of the registers); is "each word gives at most 2 integers" right; does H(0.176) + 0.176 log2 3 = 0.9502 hold numerically; and does D(m) >= (log2 m - 1)/0.956 follow for large m via m <= N(D(m))? Why is m <= N(D(m)) true?
- Q3. C2: read P4-P7 of `solver-a/proofs.txt`. Is P4 (a word with k doublings and j additions gives values <= 2^k F_(j+2)) proved; is P5's class count justified (do A and B commute in the sense used, and does the state depend only on the claimed data); is the max-min optimisation giving gamma = 0.963096 set up correctly?
- Q4. C3: are the easy bounds and the equivalence of the mod-m and integer formulations (including residue 0) proved? Is the README right that with unlimited memory the corresponding ratio is 1, as a statement about ordinary addition chains (judge the logic: ordinary chains reach every n in log2 n + o(log n) steps)?
- Q5. C4: is the README's account of who computed what consistent with `reports/referee.md` and the solver reports? Spot-check small values by hand: N(0..5) = 1, 2, 4, 7, 12, 20 and D(2..8) = 1, 2, 2, 3, 3, 4, 4.
- Q6. Does the README's section "Why two registers cost a constant factor" say anything more than, or different from, P2? Check every sentence, including "the only step that gains a full bit ... carries no choice" and the description of solver A's proof.
- Q7. Check each Status cell and each bullet of "What to distrust" against `reports/referee.md`, the solver reports and `prior-art.md`; flag any number, count or attribution that differs from its source. Does the README claim more novelty than `prior-art.md` supports?
- Q8. Is the top-level `README.md` row for experiment 24 consistent with the folder?

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
