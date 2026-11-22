<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-102726 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $2.898; this month $136.61 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.179, input 28 + cache write 63,052 + cache read 399,970 tokens — cache hit 86%
- claude:claude-opus-5-5: $0.719, input 24 + cache write 48,522 + cache read 188,507 tokens — cache hit 80%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q3** (split) | CONFIRMED | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED |
| **Q6** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| **Q8** (split) | CONFIRMED | REFUTED | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q3, Q6, Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 1 CRITICAL, 8 MODERATE, 33 LOW
- **CRITICAL** [cursor:gpt-5.6-sol-xhigh] `verifier/defsolve.cpp:12-13,59-61` — unchecked \(k>6\), \(n>10\), or missing arguments cause out-of-bounds access/undefined behavior; duplicated in `deflemma.cpp` — confidence: high — validate argc and bounds or use dynamic storage
- **MODERATE** [claude:claude-opus-5-5] `README.md:69` — The closed forms are stated with no range of j. Taken literally, N(2,2)=3·5⁻¹·L_2=9/5, and the odd form gives N(2,1)=3/5. — high — Add "j ≥ 1 / j ≥ 2; N(2,1)=1, N(2,2)=2".
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:45` — closed forms are false for \(n=1,2\) — confidence: high — add domains and exceptions from `solver-b/proofs.txt:68`
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:61` — \(f(s,5)\) is not proved for multi-action states because `prove.py:58-60` never checks \(d(3)\) — confidence: high — restrict the all-state conclusion to \(u\ge6\); retain \(u=5\) only for the root
- **MODERATE** [cursor:grok-4.7-medium] `solver-b/proofs.txt:47` — the suffix is claimed to hold exactly `j` items when the buried item leaves; extra covers from the left can make it larger, so that sentence is false, while `≥ j` still fires C(`j−1`) — high — say “at least `j` items”
- **MODERATE** [cursor:grok-4.7-medium] `solver-a/proofs.txt:61` — “every state, `u ≥ 5`” is not proved at `u = 5`, because `d(3) ≥ 0` is unchecked; `verifier/flag_out.txt:3` shows no violating state, and `N` only needs the empty state — high — limit the sentence to `u ≥ 6` plus the empty state at `u = 5`
- **MODERATE** [cursor:grok-4.7-medium] `README.md:112-113` — “three times A052936” drops the index shift that `prior-art.md:16` records; `3·(1,2,7,25) = (3,6,21,75)` is not `N(2,n)` — high — write `3·A052936(n−2)` for `n ≥ 3`
- **MODERATE** [cursor:gemini-3.8-flash-high] 13-blind-stacks/README.md:47,122,134 — Directory `reports/` is referenced in the table, text, and files list but is missing from the repository — high — Restore `reports/` containing `solver-a.md`, `solver-b.md`, and `referee.md`, or update README references to point to existing logs in `verifier/` and `solver-*/`
- **MODERATE** [cursor:gemini-3.8-flash-high] 13-blind-stacks/solver-a/proofs.txt:61-62 — Closing remark claims f(s,5)=5f(s,4)-5f(s,3) for every state, but `prove.py` never checks d(3) >= 0, so the claim is unproved for non-empty states at u=5 — high — "So f(empty,5)=5f(empty,4)-5f(empty,3) (since IN is the sole legal move from empty), and f(s,u)=5f(s,u-1)-5f(s,u-2) for all s and u >= 6; s=empty gives the theorem."
- **LOW** [claude:claude-fable-5-1] README.md:69 — closed forms stated without range; N(2,2)=2 ≠ 3·5^{−1}·L₂, N(2,1)=1 ≠ 3·5^{−1}·F₁ — high — add "(n ≥ 3)".
- **LOW** [claude:claude-fable-5-1] README.md:136 — "three times OEIS A052936" drops the index shift; prior-art.md:10 says N(2,n)=3·A052936(n−2) — high — "three times A052936 shifted by two".
- **LOW** [claude:claude-fable-5-1] README.md:123-124 — "decides the one sign condition … in Q(√5)" understates: nine integer non-negativity checks (r=1..9) are also needed and done (`prove_k2.py:74`) — high — "checks slack ≥ 0 for r = 1..9 in integers and the one Q(√5) inequality slack(9) ≥ β·slack(8)".
- **LOW** [claude:claude-fable-5-1] README.md:71 — "k = 4, 5 up to n = 7" implies literal-rule runs for all smaller n; `def_out.txt` has k=4 only n=5..7, k=5 only n=6,7 — high — state the actual ranges.
- **LOW** [claude:claude-fable-5-1] solver-a/proofs.txt:61 — closing remark "for every s and u ≥ 5" needs d(3) ≥ 0, never checked (referee already notes; theorem unaffected) — high — "for every s and u ≥ 6; for s = empty from u = 5".
- **LOW** [claude:claude-fable-5-1] README.md:86-90 — "Enough room" omits shifting items right to free S_1 before each IN, which the proof needs (`solver-b/proofs.txt:24-26`) — medium — add "shift items right so S_1 is free before each arrival".
- **LOW** [claude:claude-fable-5-1] README.md:70 — "proved by solver A by the same method" for k=1: `solver-a/proofs.txt:62` is one sentence; the k=1 induction (d(u+1)=2d(u)) is not written out, only the script run (I re-ran: failures=0) — medium — "by the same script; the k = 1 induction is not written out".
- **LOW** [claude:claude-fable-5-1] solver-b/proofs.txt:18 — well-founded measure "(#items left, positions, −#flags)": IN leaves items-left unchanged, so "positions" must count unarrived as position 0; not stated — low — add that clause.
- **LOW** [claude:claude-opus-5-5] `README.md:69` — "Checked to n=300" is credited to `solver-b/proofs.txt`, but B's code checks only to 130. The 300 check is in `verifier/flag_out.txt`. — high — Cite `verifier/flag_out.txt`.
- **LOW** [claude:claude-opus-5-5] `README.md:67` — The row says "twice", but the Where cell omits `solver-a/proofs.txt` Theorem 2. — high — Add it.
- **LOW** [claude:claude-opus-5-5] `README.md:123-124` — "the one sign condition needed at the start" understates the check: B also checks slack ≥ 0 for r=1..9 in integers (`prove_k2.py:74`). — high — Say "checks slack ≥ 0 for r ≤ 9 and decides slack(9) ≥ β·slack(8) exactly".
- **LOW** [claude:claude-opus-5-5] `README.md:110-112` — Fixing a rule gives a 49-dimensional linear recurrence, not x²−5x+5. That characteristic polynomial is a checked fact. A's proof does not use the matrix view at all. — medium — Say "…a fixed linear function…; computation shows the values satisfy x²−5x+5".
- **LOW** [claude:claude-opus-5-5] `README.md:116-119` — Solver A's induction also carries forward the recurrence for every action value, in the within-level order. It also needs the margin at u−2. — medium — Add "and every action value obeys the recurrence".
- **LOW** [claude:claude-opus-5-5] `README.md:100-101` — "has learned nothing" is not the real reason. The sorter is deterministic, its answers within a phase depend only on which item is the minimum, and flags clear at each phase start. — medium — Use that wording.
- **LOW** [claude:claude-opus-5-5] `README.md:136` — "three times A052936" leaves out the offset. — high — Write "N(2,n) = 3·A052936(n−2), n ≥ 3" (`prior-art.md:10`).
- **LOW** [claude:claude-opus-5-5] `README.md:92-93` — "When it is finally output, at least k+1 items sit in k stacks" is only true if 1 arrives at all. For n ≥ 2k+2 it never does, so the sorter fails outright. — high — Add "(if ever)".
- **LOW** [claude:claude-opus-5-5] `README.md:71` — The literal-rule list leaves out k=1 to n=10. — high — Add it.
- **LOW** [claude:claude-opus-5-5] `solver-a/proofs.txt:44-45` — "after IN and OUT of 1" does not say that SHIFTs before that OUT only add items to S_2..S_k. The conclusion holds, but the step is unstated. — medium — State that items only move forward.
- **LOW** [claude:claude-opus-5-5] `solver-b/proofs.txt:55` — "topological order" reads as PI's order. Optimality needs an order that covers every attempt, which `rank` does provide. — medium — Name `rank` as the order.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:96` — solver A forces probing at `prove.py:20-21`, so it does not check every literal move — confidence: high — say “all normal-form actions”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/table.py:5,10-11` — using \(N(2,0)=1\) prints the wrong generating-function numerator — confidence: high — set the generating-function constant term to zero
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:112-114` — OEIS indexing is omitted — confidence: high — write \(N(2,n)=3\,A052936(n-2)\)
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:117` — “chain’s invention” exceeds a search whose author calls novelty evidence weak (`prior-art.md:43`) — confidence: medium — say “not found in the limited search”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:42` — “every state” relies on hash-only memo keys (`verifier/defsolve.cpp:7,31`) — confidence: high — disclose the 128-bit collision caveat
- **LOW** [cursor:grok-4.7-medium] `README.md:45` — Fibonacci/Lucas formulas are printed as the closed form for general `j`, but they are false at `n = 1` and `n = 2` — high — add the two initial exceptions and the ranges `j ≥ 1`, `j ≥ 2`
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:21-23` — the bijections-with-`m`-off-flags description is one asserted sentence; the probing argument above it does not derive it (B’s Lemma 0 does) — high — point the consequence at Lemma 0
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:43-44` — “after IN and OUT of 1” ignores shifts of 1 into the suffix before that OUT; the suffix still ends with at least `k` items — medium — add that those shifts cannot decrease the suffix
- **LOW** [cursor:grok-4.7-medium] `solver-b/proofs.txt:18` — the well-founded measure names “positions” but never defines it — medium — use the rank in `prove_k2.py:19-20`
- **LOW** [cursor:grok-4.7-medium] `README.md:42` — the literal-rule comparison is stated as exact, while `verifier/defsolve.cpp:19-25` memoizes 128-bit hashes (`reports/referee.md:38`) — medium — name the hash as a residual risk
- **LOW** [cursor:grok-4.7-medium] `prior-art.md:24` — the divided terms are listed from 7, 25, 90 and called A052936, whose expansion starts 1, 2, 7 — high — start at 2 or say “from the third term”
- **LOW** [cursor:grok-4.7-medium] `verifier/defsolve.cpp:13` — `st[6][2]` and a length-10 permutation array have no bounds check; claimed runs stay inside `k ≤ 5`, `n ≤ 10` — high — reject `K > 6` or `N > 10`
- **LOW** [cursor:gemini-3.8-flash-high] 13-blind-stacks/solver-a/table_out.txt:12 — Printed numerator coefficients `[-4, 2, 1, 1, 0...]` reflect a different shift convention than the standard generating function x(1-3x+x^2+x^3)/(1-5x+5x^2) — high — Print `[1, -3, 1, 1]` corresponding to x(1 - 3x + x^2 + x^3)
- **LOW** [cursor:gemini-3.8-flash-high] 13-blind-stacks/README.md:84-87 — Text claims both solvers use W_r = A·W_(r-1) and characteristic polynomial x^2 - 5x + 5, but only Solver B formulates the matrix/Cayley-Hamilton proof (Solver A inducts directly on Q and margins) — high — "Solver B shows values obey W_r = A·W_(r-1) via Cayley-Hamilton; Solver A propagates margins directly by showing d(u) >= lam2·d(u-1)."

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
