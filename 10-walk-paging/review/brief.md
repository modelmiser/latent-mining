<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 10 (paging along a walk)

Repository folder under review: `10-walk-paging/` (present in the working tree; it may not be committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `10-walk-paging/chain/3-question.txt` (formal) and `README.md` ("The game").

Claims made in `10-walk-paging/README.md` (h = ⌊k/2⌋):

- C1. Upper bound F(k,m) ≤ max(m+1, 2m+1−h): `solver-b/proofs.txt` UPPER BOUND (self-contained) and `solver-a/proofs.txt` Lemmas 1–3, Theorem U (uses Belady's theorem).
- C2. Lower bound F(k,m) ≥ max(m+1, 2m+1−h): `solver-b/proofs.txt` LOWER BOUND and `solver-a/proofs.txt` Theorem L.
- C3. Hence F(k,m) = max(m+1, 2m+1−⌊k/2⌋) for all k ≥ 2, m ≥ 1.
- C4. The pruning lemma used by the game solvers.
- C5. The README's status wording, table and the two strategy descriptions match what the files support.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. Solver B's upper bound: the strategy ALG is well defined (is a fault always at a page just outside the window? what does it do in round 1? even and odd k); invariant (1) of the pair phase and the bound ALG ≤ opt + 1 there; in (2) the count of distinct visited pages after the first run, the claim about the window after the last fault of any run, the "k + j distinct pages between t0 and t1" step, the conclusion "≥ j offline entries in (t0, t1]", disjointness of the intervals, and the final arithmetic.
- Q2. Solver B's lower bound: Stage 0 (legality: is opt of every prefix ≤ m? the offline schedule loading pairs; the online count ≥ g + 1; the case m < h); the invariant; the free step; the cycle, cases (i) and (ii), for |O| = k and |O| = k − 1 and both parities of k; that the offline service chosen after seeing Online's reply is a single consistent legal schedule for the whole history; legality of every request inside a cycle; the final count.
- Q3. Solver A's upper bound: are Lemmas 1 and 2 correct; is Belady's theorem applied only to demand paging (no prefetch) so that it applies; does Theorem U follow?
- Q4. Solver A's lower bound (Theorem L): the fill stage count ("at most j − 1 free among the first 2j ranks"), the block stage, legality, both parities, and small m.
- Q5. The pruning lemma (`solver-b/proofs.txt` last section; `solver-a/proofs.txt` Lemma D): is the simulation argument right, and are the two prunings it licenses the ones the solvers use?
- Q6. Does the game the proofs treat match the game as defined: round 1, hits change nothing, legality tested on the history including the new request, the adversary may stop, infinite play, F as the least N?
- Q7. Do the table in the README and the statement "computed exhaustively for k ≤ 14, m ≤ 6" match the logs in `verifier/` and the reports?
- Q8. Does the README overstate anything? Check each Status cell, and each sentence of "What the online cache does" and "What the adversary does" against the proofs: flag any sentence that says more than, or something different from, the proof. Check the prior-art paragraph against `prior-art.md`.

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
