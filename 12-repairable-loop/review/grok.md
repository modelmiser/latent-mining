<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `open-growth/proofs.txt:11-26` (prefix count; loss drops k and adds k−1; doubling drops k−2 and adds k−1; ρ = T+2a, a ≤ e/2) — none
Q2: CONFIRMED — `open-growth/proofs.txt:35-57` against `solver-b/proofs.txt:9-29` (Facts A–C apply; S = σ+2 and ρ = T+2) — none
Q3: CONFIRMED — `open-growth/proofs.txt:59-90`; the four losses in `open-growth/t_task1.out:7-10` are exactly “delete one 0 from a 00 between 1s”, and the only doubling is context 101 — none
Q4: CONFIRMED — `open-growth/proofs.txt:88-96` (0001000 sits in D for every n ≥ 7; Z, Z=A, and Z=B all miss length n+2; w need not be universal) — none
Q5: CONFIRMED — `open-growth/proofs.txt:119-131` (G2 is the pass; F_s is a set and the −k still counts k windows, repeats included) — none
Q6: CONFIRMED — `open-growth/proofs.txt:134-157`; the half-count repair in `reports/open-growth-referee.md:30` is the period-d argument on cells −1 and 0 — none
Q7: CONFIRMED — from `(**)` with k = n−4, `open-growth/proofs.txt:158` rearranges to e(19k−2) ≥ 2^(n−3)−19n²+117n−64, which is at least (2^(n−3)−19n²)/(19n); that tends to infinity. The figures 9, 418, 272775 are the unloosened `(**)` (`open-growth/bound.out:10,15,25`); the closed form itself is 0, 325, 235440 — none
Q8: REFUTED — lower bounds hold (`solver-a/proofs.txt:42-60`, `solver-b/proofs.txt:31-42`) and the length-76 loop is present and checked (`solver-b/witnesses.txt:12`, `verifier/witness_checks.txt:11,67`), but `README.md:11` and `:50-51` attribute 2^n/(19n) and 9 / 418 / 272775 to the closed form, and `:96-99` says coincidences are fewer than 2^(n−3) — “Excess ≥ (2^(n−3)−19n²)/(19n) ∼ 2^n/(152n). Sharp `(**)` values are e ≥ 9, 418, 272775 at n = 15, 20, 30; the closed form gives 0, 325, 235440. Bad singletons are ≤ 2^(n−3)+16(k−3)+2(k−1)(e+n−1).”

F: MODERATE — `README.md:11` — “at least like 2^n/(19n)” overstates the proved constant by 8; the closed form is ∼ 2^n/(152n) (`open-growth/proofs.txt:113`) — high — replace with “at least on the order of 2^n/(152n)”
F: MODERATE — `README.md:50-51` — 9, 418, and 272775 are not values of the displayed closed form (negative, 325, 235440) — high — attribute them to `(**)` before loosening, and give the closed form its own numbers
F: MODERATE — `README.md:96-99` and `open-growth/proofs.txt:162` — “fewer than 2^(n−3) coincidences” is false; the pattern sum alone is 2^(n−3)+16(k−3) — high — quote the full G4 upper bound
F: MODERATE — `open-growth/proofs.txt:30` — “three independent exhaustive runs” at n = 6; solver B did not rule out non-universal loops (`solver-b/proofs.txt` is not the source: `reports/solver-b.md:18`) — high — “two exhaustive runs (solver A and the referee); solver B only for universal loops”
F: LOW — `open-growth/proofs.txt:149` — a repeat cell meets s in [t+2, t+k−1], which is k−2 indices, not k−1; the written bound stays valid because it is larger — high — “at most k−2”
F: LOW — `open-growth/proofs.txt:89` — “length n (as in Step 5)” cites the n+2 conclusion; length n is the de Bruijn case mult(0^n)=1 — high — “length n, by the same count as Step 5 with mult(0^n)=1”
F: LOW — `open-growth/proofs.txt:108` — “for all n”, while k = n−4 is allowed only for n ≥ 6; for n < 6 the right-hand side is negative — high — “for all n ≥ 6 (and, being negative, for n < 6 as well)”
F: LOW — `open-growth/proofs.txt:146` — “len(w) − #distinct repeats” does not say #distinct n-words; the formula in line 135 does — medium — “len(w) − (number of distinct n-words) repeat cells”

Prior art in `README.md:125-130` matches `prior-art.md:6-8` and `:36-37`. The formal proofs of Theorems 1 and 2, including non-universal w, do support the headlines; the failures are the growth narrative and the “three exhaustive runs” label.
