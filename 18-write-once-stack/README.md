# Experiment 18: a stack in memory that can only be written once

A memory of n cells starts all 0, and a cell can be changed to 1 but never
back. It has to keep track of a stack of bits under pushes and pops chosen by
an adversary, well enough that the top of the stack can always be read off
the cells. How many operations can it survive?

**Short answer: at most n − 1, and exactly n − 1 for n = 2 to 8; from n = 9
the question is open.** In general the best lower bound found is
⌊n/2⌋ + 2 (for n ≥ 6). The chain asked whether n − 1 is always reached and
bet yes; the experiment does not settle it. Neither solver found a
construction that works for every n, and the searches ran out of time at
n = 9.

## The game

A configuration is a string of n bits, starting at 0^n. The stack starts
empty. The operations are PUSH0, PUSH1 and POP (POP only when the stack is
not empty).

A scheme is two functions. Q maps each configuration to EMPTY, 0 or 1. E maps
a configuration and an operation to a new configuration in which every cell
that was 1 is still 1. Both see only the configuration: not the stack, not
the history, not how many operations have happened.

The scheme is fixed first, knowing n and a number t. Then the adversary picks
at most t legal operations. The scheme survives if after every operation Q of
the current configuration is the top of the stack, or EMPTY when the stack is
empty. R(n) is the largest t that some scheme survives.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): is
R(n) = n − 1 for every n?

There are two readings of one sentence. The chain's text says nothing is
checked before the first operation, which leaves Q(0^n) free ("literal"); the
other reading requires Q(0^n) = EMPTY ("strict"). They differ only at n = 1.

## Results

| Claim | Status | Where |
|---|---|---|
| R(n) ≤ n − 1 for n ≥ 2 (for n ≥ 1 under the strict reading) | proved by hand by both solvers; referee read both and found no gap | `solver-a/proofs.txt` §1, `solver-b/proofs.txt` P1 |
| R(1) = 1 under the literal reading, 0 under the strict one | computed by all three instances, with a short hand argument | `reports/` |
| R(n) = n − 1 for n = 2, …, 8, both readings | lower bound by explicit schemes found by each solver's search; the referee's own checker accepted all of them over every legal sequence. Upper bound from the first row | `solver-a/witness_n*`, `solver-b/s_n*` |
| R(9) is 7 or 8 | follows from the rows above (ignore one cell); both solvers' searches for 8 timed out | |
| The two readings agree whenever t ≥ 3 | proved by hand by solver B; read by the referee | `solver-b/proofs.txt` P2 |
| R(n) ≥ ⌊n/2⌋ for all n | proved by hand by both solvers (the pair scheme); read by the referee; checked by program for n ≤ 10 | `solver-b/proofs.txt` P3 |
| **R(n) ≥ ⌊n/2⌋ + 2 for all n ≥ 6** | proved by hand by solver B, relying on 246 computed small schemes, each of which the referee's checker accepted; referee read the proof and found no gap | `solver-b/proofs.txt` P4, `solver-b/mwit/` |
| Starting from a known stack, r + 1 cells are enough for r more operations, for every stack and every r ≤ 6 | computed: schemes exist for every case (those for r ≤ 5 stored by solver B, those for r = 6 regenerated and checked by the referee) | `solver-b/Mtable.txt`, `verifier/m6/` |
| For 3 ≤ r ≤ 6, r cells are never enough from a known stack | computed; for r ≤ 5 by solver B and recomputed by the referee; for r = 6 by solver B's own SAT solver only, with no certificate | `solver-b/Mtable.txt` |
| R(n) = n − 1 for every n ≥ 2 | open | |

## Why n operations are too many

This follows `solver-b/proofs.txt` P1.

Take the two sequences PUSH0, PUSH1, PUSH0, … and PUSH1, PUSH0, PUSH1, …,
n operations each. In both, the top changes at every step after the first, so
Q must change, so the configuration must change, and since cells only go up,
at least one new cell is set at each of those steps. If the first step also
sets a cell in both sequences, each has set n cells after n steps, so both
are at 1^n, where they need different tops.

