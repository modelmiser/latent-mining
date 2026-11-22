<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED, with one false statement — `solver-b/proofs.txt:14-23` proves, for one fixed sequence of named pairs, that cost(L) + d(D_L, D_A) ≤ cost(A) holds round by round. I traced both cases. Case 1 is the triangle inequality, so flips outside the merging trees or beyond a path only add to the arbitrary play's cost p. Case 2's walk is sound: each saturated w_i keeps at most one of its D_L out-edges in D_A′. Line 12 also claims "against every Adversary strategy … cost(L) ≤ cost(A)", which is false for adaptive Adversaries (counterexample in F1). The conclusion T = T_normal still holds: run L against an optimal full-game Algorithm, which pays at most T on every sequence. — Replace line 12 with: "For every Algorithm strategy A there is a normal-form L with cost(L on s) ≤ cost(A on s) for every pair sequence s; applied to an optimal A this gives T(n) = T_normal(n)."

Q2: CONFIRMED, but the step is not needed — `reports/open-limit-referee.md:16`. The shadow reply is always available (Lemma 1, Case 2). Pairs are legal because the forest G is the same in both games. The invariant does not depend on how pairs are chosen, so cost(A) ≥ cost(shadow) ≥ c. However, Q1's value equality already gives T ≥ T_normal ≥ c; `open-limit/proofs.txt:5-10` was already correct. The shadow only makes the Adversary strategy explicit. — Replace "one missing step" with "an explicit full-game strategy (optional; Lemma 1 already gives value equality)".

Q3: CONFIRMED — `open-limit/proofs.txt:12-13, 30-36, 45-49`. h(v) = P+1. In A(b), the tree path from o′ to x or to any w_i starts with the edge o′x, which is oriented x→o′, so no flipped edge leaves the set reachable from o′. A free reply needs P = −1, which forces p = −1 and cost 0 = p+1. A path reply through slot P costs ≥ P+1; through slot Q it costs ≥ Q+1. A reply that is unavailable only removes Algorithm options. — none

Q4: CONFIRMED — `open-limit/proofs.txt:60-62, 69-71`. A_k gives cost ≥ k and type R_k in both cases (sorted pair (k−1, k)). R_k gives cost ≥ k with A_(k+1), or cost ≥ k+1 with R_k. In (*), the first two cases are equalities and the third has slack 1/3. For singletons, P = 0 + 1/3 − 1/3 = 0. The step P(new) ≥ P(X)+P(Y) follows directly. — none

Q5: CONFIRMED (except the 189 remark) — `open-limit/proofs.txt:72-80`. I redid the telescoping: it gives F(i_m) + Σ(F(b_r) − F(i_r)), and each difference is ≤ 2/3. Also i_m ≤ 2·log₂(n+1) − 1, m ≤ i_m + 1, and F(2k+1) = i/2 + 1/6. The algebra gives exactly +5/6. Recomputed bound: −3.37 at n = 16, 8.11 at n = 64, 67.49 at n = 256. It is positive exactly from n = 34. It first exceeds B(n) at n = 179 (also 183, 186, 187), and for every n ≥ 189. — `README.md:52`: "exceeds B(n) for every n ≥ 189 (first at n = 179)".

Q6: CONFIRMED — solver-b Lemma 2 (all cases traced); `solver-a/proofs.txt:36-54, 56-60, 70-86`; `solver-b/proofs.txt:46-52`. The levels-0..k count gives 2^(k+1) − 1 vertices. The induction uses 2(b+1) ≥ n+1. The inequality min(p,q) ≥ min(j,p) + min(j,q) − j holds. The certificates `cert_W2_14_ge4.txt`, `cert_W2_16_ge5.txt` and `solver-b/cert_n30_k7.txt` are present. — none

