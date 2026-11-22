<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-082213 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $2.039; this month $122.24 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $1.512, input 32 + cache write 44,852 + cache read 402,263 tokens — cache hit 90%
- claude:claude-opus-5-5: $0.526, input 24 + cache write 39,525 + cache read 228,054 tokens — cache hit 85%
- **cursor:grok-4.7-medium produced no answer:** NonRetriableError: Agent Looping Detected The model got stuck in a repeating response pattern, so this turn was stopped. Please try again with a different model or start a new conversation. If the pro

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q8** (split) | CONFIRMED | REFUTED | CONFIRMED | — | REFUTED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | — | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | — | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | — | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | — | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | — | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | — | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | — | CONFIRMED |

Disagreements (read these first): Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 1 CRITICAL, 4 MODERATE, 23 LOW
- **CRITICAL** [cursor:gpt-5.6-sol-xhigh] `verifier/ref.cpp:35-38`, `verifier/dp_7.log:7-16` — interior n=7 DP exhausts memory and aborts — confidence: high — guard unsupported sizes or use branch-and-bound
- **MODERATE** [claude:claude-opus-5-5] README:42 — The "exactly when" part is labelled "proved by hand, twice", but solver B's P7 (`solver-b/proofs.txt:25`) proves only that the minimum is attained. Only A proves the converse, and only A's script (p5check.py) and the referee's lemmas.py check it (n ≤ 9); B's misc.py checks only the minimum (n ≤ 8). — high — fix: "proved by hand by solver A; solver B proved the minimum only".
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:41-42` — the cited Hoeffding bound does not imply “at most n⁻²”; its factor reaches 1.6486 at n=9 (`verifier/lemmas.log:2`) — confidence: high — use `(1+o(1))n⁻²`
- **MODERATE** [cursor:gemini-3.8-flash-high] 06-one-feedback-bit/README.md:43 — Status cell claims the run-count biconditional was proved by hand twice, but solver-b/proofs.txt:26 only proved attainment — high — Change status to "attainment proved twice; biconditional proved by solver-a and brute-forced for n <= 9"
- **MODERATE** [cursor:gemini-3.8-flash-high] 06-one-feedback-bit/README.md:47 — Claims upper bounds without certificates rest on exhaustive search, but F(8,1) = 32 was proved via theorem P4 (reports/referee.md:12) — high — Change to "an exact-arithmetic certificate where one exists, otherwise theorem or exhaustive search"
- **LOW** [claude:claude-fable-5-1] `README.md:83-84` — "0011 and 1100 become usable behind the prefixes 01, 10 and 11": in the witness 1100 also sits behind 00 (`wit_6_2.txt:3`), and 0011 only behind 01, 11; also one of 375,360 optimal packings, so the mechanism is illustrative, not established — high — "In this packing 1100 is used behind every prefix and 0011 behind 01 and 11; this is one of 375,360 optimal packings."
- **LOW** [claude:claude-fable-5-1] `README.md:43` — "A(5) = 6 by exhaustive search (same four)": solver A's exhaustive log stops at n=4 (`solver-a/log_slow5.txt`), its A(5)≤6 is by certificate; the README author's "script of mine" (`README.md:12,43`) is not in the folder — high — "A(5) ≤ 6 by certificate (both solvers) and exhaustive search (referee); my own check script is not included."
- **LOW** [claude:claude-fable-5-1] `README.md:42` — "reached exactly when … proved by hand, twice": the "exactly" direction is proved only by solver A (`solver-a/proofs.txt:29-34`); solver B's P7 states "attained by" (`solver-b/proofs.txt:26`) — high — "proved by hand (the 'exactly' part by one solver); brute-forced for n ≤ 9".
- **LOW** [claude:claude-fable-5-1] `README.md:79` — "the b-words can be chosen so that a deletion inside the prefix lands on outcomes no other strategy uses" restates disjointness; not an explanation of why t=2 beats t=1 — medium — mark as "a description of the packing, not a mechanism".
- **LOW** [claude:claude-fable-5-1] `README.md:10` — "from there the best moment drifts towards the middle" rests on n=7,8 only (n=9 is lower bounds) — medium — "for n = 7, 8 the best moment is t = 3 or 4".
- **LOW** [claude:claude-fable-5-1] `README.md:81-83` — "as happens at n = 5": the (5,2) optimum already uses non‑constant‑run a=110 (`solver-a/wit_5_2.txt:8`), so the chain's premise was already shaky at n=5; the README reports it without noting this — medium — add "(already not quite true at n = 5: one optimal packing uses a = 110)".
- **LOW** [claude:claude-opus-5-5] README:83-84 — "0011 and 1100 become usable behind the prefixes 01, 10 and 11" doesn't match the witness. 1100 also follows 00 (`wit_6_2.txt:3`), and 0011 never follows 10 (only lines 6 and 12, under 01 and 11). — high — fix: "1100 is used behind all four prefixes, 0011 behind 01 and 11".
- **LOW** [claude:claude-opus-5-5] README:76-79 — The mechanism sentence ("b-words chosen so a deletion inside the prefix lands on unused outcomes") just restates disjointness; nothing in the folder analyses why 13 is possible. — medium — fix: label it "an informal description, not an explanation".
- **LOW** [claude:claude-opus-5-5] README:12, 43 — "a script of mine" and "(same four)": no such script is in the folder. Also, the solvers established A(5) ≤ 6 with LP certificates, not exhaustive search (`a:49-50`, `b:31`). — high — fix: "A(5) = 6 by exact-arithmetic dual certificates (both solvers) and the referee's exhaustive search"; drop "mine" or add the script.
- **LOW** [claude:claude-opus-5-5] README:47 — The Claim column states "F(9,3) = 67" as a fact while the Status column says "not established". The referee's F(9,7) = 60 (referee.md:22) is left out. — high — fix: "F(9,3) ≥ 67 and F(9,4) ≥ 67, with search bounds of 67 and 68 that are not certified".
- **LOW** [claude:claude-opus-5-5] README:9-10 — "from there the best moment drifts towards the middle" generalises from n ≤ 8; n = 9 has lower bounds only. — medium — fix: "for n = 6 to 8 the best t moves from 2 to 3–4".
- **LOW** [claude:claude-opus-5-5] `solver-a/wit_6_2.txt:1` — The header says "dualbound=13.0", but the LP bound is 14 (`log_lp.txt:18`); the 13 is the MILP bound. A reader could think a tight dual certificate exists for (6,2). — medium — fix: rename the field to "milp_bound".
- **LOW** [claude:claude-opus-5-5] README:95-96 — "Both solvers used the same solver library for those" leaves out solver A's independent C++ search `bb2.cpp`, which agrees on all of n = 6 (`reports/solver-a.md:25`). — medium — fix: mention it.
- **LOW** [claude:claude-opus-5-5] `solver-b/proofs.txt:20` — The corollary F(n,t) ≤ 2^t·A(n−t) relies on A(0) = 1 and F(1,·), both outside n ≥ 2. The referee flagged this as harmless. — high — fix: state the convention A(0) = 1.
- **LOW** [claude:claude-opus-5-5] `solver-a/proofs.txt:41` — The n⁻² constant should be (1+o(1))·n⁻²; the README already discloses this. — high — fix: as the referee says.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:20-22` — the stated n≥2 range invokes undefined F(1,·), and the corollary at t=n needs undefined A(0) — confidence: high — restrict the range or define lengths 0 and 1
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-b/dp.cpp:21` — argument evaluation makes every memo-size diagnostic print zero (`solver-b/dp.log:1-25`) — confidence: high — evaluate `f(0)` before `printf`
- **LOW** [cursor:gemini-3.8-flash-high] 06-one-feedback-bit/README.md:76-79 — Explains 13 by asserting b-words are chosen so prefix deletions land on unused outcomes, which is speculative post-hoc narrative of one witness rather than an established causal mechanism — high — Qualify the paragraph as an empirical breakdown of the witness
- **LOW** [cursor:gemini-3.8-flash-high] 06-one-feedback-bit/README.md:45 — States A(5) = 6 was established by exhaustive search across all four checkers, omitting that solver-a and solver-b verified it via LP dual certificates — high — Change to "by dual certificates and exhaustive search"
- **LOW** [cursor:gemini-3.8-flash-high] solver-a/proofs.txt:42 — Hoeffding bound tail probability at m = n/2 - sqrt(n ln n) is stated as "at most n^-2", but the bound yields (1+o(1))n^-2 (1.65 n^-2 at n=9; reports/referee.md:33) — high — Replace "at most 2^n * n^(-2)" with "(1+o(1)) 2^n * n^(-2)"
- **LOW** [cursor:gemini-3.8-flash-high] solver-b/proofs.txt:20 — Corollary F(n,t) <= 2^t A(n-t) for 1 <= t <= n evaluates at t=n to undefined values A(0) and F(1,·) outside the problem domain n >= 2 — high — Restrict t to 1 <= t <= n-2 or define base cases A(0)=1, F(1,0)=1
- **LOW** [cursor:gemini-3.8-flash-high] solver-b/dp.cpp:21 — Unspecified argument evaluation order in printf evaluates memo.size() before f(0) runs, printing "memo 0" on every line of solver-b/dp.log — high — Evaluate f(0) into a local variable before calling printf
- **LOW** [cursor:gemini-3.8-flash-high] reports/solver-a.md:39 — Mislabels bad2_wit_6_2.txt as having an "extra conflicting strategy", but the file has 8 strategies (7 valid, 1 conflicting) rather than 14 — high — Correct description to "truncated witness with one conflicting strategy"

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
