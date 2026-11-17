REPORT — solver B. Headline: the conjecture "S(P) = 1 for every P ≥ 2" is FALSE. S(3) = F(3) = 2 and S(4) = F(4) = 3.

All files are in ./solver-b/.

## 1. Tables

Every entry is COMPUTED by `game.cpp` (memoised minimax, one yes/no game per lateness threshold). "Exhaustive" means the full adversary: every feasible release multiset at every tick.

| | value by T | how far exhaustive |
|---|---|---|
| S_T(2) | 0 for T ≤ 3; 1 for T ≥ 4 (final) | T ≤ 9 |
| F_T(2) | 0 for T ≤ 2; 1 for T ≥ 3 (final) | T ≤ 11 |
| S_T(3) | 0 for T ≤ 3; 1 for 4 ≤ T ≤ 8; 2 for T ≥ 9 (final) | T ≤ 8; T = 9 by explicit adversary tree |
| F_T(3) | 0 for T ≤ 2; 1 for 3–5; 2 for T ≥ 6 (final) | T ≤ 9 |
| S_T(4) | 0 for T ≤ 3; 1 for 4–8; 2 at T = 9; 2 or 3 for 10–14; 3 for T ≥ 15 (final) | lateness-2 threshold T ≤ 8, lateness-3 threshold T ≤ 9; T = 9 and T = 15 by explicit trees |
| F_T(4) | 0 for T ≤ 2; 1 for 3–5; 2 for 6–9; 3 for T ≥ 10 (final) | T ≤ 10 |

- **"Final"** rests on the proved upper bound P−1 plus monotonicity in T (both in `proofs.txt`).
- **S_T(4) for 10 ≤ T ≤ 14 is undetermined.** The exhaustive run ran out of memory at T = 10. Restricted adversaries (one release per tick, lengths {1,4}) found no win at T ≤ 14, which proves nothing.
- **The T = 15 win for S(4)** came from a restricted adversary (one release per tick, lengths {1,4}, slack ≤ 4 or d = T). A restricted win is still a valid lower bound.
- **Pattern, guessed:** the F thresholds 3, 6, 10 are triangular numbers; the S thresholds are 4, 9, ≤ 15.

## 2. Witnesses

- **S ≥ 1 (P ≥ 2, T ≥ 4), by hand.** Release (0,2,4).
  - If online runs it at tick 0: release (1,1,2). The promise holds via idle, u, a, a. Online is locked, so u is late.
  - If online does not run it: release (1,1,3) and (1,1,4). The promise holds via a, a, u, u′. Online has 4 units for 3 ticks.
  - The brief's one-line witness covers only the first branch.
- **F ≥ 1 (T ≥ 3), by hand.** Release (0,2,3), then (1,1,2) whatever online does.
- **S_9(3) ≥ 2.** A 19-node tree is in `wit_S3_T9_L1.txt`. The main line:
  - Release decoy (0,2,9); online runs it.
  - Release (1,3,7); online is delayed one tick by the lock and starts it at tick 2.
  - Release (3,1,5) while online is locked with 2 units left.
  - If online switches: release (4,2,7). Hindsight runs B at 1–3, u at 4, C at 5–6, decoy at 7–8 with no switch; online is 2 late.
  - If online continues: release (4,1,5). Hindsight had instead run B at 1, switched at 2, u at 3, u′ at 4, B at 5–6; online is 2 late.
  - The adversary keeps two incompatible hindsight plans alive; the decoy's deadline is T.
