## KNOWN

The question as given (zero test only) looks easy: g(2) = 2 and g(n) = infinity for all n ≥ 3, any s.

n = 2, b = 2: R is the XOR of two flags, so a zero test reads the other's flag; a Burns-style asymmetric algorithm works.

n ≥ 3: run P2 alone until just after its last test at net contribution 0. Run P1 alone into crit. Continue P2: either every test says nonzero, as when alone (P2 enters crit), or one says zero, so R = 0 with P1 critical, and P3, still in rem, runs as if alone into crit.

Not machine-checked; citation: unsure. The repair lets a test return the whole value.

## FINAL QUESTION

Fix integers n ≥ 2, b ≥ 2, s ≥ 3. One shared counter R holds an element of Z_b (integers mod b), initially 0. Process i (i = 1..n) is a deterministic automaton with at most s states, including distinct states rem_i (initial) and crit_i. Programs may differ between processes. Every state carries exactly one action:

- read, with successors q_0, ..., q_{b-1} (not necessarily distinct): a step moves the process to q_v, where v is the value of R at that moment; R is unchanged.
- add c, for a fixed c in Z_b, with one successor q': a step replaces R by R + c mod b and moves the process to q'; the process learns nothing.

Structural rules: every successor of rem_i differs from rem_i; every successor of crit_i is rem_i; no other state has rem_i as a successor.

A schedule is an infinite sequence over {1..n}; the t-th step is taken, atomically, by the named process. "Time t" means after t steps. A process is active when its state is not rem_i.

The automata solve mutual exclusion if:

- Safety: under no schedule are two processes in their crit states at the same time.
- Progress: there is no schedule and time T such that (i) some process is active at time T, (ii) no process is in its crit state at any time ≥ T, and (iii) every process active at some time ≥ T takes infinitely many steps.

(So a process may stay in rem_i or stall in crit_i forever, and individual starvation is allowed.)

Let g_s(n) be the least b admitting a solution with at most s states per process (infinity if none), and g(n) = min over s of g_s(n).

HEADLINE: find the exact value of g(3). General form: g(n) as a function of n.

Finite instances: for n = 2, s ≤ 4, b ≤ 3, and for n = 3, s = 3, b ≤ 4, decide whether a solution exists.

UNVERIFIED beliefs:
- g(2) = 2: R is the XOR of two flags; a Burns-style asymmetric algorithm, about 6 states.
- g(n) ≥ 3 for n ≥ 3: with b = 2, a process whose own net contribution is nonzero cannot tell "a critical process plus a cancelling one" from "nobody", and a third process in rem then enters.
- g(n) ≤ 2^n − 1: process i toggles a flag by adding ±2^(i−1) mod 2^n − 1, decodes the others' flags from one read, and runs Burns' one-bit-per-process deadlock-free algorithm.
- So 3 ≤ g(3) ≤ 7; weights 1, 1, 2 mod 5 suggest g(3) ≤ 5.

GUESS (not a result): g(3) = 4, because three values seem too few to keep "someone is critical" apart from "nobody is here" under every interleaving, while contenders' identities are not needed.

## WHY IT IS PROBABLY UNCHARTED

Classical bounds concern read/write registers (n needed, Burns–Lynch) or one atomic read-modify-write variable. Here reading and adding are separate steps; adds commute and never overwrite, so covering arguments fail, yet sums can collide. I know no value-count result for a blind-add counter. Doubts: a fetch-and-add bound may transfer; the single-step exit rule may make the answer an artefact.

## HOW TO CHECK AN ANSWER

Fixed (n, b, s): enumerate all n-tuples of automata obeying the structural rules; build the graph on (states, R). Safety: no reachable node has two crit states. Progress: no reachable cycle without crit states on which every process active somewhere on it steps. Prune: each process alone must reach crit; each pair must be correct.

A claim g(3) = v needs explicit automata for b = v, model-checked, plus a proof for each b < v covering all s.
