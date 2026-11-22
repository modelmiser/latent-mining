<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-170429 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q6** (split) | UNSUPPORTED | UNSUPPORTED | CONFIRMED |
| **Q7** (split) | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q6, Q7
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 3 MODERATE, 10 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `23-gossip-tug/growth/dcheck.cpp:2,41` — The comment and README claim every reply is explored, but phase-one exploration stops after the first legal reply; numeric phase-two tie-breaking means interchangeability is not encoded as a valid symmetry reduction — confidence: high — qualify the claim or use equivariant tie-breaking/full branching
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `23-gossip-tug/growth/dcheck.log:18-21` — The heading says every deliberately wrong rule “must print BELOW,” but variant 1 prints OK three times, so that negative control did not fail — confidence: high — replace variant 1 with a mutation that demonstrably lowers the guarantee or relabel it
- **MODERATE** [cursor:grok-4.7-medium] `growth/PROOFS.md:101-106` — Lemma 2 is labeled a complete restatement of solver-b's Lemma 3, but `solver-b/proofs.txt:13-18` is `value ≤ u` when each non-expert misses some expert, while Lemma 2 is "exactly `u` calls" when one expert is unknown to all non-experts, and no proof is written — high — prove the stated rule, or cite Lemma 3's actual bound
- **LOW** [cursor:gpt-5.6-sol-xhigh] `23-gossip-tug/README.md:152-159` — The prior-art warning discusses only the older \(2n-3\) result and does not say the search predates \(3n-6\) — confidence: medium — add that the new bound has not undergone a fresh literature search
- **LOW** [cursor:grok-4.7-medium] `23-gossip-tug/README.md:54` — "every reply … for `n=3..11`" is not a full tree: `growth/dcheck.cpp:42-43` keeps one phase-1 partner, `dcheck.log:14-17` is the full tree only for `n=5..8`, and `:10-13` aborts before any larger full run. Symmetry of untouched partners makes the minimum valid anyway — high — say "every reply up to phase-1 symmetry; full tree for `n=5..8`"
- **LOW** [cursor:grok-4.7-medium] `growth/PROOFS.md:10` — "exact for `n=3..6`" sits in a complete-proof header; equality for `n=5,6` is the computed row `README.md:49`, not this argument (`n=3,4` do follow from `3n-6` plus `n(n-1)/2`) — high — limit the parenthetical to `n=3,4` or point at the computed rows
- **LOW** [cursor:grok-4.7-medium] `growth/dcheck.log:18-21` — the banner says the wrong rules must print BELOW, but variant 1 prints OK and still meets `3n-6` — high — drop variant 1 from that claim
- **LOW** [cursor:grok-4.7-medium] `growth/PROOFS.md:25-26` — "flagged iff one of them was" reads as exclusive or; the union and line 27 are inclusive or — medium — "flagged iff at least one was"
- **LOW** [cursor:grok-4.7-medium] `growth/PROOFS.md:64` — "Hence" does not follow from the backward sentence; R1 stays gone because levels and flags never fall — medium — cite line 27 and the R2 case split
- **LOW** [cursor:grok-4.7-medium] `solver-b/proofs.txt:45` — still says `A(n) ≥ 3n-6` is unproved and only computed through `n=10`, which is now false of the experiment — medium — point that line at `growth/PROOFS.md`
- **LOW** [cursor:gemini-3.8-flash-high] `23-gossip-tug/README.md:79` — Section heading "Why at least 2n − 3 in general" does not mention that the bound is superseded for \(n \ge 4\) by the \(3n - 6\) proof directly following it — confidence: high — Retitle to "Why at least 2n − 3 in general (superseded for n ≥ 4)" or add a brief cross-reference note.
- **LOW** [cursor:gemini-3.8-flash-high] `23-gossip-tug/growth/PROOFS.md:30` — Range \(l_1..l_{m-1}\) is empty when \(n = 3\) (\(m = 1\)), leaving \(p\) as the sole non-top agent, which is correct in count (\(m = 1\)) but degenerates syntactically — confidence: high — Clarify with "(empty when n = 3, leaving only p)".
- **LOW** [cursor:gemini-3.8-flash-high] `23-gossip-tug/growth/PROOFS.md:100` — Lemma 2 is presented as a standalone numbered lemma immediately following Theorem 1 despite having no role in the lower bound proof — confidence: medium — Subordinate Lemma 2 under the "Upper bound: NOTHING PROVED" section.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
