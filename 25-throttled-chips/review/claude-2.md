<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of a Claude seat run on the plan login (one of the two on Claude Opus). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Claude seat 2: experiment 25 review

## Verdicts

**Q1 (C1): CONFIRMED.** At `solver-a/proofs.txt:8-12`, (x−1)²+(x+1)²−2x² = 2 holds, and an add at 0 adds 0. Before the m-th step (2), only rounds 1..m−1 have fired, so there are at most m−1 firings. x² ≥ 9−w_x holds with equality at |x| ≤ 3. Σw·η ≤ 35H gives 7m ≤ 35H−2, so m ≤ 5H−1. B's P3 (`solver-b/proofs.txt:36-43`) also holds. Nothing is ready in round 1, so there are at most m−2 firings and Q ≤ 10h−16. The greedy filling gives 2(h−1)+4(2h−3) = 10h−14, and 3(h−1) ≤ 5h−6 ≤ 5(h−1) holds for h ≥ 2. Both proofs test the position right after step (1), which `chain/3-link.md` allows ("immediately after some step (1)").

**Q2: CONFIRMED.** I expanded k_x−2T_x+T_{x−1}+T_{x+1} myself. The coefficients are k_x: 5−6+1+1 = 1, k_{x±1}: −2+3 = 1 and k_{x±2}: 1, so the result is the 5-point mean (`solver-a/proofs.txt:26-27`). Identity (**) is (*) minus this expansion (line 28). Fact (a) holds because T never decreases and a firing needs l ≥ 1 and lowers l by exactly 1. Fact (c) holds because F ≥ 0 and 3k_x+k_{x±1} ≤ 3(k_{x−1}+k_x+k_{x+1}).

**Q3: CONFIRMED.** For (d) (lines 30-32), l_x = M dominates its neighbours, so η_x ≥ η*_x ≥ M/3 ≥ 2. The same argument applies to every vertex of lag M, so every tie is ready, and the rule ("largest lag among ready") must pick lag M. A smaller-lag vertex can be chosen only when no max-lag vertex is ready, which needs M < 6. For (e) (lines 33-38), convexity gives g(u+d)−g(u) ≤ d·g′(u+d) ≤ 2d·max(M−6,0) with Σd = 1. When M ≥ 7, the firing removes 2(M−6)−1. When 6 ≤ M < 7, it removes (M−6)², so the net change is u(2−u) ≤ 1. When M ≥ 6 the firer never passes. When M < 6, Θ = 0 after the add, and it stays 0 whether the firer fires or passes. For (f) (lines 39-41), after the add the lags are at most 6.6+√(t−1), and after the firing at most 6+√t. Both give η ≤ t/5+13.2+2√t, using l_{x±1} ≥ 0.

**Q4: CONFIRMED.** The rule reads only k up to the current add, fires only ready vertices, and fires at most once per round. My algebra: if m < 5h−10√(5h)−72, then m/5 < h−2√(5h)−14.4 and 2√m < 2√(5h), so the sum is below h−1.2. Every integer m below the bound therefore loses, and G(h) ≥ 5h−10√(5h)−72 for every h, not only large h. With C1 this gives G/h → 5 and C/h → 5.

**Q5: no break found.** I wrote throwaway Python that simulates the rule exactly (lags scaled by 5) and asserts (*), lag ≥ 0, and "fired lag = M when M ≥ 6". Test lists, each 400 long: all zeros; 0,2 alternating; 0,1 alternating; 0,50 alternating; a spread over −10..10 then a 300-chip burst at 0; sites 0,2,4,6 cycled then bursts at 2 and 4; blocks of 7 cycling over three sites; 5 random lists on 7 sites.
- Real rule: largest lag 2.0, largest height minus t/5 2.4.
- A lazy variant that fires only at lag ≥ 6 reached lag 7.0 and height excess 8.0, and every assertion held.
- Sites two apart do not interact badly, because their targets overlap only through T at the vertex between them.

