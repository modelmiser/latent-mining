<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 25 (chips arrive every round, one firing per round)

Repository folder under review: `25-throttled-chips/` (present in the working tree; it is not committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `25-throttled-chips/chain/3-question.txt` (formal) and `README.md` ("The game").

Claims made in `25-throttled-chips/README.md`:

- C1. C(h) <= 5h - 6 for h >= 2 (`solver-b/proofs.txt` P3; `solver-a/proofs.txt` Theorem 1 gives 5h - 5).
- C2. Every list of length m loses whenever m/5 + 2 sqrt(m) + 13.2 <= h - 1 (`solver-a/proofs.txt` Theorem 2). This has a single author and carries the headline (the limit 5): spend the most effort here.
- C3. G(h) >= 2h - 1 for h >= 3 (`solver-b/proofs.txt` P2).
- C4. G(4) = 10 and G(5) = 15 over ALL lists, via Lemma Z (and Lemma C) of `solver-b/proofs.txt` and the searches.
- C5. The README's status wording, its three explanatory sections and "What to distrust" match what the files support.

Questions. Verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1: check that an add at 0 leaves Q unchanged and a firing raises it by exactly 2; that "at most m - 1 firings after the m-th add" is right; the inequality x^2 >= 9 - w_x with w = 9, 8, 8, 5, 5; and the arithmetic to m <= 5(h-1) - 1. Then check solver B's sharper P3. Does the bound concern heights after the add (step 1), as the win condition allows?
- Q2. C2, set-up: check identity (*) eta_x = k_x - 2F_x + F_(x-1) + F_(x+1); the expansion giving eta*_x = the mean of k over x-2..x+2; identity (**); fact (a) (lags never negative: note that the firer fires only when the lag is at least 1 and T never decreases); fact (c) l_x <= T_x <= 3 eta*_x.
- Q3. C2, the key steps: fact (d): at a vertex x of maximal lag M, is 2 l_x - l_(x-1) - l_(x+1) >= 0, hence eta_x >= eta*_x >= M/3, hence x ready when M >= 6, and is the vertex fired then one of lag exactly M? (Consider ties, and whether "ready and lag >= 1, largest lag" can pick a vertex of smaller lag when the max-lag vertex is not ready: can that happen when M >= 6?) Fact (e): how much can one add raise the potential Theta = sum max(l_x - 6, 0)^2 (the add changes T at three vertices by 3/5, 1/5, 1/5); is the bound 2 max(M - 6, 0) with M the maximum AFTER the add right, by convexity; is the decrease from firing a lag-M vertex computed correctly in both ranges; is the net change per round at most 1 in every case, including rounds where the firer passes? Fact (f): do the lag bounds after the add and after the firing give the stated height bound at both steps of every round?
- Q4. C2, consequences: does "every list of length m loses" follow (the strategy uses no knowledge of the future; is every firing legal; at most one per round)? Does G(h) >= 5h - 10 sqrt(5h) - 72 follow for large h from the displayed condition (do the algebra), and hence the limit 5 together with C1?
- Q5. Try to break C2: describe a list designed to make some lag large (for example alternating far-apart sites, bursts at one site after spreading, or sites two apart so that targets interact), and say what the strategy does. If you can compute, simulate a short adversarial list by hand or with throwaway code.
- Q6. C3 and C4: is P2 a proof? Read Lemma Z: is it true that restricting the firer can only help the list, that a shortest list winning against the restricted firer must be "zone-connected" in the lemma's sense, and that therefore "no zone-connected list of length <= M wins against the restricted firer" implies G(h) > M? Is the README right that G(4) = 10 and G(5) = 15 hold over all lists, and about what is and is not known for G(6)?
- Q7. Does the README's explanatory text say anything more than, or different from, the proofs? Check every sentence of "Why 5h is enough" and "Why no list is much shorter", including "the cheapest arrangement ...", "that is where the 5 comes from", "its height is at least a third of its lag", and the remark about the square-root term.
- Q8. Check each Status cell and each bullet of "What to distrust" against `reports/referee.md`, the solver reports and `prior-art.md`; flag any number, count or attribution that differs from its source. Is the top-level `README.md` row for experiment 25 consistent with the folder, and is "The game" the same game as `chain/3-question.txt`?

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
