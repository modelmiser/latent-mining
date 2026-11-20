# Prior-art report: cut-and-peek AND

The setting is card-based cryptography. R = Z_m is a "single-cut full-open (SCFO) protocol"; general R is a "single-shuffle static-open protocol whose shuffle is a random cut".

## CLOSEST PRIOR WORK

1. **Eriguchi, Shinagawa, "Single-Shuffle Full-Open Card-Based Secure Computation Protocols for Any Function", 2025.** https://arxiv.org/abs/2510.17308
   - Shows: Theorem 4.2 / Corollary 4.4 give every Boolean function a static-open protocol with one random cut, binary cards, zero error and perfect privacy, using c·2^(r+1) + 2^(r+2) cards (c, r = communication and randomness of a PSM protocol). The construction interleaves piles with delimiters and opens equally spaced positions.
   - Differs: no explicit count for 3-input AND and no minimality; it also states 3-input AND had no known single-shuffle full-open protocol.
2. **Shinagawa, Nuida, "Cyclic Equalizability of Words and Its Application to Card-Based Cryptography", FCT 2025.** https://arxiv.org/abs/2507.04916
   - Shows: Theorem 5.2, an SCFO protocol for f needs at least max(N_0(f), N_1(f)) cards, which is 2^n − 1 for AND; it lists existence of a 3-bit AND SCFO protocol as open.
   - Differs: full-open only, one commitment pair per input.
3. **Ikeda, Shinagawa, "Impossibility Results of Card-Based Protocols via Mathematical Optimization", 2025.** https://arxiv.org/abs/2509.17595
   - Shows: by ILP, no SCFO protocol for 3-input AND (or other 3-variable classes except equality) when all helping cards have one colour, for any card count.
   - Differs: one pair per input and one helper colour; the mixed-colour case is stated as open.
4. **Shinagawa, Nuida, "A Note on Single-Cut Full-Open Protocols", 2025.** https://arxiv.org/abs/2507.03323
   - Shows: 8-card SCFO protocols for 3-input XOR and two other functions; recalls the five-card trick (den Boer) and six-card trick.
   - Differs: nothing on 3-input AND or partial opening.
5. **Pebody, "The Reconstructibility of Finite Abelian Groups", 2004.** https://www.cambridge.org/core/journals/combinatorics-probability-and-computing/article/reconstructibility-of-finite-abelian-groups/AC34CE37E4EADC905C023A24E93C9951
   - Shows (abstract): the set-reconstruction number of Z_n is at most 6.
   - Differs: it is the tool behind belief (e), not a card result.
6. **Shinagawa, Nuida, "Single-shuffle Full-open Card-based Protocols Imply Private Simultaneous Messages Protocols", 2022.** https://eprint.iacr.org/2022/1306
   - Shows: k-card single-shuffle full-open protocols give PSM protocols.
   - Differs: yields card lower bounds only through PSM bounds.

## VERDICTS

- **r(3) < ∞ (and r(n) < ∞ for all n): FOUND**, item 1. Solver B's 55% guess of r(3) = ∞ contradicts it. Solver A's m = 36 witness (six strands, R a subgroup) has the same shape as item 1's pile-interleaving.
- **r(3) ≤ 36, r(3) ≥ 16, E(3,m) = 0 for m ≤ 15, E(4,m) = 0 for m ≤ 13: NOT FOUND.**
- **f(n) = ∞ for n ≥ 3 (T2): NOT FOUND.** Items 2 and 3 treat it as open beyond the one-colour case, so T2 would answer a stated open problem if its proof holds.
- **m ≥ 2^n − 1 (T1):** FOUND for R = Z_m (item 2, Theorem 5.2); the extension to general R is NOT FOUND.
- **r(2) = f(2) = 5:** upper bound FOUND (five-card trick); E(2,4) = 0 is LIKELY FOLKLORE, a small case check, not located in this exact model.
- **L1/L2, r(n) ≥ 2n:** EASY COROLLARY of the standard equal-heart-count argument.
- **L3/T5 covering, T4 (|R| ≥ n+1), C3, L6, weak r(3) = 12: NOT FOUND.**
- **L5′ (E(n,a) ⇒ E(n,ab)):** EASY COROLLARY of item 1's interleaving lemma.
- **Belief (e):** Pebody's bound is confirmed from the abstract only. A snippet of arXiv 2503.21022 also credits Grünbaum with a sixth-order result on cyclic groups.

## Sequences

- 4,3,4,4,5,4,4,3 (min |R|, n = 2): six OEIS hits, all unrelated.
- 216, 1152, 7596, 34152 (weak counts): no OEIS match.

## Queries run (17)

Three-input AND single random cut minimum cards; single-shuffle multi-input AND random cut; five-card trick generalisation one random cut; card-minimal one-shuffle impossibility; non-committed 3-input random cut; homometric k-deck Pebody; single-shuffle full-open PSM; single-cut full-open impossibility; static-open/partial-open random cut lower bound; PSM AND complexity and card lower bound; six-card trick single-cut garbage-free; SCFO three-input AND both colours; Grünbaum–Moore higher-order invariants; random cut 36 cards; cyclic words window multiset 2^n − 1; two OEIS lookups.

## Whom to ask

A card-based cryptography specialist, in particular the authors of items 1–3; for T2 and L6, someone in combinatorics on words or homometric sets.

## Not checked

- I read four sources through a summarising fetch tool (items 1–4, item 1 in two parts), not the raw text. Items 5 and 6 are abstract only.
- I did not confirm that item 1's formal model matches the partition/helper conventions here line by line.
- I did not compute what Theorem 4.2 gives for 3-input AND, so I cannot say whether 36 beats it.
- I did not check the solvers' proofs.
- Japanese-language venues (IPSJ, SCIS) were not searched.
