<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-165436 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q2** (split) | UNSUPPORTED | CONFIRMED | CONFIRMED |
| **Q6** (split) | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q2, Q6
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 4 MODERATE, 7 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `shape/PROOFS.md:38-40` — the general-family formula lacks the required once-used empty exit, so it does not follow from the stated assumptions — confidence: high — add that hypothesis
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:67` — 6,003 trees are mislabeled as random class-C trees; only 6,000 are random and only H is generated — confidence: high — describe the actual corpus
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:65,154-158` — elapsed-time and single-author provenance are unsupported by repository files — confidence: high — remove or cite provenance
- **MODERATE** [cursor:grok-4.7-medium] `README.md:67` — “6,003 random trees of the class” overstates `shape/out_testZ.txt:1`: 6,000 generators plus 3 fixed trees, and `shape/test_Z.py:26-35` reuses indices inside `S_i`, so class `𝒞` is not what was sampled — high — state the sample as top-level `(H)` only, plus the three named trees
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:67,148-150` — “recursively inside them” omits that recursion is on the pruned S_i° — confidence: high — say “recursively inside the pruned hanging subtrees”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `shape/PROOFS.md:157-159` — wording suggests the abstract general clause rests on computation; only its depth≤4 application does — confidence: high — distinguish theorem from corollary
- **LOW** [cursor:grok-4.7-medium] `README.md:67` — “the two small examples below” names both, but the page prints only `(1 (2 [1] []) (2 [2] [2]))` at `README.md:134`; (b) is only `upper/PROOFS.md:159-164` — high — point both examples at `upper/PROOFS.md` §5
- **LOW** [cursor:grok-4.7-medium] `README.md:65` — “for a few hours on 21 November” is not recoverable from the files (no commit or timestamp of when the conjecture was inserted) — medium — delete the duration
- **LOW** [cursor:grok-4.7-medium] `shape/out_testZ.txt:2-7` — six `WITNESS Z1a` lines sit directly under the clean `MUT 0` summary but belong to the following `MUT 1` run (`viol_Z1a: 161` at line 8) — high — print the mutation id on each witness line
- **LOW** [cursor:gemini-3.8-flash-high] `16-erasing-counter/README.md:67` — "tested on 6,003 random trees of the class" conflates 6,000 randomly generated trees with 3 deterministic witness trees (examples a, b, and \(T^*\)) tested in `shape/test_Z.py:180-196` and recorded in `shape/out_testZ.txt:1` — confidence: high — replace "6,003 random trees" with "6,000 random trees and 3 fixed witnesses"
- **LOW** [cursor:gemini-3.8-flash-high] `16-erasing-counter/shape/PROOFS.md:85` — Theorem Z1 "moreover" clause specifies destination mode as "\(m_{u+1}\) if \(u < l\)" but leaves the destination mode unspecified when \(u = l\) and mode \(m_l\) is a jumper (which loops back to some \(m_p\) with \(p \le l\)) — confidence: high — change "\(m_{u+1}\) if \(u < l\)" to "\(m_{u+1}\) if \(u < l\), or some \(m_p\) with \(p \le l\) if \(u = l\)"

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
