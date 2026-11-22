<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `solver-b/proofs.txt:49-71` and `solver-a/proofs.txt:11-40`. A job released during the run of k is covered by r ≥ s+1 (release is before the choice at s, and NP-EDF never drops a live job). Same-cost feasibility implies free feasibility by reading switch ticks as idle, so S_T ≤ F_T. — none

Q2: CONFIRMED — `solver-b/proofs.txt:80-90`. Run-at-0: locked on (0,2,4), so (1,1,2) finishes at ≥ 3; hindsight idle, u, a, a. Decline: (1,1,3) and (1,1,4) with hindsight a, a, u, u′; 4 units and only ticks 1–3 before time 4. Free case: (0,2,3) then (1,1,2), hindsight a, u, a, same split. — none

Q3: CONFIRMED — `open-f/proofs.txt:18-85`. For P=3, T=6 the three behaviours all lose: refusing J_1 through ticks 0–1 stalls at s_1=2 and δ ≥ 3; leaving after one unit records an empty x_j inside every outer window; idling is the stall case. Nesting, the free hindsight (left and right pieces of each window), saturation, the lock dichotomy, and the downward induction all check. — none

Q4: CONFIRMED — `open-s/proofs.txt:32-167`. (a) Lag is in units (`:94-105`); invariant (S1) (`:85-86`) keeps only stream units on H, and a partial earlier decoy only increases Oin. Mode F’s “P−1 left” is the current decoy, untouched before tick x (`:64`, `:124`). (b) The stream release depends on PLAN0 before the move (`:51-52`), which is legal online. (c) B ≤ Ψ+3P−1 and Ψ_ρ ≤ ρ(3P−1) give B ≤ (ρ+1)(3P−1) ≤ Δ (`:152-154`). (d) f = D*−t−c ≥ 0 because t+c ≤ B ≤ Δ ≤ D* (`:163`); Lemma 0 (`:25-27`) yields the stated lateness (`:118`, `:134`). (e) Lateness ≥ P is a scheduler that ate the decoy or idled; P−1 is the minimax, not a cap on bad play. (f) P=2 is one round and a one-tick fork; the branches close. — none

Q5: CONFIRMED — `03-switch-cost/README.md:55` with `solver-b/proofs.txt:49` and `open-s/proofs.txt:8-11`, `:131-134`. S ≥ P−1 and S ≤ F ≤ P−1 squeeze both limits. The finite-T claim is only for T ≥ (P−1)(4P−1). — none

Q6: CONFIRMED — `open-f/proofs.txt:98-119`. A′ is Theorem A at P′=k+1 ≤ P plus monotonicity; for k > P−1 the T threshold is loose and the bound is still P−1. Theorem B’s traps, the a_N ≤ T−3 count, and τ ≤ work−1 for the fillers all check; extending past T=P²−1 is monotonicity. — none

Q7: REFUTED — `03-switch-cost/README.md:59-68` prints S_9(4)=2 in the exact grid, but `solver-a/proofs.txt:115-116` leaves 2 ≤ S_T(4) ≤ 3 for T=9..15, and `solver-b/results.txt:179` is only maxrel=1 (a restricted adversary failing to force 3 is not an upper bound). The unrestricted “cannot force 3” is `solver-b/table_run_S4_L2_full.txt:9`, which `reports/referee.md:74` did not check. Shared cells do match, including F_10(4)=3 at `solver-a/tables.txt:60` and `solver-b/results.txt:218`. Corollary A′ does not exceed any F cell. The legend “blank = not computed” (`README.md:59`) is false for S_9(2) and F_10(2): `solver-b/results.txt:40-50` and `:61-73`. — “S_9(4)=2 only via solver B’s dominance-pruned run; solver A still has 2 or 3. S_9(2)=F_10(2)=1 were computed.”

Q8: CONFIRMED — `03-switch-cost/README.md:57` “between 10 and 15” needs S_9(4)≤2 from that same unchecked run. “Exactly P−1” (`README.md:8`) and “no online rule beats never preempting” (`README.md:34`, top-level `README.md:34`) match Q1+Q4. “4, 9, at most 15 against 3, 6, 10” (`README.md:79`) matches the tables plus the T=15 certificate `reports/referee.md:33`. “About 4P²” / “about 2P² for P≤5” matches `open-s/proofs.txt:169` and `:204-207`. — “Smallest T with S_T(4)=3 is at most 15; it is at least 10 only if the dominance-pruned S_9(4)≤2 run is accepted.”

Q9: CONFIRMED — `reports/referee.md:71`, `solver-b/proofs.txt:101-103` and `solver-a/proofs.txt:110-113` still call S(P)=F(P)=P−1 a conjecture for P≥5, while `open-f/proofs.txt:13-16`, `open-s/proofs.txt:8-11` and both READMEs say proved. The reports are the earlier stage, left verbatim (`03-switch-cost/README.md:137`). — “Mark solver and referee reports as pre-follow-up; the general bounds are the later open-f and open-s proofs.”

F: MODERATE — `03-switch-cost/README.md:57` and `:68` — S_9(4) is printed as 2 and the P=4 horizon as 10..15, but solver A leaves 2..3 and the only unrestricted upper bound is a Lemma D run the referee did not read — high — footnote those cells as solver-B-only
F: LOW — `03-switch-cost/README.md:59` — “blank = not computed” hides S_9(2)=1 and F_10(2)=1, both in `solver-b/results.txt` — high — fill 1s or change the legend
F: LOW — `open-s/proofs.txt:77-78` — “one job of length f mod P” is a length-0 job when f≡0 (mod P). P=2, idle at tick 0, cash-in at t=2 with c=1 gives f=4 — high — omit the zero job, as `open-s/play.py:157` already does; the unit count f is unchanged
F: LOW — `reports/referee.md:71` — says the general equality is conjecture only, contradicting the later proofs a reader meets first in the README — high — date the report as pre-follow-up
F: LOW — `03-switch-cost/README.md:147` — the S_15(4)≥3 tree is not in the tree; the “at most 15” claim rests on `reports/referee.md:33` — high — keep the referee citation next to the horizon sentence

The questions already cover the proof gaps that matter. The extra items are the table legend, the zero-length filler, and the stale “still a conjecture” reports.
