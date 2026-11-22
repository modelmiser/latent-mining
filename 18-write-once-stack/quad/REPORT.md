<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the follow-up instance (R(4q + 1) >= 3q - 1), verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

# Experiment 18, lower bound: report

Folder: `(staging)/`

The harness refused the write of `REPORT.md` ("subagents should return findings as text"), so this text is the report. `PROOF.md` and the code were written normally. Nothing was written under `(workspace)`, and no process of mine is still running.

## Strongest statement

**R(4q+1) ≥ 3q − 1 for every q ≥ 1, hence R(n) ≥ 3·⌊(n−1)/4⌋ − 1 ≥ (3n − 16)/4 for all n ≥ 5.** This is goal (B): a constant 3/4 in place of 1/2. It is strictly better than ⌊n/2⌋ + 2 at n = 17, at n = 21 to 23, and for every n ≥ 25 (n = 25: 17 against 14); it ties at n = 13, 18, 19 and 24, and loses by one at n = 14 to 16 and at n = 20. It beats "7 for n ≥ 8" from n = 13 on.

The headline (is R(n) = n − 1 for all n?) is **not settled**. Goals (A), (C) and (D) were not reached.

| Part | Status | Where |
|---|---|---|
| R(4q+1) ≥ 3q − 1, all q | PROVED by hand, complete | `PROOF.md` |
| The proof's scheme survives 3q − 1 for q = 1..6 (n = 5..25) | COMPUTED: every legal sequence, checker written from the definition | `quad.py`, `checker.py`, `run_quad.log` |
| The same scheme also survives 3q for q = 1..6 and fails at 3q + 1 | COMPUTED only; I have no proof of 3q | same |
| R(9) = 8 | FAILED: one run of solver B's SAT program at the 3000 s cap ended with no answer | `run_sat9.log` (empty) |
| A construction for n − 1, or for n − o(n) | FAILED | below |

For q = 6 the check covered 296,855,529 sequences of length ≤ 18. q = 7 hit the 6 GB memory cap and is not checked.

## The construction (two levels)

n = 4q + 1: 2q pairs of cells, grouped two by two into q quads, plus one flag cell.

- **First phase.** The pair scheme runs as in solver B's P3.
- **Freeze.** When a push finds no untouched pair, the flag is set and the pairs become the cells of a second pair scheme.
- **Second phase.** A quad whose two pairs are both live holds two free cells. A push kills one of the two pairs, and which one survives is the pushed bit. The two elements stored in that quad are lost; they are the bottom-most readable ones.
- **Waste.** Half-dead quads left by first-phase pops are killed, without an operation, when the frontier passes them.
- **Boundary.** New elements sit to the left of the first quad with two live pairs, old ones from it rightwards.

The proof charges each operation to one quad and gets s = 4q − |R| − w, where s is the number of operations done, R the readable top part of the stack, and w the number of wasted quads. With w ≤ q − 1 this gives |R| ≥ r + 2 whenever r operations remain, which is all the reading needs. Destroying the bottom of the stack is allowed only because it lies deeper than the remaining budget can reach.

## What failed, and why

- **More levels (octets and up).** With pushes only, the recursion gives n/2 + n/4 + … = n − 1 and stays exactly inside the window. With pops it breaks: a half-dead quad left by first-phase pops looks the same as a second-level element, so the frontier must kill it. The adversary can put a long run of them between two intact quads at 3 operations per 4 cells, which is the 3/4 ratio again. I found no way to tell the two kinds apart without spending cells. This is a sketch, not coded or tested, and I claim no bound from it.
- **Storing the frontier.** A pointer that moves at every push costs a cell per move. A snapshot once per level costs about log n cells but does not remove the waste between intact quads.
- **The tight case, t = n − 1.** When the pairs run out, every live pair is still inside the window, so nothing may be destroyed. A push must then merge the two bottom-most live pairs wherever they sit, and the reader cannot tell which was killed. This agrees with solver A's remark that the pair scheme cannot be continued to n − 1.
- **Rank-order readings.** If the stack order depends only on the rank of each free cell and on how many there are, the pop and push rules contradict each other at four free cells. This is a hand check that I did not write to a file.
- **R(9).** I copied solver B's `sat.cpp` and its table and ran n = 9, t = 8 once. That the run timed out is inferred: the log is empty, there is no witness file, and the process was gone well after the cap. No SAT solver is installed on the machine.

## What a reader must still trust

- **The proof.** `PROOF.md` is a hand proof by one model; nobody else has read it. The delicate steps are the table of charges per quad and the claim that a POP never hits the first intact quad.
- **The machine check.** It covers q ≤ 6 only. It explores (configuration, stack) pairs step by step, which is every legal sequence; its sequence counts agree with a separate count by depth (73, 209, 617, 1801). The q = 7 tracebacks are in `run_quad.log`.
- **The checker was seen to fail** on:
  - solver A's n = 5 witness at t = 5;
  - solver A's `mutant_swap_n7.txt`;
  - the quad scheme at 3q + 1;
  - two mutants of the quad scheme: `nowaste`, and `nofreezeclean`, which is caught at q = 4 but not at q = 3.
- **Both readings.** The bound holds for the literal and the strict reading; the check requires Q(0^n) = EMPTY.
- **Untested file.** `check_qe.py` was never run, because the SAT run produced no witness for it to read.

## Files

- `PROOF.md`: statement, scheme, proof
- `quad.py`: the scheme as two functions of the configuration
- `checker.py`: checker from the definition; also reads solver A's table format
- `check_qe.py`: reader for solver B's Q/E witness format, unused
- `run_quad.log`
- `satb.cpp`, `satb`, `Mtable_b.txt`: copies of solver B's SAT program and table, used only for the R(9) attempt
- `run_sat9.log`: empty
- `w7.txt`: an n = 7 witness regenerated as a smoke test of the copied SAT program; not put through my checker
- `compile.log`
