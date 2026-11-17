REPORT (open-f): F(P) = P-1 for every P >= 2, with horizon T = P(P+1)/2

Result: an explicit adversary under the free promise forces lateness P-1 against every scheduler at T = P(P+1)/2, with a hand proof for all P. Full argument: ./open-f/proofs.txt

THE RULE ("staircase"; P jobs, one per release, total work exactly T = P(P+1)/2)
- Jobs J_1..J_{P-1} with lengths 2, 3, ..., P, plus one unit job u.
- Window lengths: L_j = sum_{i=j+1}^{P-1}(i+1) + 1, so L_0 = T and L_{P-1} = 1.
- Tick 0: release J_1 = (0, 2, T). Set s_0 = 0.
- For j = 1..P-1, let a_j be the tick of the scheduler's first unit of J_j.
  s_j = a_j + 1 if a_j <= s_{j-1} + j; otherwise s_j = s_{j-1} + j + 1 (the scheduler stalled j+1 ticks without starting J_j).
- At tick s_j: release J_{j+1} = (s_j, j+2, s_j + L_j) for j <= P-2, and u = (s_j, 1, s_j + 1) for j = P-1.
- So each time the scheduler commits to a job (and is locked with j units left), a new nested window [s_j, s_j + L_j] opens that excludes that job and is exactly filled by the later jobs.

CLAIMS
1. Promise and legality at every release — COMPLETE. Windows are nested because s_j - s_{j-1} <= j+1. The hindsight schedule runs J_{i+1} in the ticks of window i outside window i+1, which is exactly i+2 ticks.
2. Every scheduler has a job at least P-1 late, for all P >= 2 and T >= P(P+1)/2 — COMPLETE.
   - Saturation: window i's jobs fill it exactly, so lateness >= delta_i, the number of ticks in the window (before its last job completes) not spent on its jobs.
   - Lock: after the first unit of J_j the scheduler either finishes it (j ticks lost in window j) or hits an empty tick (lost in every window 0..j).
   - Induction from the last level outward: finishing J_{P-1} makes u P-1 late; finishing at level j gives delta_j >= j + (P-1-j); a stall at level j gives delta_{j-1} >= P. So every level pays an empty tick, and delta_0 >= P-1.
   - This is not the suggested "adversary for P-1 inside a lock" induction; it is one chain of nested windows with induction over the levels of a single play.
3. F_T(P) = P-1 for T >= P(P+1)/2, hence F(P) = P-1 — COMPLETE given the upper bound F_T <= P-1. I re-read solver A's proof of that bound and found no gap, but did not re-derive it.
4. Corollary: F_T(P) >= min(P-1, k) whenever T >= (k+1)(k+2)/2 — COMPLETE. It matches every free-promise entry in solver-a/tables.txt.
5. Second, independent adversary at T = P^2 - 1 — COMPLETE. Release P-1 jobs (0, P, T) and a tight unit job after each one's first unit; each long job costs a distinct empty tick or leaves a unit job P-1 late.
6. P(P+1)/2 is the smallest horizon — NOT PROVED for general P. It is exact for P = 2, 3, 4 only if the earlier solvers' tables are right (F_2(2)=0, F_5(3)=1, F_9(4)=2); I did not re-run them. A general proof needs a scheduler strategy, which I do not have.

COMPUTED (exhaustive play of the fixed rule against every scheduler)
- Staircase forces exactly P-1:

| P | T | forced lateness |
|---|---|---|
| 2 | 3 | 1 |
| 3 | 6 | 2 |
| 4 | 10 | 3 |
| 5 | 15 | 4 |
| 6 | 21 | 5 |

  P = 6 explored 1,095,815 states. P = 7 (T = 28) did not finish in the 1500 s limit, so it is not computed.
- Simple rule forces P-1 for P = 2, 3, 4 (T = 3, 8, 15) and P = 5 (T = 24); P = 5 used a symmetry reduction on identical unlocked jobs. P = 6 did not finish in 600 s.
- The harness uses the unsimplified machine (run / idle / switch), checks legality of every job, and re-tests the promise after every single release by EDF simulation of all jobs from tick 0. The EDF test agreed with an independent brute force on 20,000 random instances (14,000 feasible, 6,000 infeasible).

NEGATIVE CONTROLS (all reported as failures by the harness)
- One job only: value 0.
- Two tight unit jobs at tick 0: promise violation raised.
- Staircase without the stall rule: illegal job (deadline > T) raised when the scheduler refuses to start a job.
- Staircase without u: value 0, 1, 2, 3 for P = 3..6.
- Staircase with last job of length P-1: value P-2 for P = 3..6.
- Staircase with every window one tick longer than its work (horizon T+1 with slack): value P-2 for P = 3..6.
- Simple rule with P-2 long jobs: value P-2 for P = 3, 4, 5.
- One control ("short") was mis-built: its stall timing does not match its job lengths, so it only raises an illegal-job error and shows nothing more.

NOT CHECKED
- The hand proof was written and checked by me alone; no second reader, no mechanisation. The exhaustive runs for P <= 6 are the only independent test of it.
- Staircase at P >= 7 and simple rule at P >= 6 by computation.
- Minimality of the horizon beyond what the earlier tables say.
- The harness has no independent second implementation. Its promise test was cross-checked against brute force; its scheduler move generator was not, beyond the controls.
- The same-cost promise (out of scope). Note the staircase hindsight schedule preempts freely, so it does not transfer as is.

Files are in ./open-f/:
- proofs.txt
- harness.py
- rules.py
- control_loose.py
- stair.log
- controls.log
- simple_nosym.log
- simple_sym.log
