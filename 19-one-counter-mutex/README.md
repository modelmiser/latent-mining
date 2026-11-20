# Experiment 19: mutual exclusion with one shared counter

Several processes share a single counter that wraps around at b. A step
either reads the counter or adds a fixed amount to it without seeing the
result. An adversary decides whose step comes next. How large must b be for
the processes to take turns in a critical section safely?

**Short answer: for three processes, b = 4 is enough and b = 3 is not.** For
two processes b = 2 is enough. For more than three, all that is known here is
that at least 4 is needed and that 8 suffices for four processes and 14 for
five. The chain guessed 4 for three processes and was right, though not for
the reason it gave.

## The problem

Fix n ≥ 2 processes and b ≥ 2. The counter R holds a number modulo b and
starts at 0. Each process is a deterministic automaton with finitely many
states, among them a resting state and a critical state. Every state has one
action: either read (move to a state that depends on the value of R, leaving
R unchanged) or add c for a fixed c (replace R by R + c modulo b and move to
a fixed state, learning nothing). From the critical state a process can only
go back to rest, and nothing else leads to rest. Programs may differ between
processes.

A schedule names, step by step, which process moves. The automata solve
mutual exclusion if under no schedule two processes are critical at once
(safety), and there is no schedule under which, from some time on, somebody
is out of rest, nobody is ever critical, and everyone who is out of rest
keeps taking steps (progress). A single process may starve.

g(n) is the least b for which a solution exists, with any number of states.
The question, as the chain left it ([chain/3-link.md](chain/3-link.md),
formal text in [chain/3-question.txt](chain/3-question.txt)): find g(3).

## Results

| Claim | Status | Where |
|---|---|---|
| **No solution with b = 3 for n ≥ 3, with any number of states** | proved by hand by both solvers independently, with the same idea; the referee read both and found no gap, and turned each proof into a program that builds the violating schedule the proof describes: 561,600 logged trials on random automata and, by its report, 8,000 more on hand-built ones whose output was not saved; every schedule was accepted by a simulator that knows nothing of the proof | `solver-b/proofs.txt` T3, `solver-a/proofs.txt` Theorem 2, `verifier/refuter.py` |
| No solution with b = 2 for n ≥ 3 | proved by hand by both solvers; read and mechanised the same way by the referee | `solver-b/proofs.txt` T2, `solver-a/proofs.txt` Theorem 1 |
| **g(3) ≤ 4** | explicit automata (two versions, one per solver), each accepted by its author's model checker and by the referee's, written separately | `solver-b/w_n3_b4_s7.txt`, `solver-a/w_n3_b4_burns.txt` |
| So g(3) = 4, and g(n) ≥ 4 for n ≥ 3 | from the rows above | |
| g(2) = 2 | explicit automata with at most 6 states, checked; b ≥ 2 by definition | `solver-b/w_n2_b2_s6.txt` |
| g(4) ≤ 8 and g(5) ≤ 14 | explicit automata, accepted by the referee's checker and by solver B's Python checker (see "What to distrust" for solver B's C++ checker) | `solver-b/w_n4_b8_s7.txt`, `solver-b/w_n5_b14_s7.txt` |
| With at most s states per process: two processes with b = 2 need exactly 6 states; three processes with b = 4 have no solution with 4 states, and the known solution has 7 | the two-process result by both solvers' exhaustive searches, recomputed by the referee. The three-process result by solver B (which left no log; solver A's search timed out) and by the referee, whose saved log covers one of the two halves of its run | `reports/referee.md` |
| Two processes, b = 3: no solution with 5 states | solver B's search only; the referee's own search covered part of it and timed out on the rest | `reports/solver-b.md` |
| g(n) ≤ 2^(n−1) (solver B) or 2^n − 1 (solver A) for all n | sketches, relying on the correctness of a known one-bit algorithm for every n; checked by program for n ≤ 5 (solver B's bound) and for n = 3, 4 (solver A's) | |
| g(n) for n ≥ 4 | open | |

## Why three values are not enough

This follows `solver-b/proofs.txt`, L1 and T3.

For a process p, let E_p be the set of counter values v such that p, starting
from rest with R = v and running alone, reaches its critical state. Running
alone from the true start it must get there (progress), so 0 is in E_p. Let
e_p be the value of R when it gets there, and a_p the first non-zero amount it
adds on the way.

If p runs alone into its critical state and stays, another process q that
then runs alone must not enter: e_p is not in E_q. With a third process j
parked just before its first non-zero add, the same argument after j takes
that one step gives: e_i + a_j is not in E_k, for distinct i, j, k. Modulo 3
these facts leave no room. E_k is exactly {0}, and every a and every e is the
same non-zero value, which may be taken to be 1. Whenever everyone is at rest in
a reachable configuration R must be 0, so leaving the critical state adds 2.

Now each process is forced into a fixed pattern, with the third process used
as a helper that enters and leaves through its critical state to set the
scene. Having raised the counter by 1 and seeing 2 (someone else has raised
it too), a process running alone must not become critical and must not bring
R to 0. So either it never changes R again (it holds), or its next change is
to subtract 1 (it yields). After yielding, and after the helper has gone
through its critical state and left, R is 0 and the others are at rest; the
process must then try again, and its next change is again to add 1. This gives
each process a chain of levels, each level a hold or a yield; the chain ends
at the first hold or goes on forever. The helper also shows that any level of
one process can be reached together with any level of another.

