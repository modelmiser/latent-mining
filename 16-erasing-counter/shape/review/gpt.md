<!-- Archived by the organiser on 2026-11-21. Report of the GPT seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — legality, depth 5, L=50, and the 52-cycle follow directly from the model and carry trace; independent outputs also confirm 114>98 and 762>602 (`shape/PROOFS.md:18-35`, `shape/out_cex.txt:1-10`, `shape/out_checks.txt:1-3`); depth≤4 was exhaustive and W₅=3263442 remains untouched (`upper/out_lazy2_r4.txt:1-3`) — none

Q2: UNSUPPORTED — the algebra is correct for the examples, but the stated “general form” assumes only L-tight cycles and omits the necessary designated empty leaf, visited once, that is changed to write z₃ (`shape/PROOFS.md:38-41`) — “Assume the c₁-cycle has a designated empty leaf used exactly once, changed to write z₃; then N=c₃(3c₁+1) and L=(c₁+1)(2c₃+2).”

Q3: CONFIRMED — constant weight forces exactly one read 1 and one write; F2 makes the first spine 1 the erased bit, periodicity preserves the number of spine 1s, and successively fixing the maximum leaves at most k−c+1 states (`upper/PROOFS.md:14-22`, `shape/PROOFS.md:52-66`) — none

Q4: CONFIRMED — pruning preserves path legality; in mode i every pruned spine node is read as zero, H isolates D_i, Theorem E supplies one jump value, and the Ωτ sets give the stated count (`shape/PROOFS.md:72-128`). Mode occurrences may recur, but only their first-occurrence modes are distinct; the count permits recurrence — none

Q5: CONFIRMED — induction supplies c_u≤W_(r−m_u), decreasing weights maximize each suffix product, and telescoping the W recurrence gives W_r; examples (a), (b), and T* have the claimed disjoint data sets (`shape/PROOFS.md:130-158`, `upper/PROOFS.md:153-164`) — “Only the depth≤4 application rests on computation; the conditional general clause rests on its local M(S_i°) bounds.”

Q6: REFUTED — most mathematical statements match the files, but “6,003 random trees of the class” is false: the run has 6,000 random top-level-H trees plus three fixed examples, and the generator does not enforce recursive class C (`shape/test_Z.py:3-10`, `shape/test_Z.py:177-195`, `shape/out_testZ.txt:1`); “for a few hours” is not evidenced by repository files — “6,000 random H-trees plus three fixed class-C examples”; remove or source the elapsed-time claim.

F: MODERATE — `shape/PROOFS.md:38-40` — the general-family formula lacks the required once-used empty exit, so it does not follow from the stated assumptions — confidence: high — add that hypothesis

F: MODERATE — `README.md:67` — 6,003 trees are mislabeled as random class-C trees; only 6,000 are random and only H is generated — confidence: high — describe the actual corpus

F: MODERATE — `README.md:65,154-158` — elapsed-time and single-author provenance are unsupported by repository files — confidence: high — remove or cite provenance

F: LOW — `README.md:67,148-150` — “recursively inside them” omits that recursion is on the pruned S_i° — confidence: high — say “recursively inside the pruned hanging subtrees”

F: LOW — `shape/PROOFS.md:157-159` — wording suggests the abstract general clause rests on computation; only its depth≤4 application does — confidence: high — distinguish theorem from corollary

Unasked: no additional correctness defect found.
