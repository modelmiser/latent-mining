<!-- Archived by the organiser on 2026-11-21. Report of the second Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 2: shape2 (Theorems NC, T), experiment 16

Own checks (scratchpad only, simulator written from `chain/3-question.txt`): trees A, B, C, D of `cex.txt` give legal, depth 5/5/6/7, L 56/50/33/32, longest cycle 60/52/36/36. The `root1.txt` tree gives depth 4 and cycle 258, with 126 states having the root bit set. T(q,K) for three choices of K and q = 1..4 matches both (q+2)c and 2q+4L(K) in all 12 cases (including 36 > 32 at depth 7). I enumerated Lemma T at extreme values for r ≤ 6 (every unit order and τ, c ∈ {1, d}, c_Θ ∈ {1, 2, ω}, H_Θ ∈ {0, 1, ⌊c_Θ/2⌋}; X is monotone/linear in each variable): 1,331,126 cases, max X/W_r = 1.0.

## Verdicts

**Q1 CONFIRMED.** PROOFS.md:18-44. Legality holds (S_1 reads z_(q+1), which is off the path above it, and every write is on the writer's path). The four transitions at lines 33-37 are correct. (U) at line 11 is correct. L = 2q + 4L(K) checked by hand and by simulation. The ratio (q+2)/6 → ∞ is correct.

**Q2 CONFIRMED.** Lines 63-136. G_i = S_i°, (MD) = (H), and H + Σ n_j ≤ 2C_1 + C_2 + … + C_p is Z1's bound. Step 3 is sound: non-overflow implies (β), so the next state is home, and the overflow leaf depends on x_i only. Step 4(b) is sound: the orbit reaches i_j only after i_1..i_(j-1), and every repeat of i_t mid-excursion is again an overflow. Lemmas S and V (144-152) checked.

**Q3 CONFIRMED.** Lines 186-232. Disjointness is replaced by Step 0. A step changes only bits it reads (ch_i ⊆ R_i), and (NC) means only mode-i steps change R_i. Overlapping R_i ∩ R_j bits are therefore constant. The S_i walk visits only Z nodes (read as 0) and R_i nodes, so it is the G_i^R walk on y_i. Nodes pruned for "index ∉ R_i" are never reached. Theorem E is applied to G_i^R. The count (Step 5) is injective into a set of size ≤ the product, because overlap only shrinks it. The depth-5 consequence (229-231) follows. It rests on: §2(A) / [U] Case A (cycles without home ≤ 2^5 ≤ W_5), Theorem E, and the computed D(ρ) = W_ρ for ρ ≤ 4. It does **not** use Lemma A.

**Q4 CONFIRMED.** Lines 238-314, with every gap in the brief traced:
- A prefix mode cannot read tail data (D_m ∩ D_Θ = ∅). It cannot write tail data either: its writes are on its path, which holds only Z ∪ D_m.
- Tail spine bits read by a prefix step are 0 and stay 0.
- The tail cannot touch D_m. It can write z_m (m ≤ i) and re-enter the prefix mid-excursion, but such a leaf is empty in Θ*. By Theorem E on Θ* at most one ε exits to a prefix mode, so a(Θ) is well defined and θ = ε⁺ afterwards.
- Θ* is legal: z_(j*) lies on Θ's own all-zero path, and Θ contains no z_m with m ≤ i.
- Theorem E is applied to the right tree (C_Θ is an f_(Θ*) cycle).
- (EC) is needed so that every entry acts on a Θ-home θ by one fixed map, f_(Θ*) with λ0* = [z_(j*)]. With two entry modes, θ's update would depend on the entering unit, not on θ.
- c_Θ ≥ 2H_Θ holds because f_(Θ*) never maps Θ-home to Θ-home.

**Q5 CONFIRMED** (wording issues below). Lemma T's proof (316-330) is correct. I traced the index bookkeeping (a+1, a+2..q, 1..a, each used once) and the numerical check agrees. K** is well founded: G_m and Θ* have fewer nodes. Rule (4) with i = k is (MD), so K** ⊇ K = 𝒞. The example at lines 346-348 is in K** and not in K*. Its membership needs rule (1), i.e. the computation. "Unconditional at depth 5" is accurate only as "given the depth ≤ 4 computation".

**Q6 REFUTED in part.** The four rows' numbers match the out files (20,592; 82,420; 35,387 / 165,091; 60 > 56, 52 > 50; 258 / 126). The description of what remains open overstates (F1). So does the "node by node" sentence (F2).

## Findings

**F1, MODERATE, conf. 0.85.** PROOFS.md:354-357 and README.md:153.
- Theorem T's hypothesis (MD_i) is a tree-level disjointness condition, not a cycle-level "no exchange" condition. Theorem NC is cycle-level, but it requires no exchange among *all* modes.
- Nothing proved covers a cycle where the prefix modes exchange nothing with anyone, but the prefix data share an index with the tail at tree level, and the tail modes communicate internally.
- So "a cycle longer than W_5 would need (i) or (ii) for every split" is unproved as worded. What is proved: for every split i, (MD_i) fails at tree level or (EC) fails on C; and (by NC) some two modes communicate on C.

Replacement for §6: "(i) for the split i, a prefix subtree shares a data position with another prefix subtree or with the tail (in the tree, not necessarily used on the cycle); (ii) the tail is entered from outside at two different modes. At depth 5 a cycle longer than W_5 needs (i) or (ii) for every i ≥ 1, and also two modes that communicate through a data position (Theorem NC)."

Matching README replacement: "what remains is a cycle on which two modes communicate through a shared position and, for every split of the path, either a subtree above the split shares a position with another subtree, or the part below the split is entered at two different modes."

**F2, MODERATE, conf. 0.8.** README.md:153 and PROOFS.md:358.
- "no bound charged node by node can work" generalises from one refuted bound (root-bit states ≤ W_(r-1)).
- "so only the total is bounded" reads as if the total bound were proved. It is open beyond depth 4.

Replacement: "It also refuted one natural per-node bound: on a cycle of a depth-4 tree, the states with the root bit set can number 126, above the next depth's Sylvester value 42. So an induction through the root cannot charge the root's states separately."

**F3, LOW, conf. 0.9.** README new row 3 says "all jumps from them into the rest of the path land on one mode". (EC) also covers the home step (λ0 writing a tail index). Replacement: "all jumps into the rest of the path, from the all-zero leaf or from those subtrees, land on one mode".

**F4, LOW, conf. 0.9.** `out_root1.txt` is cited for row 4, but it does not come from `root1.txt`.
- It has four trees (`root1.txt` has one), and one of them differs from tree A.
- It ends in a Traceback (AssertionError at `root1.py:7`), with no clean-run marker.

Repair: rerun `root1.py root1.txt` into `out_root1.txt` and put any deliberately illegal run in a separately named file.

**F5, LOW, conf. 0.9.** PROOFS.md:70-71: "all trees of §1 when K ∩ nothing is shared" is garbled. Replacement: "all trees T(q, K) of §1 (D_i = ∅ for i ≤ q, D_(q+1) = idx(K))".

**F6, LOW, conf. 0.8.** PROOFS.md:334: "Unconditionally for r = 5, by the repository's D(ρ) = W_ρ" contradicts itself. Replacement: "For r = 5 this needs only the repository's computation D(ρ) = W_ρ, ρ ≤ 4."

The same applies to Corollary K** (339-342): it inherits rule (1) and should say "uses the computation D(ρ) = W_ρ for ρ ≤ 4". Its strictness example (346-348) depends on that rule.

No defects found in Proposition X, Theorems MD, NC, T, or Lemmas S, V, T.
