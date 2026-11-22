<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — `solver-a/proofs.txt:6-13`; sharper argument uses the removable final firing and discrete filling in `solver-b/proofs.txt:7-12,36-44`; both test immediately after the add — none

Q2: CONFIRMED — identities, nonnegative lag, and \(l_x\le T_x\le3\eta_x^*\) follow exactly as stated in `solver-a/proofs.txt:1-29` — none

Q3: CONFIRMED — a maximum-lag vertex is ready for \(M\ge6\), so the strategy selects lag exactly \(M\); convexity, both firing ranges, pass rounds, and both-step height bounds are correct (`solver-a/proofs.txt:30-41`) — none

Q4: CONFIRMED — the strategy is online, legal, and fires at most once (`solver-a/proofs.txt:21-24,40-48`); taking \(m=\lfloor5h-10\sqrt{5h}-72\rfloor\) gives the condition with 0.2 slack, hence both limits are 5 — none

Q5: CONFIRMED — attempting the interacting list \(0,2,0,2,0,2\) gives after-add maximum lags \(3/5,3/5,6/5,6/5,1,4/5\), with moves pass, pass, 0, 2, 1, pass; \(\Theta\) remains zero, consistent with the implemented rule (`verifier/strat.cpp:20-55`) — none

Q6: CONFIRMED — P2’s survivor is valid (`solver-b/proofs.txt:20-34`); Lemma Z correctly uses restricted lines, minimality, superposition, and connectivity (`solver-b/proofs.txt:82-112`); searches cover \(G(4)=10,G(5)=15\), while only \(15\le G(6)\le20\) is independently reproduced (`reports/referee.md:43-50`) — none

Q7: REFUTED — “the cheapest arrangement puts \(h-1\)” on all five sites is false for arbitrary \(m\), e.g. \(m=h-1\) is cheapest entirely at 0 (`25-throttled-chips/README.md:57-61`); the remaining explanatory claims track `solver-a/proofs.txt:21-45` — “At \(m=5(h-1)\), the cheapest safe arrangement fills \(0,\pm1,\pm2\) with \(h-1\) chips each.”

Q8: REFUTED — the status numbers otherwise match the reports, but `README.md:32` links to nonexistent `chain/3-question.txt`, and `README.md:143-147` contains process claims absent from the cited reports; “The game” does match `chain/3-link.md:5-18` — “The formal text is `chain/3-link.md`; remove unsupported process-history claims.”

F: MODERATE — `solver-a/proofs.txt:6` — Theorem 1 claims \(h\ge1\), which yields the false \(C(1)\le0\) — confidence: high — change to \(h\ge2\)

F: MODERATE — `reports/solver-a.md:3-16` — \(G(6)=20\) is called computed although only width-four lists were exhausted — confidence: high — report only the windowed value and unconditional upper bound

F: MODERATE — `reports/solver-b.md:28-29` — equality \(G=C\) at two finite heights does not contradict unequal asymptotic limits — confidence: high — cite Theorem 2 instead

F: LOW — `25-throttled-chips/README.md:10-14; README.md:55` — “no list does better/as good as anything” overstates a first-order asymptotic result — confidence: high — say “asymptotically optimal to first order”

F: LOW — `25-throttled-chips/README.md:57-61` — cheapest-arrangement wording omits the required \(m=5(h-1)\) scope — confidence: high — scope it explicitly

F: LOW — `25-throttled-chips/README.md:32` — broken formal-text link — confidence: high — link `chain/3-link.md`

F: LOW — `25-throttled-chips/README.md:143-146` — four removed error files and their provenance are unverifiable from repository sources — confidence: high — remove or cite retained evidence

F: LOW — `25-throttled-chips/README.md:147` — only solver B, not the referee report, records an overlapping third process (`reports/solver-b.md:77`) — confidence: high — attribute only solver B

Unasked: none beyond the findings above.
