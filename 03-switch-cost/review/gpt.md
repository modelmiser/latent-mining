<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — arrivals during execution are handled by proving they were released after the blocking job was selected; the bound is \(C\le d_j+p_k-1\), and same-cost adversaries form a subset of free-promise adversaries (`03-switch-cost/solver-b/proofs.txt:50-71`) — none

Q2: CONFIRMED — running the first job causes the tight unit to miss; declining leaves four units for three ticks in S, and three units for two ticks in F (`03-switch-cost/solver-b/proofs.txt:80-90`) — none

Q3: CONFIRMED — stalls contribute \(j+1\) excluded ticks, saturation converts excluded ticks to lateness, and the lock/downward-induction cases cover refusal, departure after one unit, finishing, and idling (`03-switch-cost/open-f/proofs.txt:34-46`, `03-switch-cost/open-f/proofs.txt:48-86`) — none

Q4: REFUTED — CASH-IN always says to release “one job of length f mod P”; for P=2, running stream units at ticks 0–1, K at 2, then an empty tick at 3 reaches final cash-in at t=4 with f=2, hence releases a forbidden zero-length job (`03-switch-cost/open-s/proofs.txt:75-78`; legality requires p≥1 at `03-switch-cost/README.md:17-18`) — “release floor(f/P) length-P jobs and, only if f mod P>0, one remainder job”; with that repair, the remaining Round Lemma and promise argument hold

Q5: UNSUPPORTED — Q1 supplies the upper bound, but the cited Q4 rule is formally illegal on the cash-in path above (`03-switch-cost/README.md:53-56`; `03-switch-cost/open-s/proofs.txt:75-78`) — “After omitting a zero remainder job in ADV(P), Q1 and Q4 imply S(P)=F(P)=P−1”

Q6: CONFIRMED — Corollary A′ follows by applying Theorem A at the smaller parameter, while Theorem B establishes distinct trap ticks, legal last-trap timing, and filler feasibility (`03-switch-cost/open-f/proofs.txt:98-119`) — none

Q7: REFUTED — the displayed S₉(4)=2 is correct, but the three named sources provide only its lower bound; the unrestricted upper run is instead in `solver-b/table_run_S4_L2_full.txt:1-9`, while the referee explicitly did not check it (`03-switch-cost/reports/referee.md:74-77`) — retain “2” but cite `solver-b/table_run_S4_L2_full.txt`; all F entries satisfy Corollary A′

Q8: REFUTED — the Results claim “Smallest horizon … is P(P+1)/2” is categorical although minimality is explicitly unproved beyond P≤4 (`03-switch-cost/README.md:55-57`; `03-switch-cost/open-f/proofs.txt:122-126`) — “P(P+1)/2 suffices generally and is the smallest horizon for P=2,3,4; general minimality is open”

Q9: CONFIRMED — apart from the defects above, the proof files and reports preserve the distinctions between proved, computed, sampled, and open claims (`03-switch-cost/open-f/proofs.txt:121-126`; `03-switch-cost/open-s/proofs.txt:219-224`) — none

F: MODERATE — `03-switch-cost/open-s/proofs.txt:75-78` — cash-in can release a zero-length job, making ADV(P) illegal as written — confidence: high — omit the remainder job when it is zero

F: LOW — `03-switch-cost/README.md:55-57` — general minimality of the triangular free-promise horizon is worded as fact — confidence: high — label it sufficient/conjecturally minimal

F: LOW — `03-switch-cost/README.md:47-49` — the table’s cited sources omit the unrestricted upper computation establishing S₉(4)=2 — confidence: high — cite `solver-b/table_run_S4_L2_full.txt`

F: LOW — `03-switch-cost/open-f/proofs.txt:23-27` — \(a_j\) is undefined if J_j is never run, though the intended stall rule and code make the construction total — confidence: medium — define \(a_j=\infty\) when absent

F: LOW — `03-switch-cost/open-f/proofs.txt:98-100` — Corollary A′ omits that k is a nonnegative integer and its one-line proof does not state the trivial k=0 or k≥P−1 branches — confidence: high — state the domain and split those cases

Unasked: none.
