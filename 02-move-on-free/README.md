# Experiment 2: move on free

A memory allocator that may move blocks, but only at the moment something is
freed, and only a little. How much is that right worth?

**Short answer: not settled.** The right is worth nothing to an allocator that
packs its first blocks together (proved), and nobody here found an allocator
for which it is worth everything. The exact value is open.

## The game

Cells are 0, 1, 2, …. Fix M ≥ 3. Blocks have size 1, 2 or 3. An adversary who
sees the layout issues requests forever:

- `allocate(s)`: legal only if (live cells) + s ≤ M. The manager must place the
  block at once on s contiguous free cells.
- `free(b)`: the live block b vanishes.

Three games on these requests:

- **N (no moves).** Blocks never move.
- **F (move on free).** Right after a block of size s is freed, the manager may
  move blocks whose sizes total at most s − 1. Freeing a 1 grants nothing,
  freeing a 2 grants one move of a 1, freeing a 3 grants one move of a 2 or two
  moves of 1s. Unused allowance is lost. 3s never move.
- **O (only 1s freed).** Game N, except the adversary may free only 1s.

h^G(M) is the least heap size H such that some manager serves every legal
request sequence inside cells 0 … H−1. Always h^O ≤ h^F ≤ h^N. Write
μ^G = limsup h^G(M)/M.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): is
μ^O < μ^F < μ^N, and what is μ^F?

## Results

| Claim | Status | Where |
|---|---|---|
| Table of h^N, h^F, h^O for M = 3 … 13 (below) | computed; three independent programs agree for M ≤ 11, two for M = 12, 13 | `solver-a/`, `solver-b/`, `verifier/` |
| h^O(M) ≥ 5M/3 − 14/3 | proved by hand; referee read it | `solver-a/proofs.txt` P1 |
| h^O(M) ≤ ⌈5M/3⌉ (first fit) | proved by hand; referee read solver A's version; exhaustive check of first fit for M ≤ 16 | `solver-a/proofs.txt` P2, `solver-b/proofs.txt` P5 |
| **μ^O = 5/3** | follows from the two rows above | |
| h^N(M) ≥ 7M/4 − 17/4, so **μ^N ≥ 7/4 > μ^O** | proved by hand; referee read it and played it against 321 managers per M; I re-derived it | `solver-b/proofs.txt` P6, `verifier/p6*.log` |
| h^N(M) ≤ 43M/18 + 12 | proved by hand; referee read it; only a random stress test behind it | `solver-a/proofs.txt` P3 |
| **Theorem A.** In game F, a manager that puts the first M 1s on consecutive cells needs H ≥ 7M/4 − 7 | proved by hand by one follow-up instance; I re-derived it; played out for M = 3 … 60 against five such managers, no violation | `open-f-lower/proofs.txt` L1, L2, Theorem A |
| In game F, a manager that puts the first M 1s on cells 0 … M−1 needs H ≥ (5/3 + 1/144)M − 12 | computer-assisted, by a second follow-up instance that did not see the first; weaker than Theorem A, by a different route; not re-derived | `open-f-upper/proofs.txt` R, R′ |
| Against managers that leave 3-cell gaps among the first 1s, the same adversary family forces H ≥ about 1.70·M | computed, in a periodic model only (350 layouts, at most 30 ones per period) | `open-f-lower/proofs.txt` section E |
| **μ^F** | **open.** Proved: 5/3 ≤ μ^F ≤ μ^N ≤ 43/18 | |
| μ^N exactly | open, in [7/4, 43/18] | |

Exact values, M = 3 … 13:

| M | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 |
|---|---|---|---|---|---|---|---|---|---|---|---|
| h^O | 4 | 6 | 7 | 9 | 11 | 13 | 14 | 16 | 18 | 20 | 21 |
| h^F | 4 | 6 | 7 | 9 | 11 | 13 | 14 | 16 | 18 | 20 | 21 |
| h^N | 4 | 6 | 7 | 9 | 11 | 13 | 14 | 16 | 18 | 20 | 22 |

## What the table cannot tell you

The three games agree up to M = 12, and F agrees with O up to M = 13. That
looks like evidence that the move right is worth everything. It is not:

- The proved gap between N and O (7/4 against 5/3) cannot show before M is
  about 59, because of the additive constants.
- Theorem A's bound passes 5M/3 only for M > 84.
- ⌊7M/4⌋ − 1 fits every exact value for M ≤ 12 in all three games, and is
  provably wrong for game O at M = 24 and for every M > 32.

Both solvers leaned towards μ^F = 5/3 from this data. Both follow-up instances,
working at M in the hundreds, came to believe the opposite.

## Why Theorem A works

The adversary allocates M 1s, frees the residue class mod 3 that holds fewest,
and allocates 2s. It then frees every second 2. In game N that is the whole
7/4 attack. In game F each freed 2 lets the manager move one 1, and if the
adversary has already committed to one residue class, moving a 1 off that class
cancels the gain exactly.

The fix is not to commit. The adversary frees the moved 1 at once, which costs
nothing, so the manager's move amounts to deleting one 1. With two residue
classes still alive, a deletion hurts only one of them, and the adversary keeps
the other. A reply is then worth 1 where the freed 2 gained 2.

A manager escapes this only by leaving room for 2s among its first M 1s. The
computed cases suggest that this lowers the loss from 3/4 to about 0.70 per
unit of M and no further, but that is a search over periodic layouts, not a
proof.

## What to distrust

- **μ^F > 5/3 is a belief, not a result.** The two follow-up instances put it
  at "more likely than not" and "about 3 to 1". Longer periods helped the
  manager in one computed case, so a sequence of managers approaching 5/3 is
  not excluded.
- **Everything is by one model family.** "Proved by hand" means one instance
  wrote it and another read it. Nothing in this experiment is machine-checked.
- **The table at M = 12 and 13** rests on the two solvers' programs. The
  referee's independent program stopped at 11, so the one place where N and F
  differ (M = 13) has two implementations behind it, not three.
- **Theorems R and R′** rest on a graph check (`open-f-upper/cyc.cpp`) that
  only its author ran. They are implied by Theorem A, so nothing else depends
  on them.
- **The 43/18 upper bound** has no exhaustive test.
- **The periodic model** in `open-f-lower` gives the manager foresight and
  restricts the adversary, so each slope is a lower bound against that one
  manager. `pergame.cpp` was checked against the Python prototype in three
  cases only, and linearity beyond 8 periods is assumed.
- **Prior art** ([prior-art.md](prior-art.md)): the search found none of this
  in print, but read one paper in full. Robson's 1971 paper, the most likely
  place for the no-move value with sizes {1, 2, 3}, was not read.

## Files

- `chain/`: the seed and the three links, verbatim.
- `brief-solver-a.txt`, `brief-solver-b.txt`, `briefs-followups.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: exact solvers, logs, proofs.
- `verifier/`: the referee's own solver and its tests of P6.
- `open-f-upper/`: the search for a 5M/3 manager in game F.
- `open-f-lower/`: the search for an adversary that beats every manager in F.
- `prior-art.md`: the literature search.

Binaries and four dump files over 300 kB are left out.
