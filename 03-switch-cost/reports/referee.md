REFEREE REPORT — verdict: the solvers are right that the author's guess is false; no claim was refuted and no shared misreading of the rules was found. Two parts of C4 I could not reproduce exhaustively and they rest on the solvers' own runs (listed at the end).

All my files are in ./verifier/ .

**C1–C5**

- **C1 (S_T ≤ F_T ≤ P−1): CONFIRMED, proved by hand.** I read solver-b's Theorem 1 line by line and every step follows; solver-a's is the same argument and also follows.
  - S ≤ F: a same-cost schedule with switch ticks read as idle is a preemptive schedule, so the same-cost adversary has a subset of the free adversary's strategies.
  - The load bound uses only preemptive (free) feasibility of the final multiset, which is the weaker promise, as required.
  - Case "tick before the busy run is idle": the busy jobs were released at or after t1 and lie wholly in [t1, C), giving a contradiction.
  - Case "tick before runs a later-deadline job k": the busy jobs were released after k was chosen (release precedes the choice), so C ≤ d_j + p_k − 1. This is exactly where jobs arriving during a lock are handled.
  - The intervals are non-degenerate because j itself lies in them.
- **C2 (monotone in T and P): CONFIRMED by hand.** The same jobs, promise and payoff carry over.
- **C3 (S_4(2) ≥ 1, F_3(2) ≥ 1): CONFIRMED by hand.**
  - Solver-b's second branch (scheduler declines (0,2,4)) is sound: release (1,1,3) and (1,1,4); hindsight runs a, a, u, u′ with no switch; online has 4 units for ticks 1–3 with all deadlines ≤ 4.
  - Solver-a's wait-and-pounce tree (b1/b2) is also sound.
  - The one-line witness does cover only the first branch.
- **C4 (tables): CONFIRMED where I could reach, otherwise not checked.** My solver reproduces:
  - S_T(2) = 0,0,0,1 and F_T(2) = 0,0,1: lower bounds to T = 10; "cannot force 2" exhaustive to T = 8 (S) and T = 9 (F).
  - F_T(3) = 0,0,1,1,1,2,2 for T = 1..7.
  - S_T(3) = 0,0,0,1,1,1,1,1 for T = 1..8, exhaustive (T = 8 took 1.8M nodes).
  - S_T(4) and F_T(4) for T = 3..6 only.
  - S_9(3): my exhaustive run timed out at 1500 s. My solver restricted to one release per tick answers YES at T = 9 and NO at T = 8, which is a valid lower bound.
- **C5: CONFIRMED** as lower-bound certificates plus C1 and C2. Every certificate I ran passed my checkers:

| Certificate | Bound | Result |
|---|---|---|
| solver-b `wit_S3_T9_L1.json` (19 nodes) and `_len13` | S_9(3) ≥ 2 | accept |
| solver-a `strat_S_P3_T9.txt` (163 nodes) | S_9(3) ≥ 2 | accept |
| both solvers' F trees for P=3, T=6 | F_6(3) ≥ 2 | accept |
| both solvers' F trees for P=4, T=10 | F_10(4) ≥ 3 | accept |
| solver-a `strat_S_P4_T9.txt` | S_9(4) ≥ 2 | accept |
| solver-b `wit_S4_T15_L2.json` | S_15(4) ≥ 3 | accept |
| solver-a `strat_S_P4_T16.txt` | S_16(4) ≥ 3 | accept |

**Task 1 — rules audit: no misreading found.**
- Both `game.cpp` files have the hindsight machine start at tick 0 with no current job, allow idling, and pay a switch tick exactly as the online machine does.
- Both release before the scheduler moves in tick t, so the adversary does not see that action.
- A job released at r can run in tick r, and lateness is counted after T.
- Remaining interpretations are harmless: repeated switch ticks are allowed, and identical twins are merged.
- Solver-b's `verify.py` never tries a second consecutive switch tick in its leaf search. My checker has no such restriction and still accepts, so nothing depends on it.
- Solver-a's `bad2.txt` is byte-identical to the valid strategy (a stale file its `run_checks.sh` never uses); my checker correctly accepts it.

**Task 2 — own solver.**
- `vgame.cpp` keeps run, idle and switch as separate actions and tracks the exact set of live hindsight states.
- `feastest.py`: its feasibility matched raw enumeration (`mach.py`) on 4,969 job lists per promise with 0 mismatches. Deliberately swapping the promise produced 25 mismatches, so the test can fail.
- `naive_game.py` (full-history minimax, no state compression) agrees on P = 2 for T ≤ 4 and P = 3 for T ≤ 4.
- S_3(2) = 0 by hand:
  - A feasible set containing a 2-job has at most one other job, a unit.
  - (0,2,3) with (1,1,2) is same-cost infeasible: every placement needs a switch tick or is late. Under the free promise it is feasible, which is the F_3(2) = 1 witness.
  - In every remaining case non-preemptive EDF is on time.
- S_4(2) = 1 follows from C3 plus C1.
- Outputs: `table_P3_a.txt`, `table_P2_b.txt`, `table_misc.txt`, `run_S3_T8_L1.txt`.

**Task 3 — S_9(3) ≥ 2 certificate.**
- `check_b.py` (output in `check_S3_T9_verbose.txt`) covers every online action with idle and switch separately, exhibits a hindsight schedule for every release, and searches every leaf exhaustively.
- Main line by hand:
  - X = (0,2,9) is released and run. Y = (1,3,7) arrives; the scheduler is locked, so Y starts at tick 2. U = (3,1,5) arrives.
  - If the scheduler switches: (4,2,7) arrives, leaving 5 units from tick 4, so a deadline-7 job ends at 9 or later. Hindsight: idle, Y×3, U, C×2, X×2.
  - If it continues Y: (4,1,5) arrives and the second unit ends at 7 or later. Hindsight: idle, Y, switch, U, U′, Y×2, X×2.
  - If it idles instead of starting Y: a second (3,3,7) arrives, giving 6 units due at 7 from tick 3.
- Mutants `mut1`–`mut6` are all rejected: a loosened deadline and a deleted reply as incomplete, a tightened Y and an altered unit as promise violations, an illegal job, and the F tree relabelled as S.

**Task 7 verdicts**
- (i) F(P) ≤ P−1: PROVED BY HAND.
- (ii) S(2) = F(2) = 1: PROVED BY HAND, and confirmed by three programs.
- (iii) F(3) = 2: MACHINE-CHECKED CERTIFICATE plus hand upper bound. Checked by four programs: the two solvers' verifiers, my checkers, and my solver's exhaustive search.
- (iv) S(3) = 2: MACHINE-CHECKED CERTIFICATE plus hand upper bound. Two different trees, each accepted by its author's verifier and by mine; no hand proof.
- (v) S(4) = F(4) = 3: MACHINE-CHECKED CERTIFICATE plus hand upper bound. Three checkers; no hand proof.
- (vi) P ≥ 5: only 3 ≤ S(P) ≤ F(P) ≤ P−1 is established. S(P) = F(P) = P−1 has no data behind it at all for P ≥ 5; it is conjecture only.

**Not checked**
- S_9(3) exhaustively (certificate only).
- The exhaustive upper bounds S_T(4) = 1 for T = 7, 8 and S_9(4) ≤ 2, which rest on the solvers' dominance pruning (Lemma D); I did not read that lemma.
- F_T(4) for T = 7..10 and F_T(3) for T = 8, 9.
- My own P = 2, T = 9 same-cost "cannot force 2" run, which timed out.
- Solver-a's and solver-b's own verifiers were not re-run; I relied on my own.
