<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED. I recomputed every entry of solver-a/proofs.txt:5-9 from the letter maps at line 1. All 12 pair images and all 12 round-table entries are correct. — none

Q2: CONFIRMED. solver-a/proofs.txt:11-23 holds. Legality: τ ≥ d+2, so x_t = a for 2 ≤ t ≤ d+1 whatever y_1 is (witnesses.txt:5-7). For y_1 = a, 134·a = 123, 123·a = 12, then {2} or 34. For y_1 = b, 134·b = 14, 14·a = 23, then 12 or 13. Both invariants I and J pass through the a-round table. At τ the image is a singleton or 34, and 34·b = {1}. Singletons stay singletons. The table T(d) = 4 4 6 7 7 9… matches the formula. — none

Q3: CONFIRMED. All 8 family steps at proofs.txt:28-31 check out. The Lemma's induction is sound, including k = 1, where P' = P·x_1·y forces |P·x_1| = 2. The Spoiler's dependence on the whole word is legitimate because the blind word is fixed in advance. First round: Q·a = 123, Q·b = 134. Pairs reachable at turn 2 after a are 12 plus 13/14/34, which contain F4. After b they are 123 and 14, which contain F1. — The README wording "closed under either letter" should read: "each family's successor pairs (letter followed by Spoiler reply) contain another family".

Q4: CONFIRMED. Copying state 1 leaves Q·x_1 unchanged for both letters, so after the first letter every image is the same as in W. This carries both d-winnability and blind non-winnability, including T = 1 (proofs.txt:39-42). Solver B's version works too, because 1 ∈ Q·a and 1 ∈ Q·b (solver-b/proofs.txt:65-67). — none

Q5: CONFIRMED with one wording slip. README:18-27 matches chain/3-question.txt:3-7: Synchronizer moves first, x_t sees y_1..y_(t−1−d), singleton after x_T, uniform T. König (proofs.txt:44-45) is fine. The reduction (proofs.txt:51-56) is correct: when the Spoiler picks y_j, the letters x_(j+1..j+d) depend only on y_1..y_(j−1), so he already knows them. The slip: "(if |q| = d)" should be "after appending, if d+1 letters are pending". — LOW wording

Q6: CONFIRMED. I recomputed the round maps (solver-b:26-27), the main line 123→14→34→13, and facts F1–F3, and traced Lemma B's invariant through every run type. The only false step is "M_(r−1) = 13" (line 36), which fails at r = 2 (y_1 = a, d ≤ 1), where M_1 = 123. It is harmless: S_1 = 123 can never become 12, since 123·aa = {1}. The off-by-one in Lemma 1 (line 12) is real (edge-height + 2) and harmless, since only existence is used. — none

Q7: REFUTED.
- n ≤ 5 table: three programs agree only at n = 5. At n = 4 there are saved logs from two programs (solver-a/dump4.txt tallies to 28960/27952/27808; solver-b/table_py_n4.txt). At n = 3 there is one (verifier/chk_out.txt:9). At n = 2 there are none (solver-a/dump2.txt and dump3.txt are 0 bytes).
- "Why the example works": "b collapses 34 and no other" is true for the letter b. "Any other choice collapses the pair" is true only one round later (12·a = {2}). The phase is fixed by y_1 only for the strategy b a a …. The blind description (README:80-82) ignores words starting with a and later Spoiler choices, which is exactly why Theorem 2 needs families.
- "exactly 96" and the n = 6 column hold as stated.

Q8: REFUTED (the second part). Calling C5 a computation is fair; I recomputed it independently and got 12/12 at n = 2 and 405/405 at n = 3. But the folder holds no n = 2 log at all, and only one program's n = 3 log (verifier/chk_out.txt:9). Solver B says its n = 2..4 tables "were printed to the terminal" (reports/solver-b.md:91).

F: MODERATE — README.md:49 (also referee.md:53) — At least 18 of the 170 "open" n = 5 classes are already proved patient by the existing padding argument. Each has one state outside both letter images whose a- and b-images lie inside the other four states' images. Removing it leaves an automaton in a proved 4-state orbit. Example: verifier/n5_classes.txt `11124 45222`, where state 3 is redundant (I checked all 170 by script). Solver A's own report already says the padded Witness 1 is proved. — high — Fix: "152 classes open; 18 are paddings of the 4-state examples and are patient by the Corollary". If unsure of the exact count: "at most 152 classes open".

F: MODERATE — README.md:44, 47, 92-93 — "three independent programs" for n = 2, 3, and n ≤ 5 generally, is not backed by the saved logs (dump2.txt and dump3.txt are empty; one n = 3 log; two n = 4 logs). — high — Fix: "n = 5: three programs (logs present); n = 4: two saved logs, plus the referee's report; n = 3: one saved log; n = 2: reported, no log saved."

F: LOW — README.md:40, 9 — "both solvers' checkers" did not play this automaton. solver-b/verify.py:7-11 runs B's strategy on C1, an isomorphic copy, not on 2213/4311. — high — Fix: "…and by solver B's checker on an isomorphic copy (C1)".

F: LOW — README.md:72-82 — "Any other choice collapses the pair" is only true one round later. "Picks the first letter that puts the cycle out of phase" oversimplifies, because the Spoiler needs later choices too (proofs.txt:26-37). — high — Fix: "…collapses within a round" and "the Spoiler can always stay out of phase (Theorem 2)".

F: LOW — README.md:41 — "four families of pairs closed under either letter": the families are closed under rounds, and only up to containment. — high — Fix: as in Q3.

F: LOW — README.md:98 — Calls Lemma 1 of solver-b/proofs.txt:9-12 "a remark about T"; it is a lemma marked COMPLETE. — high — Fix: "an off-by-one in Lemma 1 (uniform T)".

F: LOW — solver-a/proofs.txt:55 — "(if |q| = d)" is ambiguous about whether it means before or after appending. — medium — Fix: as in Q5.

F: LOW — README.md:46 — "computed by three programs" for the 96: the referee's n = 4 output is not in the folder (chk_out.txt:10 says "n=4 python recount skipped"). — medium — Fix: "two saved logs plus the referee's report".

Not asked about: the headline itself holds. Both 4-state classes are proved patient, and the orbit claims check out by my script: W1 is in C1's orbit, W2 and C2 share an orbit, and W1 and C2 are in different orbits. Combined with the computed d = 2 count, "exactly 96" follows.
