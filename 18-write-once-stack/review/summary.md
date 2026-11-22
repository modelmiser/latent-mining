<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261119-180358 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $2.751; this month $183.22 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $1.958, input 42 + cache write 57,708 + cache read 577,005 tokens — cache hit 91%
- claude:claude-opus-5-5: $0.792, input 32 + cache write 52,367 + cache read 481,889 tokens — cache hit 90%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q6** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| **Q7** (split) | CONFIRMED | CONFIRMED | UNSUPPORTED | REFUTED | CONFIRMED |
| **Q8** (split) | CONFIRMED | REFUTED | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q6, Q7, Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 9 MODERATE, 30 LOW
- **MODERATE** [claude:claude-fable-5-1] top-level `README.md:47` — "explicit schemes that three checkers accept": `solver-a/check.py` crashes (IndexError) on B's files and `solver-b/check.py` misparses A's (reports FAIL); each scheme was accepted by two checkers (its author's and `vcheck.py`), never three — high — "schemes that the author's checker and the referee's checker both accept"
- **MODERATE** [claude:claude-opus-5-5] top-level `README.md:47`; folder `README.md:9-10` — "three checkers accept" is false: the solvers' checkers cannot read each other's formats (one crashes, one reports a false FAIL), so each scheme passes only two checkers — high — "accepted by two independent checkers (own solver's and the referee's)".
- **MODERATE** [claude:claude-opus-5-5] `README.md:9-10,85`; top-level `README.md:47` — "⌊n/2⌋+2 the best general lower bound" understates what is available. R(n) ≥ R(8) = 7 for n ≥ 8, which beats ⌊n/2⌋+2 for n=8,9. The referee-checked `verifier/m6/` covers all 255 (w, 6) cases (I verified this), so the P4 argument with 7 spare cells gives ⌊(n+5)/2⌋ for n ≥ 7, but nobody has written or checked that proof (`referee.md:53`) — high — "best general lower bound max(⌊n/2⌋+2, 7 for n ≥ 8); the r = 6 schemes would give ⌊(n+5)/2⌋ (not written up)".
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:47` — unqualified upper bound is false at literal n=1 — confidence: high — add the n≥2 qualifier and n=1 exception
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `reports/solver-b.md:20-24` — r=6 impossibility values lack a certificate or independent computation — confidence: high — label that half unsupported
- **MODERATE** [cursor:grok-4.7-medium] `18-write-once-stack/README.md:6-11,31-32` — the lead calls the chain’s question open, but literal `R(1)=1` already falsifies `R(n)=n−1` for every n≥1; the chain’s only NO form (`R(n0)≤n0−2`, `chain/3-question.txt:7`) cannot state that — high — “Literal equality fails at n=1; open only for n≥2.”
- **MODERATE** [cursor:gemini-3.8-flash-high] 18-write-once-stack/chain/3-question.txt:10 — Upper bound argument assumes first push changes configuration and concludes R(n) <= n - 1 for all n >= 1, which fails at n = 1 where R(1) = 1 under literal text — confidence: high — Restrict to n >= 2 (literal) or require Q(0^n) = EMPTY.
- **MODERATE** [cursor:gemini-3.8-flash-high] 18-write-once-stack/chain/3-question.txt:12 — Claim that every non-final operation must change configuration is false; refuted by third PUSH1 in solver-a/witness_n5_t4.txt:7 leaving 01010 unchanged — confidence: high — Replace with exact condition requiring top r elements to be identical.
- **MODERATE** [cursor:gemini-3.8-flash-high] 18-write-once-stack/solver-b/proofs.txt:79 — Claimed stay condition for push ("iff |S| >= r and top r entries of S are all b") fails for r = 1 from empty stack under literal reading L where E(0, PUSH0) = 0 stays — confidence: high — Restrict iff to histories of length >= 1 or reading S.
- **LOW** [claude:claude-fable-5-1] top-level `README.md:47` and `18-write-once-stack/README.md:8` — "survives at most n − 1" is false at n=1 under the literal reading both solvers took as primary (R(1)=1, `solver-b/proofs.txt:34-36`) — high — "at most n − 1 for n ≥ 2"
- **LOW** [claude:claude-fable-5-1] `README.md:119` — "the third of three equal pushes leaves the memory unchanged": in that file only PUSH1,PUSH1,PUSH1 stays; PUSH0×3 sets a cell (10100→11100) — high — "the third of three PUSH1s"
- **LOW** [claude:claude-fable-5-1] `README.md:125` — "both stored schemes for n = 8": four n=8 files are stored (two per solver); B's S file is B's L file minus one Q line — medium — "in every stored n = 8 scheme"
- **LOW** [claude:claude-fable-5-1] `README.md:121-130` omits a real error in `reports/solver-a.md:38`: "Case A, both first pushes leave 0^n" inverts the case (proofs.txt:21 correctly says neither leaves 0^n) — high — add the bullet
- **LOW** [claude:claude-fable-5-1] `README.md:49` — "regenerated and checked by the referee" hides that the r=6 schemes were produced by B's `sat` binary (`referee.md:24`); only the checking is independent — medium — "regenerated with solver B's solver and checked by the referee"
- **LOW** [claude:claude-fable-5-1] `README.md:79` — "When the last pair has been touched" has no meaning for n=6,7 (p=0): the computed scheme runs from the start — high — add "(for n = 6, 7 there are no pairs and the computed scheme is used from the start)"
- **LOW** [claude:claude-fable-5-1] `README.md:107` — "read no source in full" vs `prior-art.md:38`, which says Sections I–III of item 1 were read directly — low — "read only part of one source"
- **LOW** [claude:claude-opus-5-5] `README.md:8`; top-level `README.md:47` — "at most n − 1" with no qualifier, but R(1)=1 under the literal reading (`README.md:69`) — high — "at most n − 1 (n ≥ 2)".
- **LOW** [claude:claude-opus-5-5] `README.md:87` — "would give R(n) = n − 1" fails at n=1 under the literal reading — high — add "for n ≥ 2".
- **LOW** [claude:claude-opus-5-5] `README.md:125` — "both stored schemes for n = 8": there are four, and all show the two-cell step — high — "all four stored n = 8 schemes".
- **LOW** [claude:claude-opus-5-5] `reports/solver-a.md:97` — Case A is stated with the opposite condition from the proof (`solver-a/proofs.txt:20`), and neither the referee nor the README flags it — high — "neither first push leaves 0^n".
- **LOW** [claude:claude-opus-5-5] `solver-b/check.py` — it ignores lines it cannot parse instead of rejecting the file, so a malformed file is checked as a different scheme (here a false FAIL, not a false pass) — medium — reject unparseable lines.
- **LOW** [claude:claude-opus-5-5] `solver-b/proofs.txt:16` — "depends only on whether |S| <= r" is loose: for |S| ≤ r the exact length matters — low — "depends only on min(|S|, r+1)".
- **LOW** [cursor:gpt-5.6-sol-xhigh] `18-write-once-stack/README.md:8,31-32,87-88` — three statements omit n≥2 despite R_L(1)=1 — confidence: high — add n≥2
- **LOW** [cursor:gpt-5.6-sol-xhigh] `chain/3-question.txt:5-10` — the formal bound and headline are already false at literal n=1 — confidence: high — scope them to n≥2
- **LOW** [cursor:gpt-5.6-sol-xhigh] `18-write-once-stack/README.md:47` — attributes a proof to Solver A where none is written — confidence: high — use the Q7 replacement
- **LOW** [cursor:gpt-5.6-sol-xhigh] `reports/solver-a.md:38` — “both first pushes leave 0^n” reverses Case A — confidence: high — say “both first pushes change 0^n”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:79-81` — the unrestricted push-stay “iff” fails at the literal start — confidence: high — exclude the start situation
- **LOW** [cursor:gpt-5.6-sol-xhigh] `reports/solver-b.md:43` — “no slack until the last two operations” is contradicted by the n=8 witnesses — confidence: high — remove it
- **LOW** [cursor:grok-4.7-medium] `README.md:47` — “n≤10” includes n=1, which no checker run covers — high — “checked for n=2..10”
- **LOW** [cursor:grok-4.7-medium] `README.md:108` — attributes asymptotic bounds to the 2009 paper; `prior-art.md:8-11` gives an exact linear bound there — high — “2009: whole value, t(C)≤n(q−1); 2013 journal bounds asymptotic”
- **LOW** [cursor:grok-4.7-medium] `solver-b/proofs.txt:16` — “POP legality depends only on `|S|≤r`” is false (r=2: empty vs `"0"`) — high — “keep the full stack if `|S|≤r`; a hidden suffix is unreachable iff `|S|≥r+1`”
- **LOW** [cursor:grok-4.7-medium] top-level `README.md:47` and folder `README.md:6` — “at most n−1” drops the n=1 literal exception — high — “at most n−1 for n≥2, and for all n under the strict reading”
- **LOW** [cursor:grok-4.7-medium] `README.md:75-76` — “one pair per operation” is not why the pair scheme lasts ⌊n/2⌋ steps: a pair is written on push and again on pop — medium — “at most ⌊n/2⌋ pushes, one fresh pair each”
- **LOW** [cursor:grok-4.7-medium] `solver-b/proofs.txt:64` — r=5−d≤5 is stated, but r≥0 is only true because a handover with d>5 is already past t — medium — “inside a play of length ≤t, 0≤r≤5”
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:38-40` — the n=1, t=2 sketch says both plays end at config 1, but both first pushes staying already needs Q(0)∈{0,1} — medium — “the second step needs the only cell, so the first cannot set it, and both pushes cannot stay”
- **LOW** [cursor:gemini-3.8-flash-high] 18-write-once-stack/reports/solver-b.md:44 — Claim that schemes have "no slack until the last two operations" is false; in both n = 8, t = 7 witnesses operation 5 of 7 gains two cells in 12 plays — confidence: high — Replace "last two operations" with "last three operations".
- **LOW** [cursor:gemini-3.8-flash-high] 18-write-once-stack/reports/solver-b.md:12 — States "Every entry is COMPUTED", but the r = 6 impossibility row of Mtable.txt on 6 cells was generated by uncertified SAT search only without certificate or referee verification — confidence: high — Clarify that M(w, 6) impossibility on 6 cells is SAT-search-only without certificate.
- **LOW** [cursor:gemini-3.8-flash-high] 18-write-once-stack/solver-b/hybrid.py:4 — Script sets n = 2p + 6, supporting only even n and failing to construct witnesses for odd n despite P4's claim for all n >= 6 — confidence: high — Accept n directly and pad with an unused trailing bit when n is odd.
- **LOW** [cursor:gemini-3.8-flash-high] README.md:46 — Top-level table claims "the memory survives at most n − 1 pushes and pops" omitting the qualification n >= 2 (since R(1) = 1 under literal reading) — confidence: high — Qualify with "for n >= 2" or note literal reading at n = 1.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