The first step need not set a cell under the literal reading: a scheme may
leave 0^n in place on, say, PUSH0 and let Q(0^n) = 0. The chain's own
argument missed this case. It is closed separately. For n ≥ 3: PUSH0, PUSH0
then also stays at 0^n, so PUSH0, PUSH0, POP and PUSH0, POP end in the same
configuration but need the answers 0 and EMPTY. For n = 2 a short case check
does it. For n = 1 the case is real, and R(1) = 1.

## The lower bounds

**Pairs.** Use the cells two at a time. A push writes 10 or 01 into the
leftmost untouched pair; a pop turns the rightmost pair that reads 10 or 01
into 11; Q reads the rightmost such pair. Each operation uses up at most one
pair, which gives ⌊n/2⌋ operations.

**Pairs, then a computed ending** (solver B's P4). Keep six cells aside and
run the pair scheme on the rest. When the last pair has been touched, the
pairs still spell out the whole stack and how many operations have been used,
so the number r of operations left is known and is at most 5. From there, use
the computed scheme for that stack and that r on r + 1 of the six spare
cells. This gains two operations over pairs alone.

The gap between ⌊n/2⌋ + 2 and n − 1 is the open question. The computed
table says that from any known stack r + 1 cells suffice for r operations,
for r ≤ 6. If that held for every r it would give R(n) = n − 1 (start from
the empty stack). Solver B reports that the natural induction on r fails.

## What to distrust

- **Everything is by one model family.** Nothing is machine-checked as a
  proof.
- **The headline is not answered.** The values for n ≤ 8 show only that a
  counterexample, if there is one, has n ≥ 9. The solvers' confidence that
  n − 1 always holds is 65 and 75 to 80 per cent, and both say plainly that
  it is a guess. The searches at n − 1 get rapidly harder with n.
- **Values for n = 6, 7, 8 rest on witnesses, not on the referee's own
  search.** The referee's solver confirmed the full table only for n ≤ 5; for
  larger n existence rests on schemes that its checker accepted, and
  impossibility on the hand proof.
- **One table half has a single source.** That 6 cells never suffice for 6
  operations from a known stack is from solver B's self-written SAT solver
  alone. During development solver A's own SAT solver produced one wrong
  "impossible" from a bug that was then fixed. No value of R(n) depends on
  any solver's "impossible" answer.
- **The literature search read no source in full.** It found no paper on a
  stack in write-once memory. The nearest framework is rewriting codes over a
  data graph (Jiang, Langberg, Schwartz, Bruck, 2009), where the whole value
  must be decoded and bounds are asymptotic. Buffer codes (Bohossian, Jiang,
  Bruck, 2007) store the last r bits of a stream in such memory, and their
  abstract gives a number of rewrites that equals n − 1 for binary cells and
  r = 2. That setting has no pop and decodes all r bits, and the search found
  no reduction in either direction, but the paper was not read and should be
  before any of this is called new.
- **Errors in the chain's question.** Its upper-bound argument has the gap
  described above, and its statement fails at n = 1 as worded. Its claim that
  every operation except the last must change the configuration is false: in
  `solver-a/witness_n5_t4.txt` the third of three equal pushes leaves the
  memory unchanged.
- **Small errors in the solvers' reports.** Solver B states the condition
  under which a push may leave the memory unchanged as an "if and only if"
  that fails at the very first operation under the literal reading (solver
  A's version is restricted correctly). Solver B's remark that there is no
  slack until the last two operations is wrong: in both stored schemes for
  n = 8 the fifth of seven operations sometimes sets two cells. Its "every
  entry is computed" does not hold for the r = 6 impossibility half of its
  table. Two of the solvers' negative tests only raise t to n, which the
  theorem already forbids. Solver B reported that three of its processes
  overlapped for a few minutes, over its two-core limit.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: searches (backtracking and SAT), schemes,
  checkers, proofs; `solver-b/mwit/` holds the 246 small schemes.
- `verifier/`: the referee's checker and solver, and `m6/` with the 255
  schemes for r = 6.
- `prior-art.md`: the literature search.

Binaries are left out.
