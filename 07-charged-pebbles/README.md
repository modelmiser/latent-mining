# Experiment 7: charged pebbles

The reversible pebble game on a line, where a pebble may be put down or taken
up only next to another pebble. Give each pebble the right to be taken up the
forbidden way a fixed number of times. How far can a single pebble be
delivered, leaving nothing else on the board?

**Short answer: exactly (pk + 2)·2^(p−2)**, with p pebbles that may each be
forgotten k times. No forgetting ever buys more than 2^(p−2) nodes, however
it is spent, and the strategy gets exactly that from each one. Proved by hand
for every p and k: the
strategy by both solvers, the impossibility by one follow-up instance and read
by one other. Not machine-checked. Exhaustive search confirms it in every case
reached.

**Added after a deeper literature check (2026-11-17).** The upper bound here
is not new. It follows in a few lines from a recurrence in a preprint posted
a month before this experiment: Thumann and Soloveichik, "Energy-optimal
scheduling for clocked reversible computing", arXiv:2610.24435 (17 Oct 2026),
Theorem 1, together with Knill's 1995 theorem on the fully reversible game.
That paper studies the same game with a price on erasures, does not state the
closed form, and proves its recurrence by a different method. What remains
unaccounted for in what was read: the closed form as a stated result, the
bound on the highest node ever pebbled, the per-pebble version, and the proof
by anchors given here. Details in [prior-art-deep.md](prior-art-deep.md) and
under "What to distrust".

## The game

Nodes 1, 2, 3, … on a line; node 0 always counts as pebbled. You hold p
pebbles, each with a charge, initially k. A move is one of:

- **Place** a pebble from your hand on an empty node whose predecessor is
  pebbled.
- **Remove** a pebble whose predecessor is pebbled.
- **Forget**: remove a pebble whose predecessor is empty. Its charge must be
  at least 1 and drops by 1.

A board is clean at n if node n is the only node pebbled. C(p, k) is the
largest n for which some sequence of moves reaches a board clean at n.

With k = 0 this is Bennett's reversible pebbling, where C(p, 0) = 2^(p−1).

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): is
C(p, k) = (pk + 2)·2^(p−2) for all p ≥ 2 and k ≥ 0?

## Results

The relaxed game drops the charges: q pebbles and a pooled budget of f
forgettings. Every play of the real game is a play of the relaxed game with
q = p and f = pk.

| Claim | Status | Where |
|---|---|---|
| C(p, k) ≥ (pk + 2)·2^(p−2) for all p ≥ 2, k ≥ 0 | proved by hand, by each solver with a different rotation rule; referee read both; 39 move lists replayed by an independent checker | `solver-a/proofs.txt` P1, `solver-b/proofs.txt` Theorem 1 |
| C(2, k) = 2k + 2 | proved by hand, twice; referee read both | `solver-a/proofs.txt` P2, `solver-b/proofs.txt` Theorem 2 |
| C(p, 0) = 2^(p−1) | proved by hand; known (Li, Tromp, Vitányi 1998) | `solver-b/proofs.txt` Lemma W |
| In the relaxed game, after f forgettings a clean board has n ≤ (f + 2)·2^(q−2), and no pebble is ever above (f + 4)·2^(q−2) − 1 | proved by hand by one follow-up instance; read line by line by a second instance, which found no gap and tested every lemma with its own program for q ≤ 6; I read it once and checked the arithmetic of Part 3, not every case of Lemma Step (no record of my reading is kept here) | `open-upper/proofs.txt`, `reports/open-upper-referee.md` |
| **C(p, k) = (pk + 2)·2^(p−2) for all p ≥ 2, k ≥ 0** | follows from the first and fourth rows | |
| Exact values for p = 2 (k ≤ 10, 15), p = 3 (k ≤ 8, 15), p = 4 (k ≤ 6), p = 5 (k ≤ 2) | computed by exhaustive search of the real game; each value by at least one solver, both agreeing wherever both ran (solver A lacks (2,6), (2,7), (2,9), (2,10), (3,7); solver B lacks (2,15), (3,15)); referee recomputed all but (4,5), (4,6), (5,2) | `reports/` |
| Relaxed game exact for q = 2 (f ≤ 100), 3 (f ≤ 60), 4 (f ≤ 40), 5 (f ≤ 25) | computed by solver B and the referee | `solver-b/rel_q*.txt` |
| With unequal starting charges the reach is not a function of total charge | computed in seven cases; no formula | `reports/solver-a.md` |
| The j-th lowest anchor after f ≥ 1 forgettings is at most (f + 3)·2^(q−2) − 2^(q−1−j) | tested for q ≤ 5, proved only for f = 1; not needed | `open-upper/proofs.txt`, last section |

The highest node ever pebbled on the way is C(p, k) + 2^(p−1) − 1 in every
computed case.

## How the strategy works

Deliver a pebble cleanly to node 2^(p−1) with no forgetting. Then repeat: from
a lone pebble at m, use the other p − 1 pebbles reversibly to put a pebble at
m + 2^(p−2), and forget the one at m. Rotate which pebble is left behind so
that every pebble is forgotten exactly k times. That is pk rounds of 2^(p−2)
nodes each.

## Why nothing does better

