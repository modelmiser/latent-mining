# Referee report: open-limit

Own code and logs: `open-limit-referee/` (`t1`–`t4_*.py`, `run_*.txt`). I did not run the author's programs.

## Bottom line

- **lim T(n)/n ≥ 1/3: established**, given solver B's Lemma 1 plus one transfer step the author omits (supplied below).
- **T(n) > B(n) for every n ≥ 30: established**, given Lemma 1 and the earlier referee's T(64q+r) ≥ 18q + B(r). My independent DP reproduces the whole table, n = 1..66.
- Nothing is refuted. One gap (§0) and three small overstatements.

## Verdicts

**§0 Setting: definitions sound; GAP in the use of Lemma 1, repaired.**
- h is finite, slots exist for every vertex (singletons included), h(v) = P + 1. Witnesses need not be saturated: A_0 is every vertex, R_0 any vertex with an out-edge. The type is a function of the current orientation.
- Gap: Lemma 1's proof fixes the Adversary's sequence, but Theorem 1's Adversary is adaptive and reads types off the position. Against non-normal replies the real position does not carry the types. Example: two cherries named at their saturated vertices, one edge flipped in each (cost 2), leave no R_1 vertex.
- **Repaired step.** Let S be an Adversary strategy for the normal-form game forcing ≥ c. In the full game Adversary keeps a shadow orientation D_L on the same forest, initially equal to the real one. Each round it names the pair S prescribes for the shadow position (legal: same G). When Algorithm answers with D_A′, the shadow is updated by the Lemma 1 reply: same tail x; free if x is unsaturated in D_L, otherwise the path of D_L-edges reversed in D_A′. Lemma 1's invariant cost(L) + d(D_L, D_A) ≤ cost(A) holds round by round whatever pairs are named, and the shadow play is a normal-form play against S. So cost(A) ≥ cost(L) ≥ c.
- So Remark (i) ("inductive from every position") is true only in the normal-form game or on the shadow.

**Lemma A: CONFIRMED.** No gap found.

**Lemma B: CONFIRMED.** Three reply kinds per endpoint enumerated; a free reply forces p = −1; an unavailable reply only removes Algorithm options.

**Type Lemma: CONFIRMED** against normal replies; false against arbitrary replies (above). The new witness is the tail of the new edge.

**Theorem 1: CONFIRMED** with the repair.
- Base: a singleton is A_0 with P = 0. Main phase: (*) gives P(new) ≥ P(X) + P(Y).
- Cleanup: nesting makes M(i_r) legal; the telescoping sum is right. All n − 1 rounds are played, none at negative cost.
- Leftovers: index 2k or 2k+1 needs ≥ 2^(k+1) − 1 vertices, so i_m ≤ 2·log₂(n+1) − 1 and m ≤ i_m + 1.
- Constant recomputed: n/3 − F(i_m) − (2/3)(m−1) ≥ n/3 − (7/6)·i_m − 1/3.
- Bound at n = 16, 64, 256: −3.4, 8.1, 67.5; positive from n = 34, above B(n) only from n = 189.
- Strategy worst case (my abstract DP) at 16, 64, 256: 3, 17, 79. Against shortest-path replies on real forests: 3, 18, 81. Both match the author.

**Theorem 2 reduction: CONFIRMED.** The merged tree contains both the new and the kept witness; extra abstract options only lower V_pair.

**Theorem 2 values: CONFIRMED by recomputation.** V_pair(n) = B(n) for n ≤ 29; every value for 30..66 equals the author's, including 62–66, which the author had from one program. V_pair(n) ≤ T(n) for n ≤ 15; V_pair(n) > B(n) for 30..66.

**Theorem 3: CONFIRMED.** Exceptional case and cases (a)–(c) followed; g(final) ≥ 1/3. Overstatement: "no one-witness strategy proves more than 1/3" holds for the pair game as defined only.

**Corollary 7: CONFIRMED.** Identities and ΣF ≤ (D+1)(D+3/2) check. Caveats: the author does not show such an Algorithm exists for a given D; and "an optimal Algorithm must let h grow" holds for normal-form Algorithms only, since Lemma 1 cannot lift the restriction.

**Small error.** The author's "never exceeds the exact T(n), n ≤ 16" relies on T(16) ≤ 3, which the earlier referee left unsupported.

## Mechanical tests (each false variant failed as it should)

- **Type Lemma and Lemma B** on explicitly merged trees, h recomputed: all pairs of trees with ≤ 6 vertices, every vertex pair; all 5,083 trees ≤ 9 against 12 small trees; 1,500 against 57 deep trees. 34.3 M cases, 0 failures. False: A_k cost ≥ k+1, 617,058 failures; R_k always gives A_(k+1), 196; other endpoint as witness, 480,646.
- **Every valid re-orientation**, merged tree ≤ 9 vertices, 3.9 M orientations: the strict lemma fails 15,743 times, never on a normal reply; the Lemma 1 step holds every time. False (one unit of slack): fails on all 500,856 at ≤ 8 vertices.
- **Strategy on real forests**, n ≤ 1000; shortest, longest (overpaying), random and potential-greedy replies; three merge orders: lemma, invariant, cleanup inequality and final bound, 0 violations. False F(R_k) = k+1: 64,141.
- **Exhaustive normal replies** against the strategy, n ≤ 14: minimum equals the exact T(n); 0 violations.
- **Non-normal Algorithm** (random extra flips) against the shadow Adversary, n ≤ 600: 0 violations. False (Adversary reads the real position): 1,740.
- **Pair DP**, n ≤ 66: equals the author's table. False (slot-1 cost p+2): exceeds T(4).
- **Theorem 1 bound** vs strategy worst case, n ≤ 300: 0 failures. False (coefficient 1): 7.
- **Theorem 3**, 11,025 type pairs: the prescribed option always works. False (φ(1) = 1/3): 13.
- **Corollary 7**, capped abstract game, D ≤ 3: 0 failures. False (δ doubled): 262.

## Not checked

- Lemma 1 beyond the earlier referee's n ≤ 11 and my single-round check.
- T(64q+r) ≥ 18q + B(r): taken from the earlier report.
- §8 and the author's scripts.
- The pair DP values rest on two independent programs, not on a certificate.
