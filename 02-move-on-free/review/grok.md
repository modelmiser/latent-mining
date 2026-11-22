<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `solver-a/proofs.txt:8-21` — none
Q2: CONFIRMED — `solver-a/proofs.txt:29-51` and `solver-b/proofs.txt:60-83` (same bound; `floor((5M+2)/3)=ceil(5M/3)`) — none
Q3: CONFIRMED — `solver-b/proofs.txt:98-117`; no placement assumption: every 2 meets `{a,a+1}`, so some `r` has `2x_r+y_r ≥ X+y/2` — none
Q4: CONFIRMED — `solver-a/proofs.txt:76-81`; the three inequalities hold with `M'=M+5`, the objective rises in both variables so the maximum is all-tight, and `(1,2)` gives `86/216`; `H ≤ 6·(43/108)(M+5)=43(M+5)/18 ≤ 43M/18+12` — none
Q5: CONFIRMED — `open-f-lower/proofs.txt:24-41`; freeing the moved 1 is legal, and if it lands in the hole just freed, freeing it empties that cell again, so the layout is the pre-reply layout minus that one 1 — none
Q6: CONFIRMED — `open-f-lower/proofs.txt:72-107`; interior count is `max(l-1,0)+max(r-2,0) ≥ y-3` (empty chain, `y<3` included); if `y-3-2k<0` then `k>(y-3)/2`; the chain yields `H ≥ 7M/4-5.75`, hence `≥ 7M/4-7`; at `M=3` the claim is `H≥-1.75` — none
Q7: CONFIRMED — `open-f-upper/proofs.txt:90-120,146-164`; Steps 1–4 and 6 and the R′ gap algebra check, and `-43/4` weakens to `-11` and `-47/4` to `-12`; `7M/4-7` is at least both R and R′ for every `M≥1`, and both classes put the first `M` 1s on consecutive cells, so A implies those cell bounds — none
Q8: REFUTED — `02-move-on-free/README.md:64-68` against `solver-b/proofs.txt:94-96` and P5’s `B(M)=floor((5M+2)/3)`; the other two claims hold (`7M/4-7>5M/3` iff `M>84`; `floor(7M/4)-1` matches the table for `M≤12`, and `41>40=B(24)`, and for `M>32`, `7M/4-2>(5M+2)/3`) — the proved N-vs-O gap already appears at `M=54` (`7·54/4-17/4=90.25>90=ceil(5·54/3)`), not only after about 59; it is forced for every `M>59`
Q9: CONFIRMED — table row `02-move-on-free/README.md:39-48` matches `verifier/table_3_8.log`, `table_9_11.log`, `solver-a/ff_O.log:1-15` (`M=3..16`), `open-f-lower/thmA_check.log:1-61` (five managers, `M=3..60`, 0 violations), and both READMEs leave `μ^F` open; `h^F(12)=20` is forced by solver A’s `h^O(12)=h^N(12)` (`reports/solver-a.md:12`) and computed directly in `solver-b/outF10_12.txt:8-10` — none
Q10: CONFIRMED — no cross-file contradiction: A’s `7M/4-7` and R/R′ are different adversaries, and the dp4 extents (`open-f-upper/dp4_all.log`) sit under A without claiming to meet P6R — none

F: MODERATE — `02-move-on-free/README.md:64` — “cannot show before M is about 59” is false: P6 vs `ceil(5M/3)` separates at `M=54` (`90.25>90`) — high — say the bounds first force `h^N>h^O` at `M=54`, and for every `M>59`
F: LOW — `open-f-upper/proofs.txt:11` — “NEGATIVE for `M≤192`” but `dp4_all.log:14` has the theorem row at `M=192` equal to `5M/3` (`320-320=0`) — high — “negative for `M<192`, zero at `192`”
F: LOW — `reports/solver-a.md:11-12` — “all entries are COMPUTED” by exhaustive fixpoint, then `h^F(12)` and two `M=13` losses are inferred — high — say those three cells are inferred from `h^O≤h^F≤h^N`
F: LOW — `open-f-upper/proofs.txt:10` — “first exceeds `5M/3` at `M=384`” is only sampled at `48,96,192,384,…` (`dp4_all.log`); intermediates such as `288` are absent — medium — “first computed point past `192` is `384`”

The dp4 minimum at `M=96` is `158` (`dp4_all.log:11`) while Theorem A requires `161` against P6R. That is not a contradiction: R optimizes moves against one script, and A is a different adversary. Solver B’s `5M/3-4` (`solver-b/proofs.txt:8-23`) is a valid stronger sibling of A’s `(5M-14)/3`, not a conflict.
