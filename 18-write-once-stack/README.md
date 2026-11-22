# Experiment 18: a stack in memory that can only be written once

A memory of n cells starts all 0, and a cell can be changed to 1 but never
back. It has to keep track of a stack of bits under pushes and pops chosen by
an adversary, well enough that the top of the stack can always be read off
the cells. How many operations can it survive?

**Short answer: for n ≥ 2 at most n − 1, and exactly n − 1 for n = 2 to 8;
from n = 9 the question is open.** In general three lower bounds are proved here, and the best is their maximum: ⌊n/2⌋ + 2 (for n ≥ 6) and 7 (for n ≥ 8, from ignoring cells) from the first version, and, from a follow-up of 21 November, 3·⌊(n − 1)/4⌋ − 1, about three quarters of n, which is the largest of the three at n = 17, n = 21 to 23 and from n = 25 on (17 against 14 at n = 25). The chain asked whether n − 1 is always reached and
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
R(n) = n − 1 for every n? Read literally this already fails at n = 1 (see
the next paragraph), so the question that is open is the one for n ≥ 2.

There are two readings of one sentence. The chain's text says nothing is
checked before the first operation, which leaves Q(0^n) free ("literal"); the
other reading requires Q(0^n) = EMPTY ("strict"). They differ only at n = 1.

## Results

| Claim | Status | Where |
|---|---|---|
| R(n) ≤ n − 1 for n ≥ 2 (for n ≥ 1 under the strict reading) | proved by hand by both solvers; referee read both and found no gap | `solver-a/proofs.txt` §1, `solver-b/proofs.txt` P1 |
| R(1) = 1 under the literal reading, 0 under the strict one | computed by all three instances, with a short hand argument | `reports/` |
| R(n) = n − 1 for n = 2, …, 8, both readings | lower bound by explicit schemes found by each solver's search; each scheme is accepted by its author's checker and by the referee's own checker, over every legal sequence (the two solvers' checkers cannot read each other's file format). Upper bound from the first row | `solver-a/witness_n*`, `solver-b/s_n*` |
| R(9) is 7 or 8 | follows from the rows above (ignore one cell); both solvers' searches for 8 timed out | |
| The two readings agree whenever t ≥ 3 | proved by hand by solver B; read by the referee | `solver-b/proofs.txt` P2 |
| R(n) ≥ ⌊n/2⌋ for all n | proved by hand by solver B (the pair scheme); solver A states it and checks it by program without writing the argument out; read by the referee; checked by program for n = 2 to 10 | `solver-b/proofs.txt` P3 |
| **R(n) ≥ ⌊n/2⌋ + 2 for all n ≥ 6** | proved by hand by solver B, relying on 246 computed small schemes, each of which the referee's checker accepted; referee read the proof and found no gap | `solver-b/proofs.txt` P4, `solver-b/mwit/` |
| Starting from a known stack, r + 1 cells are enough for r more operations, for every stack and every r ≤ 6 | computed: schemes exist for every case (those for r ≤ 5 stored by solver B, those for r = 6 regenerated with solver B's SAT solver and checked by the referee's checker) | `solver-b/Mtable.txt`, `verifier/m6/` |
| For 3 ≤ r ≤ 6, r cells are never enough from a known stack | computed; for r ≤ 5 by solver B and recomputed by the referee; for r = 6 by solver B's own SAT solver only, with no certificate | `solver-b/Mtable.txt` |
| **R(4q + 1) ≥ 3q − 1 for every q ≥ 1, hence R(n) ≥ 3·⌊(n − 1)/4⌋ − 1 for n ≥ 5** (better than ⌊n/2⌋ + 2 at n = 17, n = 21 to 23 and every n ≥ 25) | proved by hand by one follow-up instance on 21 November (a two-level version of the pair scheme); the scheme was checked from the definition against every legal sequence for q = 1, …, 6 (n up to 25; at q = 6, 99,095,529 sequences of length ≤ 17): q = 5, 6 by the instance (`quad/run_quad.log`), q ≤ 4 rerun by the organiser (`quad/organiser_rerun.txt`); five reviewers traced the proof and found no gap, and two rebuilt the scheme from the proof's text with their own checkers for q ≤ 4 (see Review) | `quad/PROOF.md`, `quad/quad.py`, `quad/checker.py`, `quad/run_quad.log`, `quad/organiser_rerun.txt` |
| The same scheme survives 3q operations for q ≤ 6 and fails at 3q + 1 | computed only; not proved (q = 5, 6 in `quad/run_quad.log`, with 296,855,529 sequences of length ≤ 18 at q = 6; q ≤ 4 in `quad/organiser_rerun.txt`) | `quad/run_quad.log`, `quad/organiser_rerun.txt` |
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
into 11; Q reads the rightmost such pair. Only a push needs a fresh pair, and
⌊n/2⌋ operations contain at most ⌊n/2⌋ pushes.

