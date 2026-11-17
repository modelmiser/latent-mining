REPORT (open-s): S(P) >= P-1 for every P >= 2 under the same-cost promise

I have a hand proof for all P. It is unrefereed; exhaustive play confirms it for P = 2..5. Full argument: ./open-s/proofs.txt; raw runs: results.txt in the same folder.

ADVERSARY RULE ADV(P)
Constants: Delta = (P-1)(3P-1), T = Delta + P(P-1) = (P-1)(4P-1). Rounds rho = 0..P-2. The adversary keeps a reference hindsight schedule H that never wastes a tick and never switches.

Round rho, starting at time s:
- Release a decoy K = (s, P, Delta+(rho+1)P), the latest deadline so far.
- Keep two continuations of H: PLAN1 runs K at once (ticks s..s+P-1), then unit jobs; PLAN0 runs one unit job per tick and never K.
- Phase 1 (ticks s..s+P-1): whenever PLAN0 has no unit to run, release a stream unit (t, 1, Delta+rho*P).
  - Scheduler leaves a tick empty: wait until s+P, set H := PLAN1, next round.
  - Scheduler runs K: keep the stream going. If it runs K to completion with no empty tick, cash in with PLAN0. At its first empty tick, set H := PLAN1, next round.
- Phase 2 (from s+P, scheduler ran only non-K units so far): no stream.
  - Scheduler leaves a tick empty: H := PLAN1, next round.
  - PLAN1 runs out of work: cash in with PLAN1.
  - Scheduler starts K at tick x (fresh lock, P-1 units left): release a tight unit (x+1, 1, x+2), plus further tight units only if PLAN1 would otherwise idle. Finishing K without an empty tick makes that unit P-1 late. An empty tick sets H := PLAN1, next round.
- After P-1 rounds, or at a cash-in: release filler with deadline D* (the pack deadline) so the plan is exactly full up to D*.

Why it works: the scheduler's lag behind a waste-free plan grows by at least 1 per round. The two plans differ in order (decoy first vs unit work first), not in preemption, so neither ever pays a switch tick.

CLAIMS
1. COMPLETE: Round Lemma. From "scheduler is w units of work behind H" a round ends in an adversary win or in "w+1 behind". Per tick: a non-K unit changes neither lag; a K unit adds 1 to the lag behind PLAN0; an empty tick adds 1 to both.
2. COMPLETE: ADV(P) forces lateness >= P-1 against every scheduler (P-1 rounds, then a load bound: work due by D* exceeds the time left by the lag).
3. COMPLETE: every release keeps the same-cost promise. Both plans are explicit switch-free schedules; every plan clears its pending units by (rho+1)(3P-1) <= Delta.
4. COMPLETE given 1–3 and the brief's upper bound (not re-derived here): S(P) = F(P) = P-1 for all P >= 2.
5. COMPUTED: exhaustive play, every scheduler, on the brief's literal machine (run/idle/switch):

| P | T (proven Delta) | nodes | result |
|---|---|---|---|
| 2 | 7 | 21 | adversary wins |
| 3 | 22 | 1,064 | adversary wins |
| 4 | 45 | 115,138 | adversary wins |
| 5 | 76 | 20,830,663 | adversary wins |

   - Two implementations (play.py, play.cpp) give identical node counts for P = 2..4; for P = 5 the C++ count is matched by a Python run (64-bit-hash memo) at Delta = 60.
   - P = 2, 3 were also run without memo, with a brute-force same-cost feasibility search of the raw job list at every release and an exact lateness search at every terminal. No disagreement.
   - The brute-force search matched a naive enumerator on 4000 random tiny job lists.
6. COMPUTED: the smallest Delta at which this rule's witness plans pass is 5, 12, 21, 32 for P = 2..5, i.e. (P-1)(P+3), so T = (P-1)(2P+3) = 7, 18, 33, 52. Delta-1 fails each time.
7. NOT PROVED: that formula for general P, and anything about the minimal horizon.

HORIZON
- Proved: T(P) = (P-1)(4P-1), about 4P².
- Computed for P <= 5: (P-1)(2P+3), about 2P².
- So the order is P², but this rule is not horizon-optimal (the solvers' exact values are 4, 9, <= 15). It does not settle whether the first horizon is exactly P².

P >= 6 (not exhaustive)
P = 6 exhaustive was not run: extrapolated above 10^9 nodes, beyond the 8 GB / 1500 s limits. Instead:
- Every scheduler deviating from non-preemptive EDF in at most 9 ticks (P=6), 7 (P=7), 6 (P=8): all lose. The P=8 run only exercises early rounds, since six deviations cannot survive the forks.
- 20,000 sampled random/structured schedulers each for P = 5..8: all lose.

NEGATIVE CONTROLS (all detected)
- Fork without the tight unit: scheduler survives by running the decoy through.
- Fork unit with a slack deadline: scheduler survives.
- P-2 rounds instead of P-1: scheduler survives with lateness P-2.
- Delta too small: the promise check objects.
- Sampling is weak for the promise: at P=5, Delta=31 passes 20,000 samples but fails exhaustively.

NOT CHECKED
- Nobody else has read the proof.
- The P = 5 memo uses a 128-bit hash, not exact keys.
- With memo on, the promise is checked through the rule's own witness plans (finish the locked job, then non-preemptive EDF); the raw-job-list brute force ran only for P = 2, 3.
- No full P >= 6 run.
- I did not compare against the solvers' trees beyond reading them for ideas.

The rule finally used is my own construction, not extracted from the solver trees.

Files are in ./open-s/:
- proofs.txt
- results.txt
- run_all.sh
- play.py
- play.cpp
- sample.py
- test_brute.py
