# Experiment 3: the price of changing your mind

One machine, jobs arriving online with deadlines, and a one-tick penalty for
leaving a job unfinished. How much does that penalty cost an online scheduler,
and does it matter whether the benchmark it is compared with pays it too?

**Short answer.** For every maximum job length P ≥ 2 and every long enough
horizon, the best worst-case lateness an online scheduler can guarantee is
exactly P − 1, which is what it
gets by never preempting at all. This holds even when the adversary must keep
the jobs feasible for a clairvoyant scheduler that pays the same penalty. The
chain that posed the question guessed the opposite.

## The game

Parameters: P ≥ 1 (maximum job length) and T ≥ 1 (horizon). Time is ticks
0, 1, 2, ….

- **Jobs.** A job is (r, p, d) with 1 ≤ p ≤ P and r + p ≤ d ≤ T. It is revealed
  at the start of tick r and may run in tick r. Identical jobs may repeat.
- **Machine.** In each tick it runs one unit of one job, idles, or spends a
  switch tick. Let A be the job most recently run. Running a job B ≠ A while A
  is unfinished requires the tick immediately before B's unit to be a switch
  tick. If A is finished, or nothing has run yet, any job may start or resume
  at once. Idling leaves A unchanged. A switch tick is not aimed at a job.
- **Order of play.** At tick t the adversary, having seen ticks 0 … t−1 of the
  online schedule, releases any finite multiset of jobs with r = t. Then the
  online scheduler chooses what the machine does in tick t.
- **Promise.** After every release, all jobs released so far must be feasible
  for some hindsight schedule built from tick 0 that meets every deadline.
  - *Same-cost promise:* the hindsight schedule runs on the machine above.
  - *Free promise:* the hindsight schedule switches at no cost (ordinary
    preemptive feasibility).
- **Value.** The online scheduler works on after T until every job is done.
  Lateness of a job is max(0, completion − d). The scheduler minimises the
  largest lateness and the adversary maximises it. S_T(P) is the value under
  the same-cost promise, F_T(P) under the free promise, and S(P), F(P) are the
  limits as T grows.

The question as the chain left it ([chain/3-link.md](chain/3-link.md)): is
S(P) = 1 for every P while F(P) = P − 1? That is, does a switch cost hurt an
online scheduler only by the amount the benchmark is excused from paying it?

## Results

| Claim | Status | Where |
|---|---|---|
| S_T(P) ≤ F_T(P) ≤ P − 1 for all T (never preempt, earliest deadline first) | proved by hand by both solvers; referee read it line by line. The prior-art search calls it an easy corollary of a textbook lemma | `solver-a/proofs.txt` Theorem 1, `solver-b/proofs.txt` Theorem 1 |
| S_T and F_T are nondecreasing in T and in P | proved by hand; referee read it | same files |
| Exact tables for P = 2, 3, 4 (below) | computed; the referee's own solver reproduces P = 2, P = 3 to T = 8 (S) and T = 7 (F), and P = 4 for T = 3 … 6 only | `solver-a/tables.txt`, `solver-b/results.txt`, `verifier/` |
| S(2) = F(2) = 1 | proved by hand; three programs agree | `solver-b/proofs.txt` Theorems 3, 3′ |
| S(3) = F(3) = 2, S(4) = F(4) = 3 | lower bounds are machine-found adversary trees, each accepted by its author's checker and by the referee's independent checker; six deliberately broken trees rejected | `solver-a/strat_*.txt`, `solver-b/wit_*`, `verifier/` |
| **F_T(P) ≥ P − 1 for every P ≥ 2 and T ≥ P(P+1)/2** ("staircase" adversary) | proved by hand by one follow-up instance; I read it line by line; the rule was played against every scheduler for P = 2 … 6 | `open-f/proofs.txt` Theorem A |
| F_T(P) ≥ min(P − 1, k) whenever T ≥ (k+1)(k+2)/2 | follows from the row above and monotonicity; agrees with every free-promise table entry | `open-f/proofs.txt` Corollary A′ |
| **S_T(P) ≥ P − 1 for every P ≥ 2 and T ≥ (P−1)(4P−1)** | proved by hand by a second follow-up instance; I read it line by line; the rule was played against every scheduler for P = 2 … 5 | `open-s/proofs.txt` |
| **S(P) = F(P) = P − 1 for every P ≥ 2** | follows from the upper bound and the row above | |
| Smallest horizon at which F reaches P − 1 is P(P+1)/2 | exact for P = 2, 3, 4 by the tables; not proved in general | |
| Smallest horizon at which S reaches P − 1 | 4 and 9 for P = 2, 3; between 10 and 15 for P = 4; open in general. About 4P² is proved sufficient; this rule's own witness plans pass from about 2P² for P ≤ 5 (computed). Neither is a lower bound on the horizon | |

Exact values (blank = not computed):

| T | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|---|
| F_T(2) | 0 | 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
| S_T(2) | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 1 | 1 | |
| F_T(3) | 0 | 0 | 1 | 1 | 1 | 2 | 2 | 2 | 2 | |
| S_T(3) | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 1 | 2 | |
| F_T(4) | 0 | 0 | 1 | 1 | 1 | 2 | 2 | 2 | 2 | 3 |
| S_T(4) | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 1 | 2 | |

