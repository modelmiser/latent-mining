<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `solver-b/proofs.txt:14-25`, `chain/3-question.txt:8-14` — none
Q2: CONFIRMED — `solver-b/proofs.txt:40-46`, `chain/3-question.txt:2-7,12-14` — none
Q3: CONFIRMED — `solver-b/proofs.txt:47-73`, `chain/3-question.txt:11-14` — none
Q4: CONFIRMED — `solver-b/proofs.txt:75-97`, `chain/3-question.txt:12-14` — none
Q5: CONFIRMED — `solver-b/proofs.txt:40-97`, `verifier/refuter.py:70-200` — none
Q6: CONFIRMED — `verifier/vcheck.py:62-97,150-192`, `solver-b/w_n3_b4_s7.txt:1-26`, `solver-a/w_n3_b4_burns.txt:1-21` — none
Q7: REFUTED — `19-one-counter-mutex/README.md:93-94,100`, `solver-a/proofs.txt:102-104`, `solver-b/proofs.txt:108-111` — Replace lines 93-94 with: "The read decoding logic is sketched by solver A (distinct sums mod 4) and proved algebraically by solver B." Replace line 100 with: "both solvers report these as the least moduli for which this particular construction works."
Q8: REFUTED — `19-one-counter-mutex/README.md:47,100`, `reports/solver-a.md:44,58`, `solver-a/proofs.txt:107-108`, `reports/solver-b.md:67` — In README status table row 9, replace "checked by program for n ≤ 5" with "checked by program for n ≤ 5 (solver B) and n ≤ 4 (solver A)". In line 100, replace "solver B reports" with "both solvers report".

F: MODERATE — 19-one-counter-mutex/README.md:93-94 — The claim that "Why these amounts let each process learn enough from a read is not written out by either solver" is false; solver A gives the sum distinctions (`solver-a/proofs.txt:102-104`) and solver B gives the general decoding proof (`solver-b/proofs.txt:108-111`) — confidence: high — Correct the sentence to credit both solvers' decoding explanations.
F: LOW — 19-one-counter-mutex/README.md:100 — Attributes least moduli (2, 4, 8, 14) reporting solely to solver B, whereas solver A also computed and reported them via `burnsmin.py` (`reports/solver-a.md:44`, `solver-a/proofs.txt:108`) — confidence: high — Change "solver B reports" to "both solvers report".
F: LOW — 19-one-counter-mutex/README.md:47 — Claims sketch verified for n ≤ 5 generally, but solver A's `2^n - 1` sketch was only checked for n = 3, 4 (`reports/solver-a.md:58`) — confidence: high — Clarify that n = 5 was checked for solver B's weights only.
F: LOW — 19-one-counter-mutex/solver-a/proofs.txt:107 — Lists b = 4, 8, 14 under the `2^n - 1` sketch, which actually gives moduli 7 and 15 (`reports/referee.md:55`) — confidence: high — Correct listed moduli to 7 and 15 for `2^n - 1`.
F: LOW — 19-one-counter-mutex/verifier/broken2_progress.txt:1-26 — File intended as a progress test fails on safety (`reports/referee.md:58`) — confidence: high — Fix transition causing safety violation to isolate progress failure.

### Additional Issues Not Covered by the Questions

1. **Unproved General Upper Bounds**: Both solvers state general upper bounds (\(2^{n-1}\) in `solver-b/proofs.txt:106` and \(2^n - 1\) in `solver-a/proofs.txt:106`), but neither proves correctness of the single-read Burns variant for general \(n\); both rely on citations to prior art that used different primitives (atomic fetch-and-store registers).
2. **Missing Solver B Log and State Pruning**: Solver B left no log for its \(n = 3, b = 4, s = 4\) exhaustive enumeration (`reports/referee.md:46`), and omitted raw pair tables (`pairs_b4_s4_p*.txt`); the negative result was rescued solely by the referee recomputing the 16.6M nodes.
3. **Asymmetric State Counts in Headline Witness**: Solver A describes `w_n3_b4_burns.txt` as a "7-state witness" (`reports/solver-a.md:3`), but processes 1, 2, and 3 have 3, 7, and 6 states respectively.
