<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-181709 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q2** (split) | CONFIRMED | REFUTED | CONFIRMED |
| **Q6** (split) | UNSUPPORTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q2, Q6
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 11 MODERATE, 15 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `18-write-once-stack/quad/PROOF.md:141-146` — claims a complete q=1…6 3q/3q+1 run matrix absent from both logs — confidence: high — add the missing outputs or narrow the claim
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `18-write-once-stack/README.md:8-12` — calls the superseded ⌊n/2⌋+2 bound “best” immediately before stating a stronger bound — confidence: high — say “previous bound” and state the combined maximum
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `18-write-once-stack/README.md:88-96` — describes the open gap using the obsolete lower endpoint — confidence: high — use the strongest current lower bound
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `18-write-once-stack/README.md:127` — the traceback records MemoryError but not q=7, n=29, command, or 6-GB limit — confidence: high — preserve a command-and-resource transcript
- **MODERATE** [cursor:grok-4.7-medium] `quad/PROOF.md:111-114` — left-of-j* and between-j* lives are called the bottom of S, and j*’s two values “the next” bottom; the q=3 prefix P0×4, POP, P0, P0 has order (j* values, then the between-pair). The set dropped is still a prefix, so R=O+(b) is a suffix, but the sentence given as the reason is false — high — state the order left, then j*, then between, and say all three are dropped.
- **MODERATE** [cursor:grok-4.7-medium] `18-write-once-stack/README.md:89` — “the gap between ⌊n/2⌋+2 and n−1 is the open question” is false once R(25)≥17 — high — name the max of the three proved floors.
- **MODERATE** [cursor:grok-4.7-medium] `README.md:49` — “from n=25 on” drops n=17 and 21–23, and “on top of 246 computed small schemes” is P4 only, not the quad proof — high — split the two lower bounds.
- **MODERATE** [cursor:grok-4.7-medium] `18-write-once-stack/README.md:120` — “attempt to compute R(9) … ran out of time” has no transcript, command, or output in the tree — high — remove it or point at a log.
- **MODERATE** [cursor:gemini-3.8-flash-high] 18-write-once-stack/README.md:53 — Claim that survival at 3q for q <= 6 is verified in `quad/run_quad.log` is unsupported; `run_quad.log` only logs q=5,6 and `organiser_rerun.txt` only adds q=4, with q=1,2,3 completely unlogged at t=3q — confidence: high — Restrict claim to q=4..6 or log test runs for q=1..3 at t=3q.
- **MODERATE** [cursor:gemini-3.8-flash-high] 18-write-once-stack/README.md:120 — Claim that the follow-up instance attempted R(9) with solver B's program and ran out of time has no supporting script, log, or report anywhere in `quad/` or the repository — confidence: high — Remove the sentence or commit the execution log.
- **MODERATE** [cursor:gemini-3.8-flash-high] README.md:49 — Top-level row 18 Status column is stale; it attributes the general lower bound exclusively to solver B's 246 small schemes (floor(n/2)+2), omitting the newly added 3·floor((n-1)/4)-1 quad scheme hand proof — confidence: high — Update row 18 Status to cite the quad scheme proof for n >= 25.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `18-write-once-stack/quad/checker.py:20-53` — validates monotonicity only on reachable legal transitions, not E on every configuration/operation as required of a top-scheme; `quad.py` itself is globally monotone — confidence: high — document the limitation or enumerate all inputs
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:48` — experiment 18’s headline/status omits the smaller-n improvements and the quad proof/check evidence — confidence: high — update the row
- **LOW** [cursor:gpt-5.6-sol-xhigh] `18-write-once-stack/README.md:120` — “Its attempt” attributes the R(9) timeout to the follow-up, whereas only Solver B’s earlier timeout is evidenced (`reports/solver-b.md:19`) — confidence: medium — attribute it to Solver B
- **LOW** [cursor:grok-4.7-medium] `18-write-once-stack/README.md:9` — “the best lower bound proved here is ⌊n/2⌋+2” is stale; the next sentence corrects it only from n=25 — high — lead with the max of the three bounds.
- **LOW** [cursor:grok-4.7-medium] `18-write-once-stack/README.md:52` — 296,855,529 is the length-≤18 count (`run_quad.log:17`); length ≤17 is 99,095,529 (`run_quad.log:16`), and this number sits in the 3q−1 row — high — say “length ≤ 18”.
- **LOW** [cursor:grok-4.7-medium] `18-write-once-stack/README.md:103-104` — the second level is not the pair scheme: the right zone is still static pair values, and a right-zone pop kills one pair — high — describe O then N.
- **LOW** [cursor:grok-4.7-medium] `18-write-once-stack/README.md:124` — “the bound 3n/4” is stronger than 3⌊(n−1)/4⌋−1 (at n=25, 18.75 versus 17) — high — say “the three-quarters bound”.
- **LOW** [cursor:grok-4.7-medium] `quad/PROOF.md:56` — “functions of the configuration alone” drops the operation, which E takes (`PROOF.md:46-54`) — high — add “and the operation”.
- **LOW** [cursor:grok-4.7-medium] `quad/PROOF.md:141` — “see run_quad.log” for q=1..6, but that log has no successful q=1..4 lines; those are in `organiser_rerun.txt:1-13` — high — cite both files.
- **LOW** [cursor:grok-4.7-medium] `quad/PROOF.md:10-11` — the corollary uses R(n)≥R(4q+1) with no sentence; the fact is real (`solver-a/proofs.txt:89`) — high — one line that extra cells stay 0.
- **LOW** [cursor:grok-4.7-medium] `quad/PROOF.md:127-128` — “everything right of the rightmost live pair is dead” does not place that pair outside j*; the missing fact is that j* owns the leftmost pairs of the right zone (`PROOF.md:38-42`) — medium — say that.
- **LOW** [cursor:gemini-3.8-flash-high] 18-write-once-stack/README.md:88 — "The gap between floor(n/2) + 2 and n - 1 is the open question" is stale; with the quad scheme established, the gap for n >= 25 is between ~3n/4 and n - 1 — confidence: high — Rephrase to reference the ~3n/4 lower bound.
- **LOW** [cursor:gemini-3.8-flash-high] 18-write-once-stack/README.md:10 — Short answer statement that "the best lower bound proved here is floor(n/2) + 2" directly contradicts the subsequent sentence giving ~3n/4 from n = 25 — confidence: high — Clarify that floor(n/2) + 2 is the best bound proved here for small n (n <= 24).
- **LOW** [cursor:gemini-3.8-flash-high] 18-write-once-stack/quad/PROOF.md:112 — Prose describes live pairs in quad j* as "next bottom-most" after quads between j* and j**, but quad j* index-precedes quads j*+1..j**-1 — confidence: high — Rephrase to state that quads 1..j**-1 collectively contain all stack elements strictly below quad j**.
- **LOW** [cursor:gemini-3.8-flash-high] 18-write-once-stack/quad/PROOF.md:89 — Equation (1) is stated as an inequality `s >= 4q - |R| - w (in fact equality)` when exact equality `s = 4q - |R| - w` holds identically across all four quad states in Mode B — confidence: medium — State `s = 4q - |R| - w` directly.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
