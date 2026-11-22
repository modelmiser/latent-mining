<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261119-201844 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $2.882; this month $189.56 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.191, input 30 + cache write 64,384 + cache read 483,593 tokens — cache hit 88%
- claude:claude-opus-5-5: $0.690, input 20 + cache write 49,656 + cache read 206,076 tokens — cache hit 81%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q7** (split) | CONFIRMED | REFUTED | CONFIRMED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q8 | REFUTED | REFUTED | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q7
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 7 MODERATE, 25 LOW
- **MODERATE** [claude:claude-opus-5-5] `20-short-memory-count/README.md:11-12` — "one too many at every even w from 4 on" assumes f(w) = w/2 − 1 exactly for all even w. That has been computed only for w = 4, 6, 8, 10; Theorem D proves only "at least one too many". — high — "which is too large at every even w from 4 on (by Theorem D), and exactly one too many at w = 4, 6, 8, 10"
- **MODERATE** [claude:claude-opus-5-5] `README.md:49` (top level) — "values to w = 6 by exhaustive search three times". Solver B's w = 6 run is pruned (`reports/solver-b.md:157-158`). — high — "values to w = 5 by unpruned search three times, w = 6 twice unpruned and once pruned, beyond that by pruned searches"
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:45` — “it equals e” is false for e>w — confidence: high — use min(e,w)
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-b/fast3.cpp:4,138-148` — NOSYM10 is documented as skipping a branch but only changes the log label — confidence: high — implement the skip or remove the option
- **MODERATE** [cursor:grok-4.7-medium] `20-short-memory-count/README.md:43` — attributes the w−s+1 state bound to both solvers; A stops at a conditional lemma (`solver-a/proofs.txt:41-46`) — high — credit only Theorem B.
- **MODERATE** [cursor:grok-4.7-medium] `README.md:49` — “exhaustive search three times” through w=6; B’s w=6 search is pruned (`solver-b/final_lb.log:9-10`) — high — say two unpruned searches at w=6, three only for w≤5.
- **MODERATE** [cursor:grok-4.7-medium] `20-short-memory-count/README.md:94-95` — treats N*(9,3)=11 as settled; the tree shows an 11-state witness and `results.txt:38` “≥11,” not a 10-state failure — medium — write “≤11 (11-state witness); results.txt’s ≥11 is the recorded lower claim.”
- **LOW** [claude:claude-fable-5-1] `README.md:49` (top-level) — "exhaustive search three times" to w=6 contradicts folder README:37 (B unpruned only to w=5); "no general lower bound" omits f≥1 — high — wording above
- **LOW** [claude:claude-fable-5-1] `20-short-memory-count/README.md:38` — "two solvers visit the same tree" ignores B's DFS order (`final_lb.log:12,14`); undercounts checks — high — wording in Q7
- **LOW** [claude:claude-fable-5-1] `20-short-memory-count/README.md:96` — Theorem C's "k ≥ w" listed under "Small errors" though the proof is valid as written — medium — "can be sharpened to k = w"
- **LOW** [claude:claude-fable-5-1] `20-short-memory-count/README.md:65-66` — S_w = B_w for all w 3..12, not only even — high — wording in Q6
- **LOW** [claude:claude-fable-5-1] `solver-b/proofs.txt:46-47` — Theorem A's complement symmetry moves the start state to K without stating start-independence (Theorem D:100-103 does) — medium — add one sentence
- **LOW** [claude:claude-fable-5-1] `20-short-memory-count/README.md:30` — points to `chain/3-link.md` for the question; the formal statement is `chain/3-question.txt` — high — link both
- **LOW** [claude:claude-opus-5-5] `20-short-memory-count/README.md:38` — "The two solvers' searches visit the same tree (identical node counts)". This holds only for B's breadth-first run; B's depth-first run has different node counts (3,883,022 and 16,682,611), though it uses the same code base. — high — "A's search and B's breadth-first run visit the same tree (identical node counts) and count as one check; B's depth-first run uses the same program; the referee's program is independent"
- **LOW** [claude:claude-opus-5-5] `20-short-memory-count/README.md:43` — "proved by hand by both solvers". A's P3 states the path lemma and stops short of the state-count bound; deriving the bound from it is immediate, but A did not write that step. — medium — "proved by solver B (Theorem B); solver A's P3 is the same path lemma"
- **LOW** [claude:claude-opus-5-5] `20-short-memory-count/README.md:57-58` — "The even inner levels may be worse". By A's P1 and my recomputation, only state 2 at w = 4 is worse; for even w ≥ 6 every state is within the bound. — medium — "Only at w = 4 is an even inner level worse (state 2, spread 2)…"
- **LOW** [claude:claude-opus-5-5] `20-short-memory-count/README.md:63-66` — "for even w it is the same machine". The isomorphism holds for every w ≥ 3, which matters for crediting the proofs as independent. For even w, A's P1 and B's Theorem D are two proofs about one machine. — medium — add "so for even w the two proofs are of the same machine"
- **LOW** [claude:claude-opus-5-5] `20-short-memory-count/README.md:41` — "tested the stated ranges by program" leaves out that `lemmas.py` printed 8 FAIL lines at w = 3, which the referee attributes to its own test's over-strict bound (`verifier/runs.log:42`). — low — mention it in "Small errors"
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:48` — “no general lower bound” contradicts f(w)≥1 — confidence: medium — add “beyond f(w)≥1”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-b/results.txt:38` — N*(9,3) is left as ≥11 although an 11-state witness proves equality — confidence: high — write “=11”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `verifier/mkbad.py:14` — the intended unlit-cycle test also lights a spread-2 state, so rejection exercises the wrong check — confidence: high — omit state 2 from the lit set
- **LOW** [cursor:gpt-5.6-sol-xhigh] `reports/solver-b.md:65` — the stated two-core limit was exceeded — confidence: high — rerun affected jobs under the limit if procedural compliance is required
- **LOW** [cursor:grok-4.7-medium] `20-short-memory-count/README.md:40` — “25 minutes” vs `verifier/runs.log:19` 24m44s — high — write 24m44s.
- **LOW** [cursor:grok-4.7-medium] `README.md:49` — “no general lower bound” omits proved f(w)≥1 for w≥2 — high — “no lower bound matching the upper bound; f≥1 is proved.”
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:16-17` — “Thus m(j)+m(j′)≥w/2” is not implied by m(x)≥(x+2)/2 when one of j,j′ is 1 and the other is not (that bound fails at x=1); the sum still holds, including equality only at w=4 — medium — add the j=1 case: 1+m(w−3)≥(w+1)/2.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:23` — “spread(B1)≤⌊w/2⌋−1” is false at w=3, where the same proof gives C(B1)⊆[1,2] (`verifier/runs.log:27-28`) — high — scope that sentence to w≥4.
- **LOW** [cursor:gemini-3.8-flash-high] README.md:49 — claims values to w = 6 were computed by exhaustive search three times, but solver B used pruned search at w = 6 (unpruned only up to w = 5) — confidence: high — replace with "values to w = 5 by unpruned search three times, w = 6 unpruned twice and pruned once, beyond that by pruned searches"
- **LOW** [cursor:gemini-3.8-flash-high] 20-short-memory-count/verifier/lemmas.py:65 — test harness asserts hi <= w // 2 for B1 which evaluates to 1 for w = 3, causing 8 spurious FAIL reports because P1 treats w = 3 separately with C(B1) in [1, 2] — confidence: high — change check to allow hi <= 2 when w == 3
- **LOW** [cursor:gemini-3.8-flash-high] 20-short-memory-count/solver-b/results.txt:38 — table gives N*(9,3) as ">=11" whereas an explicit 11-state witness exists in w9n11_s3.txt establishing N*(9,3) = 11 (as reported in solver-b.md:23) — confidence: high — replace ">=11" with "11"
- **LOW** [cursor:gemini-3.8-flash-high] 20-short-memory-count/solver-a/proofs.txt:41 — in P3 lemma proof "for e > w-b it equals e" is invalid when e > w because the sliding window length is w — confidence: high — replace "it equals e" with "it equals min(e, w)"
- **LOW** [cursor:gemini-3.8-flash-high] 20-short-memory-count/solver-b/proofs.txt:71 — in Theorem C proof "k >= w" is imprecise since there are only w+1 states total and p_0..p_w are distinct, forcing k = w — confidence: high — replace "k >= w" with "k = w"
- **LOW** [cursor:gemini-3.8-flash-high] 20-short-memory-count/verifier/bad/a3_w4_unlit_cycle.txt:1 — negative test file a3 was intended to test unlit-cycle detection but was rejected earlier by chk.py:65 for marking a spread-2 state lit — confidence: high — mark state 1 unlit and remove state 0 from the cycle so the unlit-cycle branch triggers

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
