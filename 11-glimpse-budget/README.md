# Experiment 11: a budget of glimpses

A cop chases a robber he cannot see on a complete binary tree. He may look a
limited number of times. One kind of look tells him exactly where the robber
is; a weaker kind tells him only which way to go. How many looks does each
kind need, and when does the weaker kind start to cost more?

**Short answer: at height 3, and the gap grows without bound.** On the tree
of height h ≥ 3 the cop needs exactly h − 1 direction glimpses, or exactly
⌊h/2⌋ exact glimpses. For h ≤ 2 he needs none of either. The chain guessed
the two would first differ at height 4 or 5. Proved by hand for every
h ≥ 3, resting on two small computed base cases; the blind win at height 2 is
a checked strategy; computed exhaustively for h ≤ 8.

## The game

T_h is the complete binary tree of height h. The cop picks a start vertex;
the robber, seeing it, picks a different one, which the cop is not told. Each
round:

1. **Glimpse (optional).** If the cop has glimpses left he may spend one. In
   game E he learns the robber's vertex. In game D he learns only which
   neighbour of his own vertex lies towards the robber.
2. **Cop move.** He stays or moves along an edge. Landing on the robber is a
   capture.
3. **Robber move.** She stays or moves along an edge. Landing on the cop is a
   capture.

The robber sees everything and knows the cop's strategy, which is
deterministic. The cop wins if some strategy guarantees capture within a
fixed number of rounds. d(h) and e(h) are the least numbers of glimpses with
which he wins games D and E.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): find
the least h with d(h) > e(h), and formulas for both.

## Results

| Claim | Status | Where |
|---|---|---|
| The game is equivalent to one on the set of vertices where the robber could be, with an adversary choosing the glimpse answers | proved by hand (both solvers use it); referee read it and tested it against brute-force enumeration of robber paths on T_3, T_4 | `solver-a/proofs.txt` §0 |
| d(h) ≤ h − 1 and e(h) ≤ ⌊h/2⌋ for h ≥ 1; both are 0 for h ≤ 2 | the two inequalities proved by hand, twice; referee read both. The blind win at h = 2 is an explicit strategy file checked by program, not a written argument | `solver-a/proofs.txt` §2, `solver-b/proofs.txt` U1, U2 |
| A cop with no glimpses never wins on T_h for h ≥ 3 | proved by hand by solver A; referee read it and tested it for h ≤ 8. It also follows from solver B's projection lemma and its certificate for height 3 | `solver-a/proofs.txt` §3 |
| d(h) ≥ h − 1 for h ≥ 3 | proved by hand, twice, given one computed base case (no win with 1 direction glimpse on T_3; certificate of 250 states, checked by solver A's checker and the referee's; solver B has its own certificate); referee read both | `solver-a/proofs.txt` §4, `solver-b/proofs.txt` L1, L2 |
| e(h) ≥ ⌊h/2⌋ for h ≥ 3 | proved by hand by solver A only, given one computed base case (8 starting positions on T_3, whose closure is 143 positions); solver B looked for this proof and did not find one; referee read it line by line and tested the seeded blind lemma and the choice of answer on 1.9 million positions, the induction claim for h ≤ 8 with up to 3 glimpses, and the projection lemma on random positions; I read it once | `solver-a/proofs.txt` §0, §1, §3, §5; `solver-a/lemma_check_rerun.txt` |
| **d(h) = h − 1 and e(h) = ⌊h/2⌋ for all h ≥ 3; the first difference is at h = 3** | follows from the rows above | |
| Table for h ≤ 8 (below) | computed exhaustively by both solvers and the referee, three separate programs | `reports/` |
| d(9) = 8, e(9) = 4 | e(9) computed by solver A and the referee; d(9) reported by solver A only, with no log kept | |

| h | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|---|
| d(h), direction | 0 | 0 | 0 | 2 | 3 | 4 | 5 | 6 | 7 |
| e(h), exact | 0 | 0 | 0 | 1 | 2 | 2 | 3 | 3 | 4 |

## How the cop uses a glimpse

Say the cop stands at a vertex with the robber somewhere strictly below him.

- A direction glimpse names the child to step to. After the step the robber
  is caught or strictly below that child. One glimpse, one level.
- An exact glimpse showing the robber two or more levels down names two
  steps. After the first, the robber has moved at most one edge and is still
  inside the subtree of the vertex two steps down the path; after the second
  step she is caught or strictly below that vertex. One glimpse, two levels.
- When only two leaves hang below him, he needs no glimpse: step to one leaf,
  back, and to the other.

