<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261119-193357 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $3.463; this month $186.68 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.786, input 32 + cache write 83,446 + cache read 634,104 tokens — cache hit 88%
- claude:claude-opus-5-5: $0.677, input 26 + cache write 51,427 + cache read 274,908 tokens — cache hit 84%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q7** (split) | CONFIRMED | CONFIRMED | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q8 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q7
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 7 MODERATE, 19 LOW
- **MODERATE** [claude:claude-opus-5-5] README:181-183 — The text suggests that m ≤ 13 is protected by an independent recomputation. referee.md:64 says "All three exhaustive programs use the same normalisation", so the referee's red.cpp shares it too. — confidence: medium — Fix: "The referee's recomputation to 13 cards uses the same normalisation as the solvers."
- **MODERATE** [claude:claude-opus-5-5] README:67 — The claim cell states "36 is the least size" as a fact, but cases (7,5), (11..12, partial R) and (13..17, 2) were not searched (solver-a.md:27, referee.md:74). — confidence: high — Fix: "Within the strand-family cases searched, none is below 36; some cases were not searched."
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `17-cut-and-peek/README.md:67` — strand-family minimality is presented as fact although cases below 36 were not searched — confidence: high — call it the least found, not the least
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `verifier/vcheck.py:53-61` — declared n is only printed, not enforced; a three-player witness declaring n=4 can be reported valid — confidence: high — reject when declared and inferred n differ
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `verifier/tests.py:6-24` — negative tests have no assertions; one of ten objects labelled “must be rejected” is accepted, and the script still succeeds (`verifier/tests.out:8-11`) — confidence: high — assert expected outcomes and relabel the valid swap
- **MODERATE** [cursor:grok-4.7-medium] `17-cut-and-peek/README.md:73-83` — the writeup of T2 never says the frozen players are set to 1, so “against correctness” is justified for n = 3 and not, from this text alone, for n > 3 (the cited proof does say it: `solver-b/proofs.txt:37-38`) — high — insert “other players fixed to 1” at the definition of W.
- **MODERATE** [cursor:grok-4.7-medium] `17-cut-and-peek/README.md:12-18` — open-question and prior-existence claims are asserted, while the only in-repo source is an instance’s PDF reading (`prior-art-deep.md:13-18`, `prior-art.md:52-55`) — high — attribute them as in the Q7 replacement.
- **LOW** [claude:claude-fable-5-1] `README.md:184-185`, `solver-a/proofs.txt:82` — "validated against brute force … three players at 6 cards": the referee's n=3,m=6 raw/red comparison is zero-vs-zero (`verifier/crosscheck.txt`), and solver A's non-zero cross-check "1344 = 1344" has no slowred log in the folder (`crosscheck1.txt` holds only the fast side) — high — "for three players only a zero-against-zero check at 6 cards; solver A's private-pair count match is reported without a log"
- **LOW** [claude:claude-fable-5-1] `README.md:12-13,18,142-148,188-190` — statements about the papers asserted without per-line attribution — high — add "per the deep-read instance"
- **LOW** [claude:claude-fable-5-1] `README.md:186` — "Nothing is known between 16 and 35" slightly overstates; strand-family exclusions (a≤5 by LP, a=6 b≤5 exact) do exist, as the next sentence admits — medium — "No exhaustive result between 16 and 35"
- **LOW** [claude:claude-fable-5-1] `README.md:59`, `verifier/tests.out` — ten "broken" variants were tried; one (swap of two identical cards) is a no-op and was accepted; "nine" is right but the no-op is unmentioned — high — none needed
- **LOW** [claude:claude-fable-5-1] `README.md:33` — shift direction left implicit; harmless since M(x) is the same either way — high — "moves j places forward"
- **LOW** [claude:claude-opus-5-5] README:74 — "Fix all players but three" does not say the others hold 1. If they hold 0, all eight inputs are AND-0 and there is no contradiction. — confidence: high — Fix: add "holding 1".
- **LOW** [claude:claude-opus-5-5] README:103 — The zero-sum case does not say that it needs m even and is empty for m odd. — confidence: high — Fix: add "(possible only for m even)".
- **LOW** [claude:claude-opus-5-5] README:107-108 — Only the pairs (1,2),(1,3) are checked, with no symmetry remark. — confidence: high — Fix: "any two of the three pairs share an index; by symmetry take (1,2),(1,3)".
- **LOW** [claude:claude-opus-5-5] README:135-138 — For n > 4 the extra bits must also be fixed. The README never says that three-bit equality is not covered. — confidence: high — Fix: "fix x₄…xₙ = 0; three-bit equality (two inputs with output 1) is not covered."
- **LOW** [claude:claude-opus-5-5] README:11-14, 49-51, 142-145 — Statements about the outside papers are asserted as fact here. Hedging and attribution appear only at lines 47 and 126/175. — confidence: medium — Fix: prefix with "according to the deep-read instance".
- **LOW** [claude:claude-opus-5-5] README:61 — The Where column for m = 14 omits solver-a/fast_3_14_p*.txt, solver-b/fast2.out and m14w.part*.out. — confidence: high — Fix: list them.
- **LOW** [claude:claude-opus-5-5] README:184-185 — The validation list omits solver B's leaf-count cross-check (leafcount.py, solver-b.md:94). — confidence: low — Fix: mention it.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `17-cut-and-peek/README.md:160-165,188-190` — outside-paper facts are sometimes voiced as verified facts despite relying on model PDF reads — confidence: high — attribute them explicitly to `prior-art-deep.md`
- **LOW** [cursor:gpt-5.6-sol-xhigh] `17-cut-and-peek/README.md:185-186` — “Nothing is known between 16 and 35” is too absolute given several strand-family exclusions — confidence: high — say no general existence or impossibility result is known
- **LOW** [cursor:gpt-5.6-sol-xhigh] `17-cut-and-peek/README.md:57-58,169-171` — organiser, panel and absence-of-human-review claims are not independently evidenced in repository artifacts — confidence: medium — distinguish documented reviews from process self-report
- **LOW** [cursor:grok-4.7-medium] `17-cut-and-peek/README.md:58` — “this is what T2’s proof shows” covers the all-ones corner; the any-corner sentence is the organiser’s, as the same cell later says — high — split the cell into T2’s corner and the organiser’s restatement.
- **LOW** [cursor:grok-4.7-medium] `17-cut-and-peek/README.md:103` — m/2 is used without the “(m even)” of `solver-b/proofs.txt:51`; for odd m the zero-sum case is empty — high — add “m even; impossible if m is odd”.
- **LOW** [cursor:grok-4.7-medium] `17-cut-and-peek/README.md:135-140` — “fix x₄ = 0” and the string 0000 are the n = 4 cube; n > 4 needs x₄ = … = x_n = 0, which is not written — medium — write the extra fixings explicitly.
- **LOW** [cursor:grok-4.7-medium] `17-cut-and-peek/verifier/tests.py:17` — the position swap is listed as a broken witness but `verifier/tests.out:9` accepts it (9 of 10 rejected); the README’s “nine” still matches the failures — high — move that case out of the broken dict.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
