<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

**Verdicts**

Q1: CONFIRMED — open-growth/proofs.txt:12-26. I checked C1's prefix/offset count with its surplus refinement, C2's loss formula (k windows lost, k−1 gained) and its doubling formula (k−2 lost, k−1 gained, because the old window at c survives at the copy), the k=3 contexts at :22, and C3: N_01 = N_10 = 2^(n−2) + a, and the e surplus windows split between the two counts, so 2a ≤ e. — none

Q2: CONFIRMED — :35-57. (E1): an overlapping alternating union has ≥6 interior singletons; disjoint occurrences have ≥5 each. (E3): N_00100 ≥ 4, and one altered run spoils at most 2. Fact B (−3 / +1) and Fact A are used within their hypotheses (solver-b/proofs.txt:9-29). Step 2 uses C3 at lengths 2^n, 2^n+1, 2^n+2 correctly. — none

Q3: CONFIRMED — :59-89. I re-derived the excess vector from (*). My own enumeration (pass, all 32 loss contexts, all 8 doubling contexts) gives: Step 4, only (t,z)=(0,0) survives; Step 5, only doubling the 0 of 101; Step 6, only losses 01001/10010/10011/11001, each removing a 0 of 1001. "De Bruijn plus alternating pair" is imprecise (see F4). — none for the maths

Q4: CONFIRMED — :88-96. Runs of length ≥3 are untouched, so Z has length n. s* comes from D and neither operation touches it. The three cases give n, n+q ≥ n+3 or p+n, never n+2. No step assumes w universal (only w'' and u0). The proof holds for all n ≥ 7. — none

Q5: CONFIRMED — :117-129. G2 uses only the pass move. G3's sum is over the distinct words in F_s and subtracts exactly the k through-windows, so repeated words are handled. G3 holds for any cell. — none

Q6: CONFIRMED — :134-157. I re-derived the equations, the reversal d→1−d, the d=0 and d=1 exclusions, the k−d values of p, and 2^(m−1) or 2^(d−1). The referee's repair (period d on [p, d−1]; cells −1 and 0 fall in different classes, so the condition halves) is right (reports/open-growth-referee.md:30). The repeat term is really k−2 per repeat cell, so the stated bound is loose but valid. Nothing else is missing. — none

Q7: CONFIRMED — :158. With m=4 the exact sum term is 2^(n−3)+16k−48 ≤ 48(k−2)+2^(n−3). Expanding with k=n−4 gives exactly 2^(n−3) − 19n² + 117n − 64. Since 19k−2 < 19n and 117n−64 > 0, the closed bound follows, and it is trivially true when its numerator is negative. Recomputed at n=15, k=11: (8192−2−4224−280−1870)/207 = 1816/207 → 9. bound.out also gives 418 and 272775. "Unbounded" follows. — none

Q8: REFUTED (README only) — P5 (solver-a/proofs.txt:42-61) and solver B's Theorem 1 are correct. The 76-loop is at solver-b/witnesses.txt:12; I verified it from the definition myself (universal, all 153 adversary moves answered). But README:33, :65, :105 and :121-122 misstate what the proofs say (findings below). The prior-art paragraph matches prior-art.md. — see F lines

**Findings**

F: MODERATE — README.md:33 — says the excess grows "at least like 2^n/(19n)". The proved rate is 2^(n−3)/(19n) = 2^n/(152n), so the README overstates it by a factor of 8 (proofs.txt:113 has it right) — high — "growing at least like 2^n/(152n)"

F: MODERATE — README.md:65 (also proofs.txt:30) — says length 66 was "computed three ways". Solver B's search covers only universal w (solver-b/witnesses.txt:29: "L=66, w NOT universal: NOT excluded"). So only two computations cover the full claim: solver A's, which rests on P8 pruning validated only empirically, and the referee's, which rests on Lemma 5 plus the referee's own run-count lemma — high — "computed twice in full (solver A's pruned search; the referee's enumeration via Lemma 5), and for universal loops a third time by solver B"

F: LOW — README.md:120-123, proofs.txt:162, reports/open-growth.md:25 — say the junction coincidences number "fewer than 2^(n−3)", leaving "at least 2^(n−3)" lone bits. In fact the d-sum alone is 2^(n−3)+16k−48 > 2^(n−3), and there is a further repeat term 2(k−1)(e+n−1). The formal proof is unaffected — high — "about 2^(n−3) + O(n·e) of them, which leaves about 2^(n−3) − O(n·e) lone bits"

F: LOW — README.md:105 (also proofs.txt:86-87 and :102, open-growth.md:23) — "a de Bruijn cycle plus one alternating pair". Step 6 proves a wider class: a 0 added at one place and a 1 inserted into a different 00 elsewhere (the test's family2). The n=4,5 structure check tested only the local form — high — "a de Bruijn cycle with one 0-singleton lengthened and one 1 inserted into a 00 run (for n = 4, 5: one alternating pair inserted)"

F: LOW — README.md:26-27 — "A mechanic may then make one edit of the same kind" contradicts chain/3-question.txt:10, which allows either kind — high — "one edit of either kind"

F: LOW — README.md:68 and :134-135 — "says nothing below n = 15". The displayed closed formula is also vacuous at n=15 (its value is negative, first positive at n=16). Only (**) with the best k gives e ≥ 9 at n=15 — high — "(**) says nothing below n = 15; the closed formula nothing below n = 16"

F: LOW — proofs.txt:113 — "first non-trivial at n = 14", but e ≥ 1 there is weaker than Theorem 1's e ≥ 3 — high — "first improvement on Theorem 1 at n = 15"

F: LOW — README.md:94 — "216 + 20 such loops" mixes strings (n=4) with necklaces/normalised words (n=5) — medium — state the units

F: LOW — proofs.txt:149 — the repeat-cell term is k−1 where k−2 is proved; valid but loose — high — use k−2

**Not asked about**

1. Theorem 1 at n=6 and the "M(n) ≥ 2^n+3 for n ≥ 6" row depend on the L=66 computation. Solver A's k ≥ 2 runs never reached a leaf, so they rest entirely on P8 (solver-a.md:24). The referee's route also depends on its own 34-run lemma, which is not written up as a proof.
2. Lemma 5 is trivial, yet the README presents it as a structural result that the computations share.

**Bottom line:** I found no gap in the mathematics of Theorems 1 and 2 or of C1, C2 and C5. The defects are in the README's wording, and the 2^n/(19n) rate at README.md:33 is wrong by a factor of 8.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
