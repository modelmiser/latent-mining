<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

**Verdict:** I traced every case of both lemmas by hand and found no counterexample. Theorem 3 holds for the game value T(n) as NOTE.md defines it. Three things need fixing: the k = 0 case in Lemma A has no argument, the comparison with Theorem 2 should be made against the f(n) table rather than the 9n/16 − log₂ n formula, and the enumeration programs only test path lengths up to 2. I ran Attachments B and C on copies in /tmp; nothing in the repository was changed.

Q1: CONFIRMED with one gap — Draft Lemma A:
- (a) The out-neighbours after the flip are as stated.
- (b) If some cᵢ (or z, or q) could reach a path vertex, that route plus the path and the edge wᵢcᵢ would form a cycle in a forest. The other tree has no edges into the first.
- (c) Assume g_j ≥ k − j. Then g_{i−1} ≥ k − i + 1 and aᵢ ≥ k − i − 1, so gᵢ ≥ k − i. Also g₀ ≥ k, because h(q) ≥ h(p).
- (d) All four cases for wᵢ, all cases for w₀, all three cases for w_k, and k = 1 hold line by line. For w₀ with a ≥ d+2 and b ≤ a, the step needs splitting into b = d+1 and b ≥ d+2.
- (e) Every edge is the out-edge of exactly one vertex, so nothing is missed.
- Gap: for k = 0, Steps 1 and 2 do not cover p's own terms, because p gains an out-edge. Step 3 assumes k ≥ 1. The claim is still true: from 0 out-neighbours the gain is ≥ 0; from one, z, the gain is ≥ 0 by checking h(z) = 0 and h(z) ≥ 1 with b = 0 or b ≥ 1.
- Replacement: "For k = 0, p's terms go from 0 or ½·min(h(z),1) to at least the same; checked case by case."

Q2: CONFIRMED — Draft Lemma B:
- A vertex of in-degree 0 exists, since there are n−1 edges and n vertices. Its neighbours are all out-neighbours, so removing it leaves one or two parts, and no other h changes.
- The three cases cover every way the maximum can be attained. Each inequality holds.
- In the third case the slack is exactly 0. The text writes "−2(m+1)" against the other side instead of 0 — inconsistent with the first two cases but not wrong.
- Base case: 0 ≤ 0.
- Replacement: write the third case as "2m − 4m − 2 + 2(m+1) = 0".

Q3: CONFIRMED — NOTE.md:42-48 defines the game: n−1 joins, so the final forest is always one tree, and T(n) is the game value. The greedy rule is a legal algorithm, so T(n) ≤ its cost.
- Lemma A plus Φ(empty) = 0 give total cost ≤ Φ(T). Lemma A allows any tie and any shortest path, so every tie-breaking is covered.
- If cost > 0 then Φ > 0, so H ≥ 1 and the cost is ≤ (n−3)/2. The integer floor is fine, and n ≤ 2 gives 0.
- L ≤ ½ follows because L exists (NOTE.md:324-329).
- Replacement: say that the rule is the smaller-h rule of [7] (NOTE.md:258), not the smaller-tree algorithm of NOTE.md:246-249. Lemma A uses h(q) ≥ h(p), which the smaller-tree rule does not guarantee.

Q4: CONFIRMED (no contradiction) —
- NOTE.md:333-337 gives T(n) = B(n) ≤ ⌊(n−3)/2⌋ for n ≤ 15, and T(30) ≥ 7, T(32) ≥ 8, T(64) ≥ 18 are all below the bound. Theorem 1's n/3 is below n/2.
- f(n) is a bound for the recurrence, not a lower bound, so the f table cannot contradict it.
- Hand run: join two 3-stars at their centres. The cost is 1, Φ(final) = 1 (tight), and ⌊3/2⌋ = 1. I could not build a run that exceeds either bound.
- Replacement: none.

Q5: REFUTED in detail —
- The formula crossover, log₂n − 3/2 < n/16, first holds at n = 76, not about 80.
- The proved bound is f(n) (NOTE.md:257), not the formula. Computed: f(n) ≤ ⌊(n−3)/2⌋ for all n ≤ 95, with equality at 64, 66–72 (even), 80–90 (even), 94, 95. f(n) is strictly larger at 96, 98–108 (even), 112, and every n from 126 to 400.
- "The table of f(n) is smaller" should read "no larger". "Minimum of the two" is correct, since both are valid upper bounds on T(n).
- Replacement: "T(n) ≤ min(f(n), ⌊(n−3)/2⌋); the new bound is strictly better at n = 96 and for all n ≥ 126 (checked to 400)."

Q6: CONFIRMED with gaps —
- Generating trees by adding leaves (an edge either way) reaches every oriented tree with out-degree ≤ 2, and the canonical form is correct. My run of lemma2.py 10 gives 3, 7, …, 15000 trees with 0 violations. The check of Lemma A (allcheck.py) covers both orders of every pair, all endpoints with h(p) ≤ h(q), and all shortest paths; with cap 2 the program's Φ equals the draft's Φ. My run of allcheck.py 7 12 gives 816,601 merges with minimum slack 0.
- Gaps:
  1. Trees of at most 7 vertices mean k ≤ 2 (the size fact, NOTE.md:56). So the middle case is tested only at d = 0, and the case a > g is never reached with d ≥ 1.
  2. lemma2.py at n ≤ 10 means H ≤ 2.
  3. The minimum slack is 0 for caps 2, 3 and 99 alike, so the check cannot tell the caps apart.
  4. The pool search, the exact greedy runs (n ≤ 14), the LP and the `allcheck 10 15` result are not in the repository.
- Replacement: "Lemma A checked for k ≤ 2, Lemma B for H ≤ 2."

F: MODERATE — Draft Lemma A, last line — the k = 0 case leaves p's changed out-terms without an argument; the result is still true — high — add the case check from Q1.
F: MODERATE — Draft, comparison paragraph — the crossover is computed against the formula, not f(n); the new bound is strictly better only at n = 96 and n ≥ 126; "smaller" should be "no larger"; "about 80" should be 76 — high — use the Q5 wording.
F: MODERATE — Draft, "Checks run" — the enumeration exercises only k ≤ 2 and H ≤ 2, and the general cases rest on the hand proof alone; the pool, exact-greedy and LP artifacts are missing — high — state the limits and add the programs or drop the claims.
F: LOW — Draft "Greedy rule" vs NOTE.md:246-249 — the draft's rule is the smaller-h rule of [7], not the note's Section 5 algorithm; Lemma A needs h(q) ≥ h(p) — high — name the rule explicitly.
F: LOW — Draft Step 1 — the induction hypothesis is stated unclearly ("in fact g_{i−1} ≥ …") — high — state gᵢ ≥ k−i as the hypothesis.
F: LOW — Draft Step 3, w₀ with a ≥ d+2, b ≤ a — the bound d+3/2 needs the split b = d+1 vs b ≥ d+2 — high — add it.
F: LOW — Draft Lemma B, third case — the slack is written inconsistently with the first two cases — high — write it as "= 0".
F: LOW — Draft title — "Theorem 3" already names results in open-limit/proofs.txt:125 and README.md:52 — medium — rename it.

Not asked:
1. If adopted, NOTE.md:18-25, 309-311 and README must change: "9/16 best constant" stays true only for the recurrence, and L ≤ ½ should be added.
2. The draft's "0.45·n at n = 120" bounds the greedy rule's cost, not T(n). It should not be read as evidence about L.
