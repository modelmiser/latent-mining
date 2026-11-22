<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

Q1: CONFIRMED — NOTE.md:74-90 each step traces (two old out-edges in D_L vs ≤1 in D_A′; w₁ cannot have out-edge w₁x in D_L; forest ⇒ no repeats); Corollary 95-101 matches reports/open-limit-referee.md:16 and Lean `shadow_path`/`transfer` (FO/Full.lean:174,273). Gaps: validity of L's flipped path (solver-b/proofs.txt:32) is not argued; Lemma 1 is stated for a fixed sequence but the Corollary uses it round-by-round under an adaptive S — "Lemma 1 holds one round at a time, so it applies when the next pair depends on D_L; the flipped path is a position because inner vertices keep out-degree 2 and w_d gains one."

Q2: CONFIRMED — NOTE.md:131-152 against open-limit/proofs.txt:40-65; free reply at out-degree 0 gives {−1,h(y)} = {Q,h(y)}, at out-degree 1 gives {h(o′),h(y)} with (b) covering o′. Dropped: "y still has type (p′,q′)" (proofs.txt:59), which Section 5's "keeps … the untouched one" (NOTE.md:261) needs — add that clause to Lemma 2.

Q3: CONFIRMED — F(2k)=k+1/3=i/2+1/3, F(2k+1)=i/2+1/6; differences alternate 1/3, 2/3 (computed); nesting R_k⊂A_k, A_{k+1}⊂R_k; Lemma 3 cases NOTE.md:170-174 match proofs.txt:76-78 — none.

Q4: CONFIRMED — recomputed: Φ(Y_r) inequality follows from Φ(Y_{r+1})−F(b_{r+1})+Φ(X_r)−F(i_r)+cost+F(b_r); telescoping gives n/3−F(b_m)−Σ_{r<m}(F(b_r)−F(i_r)); i_m ≤ 2⌊i_m/2⌋+1 ≤ 2log₂(n+1)−1; n/3−(7/6)(2log₂(n+1)−1)−1/3 = n/3−(7/3)log₂(n+1)+5/6; positive from n=34 (computed) — none.

Q5: CONFIRMED with reservations — Theorem 2 proof NOTE.md:227-237 = solver-b/proofs.txt:56-62; 9-vertex example re-derived by hand (slots of 3 become (1,1), 1+7/3 > 8/3); "needs a different invariant" is within N3's scope. But NOTE.md:252 "36 ≤ n ≤ 63" overstates: T(62) ≥ T(32)+T(30) ≥ 15 > B(62)=14 and T(63) ≥ T(62) from certificates alone — "for 36 ≤ n ≤ 61".

Q6: CONFIRMED — FO/Main.lean:9 statement; FO/Spec.lean is 89 lines; 1953 lines total; AXIOMS.txt lists three axioms; computed: n+3−7⌊log₂(n+1)⌋ first >0 at n=26; n=1000 gives ⌈940/3⌉=314, 1000−log₂1001=990.03 — none.

Q7: CONFIRMED for numbers (gdp.py run: max ratio 0.5579 at n=2816; positive from 34; 1/3 ≤ L ≤ 1) — but NOTE.md:390-391 "a proof would need an Algorithm whose analysis sees more than one vertex per tree" is not a consequence of Section 5 (proofs.txt:154-164 shows one potential fails on one tree, and that potential already maxes over all vertices) — "Section 5 shows that the potential of Theorem 3, lifted to trees, does not give an Algorithm; we have no candidate."

Q8: REFUTED in parts — NOTE.md:318 cites Gupta slides [8] as read, but neither prior-art.md nor prior-art-deep.md mentions them (grep: no hit), contradicting NOTE.md:312 "the search is recorded in"; NOTE.md:350-351 "we know those results only through [6]" contradicts prior-art-deep.md:17 (Chaudhuri et al. introduction read); NOTE.md:327 "[2] obtain O(1) amortized time and flips with out-degree 3" is not in either file (prior-art.md:8 records O(log log n) w.h.p.); NOTE.md:329 "random-walk rule … folklore" vs prior-art.md:10 (folklore = path flip). Novelty claims are properly hedged.

F: MODERATE — NOTE.md:318,312 — source [8] (Gupta slides) and the "3 amortized flips" claim appear nowhere in the recorded search; provenance sentence is false for this item — high — add [8] to prior-art-deep.md or say "read for this note, not part of the recorded search".
F: MODERATE — NOTE.md:350-351 — "known only through [6]" but prior-art-deep.md:17 says Chaudhuri et al. was read directly — high — "Chaudhuri et al. was read in part; Grove et al. only through [6] and [9]".
F: MODERATE — NOTE.md:327 — "O(1) amortized time and flips with out-degree 3" unsupported by prior-art files, which record O(log log n) w.h.p. — medium — state what prior-art.md says or cite where the amortized claim was read.
F: MODERATE — NOTE.md:390-391 — "a proof would need an Algorithm whose analysis sees more than one vertex per tree" overclaims; Section 5 refutes one potential, and that potential already reads every vertex — high — reword as above.
F: LOW — NOTE.md:252 — "36 ≤ n ≤ 63" should be 36–61 (62, 63 follow from certificates + superadditivity) — high — change range.
F: LOW — NOTE.md:251 — T(64) ≥ 18 "by certificates": that 10 MB certificate is not in the repository (README.md:247-249; solver-b/ has only n=8,12,30,32) — high — say "certificate not included; generator is".
F: LOW — NOTE.md:119-127 — Lemma 2 omits "y keeps type (p′,q′)", used at 261 — high — add clause.
F: LOW — NOTE.md:87-88 — validity of the flipped path (result is a position) not argued — high — add one clause.
F: LOW — NOTE.md:64-67 vs 95-99 — Lemma 1 stated for a fixed sequence, used adaptively; proof is per-round so fine, statement should say so — medium — restate as a one-round invariant.
F: LOW — NOTE.md:242-244 — superadditivity sketch omits that Algorithm flips in the first tree during the second phase only add cost (solver-b/proofs.txt:52-54) — medium — add clause.
F: LOW — NOTE.md:249 — "three separate programs" hides that n=14,15 were searched with normal replies only (README.md:217-222) — medium — add "n = 14, 15 under normal replies".
F: LOW — NOTE.md:329 vs 323 — "random-walk rule" conflicts with prior-art.md's description of the folklore rule as path flipping — medium — "the path-flipping rule".
F: LOW — NOTE.md:269-271 — the 9-vertex failure is presented as a fact; source marks it COMPUTED (proofs.txt:159). I verified it by hand, so true — low — say "(checked by hand and by program)".

Other items not asked: none beyond the above; the mathematics of Sections 2–4 and 6 is sound and matches the Lean build (BUILD.log: success, no sorry).
