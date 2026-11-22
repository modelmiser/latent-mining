<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — alternating tops force strict configuration growth; the fixed-push, n=2 symmetric case, c1=11, and n=1 witness are handled correctly (`solver-b/proofs.txt:20-37`; `solver-a/witness_n1_t1.txt:3-5`) — none

Q2: CONFIRMED — e.g. PUSH0,PUSH1,POP follows 0000→1000→1100→1110 with top 0, while PUSH1,PUSH0,POP follows 0000→0100→0110→0111 with top 1 (`solver-a/witness_n4_t3.txt:4-14`); transitions expose monotonicity, and `vcheck.py` checks it then recursively visits every legal prefix and checks Q after each operation (`verifier/vcheck.py:39-59`) — none

Q3: CONFIRMED — the hosts conditions are exactly reachability induction, and a bottom marker plus the top r+1 bits preserves both future tops and POP legality; P2 correctly proves 0^n cannot be revisited after either first push when t≥3 (`solver-b/proofs.txt:8-16,40-44`) — none

Q4: CONFIRMED — pushes extend the touched prefix, pops kill the rightmost live pair, and surviving pairs remain the stack bottom-to-top under arbitrary interleaving; before any push among ≤p operations there have been at most p−1 prior pushes (`solver-b/proofs.txt:47-56`) — none

Q5: CONFIRMED — exhaustion means exactly p pushes and d pops, so the configuration determines w, d, and r=5−d≤5; strict sub-schemes start with Q(0)=top(w), r=0 needs no further transition, unexhausted paths retain the pair invariant, and one odd cell is unused (`solver-b/proofs.txt:58-70`; `solver-b/hybrid.py:15-45`) — none

Q6: REFUTED — the implication works only for n≥2: at n=1 it invokes G(0), while literal R(1)=1≠0 (`18-write-once-stack/README.md:87-88`; `solver-b/proofs.txt:34-37`); otherwise the README consistently calls the n≥2 headline open (`18-write-once-stack/README.md:51`) — “If that held for every r, it would give R(n)=n−1 for every n≥2.”

Q7: UNSUPPORTED — the reported counts and disclosed defects match the reports (`reports/referee.md:10-45`), but “proved by hand by both solvers” is not supported: Solver A merely labels the pair bound COMPLETE without an argument (`18-write-once-stack/README.md:47`; `solver-a/proofs.txt:76-77`) — “proved by solver B; stated and computationally checked by solver A”

Q8: REFUTED — the game prose matches the formal definition (`18-write-once-stack/README.md:12-28`; `chain/3-question.txt:1-5`), but the top-level row’s unqualified “at most n−1” fails for literal n=1 (`README.md:47`; `solver-b/proofs.txt:20,34-37`) — “for n≥2, at most n−1; under the literal reading R(1)=1”

F: MODERATE — `README.md:47` — unqualified upper bound is false at literal n=1 — confidence: high — add the n≥2 qualifier and n=1 exception

F: MODERATE — `reports/solver-b.md:20-24` — r=6 impossibility values lack a certificate or independent computation — confidence: high — label that half unsupported

F: LOW — `18-write-once-stack/README.md:8,31-32,87-88` — three statements omit n≥2 despite R_L(1)=1 — confidence: high — add n≥2

F: LOW — `chain/3-question.txt:5-10` — the formal bound and headline are already false at literal n=1 — confidence: high — scope them to n≥2

F: LOW — `18-write-once-stack/README.md:47` — attributes a proof to Solver A where none is written — confidence: high — use the Q7 replacement

F: LOW — `reports/solver-a.md:38` — “both first pushes leave 0^n” reverses Case A — confidence: high — say “both first pushes change 0^n”

F: LOW — `solver-b/proofs.txt:79-81` — the unrestricted push-stay “iff” fails at the literal start — confidence: high — exclude the start situation

F: LOW — `reports/solver-b.md:43` — “no slack until the last two operations” is contradicted by the n=8 witnesses — confidence: high — remove it

Unasked: no further material defect found.