From the root that is h − 1 direction glimpses or ⌊h/2⌋ exact ones (h ≥ 1).

## Why he cannot do with fewer

**Without glimpses he learns nothing useful.** On a tree of height at least
3, however a blind cop walks, every vertex at least two levels above the
leaves, other than his own, remains a possible robber position for ever. The
robber can always re-enter a vertex the cop has just left, from a neighbour
he could not also be covering. For a vertex just above the leaves that
neighbour may have to be one of its children, and showing that one is still
possible takes a separate argument about when the cop last visited the leaves
below. That is the delicate part of the lemma.

**Direction glimpses.** The cop's walk before his first glimpse is fixed in
advance. Of the two subtrees under the root, take the one that does not
contain him at that moment, and consider a robber who never leaves it. The
answer "towards that subtree" is forced and tells him nothing about where in
it she is. So he has spent a glimpse and must still win on a tree one level
shorter with one glimpse fewer. That is an induction on the height, started
from the computed fact that one direction glimpse does not suffice at
height 3.

**Exact glimpses.** Here an answer does carry information, so the adversary
must choose it. By the blind lemma there is always a possible robber position
u at distance at least 2 from the cop, heading a subtree that the cop is
outside and that is at most two levels shorter than the one before. The
adversary answers u. After the cop's move the robber may be at u or next to
it, which is again a position of the same kind, with one glimpse spent and at
most two levels gone. "Of the same kind" includes that the subtree has a
sibling of equal height. A version of the blind lemma that starts from such a
position, instead of from complete ignorance, keeps the induction going while
the subtree has height at least 3. At height 2 it does not apply, and the
argument ends on the computed base: the subtree, its sibling and their parent
form a T_3, where the eight starting positions are checked by program.

## What to distrust

- **Everything is by one model family.** Nothing is machine-checked as a
  proof.
- **The lower bound for exact glimpses has one author.** Solver B tried and
  failed to prove it; the referee and I have read solver A's proof and the
  referee tested each lemma by program, which checks the statements at small
  sizes and not the argument at general height. It is the one general claim
  here not proved twice.
- **Two computed base cases** sit under the lower bounds: the 250-state
  certificate for one direction glimpse on T_3, and eight blind positions on
  T_3.
- **Small errors.** One of solver B's deliberately broken test files is in
  fact a valid strategy, and its log records only one of the four rejections
  its report claims. Solver A's lemma test does not cover clause (K) of the
  blind lemmas, which the referee's does; its output was not saved, and
  `solver-a/lemma_check_rerun.txt` is my re-run of it. Loose sentences in
  solver A's proof file are listed in a note at its top.
- **Three certificate files over 300 kB** are left out of this folder (the
  height-6 losing sets and one height-5 set).
- **Prior art** ([prior-art.md](prior-art.md)): cop-and-robber games with no
  visibility, with visibility up to a fixed distance, and with a sighting
  every k steps are all in print. The search did not find a counted budget of
  glimpses, or a direction-only glimpse for a moving target. That a blind cop
  wins at height 2 and loses at height 3 is probably a special case of the
  known description of trees a single blind cop can search; the search could
  not open that paper to confirm. The pathwidth of the complete binary tree
  of height h is ⌈h/2⌉, which resembles e(h); no link between the two was
  established. The search read one source in full, a homework sheet.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files, with most of the
effort asked for on the blind lemmas and the lower bound for exact glimpses.
All five confirmed the reduction to belief sets, both upper bounds, both
blind lemmas, the induction for exact glimpses, and both proofs of the
direction lower bound. Their findings, applied above:

- "Proved by hand" was wrong for the blind win at height 2, which is a
  checked strategy file.
- "Tested every lemma on 1.9 million states" lumped together tests of
  different sizes.
- My account of the exact-glimpse lower bound left out the sibling subtree
  and the hand-over to the computed base at height 2, and my account of the
  blind lemma left out its delicate case.
- My description of the two-step descent pointed at the wrong vertex.
- Solver A's lemma-check output was cited but not saved. Re-run and saved.

The corrected wording has not been through a second round.

## Files

- `review/`: the review of this experiment before publication (16 to 20 November), archived on 21 November verbatim with absolute paths shortened: the brief given to the reviewers, each seat's report, the machine-made summary, and the organiser's note of what was done with the findings. The reports were written about the files as they stood before the findings were applied, so their line numbers and quotations may not match the current files.
- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: solvers, strategies, losing certificates,
  checkers, proofs.
- `verifier/`: the referee's own solver, checker and lemma tests.
- `prior-art.md`: the literature search.

Binaries are left out.