- **S_15(4) ≥ 3.** `wit_S4_T15_L2.json`, about 70k lines, not human-sized.
- **F trees:** `wit_F3_T6_L1.txt` (13 nodes), `wit_F4_T10_L2.txt` (46 nodes).
- **Independent check.** Every tree above passed `verify.py`, which shares no code with the solver and uses the unsimplified machine (idle, switch and the flag kept separate). It re-derives the promise for each release by exhaustive hindsight search and checks each leaf by exhaustive search. Three mutated trees were rejected: two as incomplete, one as a promise violation.
- **"Value is enough."** Non-preemptive EDF gives P−1 (proved). I have no worded rule for the smaller finite-T values, only the solver's tables.
- **Idling.** A scheduler forbidden from taking an empty tick while unlocked with work pending still achieves S_T(3) ≤ 1 at T ≤ 8, and S_T(2) = 1 exactly at T ≤ 8. So idling with work pending is not needed at those sizes.
- **Simple rule.** In the limit, non-preemptive EDF (never switch, never idle with work pending) is optimal for both S and F at P ≤ 4. At small T it is not: against any never-switch scheduler the adversary forces 2 at P = 3, T = 5, where the true value is 1. I did not test the brief's "switch exactly when a new job cannot otherwise meet its deadline" rule.

## 3. Headline

- S(2) = F(2) = 1, S(3) = F(3) = 2, S(4) = F(4) = 3. Confidence is high: two independent programs agree on the lower bounds, and the upper bound is a hand proof.
- So the switch cost hurts the online scheduler by the full P−1 even when the benchmark pays it too, at least for P ≤ 4.
- **Conjecture:** S(P) = F(P) = P−1 for all P ≥ 2. This is GUESSED for P ≥ 5, at roughly 75%. I have no general adversary, not even for F; a P = 5 attempt ran out of reach.

## 4. Proofs (in `proofs.txt`)

- **COMPLETE:** Lemma M — idle is never better than a switch tick, so the state reduces to jobs plus a lock.
- **COMPLETE:** Lemma R — sub-multisets of a same-cost-feasible set are feasible. This justifies one-at-a-time releases and discarding dead hindsight states.
- **COMPLETE:** Lemma D — the dominance pruning of hindsight states is safe. A run with it disabled gives the same answers.
- **COMPLETE:** Theorem 1 — S_T ≤ F_T ≤ P−1 for all T, via non-preemptive EDF and an interval-load argument.
- **COMPLETE:** Theorem 2 — monotone in T and P.
- **COMPLETE:** Theorems 3 and 3′ — S ≥ 1 and F ≥ 1.
- **COMPUTED only, no hand proof:** S(3) ≥ 2 and S(4) ≥ 3.
- **No sketches offered.**

## 5. Interpretations

- A switch tick may be taken at any time, and consecutive switch ticks keep the "may run anything" right. Idle after a switch loses it.
- "Promise after every release" is checked once per tick after the whole multiset; this is equivalent by Lemma R.
- The last release tick is T−1; online deadlines are unbounded after T.
- Lateness is measured against the true deadline d.

## 6. Sanity checks

- P = 1 gives 0 in both games.
- Online switch cost 0 gives 0.
- S ≤ F and monotonicity hold throughout the table.
- `brute.py` (Python, unsimplified machine, direct minimax of lateness) agrees with `game.cpp` on the seven instances it finished (P ≤ 3, T ≤ 4); T = 5 timed out.
- Non-preemptive EDF never exceeds P−1 at the sizes tried.
- The solver reports value−1 as losing for the scheduler at every threshold.

## 7. Files

- `game.cpp` (binary `game`) — all table numbers.
- `run_all.sh` → `results.txt` — reproduces every table entry, about 10 minutes.
- `table_run_S4_L2_full.txt` — the exhaustive S_T(4) lateness-3 run to T = 9, and the out-of-memory at T = 10.
- `brute.py`, `verify.py` — independent checks.
- `wit_*.txt`, `wit_*.json` — adversary trees; `mutant*.json` — the deliberately broken ones.
- `proofs.txt`.
- `game2`, `game3` are intermediate builds. `wit_S4_T15_L2` was exported with `FIRSTWIN=1 SLACKMAX=4 LENS=14 ./game2 4 15 2 S 1 1 15 wit_S4_T15_L2`.
- `witness_S3_T9.txt` is an early non-minimal dump; ignore it.
