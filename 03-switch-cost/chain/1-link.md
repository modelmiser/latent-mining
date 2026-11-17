## KNOWN

With free switching, EDF is optimal on one processor (Dertouzos), so the adversary's promise is exactly the interval-load condition. Speed augmentation for online deadline scheduling goes back to Phillips–Stein–Torng–Wein, and preemption overhead is studied in real-time systems ("limited preemption" schedulability analysis). I am unsure of any paper on this exact game.

I believe the question as given is degenerate: for c ≥ 1 no finite speed suffices. While the scheduler is mid-job on a long job, the adversary releases a length-1 job with a window of one tick. Serving it needs c ≥ 1 idle ticks first, and unbounded lengths prevent the speed from finishing every job inside one tick. So s(c) = ∞ for c ≥ 1 and s(0) = 1. Deadlines must be softened to get a real question.

## SUCCESSOR QUESTION

Fix integers P ≥ 1 (maximum job length) and c ≥ 1 (switch cost). Time is ticks 0, 1, 2, ….

- **Jobs.** A job is (r, p, d) with integers 1 ≤ p ≤ P and d ≥ r + p. It is revealed at the start of tick r and may run in tick r.
- **Scheduler.** In each tick the processor runs one unit of one job, or idles, or spends a switch tick. Let A be the job most recently run. Running a job B ≠ A while A is unfinished requires that the c ticks immediately before B's unit were switch ticks. If A is finished, or nothing has run yet, starting or resuming any job is free. Idling does not change A.
- **Adversary.** It is adaptive and sees the schedule so far. It may release any finite number of jobs per tick, subject to one rule: at all times, for all integers a < b, the total length of released jobs with a ≤ r and d ≤ b is at most b − a. That is, a free-switching scheduler could meet every deadline.
- **Value.** A job's lateness is max(0, completion time − d), where completion time is the tick index after its last unit. L(c, P) is the least integer L such that some online scheduler guarantees lateness ≤ L for every job against every adversary.

**Questions.** Determine the sequence a(P) = L(1, P) for P = 1, 2, 3, …, and then L(c, P) in general.

- Is a(P) = P − 1 always, meaning never preempting is optimal even when a switch costs a single tick?
- Or is a(P) = o(P), for example Θ(√P), from balancing refused preemptions against accumulated switch ticks?
- For which (c, P) does L(c, P) first equal its never-preempt value?
- What rule (remaining work of the running job versus laxity of the newcomer) tells the optimal scheduler to refuse?

## WHY IT IS PROBABLY UNCHARTED

Real-time literature analyses fixed policies with preemption overhead for schedulability. It does not treat the exact minimax lateness of an adaptive game with a length cap. Speed-augmentation results do not transfer, because the resource here is additive tardiness.

Doubts:
- I believe non-preemptive EDF gives lateness ≤ P − 1 (folklore blocking bound), so L ≤ P − 1.
- The answer may collapse to exactly P − 1 for all c ≥ 1. That would be a clean but dull theorem.
- A nested-preemption adversary may make a(P) depend on unbounded window lengths in a way I have not foreseen.

## HOW TO CHECK AN ANSWER

- **Computation.** For small P (say ≤ 6) and c ≤ 3, cap windows at d − r ≤ W. Encode a state as (last-run job, switch progress, multiset of (remaining work, d − t)). Solve the safety game "lateness ≤ L" by backward fixpoint. Report the least winning L and raise W until the value stabilises.
- **Sanity checks.** a(1) = 0 and L ≤ P − 1.
- **Proof.** A general claim needs an explicit adversary strategy (lower bound) and a scheduler with a potential-function argument (upper bound). Both can be replayed against the solved small cases.
