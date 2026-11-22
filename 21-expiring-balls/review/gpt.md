<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — `21-expiring-balls/README.md:66-74`, `solver-a/proofs.txt:36-42`; in the nonwinning branch, expiry leaves f’s two balls and frees three original bins plus the seventh; P5 merely takes a valid shortcut when x owns the youngest balls — none

Q2: CONFIRMED — `verifier/wcheck.py:37-45,52-63` checks empty, FIFO closure, every offer orbit including fresh bins, and load < L; canonicalization preserves tuple/age order (`wcheck.py:6-14`) — none

Q3: CONFIRMED — stages Ia–II and every forced response follow with 12 bins; the three newest f-balls survive I′ and binary merging uses the eight disjoint bins without expiry (`solver-a/proofs.txt:46-57`, `verifier/replay12.py:68-88`) — none

Q4: CONFIRMED — expiry gives exact older-ball upward closure, offer predecessors are added only after both answers win, and a seed retaining two components has at most n−k−2 additional edges (`verifier/vb.cpp:4-7,37-48`, `solver-b/back2.cpp:22,36-46`); no certificate exists (`reports/referee.md:38-39`) — none

Q5: CONFIRMED — distinct received edges give each load-(k−1) endpoint at least k component vertices, while expiry cannot increase load (`solver-a/proofs.txt:11-15`, `solver-b/proofs.txt:6-11`); binary merging and the free-removal recurrence are complete (`solver-a/proofs.txt:6-27`) — none

Q6: CONFIRMED — FIFO N(4)=12 versus free-removal N(4)=11 supports “exactly one”; k+2^(k−1) is explicitly only a pattern (`README.md:80-87`), and experiment 14 indeed permits reversals but no removals (`14-forest-orientation/README.md:27-35`) — none

Q7: REFUTED — “same winning set” is supported only by equal attractor cardinality (`README.md:46`, `reports/referee.md:35`), and the literature attribution conflicts with `prior-art.md:54`, which says item 2 received only its introduction — “same attractor size; relevant sections were read in items 1, 3 and 5, while item 2’s introduction was read”

Q8: CONFIRMED — the top-level row states the same values and qualification (`README.md:52`), while the folder’s game matches offers, FIFO expiry and value definitions in `chain/3-question.txt:1-8` (`21-expiring-balls/README.md:23-35`) — none

F: CRITICAL — `verifier/vb.cpp:21,50`; `solver-b/back2.cpp:11,48` — unrestricted n is written into fixed 12-element state arrays; vb can overflow for n≥13 and back2 for n≥14 — confidence: high — reject unsupported n or allocate dynamically

F: MODERATE — `21-expiring-balls/README.md:46,94-95` — statewise agreement is claimed although only cardinalities agree; the stored cores even differ (`verifier/out_vb_11_4.txt:2`, `verifier/out_back2_rebuilt_11_4.txt:2`) — confidence: high — say “same attractor cardinality”

F: LOW — `21-expiring-balls/README.md:91-92` — “results are computations” overlooks the explicit hand-checkable adversary strategies — confidence: high — distinguish computational lower bounds from explicit upper strategies

F: LOW — `21-expiring-balls/README.md:101-104` — literature-reading scope misstates `prior-art.md:54` — confidence: high — use the replacement in Q7

Unasked: the unchecked input bounds are the only severe defect found outside the mathematical claims; they do not affect the reviewed n=11 runs.
