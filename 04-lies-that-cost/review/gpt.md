<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — `chain/3-question.txt:3-9`; transition, dead elements, ell=0, and horizon clipping are derived in `solver-a/proofs.txt:9-19` — none  
Q2: CONFIRMED — the stopping rule guarantees ell≥1, and obligation ≥2 prevents a second lie within the triple; each round halves survivors (`open-upper/proofs.txt:16-27`) — none  
Q3: CONFIRMED — both parity constructions, d bounds, weight balance, 2ell≥j−1, exact blocked transition, and niceness are established in `open-upper/proofs.txt:30-51` — none  
Q4: CONFIRMED — the cases partition m≥2; the threshold proof covers j≥15, with T1 covering j≤14, and illegal empty outcomes are excluded (`open-upper/proofs.txt:68-77`) — none  
Q5: REFUTED — T1/T2 and the k≥13 reduction are precise (`open-upper/proofs.txt:53-62`), but the cited independent log starts at k=4 and its k=2,3 wide runs stop at j=12 (`open-upper-referee/run1.txt:1-25`) — “The author’s output covers T1; the saved referee T1 log covers k≥4, while k=2,3 need saved j=13,14 runs or an explicit citation to `refcmp_out.txt`.”  
Q6: CONFIRMED — the root outcomes, P bounds, parity-specific weights, and n=3,5 exceptions are all checked in `open-upper/proofs.txt:83-99` — none  
Q7: CONFIRMED — one-lie plays satisfy rule (ii), weight is conserved, and the odd first-question imbalance contributes q−1 (`solver-a/proofs.txt:20-40`; `solver-b/proofs.txt:30-39`) — none  
Q8: REFUTED — the T1 “two programs” status lacks the cited k=2,3 j=13,14 referee output (`README.md:45`; `open-upper-referee/run1.txt:1-25`), and “two candidates can be kept alive forever” is unqualified despite finite k=1 cases (`README.md:50,71`) — “Qualify the T1 cross-check and say that in the n=2 game two candidates can be maintained forever; the remaining status and prior-art wording stands.”

F: MODERATE — `README.md:45` — the cited logs do not substantiate independent T1 coverage for k=2,3 at j=13,14 — confidence: high — save those runs or narrow the claim  
F: LOW — `README.md:71` — the k=1 sentence can be read as applying for every n, contradicting finite Q_1(8) — confidence: medium — specify “when n=2”  
F: MODERATE — `open-upper-referee/ref.cpp:15` — memo keys truncate every count to one char, so counts such as 44 and 300 collide and the n≤300 root sweep can reuse another state’s verdict — confidence: high — encode full-width integers in the key  

Unasked: the non-injective referee memo key makes the ancillary large-root results in `run1.txt` unreliable; it does not affect T1, whose states have at most 25 candidates.