The proof stops looking at single pebbles. Two boards are equivalent if
reversible moves alone lead from one to the other. Each class has a smallest
member, found by repeatedly deleting any pebble that lies within 2^(number of
free pebbles) of the pebble below it; what remains are the **anchors**,
pebbles too far apart to be recovered without forgetting. Reversible moves
never change the anchors.

A forgetting can change them. The key lemma says how: for boards with at most
one anchor, after any pebble is forgotten the new anchors are either none, or
some board of the old class with one pebble shifted, forward or back, by at
most half the reach of the free pebbles and without passing a neighbour. From
that follows an invariant bounding the lowest and the highest anchor after f
forgettings. Any deletion preserves it, from every board that satisfies it,
reachable or not. A lone pebble beyond 2^(p−1) is its own anchor, which gives
the bound.

The proof never uses the rule that a forgotten pebble must have an empty
predecessor. The bound holds even if a "forgetting" may remove any pebble.

## What to distrust

- **Everything is by one model family.** The upper bound was written by one
  instance and read by one other instance of the same model. Its author rates
  the case analyses of Lemma N and Lemma Step as the place an error would
  hide; the reader followed them and tested each branch by program up to
  q = 6, which checks the statements at small q and not the argument at
  general q. Nothing is machine-checked as a proof.
- **Prior art, superseded in part.** A second, deeper search
  ([prior-art-deep.md](prior-art-deep.md)) read the classical papers'
  erasure sections and traced their citations. It found the preprint named at
  the top of this page. Letting the erasure price go to infinity in its
  Theorem 1 gives a recurrence for the least number of erasures,
  I(n, S) = min over m of I(m, S) + I(n − m, S − 1) + [m > 2^(S−2)], and an
  induction on S and n gives I(n, S) ≥ ⌈n/2^(S−2)⌉ − 2, which is the upper
  bound of this experiment. That derivation is by the search instance; I
  checked the induction by hand and read the preprint's game definition and
  Theorem 1, not its proof. The preprint's erasure rule is more permissive
  than the one here, so its bound transfers. The search estimates the closed
  form is probably known to that group even though it is not stated. The
  bound on the highest node ever pebbled and the per-pebble version were not
  found. The paragraph below is the first, shallower search and is out of
  date where it says no upper bound was found.
- **Prior art, first search** ([prior-art.md](prior-art.md)): the game with a pooled budget
  of erasures is Li and Vitányi's "m-erasure pebble game" (1996). The search
  found published strategies that gain the same 2^(p−2) per erasure and reach
  (f + 1)·2^(p−2), one block short of the value here, and no upper bound for
  the game with erasures. It read no source in full and did
  not trace later citations, so an existing upper bound is possible.
- **The per-pebble charge turned out not to matter** for equal charges: the
  pooled budget gives the same values. It does matter for unequal charges
  (charges 3, 0, 0 reach 6 where 1, 1, 1 reach 10), and solver B's remark
  that "the only binding constraint is the total" is true only for equal
  charges.
- **Largest searches** ((4,5), (4,6), (5,2) at over 100 million states, and
  solver B's q = 6 relaxed run) were not repeated by the referee.
- **Two certificates over 300 kB** (the full reachable sets for p = 4, k = 1)
  are left out of this folder; the smaller ones are included.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files, with most of the
effort asked for on the upper-bound proof. All five confirmed Lemma N, Lemma
Step, the Shape Lemma, Theorem I, the deduction of the Main Theorem, both
lower-bound proofs and both two-pebble proofs. Four confirmed Part 1 as
written; Grok marked it refuted on two omissions, both real and both
notational: the pebbles above the highest anchor are bounded by the same
argument as the others but the text defines no interval for them, and "every
board on the path contains the anchors" is used without pointing back to the
sentence that gives it. Those and other loose sentences are listed in a note
at the top of `open-upper/proofs.txt`; the body is left as written.

Corrections to this page from the review: an earlier draft said each
forgetting buys exactly 2^(p−2) nodes "however spent" (only the upper bound
holds for arbitrary spending), said the published strategies reach half as
far per erasure (they are one block shorter), said both solvers computed
every table entry, and described the key lemma as a forward slide only.

The corrected wording has not been through a second round.

## Files

- `review/`: the review of this experiment before publication (16 to 20 November), archived on 21 November verbatim with absolute paths shortened: the brief given to the reviewers, each seat's report, the machine-made summary, and the organiser's note of what was done with the findings. The reports were written about the files as they stood before the findings were applied, so their line numbers and quotations may not match the current files.
- `chain/`: the seed and the three links, verbatim, with the question each
  link handed on.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`,
  `brief-open-upper.txt`: what each instance was told. The reader of the
  upper-bound proof was told to assume it wrong, follow every step, test each
  lemma with its own program, and confirm its tests can fail; that brief was
  not saved as a file.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: search programs, move lists, certificates,
  proofs.
- `verifier/`: the referee's own programs.
- `open-upper/`: the upper-bound proof and its tests.
- `open-upper-referee/`: the second reader's programs and logs.
- `prior-art.md`: the first literature search.
- `prior-art-deep.md`: the second, deeper search, which found the 2026 preprint.

Binaries are left out.
