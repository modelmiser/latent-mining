<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Verdicts**

Q1: CONFIRMED — `solver-a/proofs.txt:9-16` (S1=S\{c'}, S'=N[S1]\{c'}, E answer {v}⊆S, D answer S∩branch(c,u)≠∅, start (c,V\{c},k)) matches `chain/3-question.txt:3-8`; same update in `solver-a/check.py:13-25,52-63,86-92` and `solver-b/proofs.txt:4-15`; finiteness ⇒ bounded N; referee trajectory test `verifier/reduction.log` 0 mismatches — none.

Q2: CONFIRMED — `solver-a/proofs.txt:35-40`: (b) robber in subtree(y), cannot pass y ⇒ Sit(m−1); (c) N[v]\{y}⊆subtree(w) for depth≥2 (incl. v=w), step to w ⇒ Sit(m−2); odd m ends at Sit(1) (line 33), Sit(2) = 1 glimpse; `strat_{D,E}_h2_k0.txt` ACCEPT when I ran `check.py` — none.

Q3: CONFIRMED — `solver-a/proofs.txt:46-59`: t=0 trivial; (W) root falls under height≥3 (m≥3); height-2 case uses parent (height 3) or (K) at t−1; (K): leaf fact (line 45) gives r1>r2 both defined (every leaf is c_0 or in S_0), c_{r1+1}=q1 by lastness, q2∈S_{r1+1}, then r1=t−2 or both q2-leaves survive to S_{t−1}. Only assumption: full start S_0=V\{c_0}, any c_0 — fix line 56 "that leaf or q1" → "q1 (it cannot be the leaf, r1 being the last leaf time)".

Q4: CONFIRMED — `solver-a/proofs.txt:62-73`: entry only through v gives depth(c_t)≤t−1 (line 66); (F) parent at depth t ∉{c_t}; (W) depth ≤ t+1 bound respected in each sub-case, height-2 z≠v since m≥3; (K) leaves in S_{d+1} by (F), cop not on children/leaves at d, d+1, then W_full(K) verbatim with (W) at r1 valid since d≤r1+1. Holds in any ambient tree because X's leaves are T-leaves (leaf fact) — none.

Q5: CONFIRMED — `solver-a/proofs.txt:90-107`: sibling needed only for Lam(0), m=2 (lines 94-96, π(c)∉X since nothing hangs below X or the sibling, then MONOTONE); each of the three answers is in S_t by W_seed(W) (depth 1 or 2 ≤ t+1, height ≥2 as m≥4), dist≥2, c'∈N[c_t] never in X_u, new sibling is v2 / other grandchild / v_i, m'∈{m−1,m−2}≥2(j−1)+2; Theorem both cases via W_full; verified mechanically `verifier/lemmas.log`, `verifier/lam.log` (m=2,j=0: 0/16 wins) — none.

Q6: CONFIRMED — `solver-a/proofs.txt:20-29` (R\{π(c')}⊆S\{c'} in both c'∈H / c'∉H cases; D answer R∩branch_H ⊆ one T-branch); `:79-87`: X in one branch of w_t, S_t∩X⊇R_t≠∅ by Corollary on T_{m−1}≥T_3, so answer legal; post-move X-state is blind-reachable from full start π(w_0) ⇒ contradicts L(m−1,j−1). `solver-b/proofs.txt:39-53` L1 independently sound (forced answer outside Q, identical answer inside Q) — none.

Q7: CONFIRMED — I ran `solver-a/check.py` on `lose_D_h3_k1.txt`: ACCEPT, 250 states; referee `verifier/wcheck.log:13,23` (A 250, B 334); `lemma_check.py:56-65` run by me: closed set 143 states from the 8 seeds, never empty (no saved output in `solver-a/`; referee's `lam.log` covers it). README table = `reports/solver-a.md:7-8`, `solver-b.md:8-9`, `referee.md:10`, `verifier/table789.log` — add a saved `lemma_check` log.

Q8: CONFIRMED — every Status cell traced (`verifier/reduction.log`, `lemmas.log` h≤8 and 1.94M W_seed states, `wcheck.log`, `table789.log` E h=9); prior-art paragraph matches `prior-art.md:3,28,54`; only wording looseness listed below — none.

**Findings**

F: LOW — `solver-a/proofs.txt:56` — "c_{r1+1} is that leaf or q1": the leaf is impossible (r1 is the last leaf time); the argument needs q1 — high — "c_{r1+1} = q1".
F: LOW — `README.md:47`, `reports/solver-a.md:8` — d(9)=8 rests on solver A's report only; no fast2 h=9 log in the folder — high — "d(9) = 8 reported by solver A, no log kept".
F: LOW — `README.md:44,105`, `solver-a/proofs.txt:96` — "8 states" are seeds; the checked closed set has 143 states, and solver A's run output is not saved (only referee's `lam.log`) — high — "8 seed states (closure of 143), checked by the referee's lam.log and rerunnable from lemma_check.py part 3".
F: LOW — `README.md:73-75` — "re-enter ... from a neighbour he could not also be covering" describes clause (W) only; for height-2 vertices the proof needs (K), a timing argument over leaf visits — medium — add "for vertices just above the leaves this needs a separate argument about when the cop last visited the leaves".
F: LOW — `README.md:86-93` — "a position of the same kind" omits the sibling-subtree requirement of Ψ_m, which is exactly what lets the induction bottom out at the computed m=2 base — medium — add "with a sibling subtree of equal height, so the induction can end at the computed height-2 base".
F: LOW — `README.md:42` — "proved by hand by solver A" understates: blind loss for all h≥3 also follows from B's E-projection (`solver-b/proofs.txt:69`) plus `wit/E_h3_k0.txt` — medium — "proved by A in general; B's projection plus the E(3,0) certificate gives it too".
F: LOW — `solver-a/proofs.txt:62-65` — "height" in W_seed is not redefined for the ambient tree; it works because X's leaves are leaves of T — high — add "height = distance to the leaves of X".
F: LOW — `solver-a/proofs.txt:94-95` — Lam(0), m=2 silently uses π(c)∉X (so the projected belief still contains {v,v1,v2}) — high — add "π(c)∈{parent(v)}∪sibling tree since c∉X".

**Not asked**

- Nothing CRITICAL or MODERATE found; the §5 argument (W_seed → Lam → Theorem) is sound as written and every quantifier I traced closes.
- `solver-a/proofs.txt:37` reuses `t` for depth after using it for time (line 44); harmless.
- Referee's W_seed test (`verifier/lemmas.py:56-85`) starts from S_0 = {v,v1,v2} exactly, not supersets; fine by monotonicity of the update, but worth a sentence.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