S_9(3) = 2 is not from an exhaustive run: the lower bound is a checked
adversary tree and the upper bound is the hand proof. S_9(4) = 2 rests on
solver B alone (`solver-b/table_run_S4_L2_full.txt`, a run that uses its
pruning lemma); solver A has only 2 ≤ S_9(4) ≤ 3. S_T(4) is 2 or 3 for
T = 10 … 14 and 3 from T = 15; the tree for T = 15 is too large to ship and
was accepted by the referee's checker (`reports/referee.md`).

## What this does and does not say

It says that over a long enough horizon no online rule, however clever about
when to preempt, guarantees a smaller worst-case lateness than the rule that
never preempts. Paying the benchmark's switch cost too does not weaken the
adversary; it only makes the adversary need more time (4, 9, at most 15 ticks
for P = 2, 3, 4 against 3, 6, 10).

It does not say preemption is useless. At short horizons it helps: with
P = 3 and T = 5 the value is 1, and every scheduler that never switches can be
driven to 2 (`solver-a/checks.txt`; solver B reports the same). And the statement
is about the worst case only, with switch cost exactly 1.

## How the adversaries work

**Free promise (staircase).** Release jobs of length 2, 3, …, P one at a time,
each just after the scheduler runs the first unit of the previous one, and each
with a window that excludes the previous job and is exactly filled by the jobs
still to come. A scheduler locked on a job either finishes it, losing that many
ticks in the new window, or leaves it and pays one empty tick in every window
opened so far. A tight unit job at the end makes finishing the last job cost
P − 1 outright. Counting back from the last level, every level must have paid
an empty tick, and P − 1 empty ticks in the outermost, exactly full window is
lateness P − 1. The hindsight schedule preempts freely, so this adversary does
not work under the same-cost promise.

**Same-cost promise (decoys).** The adversary keeps a hindsight schedule that
never wastes a tick and never switches. Each round it releases a slack decoy of
length P and holds two continuations: one that ran the decoy at once, one that
did earlier-deadline unit work first. It feeds exactly enough unit work to keep
the second one busy. A scheduler that touches the decoy early falls behind the
second plan; one that idles falls behind both; one that does the unit work
first must start the decoy after the first plan has finished it, and is then
hit by a one-tick job while locked on a fresh long job. Every round costs the
scheduler one wasted tick or the game. After P − 1 rounds the adversary fills
its own schedule exactly up to the last deadline, and the scheduler's P − 1
wasted ticks become lateness. The two plans differ in order, not in preemption,
which is why the same-cost promise costs the adversary nothing.

## What to distrust

- **Everything is by one model family.** "Proved by hand" means one instance
  wrote it and another read it. Nothing here is machine-checked as a proof.
- **The two general lower bounds are the newest and least tested part.** Each
  was written by one instance. I read both and found no gap, and each rule
  wins exhaustive play at small P, but exhaustive play at P ≤ 5 or 6 cannot
  confirm an argument for all P. For the same-cost rule, the promise was
  checked by brute force on the raw job list only for P = 2, 3; for P = 4, 5
  it was checked through the rule's own witness plans, and the P = 5 run
  memoises on a 128-bit hash.
- **Two P = 4 table entries rest on the solvers' pruning lemma** (Lemma D),
  which the referee did not read: S_8(4) = 1 and S_9(4) ≤ 2.
- **The upper bound's place in the literature** is from a search that read two
  sources in full ([prior-art.md](prior-art.md)). I have not read the cited
  lemma. The search found the lower bounds nowhere, which is weak evidence
  that they are new.
- **Switch cost is fixed at 1.** Nothing here covers larger costs, where the
  earlier links expected min(c, P − 1) under the same-cost promise; with the
  result above that guess is in doubt but untested.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files. All five confirmed
the upper bound, the hand proofs for P = 2, the staircase proof and its
corollary. Four confirmed the same-cost proof outright; the fifth marked it
refuted on one point, which is real and is repaired by a one-line change:

- As written, the final "cash-in" step of the same-cost adversary releases
  "one job of length f mod P" even when that is zero, which is not a legal
  job. The job must be omitted when f mod P = 0. The program already does so
  and the count of filler units is unchanged, so the argument stands.

Other findings, applied here or noted at the top of the proof files: the
headline lacked "for long enough horizons"; two table cells that had been
computed were blank; S_9(4) = 2 was shown without saying it rests on one
solver; "I re-derived it" claimed more than a reading; the same-cost proof
file says two programs' node counts agree for P = 3 when the recorded runs
used different modes (3,850 against 1,064); and the referee's report, which
calls the general result a conjecture, was written before the two proofs.

The corrected wording has not been through a second round.

## Files

- `chain/`: the seed and the three links, verbatim.
- `brief-*.txt`, `briefs-chain-and-prior-art.txt`: what each instance was told.
- `reports/`: each instance's final report, verbatim. The solver and referee
  reports predate the two general proofs and call the general result a
  conjecture.
- `solver-a/`, `solver-b/`: exact solvers, tables, adversary trees, proofs.
- `verifier/`: the referee's own solver and tree checkers.
- `open-f/`: the staircase adversary, its proof and exhaustive play.
- `open-s/`: the decoy adversary, its proof and exhaustive play.
- `prior-art.md`: the literature search.

Binaries and three files over 300 kB are left out, among them solver B's
adversary tree for S_15(4) ≥ 3; `reports/solver-b.md` gives the command that
regenerates it.
