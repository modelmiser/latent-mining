<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `solver-b/proofs.txt:22-36` and `chain/3-question.txt:5`: from step 2 on, alternating tops force a new 1; for n≥3, `PUSHb,PUSHb` stays at `0^n`, and `PUSHb,POP` vs `PUSHb,PUSHb,POP` (length 3≤n) need EMPTY and b at `E(0^n,POP)`; n=2 covers both bits and `c1=11`; `witness_n1_t1.txt:4` survives t=1 and t=2 needs three Q values — none

Q2: CONFIRMED — `solver-a/witness_n4_t3.txt:4-16` is monotone on every listed edge; sampled plays `PUSH0,PUSH1,POP`, `PUSH1,PUSH0,POP`, `PUSH1,POP,PUSH0`, `PUSH0,PUSH0,PUSH1` match the tops; targets are explicit, so `c≤E(c,op)` is checkable; `verifier/vcheck.py:39-60` walks every legal branch to depth t and checks Q after each op — none

Q3: CONFIRMED — `solver-b/proofs.txt:9-16,41-44`: hosts is an iff (reachability one way, induction the other); the top r+1 symbols, plus the whole stack when shorter, fix tops and POP legality for r steps; a t≥3 literal scheme cannot stay at `0^n`, so setting `Q(0^n)=EMPTY` is a strict scheme — none

Q4: CONFIRMED — `solver-b/proofs.txt:47-55`: touched pairs stay a prefix under interleaved pops (a pop only kills the rightmost live pair); live pairs left to right stay the stack; a play of length ≤⌊n/2⌋ has ≤⌊n/2⌋ pushes, so the next push still has a `00` pair — none

Q5: CONFIRMED — `solver-b/proofs.txt:62-70`: no `00` pair means p pushes and d=`11` pairs, so ops so far are p+d and r=t−p−d=5−d≤5, read off the cells; `Q(0^{r+1})=top(w)` matches the pair top; r=0 is Q-only; unexhausted pairs stay on the pair rule; odd n is one unused cell; t=⌊n/2⌋+2 for every n≥6 — none

Q6: CONFIRMED — `README.md:85-88`: universal “r+1 cells for r ops” at the empty stack plus the proved upper bound is R(n)=n−1 for n≥2; `README.md:6-11,51,93-96` mark that equality open and quote the 65% and 75–80% guesses — none

Q7: REFUTED — status cells match `reports/referee.md:9-26`, `reports/solver-a.md:9-16,30`, `reports/solver-b.md:9-19,41,70` (246, 255, r=6 uncertified, timeouts, iff, slack, two weak mutants) except `README.md:47` “n≤10” vs `reports/solver-b.md:32` and `reports/referee.md:17` “n=2..10”, and `README.md:108` “bounds are asymptotic” vs `prior-art.md:8-11` (2009 proves t(C)≤n(q−1); “all asymptotic” is the 2013 journal version) — “checked by program for n=2..10”; “the 2009 paper decodes the whole value and proves t≤n(q−1); the journal bounds are asymptotic”

Q8: REFUTED — `README.md:15-29` matches `chain/3-question.txt:1-5` (literal play; strict reading is named as extra); the top-level row `README.md:47` “at most n−1” is not the folder’s `R_L(1)=1` (`18-write-once-stack/README.md:43`, `solver-b/proofs.txt:34-36`) — “for n≥2 at most n−1 operations, exactly that for n=2..8; R(1) is 1 literal and 0 strict; whether n−1 holds for every n≥2 is open, best general lower bound found ⌊n/2⌋+2”

F: MODERATE — `18-write-once-stack/README.md:6-11,31-32` — the lead calls the chain’s question open, but literal `R(1)=1` already falsifies `R(n)=n−1` for every n≥1; the chain’s only NO form (`R(n0)≤n0−2`, `chain/3-question.txt:7`) cannot state that — high — “Literal equality fails at n=1; open only for n≥2.”
F: LOW — `README.md:47` — “n≤10” includes n=1, which no checker run covers — high — “checked for n=2..10”
F: LOW — `README.md:108` — attributes asymptotic bounds to the 2009 paper; `prior-art.md:8-11` gives an exact linear bound there — high — “2009: whole value, t(C)≤n(q−1); 2013 journal bounds asymptotic”
F: LOW — `solver-b/proofs.txt:16` — “POP legality depends only on `|S|≤r`” is false (r=2: empty vs `"0"`) — high — “keep the full stack if `|S|≤r`; a hidden suffix is unreachable iff `|S|≥r+1`”
F: LOW — top-level `README.md:47` and folder `README.md:6` — “at most n−1” drops the n=1 literal exception — high — “at most n−1 for n≥2, and for all n under the strict reading”
F: LOW — `README.md:75-76` — “one pair per operation” is not why the pair scheme lasts ⌊n/2⌋ steps: a pair is written on push and again on pop — medium — “at most ⌊n/2⌋ pushes, one fresh pair each”
F: LOW — `solver-b/proofs.txt:64` — r=5−d≤5 is stated, but r≥0 is only true because a handover with d>5 is already past t — medium — “inside a play of length ≤t, 0≤r≤5”
F: LOW — `solver-a/proofs.txt:38-40` — the n=1, t=2 sketch says both plays end at config 1, but both first pushes staying already needs Q(0)∈{0,1} — medium — “the second step needs the only cell, so the first cannot set it, and both pushes cannot stay”
