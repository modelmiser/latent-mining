<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — NOTE.md:64-101 matches the round-by-round invariant in solver-b/proofs.txt:18-34, and the shadow in NOTE.md:92-106 is the adaptive fix in README.md:108-118 — none
Q2: CONFIRMED — NOTE.md:128-152: (a) cost ≥ 1+h(o); (b) the other slot’s reachable set is untouched because o′x points at o′; (c) h(y)≥p′+1 is unchanged; free (out-degree 0 or 1) has P=−1=p and cost 0, and the three cases leave {Q,h(y)} or {P,h(y)} — none
Q3: CONFIRMED — NOTE.md:154-174: nesting A_{k+1}⇒R_k⇒A_k; F(2k)=k+1/3=i/2+1/3, F(2k+1)=i/2+1/6; gaps 1/3 and 2/3; (*) holds with equality on A_k and on R_k via (X1), and strictly on (X2) — none
Q4: CONFIRMED — NOTE.md:204-218 recomputes: the cleanup losses reindex to n/3−F(i_m)−Σ_{r<m}(F(b_r)−F(i_r)), each ≤2/3, then i_m≤2log₂(n+1)−1 and m−1≤i_m give n/3−(7/3)log₂(n+1)+5/6 — none
Q5: CONFIRMED — NOTE.md:227-237 matches solver-b/proofs.txt:56-62 (2(b+1)≥n+1); small cases match README.md:44-50; NOTE.md:258-271 stays inside open-limit/proofs.txt Theorem 3 and Section 8, including note N3 — none
Q6: CONFIRMED — lean/FO/Main.lean:9 and lean/FO/Spec.lean:86-87; n=25 does not force T≥1, n=26 does (⌊log₂27⌋=4, 29≤3T+28); n=1000 gives 940≤3T so T≥314, and 1000−log₂(1001)≈990.03 so T≤990; axioms in lean/AXIOMS.txt:1; Spec.lean is 89 lines — none
Q7: REFUTED — NOTE.md:390-392: a proof of L=1/3 is not shown to need more than one vertex, and a disproof is not shown to either; open-limit/proofs.txt:125-128 and N3 only cap the Lemma 2 game, and Section 8 only kills the potential g. The figures are right: the bound is negative at n=33 and positive at n=34, 0.558 matches solver-b/proofs.txt:63, and 1/3≤L≤1 follows from Theorems 1–2 — "Whether L=1/3 is open. The potential g does not prove L≤1/3 on real forests, and Lemma 2 one-witness arguments do not prove L>1/3."
Q8: REFUTED — NOTE.md:327-328 states O(1) amortized flips at out-degree 3; prior-art.md:8 records O(log log n) w.h.p. NOTE.md:340-341 drops the "Ω(log n)" in prior-art-deep.md:25. NOTE.md:350 says Chaudhuri and Grove are known only through [6]; prior-art-deep.md:15-16 records Chaudhuri as read — "Bender et al. get O(log log n) flips per insertion at out-degree 3 with high probability (prior-art.md). The tree example is recorded as not giving an Ω(log n) worst-case lower bound from the empty graph. Chaudhuri was read; Grove was not."

F: MODERATE — NOTE.md:390-392 — Open question 2 presents two necessities Section 5 does not prove; a one-witness analysis other than g, or a one-witness adversary that does not use only Lemma 2, is not ruled out — high — replace with the Q7 wording.
F: MODERATE — NOTE.md:327-328 — The O(1) sentence contradicts the only Bender bound stored in prior-art.md:8 — high — use the Q8 wording.
F: MODERATE — NOTE.md:340-341 — "does not give a worst-case lower bound" is stronger than the recorded "does not imply an Ω(log n) worst-case lower bound" (prior-art-deep.md:25) — high — restore the Ω(log n) qualifier.
F: MODERATE — NOTE.md:348-351 — "only through [6]" covers Chaudhuri, whom prior-art-deep.md:15-16 says was read; Grove is the unread one (prior-art-deep.md:20) — high — split the two citations.
F: LOW — NOTE.md:329 — prior-art.md:9 calls the folklore rule a directed path to out-degree ≤1, not a random walk — medium — "the path-flip rule, which [3] calls folklore".
F: LOW — NOTE.md:51-53 — 2^{k+1}−1 needs the unused fact that each saturated vertex has two out-edges — high — add that count.
F: LOW — NOTE.md:208-212 — the jump from F(b_{r+1})−F(i_r) to Σ(F(b_r)−F(i_r)) is correct but not written — high — insert the reindex.
F: LOW — NOTE.md:242-244 — superadditivity does not say why flips into the finished part cannot spoil the second game — medium — add that those flips only add payment.
F: LOW — NOTE.md:298-299 — the integer bound forces T≥1 on 26..30 and from 33 on, and not at 31 or 32 (README.md:166-168) — high — add that exception.
F: LOW — NOTE.md:343-344 — "(α = 1)" is not in the quotation recorded in prior-art-deep.md:26 — medium — move it outside the quotation.
