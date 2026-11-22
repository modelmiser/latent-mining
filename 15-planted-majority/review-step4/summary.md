<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-060001 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q7** (split) | CONFIRMED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q7
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 2 MODERATE, 12 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/solver-a/proofs.txt:105` — \(\min_{t\in T}\) is undefined when \(T\) is empty, although the bound then holds trivially — confidence: high — handle \(T=\varnothing\) with \(c=0\)
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/step4-rewritten.md:6-14` — the two independent adjudications and their provenance are asserted, but their reports and model metadata are not archived in the repository — confidence: high — include both reports and metadata, or label them unarchived
- **LOW** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/solver-a/proofs.txt:108-110` — the original proof still attempts to activate nonexistent \(h_{2m+2}\) after its last stage — confidence: high — activate \(h_{j+1}\) only for \(j<2m+1\)
- **LOW** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/step4-rewritten.md:55` — “stretches do not overlap” is literally false at shared endpoint configurations, though no move is double-counted — confidence: high — say their move sets are disjoint
- **LOW** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/step4-rewritten.md:71-72` — “cannot be tested by program” is too absolute; the proof could be mechanically formalized even without a satisfying protocol witness — confidence: high — say no end-to-end witness test exists and no formal verification was performed
- **LOW** [cursor:grok-4.7-medium] `step4-rewritten.md:69-70` — “the scheduler is never obliged to choose them” is too strong for a complete play: if the only legal pairs are outside `S_j`, some such pair must eventually be played. The construction still holds, because that pair is unchanged until the phase that contains both agents. — high — say it is never obliged to choose them while `S_j` has a legal move, and an outside pair is played only once both agents are in `S`.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:92-93` — “no cycle” is taken to imply “all plays are finite” without saying that a fixed finite population over finite `Q` has finitely many multisets. — high — add that the configuration graph is finite.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:92` — “a sub-multiset of some instance” is asserted with no witness. The witness `(a+b+2, b, plant q_A)` meets `|a′−b′|=a+2≥2` and contains `aA+bB`. — high — name that instance.
- **LOW** [cursor:grok-4.7-medium] `step4-rewritten.md:6-14` — the claims that two independent same-family readers both found the step sound, and that this text is the second reader’s rewrite, appear only here and in `README.md:141-145`; the repo has no adjudication transcript. — high — point to those reviews, or label the sentences as the author’s account.
- **LOW** [cursor:grok-4.7-medium] `step4-rewritten.md:54-55` — `m` disjoint stretches of at least `k+1` moves give at least `m(k+1)` moves; the text concludes the weaker `k·m`. The contradiction is unchanged. — high — write `m(k+1)`.
- **LOW** [cursor:gemini-3.8-flash-high] 15-planted-majority/step4-rewritten.md:26 — Recurrence S_j = S_(j-1) U {h_j} is stated for phase j (j = 0, ..., 2m + 1), referencing undefined S_(-1) at j = 0 — confidence: high — Restrict the recurrence to j = 1, ..., 2m + 1 after defining S_0
- **LOW** [cursor:gemini-3.8-flash-high] 15-planted-majority/solver-a/proofs.txt:110 — Stage j = 0..2m+1 activates h_{j+1}, referencing non-existent agent h_{2m+2} at stage 2m+1 — confidence: high — Index stages as j = 0..2m or use phase bookkeeping as in step4-rewritten.md
- **LOW** [cursor:gemini-3.8-flash-high] 15-planted-majority/solver-a/proofs.txt:104 — Step 3 defines delta as min over t in T of -r.D_t without explicitly noting the edge case T = empty where delta and c are undefined — confidence: high — Note that T = empty implies no moves can occur, which fails planted instances with mixed initial states like (3,1,q_A)
- **LOW** [cursor:gemini-3.8-flash-high] 15-planted-majority/solver-a/proofs.txt:118 — Claims "at least k moves" between stages j and j+2, whereas 2k+j agents changing state requires at least k+1 moves — confidence: high — Replace "at least k moves" with "at least k + 1 moves"

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
