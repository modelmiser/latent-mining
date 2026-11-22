<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261119-212448 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $2.871; this month $192.43 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.110, input 32 + cache write 64,823 + cache read 538,175 tokens — cache hit 89%
- claude:claude-opus-5-5: $0.761, input 32 + cache write 54,827 + cache read 498,371 tokens — cache hit 90%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q8** (split) | CONFIRMED | REFUTED | REFUTED | REFUTED | REFUTED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | REFUTED | REFUTED | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 1 CRITICAL, 8 MODERATE, 28 LOW
- **CRITICAL** [cursor:gpt-5.6-sol-xhigh] `solver-b/mc.h:12-16,85-93`; `reports/solver-b.md:33-38` — the default C++ checker allocates 8 read branches but parses the included b=14 witness, causing out-of-bounds memory access and potentially checking corrupted transitions — confidence: high — use dynamically sized storage or reject b>MAXB before parsing
- **MODERATE** [claude:claude-fable-5-1] README.md:93-94 — claims no solver wrote why amounts 1,1,2 mod 4 decode; solver-a/proofs.txt:102-103 and solver-b/proofs.txt:106-110 do — high — replace per Q7
- **MODERATE** [claude:claude-opus-5-5] README.md:94 — The claim that the decoding is "not written out by either solver" is false (solver-a/proofs.txt:102-103; solver-b/proofs.txt:106-110) — high — use the wording in Q7.
- **MODERATE** [claude:claude-opus-5-5] README.md:47 — "checked by program for n ≤ 5" is wrong for solver A's 2^n − 1 bound. Its only checked instances are b = 7 (n = 3) and b = 15 (n = 4) (reports/solver-a.md:133; solver-a has no n = 5 b = 31 file). Solver B's bound does have n = 2..5 (w_n5_b16_s7.txt). The referee's wording (referee.md:29) is just as loose — high — "B's checked for n = 2–5; A's for n = 3, 4".
- **MODERATE** [claude:claude-opus-5-5] README.md:45 — "computed by exhaustive search by the solvers" is wrong for n = 3, b = 4, s = 4: solver A timed out (solver-a.md:104) and solver B left no log (referee.md:46). "One with 7" is a witness, not a search result — high — "no solution with 4 states (solver B, no log; recomputed by the referee); a 7-state witness exists".
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:112`; `../README.md:49` — “nothing settled beyond three” contradicts established bounds — confidence: high — say no exact value is settled beyond three
- **MODERATE** [cursor:grok-4.7-medium] `19-one-counter-mutex/README.md:93-94` — the read-decode is claimed unwritten, but both proof files write it — high — replace as in Q7
- **MODERATE** [cursor:grok-4.7-medium] `reports/referee.md:46` — “16.6M nodes” for n=3,b=4,s=4; the only log is `verifier/run_3_4_4_st_h0.txt:1`, split 0/2, 12,862,120 nodes, and split 1/2 is absent — high — cite that log, or say the saved run is one half
- **MODERATE** [cursor:gemini-3.8-flash-high] 19-one-counter-mutex/README.md:93-94 — The claim that "Why these amounts let each process learn enough from a read is not written out by either solver" is false; solver A gives the sum distinctions (`solver-a/proofs.txt:102-104`) and solver B gives the general decoding proof (`solver-b/proofs.txt:108-111`) — confidence: high — Correct the sentence to credit both solvers' decoding explanations.
- **LOW** [claude:claude-fable-5-1] README.md:45 — "exhaustive search by the solvers": n=3,b=4,s=4 only solver B finished (solver-a.md:28) — high — "by solver B (A timed out) and recomputed by the referee"
- **LOW** [claude:claude-fable-5-1] README.md:47 — "checked by program for n ≤ 5" true only of B's 2^(n−1); A's 2^n−1 checked for n=3,4 — high — "B's for n ≤ 5, A's for n = 3, 4"
- **LOW** [claude:claude-fable-5-1] README.md:85 — "first paragraph" but e_i+a_j∉E_k is in the second — high — "first two paragraphs"
- **LOW** [claude:claude-fable-5-1] README.md:73-74 — "with R back at 0" omits that R returns to 0 only via the helper's crit/exit (proofs.txt:61-66) — medium — "after the helper has gone through crit and left, R is 0"
- **LOW** [claude:claude-fable-5-1] verifier/ — the 8,000 hand-built trials (chains.py) have no saved output, unlike the two seeded runs — medium — note "chains.py output not saved" in What to distrust
- **LOW** [claude:claude-fable-5-1] README.md:43 — "automata with 6 states": w_n2_b2_s6 has 3 and 6 states — high — "at most 6 states"
- **LOW** [claude:claude-opus-5-5] README.md:112-113 — "no search … addresses four processes with b between 4 and 7" is too strong. By L0, the s = 3 result "no two-process solution for b ≤ 8" (solver-b.md:185; referee.md:23) already rules out n = 4, s = 3 — medium — "except the trivial s = 3 case".
- **LOW** [claude:claude-opus-5-5] README.md:40 — The "Where" cell omits solver-a/proofs.txt Theorem 1 — high — add it.
- **LOW** [claude:claude-opus-5-5] README.md:71,80 — "reading 2" / "reads 2 forever": the process may also take add 0 steps, so "reads" is too narrow — high — "sees R = 2 / never changes R".
- **LOW** [claude:claude-opus-5-5] README.md:89-92 — "Same construction, lowering subtracts" does not fit solver A's witness, which is a hand-written variant: its process 1 never lowers except at exit — medium — "both solvers use variants of…".
- **LOW** [claude:claude-opus-5-5] referee.md:37 / README.md:39 — The 8,000 hand-built trials (verifier/chains.py) have no saved log; only the 561,600 are logged — medium — note this.
- **LOW** [claude:claude-opus-5-5] solver-b/proofs.txt:44-45 — The proof does not explicitly rule out crit being a read, although it is forced — high — add one clause.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:94` — claims neither solver explains decoding, but Solver A does — confidence: high — acknowledge its brief explanation
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:80` — hold loops may execute add 0 rather than repeatedly read — confidence: high — say “sees only 2 and never changes R”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:65` — omits that the all-rem configuration must be reachable — confidence: high — insert “reachable”
- **LOW** [cursor:grok-4.7-medium] `19-one-counter-mutex/README.md:85` — b=2 is pinned on the wrong paragraph — high — "L1(a)–(c) already contradict"
- **LOW** [cursor:grok-4.7-medium] `19-one-counter-mutex/README.md:45` — “the solvers” did not both finish the 4-state search — high — "solver B and the referee"
- **LOW** [cursor:grok-4.7-medium] `19-one-counter-mutex/README.md:47` — “checked for n≤5” is wider than `reports/solver-a.md:57` — high — name b=7 and b=15
- **LOW** [cursor:grok-4.7-medium] `19-one-counter-mutex/README.md:116-117` — test-and-set and “needed” are dropped from `prior-art.md:4` — high — use the prior-art wording
- **LOW** [cursor:grok-4.7-medium] `reports/referee.md:37` — 8,000 of the 569,600 trials have no run file; `verifier/refuter_run_seed7.txt:3` and `refuter_run_seed11.txt:3` sum only to 561,600 — medium — separate the logged 561,600 from the unlogged 8,000
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:101` and `reports/solver-a.md:42` — “7-state” while the file is 3, 7 and 6 states; the README already says so — high — "3, 7 and 6 states"
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:107-108` — lists b=4, 8, 14 as the checks of the 2^n−1 sketch — high — "n=3, b=7 and n=4, b=15"
- **LOW** [cursor:grok-4.7-medium] `solver-b/mc.h:53-58` — the progress graph only forces processes outside M to be in rem, so members of M may still be in rem; `verifier/vcheck.py:172-173` requires the active set to equal M. Under the rem/crit rules these coincide — low — filter on active set exactly M
- **LOW** [cursor:grok-4.7-medium] `solver-a/checker.py:36` — an add constant is reduced mod b with no range check — low — reject constants outside 0..b−1
- **LOW** [cursor:gemini-3.8-flash-high] 19-one-counter-mutex/README.md:100 — Attributes least moduli (2, 4, 8, 14) reporting solely to solver B, whereas solver A also computed and reported them via `burnsmin.py` (`reports/solver-a.md:44`, `solver-a/proofs.txt:108`) — confidence: high — Change "solver B reports" to "both solvers report".
- **LOW** [cursor:gemini-3.8-flash-high] 19-one-counter-mutex/README.md:47 — Claims sketch verified for n ≤ 5 generally, but solver A's `2^n - 1` sketch was only checked for n = 3, 4 (`reports/solver-a.md:58`) — confidence: high — Clarify that n = 5 was checked for solver B's weights only.
- **LOW** [cursor:gemini-3.8-flash-high] 19-one-counter-mutex/solver-a/proofs.txt:107 — Lists b = 4, 8, 14 under the `2^n - 1` sketch, which actually gives moduli 7 and 15 (`reports/referee.md:55`) — confidence: high — Correct listed moduli to 7 and 15 for `2^n - 1`.
- **LOW** [cursor:gemini-3.8-flash-high] 19-one-counter-mutex/verifier/broken2_progress.txt:1-26 — File intended as a progress test fails on safety (`reports/referee.md:58`) — confidence: high — Fix transition causing safety violation to isolate progress failure.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
