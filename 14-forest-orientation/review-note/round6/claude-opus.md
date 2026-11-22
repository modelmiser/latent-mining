<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED. The construction is a legal play against rule R and the proof holds. NOTE.md:467-489 checked against :335-338 and the lemma 4 out-tree argument at :363-366. (a) I(1) holds. h depends only on the out-tree, so h(x)=h(y)=j−1. (b) Every shortest path from x ends at depth j−1. After the first round x points to c and y, with h(c)=j−2, h(y)=j−1, so h(x)=j−1. (c) h(x)=h(z)=j−1, the only neighbour of x with h=j−2 is c, and the round costs j−1. (d) Flipped vertices point toward x, so they are outside the out-trees of y and z. (e) c_2=2, c_3=10, c_4=36, and 3[(3^{j−1}−1)/2−(j−1)]+2(j−1)=(3^j−1)/2−j. (f) The tree has 3·3^{j−1} vertices. H ≥ j because of the root, and H ≤ j only follows from Lemma 5; the note does not say so. — Add "(H ≤ j by Lemma 5)" at :491.

Q2: CONFIRMED with one wording gap. :498-506. The bound G(n) ≤ max(0, ⌊(n−3)/2⌋) and superadditivity hold. Σd_i c_i = (n−Σd_i)/2 − Σi·d_i ≥ n/2 − (J+1) − J(J+1) = n/2 − (J+1)². I checked digit sum ≤ f and ≤ g for n < 2200. greedy_exact.cpp:241-246 keeps only the cheapest replies from both endpoints and all paths, and the Adversary picks among them (:281-285), so it does compute G. My run gives 0,0,0,0,0,1,1,1,2,2,2,3 for n=1..12, equal to f. I did not run n=13 or 14. The gap: building A_1 also needs an adversarial tie in its second round (h=0 on both sides), and :468-469 does not say so. — "Each level, including A_1, uses one adversarial tie, in its second round."

Q3: CONFIRMED. NOTE.md:356-441. I re-derived every case.
- k ≥ 1, middle path vertices: the gains are 1, ≥1, ≥1 and ≥3/2.
- w_0 cases: each gains ≥ 1/2.
- w_k cases: each gains ≥ 1/2.
- "No h decreases": correct by induction on the new h.
- k=0: all three sub-cases hold.
- Lemma 5: E ≤ 0 in all three cases, and the case split is exhaustive.
- The smaller-tree counterexample: Φ goes from 7/2 to 5 (+3/2) at cost 2.
— none

Q4: REFUTED.
- (a) :32-35 says "an upper bound on L below 1/2 needs a different algorithm". That ignores tie-breaking, which :503-504 and Open question 1 (:696-699) admit. A smaller-tree tie-break would flip from z in round 2 and leave the construction. Wording: "…so an upper bound on L below 1/2 needs a different algorithm or a tie-breaking rule for this one that defeats the construction."
- (a) :30-31 says Theorem 2 "is the smaller bound for most n below 94". That is true of f, not of the stated 9n/16 − log₂n. Floored, that formula is below g only up to n=75, and it is not stated at all for n < 19.
- (b) No sentence attributes (n−3)/2 to the smaller-tree rule, and no sentence gives 9/16 as the current state.
- (c) compare_bounds.py checks exactly the figures in :461-464, up to 4000. But "g ≤ f for every n ≥ 94" (:463) and "from n=126 on" (:32) are only computed to 4000.

Q5: CONFIRMED with exceptions.
- ternary_family.py:22 asserts a legal endpoint and :25 follows a shortest path. Its output matches for j ≤ 7 (I ran it). A_1 is hard-coded (:31), not played.
- pool_search tests rounds it really reaches, but only one vertex per signature class (:62-63). My run (NMAX 140, K 1) reached only k ≤ 4. The "length up to 6" claim depends on run parameters the repository does not record.
- Every checker exits non-zero on a failure. greedy_exact asserts nothing and prints the irrelevant ">B" marks.

Q6: CONFIRMED. Nothing in the repository contradicts either theorem.
- The B(n) values for n ≤ 15, T(30) ≥ 7, T(64) ≥ 18 and Theorem 1 are all ≤ g.
- f(3^j) ≥ c_j for j ≤ 7.
- I replayed j=2 by hand: costs 1+1 = 2.
- j=3: 3·2 + 2·2 = 10, matching the program.

F: MODERATE — NOTE.md:32-35, 14-forest-orientation/README.md:16-17 and :240-241, README.md:45 — These say rule R is "settled at rate 1/2" and that doing better "needs a different algorithm". That rests on adversarial ties and contradicts NOTE.md:503-504 and :696-699 — high — add "or a tie-breaking rule" and say that the ties go against the algorithm.
F: MODERATE — NOTE.md:30-32 — "Theorem 2 … smaller for most n below 94" compares f with g, not the stated formula. As stated, Theorem 2 is below g only for 19 ≤ n ≤ 75 (not at 74) — high — say "the recurrence f behind Theorem 2".
F: LOW — NOTE.md:463-464 and :32 — "for every n ≥ 94" and "from 126 on" are only computed to 4000. The step for larger n is missing; it is easy: D(n)=f(n)−(n−3)/2 satisfies D(a+b) ≥ D(a)+D(b)−3/2+c(a) — high — add that argument or say "up to 4000".
F: LOW — NOTE.md:457 — "paths of length up to 6": no run parameters or log are in the repository. With n ≤ 140 I reached only k=4 — medium — record the command and output.
F: LOW — NOTE.md:505-506 — G(13) and G(14) rest on one program run, and its output is not saved — medium — attach the log.
F: LOW — NOTE.md:468-469, :502 and README.md:57 — The adversarial tie needed to build A_1 is not mentioned — high — state it.
F: LOW — NOTE.md:491 — H = j is asserted, but it needs Lemma 5 — high — cite it.
F: LOW — ternary_family.py:31 — A_1 is not played, so its two rounds are not checked — high — play them.
F: LOW — pool_search.cpp:89 — The program prints "Lemma A" (the old name). It crashes if run without arguments — high — rename it and check argc.
F: LOW — NOTE.md:461/496 vs :538/546 — g and G each mean two different things in Section 5, and "Theorem 3" also names open-limit/proofs.txt Theorem 3 — high — rename one of each.

Other notes, most severe first:
- The README's "Review" section does not yet say that Theorem 4 has had no review.
- The round-5 FIXED items are confirmed: the k=0 case, the scope paragraph, the crossover now computed by a program, and non-zero exit codes.
