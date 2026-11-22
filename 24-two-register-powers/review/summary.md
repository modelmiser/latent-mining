<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261120-002821 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $198.21 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q4** (split) | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | REFUTED | REFUTED | REFUTED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | REFUTED | REFUTED | REFUTED |
| Q8 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q4
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 9 MODERATE, 9 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `24-two-register-powers/solver-b/proofs.txt:15` — the header omits the factor 2 proved at line 41 — confidence: high — state `N(t)≤2·2^(0.956t)`
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `24-two-register-powers/reports/solver-b.md:52` — the explicit D inequality lacks the required sufficiently-large-m qualification — confidence: high — add that qualification
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `24-two-register-powers/solver-b/potential_check.py:24` — `or True` makes the assertion incapable of failing, while `P` is never updated — confidence: high — remove the dead assertion or update `P` and test exactly
- **MODERATE** [cursor:grok-4.7-medium] `24-two-register-powers/README.md:40` — “both solvers” computed D(m) for every m≤4100, but solver B stops at 4096 — high — attribute 4097..4100 to A and the referee only
- **MODERATE** [cursor:grok-4.7-medium] `solver-b/proofs.txt:15` — the header states N(t)≤2^{0.956 t}; the proof only reaches 2·2^{0.956 t} (`solver-b/proofs.txt:41`) — high — put the factor 2 in the header
- **MODERATE** [cursor:grok-4.7-medium] `24-two-register-powers/README.md:16-18` — Erdős’s almost-all theorem (`prior-art.md:7`) does not prove the worst-case unlimited-memory ratio is 1; that needs l(n)=log₂n+o(log n) for every n — medium — cite a uniform bound
- **MODERATE** [cursor:grok-4.7-medium] `solver-b/potential_check.py:24` — `assert abs(phi-P)<1e-6 or True` is always true, and the recursive call never updates P (`potential_check.py:30`) — high — delete `or True` and pass the updated potential
- **MODERATE** [cursor:gemini-3.8-flash-high] 24-two-register-powers/README.md:40 — Claims D(m) for 2..4100 was computed by both solvers, but solver B stopped at m = 4096 (`solver-b/D_table.txt`, `reports/solver-b.md:14`) — confidence: high — Correct attribution to solver A and referee to 4100, solver B to 4096.
- **MODERATE** [cursor:gemini-3.8-flash-high] 24-two-register-powers/solver-b/potential_check.py:24 — Tautological assert `assert abs(phi(x,y)-P)<1e-6 or True` with unupdated P is incapable of failing — confidence: high — Remove `or True` and pass `phi(nx,ny)` into recursive call.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `24-two-register-powers/README.md:16-18` — an almost-everywhere theorem is insufficient for the worst residue, although the conclusion is true by Brauer’s uniform bound — confidence: high — cite Brauer’s every-n bound
- **LOW** [cursor:gpt-5.6-sol-xhigh] `24-two-register-powers/README.md:40` — solver B’s table ends at 4096, leaving the attribution false for 4097–4100 — confidence: high — qualify each range
- **LOW** [cursor:grok-4.7-medium] `24-two-register-powers/README.md:111-112` — a second omitted file, “a test copy” over 300 kB, is not documented; the reports name only paths_m400.txt — medium — drop the test-copy sentence
- **LOW** [cursor:grok-4.7-medium] `24-two-register-powers/README.md:53-55` — “four cases” skips the split of type B into r≤1/2 and r>1/2 (`solver-b/proofs.txt:25-27`) — high — “four types, five calculations”
- **LOW** [cursor:grok-4.7-medium] `24-two-register-powers/README.md:67-68` — “gains less on average” is a uniform ≤3/4 cap, not an average (`solver-b/proofs.txt:23`) — medium — “raises Φ by at most 3/4”
- **LOW** [cursor:grok-4.7-medium] `README.md:52` — “tables computed three times” fails for N(22) and for D(m), m=4097..4100, which B did not compute — medium — “N(t) three times through 21; D(m) three times through 4096”
- **LOW** [cursor:gemini-3.8-flash-high] 24-two-register-powers/README.md:17 — Justifies worst-case residue ratio 1 via Erdős (1960) ("almost every number"), which does not bound exceptional worst-case residues — confidence: high — Cite Brauer (1939) or window addition chains reaching every n in log2 n + o(log n).
- **LOW** [cursor:gemini-3.8-flash-high] 24-two-register-powers/solver-b/proofs.txt:14 — P2 header claims `N(t) <= 2^(0.956 t)` omitting the multiplicative factor 2 proved on line 41 (`2 * 2^(0.956 t)`) — confidence: high — Add factor 2 to header or adjust eta to 0.0442.
- **LOW** [cursor:gemini-3.8-flash-high] 24-two-register-powers/solver-b/proofs.txt:15 — Explicit non-asymptotic lower bound `D(m) >= (log2 m - 1)/0.956` is stated without noting it only holds for t >= 9500 — confidence: high — Add explicit qualification "for large m (t >= 9500)".

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
