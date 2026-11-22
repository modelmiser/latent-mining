<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 4 (lies that cost)

Repository folder under review: `04-lies-that-cost/` (present in the working tree; it may not be committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `04-lies-that-cost/chain/3-question.txt` (formal) and `README.md` ("The game").

Claims made in `04-lies-that-cost/README.md`:

- C1. The position is the multiset of remaining honesty obligations of alive candidates (`solver-a/proofs.txt` P1; `solver-b/proofs.txt` Lemma 1).
- C2. Q_k(n) ≥ U1(n) ≥ P(n) for all k, n (`solver-a/proofs.txt` P2, P3; `solver-b/proofs.txt` Lemmas 2, 3).
- C3. Lemma C, Lemma S, Theorem 1, Theorem 2 of `open-upper/proofs.txt`: Q_k(n) ≤ P(n) for every k ≥ 2, n ≥ 2, given the finite table T1/T2.
- C4. Hence Q_k(n) = U1(n) = P(n) for every k ≥ 2, n ≥ 2.
- C5. Q_1(n) = ∞ for n ∈ {2,…,7,9} (computed).
- C6. The README's status wording and explanations match what the files support.

The heart of this review is C3, a new proof written by one instance and read by one other. Spend most of your effort there.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1: does the reduction hold for the game exactly as defined (ell_s computed from the alive set before question s; rule (ii) per candidate; questions that do not split the alive set; S containing dead elements)? Is "a candidate with remaining obligation r and j questions left with r ≥ j can never lie again" right, with no off-by-one?
- Q2. Lemma C: with k ≥ 2 and arbitrary existing obligations, is the "ask the same half up to three times" argument correct, including the stopping rule, ell ≥ 1 at each asking, and the bound 3·⌈log2 m⌉?
- Q3. Lemma S: check both parities of a, the choice of d, 0 ≤ d ≤ b, the weight difference, the bound 2·ell ≥ j − 1, statement (ii) (exact transition in the Q_k game, including the off-by-one between the obligation window and the questions left), and (iii).
- Q4. Theorem 1: do the three cases cover every nice state with weight ≤ 2^j? Check the arithmetic 3·⌈log2(2j−3)⌉ ≤ j for all j ≥ 15, the base cases, and that illegal answers (emptying the alive set) cause no problem.
- Q5. Table T1/T2: is it stated precisely, does the reduction "k ≥ 13 is the k = 13 game for j ≤ 14" hold, and do the saved outputs (`open-upper/table_out.txt`, `open-upper-referee/run1.txt`, `run2.txt`) actually cover it?
- Q6. Theorem 2: the first question from the root, the inequalities P(n) ≤ n + 1 (even n) and P(n) ≤ n (odd n ≥ 7), the weights of the two outcomes against the definition of P for both parities, and the exceptional cases n = 3, 5.
- Q7. C2: are the lower-bound proofs complete, including the odd-n refinement in P(n)?
- Q8. Does the README overstate anything? Check each Status cell against the reports, "Why repeated lies do not help" against the proofs, the k = 1 statements, and the prior-art paragraph against `prior-art.md`.

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
