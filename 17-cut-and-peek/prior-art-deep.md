# Deep prior-art read: cut-and-peek AND

P1 = arXiv 2510.17308v2, P2 = 2507.04916v1, P3 = 2509.17595v3, P4 = 2507.03323v1, all read from the PDF text.

## Q1. Model match (confidence: high)

- **P1, Def. 3.1** is our model with a general shuffle: "2n sequences of suits (L_i^b) with L_i^b ∈ {♡,♣}^{ℓ_i}, a probability distribution D over S_ℓ, and an index set R ⊆ [ℓ]". Arbitrary words per player (footnote 1: "any number of cards in the sequence representing x_i may depend on x_i"); positions where L_i^0 = L_i^1 act as helpers of either colour. "If R = [ℓ] ... full-open. If R is a strict subset ... static-open."
  - Correctness: "Pr[Output(st_x) = f(x)] = 1". Privacy: "for any pair of inputs with f(x) = f(x′), the distribution of st_x is identical". For AND with a uniform cut these are exactly our two conditions.
  - Our model = Def. 3.1 with D fixed to "known rearrangement, then one random cut" and f = AND.
- **P2, §5.2**: one commitment (x_i, x̄_i) per input, a known permutation π ∈ S_2n, inserted words "u_1,…,u_2n ∈ {0,1}*" (helpers of both colours), random cut, "Open all cards". Valid iff "z_0, z_1 are not cyclically equal and ... s(x) and z_f(x) are cyclically equal". This is our model with |P_i| = 2 and R = Z_m, so ours is strictly more general.
- **P3, §1.1/§2.5**: "standard (k0,k1)-SCFO" is P2's model with k0 ♣ and k1 ♥ helpers. **P4**: same notion; its XOR protocol "employs two pairs of (x, x̄)". All are special cases of ours with R = Z_m.

## Q2. T2, f(n) = ∞ for n ≥ 3 (confidence: high that it is stated open and not contradicted)

- Not proved or contradicted in any of the four, and web searches found no later paper resolving it. T2 covers every SCFO model in P2–P4.
- P2, Conclusion: "Is there a single-cut full-open protocol for the three-bit AND function? In particular, is there a permutation π ∈ S6 such that seven binary words {π(x1 x̄1 x2 x̄2 x3 x̄3) | (x1,x2,x3) ∈ {0,1}^3 \ {(1,1,1)}} are cyclically equalizable?" T2 answers the first sentence (no). It does not answer the second: the seven words could still be equalizable, with the eighth row cyclically equal too.
- P3 proves by ILP search that no (∗,0)- or (0,∗)-SCFO protocol exists for x1∧x2∧x3, and states: "the impossibility of general SCFO protocols that use any number of both ♣ and ♥ as additional cards remains an open problem." T2 settles this for AND only, not the other eight function classes.
- P1: "Surprisingly, even the three-input AND function is not known to have a single-shuffle full-open protocol." P1 then builds full-open AND_n (Cor. 5.4: "O(n²) cards and the shuffle is a composition of n pile-shifting shuffles"; 15 cards for n = 3 by my instantiation with m = 5). That is not a random cut, so there is no conflict, but T2 is a statement about the random cut only.

## Q3. r(3) < ∞ (confidence: high)

- Known. P1, Theorem 4.2: "there exists a single-shuffle static-open protocol Π for f such that the number of cards is c2^{r+1} + 2^{r+2}. The shuffle is a random cut."
- P1 gives no number for AND_3. My instantiation with the PSM it cites (Prop. 3.4, m = 5, U = Z_5^*): c = 9 bits, 100 randomness values, r = 7, so 2816 cards by the formula, or 2200 by the proof's exact count 2cR + 4R.
- Our 36-card protocol has the same shape as Lemma 4.3 (R piles interleaved, "open C cards at positions hR"), so solver A's L5 is that lemma's index calculation. It is not an instance of Theorem 4.2, whose strands each hold a 4-card delimiter plus at least 6 message cards; ours have 6 cards and no delimiter.

## Q4. Lower bounds (confidence: high)

- P2, Theorem 5.2: "The number of cards in a single-cut full-open protocol for a function f is at least max(N_0(f), N_1(f))", which is 2^n − 1 for AND. The proof counts distinct cyclically equal rows, so it is full-open only. T1 gives the same number by a different proof and also covers partial opening, which these papers do not.
- Nothing like "no 3-player protocol with at most 15 cards" appears. The PSM route (STACS 2025, Cor. 5) bounds only the number of opened cards.

## Q5. Other points

- Solver B's guess r(3) = ∞ contradicts P1 Theorem 4.2.
- My inference, not in the solvers' reports: T2's lemma needs only seven of eight corners of a 3-cube. Four-bit equality with x4 = 0 is such a cube, so T2 would also give "no SCFO protocol for n-bit equality, n ≥ 4", a problem P2 lists as open. This needs its own check.
- I re-read T2's proof and found no gap.

## What is new here, if the proofs hold

- T2: no single-cut full-open AND for n ≥ 3, with any encoding and both helper colours.
- T1 for partial opening; T4, T5/L3, C3.
- r(3) ≤ 36 and E(3,m) = 0 for m ≤ 15.

## What is not new

- r(3) < ∞ and r(n) < ∞ (P1 Theorem 4.2).
- The strand mechanism L5/L5′ (P1 Lemma 4.3).
- m ≥ 2^n − 1 for R = Z_m (P2 Theorem 5.2).
- The one-colour-helper case of T2 for n = 3 (P3).
- r(2) ≤ 5 (five-card trick).

## What I read

- In full: P1 (all sections, figures, appendices), P2, P3, P4.
- In part: STACS 2025 "Card-Based Protocols Imply PSM Protocols" (introduction, Theorems 1 and 4, §5.1).
- Not read: ePrint 2022/1306 (download blocked), Pebody, Feige–Kilian–Naor (PSM parameters taken from P1's Prop. 3.4).
