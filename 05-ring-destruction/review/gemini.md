<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `05-ring-destruction/solver-a/proofs.txt:10-42` — none
Q2: CONFIRMED — `05-ring-destruction/solver-a/proofs.txt:44-52` — none
Q3: CONFIRMED — `05-ring-destruction/solver-a/proofs.txt:77-87` — none
Q4: CONFIRMED — `05-ring-destruction/solver-a/proofs.txt:88-100` — none
Q5: CONFIRMED — `05-ring-destruction/solver-a/proofs.txt:103-114` — none
Q6: CONFIRMED — `05-ring-destruction/solver-a/proofs.txt:55-72` — none
Q7: CONFIRMED — `05-ring-destruction/solver-a/proofs.txt:116-125`, `05-ring-destruction/solver-b/proofs.txt:70-84,119-129` — none
Q8: REFUTED — `05-ring-destruction/README.md:73-74` (refuted by `05-ring-destruction/solver-a/proofs.txt:33` and `05-ring-destruction/reports/solver-a.md:54`) and `05-ring-destruction/README.md:41` (refuted by `05-ring-destruction/reports/solver-a.md:28`, `05-ring-destruction/reports/solver-b.md:32`, `05-ring-destruction/reports/referee.md:13-14`) — MODERATE (`05-ring-destruction/README.md:73`): "Each destruction creates at most one new (piece, value) pair, so after t rounds there are at most kt pairs in all across both directions."; LOW (`05-ring-destruction/README.md:41`): "computed by exhaustive search (upper bound NO) and verified strategy witnesses (lower bound YES where search timed out)"

F: MODERATE — 05-ring-destruction/README.md:73-74 — Explanation asserts each destruction creates at most one unit of delay on one piece of information, but one destruction can increase a piece's delay by up to \(H\) (via \(v_p(t) = v_{p-1}(t-1) + 1\)); solver A counts distinct (piece, value) pairs \(|P_A| + |P_B| \le kt\), not delay units, explicitly noting the single-unit delay argument is invalid (reports/solver-a.md:54) — confidence: high — Replace "unit of delay on one piece of information" with "(piece, value) pair"
F: LOW — 05-ring-destruction/README.md:41 — Table status claims entries computed by exhaustive search, but for (11,4) and (12,3-4) YES search timed out (1500s) and lower bounds were supplied by verified strategy witnesses (reports/solver-a.md:28, reports/solver-b.md:32, reports/referee.md:13-14) — confidence: high — Qualify status to note exhaustive search for upper bound NO and witness verification for lower bound YES where search timed out
F: LOW — 05-ring-destruction/solver-b/proofs.txt:147 — Claims F(10,4) is the first unproved entry left open by Theorem U, but Theorem U also fails to exclude \(T=3\) for (5,4) since \(2nh = 20 \le kT(2h+1-m) = 24\), making F(5,4) the true first open entry in B's table (reports/referee.md:33) — confidence: high — Change F(10,4) to F(5,4)
F: LOW — 05-ring-destruction/README.md:70-71 — Hand-waving explanation that \(k \le 2\) has too few chains for ramps to cover the ring; the formal obstruction is that the total pair budget \(2t\) cannot supply the positive deficits required at all \(n\) positions — confidence: high — Rephrase in terms of pair budget versus deficit requirement

### Important matters not asked about

1. **Unreviewed referee proof for \(k \ge n\), even \(n\)** (`05-ring-destruction/reports/referee.md:24-27`, `05-ring-destruction/README.md:42,98`):
   The headline table extends to \(k \ge n\) for even \(n\) (\(F = n-1\) for \(n \le k < 2n\), \(F = 2\lfloor n/2 \rfloor\) for \(k \ge 2n\)) based on an argument derived solely by the referee using A's Lemma L3 (showing all \(2n\) pairs of value \(H = n/2\) would have to be created simultaneously at round \(n-1\), requiring \(2n\) destructions). Neither solver produced or reviewed this proof; it rests on unreviewed referee text and computation up to \(n \le 8\).
2. **Lack of independence in search witnesses** (`05-ring-destruction/reports/referee.md:35`):
   Search witnesses submitted by solver A and solver B for identical problem sizes (e.g., (8,4), (9,3), (10,4)) are byte-identical because both implementations evaluated links in identical lexicographic search orders. They validate survival under the checker but do not provide independent search confirmation.
3. **Absence of formal machine verification**:
   No part of solver A's Theorem U or the referee's extensions has been verified in an interactive theorem prover (Lean, Coq, Isabelle); confidence rests entirely on manual inspection, cross-solver derivation, and test scripts.