Q7: CONFIRMED — `open-limit/proofs.txt:94-97`. The merged tree contains both the new witness and the kept one, so the Adversary may track either. My exhaustive check of (***) over all pairs with entries ≤ 13, Algorithm allowed every option, found 0 failures. `README.md:47` adds "in its reduced game", which matches the theorem. The overstatement is in `open-limit/proofs.txt:111-113`, not in the README. — none

Q8: REFUTED in part — see F3–F6. The other numbers check: 30…66, 17…30, 1/3 ≤ L ≤ 1, three programs for n ≤ 15, one program at n = 16, and the prior-art paragraph.

F1: MODERATE — `solver-b/proofs.txt:12` — The claim "against every Adversary strategy … cost(L) ≤ cost(A)" is false. Counterexample at n = 15: the Adversary names {1,2}, then {3, tail}. If the Algorithm ever makes a non-normal reply, the Adversary plays harmlessly (attach singletons, free). Otherwise it plays an optimal 12-vertex game on the remaining singletons, which forces ≥ 2. An Algorithm that answers round 2 with 3→1 plus flipping the first edge pays 1 in total; every normal Algorithm pays ≥ 2. The proof establishes only the per-sequence version. — confidence: high — fix: per-sequence statement plus the "optimal A" corollary (Q1).

F2: MODERATE — `README.md:45,91-104,110-113`; `reports/open-limit-referee.md:7,15` — These present the shadow step as a missing step that the n/3 bound needs. It is redundant: Lemma 1 already gives T = T_normal, and `open-limit/proofs.txt:5-10` stands as written. — confidence: high — fix: call it an explicit construction, not a gap repair.

F3: LOW — `README.md:52` — "exceeds B(n) only from n = 189" is wrong; the bound first exceeds B(n) at n = 179. — confidence: high — fix: wording in Q5.

F4: LOW — `README.md:110` — "Everything above the small table rests on the normal form" contradicts line 44. Solver A's route to T(30) ≥ 7 and T(32) ≥ 8 uses P1, P2 and P6, not Lemma 1 (`reports/referee.md:8`). — confidence: high — fix: "everything above the small table except T(30) ≥ 7 and T(32) ≥ 8".

F5: LOW — `README.md:86-88` — "The two solvers … could not turn it into an induction, because they fixed in advance which trees would be merged." Only solver B attempted an induction, and solver B gave a different reason (overpayment by one subtree, `solver-b/proofs.txt:68`). The "fixed schedule" diagnosis is the follow-up author's (`reports/open-limit.md:18`). Solver A reports no evidence that the rate exceeds 1/4. — confidence: medium — fix: "Solver B found the mechanism; its cost-plus-depth induction failed (per the follow-up, because of the fixed 4-ary schedule)."

F6: LOW — `README.md:13-15` — "holds at least to 15 … first failure from 17" silently uses the single-program value at n = 16. — confidence: high — fix: "from 17 (16 without the single-program value)".

F7: LOW — `README.md:39` — "checked by program for every position with n ≤ 11" was a single-round check over merged-tree re-orientations only, which relies on P1 (`reports/referee.md:9`). — confidence: medium — fix: say so.

F8: LOW — `README.md:45` — The "I" in "I followed the lemmas" is unidentified. — confidence: medium — fix: name who.

F9: LOW — `open-limit/proofs.txt:99` vs `open-limit/pairdp_noprune_49_66.txt` — The proofs say the n = 62–66 values come from pairdp.py alone. The noprune log does stop at 61, but the referee's own DP covers 62–66 (`open-limit-referee/run_t3_true.txt`), so the README's "two programs" is right only by counting the referee's. — confidence: medium — fix: say which two.

Not asked:
- Corollary 7's "an optimal Algorithm must let h grow" (`open-limit/proofs.txt:136`) holds only for normal-form Algorithms. The referee says so (`reports/open-limit-referee.md:39`); the README does not.
- `open-limit/proofs.txt:111-113` overstates Theorem 3 beyond the pair game.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
