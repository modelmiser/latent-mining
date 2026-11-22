<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED — I traced Theorem 3 for odd d through solver-a/proofs.txt:55-75. (*) holds: the switch at an even σ is legal because the play rests at σ−1, and (K) fixes the controls up to σ+d. Step 1 gives P_t ≥ 2. The liminf argument is sound: an integer sequence bounded below has a liminf that is reached infinitely often and is eventually a lower bound. Leaving out pushes is always legal. Step 2's use of (K) is right because the observations match. One small gap: Step 3 never says T0 ≥ τ0 = 2d+2, which Step 2 needs (line 70). — "T0 even, T0 ≥ 2d+2, with P_t ≥ λ for t ≥ T0".

Q2: CONFIRMED — lines 77-88. Step 1 at odd t gives P_t ≥ d+2−B. Step 5 holds: Q^_t = Q_t − [t odd] ≥ 1, and the switch is legal because w_{a−1} = w_a = 0. Step 6 holds: σ is odd, [σ, σ*) has d+1 ticks, of which (d+2)/2 are odd, so x = 1−(d+2) = −(d+1). The same T0 ≥ 2d+2 gap applies, and a must also be ≥ 2d+2 (it is, since a ≥ T1+d+2). — none beyond Q1.

Q3: CONFIRMED — solver-b/proofs.txt:39-61 is complete. I checked each case: A1, A1′, A2 and B in phase 1, the pairing in phase 2, and the redo with w_{s+d}=0, which gives d, d, d, d and d−1. Solver-a's Theorem 2, line 46, is false for odd d with odd l: the left side is floor(d/2)+2. It can be repaired: with x_T ≤ x_s+e+floor(d/2)+2, the existing split on e (x_s ≤ −1 when e is at its maximum, otherwise e ≤ floor(d/2)) still gives d+1. — "…≤ floor(d/2)+1+[d odd and l odd]; the e-split still yields d+1".

Q4: CONFIRMED with an overclaim — solver-a/proofs.txt:7-30. Parts (a), (b) and (d) check out step by step, and uniqueness follows from (c). But line 30's "only 3 maps are bounded, with values …+2, +2" proves only a lower bound for (+1,±1,−1). The upper bound and the boundedness are computed only (solver-b/results_R_6_12.log, verifier/run_R.txt). — "the other two maps have value ≥ floor(3d/2)+2 (equality computed for d ≤ 12)".

Q5: CONFIRMED — open-h/proofs.txt:22-67. I checked Facts 1-3, the base case z_{−1}=0, and the step bound (ii), including t−d ≤ 0, which uses the padding. All three sub-cases on q also check out. Both uses of odd d are stated correctly. — none.

Q6: CONFIRMED — lean-h/HistoryUpper.lean:49-56. Index n = t+d+1. ctrl at m+d reads x m, which is the real x_{t−d}, and wsum u m d is the real Σ_t. x0 forces w=0 at padding indices, and index 2r+2 is the real x_0 with free w. The hypotheses can be satisfied: x=u=w≡0 meets every field, and any w sequence determines x and u by recursion. I elaborated a copy in /tmp with Lean 4.36: no errors, and the axioms are only propext, Classical.choice and Quot.sound, so there is no sorry. README:56-57 is accurate. — none.

Q7: CONFIRMED — C ≥ H because every clock is a history controller (t = length of the history). C ≤ d+1 (T2) and H ≥ 2⌊d/2⌋+1 (T3) give gap ≤ 1, and the gap is 0 for even d. — none.

Q8: REFUTED, in part. (1) The C-odd row (README:28) says the lower bound is "certificate-checked … for d ≤ 13". But d=1 is by hand (solver-a:103-110), and d=3,5 come from uncertified exhaustive search (solver-a/run_clock_cpp_d5.txt, solver-b/results_clock_d5B5.log). Checked certificates exist only for d=7,9,11,13 (open-c/check_*.log). (2) The top README:32 status "partly machine-checked" is attached to the headline, but no step of the headline's proof (T2, T3) is machine-checked. The Lean result is not used by the headline, and the "open part" (C for odd d > 13) is not in it either. (3) The R row is fine apart from the Q4 point. — C-odd: "lower bound: by hand (d=1), exhaustive search (d=3,5), checked certificates (d=7–13); open beyond". Top: "proved by hand (not machine-checked); the exact odd-d clock value is open beyond d=13".

Q9: REFUTED, in part. README:67-68 says certificates over 1.5 MB are left out, but open-c/cert_7_7_KR3.txt is 2.16 MB and included. Meanwhile the exact-game cert_7_7.txt, which open-c/proofs.txt:91 says passed both checkers, is absent, as are the d=9/11/13 certificates. As shipped, then, no certificate in the repo has passed two checkers, and the d ≥ 9 checks cannot be repeated without regenerating. Missing caveats: (i) the headline rests only on hand proofs; (ii) solver-a/proofs.txt:112-116 ("NOT PROVED: H(d) ≤ d … d=7 not attempted") is stale next to Theorem A and the open-c results. — add those caveats and correct the size statement.

F: MODERATE — README.md:32 — the headline's status "partly machine-checked, one part open" describes the experiment, not the headline; the headline is entirely hand-proved, with nothing open — high — "proved by hand (one model family); not machine-checked".
F: MODERATE — 01-late-thermostat/README.md:28 — "certificate-checked … for d ≤ 13" overstates d=1,3,5, which have no certificates — high — reword as in Q8.
F: MODERATE — 01-late-thermostat/README.md:67 and open-c/ — the certificate-size policy is inconsistent: the 2.16 MB KR3 certificate is included, while the exact d=7 certificate (the only one checked by both checkers) and the d=9–13 certificates are missing; readers cannot re-check — high — ship cert_7_7.txt, or state which files are missing and the regeneration commands.
F: LOW — solver-a/proofs.txt:46 — the inequality fails for odd d and odd l (it is off by 1); the conclusion survives the e-split — high — patch as in Q3.
F: LOW — solver-a/proofs.txt:70 — T0 ≥ 2d+2 is not required, but Step 2 needs it — high — add it.
F: LOW — solver-a/proofs.txt:30 — "only 3 maps bounded, values +2" is unproved; only lower bounds are given — high — reword as in Q4.
F: LOW — solver-a/proofs.txt:112-116 — the "NOT PROVED / not attempted" notes are superseded by open-h and open-c but not marked so — high — add a pointer.
F: LOW — solver-a/run_c5.txt — logs a MemoryError for d=5, which looks like a failed run; the d=5 evidence is actually run_clock_cpp_d5.txt — medium — annotate.

Other points:
- Everything outside Lean rests on proofs I re-derived by hand, which is again model reasoning. The trusted computing base for C(7..13) is ngcheck.cpp. I read it (lines 30-56) and it implements rule (I) of open-c/proofs.txt:56-60 correctly, including the rest-twin closure and the containment of the initial set.
- The KR=3 soundness argument (open-c/proofs.txt:73-75) holds only because the checker runs the unfiltered game, which it does: ngcheck.cpp has no filter.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
