<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-095125 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $3.454; this month $129.08 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.610, input 28 + cache write 68,089 + cache read 451,746 tokens — cache hit 87%
- claude:claude-opus-5-5: $0.844, input 26 + cache write 56,098 + cache read 335,906 tokens — cache hit 86%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q7** (split) | CONFIRMED | CONFIRMED | REFUTED | REFUTED | CONFIRMED |
| **Q8** (split) | CONFIRMED | REFUTED | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q7, Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 5 MODERATE, 37 LOW
- **MODERATE** [claude:claude-opus-5-5] README.md:44 — "tested every lemma on 1.9 million states" overstates the testing: 1.9M applies to W_seed only; Projection was tested on about 24k random states, Lam(j) only for h ≤ 8, j ≤ 3 — high — use the wording in Q8.
- **MODERATE** [claude:claude-opus-5-5] README.md:92-93 — Says the seeded blind lemma "keeps the induction going". At m′=2 (reached from m=4, j=1, cop at v) W_seed does not apply (it needs m ≥3). That step rests on the 8-state computation plus Projection and the sibling subtree, which the README never mentions. — high — add "except at height 2, where the 8 computed states on T_3 (via the Projection Lemma and the sibling subtree) take over".
- **MODERATE** [cursor:grok-4.7-medium] `README.md:11` and `:41` — “proved by hand for every h” / “both are 0 for h≤2, proved by hand, twice” is false for `h=2`: §2(a) only clears a parent of two leaves, and both solvers point at strategy files — high — call `h=2` a checked root strategy; keep the hand proofs for the inequalities and for `h≤1`.
- **MODERATE** [cursor:grok-4.7-medium] `reports/solver-b.md:34` — “All four are rejected” does not match `solver-b/run_check.log:16` (only `E_h3_k1_wrongmove2.txt`) and `verifier/wcheck_theirmutants.log` accepts `bad/E_h3_k1_wrongmove.txt` — high — say one altered strategy is legal and the log shows a single rejection.
- **MODERATE** [cursor:gemini-3.8-flash-high] `solver-b/reports/solver-b.md:34` — solver B claims its checker rejected four bad test cases, but `solver-b/bad/E_h3_k1_wrongmove.txt` is accepted as a valid winning strategy, and `solver-b/run_check.log:16` records only one rejection — confidence: high — Replace the mutant with an actually invalid strategy and run/log all negative checks in `run_check.log`.
- **LOW** [claude:claude-fable-5-1] `solver-a/proofs.txt:56` — "c_{r1+1} is that leaf or q1": the leaf is impossible (r1 is the last leaf time); the argument needs q1 — high — "c_{r1+1} = q1".
- **LOW** [claude:claude-fable-5-1] `README.md:47`, `reports/solver-a.md:8` — d(9)=8 rests on solver A's report only; no fast2 h=9 log in the folder — high — "d(9) = 8 reported by solver A, no log kept".
- **LOW** [claude:claude-fable-5-1] `README.md:44,105`, `solver-a/proofs.txt:96` — "8 states" are seeds; the checked closed set has 143 states, and solver A's run output is not saved (only referee's `lam.log`) — high — "8 seed states (closure of 143), checked by the referee's lam.log and rerunnable from lemma_check.py part 3".
- **LOW** [claude:claude-fable-5-1] `README.md:73-75` — "re-enter ... from a neighbour he could not also be covering" describes clause (W) only; for height-2 vertices the proof needs (K), a timing argument over leaf visits — medium — add "for vertices just above the leaves this needs a separate argument about when the cop last visited the leaves".
- **LOW** [claude:claude-fable-5-1] `README.md:86-93` — "a position of the same kind" omits the sibling-subtree requirement of Ψ_m, which is exactly what lets the induction bottom out at the computed m=2 base — medium — add "with a sibling subtree of equal height, so the induction can end at the computed height-2 base".
- **LOW** [claude:claude-fable-5-1] `README.md:42` — "proved by hand by solver A" understates: blind loss for all h≥3 also follows from B's E-projection (`solver-b/proofs.txt:69`) plus `wit/E_h3_k0.txt` — medium — "proved by A in general; B's projection plus the E(3,0) certificate gives it too".
- **LOW** [claude:claude-fable-5-1] `solver-a/proofs.txt:62-65` — "height" in W_seed is not redefined for the ambient tree; it works because X's leaves are leaves of T — high — add "height = distance to the leaves of X".
- **LOW** [claude:claude-fable-5-1] `solver-a/proofs.txt:94-95` — Lam(0), m=2 silently uses π(c)∉X (so the projected belief still contains {v,v1,v2}) — high — add "π(c)∈{parent(v)}∪sibling tree since c∉X".
- **LOW** [claude:claude-opus-5-5] README.md:44 "Where" — Lists §3, §5, but the e lower bound also uses §1 (Projection) and the MONOTONE remark in §0 — high — "§0, §1, §3, §5".
- **LOW** [claude:claude-opus-5-5] README.md:61-63 — "subtree of the second vertex on the path" is ambiguous. Counting x as first, that is y, but the proof uses w, the third vertex (proofs.txt:37-39). With y, "strictly below it" gives only one level — high — "inside the subtree of the vertex two steps down the path; after the second step she is caught or strictly below that vertex".
- **LOW** [claude:claude-opus-5-5] README.md:74-75 — The single "re-enter from a neighbour" sentence hides the timing argument (K) needed at height-2 vertices, which is the non-obvious part — medium — "…; at a height-2 vertex the re-entry comes from a child, and showing one is still possible needs a timing argument".
- **LOW** [claude:claude-opus-5-5] README.md:88-91 — Omits that Ψ needs a sibling subtree of equal height — medium — add "with a sibling subtree of the same height".
- **LOW** [claude:claude-opus-5-5] solver-a/ (no log) — The output of `lemma_check.py` is not saved, although proofs.txt:96 cites it as the base computation — high — commit its output.
- **LOW** [claude:claude-opus-5-5] README.md:114-121 — Omits a point from prior-art.md:22-24, 53: the pathwidth of T_h is ⌈h/2⌉, which resembles e(h). An expert reader would want to know — medium — add one sentence.
- **LOW** [claude:claude-opus-5-5] proofs.txt:56 — "that leaf or q1" should read "q1" (already noted by the referee) — high — "c_{r1+1}=q1".
- **LOW** [claude:claude-opus-5-5] proofs.txt:104-107 — The Theorem does not state the never-glimpse case (it is covered by the Corollary) — high — add "(no glimpse: Corollary)".
- **LOW** [claude:claude-opus-5-5] lemma_check.py:56,65 — The code says "Lambda+_0" where proofs.txt says Lam(0) — low — rename.
- **LOW** [claude:claude-opus-5-5] proofs.txt:62 — The lemma says "ambient tree" but means a rooted tree in which X is attached only through v — medium — "rooted ambient tree".
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:44,63` — height uses undefined h although W_seed is ambient-tree/X-relative — confidence: high — define height_X=m−depth_X
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:66` — (F) invokes S_{t−1} at t=0 — confidence: high — state the seed base separately
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:72-73` — reused visit interval is empty when t=d+2 — confidence: high — handle that case directly from leaves in S_{d+1}
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:104-107` — final theorem omits the no-first-glimpse branch — confidence: high — invoke the blind corollary
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/run_all.sh:14` — lemma-check output is not archived — confidence: high — retain a log
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:41` — “proved by hand, twice” overdescribes the h≤2 strategy certificates — confidence: high — distinguish hand bounds from checked strategies
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:67,89` — unqualified h−1 and “next to” are inaccurate at h=0 and omit robber staying — confidence: high — qualify h≥1 and say closed neighborhood
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:110` — labels a correct weaker disjunction as an error — confidence: high — remove that item
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:94-96` — the m=2 projection silently relies on monotonicity because projection may yield a belief containing, not equal to, the seed — confidence: high — explicitly invoke `solver-a/proofs.txt:17-18`.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:44` — “tested every lemma on 1.9 million states” obscures differing finite/random test limits — confidence: high — state limits per lemma
- **LOW** [cursor:gpt-5.6-sol-xhigh] `reports/solver-b.md:49` — says height 2 “needs” one exact glimpse although e(2)=0 — confidence: high — say the recurrence uses one; a separate blind strategy uses zero
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:56` — “`c_{r1+1}` is that leaf or q1” cannot be the leaf, because `r1` is the last time on a leaf of `q1` — high — “`c_{r1+1}=q1`”.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:72-73` — the visit window `[d+2,t-1]` includes impossible times: depth `d+2` is unreachable before time `d+3`, and `c_{t-1}` is not a leaf — medium — “`[d+3,t-2]`”.
- **LOW** [cursor:grok-4.7-medium] `solver-a/lemma_check.py:21-48` — parts 1–2 assert `(W)` and `(F)` only; clause `(K)` is never checked (the referee’s `lemmas.py:42,71` does check it) — high — add the height-2 child assertion.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:31` — §2 is tagged COMPLETE while `(d)` at `:41` is a named file, not an argument written there — high — tag `(d)` complete-modulo-certificate.
- **LOW** [cursor:grok-4.7-medium] `README.md:44` — “tested every lemma on 1.9 million states” is the blind-walk total in `verifier/lemmas.log:9-12` (F, W, K, and the choice of `u`); it does not execute the post-move check `N[u]\{c'}⊇N_{X_u}[u]` — medium — attribute the 1.9M figure to `W_seed` and the answer choice only.
- **LOW** [cursor:gemini-3.8-flash-high] `solver-a/proofs.txt:57` — \(W_{\text{full}}(K)\) states "\(c_{r1+1}\) is that leaf or \(q_1\)", but because \(r_1\) is the last time the cop stood on a leaf of \(q_1\), the cop cannot remain at that leaf, forcing \(c_{r1+1} = q_1\) — confidence: high — Replace "\(c_{r1+1}\) is that leaf or \(q_1\)" with "\(c_{r1+1} = q_1\)".
- **LOW** [cursor:gemini-3.8-flash-high] `solver-a/lemma_check.py:44-48` — part (2) checks clauses (F) and (W) of \(W_{\text{seed}}\) across timed levels but omits an assertion for clause (K), and samples only one \(v\) per depth — confidence: high — Add an assertion checking clause (K) and test all subtrees \(X\) of height \(m \ge 3\).
- **LOW** [cursor:gemini-3.8-flash-high] `reports/solver-a.md:10` — claims "Every entry is COMPUTED, exhaustively, and also PROVED", whereas lower bounds depend on finite computed base certificates rather than a pure analytic derivation — confidence: high — Replace with "Every entry is COMPUTED exhaustively, and PROVED modulo finite computed base certificates".

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
