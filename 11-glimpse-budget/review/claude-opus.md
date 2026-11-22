<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1–Q6 hold: I traced every proof step myself and found no gap in §3 or §5. Every defect I found is in the README's description of the work (Q8).

Q1: CONFIRMED — The update order in `solver-a/proofs.txt:10-12` matches `chain/3-question.txt:6-8`. The cop's vertex is removed at both captures (S\{c'} and N[S1]\{c'}). The D answer is S∩(branch through a neighbour), non-empty, and the adversary may give any consistent answer. Finite state space gives a bounded N. The step from adversary to a single robber is stated more fully in `solver-b/proofs.txt:11-14` (backward choice) — none

Q2: CONFIRMED — `proofs.txt:37-40`: N[v]\{y} lies inside subtree(w) because parent(v)=y when v=w. The recurrence Sit(m) ≤ 1+Sit(m−2), with Sit(1)=Sit(0)=0, gives ⌊m/2⌋ for odd and even m. At m=2, w is a leaf, so the robber is caught. The Sit(1) sweep (:33) is correct. `solver-b/proofs.txt:29-37` agrees — none

Q3: CONFIRMED — `proofs.txt:49-59`. I checked (W) at height ≥3, at height 2 (parent via (W), child via (K) at t−1) and at the root (height m ≥3). In (K), the neighbours of the children are not leaves, so r1 ≤ t−2. Then q2 ∈ S_{r1+1}, and both the r1=t−2 case and the later case close. Nothing is assumed about the start beyond S_0=V\{c_0}. At :56, "that leaf" is impossible because r1 is the last time — none

Q4: CONFIRMED — `proofs.txt:62-73`. Entry to X is only through v, so depth(c_t) ≤ t−1. I checked the depth bounds in (F) and (W). (K) starts at S_d/S_{d+1}; for t=d+2 it closes at once. This holds for any rooted ambient tree with X the full descendant set of v and m ≥3 — "ambient rooted tree" should be said

Q5: CONFIRMED — `proofs.txt:90-107`. The sibling is used only in Lam(0) with m=2, to build Y=T_3 for Projection (:94). There π(c) ∈ Y\X gives 8 states. In the step:
- **Cop outside X:** u=v1, by (F)/(W).
- **Cop at v:** u is a grandchild, depth 2 ≤ t+1 since t ≥1.
- **Cop in subtree(v_i):** u is the other child.

In every case u ∈ S_t, dist ≥2, the cop is outside X_u, a same-height sibling exists, and m′ ≥ m−2 ≥ 2j. Both Theorem cases check out (h−2 ≥ 2k and h−1 ≥ 2k). The Theorem leaves the no-glimpse case implicit — none

Q6: CONFIRMED — Projection (`proofs.txt:21-29`) holds for E and D. In §4 (:79-87), R_t ⊆ S_t∩X is non-empty by the Corollary since m−1 ≥3, so "towards X" is legal. Monotone plus Projection then gives a contradiction with L(m−1,j−1). B's L1 (`solver-b/proofs.txt:39-53`) is a correct, distinct argument (free outside glimpses, k−1 budget) — none

Q7: CONFIRMED (with caveat) — I ran `check.py lose_D_h3_k1.txt`: ACCEPT, 250 states. B's `wit/D_h3_k1.txt`: OK, 334 states. I ran `lemma_check.py` part 3 on the 8 starts ([1]+sub(3)): a closed set of 143 states, never empty. No saved output of `lemma_check.py` exists in the folder. The README table matches all three reports — save the lemma_check output

Q8: REFUTED — `README.md:44` says the referee "tested every lemma on 1.9 million states". 1.9M is the W_seed count only (`reports/referee.md:21`). Projection was tested on about 24k states (`verifier/proj.log`) and Lam on seeds for h ≤ 8. The narrative sections also simplify the proofs (see F lines) — "tested W_seed on 1.9M states, Lam on all seeds for h ≤ 8, Projection on random states"

F: MODERATE — README.md:44 — "tested every lemma on 1.9 million states" overstates the testing: 1.9M applies to W_seed only; Projection was tested on about 24k random states, Lam(j) only for h ≤ 8, j ≤ 3 — high — use the wording in Q8.
F: MODERATE — README.md:92-93 — Says the seeded blind lemma "keeps the induction going". At m′=2 (reached from m=4, j=1, cop at v) W_seed does not apply (it needs m ≥3). That step rests on the 8-state computation plus Projection and the sibling subtree, which the README never mentions. — high — add "except at height 2, where the 8 computed states on T_3 (via the Projection Lemma and the sibling subtree) take over".
F: LOW — README.md:44 "Where" — Lists §3, §5, but the e lower bound also uses §1 (Projection) and the MONOTONE remark in §0 — high — "§0, §1, §3, §5".
F: LOW — README.md:61-63 — "subtree of the second vertex on the path" is ambiguous. Counting x as first, that is y, but the proof uses w, the third vertex (proofs.txt:37-39). With y, "strictly below it" gives only one level — high — "inside the subtree of the vertex two steps down the path; after the second step she is caught or strictly below that vertex".
F: LOW — README.md:74-75 — The single "re-enter from a neighbour" sentence hides the timing argument (K) needed at height-2 vertices, which is the non-obvious part — medium — "…; at a height-2 vertex the re-entry comes from a child, and showing one is still possible needs a timing argument".
F: LOW — README.md:88-91 — Omits that Ψ needs a sibling subtree of equal height — medium — add "with a sibling subtree of the same height".
F: LOW — solver-a/ (no log) — The output of `lemma_check.py` is not saved, although proofs.txt:96 cites it as the base computation — high — commit its output.
F: LOW — README.md:114-121 — Omits a point from prior-art.md:22-24, 53: the pathwidth of T_h is ⌈h/2⌉, which resembles e(h). An expert reader would want to know — medium — add one sentence.
F: LOW — proofs.txt:56 — "that leaf or q1" should read "q1" (already noted by the referee) — high — "c_{r1+1}=q1".
F: LOW — proofs.txt:104-107 — The Theorem does not state the never-glimpse case (it is covered by the Corollary) — high — add "(no glimpse: Corollary)".
F: LOW — lemma_check.py:56,65 — The code says "Lambda+_0" where proofs.txt says Lam(0) — low — rename.
F: LOW — proofs.txt:62 — The lemma says "ambient tree" but means a rooted tree in which X is attached only through v — medium — "rooted ambient tree".

**Not asked about:**
1. Everything rests on the belief-set reduction. The best check of it is the referee's trajectory test, which covers only T_3 and T_4.
2. W_full(K) is checked by machine only in the referee's code; A's own checker skips clause K.
3. B's `bad/E_h3_k1_wrongmove.txt` is a "broken" test file that is actually a valid strategy. It is still in the folder, unrenamed.

The headline (h*=3, d=h−1, e=⌊h/2⌋) stands.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
