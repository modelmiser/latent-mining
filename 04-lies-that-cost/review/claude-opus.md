<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED — `solver-b/proofs.txt:67-79` and `solver-a/proofs.txt:13-16`: by (i), the answers fix L for each x. A candidate whose obligation r_x(t−1)=0 may lie; then D ≤ t−1 < t and its new obligation is k·ell_t. ell counts only alive elements (`:80`, `:12`). A question with ell=0 gives a liar an empty window. With r ≥ j, every remaining question t+1…t+j lies inside the window that ends at t+r, so there is no off-by-one. Lemma S(ii) uses the matching convention: j−1 questions remain after the question. — none

Q2: CONFIRMED — `open-upper/proofs.txt:18-26`. Every question is asked only while both sides hold an alive candidate, so ell ≥ 1 and each lie leaves an obligation of at least 2. Each candidate therefore lies at most once in any 3 consecutive questions, and the majority answer is its true one. The round stops early only if the alive set already sits inside one side, so at most ⌈m/2⌉ survive. h(m) ≤ 3+h(⌈m/2⌉) gives 3⌈log2 m⌉. — none

Q3: CONFIRMED — `proofs.txt:37-51`.
- a even: the weight difference is b1−b2 ∈ {0,1}, and 2ell ≥ m−1 ≥ j−1.
- a odd: niceness gives b ≥ j−1, so 0 ≤ d ≤ b. j=1 is excluded correctly: a odd and w ≤ 2 force (1,0), which has m=1.
- The weight difference is (j−1)−d ∈ {0,1}, and 2ell = m−|d−1| ≥ j.
- (ii): a liar's obligation k·ell ≥ j−1 equals the questions left, so liars become blocked, blocked liars die, and the transitions are exact.
- (iii): niceness holds on both sides.
- Ran no code; lemmas.py output, run2.txt:65-74, agrees. — none (but see F3)

Q4: CONFIRMED — `proofs.txt:70-76`. The cases m ≥ 2j−2 and m ≤ 2j−3 split every state. Cases j=0,1 hold (2j−3 < 2 there). For j=15–17, ⌈log2 27…31⌉=5 gives 15 ≤ j. For j ≥ 18, 2j−3 ≤ 2^⌊j/3⌋ holds, and the induction step (+6 on the left, doubling on the right) is valid. An answer that empties the alive set is illegal and skipped, also in code (`table.cpp:29`). — none

Q5: CONFIRMED with a gap — T1 is stated exactly at `proofs.txt:55-58`. The k ≥ 13 reduction holds: k·ell ≥ 13 ≥ j−1, and ell=0 gives obligation 0 for every k. I recounted the states: 960 for T1, 1310 with a+b ≤ 2j at j ≤ 14, and 1884 at j ≤ 16 — all as claimed. `table_out.txt:1-15` covers k=2..16 with 0 lost; `:24-29` covers T2. The gap: `run1.txt:1-13` has T1 only for k=4..14, 20, 1000. There is no k=2 or k=3 line, although `reports/open-upper-referee.md:42` claims k=2..14. — The README should cite `open-upper/refcmp_out.txt` for k=2,3, or rerun ref.cpp and save the log.

Q6: CONFIRMED — `proofs.txt:87-99`.
- n(n+2) ≤ 2^{n+1} for n ≥ 2, so P(n) ≤ n+1 for even n.
- n²+2n−1 ≤ 2^n for n ≥ 7, so P(n) ≤ n for odd n ≥ 7.
- Even n: weight n(q+1)/2 ≤ 2^{q−1}. Odd n: the "yes" weight is (n(q+1)+q−1)/2 and "no" is lighter by q−1. Both match the definition of P.
- I recomputed P(3)=5 and P(5)=6; the exception weights 20, 15, 11 and 7 are correct.
- T2 is in `table_out.txt:24-29` and run1.txt:51-53. — none

Q7: CONFIRMED — `solver-a/proofs.txt:21-38` and `solver-b/proofs.txt:87-102`. The weight is conserved across the two answers. The odd-n refinement checks out: a′(q−1)+n ≥ (n(q+1)+q−1)/2. — none

