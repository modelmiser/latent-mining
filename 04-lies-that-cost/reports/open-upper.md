# open-upper report

Files: `open-upper/` (`proofs.txt`, `table.cpp`, `lemmaC.py`, `strat.py`, `*_out.txt`).

## Result

**Q_k(n) = U1(n) = P(n) for every k ≥ 2 and n ≥ 2; the headline is YES.** The proof is complete except for one finite machine table. Pelc's theorem is not used; its upper bound U1(n) ≤ P(n) is a by-product.

Notation: state (a, b; j) = a candidates free to lie, b blocked for good, j questions left; weight w = a(j+1) + b; *nice* means b ≥ a − 1.

## What is proved

1. **Lemma C (COMPLETE, by hand).** For k ≥ 2, from any position with m alive candidates and arbitrary obligations, the questioner wins within 3·⌈log2 m⌉ questions. Ask the same half S up to three times. Each such question has ell ≥ 1, so a liar is blocked for at least 2 questions and lies at most once per triple; the majority answer is true for every survivor. False for k = 1.
2. **Lemma S (COMPLETE, by hand).** A nice state with w ≤ 2^j and a + b ≥ max(2, 2j − 2) has a question with 2·ell ≥ j − 1 whose two outcomes are nice with weight ≤ 2^(j−1). It splits the free candidates evenly and, for odd a, offsets the blocked ones by j − 1 or j − 2. Since k·ell ≥ j − 1, every liar is blocked for the rest of the game, so the Q_k transition is exactly the one-lie transition.
3. **Theorem 1 (COMPLETE given table T1).** For k ≥ 2, every nice state with w ≤ 2^j is Q_k-winnable. Induction on j: Lemma S when a + b ≥ 2j − 2; otherwise Lemma C when j ≥ 15 (there 3·⌈log2(2j−3)⌉ ≤ j), and T1 when j ≤ 14.
4. **Theorem 2 (COMPLETE given T1, T2).** Q_k(n) ≤ P(n) for all k ≥ 2, n ≥ 2. The first question splits n evenly. Liars are blocked for good because P(n) ≤ n + 1 (n even) and P(n) ≤ n (n odd, n ≥ 7). Both outcomes are nice with weight ≤ 2^(P(n)−1), by the definition of P including its odd-n term. The exceptions n = 3 (k = 2, 3) and n = 5 (k = 2) are table T2.
5. **Corollary.** For k ≥ 2 and b ≥ a − 1: Q_k winnable from (a, b; j) ⇔ one-lie winnable ⇔ a(j+1) + b ≤ 2^j. This is the referee's equivalence for all nice states.

## The machine step

- **T1:** every nice (a, b; j) with w ≤ 2^j, 2 ≤ a + b ≤ 2j − 3, j ≤ 14 is Q_k-winnable for k = 2..13. For k ≥ 13 and j ≤ 14 the game is identical to k = 13.
- **T2:** Q_2(3) ≤ 5, Q_3(3) ≤ 5, Q_2(5) ≤ 6.
- My own exhaustive solver (`table.cpp`) covers a superset: k = 2..16, j ≤ 16, a + b ≤ 2j, 1884 states per k, none lost. An unpruned rerun (k = 2, 3; j ≤ 9) agrees.
- The referee's independent `ms cmp` (k = 2, 3; all a + b ≤ 26; j ≤ 14; 5250 states each) shows no difference from the one-lie game.

## Other mechanical tests

- **Lemma C, literal:** labelled candidates, every obligation multiset in 0..6 for m ≤ 7 (3424 multisets), every choice of S, every responder. No failures for k = 2, 3, 5.
- **Proof replay (`strat.py`):** every state the strategy reaches for n = 2..60000, plus 9828 n up to 2^200 including both sides of each P threshold. Niceness, weight, split existence and k·ell ≥ j − 1 are asserted at each step for k = 2, 3, 7; no failures. Only 13 to 16 table states are reached from a root, and Lemma C never.
- **End to end (`table proof`):** questioner forced to ask the proof's question under real obligation dynamics, n = 2..3000, k = 2, 3, 4. She wins at P(n) for every n.
- **Controls that fail as they should:** k = 1 in the table (30 of 400 states lost), in Lemma C (2781 failures) and in the forced strategy (21 failures for n ≤ 60); a mutated split in `strat.py` trips the weight assertion.

## What failed

- My first end-to-end harness applied the nice-state rule to the root (n, 0), which is not nice, and reported every odd n ≥ 13 as failing. A harness error (the proof treats the root separately); fixed and rerun.
- I found no proof that higher obligations always help the questioner: the natural simulation breaks because ell depends on the alive set. Lemma C avoids needing it.

## Open

- The referee's equivalence for non-nice states (b < a − 1). Not needed for the headline.
- k = 1: both lemmas fail there.
- A hand proof of T1.

## Confidence

- Lemma C and Lemma S: about 97% each.
- T1, T2: about 98% (two independent programs, small state space).
- Theorem 2 for k = 2: about 95%. It agrees with the solvers' Q_2(n) = P(n) for n ≤ 2805.
- All k ≥ 2: about 93%; the extra step is the reduction of k ≥ 13 to k = 13 at j ≤ 14.

The proofs have had no outside review. The checks most worth a cold read are the odd-a case of Lemma S and the threshold arithmetic in Theorem 1.
