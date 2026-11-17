REPORT (solver-a). All files are in ./solver-a/

**Headline: the conjecture "S(P) = 1 for all P >= 2" is false.** S(3) = F(3) = 2 and S(4) = F(4) = 3. For P <= 4 the same-cost promise costs the adversary horizon, not value.

**1. Tables**

Exhaustive entries (COMPUTED) come from a full game-tree search over every release multiset that keeps the promise, with no pruning of adversary or scheduler moves (`game.cpp`, output in `tables.txt`).

| | T=1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|---|
| F_T(2) | 0 | 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | |
| S_T(2) | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 1 | | |
| F_T(3) | 0 | 0 | 1 | 1 | 1 | 2 | 2 | 2 | 2 | |
| S_T(3) | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 1 | 2* | |
| F_T(4) | 0 | 0 | 1 | 1 | 1 | 2 | 2 | 2 | 2 | 3 |
| S_T(4) | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 1† | 2..3* | |

- P = 1 gives 0 in both games for T <= 6 (COMPUTED).
- `*` Not exhaustive. The lower bound is a strategy found by a restricted adversary (one job per tick) and checked independently; the upper bound is Theorem 1. So S_9(3) = 2 exactly.
- `†` S_8(4) = 1 is exhaustive but needed the dominance reduction (Lemma D in `proofs.txt`) to fit in memory.
- S_16(4) = 3: independently checked strategy plus the upper bound.
- For T = 9..15, 2 <= S_T(4) <= 3 is undecided. A restricted search at T = 15 found no 3, which is inconclusive.
- Stabilisation: F reaches P-1 at T = 3, 6, 10 (exact). S reaches P-1 at T = 4, 9 (exact) and by T = 16 for P = 4. All of these are final by the upper bound.

**2. Witnesses**

Strategy trees are in `strat_{S,F}_P*_T*.txt`; `python3 showstrat.py <file> <depth>` prints them.

- **S(2) >= 1, T = 4:** release X = (0,2,4). If X is run in tick 0, release (1,1,2); hindsight is idle, U, X, X. If the scheduler waits, the hindsight "already ran X", and a tight unit or (3,1,4) arrives just after the scheduler starts X. Full hand tree is Theorem 3.
- **S(3) >= 2, T = 9 (163 nodes), main line:**
  - Release decoy X = (0,2,9). If run, release Y = (1,3,7); the scheduler cannot touch Y in tick 1.
  - One tick after it starts Y (at tick s), release U = (s+1,1,s+3).
  - If it continues Y, release a second unit with the same deadline: two units plus Y's remainder gives lateness 2. Hindsight: idle, Y, switch, U, U', Y, Y, X, X.
  - If it switches, release (5,1,6) and (6,1,7): four units in three ticks. Hindsight: idle, Y, Y, Y, U, V, W, X, X.
  - If it declines X, the hindsight ran X and X itself becomes work the scheduler is behind on.
- **S(4) >= 3, T = 16 (1877 nodes):** same shape. X = (0,4,16), then (1,4,10), i.e. an inner window of length 9.
- **Why the value is enough:** non-preemptive EDF gives P-1 (Theorem 1). It never switches and never idles with work pending, so it is an optimal rule for the limit whenever the value is P-1.
- **Idling:** at finite T, a scheduler forbidden to idle with work pending still achieves the exact value at every size tested (S: P=3 T<=8, P=4 T in {4,6,7}; F: P=3 T<=8, P=4 T in {3,6,8,9}).
- **Switching:** it is needed at small T. Never-switch gives 2 at P=3, T=5, where S = F = 1.
- **"Switch only when otherwise a deadline is missed":** two variants I tried are not optimal at finite T; they give 2 at P=3, T=6 and T=7, where S_T = 1. Definitions are in `checks.txt`.

**3. Headline and confidence**

- S(2) = F(2) = 1, S(3) = F(3) = 2, S(4) = F(4) = 3: about 97% confident. Each rests on a proved upper bound and an independently checked lower-bound certificate.
- Conjecture: S(P) = F(P) = P-1 for all P, with F first reaching it at T = P(P+1)/2 and S at T = P^2. About 75% confident for P >= 5.
- For P >= 5, the proved bounds are 3 <= S(P) <= F(P) <= P-1.
- Restricted searches for F(5) >= 4 at T = 15 and 16 found nothing, and less restricted ones ran out of memory. That is inconclusive.

**4. Proofs** (`proofs.txt`)

- **Theorem 1, COMPLETE:** S_T <= F_T <= P-1 for all T, by non-preemptive EDF with a last-idle-or-later-deadline-tick argument.
- **Lemma 2, COMPLETE:** monotone in T and P.
- **Theorem 3, COMPLETE (by hand):** S_4(2) >= 1.
- **Theorem 4, COMPLETE as computer-checked certificates:** S_9(3) >= 2, S_16(4) >= 3, F_6(3) >= 2, F_10(4) >= 3. This is not a human-readable proof.
- **Lemma D, COMPLETE:** dominance of hindsight states. It is used only for S_8(4) and to speed up searches.
- **Conjecture 5, SKETCH only:** the decoy mechanism. No induction was completed.

Checks, all in `checks.txt`:
- Online switch cost 0 gives value 0.
- EDF promise equals the frontier promise at cost 0.
- The promise machinery matched brute-force feasibility on about 0.5M job lists per case, with 0 mismatches.
- Dominance on and off agree.
- S <= F and monotonicity hold in the tables.
- Failures watched on purpose: the solver reports "YES" at L = value-1; non-preemptive EDF and never-switch are caught at 2 where the optimum is 1; `verify` rejects four corrupted strategy files.

**5. Interpretations chosen**

- A switch tick may be spent at any time and repeated. An idle tick between the switch and B voids it.
- A switch tick when A is finished or absent is the same as idling.
- Idling keeps A.
- Running an identical twin of A is treated as running A; the resulting position is the same.
- Releases happen only at ticks 0..T-1 (forced by r+p <= d <= T).
- The scheduler plays on after T. A play is scored as lost once some job has t + remaining > d + L.
- The promise is checked after each tick's whole release, not job by job. This is equivalent, because feasibility is monotone under removing jobs.

**6. Files**

- `game.cpp` (binary `game`): the solver. `game2`..`game8` are older builds I could not delete.
- `run_tables.sh` produces `tables.txt`, all exhaustive entries except S_8(4).
- `run_checks.sh` produces `checks.txt`: S_8(4), sanity checks, rule tests, all `strat_*.txt` witnesses and their verification.
- `verify.cpp` (binary `verify`): independent checker of strategies and feasibility.
- `showstrat.py`: strategy printer.
- `proofs.txt`: the proofs above.
- `tree_S3_T9*.txt`: human-readable P=3 trees.
