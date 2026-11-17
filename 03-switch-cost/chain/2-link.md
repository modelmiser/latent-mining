## 1. KNOWN

All from thought alone; nothing was run.

- **Upper bound (proved in outline):** non-preemptive EDF gives lateness <= P - 1 for every c. A late job is blocked by at most one later-deadline job, which started before the busy stretch and has at most P - 1 units left; the load rule bounds the rest.
- **Lower bound (believed, not fully verified):** L(c, P) = P - 1 for all c >= 1, so the question collapses. After a long job's first unit, release (t, 1, t+1). Refusing costs P - 1. Preempting leaves the scheduler c ticks behind for good, because the adversary's benchmark switches free and it can keep load density 1. Repeating with fresh long jobs pushes backlog past P - 1.
- **Finiteness:** the game as given has no built-in bound on windows.
- **Literature:** unsure of any named source for the exact statement.

## 2. SUCCESSOR QUESTION

**The one change:** the adversary's promise is now feasibility for an offline scheduler that pays the same switch cost, not a free-switching one. This removes the permanent-backlog argument, since the benchmark must also pay. A horizon T is added only to make instances finite.

Fix integers P >= 1, c >= 1, T >= 1. Time is ticks 0, 1, 2, ....

- **Jobs.** A job is (r, p, d) with integers 1 <= p <= P and r + p <= d <= T. It is revealed at the start of tick r and may run in tick r.
- **Machine.** In each tick it runs one unit of one job, idles, or spends a switch tick. Let A be the job most recently run. Running B != A while A is unfinished requires the c ticks immediately before B's unit to be switch ticks. If A is finished, or nothing has run, starting or resuming any job is free. Idling does not change A.
- **Adversary.** It is adaptive, sees the schedule so far, and may release any finite multiset of jobs per tick. Rule: after every release, the set of all jobs released so far must be c-feasible. That means some schedule of this same machine, built with full hindsight from tick 0 and unrelated to the online schedule, completes every job by its deadline.
- **Value.** Lateness is max(0, completion - d), where completion is the tick index after the last unit. The scheduler keeps working after T. M_T(c, P) is the least L an online scheduler can guarantee for every job. M(c, P) = lim over T of M_T(c, P).

The limit exists: M_T is nondecreasing in T and at most P - 1, because c-feasibility implies the old load rule. Also M >= min(c, P - 1), by the tight unit job released after a long job's first unit.

**Questions.**
1. Compute M_T(1, P) for P = 2, 3, 4 and T up to stabilisation.
2. Is M(1, 3) equal to 1 or 2? This is the smallest case where the two bounds differ.
3. Is M(c, P) = min(c, P - 1) for all c and P? If not, give the formula. In particular, is M(1, P) bounded as P grows?
4. Does an optimal scheduler ever idle with work pending, to deny the adversary a feasible trap?

## 3. WHY IT IS PROBABLY UNCHARTED

- Online scheduling with preemption penalties is normally compared against a free-switching or non-preemptive optimum, or uses throughput or flow objectives.
- I know of no result on exact worst-case lateness against a same-cost clairvoyant promise with adaptive releases.
- The promise is not an interval-load condition, so standard demand-bound arguments do not transfer.

Doubts:
- It may still collapse to min(c, P - 1) by a short exchange argument.
- It may collapse to P - 1 by a nesting trap I did not find.
- Stabilisation in T may be slow.

## 4. HOW TO CHECK AN ANSWER

- Run memoised minimax over states (tick, pending jobs with remaining lengths, last-run job, trailing switch count, released set).
- Filter adversary moves with an exact c-feasibility search; this is small because total released length is at most T.
- Decide "lateness <= L achievable?" for each L, and tabulate M_T(1, P) for P <= 4 and increasing T until constant.
- Cross-check the feasibility search against brute-force enumeration on tiny T.
- A formula needs both an explicit adversary strategy tree for the lower bound and a scheduler rule with an invariant proof for the upper bound, each matching the table.
