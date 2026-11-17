## 1. KNOWN

Nothing here was computed; these are hand checks only.

- **Model:** I found no degeneracy or loophole, so no repair is needed. Idling instead of switching gains nothing, because the interrupted job stays "most recent".
- **Upper bound P − 1:** holds. A c-feasible set is also free-preemption feasible, and the classical blocking argument for non-idling non-preemptive EDF applies (standard; exact citation unsure).
- **Lower bound min(c, P − 1):** holds. The unit trap survives because hindsight either runs the long job before the trap or idles through its first tick. Deliberate idling before starting is punished the same way.
- **Flaw in the framing:** "permanently c ticks behind" is false under the same-cost promise. After the trap, online and hindsight have done equal work (online's early unit pays for its switch). Under the free promise the deficit is real: for c = 1, P = 3 I found a line that appears to force lateness 2, using a slack 3-job the benchmark preempts for free.

## 2. FINAL QUESTION

Fix c = 1. Parameters are P ≥ 2 and horizon T. Time is ticks 0, 1, 2, ….

- **Jobs.** A job is (r, p, d) with integers 1 ≤ p ≤ P and r + p ≤ d ≤ T. It is revealed at the start of tick r and may run in tick r.
- **Machine.** In each tick it runs one unit of one job, idles, or spends a switch tick. Let A be the job most recently run. Running B ≠ A while A is unfinished requires the tick immediately before B's unit to be a switch tick. If A is finished, or nothing has run, any job starts or resumes free. Idling leaves A unchanged.
- **Adversary.** It is adaptive, sees the schedule so far, and releases any finite multiset of jobs at each tick.
- **Promise.** After every release, the set of all jobs released so far must be feasible for a hindsight schedule built from tick 0 that respects release times and meets every deadline. There are two versions:
  - Same-cost promise: the hindsight schedule runs on this same machine.
  - Free promise: the hindsight schedule runs on the machine with switch cost 0, i.e. ordinary preemptive feasibility.
- **Value.** Lateness of a job is max(0, completion − d), where completion is the index after its last unit. The online scheduler always pays switch cost 1 and keeps working after T. S_T(P) and F_T(P) are the least maximum lateness it can guarantee under the same-cost and free promises. S(P) and F(P) are the limits in T; both sequences are nondecreasing and at most P − 1.

Known: 1 ≤ S(P) ≤ F(P) ≤ P − 1, and S(2) = F(2) = 1.

**Headline.** Find S(P) and F(P) exactly as functions of P. In particular: is S(P) = 1 for every P while F(P) = P − 1? In words: does a switch cost hurt an online scheduler only by the amount the benchmark is excused from paying it?

The smallest decisive case is whether S(3) = 1 and F(3) = 2.

**Supporting sub-question.** Is some non-idling rule optimal for S (for example: switch exactly when the newly released job cannot otherwise meet its deadline)?

**Guess:** S(3) = 1 < F(3) = 2, and S(P) stays bounded. Under the same-cost promise every trap costs hindsight a tick of early work for each switch it costs online, so no deficit accumulates. Under the free promise it does.

## 3. WHY IT IS PROBABLY UNCHARTED

Preemption-overhead scheduling is usually studied through speed augmentation, competitive throughput, or offline hardness. I know of no exact minimax lateness with a hard job-length cap and an adaptive adversary held to a benchmark that pays the same overhead; the comparison of the two promises looks new.

Doubts:
- S(P) = min(c, P − 1) may fall to a short potential argument (work done versus switches paid), making S easy.
- F(P) = P − 1 may be folklore from resource-augmentation lower bounds.
- Ordering choices among several slack jobs might break my guess already at P = 4.

## 4. HOW TO CHECK AN ANSWER

For fixed (P, T, L) and each promise, solve the finite reachability game "the adversary forces some lateness ≥ L".

- **Position:** tick, released multiset (needed for the feasibility test), remaining work per job, the current job A, and whether the previous tick was a switch.
- **Adversary moves:** every multiset that keeps feasibility. Test the free promise by preemptive EDF and the same-cost promise by exhaustive search.
- **After T:** play is a single-player finish.
- **Enumeration:** memoise positions. P ≤ 3 with T up to about 9–10 should stay within a few million positions (an estimate, not counted).
- **Reporting:** give the full sequences S_T(P), F_T(P). A value reaching P − 1 at finite T is final. Any smaller plateau is only a lower bound.

A general claim needs an explicit online rule with a lateness proof valid for all T, plus an adversary strategy with a feasibility proof.
