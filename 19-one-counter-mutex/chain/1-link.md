## 1. KNOWN

For read/write registers, deadlock-free mutual exclusion among n processes needs n registers and n bits suffice (Burns–Lynch; the lower bound is a covering argument: a write obliterates what was there). With test-and-set, one bit suffices for deadlock-freedom; the number of values of a single read-modify-write variable under fairness conditions was studied by Burns, Fischer, Jackson, Lynch and Peterson. Solo step complexity versus register width: Lamport's fast algorithm, Alur–Taubenfeld (details unsure).

Flaws in the seed: too many parameters (n, b, step type, two measures), and "step types in between" is undefined. The proposed example is degenerate: on a one-bit register, "write 1 and report whether it changed" is exactly test-and-set, so one bit suffices. The interesting middle ground is a step that never returns anything yet does not obliterate.

## 2. SUCCESSOR QUESTION

Fix integers n ≥ 2 and b ≥ 2. There is one shared register R holding an element of Z_b (integers mod b), initially 0. Process i (i = 1..n) is a deterministic finite automaton with state set Q_i containing two distinct states rem_i (its initial state) and crit_i. Every state q carries exactly one action:

- read, with a transition function d_q : Z_b → Q_i. When the process takes a step in q it moves to d_q(v), where v is the current value of R; R is unchanged.
- add c, for a fixed c in Z_b, with one successor state q'. When the process takes a step, R becomes R + c mod b and the process moves to q'. The process learns nothing from this step.

The programs may differ between processes (process identities are built in). A schedule is any infinite sequence over {1..n}; the t-th step is taken by the named process. A process is active when its state is not rem_i.

The n automata solve mutual exclusion if both hold:

- Safety: under no schedule is there ever a time at which two processes are in their crit states.
- Progress: there is no schedule and time T such that (i) some process is active at time T, (ii) no process is in its crit state at any time ≥ T, and (iii) every process that is active at some time ≥ T takes infinitely many steps.

(So a process resting in rem_i may be left unscheduled forever, and scheduling it is how it starts competing; a process may also be stalled inside crit forever, which blocks nobody's obligation.)

Let f_s(n) be the least b for which automata with |Q_i| ≤ s for all i solve mutual exclusion (infinity if none), and f(n) = min over s of f_s(n).

I believe f(n) ≤ 2^n: process i adds ±2^(i-1) to own bit i of R, which simulates n single-writer bits, and an n-bit single-writer algorithm exists. I believe f(2) = 2: with one toggling bit each process knows its own contribution, so a read reveals the other's.

Question: determine f(n) for every n. In particular: what is f(3), and is f(n) bounded, polynomial in n, or exactly 2^n? As finite instances, determine the table f_s(2) and f_s(3) for s ≤ 6, and for n = 3 list the whole set of b ≤ 8 that admit a solution (feasibility need not be monotone in b).

## 3. WHY IT IS PROBABLY UNCHARTED

The covering argument needs overwriting; additions commute and are never lost, only confounded modulo b, so the classical lower bound does not apply, while there is no atomic read-modify-write, so the single-variable results do not apply either. Doubts: Ellen, Gelashvili, Shavit and Zhu studied {read, add} and {read, increment} instruction sets for consensus space; a mutual-exclusion analogue with bounded modulus may exist that I do not know. The answer might also collapse (for instance f(n) = 2 for all n by a parity trick I have not seen), which would be a short theorem.

## 4. HOW TO CHECK AN ANSWER

Upper bounds: exhibit the n automata; a model checker explores the finite product (at most b·s^n configurations), checks safety by reachability and progress by searching for a reachable fair crit-free cycle (strongly connected component in which every active process moves). Lower bounds for f_s(n): exhaustive or SAT-guided enumeration of all automata tuples with ≤ s states, feasible for n = 2 and small n = 3 cases. A general lower bound on f(n) needs a proof, valid for every s.
