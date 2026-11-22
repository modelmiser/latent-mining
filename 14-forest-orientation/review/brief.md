<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 14 (orienting a forest as it grows)

Repository folder under review: `14-forest-orientation/` (present in the working tree; it may not be committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `14-forest-orientation/chain/3-question.txt` (formal) and `README.md` ("The game").

Claims made in `14-forest-orientation/README.md`:

- C1. Normal form: the Algorithm loses nothing by replying with "no flip, new edge out of an unsaturated endpoint" or "flip one directed path from a saturated endpoint through saturated vertices to the first unsaturated vertex" (`solver-b/proofs.txt` Lemma 1; `solver-a/proofs.txt` P1, P2).
- C2. T(n) ≥ B(n) = max(0, ⌊n/4⌋ − 1); T monotone and superadditive; the limit L exists (`solver-a/proofs.txt` P3, P4; `solver-b/proofs.txt` Lemmas 2, 3, 5).
- C3. T(n) ≤ n − log2(n+1) (`solver-b/proofs.txt` Lemma 4).
- C4. T(30) ≥ 7 (`solver-a/proofs.txt` P6 with the W_2 certificates; `solver-b/cert_n30_k7.txt`).
- C5. `open-limit/proofs.txt`: Lemmas A and B, the Type Lemma, and Theorem 1: T(n) ≥ n/3 − (7/3)·log2(n+1) + 5/6 for every n, hence L ≥ 1/3.
- C6. The transfer step in `reports/open-limit-referee.md` §0 ("shadow orientation"): a normal-form lower-bound strategy that reads the position gives a lower bound in the full game.
- C7. `open-limit/proofs.txt` Theorems 2 and 3 (the pair game: V_pair(n) ≤ T(n); V_pair(n) ≤ (n−1)/3).
- C8. The README's status wording and explanations match what the files support.

The heart of this review is C1, C5 and C6. C5 was written by one instance and read by one other; C6 was written by that reader and read by nobody. Spend most of your effort there.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1: read solver B's Lemma 1 proof. What exactly does it prove (for a fixed Adversary sequence, or for the game value against an adaptive Adversary)? Is the invariant it uses (cost of the normal-form play plus distance between the two orientations is at most the cost of the arbitrary play) correct, round by round, including when the arbitrary play flips edges outside the merging components or flips more than a path?
- Q2. C6: is the shadow-orientation argument correct? Check that the shadow play is a legal normal-form play (the shadow reply named in the report is always available and valid), that the Adversary's named pair is legal in the real game, that the inequality survives each round, and that it therefore turns any normal-form Adversary strategy forcing c into a full-game strategy forcing c. Is this step actually needed, or does Lemma 1 as proved already cover adaptive adversaries?
- Q3. C5, Lemmas A and B: definitions of h, slots and types; Lemma A (b) (why h of the other out-neighbour is unchanged by a path flip through the first); Lemma B's three reply kinds and the cost and type claims for each; unsaturated endpoints, holes, and replies that are unavailable.
- Q4. C5, Type Lemma and inequality (*): both parities, with F(A_k) = k + 1/3, F(R_k) = k + 2/3. Then Theorem 1's main phase: P(new) ≥ P(X) + P(Y); base case for singletons; "inductive from every position".
- Q5. C5, Theorem 1's cleanup phase and constants: legality of M(i_r) on trees of different index, the telescoped inequality (**), the size bound i_m ≤ 2·log2(n+1) − 1, m ≤ i_m + 1, F(i) ≤ i/2 + 1/3, and the final arithmetic to n/3 − (7/3)·log2(n+1) + 5/6. Recompute the bound at n = 16, 64, 256 and say whether the README's remarks (positive from n = 34, above B(n) from n = 189) are right.
- Q6. C2, C3, C4: are the block strategy and superadditivity proofs sound; is Lemma 4's size argument (a vertex with h ≥ k forces ≥ 2^(k+1) − 1 vertices) and induction right; is solver A's P6 inequality T(a+b) ≥ W_j(a) + W_j(b) − j sound, and are the certificates it needs in the folder?
- Q7. C7: is the pair game a valid lower bound on T(n) (what does the Adversary's freedom to keep either pair mean in a real forest), and is Theorem 3's potential argument right? Does the README's sentence about this family's ceiling say no more than Theorem 3 proves?
- Q8. Does the README overstate anything? Check every number in it against the reports and logs (n ≤ 15 by three programs; n = 16; T(30), T(32), T(64); the range 30 … 66; the bounds on L; 17 … 30), each Status cell, the sections "The simple attack", "The better attack" and "The step the second reader added" against the proofs, and the prior-art paragraph against `prior-art.md` and `prior-art-deep.md`.

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
