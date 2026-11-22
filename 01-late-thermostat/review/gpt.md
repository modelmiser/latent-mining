<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — `solver-a/proofs.txt:53-76`; bounded integer liminf is attained infinitely often, (K) supplies controls through `σ+d`, and every switch occurs after a forced rest — none

Q2: CONFIRMED — `solver-a/proofs.txt:77-88`; phase shifting changes odd states by −1, preserves positivity, and `[σ,σ*)` contains `d/2+1` odd ticks, giving `1−(d+2)=−(d+1)` — none

Q3: CONFIRMED — Solver B’s exhaustive cases and rest-sensitive phase-two argument establish T2 (`solver-b/proofs.txt:35-62`); Solver A’s inequality is false for `d=3,l=1` (`solver-a/proofs.txt:44-48`) and must be replaced, not merely algebraically corrected — none

Q4: CONFIRMED — the optimal relay’s upper/lower bounds are `solver-a/proofs.txt:7-20`; nonzero `f(0)` is strictly worse and every map with `f(+)≥0` or `f(-)≤0` is unbounded (`:21-29`), leaving the unique optimum — none

Q5: CONFIRMED — padding makes (E) valid (`open-h/proofs.txt:22-25`); Facts 1–3 and the induction prove `|z_t|≤r` (`:30-59`); oddness supplies both the even disturbance window and symmetric recentering (`:60-66`) — none

Q6: CONFIRMED — `Play.x0`, `u0`, and `ctrl` encode the stated shift exactly (`lean-h/HistoryUpper.lean:49-56`); padding disturbances follow from `step+x0+u0`, and the all-zero functions satisfy `Play`, so it is not vacuous. Per-adversary existence is not formalized, exactly as README says (`README.md:54-57`) — none

Q7: CONFIRMED — `H≤C`, T2, and T3 give odd `d≤H≤C≤d+1` and even `H=C=d+1` (`solver-a/proofs.txt:91-94`) — none

Q8: REFUTED — the odd-H cell calls the whole upper bound machine-checked although existence is omitted, and the odd-C cell says “certificate-checked … for d≤13” although the certificate section covers 7,9,11,13 (`open-c/proofs.txt:20-26`) while 3,5 are exhaustive searches — replace those statuses with “safety invariant machine-checked; controller existence proved only on paper” and “lower bound: hand proof d=1; exhaustive computation d=3,5; certificate-checked d=7,9,11,13”; keep other cells and the top table unchanged

Q9: REFUTED — `README.md:64-66` reports a literature-search conclusion, but the repository contains only its prompt (`briefs.txt:250-269`), and it omits the d=3,5 certificate-status distinction above — add “the prior-art report is not included” and the corrected computation provenance

F: MODERATE — `solver-a/proofs.txt:44-48` — claimed inequality fails at `d=3,l=1` — confidence: high — replace case II with Solver B’s rest-sensitive argument

F: MODERATE — `README.md:27` — d=3,5 are described as certificate-checked without certificates/checker logs — confidence: high — distinguish exhaustive and certified cases

F: LOW — `solver-a/proofs.txt:30` — exact values for the two nonoptimal bounded maps lack a general upper-bound proof — confidence: high — add the bound or delete that sentence

F: LOW — `solver-a/proofs.txt:70-81` — Step 3 should explicitly choose `T0≥2d+2`; even `λ=1` needs the empty-omission convention — confidence: high — state both conventions

F: LOW — `lean-h/HistoryUpper.lean:1-124` — no pinned Lean toolchain or recorded build is included — confidence: high — add reproducible checking instructions

F: LOW — `README.md:64-66` — prior-art conclusions cannot be audited because the report and citations are absent — confidence: high — include the report

Unasked: no additional material issue found.
