<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 11 (a budget of glimpses)

Repository folder under review: `11-glimpse-budget/` (present in the working tree; it may not be committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `11-glimpse-budget/chain/3-question.txt` (formal) and `README.md` ("The game").

Claims made in `11-glimpse-budget/README.md`:

- C1. The game as written is equivalent to the belief-set game (`solver-a/proofs.txt` §0).
- C2. d(h) ≤ h − 1, e(h) ≤ ⌊h/2⌋, and both are 0 for h ≤ 2 (`solver-a/proofs.txt` §2).
- C3. Blind lemmas W_full and W_seed (`solver-a/proofs.txt` §3), and the corollary that a blind cop never wins on T_m, m ≥ 3.
- C4. d(h) ≥ h − 1 for h ≥ 3 (`solver-a/proofs.txt` §1 Projection Lemma and §4; independently `solver-b/proofs.txt` L1, L2), given the computed base case D(3,1).
- C5. e(h) ≥ ⌊h/2⌋ for h ≥ 3 (`solver-a/proofs.txt` §5), given a computed base of 8 blind states on T_3.
- C6. Hence d(h) = h − 1 and e(h) = ⌊h/2⌋ for all h ≥ 3, and the first h with d(h) > e(h) is 3.
- C7. The README's status wording and its three explanatory sections match what the files support.

The heart of this review is C3 and C5: one solver wrote them, the other solver tried and failed to prove C5, and one referee instance has read them. Spend most of your effort there.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1: is the belief update right in both games (order of glimpse, cop move, robber move; removal of the cop's vertex; what a direction answer is; which answers the adversary may give), and does "the cop can force the belief empty" equal "some deterministic strategy captures within a fixed N against a robber who knows it"?
- Q2. C2: check (b) and (c) of §2, in particular that after an exact glimpse and two steps the robber is caught or strictly below the cop's new vertex, and the count ⌊m/2⌋ including odd m and the endgame at heights 1 and 2.
- Q3. W_full: follow the joint induction on (W) and (K) for every case, including the root, vertices of height exactly 2, and the argument for (K) with the times r1 > r2. Is anything assumed about the cop's start?
- Q4. W_seed: clauses (F), (W), (K); the claim that the cop needs t ≥ depth + 1 to stand at a vertex of X; the reuse of the W_full(K) argument. Is the lemma true as stated for every ambient tree and every m ≥ 3?
- Q5. §5, claim Lam(j): the definition of Ψ_m (why a sibling subtree is required and where it is used), Lam(0) for m ≥ 3 and for m = 2, and the induction step: for each of the three positions of the cop, is the chosen answer u really in the belief (by which clause of W_seed), at distance ≥ 2, heading a subtree the cop is outside with a sibling subtree of the same height, and is the state after the cop's move again a Ψ state with m′ ≥ m − 2? Then the final Theorem: both cases of the first glimpse.
- Q6. C4: the Projection Lemma for both games, and §4's step: is "towards X" always a legal answer, why is the belief inside X non-empty, and does the contradiction with L(m−1, j−1) follow? Is solver B's L1 a correct independent proof?
- Q7. Are the computed base cases actually present and checked in the folder (`solver-a/lose_D_h3_k1.txt`, the 8 states in `solver-a/lemma_check.py` part 3 and its output)? Does the table in the README match the reports?
- Q8. Does the README overstate anything? Check each Status cell, and each sentence of "How the cop uses a glimpse" and "Why he cannot do with fewer" against the proofs: flag any sentence that states more than the proof shows or tells a different story from the proof. Check the prior-art paragraph against `prior-art.md`.

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