**Q6: CONFIRMED, with the search code taken on trust.**
- P2 (`solver-b/proofs.txt:20-33`) is a proof. Only v is ever fired, so v gains chips only from its own adds. The bound k ≤ 1+(c(v)−h+1)/2 then gives a neighbour at most h−½. h ≥ 3 makes the firing at h−1 legal.
- Lemma Z(i) is immediate.
- Lemma Z(ii) (lines 91-102): A's zone at any time is contained in the union of PA's final intervals, because rad never decreases. A Z-line keeps every chip, including the chips a firing sends to neighbours, inside its zone. The two plays therefore stay on disjoint vertices, and the composite play is a legal, surviving Z-line.
- Lemma Z(iii): the induction over the shortest Z-winning list is sound, and translation and reflection preserve zones.
- The values G(4) = 10 and G(5) = 15 for all lists depend on g4.cpp and v.cpp, which I did not audit. The referee did not read g4 line by line either (`reports/referee.md:73`).
- What the README says about G(6) matches `referee.md:3,14-15,48` and `solver-b.md:14,21`.

**Q7: mostly CONFIRMED.** Every sentence of "Why no list is much shorter" matches the proof, including "a third of its lag", "6.6 + √t" and "evenly over five points". Exceptions are listed in the findings.

**Q8: REFUTED in part.** The cells match their sources except as listed below. The game in the README matches the FINAL QUESTION in `chain/3-link.md:9-11`.

## Findings

1. **MODERATE (high confidence). The headline overstates what is proved.** `README.md:9-10` says "for large h no list does better than dropping every chip on the same point", and the top-level `README.md:55` says "dropping every chip on one point is as good as anything". Only G/h and C/h → 5 is proved; C(h)−G(h) may be as large as O(√h). G = C is the open conjecture in `README.md:47`.
   Replacement: "for large h no list does much better than dropping every chip on the same point: both need 5h − O(√h) rounds."

2. **MODERATE (high confidence).** "Mixing helps only at h = 3" (`README.md:15-16`) and "From h = 4 on the computed values of G(h) equal those for zeros alone" (`README.md:~92`) cover only h = 4 and 5. G(6) is open.
   Replacement: "Mixing helps at h = 3; at h = 4 and 5 it does not, and beyond that it is unknown (by at most O(√h))."

3. **MODERATE (certain). Broken link.** `README.md:32` links `chain/3-question.txt`, which does not exist: `chain/` holds only `1-` and `2-question.txt`. The formal text is the FINAL QUESTION section of `chain/3-link.md`. The brief makes the same reference.
   Replacement: "formal text in the FINAL QUESTION section of chain/3-link.md", or add the file.

4. **LOW (high confidence). Process attribution.** "Solver B and the referee each reported a third process overlapping briefly" (What to distrust, Process). `reports/referee.md` says nothing about this; only `reports/solver-b.md:77` does.
   Replacement: "Solver B reported a third process overlapping briefly", unless another source is cited.

5. **LOW (high confidence). Scope of the referee's tests.** "ran the strategy against every short list" (`README.md:40`). The exhaustive runs covered only windows of 4 to 7 vertices (`referee.md:37`).
   Replacement: "against every short list on 4 to 7 neighbouring points".

6. **LOW (medium confidence). Z-winning versus winning.** "checked mechanically on small cases that minimal winning lists do have that shape" (The small values). The referee checked minimal *Z-winning* lists inside a window (`referee.md:47`).
   Replacement: "that minimal lists winning against the restricted firer, inside a window, have that shape".

7. **LOW (medium confidence). Source of the 1989 attribution.** `README.md:63-65` attributes the potential to Anderson et al. 1989, but `prior-art.md:7` read that paper only through Kelley's survey.
   Replacement: append "(read through a survey)".

8. **LOW (certain). The large-h qualifier is unnecessary.** "for large h" on the lower bound (`README.md:12,40`) is not needed, because the inequality holds for all h. This does not change any claim.

No defect found in Theorem 1, P3, P2, Lemma Z, or the steps (a)–(f) of Theorem 2.
