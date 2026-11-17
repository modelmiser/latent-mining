# Experiment 8: the patient automaton

Two players steer a machine whose state is unknown. One wants to pin the state
down and sees the other's moves late. Does seeing them arbitrarily late still
beat not seeing them at all?

**Short answer: yes.** There is a 4-state automaton on which the Synchronizer
wins at every finite delay and loses when blind. No automaton with 3 or fewer
states does this. The 4-state example is proved by hand and checked by three
programs; "none smaller" rests on exhaustive computation.

## The game

A is a complete deterministic automaton with states 1 … n and two letters a, b.
Each letter is any map on the states. For a word v, Q·v is the set of states
the automaton can be in after v, starting from any state.

In rounds t = 1, 2, … the Synchronizer picks a letter x_t, then the Spoiler
picks y_t. The word played is x_1 y_1 x_2 y_2 ….

- **Delay d.** The Synchronizer's letter x_t may depend only on his own earlier
  letters and on y_1 … y_(t−1−d). The Spoiler knows the Synchronizer's strategy
  and sees everything.
- **d-winnable.** Some strategy and some T make Q·(x_1 y_1 … y_(T−1) x_T) a
  single state against every Spoiler play.
- **Blind-winnable.** One word x_1 … x_T, fixed before play, does that against
  every Spoiler play.

W_d(n) and W_inf(n) are the sets of d-winnable and blind-winnable automata, so
W_0 ⊇ W_1 ⊇ … ⊇ W_inf. An automaton is **patient** if it is d-winnable for
every d but not blind-winnable.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): does a
patient automaton exist, and how small?

## Results

| Claim | Status | Where |
|---|---|---|
| The automaton a = (2,2,1,3), b = (4,3,1,1) is d-winnable for every d | proved by hand; referee read it; I checked it line by line; strategy played against every Spoiler play for small d by both solvers' checkers and the referee's (d ≤ 12) | `solver-a/proofs.txt` Theorem 1 |
| The same automaton is not blind-winnable | proved by hand (four families of pairs closed under either letter); referee read it; I checked it line by line | `solver-a/proofs.txt` Theorem 2 |
| A second automaton, a = (1,1,2,3), b = (4,1,3,4), is patient | proved by hand by the other solver; referee read it and found one misstated step that does not affect the conclusion | `solver-b/proofs.txt` Theorem 1 |
| **A patient automaton with 4 states exists** | follows from the rows above | |
| No patient automaton has 3 or fewer states | computed: W_0 = W_inf for n = 2, 3, exhaustively, by three independent programs | tables below |
| Patient automata exist for every n ≥ 4 | proved by hand (add states that copy an existing one); referee read it | `solver-a/proofs.txt` Corollary |
| Exactly 96 of the 65536 four-state automata are patient: two classes of 48 under renaming states and swapping letters | computed by three programs; each class has a hand proof from one solver | `reports/referee.md` |
| Table of \|W_d(n)\| for n ≤ 5 | computed; three independent programs agree | below |
| \|W_d(6)\| | computed by one solver only | `solver-b/n6_sym.txt` |
| Whether the 170 further classes at n = 5 that survive to d = 13 are patient | open | |

Number of d-winnable automata, by exhaustive search over all n^(2n) automata:

| d | n = 2 | n = 3 | n = 4 | n = 5 | n = 6 (one solver) |
|---|---|---|---|---|---|
| 0 | 12 | 405 | 28960 | 3645945 | 708201936 |
| 1 | 12 | 405 | 27952 | 3318105 | 601411536 |
| 2 | 12 | 405 | 27808 | 3253305 | 578288016 |
| 3 | 12 | 405 | 27808 | 3243465 | 572993136 |
| 4 | 12 | 405 | 27808 | 3241545 | 571486896 |
| 5 | 12 | 405 | 27808 | 3240825 | 570912336 |
| 6 | 12 | 405 | 27808 | 3240105 | 570537936 |
| blind | 12 | 405 | 27712 | 3199305 | 556677936 |
| all automata | 16 | 729 | 65536 | 9765625 | 2176782336 |

For n = 4 the count stays at 27808 for every d ≥ 2: the 96 automata in the gap
are the patient ones. For n = 5 it stays at 3240105 from d = 6 to at least
d = 13.

## Why the example works

After the Synchronizer opens with b, and as long as he then plays a, the
Spoiler can keep two states alive only by walking a cycle of three pairs:
34 → 14 → 13 → 34. Any other choice collapses the pair. The letter b collapses
the pair 34 and no other. So the Synchronizer needs to know the phase of the
cycle, and the phase is fixed by the Spoiler's first letter.

- At delay d the Synchronizer plays a until the Spoiler's first letter becomes
  visible, then waits for the right round mod 3 and plays b. Waiting costs
  nothing, because a never lets the Spoiler leave the cycle.
- A blind Synchronizer must fix in advance the rounds on which he plays b. The
  Spoiler, who sees that word, picks the first letter that puts the cycle out
  of phase with it.

So the whole advantage is one bit of information, and any finite delay
eventually delivers it.

## What to distrust

- **Everything is by one model family.** "Proved by hand" means one instance
  wrote it and others read it. Nothing is machine-checked as a proof, though
  the strategy and the blind certificate are checked by programs.
- **"None with 3 states"** is a computation, not a proof. Three programs
  written separately agree on it.
- **The n = 6 column** comes from one program and was not recomputed. The
  n = 5 column was recomputed by the referee using the inclusions
  W_inf ⊆ W_(d+1) ⊆ W_d to skip work.
- **Solver B's proof** has a false intermediate statement in Lemma A (for
  d = 0, 1) and an off-by-one in a remark about T. Both are listed in
  `reports/referee.md`; the proof file is left as written.
- **Prior art** ([prior-art.md](prior-art.md)): the search found the game with
  no delay (Fominykh, Martyugin, Volkov) and general games under delayed
  control, and did not find this result. It read no source in full. A general
  theorem in the delayed-control literature may imply that the counts stop
  changing in d; whether "wins at every delay but not blind" is known there as
  a phenomenon was not settled.

## Files

- `chain/`: the seed and the three links, verbatim, with the question each
  link handed on.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: solvers, witnesses, checkers, proofs.
- `verifier/`: the referee's own programs.
- `prior-art.md`: the literature search.

Binaries are left out.
