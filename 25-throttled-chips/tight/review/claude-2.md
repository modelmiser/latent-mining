<!-- Archived by the organiser on 2026-11-21. Report of the second Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 2: experiment 25, 5h − 28 ≤ G(h) ≤ 5h − 10

All algebra redone by hand; table and C(h) recomputed independently (my own brute force from `chain/3-question.txt`, not the repo's programs: C(h) = 7, 10, 15, 20, 25, 30, 35 for h = 3..9, i.e. 5h − 10 for 4 ≤ h ≤ 9).

## Verdicts

**Q1 CONFIRMED.** Win/survival matches 3-question.txt:5 (a height ≥ h after any step (1) or (2) of any round ≤ m). Showing no line survives through step (1) of round m is sufficient, so P (PROOFS.md:22) is the right position. Firings before P are in rounds 2..m−1, round 1 forced pass, f = 5H − 7 − p (:24-26). ΔQ per firing = 2, add at 0 leaves Q unchanged (:28-29). Identity checked at x = 0, ±1, ±2, |x| ≥ 3. Q(P) = 4m − 10H + 4d₀ + 3D + E = 10H − 20 + 4d₀ + 3D + E; equating with 10H − 14 − 2p gives (A) (:39).

**Q2 CONFIRMED.** A3: E ∈ {0} ∪ [5,∞); E ≥ 5 forces 2p+4d₀+3D ∈ {0,1}, so 0, E = 6, not a sum of 5,12,21,…; then D even and the three solutions are exactly (I)-(III) (:41-46). A4: first firing at 0; at x1+1 no adds and no firings, η = f_{x1} ≥ 1, E = 0 ⇒ x1 ≤ 1; the four formulas and f₁+f₋₁ = 2H − 5 + d₀ + D follow from (*) (:48-57). A5: (0,2,0) → fire → (1,0,1) → add → (1,1,1), forced pass; round 3 ≤ m−1 since m ≥ 10 (:59-62). (I): f₁ = f₋₁, sum odd. (II): rounds m−2, m−1 ≥ 4 are firing rounds at {−1,0,1}; backward trace H,H−2,H → v = ±1 gives H+2, v = 0 gives H−1,H,H−1 → H−1,H−1,H−1 → any v′ gives H+1 (:67-74). h ≥ 4 enters through m−2 ≥ 4 (and m−1 ≥ 3). Consistent with `tight/out_C.txt:1-11` (5h − 10 for h = 4..12, 7 at h = 3).

**Q3 CONFIRMED.** Every sub-case of B1 (:106-126) checked: s ≥ 6; F ≥ 2 impossible; F = 1, k ≥ 4; F = 1, k = 3 (nb = 2, s = 5, M = 6/5, neighbour u has l_u = (3k_u+3)/5 ⇒ k_u = 1, η_u ≥ 1 + F_x = 2); F = 0, k ≥ 2; F = 0, k = 1 (u with k_u ≥ 2, l_u ≥ 7/5 ≥ M ⇒ l_u = M; the nb = 3 sub-case is vacuous, harmless). The rule then fires a ready vertex of largest ready lag = M ≥ 1. 6/5 is the threshold in B1, in Wset (:132) and in case 1 (:144). The rule matches `solver-a/proofs.txt:22-23`.

**Q4 CONFIRMED.** (i) T is non-decreasing, so T_z(t) ≤ T_z(t0) < 6/5 and 5l integer ⇒ l ≤ 1 (:138-139). j ∈ Wset since T_j(t0) ≥ T_j(t+1) ≥ l′_j ≥ 6/5 (needs t+1 ≤ t0, stated). 1a, 1b algebra: (k/(k+1))(B_{k+1}+1) = k(1 + H_n − H_{k+1} + 1/(k+1)) = B_k. Exhaustive by 1/5-integrality. Conclusion max(1, H_n) follows from B₁ = H_n and (i).

**Q5 CONFIRMED.** B3 weights 1,3,1 (total 5), only far adds for |z−y| ≥ 4; the j = 2 and j = 4 case analyses are correct (j = 4: sum 4 ⇒ all neighbour counts ≤ 1, nb ≤ 2; sum 5 ⇒ z* must neighbour three vertices). B4: 7 vertices in [y−3, y+3]; I recomputed all ten rows (H₇ = 2.5929 … H₁₆ = 3.3807; λ; e_j; 4.8,4.8,4.8,4.8,4.8,4.8,4.4,4.6,4.4,4.2) — all match :182-186 and `out_arith.txt:1-10`. Tail: g(10) = 2H₁₇ − 2.4 = 4.479, decreasing since 2/(8+j) < 6/25 for j ≥ 1.

**Q6 CONFIRMED** (with one LOW wording slip). η ≤ η* + 2l_y as neighbour lags ≥ 0; η* = (t−e)/5; +1 after an add gives (t+28)/5; m ≤ 5h − 29 ⇒ h − 1/5 < h (:194-200). The rule uses only k, F of the play so far (ties may use the list; the Firer knows it). Non-trivial (≥ 2) from h = 6; it strictly beats 2h − 1 from h = 10 (PROOFS:88 correct); at h = 9 both are 17, so "2h − 1 is better for h ≤ 9" is off by one.

**Q7 CONFIRMED.** `out_rule_main.txt:7`: 89,243 plays (86,180 exhaustive incl. both tie rules + 3,000 random + 60 greedy + 3 files — I re-added the counts), largest unready top lag 1, max 5η − t = 12 after a round (t/5 + 2.4) and 16 after an add (t/5 + 3.2), top-k bound tight (slack −0.0, float). Wset in the test is recomputed per round (test_rule.py:76), i.e. B2 with t0 = t only — as PROOFS:203 says. Controls in `out_rule_controls.txt` each VIOLATE; `out_arith.txt` controls FAIL. Page descriptions (PROOFS:202-206, README:51, README:184-191) are accurate.

**Q8 CONFIRMED with LOW defects.** Short answer (README:13-23) correct: C − G ≤ 18 for h ≥ 4, and C − G ∈ {0,0,2} for h ≤ 3, so "whatever h is" holds. New rows 50-51 accurately paraphrase Parts A and B; row 52 (5h − 6, h ≥ 2) remains true. No remaining "170"/"best"; "176" survives only in historical rows/bullets. Top-level row 25 (README.md:56) accurate.

## Findings

1. **LOW**, high confidence — README.md(25):15 "(with 2h − 1 the better lower bound for h ≤ 9)". At h = 9 the two coincide (17). Replace: "(2h − 1 is the better lower bound for h ≤ 8; the two agree at h = 9)".
2. **LOW**, high confidence — README.md(25):53, 55, 57: the √-bound, 5h − 176 and log-loss rows are now dominated for every h (5h − 28 ≥ 5h − 26 − 10·H_15h always) but, unlike row 52, are not marked superseded; row 57 still advertises being "better than 5h − 176 … and better than 2h − 1 from h = 28". Append "(superseded by the 5h − 28 row)" to the bound statements of rows 55 and 57 (row 53's lower bound likewise, keeping its role in the limit row 54).
3. **LOW**, medium confidence — README.md(25):190 "random search for a list shorter than 5h − 10 at h = 6 and 7 found none": the search was length 5h − 11 only, on 3 to 6 neighbouring points (`out_search_h6.txt:1-3,21-22`). Replace: "its random hill-climbing over lists on 3 to 6 neighbouring points found no winning list of length 5h − 11 at h = 6 and 7, which is weak evidence".
4. **LOW**, high confidence — tight/test_rule.py:2 docstring says "lower bound G(h) >= 5h-32"; replace with "5h-28".
5. **LOW**, medium confidence — README.md(25):164-167 ("The limit rests on one solver's proof … also follows from Corollary U2") could add "and from `tight/PROOFS.md` Part B", which needs readiness only from lag 6/5 and whose B1/B2 were exercised.
6. Note (not a defect I can check) — README:50 "the organiser checked every step" is a process claim with no artefact in the tree.

No CRITICAL or MODERATE defect found: both proofs close as written.
