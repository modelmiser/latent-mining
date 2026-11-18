# Referee report

Own code and logs: `verifier/` (`game.cpp`, `game2.cpp`, `game3.cpp`, `certcheck.py`, `run_*.txt`, `mut*.txt`).

## Verdicts

**Headline, "Is T(n) = B(n) for every n?" NO: CONFIRMED.** T(30) ≥ 7 > B(30) = 6 by two routes that share no lemma.
- **Solver A (W₂ game + P6): CONFIRMED.** My checker accepts `cert_W2_14_ge4.txt` (280 claims) and `cert_W2_16_ge5.txt` (441 claims) against every flip set inside the merged tree, with ψ₂ computed from its definition. This needs P1 and P6, not B's Lemma 1. P6 and ψ_j: no gap found (xA ≥ dist, min(p,q) ≥ min(j,p)+min(j,q)−j, and P1 carries over to the W game).
- **Solver B (`cert_n30_k7.txt`): CONFIRMED against normal-form replies only.** A replay against all legal replies is impossible: non-normal replies leave the 140 listed positions. Instead I verified Lemma 1 mechanically: for every oriented forest on n ≤ 11 vertices and every Adversary move, the best reply over all valid orientations of the merged tree equals the best normal-form reply (467,488 comparisons at n = 11, 0 mismatches; also the W₂ game, n ≤ 9). A deliberately broken normal form gives 152 mismatches at n = 8. Lemma 1's proof: no gap found.

**Table: CONFIRMED for n ≤ 15, recomputed.** T = 0 (n ≤ 7), 1 (8–11), 2 (12–15). Exact with whole-forest re-orientation for n ≤ 9, merged-tree-only for n ≤ 11, normal form for n ≤ 13. For n = 14, 15, budget search with normal-form replies: "≥ 3?" NO (1.29 M and 5.59 M positions), "≥ 2?" YES. No entry differs from either solver.

**T(16) = 3: lower bound CONFIRMED (search); upper bound UNSUPPORTED by me.** My solver would need about 17 GB, outside the caps; T(16) ≤ 3 rests on A's `fast2` alone.

**W values (A): CONFIRMED.** W₁ = W₂ = W₃ = ⌊m/4⌋ for m ≤ 13; W₁(14) = 3; W₂(14) = 4 exactly; W₂(16) ≥ 5 by certificate.

**T(32) ≥ 8: CONFIRMED** (B's certificate; also A's 2·W₂(16) − 2).

**T(64) ≥ 18, so lim ≥ 9/32: CONFIRMED given Lemma 1.** My checker took 3 s on the 10 MB certificate (29,834 positions, all visited); it rejects the file with one line removed, and k = 19.

**"T > B for every 30 ≤ n ≤ 43" (B): UNSUPPORTED for 36–43.** T(36) ≥ 9 and T(40) ≥ 10 have no certificate, and superadditivity from 30 and 32 gives only equality. Certified deviations: n = 30–35 and every n ≥ 64 (T(64q+r) ≥ 18q + B(r)).

**Proofs marked COMPLETE, each read line by line; no gap found in any:**
- A P1 (only the merging components need be touched). Whole-forest and merged-tree values agree on every position, n ≤ 9.
- A P2 and corollary (desaturation costs the directed distance; two components are worth min(maxd)). Checked on all 434,104 two-component positions at n = 13.
- A P3, B Lemma 2 (T ≥ B; B's "P − 1 from any position"). Value ≥ P − 1 on every position, n ≤ 13.
- A P4, B Lemmas 3, 5 (monotone, superadditive, limit exists).
- A P5 (T ≤ 3(n−1)): proof read only.
- B Lemma 4 (T(n) ≤ n − log₂(n+1)). Its size bound holds on every tree up to 13 vertices; its recursion gives ratio 0.5579 for n ≤ 3000 (B: 0.558).
- A P6, B Lemma 1: above. A P7: trivial.

**Least n with T > B, exact formula, exact limit: not delivered by either solver.** Known: least n ∈ [17, 30] ([16, 30] without A's n = 16 run); 9/32 ≤ lim ≤ 1.

## Disagreements

- **Upper bound:** A 3(n−1), B n − log₂(n+1). Both correct; B's is stronger.
- **Limit:** A "no evidence τ > 1/4"; B τ ≥ 9/32. B is right.
- **Least deviation:** A [17, 30], B [16, 30]; the difference is A's n = 16 run.
- **Reading of the problem:** identical; no ambiguity or easier-neighbour substitution found.

## Errors and weaknesses

1. B presents 36 ≤ n ≤ 43 as following from monotonicity; it depends on uncertified search output.
2. A's remark that P6 "cannot give anything below 30" also needs W₂(15) ≤ 4, where A had only a lower bound. I computed W₂(15) ≤ 4 (5.59 M positions), so the remark is true for j ≤ 3, but A had not established it.
3. B's "every reachable position has value P − 1 for n ≤ 11" is true, and holds on every position for n ≤ 13. It must fail at n = 14: two 7-vertex out-trees are worth 2 by P2, while P − 1 = 1.

## Broken witnesses

All rejected by my checker:
- B, n = 30: line deleted; k lowered; digit changed; B's own MUTANT; genuine file at k = 8.
- B, n = 64: line deleted; genuine file at k = 19.
- A: A's two corrupt files; genuine files at J = 1; my three mutants (line deleted, value raised, arc flipped).

## Not checked

- T(16) ≤ 3.
- A's `table_n8…13` and B's `upper_n*.txt` (values recomputed instead).
- A's out-degree-cap-1 table and restricted-Adversary runs; W₂(17), W₂(18) ≥ 5; B's sketch for L ≥ 1/3.
- Lemma 1 mechanically beyond n = 11. My n = 14, 15 results use normal-form replies.
- I did not run the solvers' programs.

## Follow-up tasks

1. **Settle 16 ≤ n ≤ 29.** Either a certificate for T(n) ≥ B(n) + 1 at some n < 30 (B's format, or a W-style composition with more than two parts or a richer terminal potential than ψ_j), or a certified upper bound T(n) ≤ B(n) for n = 16…20 from a memory-lean budget search emitting a closed position set. Start by re-deriving T(16) ≤ 3 with a second program.
2. **The limit.** Prove or refute B's conjecture T(4^k) ≥ (4^k − 1)/3 − k: first a checkable certificate for T(256) ≥ 81 against normal-form replies, then an induction with a potential that survives Algorithm overpaying by 1 in a subtree.
