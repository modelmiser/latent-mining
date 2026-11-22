<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Verdict: I found no counterexample. Lemmas 6, 7 and 8 and the bound T(n) ≤ ⌊(n−1)/3⌋ hold under my own checks. One claim in the draft is overstated: "T(n) = n/3 − Θ(log n)" is not proved; only O(log n) is. I re-ran Attachment B and wrote my own tests in /tmp; nothing in the repository was changed.

Q1: CONFIRMED — Each line of the weight table equals the formula with the maximum. One out-neighbour of height 0 gives 1+(1+ε)−2 = ε. One of height h ≥ 1 gives 1. Two with q = p: g = 3p+4, so 3p−1+ε_p+ε_q. With q = p+1: g = 3p+5 ≥ γ_q, so 3p+1+…. With q ≥ p+2: γ_q is the maximum, so 3p+2+ε_p. g ≥ γ holds because γ(r) = 3P+4+ε. Deleting a vertex of in-degree 0 changes no other weight. t − max(t,s) = min(0, t−s) is non-decreasing, so both cases go through, and the base case is 0 ≤ 0. — none

Q2: CONFIRMED — S1–S4 cover every case: raising Q when P = Q gives S1, raising Q when P < Q gives S2, raising P gives S3 or S4. I rechecked every line of the case analysis. A larger rise works as a chain of single steps: raise the larger slot first, then the smaller, one out-neighbour after the other. A slot rising from −1 (w_k, or x when k = 0) is also covered. — State Lemma 7 as "non-decreasing when the sorted slots of every out-neighbour are componentwise non-decreasing", and write out the chaining.

Q3: CONFIRMED — Both o_i formulas and the conditions for equal slots check out. So do the old-weight and new-weight tables in (a) and (b) and the reductions (the new weight is at least 7 in (b) and at least 5 in (a)). Rule L gives x's slots ≤ y's slots, so β = 0 and ε_y = 0 force α = 0. The rule also gives a_0 ≤ Q_y, which the proof does not use. The sum telescopes to 3k, including k = 1, where f_0 says whether w_k has an out-neighbour. Hand computations, as (gain, required):
- (a) α0β0ε_y0ε_c0, f false: (1, 1)
- (a) α1β0ε_y1ε_c0, f false: (1, 1)
- (a) α1, f true: (2, 2)
- (a) α2β1ε_y0, f false: (2, 1)
- (b) α0 o0, f false: (2, 2)
- (b) α1 o0 ε_n1 ε_c1: (3, 3)
- (b) α1 o1 e true: (4, 4)
- (b) α2 o0 ε′1 ε_n1: (3, 3)
- (b) α2 o2 e true: (6, 4)
- (c) ζ0 ε_z0 o0 e false: (2, 2)
- (c) ζ0 o2 e true: (2, 2)
- (c) ζ1 o0 e false: (2, 2)
- (d) ζ0, h(y) = 0, ε_y = 1: (0, 0)

— "the only consequence of rule L used is …"

Q4: CONFIRMED (except one phrase) — Step 1 of Lemma 4 (NOTE.md:392–401) needs only b ≥ k and that y is in the other tree. Slots of path vertices do not decrease. Vertices off the path are covered by Lemma 7 whatever their in-degree. Rule L can always be carried out: a lexicographically smaller slot pair means a smaller P, so h(x) ≤ h(y), and every out-degree stays ≤ 2. The phrase "T(n) = n/3 − Θ(log n)" does not follow; Theorem 1 with Theorem 5 gives only n/3 − O(log n). — "T(n) = n/3 − O(log n)"

Q5: CONFIRMED (with a caveat) — The program models the heights, flags, states, the rule-L constraint, the end vertex and k = 0 faithfully. It also checks impossible states such as (0, true), which only makes it stricter. Values ≥ R rest on the hand reductions, which the program does not check. It does not check Lemma 6 or the off-path step at all. Running it with R = 0 prints ALL OK and exits 0. — Make it refuse R < 4.

Q6: CONFIRMED (no counterexample) —
- **Consistency:** (n−1)/3 lies above B(n) for n ≤ 15 and above T(30) ≥ 7 (bound 9), T(32) ≥ 8 (10) and T(64) ≥ 18 (21), and above Theorem 1's lower bound.
- **Path ending at a vertex with a tall out-neighbour:** I computed Φ (in thirds) by hand and confirmed by program. A path ending at w1 → z, with z of height 2: Φ goes 0 → 5. Flipping from y instead: 0 → 3.
- **Tall second out-neighbour against a tall y:** x has slots (0,3), y has (3,3), and w1 has 4 in-neighbours. Φ goes 25 → 38: x +8, w1 +1, the in-neighbours +1 each.
- **Ties in h:** x with slots (0,1) against y with (0,0). Rule R flipping from x gains only 2 < 3; rule L flips from y and gains 4. So the tie rule is essential.
- **Random trees:** about 170k rounds, with path lengths up to 4. No failure of Lemma 6, no h decreased, no off-path weight fell, and every round gained ≥ 3k.
- **Theorem 4's construction:** at the second round {x,z}, x has slots (j−2, j−1) and z has (j−2, j−2). Rule L flips from z, so the Adversary cannot force x. A_1 cannot be built either: after r→a, a vertex with no out-neighbour beats r.

— none

F: MODERATE — Attachment A, Theorem 5 statement — "T(n) = n/3 − Θ(log n)" is not proved. The upper bound is (n−1)/3 − H(T), and H(T) can be small at the end, so only O(log n) follows — high — Write "n/3 − O(log n)".
F: MODERATE — Attachment A, "Checks run" and "How it was found" — phiU_c3.py, gpoolC3.cpp, pathcheck.py, cex1_c3.py, ruleL_emit.py and runC3.sh are not in the repository. So the claims about the 43-vertex example, play up to 11 vertices and the pool search cannot be checked — high — Add the programs or drop the claims.
F: LOW — third_check.py, the R argument — R ≤ 3 makes the run vacuous; I ran R = 0 and got ALL OK with exit 0 — high — Refuse R < 4.
F: LOW — Attachment A, Lemma 7 and "Checks run" — 27,225 is the count for R = 14, but the text says the run was `third_check.py 30`, which gives 477,617 for part (M) and 21,544 for part (P). I reproduced all three counts — high — Name both runs.
F: LOW — Attachment A, Theorem 5 proof — the chaining of single-slot increases (several steps, both out-neighbours) is not written down — high — Add it, as in Q2.
F: LOW — Attachment A, Lemma 8 (a) — "Rule L is used here and only here" is inexact. Theorem 5 also needs h(x) ≤ h(y), and rule L gives more than the stated consequence (a_0 ≤ Q_y) — medium — "the only consequence used in Lemma 8".
F: LOW — Attachment A, Theorem 5 proof — Step 1 of Lemma 4 also uses that y is in the other tree — medium — Say so.
F: LOW — Attachment A, "In words" — when h = 0, "other out-neighbour shallower" means the endpoint with fewer out-neighbours wins. That is the tie that blocks A_1 — low — Spell it out.

Not asked about:
1. If merged, the NOTE.md Summary ("needs a different algorithm …", lines 31–34), Theorem 4's discussion and the open questions would need rewriting.
2. Lemma 8 holds on any pair of trees, reachable or not, which my random tests confirm. The proof is hand-checkable in full; the program only adds confidence.
