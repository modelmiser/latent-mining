<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261118-083254 — (repository) — round 5

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $2.646; this month $157.11 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.034, input 26 + cache write 72,781 + cache read 232,848 tokens — cache hit 76%
- claude:claude-opus-5-5: $0.612, input 18 + cache write 35,891 + cache read 185,137 tokens — cache hit 84%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q1** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| **Q4** (split) | CONFIRMED | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED |
| **Q5** (split) | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED | CONFIRMED |
| **Q6** (split) | CONFIRMED | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q1, Q4, Q5, Q6
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 7 MODERATE, 17 LOW
- **MODERATE** [claude:claude-fable-5-1] Attachment A title/Setting; brief "same greedy rule" — Theorem 3 is for the min-h rule; the repository's Section 5 algorithm (NOTE.md:246-249) is the smaller-tree rule, for which Lemma A fails (cost 2, ΔΦ 1.5 example above). A reader merging this into NOTE.md as "the greedy rule" would attach the bound to the wrong algorithm — high — state the rule explicitly and note Theorem 2 covers both rules while Theorem 3 covers only the shorter-path rule.
- **MODERATE** [claude:claude-opus-5-5] Draft Lemma A, last line — the k = 0 case leaves p's changed out-terms without an argument; the result is still true — high — add the case check from Q1.
- **MODERATE** [claude:claude-opus-5-5] Draft, comparison paragraph — the crossover is computed against the formula, not f(n); the new bound is strictly better only at n = 96 and n ≥ 126; "smaller" should be "no larger"; "about 80" should be 76 — high — use the Q5 wording.
- **MODERATE** [claude:claude-opus-5-5] Draft, "Checks run" — the enumeration exercises only k ≤ 2 and H ≤ 2, and the general cases rest on the hand proof alone; the pool, exact-greedy and LP artifacts are missing — high — state the limits and add the programs or drop the claims.
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `THEOREM3-draft.md:29-36,63-64` — the \(k=0\) proof invokes inapplicable steps and omits \(p\)’s edges — confidence: high — add the direct argument above
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `allcheck.py:42-62`; `lemma2.py:27-32` — both programs still exit successfully after violations, making automated “pass” detection unsound — confidence: high — exit nonzero when slack is negative or `bad>0`
- **MODERATE** [cursor:grok-4.7-medium] THEOREM3-draft.md Lemma A, `k=0` sentence — Steps 1–2 do not account for the new edge out of `p`, nor for the change in `p`’s old term when `p` already has an out-neighbour; the gain is still `≥0` if checked directly — high — write the out-degree 0 and 1 cases.
- **LOW** [claude:claude-fable-5-1] Attachment A, comparison sentence — "from about n = 80" is 76 (real) or 88 (floored) — high — give the computed value.
- **LOW** [claude:claude-fable-5-1] Attachment A, Checks — `allcheck.py 7 12` and the planned `10 15` never produce a path of length ≥ 3, so the middle-vertex case with g ≥ d+2 for i ≥ 2 is untested by enumeration; the "pool search to n = 96" that would cover it is not provided — high — say so, or attach the pool check.
- **LOW** [claude:claude-fable-5-1] Attachment A, Lemma B one-neighbour case — does not say r has out-degree ≥ 1 (true since n ≥ 2 and T connected) nor that T−r is a tree (r is an undirected leaf) — high — add one clause.
- **LOW** [claude:claude-fable-5-1] Attachment A, Theorem 3 — "H(T) ≥ 1" needs Φ(T) ≥ cost > 0, which is the telescoped Lemma A; the sentence is fine but the telescoping over the forest (Φ additive across trees, Φ(empty)=0) is never written — medium — add one line.
- **LOW** [claude:claude-fable-5-1] Attachment A, Checks — "pool lower bounds (n ≤ 128) at most (n−3)/2" and "563 patterns" are unverifiable here — high — reference files or drop.
- **LOW** [claude:claude-opus-5-5] Draft "Greedy rule" vs NOTE.md:246-249 — the draft's rule is the smaller-h rule of [7], not the note's Section 5 algorithm; Lemma A needs h(q) ≥ h(p) — high — name the rule explicitly.
- **LOW** [claude:claude-opus-5-5] Draft Step 1 — the induction hypothesis is stated unclearly ("in fact g_{i−1} ≥ …") — high — state gᵢ ≥ k−i as the hypothesis.
- **LOW** [claude:claude-opus-5-5] Draft Step 3, w₀ with a ≥ d+2, b ≤ a — the bound d+3/2 needs the split b = d+1 vs b ≥ d+2 — high — add it.
- **LOW** [claude:claude-opus-5-5] Draft Lemma B, third case — the slack is written inconsistently with the first two cases — high — write it as "= 0".
- **LOW** [claude:claude-opus-5-5] Draft title — "Theorem 3" already names results in open-limit/proofs.txt:125 and README.md:52 — medium — rename it.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `lemma2.py:19-31` — “all smaller ones” omits \(n=1\), although that base is trivial — confidence: high — check or explicitly exclude the base case
- **LOW** [cursor:grok-4.7-medium] THEOREM3-draft.md comparison sentence — “about `n=80`” is four past the first integer `n=76` — high — say `n≥76`.
- **LOW** [cursor:grok-4.7-medium] allcheck.py, `if hA[p] > hB[q]: continue` — the draft’s “every pair of endpoints” is wider than the loop — high — say “every pair with `h(p)≤h(q)`”.
- **LOW** [cursor:grok-4.7-medium] THEOREM3-draft.md “Checks run” — `816,601`, slack 0, `15,000`, equality at every `n≥3`, the pool search, and the `n≤14` greedy run are not in the repository; B and C are not those last two checks — high — do not treat those figures as part of the proof.
- **LOW** [cursor:grok-4.7-medium] THEOREM3-draft.md Lemma B, case `H=m+1` — that line equals `-2(m+1)`, while the `H=H_1` and `H=H_2` lines are already reduced by `2H`; the inequality is right, the three displays are not the same expression — medium — reduce all three to `≤0`.
- **LOW** [cursor:gemini-3.8-flash-high] attach5.txt:3 — Title states "pays at most (n - 3)/2", which is negative for \(n \in \{1, 2\}\) where cost is 0 — confidence: high — State the bound as \(\max(0, \lfloor(n - 3)/2\rfloor)\) or restrict the headline inequality to \(n \ge 3\).
- **LOW** [cursor:gemini-3.8-flash-high] attach5.txt:84 — "The best statement is the minimum of the two" is ambiguous after mentioning Theorem 2, Theorem 3, and the table of \(f(n)\), and Theorem 2 does not hold for \(n < 19\) — confidence: high — Specify the piecewise minimum across the relevant domains: exact values / \(f(n)\) for small \(n\), Theorem 2 for \(19 \le n \le 75\), and Theorem 3 for \(n \ge 76\).

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
