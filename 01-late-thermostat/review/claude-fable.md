<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Verdicts**

Q1: CONFIRMED — solver-a/proofs.txt:55-75. (K) holds (deterministic controller, o_t=sgn(x_{t-d})); (*) at :63 uses controls agreeing on ticks ≤σ+d, switch legal since w_{σ-1}=0 (odd tick of P/Q); Step 1 parity: one of t-d-1,t-d even, both give P_t-(d+1)≥-B; liminf is of an integer sequence in [2,B], so attained infinitely often; Q keeps ≥1 since P_t≥λ for t≥T0 and ≤λ-1 pushes dropped; Step 4 contradiction sound. Only gap: T0 must also be ≥τ0=2d+2 for Q to "equal P on ticks <τ0" (:67,:71); it can be chosen so. — "choose T0 even, T0 ≥ 2d+2, with P_t ≥ λ for t ≥ T0".

Q2: CONFIRMED — :77-88. Step 1: σ=t-d-1 even for odd t, ceil((d+1)/2)=d/2+1 ⇒ P_t≥2; even t uses w_{t-1}=0. Step 5: Q^ legal (w_{a-1}=w_a=0); Q^_t=Q_t-ceil((t-a)/2)+floor((t-a)/2)=Q_t-[t odd]≥1 because Q_odd≥2 for t≥T1+d+1<a. Step 6: σ=σ*-d-1 odd, w_{σ-1}=0 (even ≥a in Q^), odd ticks in [σ,σ+d+1) = d/2+1, so x_{σ*}=1-(d+2)=-(d+1)<-B. — none.

Q3: CONFIRMED — solver-b/proofs.txt:35-61: cases A1/A1'/A2/B exhaust x_s+u_s+w_s≥1 with x_s≤0 (u_s=-1 impossible), counts of active ticks/floor-ceil of w verified for both parities; phase-2 pairing (w-1+w'≤0) correct; redo with w_{s+d}=0 gives x_e≤d so M≤d+1. solver-a :46 inequality ceil((d+1+l)/2)-floor(l/2)≤floor(d/2)+1 is false for d odd, l odd (d=1,l=1: 2≤1); repair: for odd d the bound is x_s+e+(d+1)/2+[l odd], and e=(d+1)/2 forces x_s≤-1, so x_T≤d+1 either way. — "case II, sub-case s+d even, is false as written for odd d and odd l; repaired by the e-maximal ⇒ x_s≤-1 argument already used in case I".

Q4: CONFIRMED — :7-30. (a) ceil((d+1+l)/2)-l≤ceil((d+1)/2) ✓, e=d+1⇒x_s≤-1 ✓; (b) w_d=0 so push at d+1 legal, x_{2d+2}=d+ceil((d+1)/2) ✓; (c) lower bound floor(3d/2)+2 ✓; (d) all three branches traced, x_{2d+2}=+1 then stale negatives give u=+1, positives u≥0, w=+1 alternate ✓. Caveat: ":30 values +2,+2" (upper bound for the two suboptimal maps, hence "only 3 bounded") is computed (verifier/run_R.txt), not proved. — "the other two maps are unbounded-free with value floor(3d/2)+2 (computed d≤12; lower bound proved)".

Q5: CONFIRMED — open-h/proofs.txt:22-61. Padding makes (E),(O),(R) hold for all i; Facts 1-3 are index arithmetic I re-derived; case o=+1: [Σ+1,Σ+2r+1]=[q-r,q+r] exact, three clamp sub-cases close; odd d used at Fact 2 (r+1 pairs) and at window width 2r. — none.

Q6: CONFIRMED — HistoryUpper.lean:49-56: x0 (n≤2r+2), u0 (n<2r+2) match x_i=0 (i≤0), u_i=0 (i<0) under index=t+d+1; ctrl at m+d sees x m and wsum u m (2r+1)=u_{t-d..t-1}, m≥1 ⇔ real t≥0. Not vacuous: for any w with w=0 on indices<2r+2 (forced by step+x0+u0), define u,x recursively; every real adversary yields such a triple. I compiled it: Lean 4.36.0, axioms [propext, Classical.choice, Quot.sound]. README:56-57 sentence accurate. — none.

Q7: CONFIRMED — H≤C (clock is a history controller, briefs.txt:126); C≤d+1 (T2), H≥d (T3) ⇒ 0≤C-H≤1; even d: d+1≤H≤C≤d+1 ⇒ gap 0 (solver-a :92-95). — none.

Q8: REFUTED (one cell) — README.md:28 "lower bound by certificate-checked computation for d ≤ 13": certificate-check logs exist only for d=6,7,9,11,13 (open-c/check_*.log); d=1 is hand (solver-a :103-110), d=3,5 exhaustive by two solvers (solver-a/run_clock_cpp_small.txt:9,17; results_clock_small.log). Other rows supported (R: solver-b/results_R_6_12.log; H/C rows per T2/T3/Lean; top README:32 fine). — "lower bound: hand (d=1), exhaustive search by two solvers (d=3,5), certificate-checked computation (d=7..13), open beyond".

Q9: CONFIRMED with gaps — all listed caveats trace (T2 case II at :46; d=11/13 single checker per open-c/proofs.txt:75-76). Missing: (i) README:67 says files >1.5 MB omitted, yet cert_7_7_KR3.txt is 2.1 MB and cert_7_7.txt, cert_9_9*.txt, cert_11/13, mut_cert_* named in open-c/proofs.txt:200-206 are absent; (ii) certificate soundness rests on hand lemmas (M),(R),(C),(I) (open-c :38-53; I re-traced them, they hold) and on ngcheck.cpp/py, which I did not audit; (iii) no Lean toolchain pin/build log. — add those three sentences.

**Findings**

F: LOW — 01-late-thermostat/README.md:28 — "certificate-checked for d ≤ 13" overstates d=1,3,5 (no certificates/check logs for them) — high — reword as in Q8.
F: LOW — README.md:67 vs open-c/proofs.txt:200-206 — files listed in proofs.txt (cert_7_7.txt, cert_9_9*.txt, cert_11_11_KR3.txt, cert_13_13_KR3.txt, mut_cert_*) absent; 2.1 MB cert_7_7_KR3.txt contradicts "above 1.5 MB left out" — high — fix the sentence and the file list.
F: LOW — solver-a/proofs.txt:70-72 — T0 not constrained to ≥τ0=2d+2 though Step 2's validity needs Q=P on ticks<2d+2 — medium — add "T0 ≥ 2d+2".
F: LOW — solver-a/proofs.txt:46 — false inequality for odd d, odd l (d=1,l=1) — high — repair as in Q3 or delete case II for odd d and cite P2.
F: LOW — solver-a/proofs.txt:30 — "only 3 maps are bounded, with values …+2,+2": boundedness/exact value of (+1,±1,-1) only computed — high — tag as computed.
F: LOW — solver-a/proofs.txt:62 — "(legal: V rests on odd ticks)" stated for arbitrary V; holds only for the V actually used (P,Q) — high — say "for V ∈ {P,Q}".
F: LOW — lean-h/ — no lakefile/toolchain file or build log; reproducibility depends on reader's Lean version (passes on 4.36.0) — medium — add lean-toolchain and the #print axioms output.
F: LOW — open-c/proofs.txt:38-53 / README:52-68 — the soundness of the nogood method and the two checkers are themselves same-family hand/code artefacts, not mentioned under "What to distrust" — medium — add a bullet.

**Not asked but relevant**: I did not audit ngcheck.cpp/ngcheck.py, the exhaustive solvers, or the literature claim; the headline for odd d>13 remains open as stated.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
