<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED — solver-b/proofs.txt:58-70 and solver-a/proofs.txt:17-35. In Case 2, every job that runs in [t1,C) has a deadline earlier than d_k, so it was not yet released when k was chosen at tick s (release comes before the choice). That gives r ≥ s+1, which covers jobs released while the machine is in the middle of a job. Ties don't matter because the inequality is strict. S ≤ F is argued correctly (b:50-52, a:37-40): the online side is the same in both games, and the same-cost adversary's legal releases are a subset of the free adversary's. — none

Q2: CONFIRMED — b:80-90, a:52-63. Every hindsight schedule checks out (idle,u,a,a / a,a,u,u′ / a,u,a). Locked branch: u cannot run in tick 1. Declining branch: 4 units must fit in 3 ticks before deadline 4, or 3 units in 2 ticks for F_3(2). Solver-a's b1/b2 branches also hold. — none

Q3: CONFIRMED — open-f/proofs.txt:34-85.
- (1) The gap s_j − s_{j−1} lies in [1, j+1], so the windows are nested, including after a stall.
- (2) The ring for J_{i+1} has exactly L_i − L_{i+1} = i+2 ticks, and L_j ≥ j+3, so every job and the hindsight schedule are legal after every release.
- (3) C_i = D_i + δ_i holds.
- (4) The tick after an unfinished J_j cannot run another job. The ordering x_j < a_{j+1} < s_{j+1} ≤ x_{j+1} holds.
- (5) A stall gives δ_{j−1} ≥ P. Type F gives δ_j ≥ P−1, or makes u late by P−1 when j = P−1.
- P=3 counterexamples I tried all fail: refusing to start J_1 gives lateness 2, finishing J_1 gives 2, leaving J_1 after one unit gives 2. — none

Q4: CONFIRMED — open-s/proofs.txt:91-167.
- (a) A partly run earlier decoy has deadline ≤ Δ+ρP. It counts in O_in, so running it is an "i" move and both lags stay unchanged. Starting K_ρ from that lock first needs an e tick. No case assumes the scheduler's other jobs are unit jobs; only the plans' pending jobs are units, because H finished the earlier decoys.
- (b) Whether to release depends only on PLAN0, which is set by ticks < t and the current mode, so the rule is well defined online.
- (c) B ≤ Ψ+3P−1 holds in each mode, and Ψ_0 = 0.
- (d) f = D* − B ≥ 0. Lemma 0 gives lateness w+P, P, and P−1 in the respective cases.
- (e) Not a contradiction: those cases need a scheduler that played badly. NP-EDF is never in mode R and never exhausts PLAN1.
- (f) P=2 checked (Δ=5, T=7). — none

Q5: CONFIRMED — Q4 gives S ≥ P−1, S ≤ F gives F ≥ P−1, and Q1 gives the matching upper bound. Theorem A is not even needed. — none

Q6: CONFIRMED — open-f:98-119. In the early-stop case of Theorem B, τ ≤ NP+k−1 < W ≤ T, so the filler jobs fit. — none

Q7: CONFIRMED with caveats — README:61-68 matches solver-a/tables.txt:17-67 and solver-b/results.txt (S_8(4), S_9(4)≤2), and Corollary A′ matches every F entry. S_9(4)=2 is shown as exact, but its upper bound is a single DOM run (solver-b results, P=4 T=9 L=2) that the referee did not reproduce (referee.md:74); it is disclosed. S_9(2)=1 was computed (solver-b results:40,50) but is shown blank. — mark S_9(4) "2 (DOM)"; fill in S_9(2)=1.

Q8: REFUTED (wording only) — README:7-9 says "the best worst-case lateness … is exactly P−1" with no horizon, but S_5(3) = 1 (README:66). README:57 says the rule "needs about 4P²" when the proof only shows that this much is sufficient. "About 2P²" measures when the rule's own witness plans pass (proofs:204-207), not what the rule needs. — "…once the horizon is long enough (T ≥ (P−1)(4P−1) suffices)…"; "the proved rule works at (P−1)(4P−1); its witness plans pass from (P−1)(2P+3) for P ≤ 5"

Q9: REFUTED — open-s/proofs.txt:196 says the play.cpp and play.py node counts "agree exactly … for P = 2, 3, 4". results.txt:7 vs :14 shows 3850 vs 1064 at P=3, and the Python run there was memo-free, so the counts are not comparable. The proofs:210 claim of "9 (P=6)" deviations was run at Δ=45, not the proved Δ=85 (results:41-44). — "agree for P=2, 4"; "8 deviations at the proved Δ; 9 at Δ=45"

F: MODERATE — README.md:7-9 — the headline "exactly P−1" has no horizon qualifier, but the table shows S_5(3)=1 and F_5(3)=1 — high — add "for long enough horizons".
F: MODERATE — open-s/proofs.txt:196 — the claim that node counts agree for P=3 is not supported: 3850 vs 1064 (results.txt:7,14), from different run modes — medium — restrict the claim to P=2, 4 or rerun P=3 with memo on.
F: LOW — open-s/proofs.txt:209-210, reports/open-s.md:49 — the "9 deviations" figure for P=6 comes from the non-proved Δ=45 — high — state which Δ was used.
F: LOW — README.md:57, open-s/proofs.txt:222 — "needs ~4P²/~2P²" reads as necessary, but 4P² is only sufficient and 2P² is the witness-plan threshold — high — reword.
F: LOW — README.md:64 — S_9(2)=1 is computed (solver-b/results.txt:40,50) but shown blank, against "blank = not computed" — high — fill it in.
F: LOW — README.md:68,124-125 — S_9(4)=2 is shown exact, though its upper bound is one solver's DOM run. Also, S_7(4) was computed with DOM off (solver-a/proofs.txt:103-106), so listing it under Lemma D is inaccurate — medium — mark the S_9(4) entry; drop T=7 from the list.
F: LOW — open-s/proofs.txt:77 — the text calls for "one job of length f mod P" even when that length is 0, which is an illegal p=0 job; the code omits it (play.py:157) — high — add "if nonzero".
F: LOW — README.md:52,54 "I re-derived it" vs open-s/proofs.txt:221 / reports/open-s.md:60 "nobody else has read the proof" — no file records the re-derivation — medium — point to a file, or say "read".
F: LOW — reports/open-s.md:36 — the Python match for P=5 was at Δ=60 with a 64-bit hash, not the proved Δ=56 — medium — say so.

Not asked:
1. The phrase "no online rule beats never preempting" (README:76-78, top-level README:34) holds only for NP-EDF specifically. Other never-switch rules are driven to 2 at P=3, T=5 (solver-a/checks.txt:26). Say "non-preemptive EDF".
2. solver-a/proofs.txt:110-116 and referee.md:70 ("P ≥ 5 conjecture only") are superseded by the follow-up files. The verbatim reports are fine as records, but the README could flag this.
3. Neither general lower bound is machine-checked. My trace found no gap, but that is one more reader, not a formal check.
