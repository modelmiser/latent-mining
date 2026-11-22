<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-054141 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $2.821; this month $109.81 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.004, input 28 + cache write 52,718 + cache read 383,460 tokens — cache hit 88%
- claude:claude-opus-5-5: $0.818, input 30 + cache write 53,771 + cache read 435,381 tokens — cache hit 89%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q6** (split) | CONFIRMED | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED |
| **Q7** (split) | CONFIRMED | REFUTED | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q8 | REFUTED | REFUTED | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q6, Q7
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 11 MODERATE, 24 LOW
- **MODERATE** [claude:claude-opus-5-5] README.md:49 (also referee.md:53) — At least 18 of the 170 "open" n = 5 classes are already proved patient by the existing padding argument. Each has one state outside both letter images whose a- and b-images lie inside the other four states' images. Removing it leaves an automaton in a proved 4-state orbit. Example: verifier/n5_classes.txt `11124 45222`, where state 3 is redundant (I checked all 170 by script). Solver A's own report already says the padded Witness 1 is proved. — high — Fix: "152 classes open; 18 are paddings of the 4-state examples and are patient by the Corollary". If unsure of the exact count: "at most 152 classes open".
- **MODERATE** [claude:claude-opus-5-5] README.md:44, 47, 92-93 — "three independent programs" for n = 2, 3, and n ≤ 5 generally, is not backed by the saved logs (dump2.txt and dump3.txt are empty; one n = 3 log; two n = 4 logs). — high — Fix: "n = 5: three programs (logs present); n = 4: two saved logs, plus the referee's report; n = 3: one saved log; n = 2: reported, no log saved."
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-b/verify.py:31-54` — the claimed exact blind checks start from \(Q.a\), so they explore only words prefixed by a and omit words beginning b — confidence: high — start from `{Q}`
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:44,92` — “three independent programs” is not backed by retained n≤3 outputs — confidence: high — say “three implementations report agreement”
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:37-39` — \(M_{r-1}=13\) is false for d=0,1 with \(y_1=a\), though the conclusion survives — confidence: high — handle \(M_1=123\) separately
- **MODERATE** [cursor:grok-4.7-medium] `solver-b/proofs.txt:12` — “height + 1” is a uniform T only if height counts nodes; the longest bad prefix has L edges and the image after x_(L+1) is still not a singleton, so T=L+2 — high — “its height in edges, plus 2, is a uniform T”
- **MODERATE** [cursor:grok-4.7-medium] `README.md:73` — “Any other choice collapses the pair” and line 79 “a never lets the Spoiler leave the cycle”; the a-table sends 34 to the pair 12 as well as 14, and the invariant in `solver-a/proofs.txt:17-21` keeps 12 — high — “the only way to survive the next a is the cycle 34→14→13; the other successor is 12, which the next a collapses”
- **MODERATE** [cursor:grok-4.7-medium] `README.md:48` — “three independent programs agree” for every n≤5 cell; n=2 has no saved program output, and the referee n=5 logs are pruned — medium — “n=5 agrees between solver B’s full run and the referee’s pruned run; n=2 and n=3 are as reported, with a saved log only for n=3”
- **MODERATE** [cursor:gemini-3.8-flash-high] `solver-b/proofs.txt:36` — Stated claim "\(M_{r-1} = 13\)" is false when \(r=2\) (\(y_1=a, d \in \{0, 1\}\)), where \(M_1 = 123\) — confidence: high — Note that for \(r=2\), \(S_1 = 123\) and \(123 \cdot aa = \{1\}\), so \(S_2 \in \{14, M\}\) still holds.
- **MODERATE** [cursor:gemini-3.8-flash-high] `08-patient-automaton/README.md:40` — Claims Witness 1 (\(2213/4311\)) strategy was checked by "both solvers' checkers", but `solver-b/verify.py:6-10` only tests C1 and C2 — confidence: high — Replace with "by solver A's checker, the referee's checker, and solver B on its isomorphic witness".
- **MODERATE** [cursor:gemini-3.8-flash-high] `08-patient-automaton/README.md:44` — Asserts \(W_0 = W_\infty\) for \(n=2,3\) is shown by three programs in logs, but no log file for \(n=2\) exists and only `verifier/chk_out.txt:9` logs \(n=3\) — confidence: high — Add committed log runs for \(n=2,3\) from `solver-a/wd.cpp` and `solver-b/game.cpp`.
- **LOW** [claude:claude-fable-5-1] `README.md:74-82` — "Why the example works" implies the blind game is decided by y_1 alone; counterexample above (`ababaab` needs later Spoiler lookahead). Headline unaffected — high — add that the Spoiler re-phases at every misplaced b.
- **LOW** [claude:claude-fable-5-1] `README.md:44` / `README.md:92-93` — "three independent programs" for n=2,3 is not evidenced by logs in the folder (n=2 nowhere, n=3 only `chk_out.txt:9`); numbers are nevertheless correct (recomputed) — high — say "reported by three programs; one log kept".
- **LOW** [claude:claude-fable-5-1] `README.md:40` — "both solvers' checkers" ran on the stated automaton only up to isomorphism (B checked C1=1123/3422) — high — "by both solvers' checkers (B's on an isomorphic relabelling)".
- **LOW** [claude:claude-fable-5-1] `solver-b/proofs.txt:32` — "x_t = a for all t ≤ d+1" is false for t=1 (x_1=b); legality still holds — high — "for 2 ≤ t ≤ d+1".
- **LOW** [claude:claude-fable-5-1] `solver-b/proofs.txt:36,39` — "M_(r−1)=13" false for r=2 (already in referee); the file is left uncorrected — high — "M_(r−1) ∈ {123,13}, never 14 or 34".
- **LOW** [claude:claude-fable-5-1] `README.md:73` — "Any other choice collapses the pair": the other choice gives 12, which collapses only on the next a-round — high — "leads to 12, which the next a collapses".
- **LOW** [claude:claude-fable-5-1] `README.md:47` — A's n=5 counts appear only in its report (`reports/solver-a.md`), no log file; the two logged programs agree — medium — note "two logs kept".
- **LOW** [claude:claude-fable-5-1] `solver-a/proofs.txt:49` — "I regard this as a machine proof" conflicts with README's "computed, not a proof"; README wording is the safer one — high — none (README governs).
- **LOW** [claude:claude-opus-5-5] README.md:40, 9 — "both solvers' checkers" did not play this automaton. solver-b/verify.py:7-11 runs B's strategy on C1, an isomorphic copy, not on 2213/4311. — high — Fix: "…and by solver B's checker on an isomorphic copy (C1)".
- **LOW** [claude:claude-opus-5-5] README.md:72-82 — "Any other choice collapses the pair" is only true one round later. "Picks the first letter that puts the cycle out of phase" oversimplifies, because the Spoiler needs later choices too (proofs.txt:26-37). — high — Fix: "…collapses within a round" and "the Spoiler can always stay out of phase (Theorem 2)".
- **LOW** [claude:claude-opus-5-5] README.md:41 — "four families of pairs closed under either letter": the families are closed under rounds, and only up to containment. — high — Fix: as in Q3.
- **LOW** [claude:claude-opus-5-5] README.md:98 — Calls Lemma 1 of solver-b/proofs.txt:9-12 "a remark about T"; it is a lemma marked COMPLETE. — high — Fix: "an off-by-one in Lemma 1 (uniform T)".
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:55 — "(if |q| = d)" is ambiguous about whether it means before or after appending. — medium — Fix: as in Q5.
- **LOW** [claude:claude-opus-5-5] README.md:46 — "computed by three programs" for the 96: the referee's n = 4 output is not in the folder (chk_out.txt:10 says "n=4 python recount skipped"). — medium — Fix: "two saved logs plus the referee's report".
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-b/verify.py:21-24` — the “literal” strategy checker initializes at \(Q.a\), not Q; fixture-specific equality masks this — confidence: high — initialize with all state keys
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:73-79` — deviations from the cycle reach pair 12 before collapsing, contradicting the literal explanation — confidence: high — say deviations collapse on the next a
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:9-12` — “height + 1” depends on counting levels rather than edges — confidence: high — define height or use maximum prefix length + 2
- **LOW** [cursor:gpt-5.6-sol-xhigh] `verifier/chk_out.txt:36` — it labels a correctly accepted equivalent strategy “CHECKER BROKEN,” contradicting `reports/referee.md:34-35` — confidence: high — replace the negative control
- **LOW** [cursor:grok-4.7-medium] `solver-b/proofs.txt:32` — legality says every x_t with t≤d+1 equals a, but x_1=b; the strike is still at t≥d+2 — high — “x_1=b fixed; x_t=a for 2≤t≤d+1”
- **LOW** [cursor:grok-4.7-medium] `README.md:42` — “one misstated step”; the referee also records the height off-by-one — high — “a false equation in Lemma A and an off-by-one in Lemma 1’s height, neither used by the explicit T=d+4”
- **LOW** [cursor:grok-4.7-medium] `verifier/chk_out.txt:35` — “CHECKER BROKEN” on B’s strategy on A’s W1; `reports/referee.md:35` says that control is invalid because the strategies are the same play — high — drop that control; acceptance is expected
- **LOW** [cursor:gemini-3.8-flash-high] `solver-b/proofs.txt:12` — Lemma 1 states "its height + 1 is a uniform T", which has an off-by-one if height is edge distance rather than node level count — confidence: high — Clarify height as level count or use height + 2.
- **LOW** [cursor:gemini-3.8-flash-high] `verifier/chk.py:233` — Intended negative control B \(\sigma\) on A.W1 prints `ACCEPTED (CHECKER BROKEN)` in `verifier/chk_out.txt:34` because B's strategy happens to win on A's automaton — confidence: high — Replace with a genuine failing mutant as noted in `reports/referee.md:34`.
- **LOW** [cursor:gemini-3.8-flash-high] `solver-a/witnesses.txt:23-24` — Mutation test artifacts `blind_cert_2211_4311.txt` and `blind_cert_2213_4312.txt` are committed in the directory with instructions to ignore them — confidence: high — Move test artifacts to a test directory or remove them.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
