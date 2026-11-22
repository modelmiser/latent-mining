<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `solver-b/proofs.txt:15-26` matches safety at `chain/3-question.txt:12` and progress at `chain/3-question.txt:13-14` (stall in crit/rem is allowed). Leaving p in crit is a safety schedule; j parked only keeps R=0, and k entering while i is in crit is already illegal. — none

Q2: CONFIRMED — `solver-b/proofs.txt:40-45`. In Z_3 the only case consistent with L1(b)–(c) is E_k={0} and a_j=e_i=u. Multiplication of every add by 2, with read branch q'_w=q_{2w}, fixes 0 and preserves steps (2·2≡1). Solo crit at R=1 then all-rem forces the exit add to be 2. — none

Q3: CONFIRMED — `solver-b/proofs.txt:53-71`. (i) R must stay in {1,2} because h is in crit and q would run σ_q(0) if R hit 0. (iii) counter: 0 → 1 (p +1) → 2 (h +1) → 2 along τ → 1 (p +2) → 1 at h’s crit → 0 (h +2); h resumes at F_0 with R=1, the solo state after its first add, and q stays in rem. (iv) the next nonzero add a satisfies 1+a≢0, so a≠2, hence a=1. — none

Q4: CONFIRMED — `solver-b/proofs.txt:75-95`. The Step 3 replay is finite and reads only R; p’s net add is 0, so progress is not re-invoked. Steps 4 and 5: only p and q are active, both move forever, h stays in rem, nobody enters crit (`chain/3-question.txt:13-14`). Unequal level lengths are run one phase at a time; both infinite chains are all yields. Two types among three processes is exhaustive. — none

Q5: CONFIRMED — mixed first adds die at `solver-b/proofs.txt:40-42` (e_i+a_j lands in E_k). A crit read, or an exit other than +2, leaves an all-rem counter ≠0 (`solver-b/proofs.txt:44-45`). A hold/hold pair deadlocks at `solver-b/proofs.txt:81-85`; two infinite chains livelock at `solver-b/proofs.txt:87-92`. Extra reads and add 0 do not move R before the first nonzero add (`solver-b/proofs.txt:19-20,57-59`). Unreachable states are never scheduled. — none

Q6: CONFIRMED — `verifier/vcheck.py:157-168` rejects any reachable pair of crit states; `verifier/vcheck.py:169-189` flags a crit-free SCC whose active set is exactly a nonempty M and in which every member of M moves. `solver-b/w_n3_b4_s7.txt:3-25` and `solver-a/w_n3_b4_burns.txt:3-20` use state 0 as rem and state 1 as crit, only crit targets rem, and the state counts are 7/7/7 and 3/7/6. — none

Q7: REFUTED — `19-one-counter-mutex/README.md:85` says b=2 contradicts already in the first paragraph; T2 needs L1(c) (`solver-b/proofs.txt:32-34`), which that section states only in the next paragraph. `README.md:93-94` says neither solver writes why a read suffices; `solver-a/proofs.txt:102-103` and `solver-b/proofs.txt:106-112` do. The 104/182 counts are the referee’s (`reports/referee.md:17`). — "With b=2, L1(a)–(c) already contradict. The 1,1,2 mod 4 decode is written in solver A’s Theorem 3 and solver B’s T6; a general proof of the Burns protocol is not, so the n=3 claim is the model check."

Q8: REFUTED — `README.md:45` says both solvers exhaustively got “no 4-state solution” for n=3,b=4; solver A timed out (`reports/solver-a.md:28`). `README.md:47` says both sketches were checked for n≤5; solver A names only n=3,b=7 and n=4,b=15 (`reports/solver-a.md:57`). `README.md:116-117` says the 1982 step “reads and writes atomically” and that 2 values “suffice”; `prior-art.md:4` says atomic test-and-set, and that 2 values suffice and are needed for deadlock-freedom. Top-level `README.md:49` matches the folder headline, the 569,600 count, and “nothing settled” for n≥4. The other distrust bullets match `reports/referee.md:37,54-57,71` and `prior-art.md:29,41`. — "n=3,b=4,s=4 nonexistence is solver B plus the referee; solver A timed out. The 2^n−1 sketch is reported as machine-checked only for b=7 and b=15."

F: MODERATE — `19-one-counter-mutex/README.md:93-94` — the read-decode is claimed unwritten, but both proof files write it — high — replace as in Q7
F: MODERATE — `reports/referee.md:46` — “16.6M nodes” for n=3,b=4,s=4; the only log is `verifier/run_3_4_4_st_h0.txt:1`, split 0/2, 12,862,120 nodes, and split 1/2 is absent — high — cite that log, or say the saved run is one half
F: LOW — `19-one-counter-mutex/README.md:85` — b=2 is pinned on the wrong paragraph — high — "L1(a)–(c) already contradict"
F: LOW — `19-one-counter-mutex/README.md:45` — “the solvers” did not both finish the 4-state search — high — "solver B and the referee"
F: LOW — `19-one-counter-mutex/README.md:47` — “checked for n≤5” is wider than `reports/solver-a.md:57` — high — name b=7 and b=15
F: LOW — `19-one-counter-mutex/README.md:116-117` — test-and-set and “needed” are dropped from `prior-art.md:4` — high — use the prior-art wording
F: LOW — `reports/referee.md:37` — 8,000 of the 569,600 trials have no run file; `verifier/refuter_run_seed7.txt:3` and `refuter_run_seed11.txt:3` sum only to 561,600 — medium — separate the logged 561,600 from the unlogged 8,000
F: LOW — `solver-a/proofs.txt:101` and `reports/solver-a.md:42` — “7-state” while the file is 3, 7 and 6 states; the README already says so — high — "3, 7 and 6 states"
F: LOW — `solver-a/proofs.txt:107-108` — lists b=4, 8, 14 as the checks of the 2^n−1 sketch — high — "n=3, b=7 and n=4, b=15"
F: LOW — `solver-b/mc.h:53-58` — the progress graph only forces processes outside M to be in rem, so members of M may still be in rem; `verifier/vcheck.py:172-173` requires the active set to equal M. Under the rem/crit rules these coincide — low — filter on active set exactly M
F: LOW — `solver-a/checker.py:36` — an add constant is reduced mod b with no range check — low — reject constants outside 0..b−1

The b=3 write-up matches T3 apart from the two Q7 sentences. Saved pair logs `solver-b/log_b4_s4_p0.txt:2` and `log_b4_s4_p1.txt:2` sum to 116,680, and the four `log_b3_s5_q*.txt` parts sum to about 3.32·10^9 with 0 solutions, which matches solver B’s tables.
