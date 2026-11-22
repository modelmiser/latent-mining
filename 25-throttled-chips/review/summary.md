<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261120-032221 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $198.21 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q7** (split) | REFUTED | CONFIRMED | CONFIRMED |
| **Q8** (split) | REFUTED | REFUTED | UNSUPPORTED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q7, Q8
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 8 MODERATE, 12 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:6` — Theorem 1 claims \(h\ge1\), which yields the false \(C(1)\le0\) — confidence: high — change to \(h\ge2\)
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `reports/solver-a.md:3-16` — \(G(6)=20\) is called computed although only width-four lists were exhausted — confidence: high — report only the windowed value and unconditional upper bound
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `reports/solver-b.md:28-29` — equality \(G=C\) at two finite heights does not contradict unequal asymptotic limits — confidence: high — cite Theorem 2 instead
- **MODERATE** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:9-10` and `README.md:55` — “no list does better than one point” / “as good as anything” is not proved; the proofs give only G/h→5 and C/h→5, and G may still undercut C by on the order of √h. Equality is marked open at `25-throttled-chips/README.md:47` — high — say the constant list is optimal up to a 10√(5h)+O(1) term, and exact optimality is open
- **MODERATE** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:16` — “Mixing helps only at h=3” is shown for h=4 and 5 and open from h=6 — high — “Mixing does not help at h=4 or 5; from h=6 it is open”
- **MODERATE** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:40` — “every short list” is false of `reports/referee.md:37-38` — high — “on every list in those windows, and on beam-searched lists”
- **MODERATE** [cursor:gemini-3.8-flash-high] 25-throttled-chips/solver-a/proofs.txt:4 — Theorem 1 asserts \(C(h) \le 5h-5\) for every \(h \ge 1\), but at \(h=1\) this asserts \(C(1) \le 0\) when \(C(1) = 1\) — confidence: high — state "\(C(h) \le 5h-5\) for every \(h \ge 2\)" and handle \(C(1)=1\) as a base case
- **MODERATE** [cursor:gemini-3.8-flash-high] 25-throttled-chips/reports/solver-a.md:4 — Solver A reports \(G(6) = 20\) in its headline table as computed, but it only computed it on window width \(W \le 4\); unrestricted \(G(6)\) is only known to satisfy \(15 \le G(6) \le 20\) — confidence: high — qualify table entry as "20 (window \(W \le 4\))" and note unrestricted status \(15 \le G(6) \le 20\)
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/README.md:10-14; README.md:55` — “no list does better/as good as anything” overstates a first-order asymptotic result — confidence: high — say “asymptotically optimal to first order”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/README.md:57-61` — cheapest-arrangement wording omits the required \(m=5(h-1)\) scope — confidence: high — scope it explicitly
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/README.md:32` — broken formal-text link — confidence: high — link `chain/3-link.md`
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/README.md:143-146` — four removed error files and their provenance are unverifiable from repository sources — confidence: high — remove or cite retained evidence
- **LOW** [cursor:gpt-5.6-sol-xhigh] `25-throttled-chips/README.md:147` — only solver B, not the referee report, records an overlapping third process (`reports/solver-b.md:77`) — confidence: high — attribute only solver B
- **LOW** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:45` — the 15 and 16 are finished searches; the timeout is the unfinished length-16 run — high — “15 from the referee’s search through length 14; 16 from solver B’s finished search through length 15; length 16 timed out”
- **LOW** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:147` — the referee report never mentions a third process — high — attribute that only to solver B
- **LOW** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:144-146` — “four one-line error files” is not in either solver report or the referee report — medium — drop it or mark it as not in the reports
- **LOW** [cursor:grok-4.7-medium] `25-throttled-chips/README.md:31` — `chain/3-question.txt` is absent — high — cite `chain/3-link.md`
- **LOW** [cursor:grok-4.7-medium] `verifier/strat.cpp:55` — the numeric height bound is checked only after the firing, not after the add, though `(f)` claims both steps — high — check the bound after step (1) as well
- **LOW** [cursor:gemini-3.8-flash-high] 25-throttled-chips/README.md:31 — Link to `chain/3-question.txt` points to a non-existent file; the formal question was authored in `chain/3-link.md` — confidence: high — update link target to `chain/3-link.md`
- **LOW** [cursor:gemini-3.8-flash-high] 25-throttled-chips/solver-a/strategy_sim.py:46 — Simulation uses random trials reporting max lag 2.2 and height excess 3.0, underestimating worst-case values 2.4 and 3.2 found by referee beam search — confidence: high — add adversarial beam sequences to simulation suite

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
