<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-153803 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q9** (split) | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q8 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q9
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 2 MODERATE, 9 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/README.md:61` — Theorem RO is overstated; `(1 [] [])` is a concrete counterexample — confidence: high — use the Q9 replacement
- **MODERATE** [cursor:grok-4.7-medium] `16-erasing-counter/README.md:62` — “separate at depth r−5 or higher” reads as depth ≥ r−5, but the argument gives LCA depth ≤ r−5; the r=5 clause (“across the root”) only matches the ≤ reading — high — write “lowest common ancestor at depth at most r−5 (at depth 5, the root)”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/upper/PROOFS.md:47-55` — U(i,K) depends on unrecorded branch-local mode history, although the intended construction is recoverable — confidence: high — add a mode-history parameter V
- **LOW** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/README.md:143-146` — the claim that the organiser read the proofs and traced both examples has no supporting record elsewhere in the repository — confidence: high — remove it or cite a review record
- **LOW** [cursor:grok-4.7-medium] `16-erasing-counter/README.md:134` — “four natural strengthenings”, while `upper/PROOFS.md:168-172` lists (i)–(v) — high — “five”
- **LOW** [cursor:grok-4.7-medium] `16-erasing-counter/README.md:143-144` — “The organiser read the proofs and traced the two small examples by hand” is not in `PROOFS.md` or the cited outputs — high — delete it, or cite a source that says so
- **LOW** [cursor:grok-4.7-medium] `upper/PROOFS.md:57-58` — leaf legality is spelled only for a leaf of the current S_i; a leaf reached after a later graft is the same argument with K′ and is not written out — medium — add that sentence for the grafted call
- **LOW** [cursor:grok-4.7-medium] `16-erasing-counter/README.md:16-17` — the upper bound is smaller than the state space by 2^r/(r+1), so “close to the number of memory states” is only fair of the exponent — medium — “within a factor 2^r/(r+1) of the number of states”
- **LOW** [cursor:gemini-3.8-flash-high] 16-erasing-counter/README.md:134 — The text states "The follow-up lists four natural strengthenings", but `16-erasing-counter/upper/PROOFS.md:168-173` enumerates five strengthenings ((i) through (v)) — confidence: high — Change "four natural strengthenings" to "five natural strengthenings"
- **LOW** [cursor:gemini-3.8-flash-high] 16-erasing-counter/upper/PROOFS.md:116 — "lowest common ancestor has depth $\le r - 5$" is stated unconditionally but is informative only for $r \ge 5$ (for $r \le 4$, no cycle exceeds $W_r$, and $r-5 < 0$) — confidence: high — Add "For $r \ge 5$," or note vacuity for $r \le 4$
- **LOW** [cursor:gemini-3.8-flash-high] 16-erasing-counter/upper/PROOFS.md:139 — The listed evaluations in Theorem G begin at $r=2$ despite the theorem stating "for every $r \ge 1$" (formula at $r=1$ yields 2, which is exact) — confidence: high — Include "$r = 1: 2$, exact;" in the list

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
