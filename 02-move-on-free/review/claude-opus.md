<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED — solver-a/proofs.txt:9-21. Since g(K+1)−g(K) ≥ 0, K+t ≥ L(M), each 3 needs a free class-q cell, and the algebra (5M+2r−14)/3 checks. — none

Q2: CONFIRMED — A: solver-a/proofs.txt:29-51. Facts (a)–(c) hold even though the extent can shrink; f ≤ (s−1)(M+1)/s; floor((5M+2)/3)=ceil(5M/3). B: solver-b/proofs.txt:60-83. (I2) holds; o ≥ ceil((M−2)/3); 2M−floor(M/3)=floor((5M+2)/3); the M=3 case is handled. solver-a/ff_O.log gives ceil(5M/3) for M=6..16 and ceil−1 for M=3..5, as claimed. — none

Q3: CONFIRMED — solver-b/proofs.txt:98-117. Every 2 covers at least one of classes a, a+1, so y_a+y_{a+1} ≥ y. A 2 covers at most one class-r cell. Stage-3 legality holds. The chain of inequalities ends at 7M/4−17/4. Nothing is assumed about where the 2s go. — none

Q4: CONFIRMED — solver-a/proofs.txt:71-81. Type counts cannot rise after τ_s, so the counts at τ_b and τ_c are ≥ n_a and n_b. 6(n_s−1)+s ≤ M gives M′=M+5. With n_c eliminated, the coefficient of n_a is (36−12a+ab)/36 > 0, so the all-tight vertex is the maximum. Over the six orderings the values are 86, 81, 80, 72, 69, 66, so (1,2) is the largest. The lowest empty index is ≤ the number of non-empty hexads, so H ≤ 6·43M′/108 = 43M/18+11.94. — none

Q5: CONFIRMED — open-f-lower/proofs.txt:28-41. In L1, t ≤ Σfloor(l_i/3) gives H ≥ M−2+W. In L2, freeing a 2 allows at most one 1 to move. Freeing that 1 is always legal, grants nothing, and leaves the cell set "before the move minus the moved 1's source cell". That holds when the 1 lands in the freed hole too: the hole is free again. — none

Q6: CONFIRMED — open-f-lower/proofs.txt:72-107. At least y−3 2s are interior in every case (l=0 gives y−2; r ≤ 1 gives ≥ y−2). Unclean interior 2s are ≤ 2k, and |F| ≥ (y−3−2k)/2. F members are never adjacent, so the merged runs (≡2 mod 3) are distinct and do not touch the k runs. Pin runs are ≡2. The case split gives k+|F| ≥ (y−3)/2. The final arithmetic gives 7M/4−5.75. If x′_r=0, y<3 or a chain is empty, the terms only weaken harmlessly. For M ≤ 4 the bound is ≤ 0, so it is true and vacuous; at M=3 thmA_check.log shows extent 3 against a bound of −1. — none

Q7: CONFIRMED — open-f-upper/proofs.txt:104-120 and 152-164. Hand-checked: 81/12 = 27/4; 81M/48 − 43/4 ≥ 27M/16 − 11; the R′ extent ≥ 2M−v+W−5; W ≥ 2(|F|−b); max(b, n2/8−2b) ≥ n2/24. On the implication: R′'s managers are a subset of Theorem A's, and 7M/4−7 ≥ 241M/144−12 for every M ≥ 0. R's managers are defined as putting the first M ones on 0..M−1 (line 82), so the same holds. — none

Q8: CONFIRMED with imprecision — 7M/4−17/4 > (5M+2)/3 ⇔ M > 59. With integer rounding, ceil(7M/4−17/4) > ceil(5M/3) first at M=54 (91 vs 90), again at 57 and 58, and for all M ≥ 60. Theorem A: M/12 > 7 ⇔ M > 84, correct. floor(7M/4)−1 matches the table for M=3..12. It exceeds B(M) at 24, 27, 28 and every M ≥ 30, since the difference is ≥ (M−29)/12. So "every M > 32" is true but too weak. — Replace with: "first separate at M = 54, and for every M ≥ 60"; "wrong at M = 24, 27, 28 and every M ≥ 30".

Q9: REFUTED — Most rows are supported: verifier/table_*.log reaches 11; solver-a t12_*, t13*.log and solver-b out*.txt cover 12–13 (F at 12 by sandwich in A); ff_O.log covers M ≤ 16; thmA_check.log has 58×5 = 290 games and 0 violations. The overclaim is README.md:6 and ../README.md:33, "the right is worth nothing to an allocator that packs". Theorem A only shows that such an allocator still needs 7M/4−7 in F, which equals the N *lower bound*. It does not show its F cost equals its N cost. Counterexample: first fit with no moves reaches 11M/6 against script S, and with compaction 7M/4 (open-f-upper/proofs.txt:63-64), so for first fit the right is worth something. μ^F is stated as open everywhere it should be. — Replace with: "an allocator that packs its first blocks together still needs 7M/4 − O(1) cells with the move right, the same slope as the proved no-move lower bound".

Q10: CONFIRMED (no contradiction, minor issues) — The two files use different adversaries. R's dp4 values (159 at M=96) sit below Theorem A's 161 against the same managers, which is consistent because P6R deletes the moved 1s. The problems found are listed below.

F: MODERATE — README.md:6, ../README.md:33 — "the right is worth nothing to packing allocators (proved)" overstates Theorem A; first fit goes from 11/6 without moves to 7/4 with compaction — high — use the Q9 wording.
F: MODERATE — open-f-lower/proofs.txt:8-10 — headline says Theorem A covers every manager "whose first M 1s leave no room for a 2"; the hypothesis is about the layout after step 2. Counterexample: 1s on even cells; freeing one class opens 3-cell gaps, so the hypothesis fails — high — restate it with the after-step-2 hypothesis (line 66).
F: LOW — README.md:67-68, solver-b/proofs.txt:89 — "every M > 32" should be "M = 24, 27, 28 and every M ≥ 30" — high — reword.
F: LOW — README.md:64-65 — "about 59": integer bounds already separate at M=54 — high — give 54 and 60.
F: LOW — README.md:46 — R′ row omits "M ≡ 0 (mod 12)" — high — add it.
F: LOW — open-f-upper/proofs.txt:91 vs 161 — "odd rank" means 2nd, 4th, …, last in R but 1st, 3rd, … in R′ — high — use one convention.
F: LOW — open-f-lower/proofs.txt:66-69 — the theorem says "P6R forces", but the proof uses a "proof version" of step 4 that differs from the coded step 4 (line 56) — medium — name the proof version in the statement.
F: LOW — open-f-upper/proofs.txt:9-12, 17 — "invisible for M ≤ 192" and "greedy rules reach 1.70M" hold only for that adversary; Theorem A forces ≥ 161 > 160 at M=96 and ≥ 1.75M − 7 on these first-fit managers — medium — say "against this adversary".
F: LOW — ../README.md:33 — "at least 7/4 of live memory" is asymptotic; the proved bound is 7M/4 − 17/4 — low — say "asymptotically".

Not asked about:
- "Nothing machine-checked" is accurate, but R and R′ rest on cyc.cpp, which only its author ran.
- One of the five Theorem A test managers never uses moves ("FF(no moves)"), so effectively four managers test the F-specific part.
