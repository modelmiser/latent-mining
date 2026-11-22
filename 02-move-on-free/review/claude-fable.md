<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Q1: CONFIRMED** — `solver-a/proofs.txt:9-21`: K ≥ ⌈M/3⌉, t 3-blocks each pin a distinct class-q cell, g(K) monotone (g(K+1)−g(K) ≥ 0 since ⌊(M−K−1)/3⌋ ≥ ⌊(M−K)/3⌋−1), ⌈H/3⌉ ≥ L ⇒ H ≥ 3L−2; arithmetic (5M+2r−14)/3 rechecked — none.

**Q2: CONFIRMED** — A `:29-51`: no straddle of E (E−1 occupied), #gaps ≤ b+1, f+b ≤ c+1 ≤ M, f ≤ (s−1)(M+1)/s, s=2 case (3M+1)/2 ≤ (5M+2)/3; ⌊(5M+2)/3⌋=⌈5M/3⌉ all hold. B `:62-83`: I1, I2 induction, o ≥ ⌈(M−2)/3⌉, p+s−1 ≤ 2M−1−o, M=3 cases hold; B(M) table values recomputed. `ff_O.log` covers M=3..16 — none.

**Q3: CONFIRMED** — `solver-b/proofs.txt:99-117`: y_a+y_{a+1} ≥ y holds for any placement (a 2 covers two of three residues); survivors pin distinct class-r cells; 3t ≥ M−x_r−2y_r−2; expression monotone in X so X ≥ 2M/3 is valid. No placement assumption used — none.

**Q4: CONFIRMED** — `solver-a/proofs.txt:71-81`: at τ_s type-s hexads full, earlier-opened types still ≥ n_u each with ≥ u live cells, legality gives 6n_s'+Σ ≤ M+6−s ≤ M+5. LP: n_a-coefficient [6(6−a)−a(6−b)]/36 > 0 for a,b ≤ 3, so all-tight; [108−12a−6b+ab]/216 maximal at (1,2)=86/216. New hexad index ≤ #non-empty−1 ⇒ H=6·max suffices — none.

**Q5: CONFIRMED** — L1 `open-f-lower/proofs.txt:28-34`: t ≤ Σ⌊l_i/3⌋ ≤ (H−L−W)/3. L2 `:36-41`: free(moved 1) always legal, grants nothing; result = post-free layout minus the 1's *source* cell regardless of destination (hole case: hole is re-emptied) — none.

**Q6: CONFIRMED** — `:72-107`: interior ≥ y−3 in all (l,r) cases incl. l=0 or r≤1; clean ≥ y−3−2k; |F| ≥ ⌈n/2⌉ per maximal run, F non-adjacent so merged runs g+2+g' ≡ 2 are distinct from each other and from the k bad runs; pin runs ≡ 2, disjoint from chain runs, below top block; y<3 ⇒ F=∅, k > (y−3)/2 handles it; proof yields 7M/4−5.75 for all M ≥ 3 (M=3: X=2,y=0 trivial). `thmA_check.log` 290 games, 0 violations — none.

**Q7: CONFIRMED** — R `open-f-upper/proofs.txt:91-120`: e2=4M/3−2, r ≤ k, a ≤ r, 2-cell holes hold no 3, extent ≥ 13M/6−4−v, 81M/48−43/4 ≥ 27M/16−11. R' `:146-164`: e2 identity, straddle +1, extent ≥ 2M+W−v−5, W ≥ b / W ≥ 2(|F|−b), max(b,n2/8−2b) ≥ n2/24. Implication: 7M/4−7 ≥ 27M/16−11 and ≥ (5/3+1/144)M−12 for all M, and both manager classes ⊂ Theorem A's class — none.