Q8: REFUTED (minor overstatements) — `README.md:69` cites run1.txt for k=2..13, but run1 lacks k=2,3. `README.md:82-84` omits Lemma S's hypotheses. `README.md:81` says "more answers than remain", but the proof only gives "at least as many". The prior-art paragraph is consistent with `prior-art.md:5,31-33`. — see the F lines

F: MODERATE — README.md:69, open-upper-referee/run1.txt:1 — The T1 row claims a second-program check for k=2..13 and cites run1.txt. That log has no T1 run for k=2 or k=3, the headline case; "wide" (run1.txt:27) reaches only j ≤ 12. The independent evidence for k=2,3 at j=13,14 is only `open-upper/refcmp_out.txt`, which compares against a one-lie DP and comes from a different program. — high — Cite refcmp_out.txt for k=2,3, or rerun `ref t1 2` and `ref t1 3` and save the output.

F: MODERATE — README.md:82-84 — The text says "Lemma S shows such a question exists… whenever there are at least 2j−2 candidates". It drops the hypotheses b ≥ a−1, weight ≤ 2^j and m ≥ 2, and the niceness hypothesis is needed (run2.txt:72 fails without it). — high — "…whenever the position is nice (b ≥ a−1), within the weight bound a(j+1)+b ≤ 2^j, and has at least 2j−2 (and at least 2) candidates."

F: LOW — open-upper/proofs.txt:31-50 — Lemma S never states ell ≥ 1. It is not needed, because k·ell ≥ 2ell ≥ j−1 is all (ii) uses, but the proof reads "obligation k·ell" as "blocked". — medium — Add "(if j ≥ 2 then ell ≥ 1)".

F: LOW — README.md:79-81 — "that is more answers than remain in the game" is stronger than what the proof gives: k·ell ≥ j−1 can hold with equality. — high — "at least as many answers as remain".

F: LOW — README.md:94-95 — "two candidates can be kept alive for ever" reads as general for k=1, but Q_1(8)=6 and Q_1(n)=P(n) for 10 ≤ n ≤ 700. — medium — "…and for n ∈ {2,…,7,9} the answerer can keep two candidates alive for ever."

F: LOW — README.md:36-37 — "proved by hand apart from a finite table" omits T2: the n=3,5 roots also rest on computation. — high — "…apart from a finite table (T1) and three small root cases (T2) settled by program."

F: LOW — README.md:69 vs open-upper/proofs.txt:3,62-63 — The two files name different second programs: the README says the reader's ref.cpp, while proofs.txt says the earlier referee's verifier/ms. — high — Name both programs and the k ranges each covers.

F: LOW — open-upper/table_out.txt:16-20 — The log is filtered: the k=1 control reports lost=30 but shows only 2 LOST lines, and per-j lines are missing for k ≥ 2. — medium — Save the unfiltered log, or say the log is filtered.

F: LOW — README.md:103-105 — "checked against the literal definition only for n ≤ 4" understates the evidence: 4,000 random histories with n ≤ 8 were also run (reports/referee.md:11). — high — "exhaustively for n ≤ 4, randomly for n ≤ 8".

F: LOW — README.md:67-68 — "I checked it by hand" is unverifiable, since no trace is saved. — low — Drop it or attach notes.

F: LOW — README.md:73 — The "Where" cell for the Q_1 = P row is empty. — high — Cite reports/referee.md:24 and the solver-a/solver-b k=1 output files.

Not asked:
1. The whole upper bound rests on the obligation-multiset reduction, and the literal checks run only to n ≤ 5. The reduction's hand proof is sound, so the risk is low.
2. Corollary 1a (`proofs.txt:78-80`) leans on solver-a P3′ for a state that is not a root. That holds, but it is only cited, not argued.

**Overall:** C1, C2, C3, C4 and C5 hold. I found no mathematical error in Lemma C, Lemma S, Theorem 1 or Theorem 2. The defects are in how the README cites evidence and words its explanations (C6).

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