Two processes whose chains end in a hold can be brought there together: each
adds 1, each then sees only 2 and never changes R, nobody enters. Two processes whose chains
never end can be run in lockstep: both add, both read 2 and yield, both read
0 and try again, forever. Either way progress fails. Among three processes
two have chains of the same kind.

With b = 2 the facts of the first two paragraphs already contradict each
other.

## Why four values are enough

Both solvers use variants of one construction: a known algorithm with one
flag bit per process (the solvers call it Burns' one-bit algorithm), with the
flags stored as amounts in the one counter, 1, 1 and 2 modulo 4. Raising a
flag adds the process's amount and lowering it subtracts it. Solver A notes
why these amounts are enough: the second process has to tell whether the
first or the third has its flag up, and the sums 0, 1, 2, 3 are distinct;
the first and third only need to know whether anybody has. The automata have
7 states at most. Correctness is by model checking all reachable
configurations (104 for solver A's version, 182 for solver B's), not by a
written proof.

The same idea with amounts 1, 1, 2, 4 modulo 8 and 1, 1, 2, 5, 5 modulo 14
gives the bounds for four and five processes; both solvers report these as
the least moduli for which this particular construction works.

## What to distrust

- **Everything is by one model family.** The impossibility proof is not
  machine-checked as a proof. The referee's mechanisation is a strong test
  (the proof's construction always produced a genuine violation) on automata
  with at most about 30 states, and it is not a proof.
- **The upper bound is a model-checked object, not an argument.** The
  checkers that accept it were written from the same problem text by
  instances of the same model.
- **No exact value is settled beyond three processes.** The lower-bound
  argument does not extend to b = 4 as written, and no search or proof
  addresses four processes with b between 4 and 7, apart from the 3-state
  case, which the two-process searches already exclude.
- **Solver B's C++ checker overruns an array on the five-process file.**
  `solver-b/mc.h` allots 8 read branches per state by default and the
  b = 14 automata have 14 (found by the review panel, GPT; I confirmed the
  default in the source). Its verdict on that file is therefore not
  evidence. The referee's Python checker accepts the file (7,070 reachable
  configurations, rerun by the organiser), and solver B reports that its own
  Python checker does.
- **The literature search read two sources in part and none in full.** The
  classical paper on mutual exclusion with one shared variable (Burns,
  Jackson, Lynch, Fischer, Peterson, 1982) lets a step test and set the variable
  atomically, where 2 values suffice and are needed for any number of processes; that does
  not transfer to separate read and add steps. A single location with read
  and add steps is studied for consensus by Ellen, Gelashvili, Shavit and Zhu
  (2016), with unbounded size. The search did not find the impossibility for
  b = 2, 3 or the value g(3) = 4, and notes that older technical reports and
  textbook exercises are poorly indexed. The general upper bound is probably
  an easy consequence of known work.
- **One search result has a single source**: no 5-state solution for two
  processes with b = 3.
- **The chain's reasoning.** Its guess of 4 was right. Its reason (that the
  identities of contenders are not needed) is, in solver B's words,
  backwards: three values fail because a process cannot tell whom to yield
  to. Its suggestion of amounts 1, 1, 2 modulo 5 works, and modulo 4 does
  too.
- **Small errors.** Solver A's proof file ends by listing the wrong moduli as
  the checked instances of its general sketch (its report has the right
  ones), calls its three-process automata "7-state" when they have 3, 7 and 6
  states, and has one unnecessary detour in its proof. Solver B's run for
  three processes with 4 states left no log; the referee recomputed that
  case. One of the referee's broken test files breaks safety where its name
  says progress. The solvers disagree on the general upper bound (2^n − 1
  against 2^(n−1)); neither is proved. The referee reported one short test
  that ran as a third process against the two-core rule.
- **Two files over 300 kB** (`solver-b/pairs_b4_s4_p*.txt`, lists of
  two-process solutions) are left out.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files, with most of the
brief aimed at the impossibility proof for b = 3. All five confirmed each
step of that proof, the b = 2 case, and that the checkers test safety and
progress as defined; none found automata that evade the proof. All five
marked my explanatory sections refuted on one sentence: I wrote that neither
solver explains why the amounts 1, 1, 2 suffice, and both do. Other findings,
applied above:

- GPT rated the array overrun in solver B's C++ checker critical. It does
  not touch the headline (three processes, b = 4, where the array is large
  enough) and the five-process bound has another checker behind it.
- The count of referee trials mixed 561,600 logged ones with 8,000 unlogged.
- "Checked for n ≤ 5" was true of only one of the two general sketches, and
  the 4-state search was credited to both solvers when one timed out.
- "Nothing is settled beyond three processes" ignored the bounds that are.
- Several phrases in my retelling of the proof were looser than the proof
  ("reads 2" where a process may also add 0; R returning to 0 without
  mention of the helper).

The corrected wording has not been through a second round.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: searches, model checkers, automata (`w_*.txt`),
  proofs.
- `verifier/`: the referee's checker, searches, and `refuter.py`, which runs
  the impossibility proofs as programs.
- `prior-art.md`: the literature search.

Binaries are left out.
