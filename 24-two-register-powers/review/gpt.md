<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — `solver-b/proofs.txt:17-34`: A has `(M',r')=(2M,r/2)`; B has `(M,2r)` or `(2rM,1/(2r))`; C has `((1+r)M,r/(1+r))`; D has `((1+r)M,1/(1+r))`; ties merely duplicate types; gains are ≤1 and ≤3/4, and Φ(1,1)=0 gives (*) including the start — none

Q2: CONFIRMED — `solver-b/proofs.txt:35-41`: (*) gives `k<4(j−log₂X)+2`; the binomial/entropy count is valid for large t, ordered-relative types determine the next unordered state, each state contributes ≤2 integers, the exponent is 0.95020, and `m≤N(D(m))` because one distinct minimizing positive integer is chosen from each residue class — none

Q3: CONFIRMED — `solver-a/proofs.txt:15-38`: deleting doublings and inducting proves P4; A/B commute within each addition-delimited gap, yielding `2^j C(k+2j+1,2j+1)` classes; the two exponents are set up correctly and cross at `d≈0.1206975`, `γ≈0.9630958` (`reports/referee.md:9-12`) — none

Q4: REFUTED — the bounds, residue-0 construction, and lifting are proved at `solver-a/proofs.txt:3-14`, but `README.md:16-18` invokes an “almost every n” theorem, which does not imply a worst-residue bound; the necessary uniform theorem is instead stated at `chain/1-link.md:3` — “With unlimited memory the ratio is 1 because Brauer’s uniform bound gives l(n)=log₂n+o(log n) for every n.”

Q5: REFUTED — the displayed values match `solver-a/nt22.txt:1-6` and `solver-a/D_all.txt:1-7`, but solver B computed D only through 4096 (`reports/solver-b.md:15`), not 4100 as claimed at `README.md:40` — “computed by solver A and both referee methods through 4100, and by solver B through 4096”

Q6: CONFIRMED — `README.md:48-76` accurately summarizes P2’s potential/counting argument and P4–P5; “commute” is valid when read as P5’s commutation of the two doublings within each gap (`solver-a/proofs.txt:21-23`) — none

Q7: REFUTED — every Status cell and distrust bullet matches its sources except the 4100 attribution at `README.md:40` versus solver B’s 4096 limit at `reports/solver-b.md:15`; the novelty wording remains appropriately “not found” (`prior-art.md:14-18`) — use the corrected attribution from Q5

Q8: CONFIRMED — the top-level summary at `README.md:52` matches the two proved constants, classical-method qualification, repeated computations, and open exact constant documented in the folder — none

F: LOW — `24-two-register-powers/README.md:16-18` — an almost-everywhere theorem is insufficient for the worst residue, although the conclusion is true by Brauer’s uniform bound — confidence: high — cite Brauer’s every-n bound

F: LOW — `24-two-register-powers/README.md:40` — solver B’s table ends at 4096, leaving the attribution false for 4097–4100 — confidence: high — qualify each range

F: MODERATE — `24-two-register-powers/solver-b/proofs.txt:15` — the header omits the factor 2 proved at line 41 — confidence: high — state `N(t)≤2·2^(0.956t)`

F: MODERATE — `24-two-register-powers/reports/solver-b.md:52` — the explicit D inequality lacks the required sufficiently-large-m qualification — confidence: high — add that qualification

F: MODERATE — `24-two-register-powers/solver-b/potential_check.py:24` — `or True` makes the assertion incapable of failing, while `P` is never updated — confidence: high — remove the dead assertion or update `P` and test exactly

Unasked: none beyond these findings.
