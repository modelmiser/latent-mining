<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — residue counting and monotonicity establish both inequalities (`02-move-on-free/solver-a/proofs.txt:10-21`) — none

Q2: CONFIRMED — solver A’s gap count bounds extent by ⌊(5M+2)/3⌋ (`solver-a/proofs.txt:29-51`); solver B independently derives the same bound via permanent occupancy above M (`solver-b/proofs.txt:53-83`) — none

Q3: CONFIRMED — every placement of a 2 covers two residue classes, yielding the averaging inequality; no packing/location assumption occurs (`solver-b/proofs.txt:97-118`) — none

Q4: CONFIRMED — opening-time legality gives the three inequalities; positive LP coefficients force the all-tight vertex and maximum 43/108 (`solver-a/proofs.txt:70-81`) — none

Q5: CONFIRMED — run capacities prove L1 (`open-f-lower/proofs.txt:24-35`); after the moved 1 is freed, its destination—including the freed 2-hole—is restored and only its source remains deleted (`open-f-lower/proofs.txt:37-42`) — none

Q6: CONFIRMED — the interior/clean counts, alternating selection, chain waste, pin runs, and arithmetic all hold (`open-f-lower/proofs.txt:65-108`); absent chains and y<3 make the inequalities merely vacuous, and the small-M bound remains valid but trivial — none

Q7: CONFIRMED — from the assumed Step-5 inequality, R’s arithmetic gives 27M/16−43/4 (`open-f-upper/proofs.txt:104-121`), and R′’s two alternatives optimize to M/144 (`open-f-upper/proofs.txt:142-165`); Theorem A is strictly stronger for every consecutive-first-layout manager (`open-f-lower/proofs.txt:65-108`) — none

Q8: REFUTED — the latter two claims follow from Theorem A and P5 (`open-f-lower/proofs.txt:104-108`; `solver-b/proofs.txt:88-90`), but integrality makes the N/O bounds first separate at M=54: ⌈(7M−17)/4⌉=91>⌈5M/3⌉=90, not about 59 (`solver-b/proofs.txt:94-118`) — “The displayed sharp bounds first certify an N/O gap at M=54; 59 comes only from discarding rounding.”

Q9: UNSUPPORTED — the program-count, first-fit, play-out, and open-μF statuses have logs (`reports/referee.md:19-32`; `solver-a/ff_O.log:1-15`; `open-f-lower/thmA_check.log:1-61`; `README.md:48`), but “the right is worth nothing” asserts a comparison never proved (`README.md:35`) — “For compact-first-layout managers, moves do not reduce the proved 7/4 lower-bound slope.”

Q10: CONFIRMED — no additional false COMPLETE theorem or contradiction appears: R/R′ concern the same compact class and are weaker than independently proved Theorem A (`open-f-upper/proofs.txt:132-165`; `open-f-lower/proofs.txt:65-108`) — none

F: MODERATE — `02-move-on-free/README.md:63` — “about 59” misses the sharp crossover M=54 — confidence: high — state 54 or identify the coarse arithmetic

F: MODERATE — `README.md:35` — “worth nothing” overstates a matching lower-bound slope as a value comparison — confidence: high — use the Q9 replacement

F: LOW — `solver-a/proofs.txt:77-81` — the LP exposition omits the one-/two-type cases and writes equality where only a historical-maximum upper bound is needed — confidence: high — add those trivial cases and write H≤6·maxₜN(t)

F: LOW — `open-f-upper/proofs.txt:91,161` — “odd rank” means even-numbered blocks in R but odd-numbered blocks in R′ — confidence: high — use explicit one-based ranks consistently

Unasked: none.
