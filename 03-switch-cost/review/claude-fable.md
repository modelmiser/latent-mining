<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Q1: CONFIRMED** — `solver-b/proofs.txt:53-71`, `solver-a/proofs.txt:11-40`. Jobs arriving mid-lock are handled by Case 2 (job k chosen at s with earliest deadline; every job run in [t1,C) has d ≤ d_j < d_k so r ≥ s+1; LB on [s+1,d_j] gives C ≤ d_j+p_k−1). Case 1 uses "idle ⇒ nothing released and unfinished". S_T ≤ F_T: switch ticks read as idle give a preemptive schedule, so the S-adversary's strategy set is a subset (`solver-b:50-52`). — none

**Q2: CONFIRMED** — `solver-b/proofs.txt:80-90`. Branch 1: idle,u,a,a is switch-free and on time; online locked on a cannot run u at tick 1. Branch 2: a,a,u,u′ feasible; 4 units in ticks 1–3 with all d ≤ 4. 3′: a,u,a is free-feasible; both branches give ≥1. — none

**Q3: CONFIRMED** — `open-f/proofs.txt:34-85`. (1) s_j−s_{j−1} ∈ [1,j+1] holds in both timely and stall cases, giving nested windows and W_j-work = L_j. (2) The hindsight schedule J_{i+1} ⊂ [s_i,s_{i+1}) ∪ [D_{i+1},D_i) has exactly i+2 ticks, disjoint from the newest job's first L_m ticks; valid at every release. (3) C_i = D_i + δ_i is exact. (4) x_j empty because tick x_j−1 runs unfinished J_j. (5) Stall gives δ_{j−1} ≥ (j+1)+(P−1−j) = P; type F at j ≤ P−2 gives δ_j ≥ P−1 using s_{j+1} ≥ s_j+j+1; j = P−1 gives u late by P−1; H(1) gives δ_0 ≥ P−1. I played P=3 (T=6) by hand: refuse J_1 (stall → J_2=(2,3,6), ≥2), leave J_1 after one unit (switch at 1, J_2 at 2, u=(3,1,4): ≥2), idle at 0 then finish J_1 (J_2=(2,3,6), u=(4,1,5): ≥2). All ≥ 2. — none

**Q4: CONFIRMED** — `open-s/proofs.txt:84-134, 139-167`. (a) O(t), Oin and Lemma 0 count all remaining work regardless of job type; an earlier decoy is an "i" job; modes R/F only rely on the lock on the current K_ρ, which a k-play implies (a locked scheduler must play e first). (b) The stream release depends only on PLAN0's state, fixed before the scheduler moves (`:53`) — well defined. (c) Ψ grows ≤ 3P−1 per round (PLAN0 ≤ max(Ψ,s+2P−1); PLAN1 ≤ max(Ψ+P,s+3P−1), P2 constant, F adds ≤ P−1), so B ≤ (ρ+1)(3P−1) ≤ Δ. (d) f = D*−t−c ≥ Δ−B ≥ 0; fillers released at t ≤ Δ with t+p ≤ D*; Lemma 0 gives w+P, P, P−1 respectively. (e) Lateness ≥ P occurs only when lag1 ≥ P already (O(t) = lag1 when PLAN1 is empty) — a scheduler that wasted ≥ P ticks; NP-EDF never enters those branches (I traced it: it plays i through mode A, then k at s+P, loses exactly P−1 at the fork). No contradiction. (f) P=2: one round, Δ=5, T=7; fork tick x+1 only; traced by hand, lateness ≥1. — none

**Q5: CONFIRMED** — `solver-b/proofs.txt:49,75-78` + `open-s/proofs.txt:8-11`: P−1 ≤ S_T ≤ F_T ≤ P−1 for T ≥ (P−1)(4P−1), limits exist by monotonicity. — none

**Q6: CONFIRMED** — Corollary A′ (`open-f:98-100`): Theorem A at P′=min(P,k+1) plus monotonicity in P,T; equals every F entry of the README table. Theorem B (`:103-119`): distinct x_i, trap gives a_i+P+1−(a_i+2) = P−1, a_N ≤ T−3, early-stop fillers feasible since τ < NP+k. — none

**Q7: CONFIRMED** — README table matches `solver-a/tables.txt` (MODE 0/1) and `solver-b/results.txt:143-209, table_run_S4_L2_full.txt:9` (S_9(4) ≤ 2), `checks.txt:78` (S_9(4) ≥ 2), referee C4/C5. No exact entry rests on a bound only; S_9(3) is footnoted. — none

**Q8: CONFIRMED** — All phrases are backed: "no online rule beats never preempting" = S(P)=F(P)=P−1 attained by NP-EDF; horizons 4/9/≤15 vs 3/6/10 match tables and the S_15(4) certificate (`referee.md` C5); 4P² = (P−1)(4P−1), 2P² = (P−1)(2P+3) (`open-s/results.txt` §4). — none

**Q9: CONFIRMED** (minor inconsistencies only, listed below) — none

F: LOW — `open-s/proofs.txt:107` — says tight units have deadline ≤ Δ; they are released at t ≤ Δ so deadline ≤ Δ+1. Harmless (needs only ≤ Δ+ρP, ρ ≥ 1). — high — write "≤ Δ+1".
F: LOW — `open-s/proofs.txt:77` — "one job of length f mod P" is a zero-length job when P | f; `play.py:157` skips it, the text does not say so. — high — add "if f mod P > 0".
F: LOW — `open-s/proofs.txt:196`, `reports/open-s.md` claim 5 — "node counts agree exactly for P = 2, 3, 4" but `results.txt` shows Python P=3 at 3850 nodes vs C++ 1064 (the Python line is apparently the no-memo run; no memoised Python P=3 count is recorded). — medium — record the memoised Python P=3 run or restrict the claim to P = 2, 4.
F: LOW — `03-switch-cost/README.md:124-125` — says S_7(4)=1 rests on Lemma D; `solver-a/proofs.txt:106` reports P=4 T≤7 with DOM off agreeing. Over-cautious, not an overclaim. — medium — drop T=7 from that list.
F: LOW — `03-switch-cost/README.md:64` — S_9(2) left blank though `solver-b/results.txt:40,50` computes it (=1, also forced by Theorems 1–3). — high — fill in 1.
F: LOW — `open-f/proofs.txt:13`, `open-s/proofs.txt:8` — "for all T ≥ …" silently uses the solvers' monotonicity lemma (same jobs legal at larger T); not cited. — high — cite Theorem 2 / Lemma 2.

Not asked but worth noting: the strongest unverified dependency is still the solver-b S_9(4) ≤ 2 and S_8(4) = 1 entries, which rest on Lemma D pruning (`solver-b/proofs.txt:34-46`); I read Lemma D and the simulation argument holds (adjacent run ticks of X are adjacent in s; the lock condition is preserved), so I see no reason to doubt those entries.
