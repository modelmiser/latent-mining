## KNOWN

The question as given is degenerate. Nothing forces a process to leave rem or crit: let process 1 go rem → crit and stay, and let every other process loop in rem forever. Safety holds, progress holds vacuously, so f(n) = 2 with s = 2 for every n.

Nearby established results: Burns and Lynch showed n read/write registers are necessary for n-process deadlock-free mutual exclusion; Burns' one-bit algorithm uses n single-writer bits (so 2^n is plausible after repair); with one general read-modify-write variable, two values suffice (Burns, Fischer, Jackson, Lynch, Peterson). For blind commutative adds I know no result: unsure. Identical programs can never work: a lockstep schedule keeps all processes in equal states.

## SUCCESSOR QUESTION

Fix integers n ≥ 2, b ≥ 2, s ≥ 3. One shared counter R holds an element of Z_b (integers mod b), initially 0. Process i (i = 1..n) is a deterministic automaton with at most s states, including distinct states rem_i (initial) and crit_i. Programs may differ between processes. Every state carries exactly one action:

- test, with successors q0, q1: a step moves the process to q0 if R = 0 at that moment, else to q1; R is unchanged. (The process learns only whether R is zero.)
- add c, for a fixed c in Z_b, with one successor q': a step replaces R by R + c mod b and moves the process to q'; the process learns nothing.

Structural rules: every successor of rem_i differs from rem_i; every successor of crit_i is rem_i; no other state has successor rem_i.

A schedule is an infinite sequence over {1..n}; the t-th step is taken, atomically, by the named process. Time t means after t steps. A process is active when its state is not rem_i.

The automata solve mutual exclusion if:

- Safety: under no schedule are two processes ever in their crit states at the same time.
- Progress: there is no schedule and time T such that (i) some process is active at time T, (ii) no process is in its crit state at any time ≥ T, and (iii) every process active at some time ≥ T takes infinitely many steps.

(So a process in rem_i may stay unscheduled forever, a process may be stalled in crit_i forever, and individual starvation is allowed.)

Let g_s(n) be the least b admitting a solution with at most s states per process (infinity if none), and g(n) = min over s of g_s(n).

Question: determine g(n) for every n ≥ 2. In particular, is g(3) finite, and is g bounded, polynomial, or exponential in n? Finite instances: for n = 2 and each s ≤ 5, and for n = 3 and each s ≤ 4, list exactly which b ≤ 4 admit a solution (feasibility need not be monotone in b).

## WHY IT IS PROBABLY UNCHARTED

Zero-test kills the bit-per-process encoding: a process sees only whether all contributions cancel, and checking "am I alone" means withdrawing its own announcement non-atomically. Classical bounds assume overwriting writes (covering) or atomic read-modify-write; a blind commutative counter with zero-test fits neither. Doubts: g(3) may be infinite by a short indistinguishability argument I have not found; counter-machine or Petri-net literature might hold it in disguise; for b = 2 the test is a full read, so n = 2 may be easy.

## HOW TO CHECK AN ANSWER

For fixed n, b, s there are at most b·s^n configurations. Safety is reachability; progress fails iff a reachable crit-free cycle of configurations exists containing an active process, in which every process active somewhere on it moves. Enumerate automata up to state renaming; prune using the fact that any sub-family of a solution is a solution (unscheduled processes rest in rem): singles, then pairs, then triples. A general formula needs a construction plus an impossibility proof, checked against the tables.