**Pairs, then a computed ending** (solver B's P4). Keep six cells aside and
run the pair scheme on the rest. When the last pair has been touched, the
pairs still spell out the whole stack and how many operations have been used,
so the number r of operations left is known and is at most 5. From there, use
the computed scheme for that stack and that r on r + 1 of the six spare
cells. This gains two operations over pairs alone. (For n = 6 and 7 there
are no pairs and the computed scheme is used from the start.)

The gap between the best lower bound (the maximum of ⌊n/2⌋ + 2, 7 and, see below, 3·⌊(n − 1)/4⌋ − 1) and n − 1 is the open question. The computed
table says that from any known stack r + 1 cells suffice for r operations,
for r ≤ 6. If that held for every r it would give R(n) = n − 1 for every
n ≥ 2 (start from the empty stack). Solver B reports that the natural
induction on r fails. Solver B also remarks that the same argument with the
r = 6 schemes and seven spare cells would give ⌊(n + 5)/2⌋; the referee's
folder holds those schemes, but nobody has written or checked that proof, so
it is not claimed here.

### Three quarters of n (follow-up of 21 November)

This follows `quad/PROOF.md`. Take n = 4q + 1 cells: 2q pairs, grouped two
by two into q "quads", and one flag cell. First the pair scheme runs: each
push writes one of the two cells of a fresh pair, each pop sets the other
cell and so kills the pair. When a push finds no fresh pair, the flag is
set and the pairs are reused at a second level: a quad whose two pairs are both still alive stores a new bit by which of its two pairs gets killed, while the pairs to its right keep the values they already hold and are still popped one pair at a time. That destroys the two old bits the quad held, but
they are the bottom-most readable ones, and the proof shows they lie deeper
than the remaining operations can reach. Half-dead quads left behind by
earlier pops are killed, without costing an operation, when the frontier
passes them. Charging every operation to a quad gives, once the flag is set, s = 4q − |R| − w (the proof needs only ≥), where s is the number of operations so far, R the part of
the stack that can still be read, and w the number of quads wasted; with w ≤ q − 1 the readable part has, once the flag is set, at least two more entries than there are operations left.

The instance could not push the idea further: with more levels, a half-dead
quad from the first phase looks like a second-level element, and an
adversary can place long runs of them at a cost of 3 operations per 4
cells, which gives the same ratio again. That is its sketch, not a result. It reports that its attempt to compute R(9) with solver B's program ran out of time; no log of that run was kept. (Both statements are from the instance's report, which came back as text and is not stored in this folder.)

## What to distrust

- **The three-quarters bound is hours old and has one author.** The delicate steps
  of its proof are the table of charges per quad and the claim that a pop
  never hits the first intact quad. The machine check covers n = 4q + 1 up to 25 (q ≤ 6) only (the log ends with two out-of-memory errors that the instance says are the runs for n = 29; the log does not record their commands) and tests the scheme, not the proof for general q. It checks that the configuration only grows along reachable plays, not for every configuration. `quad/PROOF.md` is left as written; reviewers noted that one sentence gives a wrong reason at the freeze (it misstates the order of the destroyed entries, though the set destroyed is the bottom of the stack as needed), that its "equality" in the charging equation holds as long as no operation is a no-op, which the budget guarantees, that it cites `run_quad.log` for small cases that are in the organiser's file, and that the update is specified only for configurations that can be reached. One of its two deliberately broken variants is caught at
  q = 4 but not at q = 3.
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
- **The literature search read part of one source and none in full.** It found no paper on a
  stack in write-once memory. The nearest framework is rewriting codes over a
  data graph (Jiang, Langberg, Schwartz, Bruck, 2009), where the whole value
  must be decoded and the bounds are a counting bound and asymptotic
  estimates. Buffer codes (Bohossian, Jiang,
  Bruck, 2007) store the last r bits of a stream in such memory, and their
  abstract gives a number of rewrites that equals n − 1 for binary cells and
  r = 2. That setting has no pop and decodes all r bits, and the search found
  no reduction in either direction, but the paper was not read and should be
  before any of this is called new.
