<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — Full opening makes equal multisets equivalent to rotationally equal rows; local, disjoint ownership gives the stated affine form, with zero entries on agreeing/helper cards; no further condition beyond the finite cyclic layout and binary-card encoding is used (`solver-b/proofs.txt:37-46`, `chain/3-question.txt:1-8`) — none

Q2: CONFIRMED — (*) and row(111) follow by substitution; the nonzero-sum unit-pair lemma is valid, zero sums require even m, zero θ is correctly nonexceptional, any two exceptional pairs contradict the third pair, and both cases yield the same fixed shift t at every Fourier frequency (`solver-b/proofs.txt:45-63`). Periodic W causes no issue: arbitrary chosen shifts satisfy the equations, while period changes vanish wherever Ŵ(r)≠0 — none

Q3: CONFIRMED — The algebra excludes every m; independently, the small search finds no admissible triple through m=18, though that test is vacuous (`solver-b/fullR.out:1-14`, `reports/referee.md:60-61`). If a_i=0 or W is constant the eighth row is immediately W; periodicity is covered by the frequency argument — none

Q4: CONFIRMED — Complementing coordinates preserves the affine form and disjoint supports (`17-cut-and-peek/README.md:121-124`). With remaining bits fixed to 0, exactly 000 is equality-1, so seven equality-0 rows force it into their orbit; for three-bit equality both 000 and 111 are equality-1, so the lemma does not apply (`17-cut-and-peek/README.md:134-141`) — none

Q5: CONFIRMED — The witness has three nonempty players, eight helpers, legal words and |R|=6 (`solver-a/w_n3_m36.txt:1-7`, `verifier/tests.out:16-18`). The checker constructs w[q−j], compares all seven AND-0 Counters, and tests support disjointness (`verifier/vcheck.py:22-41`); the six interleaved length-6 strands are described correctly (`solver-a/proofs.txt:34-41`) — none

Q6: CONFIRMED — T1’s Walsh inversion forces all nonzero parity classes (`solver-b/proofs.txt:22-36`); T4’s degree/sign argument correctly excludes |R|=3,4 (`solver-b/proofs.txt:88-109`). The referee recomputed through 13 but not 14–15, whose two searches share normalization (`reports/referee.md:15-18,63-69`); all m=15 chunks report zero (`solver-b/m15.part*.out:1-8`) — none

Q7: REFUTED — Counts, attributions and restrained novelty otherwise match the reports, but “36 is the least size” in the strand family is asserted despite explicit unsearched sub-36 cases (`17-cut-and-peek/README.md:67`, `reports/solver-a.md:21-28`), and several literature claims are stated directly rather than attributed (`17-cut-and-peek/README.md:160-165,188-190`) — “36 is the least size found in the searched strand cases; overall strand-family minimality remains open. According to the deep-read instance, …”

Q8: CONFIRMED — The top-level row accurately gives full-open impossibility, the 36-card witness and computed lower bound 16 (`README.md:46-48`); shift direction, multiset M(x), privacy and disjoint-support correctness match the formal definition (`17-cut-and-peek/README.md:24-40`, `chain/3-question.txt:1-8`) — none

F: MODERATE — `17-cut-and-peek/README.md:67` — strand-family minimality is presented as fact although cases below 36 were not searched — confidence: high — call it the least found, not the least

F: MODERATE — `verifier/vcheck.py:53-61` — declared n is only printed, not enforced; a three-player witness declaring n=4 can be reported valid — confidence: high — reject when declared and inferred n differ

F: MODERATE — `verifier/tests.py:6-24` — negative tests have no assertions; one of ten objects labelled “must be rejected” is accepted, and the script still succeeds (`verifier/tests.out:8-11`) — confidence: high — assert expected outcomes and relabel the valid swap

F: LOW — `17-cut-and-peek/README.md:160-165,188-190` — outside-paper facts are sometimes voiced as verified facts despite relying on model PDF reads — confidence: high — attribute them explicitly to `prior-art-deep.md`

F: LOW — `17-cut-and-peek/README.md:185-186` — “Nothing is known between 16 and 35” is too absolute given several strand-family exclusions — confidence: high — say no general existence or impossibility result is known

F: LOW — `17-cut-and-peek/README.md:57-58,169-171` — organiser, panel and absence-of-human-review claims are not independently evidenced in repository artifacts — confidence: medium — distinguish documented reviews from process self-report

Unasked: No further material defect found.