**Q8: CONFIRMED (with caveats)** — 7M/4−7 > 5M/3 ⇔ M > 84 ✓; ⌊7M/4⌋−1 matches all 33 table entries M ≤ 12, exceeds ⌊(5M+2)/3⌋ at 24 ✓ and all M > 32 ✓, but also at 27, 28, 30, 31, 32 (computed); integer bounds ⌈7M/4−17/4⌉ > ⌈5M/3⌉ already at M=54 (91 > 90), 57, 58, then from 60 on — "cannot show before about 59" should read "real-valued bounds cross at M=59; integer bounds first separate at M=54, permanently from 60"; and "wrong at M = 24, 27, 28 and every M ≥ 30".

**Q9: REFUTED (one phrase)** — Tables: verifier `table_3_8.log`/`table_9_11.log` M=3..11 all three games; M=12,13 in `solver-a/t12_*.log, t13*.log` (F at 12 by O=N sandwich) and `solver-b/outF10_12.txt, outF13.txt, outN13.txt` ✓. `ff_O.log` M=3..16 ✓. `thmA_check.log` 5 managers, 290 games ✓. `verifier/p6_B.log` 321 managers ✓. But top-level `README.md:33` and `02-move-on-free/README.md:6-7` "worth nothing to an allocator that packs its first blocks together (proved)" is not what Theorem A says: it proves such an allocator cannot go *below* the universal no-move bound 7/4; first fit without moves needs 2M−1 (`ff_N.log`, M=7..13) and ≈11M/6 (`open-f-upper/proofs.txt:63`), with moves ≈7M/4 (`:64`), so the right is worth ≈M/12 to it. Replacement: "the right cannot bring an allocator that packs its first blocks together below the 7/4 bound that every allocator already faces without moves". μ^F is marked open in both READMEs and `open-f-*/proofs.txt`; solver reports give it as a hedged guess ✓.

**Q10: CONFIRMED-with-notes** — no proof contradiction; see F lines.

F: MODERATE — `README.md:33`, `02-move-on-free/README.md:6-7` — "worth nothing … (proved)" overstates Theorem A (see Q9) — high — reword as above.
F: MODERATE — `02-move-on-free/README.md:46` — R' row omits hypothesis M ≡ 0 (mod 12) (`open-f-upper/proofs.txt:143`); stated for all M — high — add "M ≡ 0 mod 12".
F: LOW — `open-f-upper/proofs.txt:10-12` — "effect invisible below M=384 … even M=100" is contradicted by Theorem A (≥7M/4−5.75 > ⌈5M/3⌉ from M≈70) and by `open-f-lower` battery (+7..+16 at M=96): the dp4 figure is the optimum against one script, not the manager's need — high — say "against script S".
F: LOW — `open-f-lower/proofs.txt:8-10, 294` — headline/status paraphrase "no room for a 2 between the first M 1s" is weaker than the theorem's hypothesis (no free pair *after step 2*); e.g. 1s on even cells satisfy the paraphrase but not the hypothesis — high — use the line 66-68 wording.
F: LOW — `02-move-on-free/README.md:64-68` — numeric claims imprecise (first integer separation M=54; ⌊7M/4⌋−1 also wrong at 27, 28, 30-32) — high — see Q8.
F: LOW — `open-f-lower/proofs.txt:44-48` — L3 tagged COMPLETE but rests on correctness of `core.thin_best` (code, unproved) — medium — tag COMPUTED.
F: LOW — `open-f-upper/proofs.txt:91` — "odd rank (2nd, 4th, …)" is self-contradictory naming; argument needs the *top* 2 freed, which is what is used — high — say "even rank".
F: LOW — `02-move-on-free/README.md:39` — solver A has no direct F run at M=12 (`t12_*.log` only N, O); value follows by O ≤ F ≤ N sandwich — medium — note it.

Not asked but worth noting: the 02 README line 41 states "h^O ≤ ⌈5M/3⌉" but P2's own exhaustive log shows equality only from M=6, so the bound is 1 above truth at M=3,4,5 — consistent, just not tight.