- **Errors in the chain's question.** Its upper-bound argument has the gap
  described above, and its statement fails at n = 1 as worded. Its claim that
  every operation except the last must change the configuration is false: in
  `solver-a/witness_n5_t4.txt` the third of three PUSH1s leaves the
  memory unchanged.
- **Small errors in the solvers' reports.** Solver B states the condition
  under which a push may leave the memory unchanged as an "if and only if"
  that fails at the very first operation under the literal reading (solver
  A's version is restricted correctly). Solver B's remark that there is no
  slack until the last two operations is wrong: in every stored scheme for
  n = 8 the fifth of seven operations sometimes sets two cells. Its "every
  entry is computed" does not hold for the r = 6 impossibility half of its
  table. Two of the solvers' negative tests only raise t to n, which the
  theorem already forbids. Solver A's report states one case of its
  upper-bound proof the wrong way round ("both first pushes leave 0^n" for
  "neither does"); its proof file has it right. Solver B's `check.py` skips
  lines it cannot parse instead of rejecting the file, and its `hybrid.py`
  builds the combined scheme for even n only. Solver B reported that three of its processes
  overlapped for a few minutes, over its two-core limit.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files. All five confirmed
the upper bound, the schemes for n ≤ 8, the reduction, the pair scheme and
the ⌊n/2⌋ + 2 bound. Their findings on this page, applied above:

- "At most n − 1" was stated without n ≥ 2 in several places, although
  R(1) = 1 under the literal reading; three seats marked the summary refuted
  for that.
- I wrote that three checkers accept each scheme; each is accepted by two.
- "Best general lower bound ⌊n/2⌋ + 2" overlooked that R(8) = 7 already
  gives 7 for all n ≥ 8 (Claude Opus), and that the stored r = 6 schemes
  would support a stronger bound nobody has written up.
- The pair bound was credited to both solvers as a proof; only solver B
  wrote one.
- A reversed case in solver A's report had been missed by the referee and by
  me.

The corrected wording has not been through a second round. The first commit
of this folder went out with the uncorrected page by mistake; this is the
corrected one.

The three-quarters bound of 21 November went to the same five reviewers.
All five confirmed the proof: the first phase, the freeze, the table of
charges and the equation it gives, both invariants under push and pop, the
claim that a pop never hits the first intact quad, and the corollary with
the list of n where the bound is best. Both Claude seats rebuilt the scheme
from the proof's text alone and checked it with their own exhaustive
checkers for q = 1 to 4: it survives 3q − 1 and 3q and fails at 3q + 1,
with the same sequence counts as the logs. Their findings, applied above:
the log cited for the small cases did not contain them (four seats; the
organiser ran and stored them); the Short answer and the lower-bounds
section still gave ⌊n/2⌋ + 2 as the best bound (all five); the count
296,855,529 belongs to the unproved length 3q; two statements about failed
runs had no log; one sentence of the proof gives a wrong reason for a true
step (Grok, Gemini). A second-round reviewer (one Claude instance) then rechecked the edited page, recomputing the list of n, the sequence counts and rerunning q ≤ 5; every number held, and its wording corrections are applied.


## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each instance of the first version was told.
- `reports/`: the final reports of the two solvers and the referee, verbatim (the follow-up instance's report is not stored).
- `solver-a/`, `solver-b/`: searches (backtracking and SAT), schemes,
  checkers, proofs; `solver-b/mwit/` holds the 246 small schemes.
- `quad/`: the follow-up of 21 November: the brief, the proof of R(4q + 1) ≥ 3q − 1, the scheme, a checker written from the definition (and `check_qe.py`, an unused reader for another file format), the instance's log and the organiser's rerun.
- `verifier/`: the referee's checker and solver, and `m6/` with the 255
  schemes for r = 6.
- `prior-art.md`: the literature search.

Binaries are left out.
