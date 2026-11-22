<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-064122 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $3.457; this month $116.08 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.597, input 24 + cache write 67,985 + cache read 338,916 tokens — cache hit 83%
- claude:claude-opus-5-5: $0.860, input 22 + cache write 58,261 + cache read 278,855 tokens — cache hit 83%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q1** (split) | CONFIRMED | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q8 | REFUTED | REFUTED | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q1
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 9 MODERATE, 23 LOW
- **No parseable F: lines from:** claude:claude-opus-5-5 — read their reports by hand
- **MODERATE** [claude:claude-fable-5-1] README.md:93, prior-art.md:8 — "about half as far per erasure" contradicts prior-art.md:12's own translation ((f+1)·2^(p−2)); the prior strategies have the same per-erasure rate, only the constant differs — medium — "whose strategies gain the same 2^(p−2) per erasure but start one round behind; no upper bound was found".
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:92-94` — “about half as far per erasure” is arithmetically false under the repository’s own prior-art formulas — confidence: high — say “same asymptotic gain, one 2^(p−2) block shorter for fixed f”
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:72-75` — a sole-anchor deletion yields empty, not a shifted same-class board — confidence: high — enumerate Lemma N’s three outcomes
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `open-upper/proofs.txt:138-141` — “Always m≤q−1” is false for the theorem’s unreachable boards; e.g. q=2, A={2,4}, f=2 satisfies J with m=q — confidence: high — restrict the assertion to reachable post-F boards
- **MODERATE** [cursor:grok-4.7-medium] `open-upper/proofs.txt:62` — L4 never bounds the ray above `a_m`, but `:161`, `:165`, and `:174` cite L4 for it; as written, “every element ≤ `a_m+2^r−1`” does not follow — high — define `T_m=S∩(a_m,∞)` and apply W with c=r.
- **MODERATE** [cursor:grok-4.7-medium] `open-upper/proofs.txt:66` — L4/L5 assume every configuration on the P/R path contains A; that freezing fact is not proved — high — note that deleting an anchor would make `red` drop it, contradicting uniqueness.
- **MODERATE** [cursor:grok-4.7-medium] `README.md:92` — “about half as far per erasure” contradicts the reach formulas in `prior-art.md:7` and `:12` — high — replace with the `(f+1)` versus `(f+2)` comparison.
- **MODERATE** [cursor:gemini-3.8-flash-high] 07-charged-pebbles/README.md:9-10 — Overstates that each forgetting buys \(2^{p-2}\) nodes "however the forgettings are spent"; unequal charges buy strictly less, e.g. charges \((3,0,0)\) at \(p=3\) reach only 6 (`reports/solver-a.md:23`) — confidence: high — Replace with "Each forgetting buys at most \(2^{p-2}\) more nodes, and exactly that much when forgettings are distributed equally across pebbles under the optimal strategy."
- **MODERATE** [cursor:gemini-3.8-flash-high] 07-charged-pebbles/reports/solver-b.md:40 — Claims "the only binding constraint is the total pk", which fails for unequal starting charges where per-pebble distribution binds (`reports/solver-a.md:23`) — confidence: high — Qualify that total charge is the sole binding constraint only when initial charges are equal.
- **LOW** [claude:claude-fable-5-1] README.md:9 — "buys … and no more" overstates; only the upper bound holds for arbitrary spending — high — "No forgetting ever buys more than 2^(p−2) nodes, however spent; the strategy gets exactly that from each."
- **LOW** [claude:claude-fable-5-1] README.md:67-68 — reducible pebble sits close enough *above* its predecessor, not "behind another pebble" — high — "any pebble close enough above the pebble below it for the free pebbles to reach".
- **LOW** [claude:claude-fable-5-1] README.md:73-75 — shift may be backward (proofs.txt:119); "equivalent to" should be "has the same anchors as" — high.
- **LOW** [claude:claude-fable-5-1] README.md:47 — "two solvers agree" false for (3,7), (2,15), (3,15); referee covers them — high — "solvers and referee together cover every entry, each by ≥2 programs".
- **LOW** [claude:claude-fable-5-1] open-upper/proofs.txt:138-139 — "Always m≤q−1" is reachable-only; Theorem I covers m=q where 2^(r−1)=½ — high — state it.
- **LOW** [claude:claude-fable-5-1] open-upper/proofs.txt:63 — a_{m+1} undefined (should be ∞) — high.
- **LOW** [claude:claude-fable-5-1] open-upper/proofs.txt:29 — "as in B's proof of W(ii)" should say "by W(i) a nonempty set has an element ≤b" — high.
- **LOW** [claude:claude-fable-5-1] open-upper/proofs.txt:176 — f≤pk unjustified in text — high — add the charge-sum sentence.
- **LOW** [claude:claude-fable-5-1] open-upper/proofs.txt:153 — "f≥1" listed as a hypothesis in case m=1 but unused — low.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `open-upper/proofs.txt:62-68` — a_{m+1} is never defined although T_m is later used — confidence: high — define a_{m+1}=+∞
- **LOW** [cursor:gpt-5.6-sol-xhigh] `open-upper/proofs.txt:88` — succ_V(u) is undefined when u is maximal — confidence: high — define a missing successor as +∞
- **LOW** [cursor:grok-4.7-medium] `README.md:74` — “sliding one pebble forward” omits the backward retreat at `open-upper/proofs.txt:119` — high — say the shift has absolute value at most `2^(q−k−1)`.
- **LOW** [cursor:grok-4.7-medium] `open-upper/proofs.txt:87` — H_A requires `u'<succ_V(u)`; for q=3, A=∅, S0={3,4}, z=3, V={3} has no successor, while |4−3|=1 equals `2^(q−k−1)` — high — add “if a successor exists, else no upper neighbor”.
- **LOW** [cursor:grok-4.7-medium] `open-upper/proofs.txt:101` — the last case of Lemma N writes `w<succ_V(z)` in that same situation — high — same proviso.
- **LOW** [cursor:grok-4.7-medium] `open-upper/proofs.txt:138` — “Always m≤q−1” is false: for q=2 the set {2,4} is reduced and has m=2; it is true for boards reachable from empty — high — restrict the sentence to reachable boards.
- **LOW** [cursor:grok-4.7-medium] `open-upper/proofs.txt:172` — for f=0 the text reads the numeric clause of J, which is only stated for f≥1; the real reason is that J(0) forces A empty — medium — say that.
- **LOW** [cursor:grok-4.7-medium] `open-upper/proofs.txt:28` — W(iii) does not say why nodes >b form a play with ≤c−1 pebbles (it is W(i)) — high — add that sentence.
- **LOW** [cursor:grok-4.7-medium] `open-upper/proofs.txt:115` — “e≠s” is asserted, not tied to `s−u>2^φ` — high — add “so s is not reducible in V”.
- **LOW** [cursor:grok-4.7-medium] `open-upper/proofs.txt:176` — `f≤pk` is one line; the charge sum is only in `solver-b/proofs.txt:6-7` — high — copy that justification.
- **LOW** [cursor:gemini-3.8-flash-high] 07-charged-pebbles/open-upper/proofs.txt:138 — Claims "Always \(m \le q-1\)" while Theorem I is stated for "reachable or not"; unreachable boards can have \(m = q\) (\(r = 0\)), leaving \(2^{r-1} = 1/2\) in \(J(f)\) — confidence: high — State that \(m \le q-1\) holds for reachable boards, and clarify that for unreachable boards with \(m = q\), \(a_m \le (f+3)U - 1/2\) implies integer \(a_m \le (f+3)U - 1\).
- **LOW** [cursor:gemini-3.8-flash-high] 07-charged-pebbles/open-upper/proofs.txt:66 — States an arbitrary P/R path from \(A\) to \(S\) keeps \(A\) present, whereas an arbitrary path could temporarily remove anchors; this holds specifically for the canonical path reversing reduction steps \(S \to A\) — confidence: high — Specify taking the reverse of the reduction sequence from \(S\) to \(A\).
- **LOW** [cursor:gemini-3.8-flash-high] 07-charged-pebbles/open-upper/proofs.txt:27-28 — Proof of W(iii) cites B's proof of W(ii) instead of stating directly that by W(i) any nonempty set has an element \(\le b\), leaving \(\le c-1\) elements \(> b\) — confidence: high — Add a self-contained sentence invoking W(i).
- **LOW** [cursor:gemini-3.8-flash-high] 07-charged-pebbles/open-upper/proofs.txt:176 — Real-game reduction is compressed to one line without explicitly noting that P/R preserve charges and F decreases total charge by 1 with floor 0 — confidence: high — Add one sentence tracing charge conservation.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
