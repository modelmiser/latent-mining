<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Verdicts**

Q1: CONFIRMED — recomputed all 12 pair images and 12 round-table entries from a=(2,2,1,3), b=(4,3,1,1); every entry in `solver-a/proofs.txt:5-9` is right (e.g. 14.a={2,3}, 34.b={1}, 12.b=34) — none
Q2: CONFIRMED — `solver-a/proofs.txt:12-23`: τ≥max(d+2,3) so x_2..x_(d+1)=a independent of y_1; y_1=a: 134→123→12→{M,34} at turn 3, I(t) closes under the a-table (34→{12,14}, 14→{12,13}, 12→M, 13→{M,34}); y_1=b: 134→14→23→{12,13}, J(t) likewise; at τ image ∈{M,34}, 34.b={1}; singletons persist so T=max τ. Also matches my T(d) recomputation 4,4,6,7,7,9 — none
Q3: CONFIRMED — `solver-a/proofs.txt:26-37`: all eight family images rechecked (F1.a=F3, F1.b=F2, F2.b⊇F2, F3.b⊇F1, F4.a⊇F1, F4.b⊇F1); the Lemma is a lookahead induction over all four families at once, legitimate because the word is fixed; turn-2 images 12/134 ⊇ F4 and 123/14 ⊇ F1, with the Spoiler choosing y_1 after the Lemma supplies P — none
Q4: CONFIRMED — `solver-a/proofs.txt:40-42`: padded states map to 2/4 so Q.a=123, Q.b=134 exactly as in W; all later images lie in {1..4} and both Theorem 1 and Theorem 2 (incl. T=1: |Q.a|=|Q.b|=3) apply verbatim; same for B's padding `solver-b/proofs.txt:65-68` (new states→1∈Q.a∩Q.b) — none
Q5: CONFIRMED — `README.md:18-27` = `chain/3-question.txt:3-7` (Synchronizer first, x_t sees y_1..y_(t−1−d), singleton after x_T, uniform T, Theorem 3/Lemma 1 via König); reduction `solver-a/proofs.txt:51-56`: Spoiler choosing y_j can compute x_(j+1..j+d), so revealing y_(t−d) after x_t is exact; (S,q) is a sufficient statistic; matches B's Lemma 2 `solver-b/proofs.txt:14-19` — none
Q6: CONFIRMED — recomputed round maps `solver-b/proofs.txt:26-27`, main lines (123→14→34→13→14; 34→13→14→34), the "S_k=12 only if M_(k−1)∈{14,34}" induction, and F1–F3 of Lemma B with the run invariant; "M_(r−1)=13" fails only at r=2 (M_1=123) where 123.aa={1} and 123.ab=14 still give S_2∈{14,M}, so harmless as `reports/referee.md:14` says — none
Q7: CONFIRMED with LOW caveats — "three programs": `solver-a/check_witness.py`, `solver-b/verify.py:9` (on C1=1123/3422, isomorphic to W1 — I verified the orbit), `verifier/chk.py:161`; n≤5 table: `solver-b/n5_full.txt`, `verifier/n5_p1.txt`+`n5_p2.txt` (sums match), A's n=5 figures only in `reports/solver-a.md:12-20`; "exactly 96": `solver-a/dump4sym.txt` has exactly 2 classes×48 with win_d all 1; b collapses only 34 (`proofs.txt:6`); but "the phase is fixed by the Spoiler's first letter / Spoiler picks the first letter" (`README.md:74-82`) oversimplifies: a Spoiler who fixes y_1 and then plays myopically loses to 1820 words of length ≤12 (e.g. `ababaab`); full lookahead (Theorem 2) is needed — "…and the phase is fixed by the Spoiler's first letter; against a blind word the Spoiler, reading the whole word, also re-phases at each misplaced b (Theorem 2's families)."
Q8: REFUTED (second half) — description as computation is fair (`README.md:44,92`), and my own independent Python recount gives W_0=W_inf=12, 405 for n=2,3; but the folder's logs show it only in `verifier/chk_out.txt:9` (n=3, referee's Python) and A's empty `dump2.txt`/`dump3.txt`; n=2 appears in no log, B printed to terminal (`reports/solver-b.md:91`) — "three programs report agreement; only the referee's n=3 recount is logged here."

**Findings**

F: LOW — `README.md:74-82` — "Why the example works" implies the blind game is decided by y_1 alone; counterexample above (`ababaab` needs later Spoiler lookahead). Headline unaffected — high — add that the Spoiler re-phases at every misplaced b.
F: LOW — `README.md:44` / `README.md:92-93` — "three independent programs" for n=2,3 is not evidenced by logs in the folder (n=2 nowhere, n=3 only `chk_out.txt:9`); numbers are nevertheless correct (recomputed) — high — say "reported by three programs; one log kept".
F: LOW — `README.md:40` — "both solvers' checkers" ran on the stated automaton only up to isomorphism (B checked C1=1123/3422) — high — "by both solvers' checkers (B's on an isomorphic relabelling)".
F: LOW — `solver-b/proofs.txt:32` — "x_t = a for all t ≤ d+1" is false for t=1 (x_1=b); legality still holds — high — "for 2 ≤ t ≤ d+1".
F: LOW — `solver-b/proofs.txt:36,39` — "M_(r−1)=13" false for r=2 (already in referee); the file is left uncorrected — high — "M_(r−1) ∈ {123,13}, never 14 or 34".
F: LOW — `README.md:73` — "Any other choice collapses the pair": the other choice gives 12, which collapses only on the next a-round — high — "leads to 12, which the next a collapses".
F: LOW — `README.md:47` — A's n=5 counts appear only in its report (`reports/solver-a.md`), no log file; the two logged programs agree — medium — note "two logs kept".
F: LOW — `solver-a/proofs.txt:49` — "I regard this as a machine proof" conflicts with README's "computed, not a proof"; README wording is the safer one — high — none (README governs).

**Not asked, worth noting**
- The whole n≥5 picture rests on padding; the 170 n=5 survivor classes (`verifier/n5_classes.txt`) and the n=6 column remain unverified beyond stated delays, as the README correctly says.
- Both hand proofs are consistent with each other after relabelling (W1≅C1, W2≅C2 verified), so the 96-count is fully proved given the d=2 census — the README could state this as "proved" rather than "computed".
