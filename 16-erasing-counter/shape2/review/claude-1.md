<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1 — experiment 16, Theorems NC and T

Paths are under `16-erasing-counter/`. P = `shape2/PROOFS.md`. I traced every proof by hand and wrote my own simulator and checkers from `chain/3-question.txt` (not the repository's programs).

## Verdicts

**Q1 — CONFIRMED.** P:18-40. T(q,K) is legal (z_(q+1) occurs once on each path; every leaf writes an index of its own path). The four step rules follow from the definition; (q+2)c distinct states. L: ℓ_1 = 2, ℓ_2..ℓ_q = 1, ℓ_(q+1) = L(K) gives 2q + 4L(K) by (U). Arithmetic at P:43-44 correct. My simulator: `cex.txt` A 60 > 56, B 52 > 50, C 36 > 33, D 36 > 32, all legal, depths 5, 5, 6, 7; `root1.txt` legal, depth 4, longest cycle 258, 126 states with the root bit set.

**Q2 — CONFIRMED.** P:73-136 case (B) is Theorem Z1 (`shape/PROOFS.md`:80-128): H ≤ c_1···c_p, n_j ≤ c_j···c_p is N ≤ 2C_1 + C_2 + … + C_l. Case (A) gives 2^k, weaker than Lemma A's k, harmless. Steps 1–5 sound, including self-jumps and recurring modes. Lemmas S, V and Corollary MD-r check.

**Q3 — CONFIRMED.** P:189-225. Disjointness is replaced by Step 0: ch_i ⊆ R_i, so with (NC) a bit of R_i moves only at mode-i steps. Every data node a mode-i step visits is in R_i by definition, so the walk is that of G_i^R on y_i and y_i moves by g_i; periodicity puts it on one cycle. Theorem E is applied to G_i^R (a legal tree: P:198-200). The R_i may overlap, so the y_i are not independent coordinates, but Step 5 only needs state ↦ (y_i) to be injective within a mode, which it proves. Corollary NC-r and the depth-5 consequence follow; they rest on the computed D(ρ) = W_ρ, ρ ≤ 4, and on the no-home case (P:83-87 or Lemma A). My test: 8,811 qualifying cycles in 5,294 random trees with shared positions (238 with R_i ∩ R_j ≠ ∅), 0 failures; with the (NC) filter dropped, 72 failures in 1,325 trees.

**Q4 — CONFIRMED.** P:256-314. A prefix mode cannot read or write tail data ((MD_i); a leaf writes only its own path). A tail leaf writing z_m, m ≤ i, is handled: empty in Θ*, hence the single overflow ε of C_Θ by Theorem E applied to Θ* (the right tree; legal since j* > i). (EC) is what makes every entry the one λ0-step of Θ*. Counting checks. My test, all splits i: 8,073 qualifying cycles (2,528 with the tail in the chain, 211 with a prefix unit before it), 0 failures; without (EC) 16 failures, without (MD_i) 875.

**Q5 — CONFIRMED.** Lemma T (P:316-330): identity W_r = ωP_i + ΣP_n correct; all three cases traced. By hand r = 2, i = 1: chain (Θ,1) gives 6 = W_2 (tight), (1,Θ) gives 5. K\*\* is a monotone inductive definition, well defined; contains Theorem Z's class via rule (2); contains Theorem RD's trees only when T0°, T1° are in the class (all of them at depth 5). See F3, F4.

**Q6 — UNSUPPORTED in two places** (F1, F2); counts all match: 20,592 / 15,121 (`out_nc.txt`), 19,934 / 82,420 / 39,896 / 3,862 / 11,294 (`out_tail.txt`), 35,387 / 165,091 (`out_lemma_t.txt`), 258 / 126 (`out_root1.txt`), 83,005 = 41,325 + 41,680 (`out_md.txt`). The Lhat sentence matches P:362-369.

## Findings

**F1. MODERATE (confidence 0.85). P:354-357 and README:153.** Case (i) is stated at cycle level ("exchange information through a data bit") "for every split i ≥ 1". Theorem T's hypothesis (MD_i) is tree level (index sets disjoint). For i < k, a tree whose S_m (m ≤ i) merely shares an index with another subtree, with no exchange on the cycle, is covered by no theorem in the file: NC needs no exchange between *any* two modes, tail pairs included. The claim needs a Theorem T under an NC-style cycle hypothesis, which is not written. (It is probably true: my checker for that hybrid — R and ch per unit, tail pruned to what tail steps read — passed 24,512 cycles in trees violating (MD_i), 0 failures; failing without the filter.) Replacement for P:354-357: "Not covered: for a split i, (i) some S_m, m ≤ i, shares a data index with another S_m' or with the tail, or (ii) the tail is entered from outside at two different modes; and, by Theorem NC, on the cycle some two modes exchange information through a data bit. A cycle-level form of (i) (Theorem T under an (NC)-type hypothesis) is not proved here." README:153: "what remains is a subtree near the top of the path sharing a position with another, or the lower part …".

**F2. MODERATE (0.8). README:153 and P:361.** "so only the total is bounded" asserts the open statement; the total is computed at depth ≤ 4 only. "no bound charged node by node can work" generalises from one refuted bound. Replacement: "It also showed that the natural per-node bound fails: the number of states with the root bit set can exceed the Sylvester value of the next depth (126 against 42 at depth 4), so a bound, if there is one, can only be on the total." P:361: "Only the total can be bounded."

**F3. LOW (0.8). README:66 and P:346.** Without rule (1) the class K\*\* exceeds Theorem Z's class only by trees whose tail is never entered through home (if Θ\* is in Z's class and (MD_i) holds, then (MD) holds and every G_j is in Z's class). Every non-trivial new tree, including the example at P:346-348, rests on the depth ≤ 4 computation. Add to the README row: "(beyond trees with an unreachable tail, the enlargement uses the computed values for depth ≤ 4)".

**F4. LOW (0.9). P:334.** "Unconditionally for r = 5, by the repository's D(ρ) = W_ρ" → "For r = 5 the hypothesis is the repository's computation D(ρ) = W_ρ, ρ ≤ 4."

**F5. LOW (0.95). `shape2/out_root1.txt`.** The file holds four trees and ends in a Python traceback (AssertionError); `root1.txt` has one tree. It was produced from another input (the last three lines are the trees of `cex_mut.txt`, whose fourth is illegal). Regenerate from `root1.txt` alone.

**F6. LOW (0.9). P:70-71.** "all trees of §1 when K ∩ nothing is shared" is garbled; T(q,K) always satisfies (MD) (D_1..D_q = ∅). Replace by "and every tree T(q,K) of §1".

**F7. LOW (0.7). README:65-66.** Row NC omits that the cycle passes through home (modes are undefined otherwise); row T says jumps "from them" but (EC) also covers the write of the all-zero leaf. P:231 "By induction on r" is not an induction: "By the same argument at the first depth r with D(r) > W_r".

Nothing in the four rows claims D(r) = W_r; apart from F2 the page keeps it open.
