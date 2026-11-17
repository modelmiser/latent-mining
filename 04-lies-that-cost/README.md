# Experiment 4: lies that cost

Twenty questions against an answerer who may lie as often as he likes, except
that each lie must be followed by a stretch of honesty, and the stretch is
longer the more evenly the question he lied to split the remaining
candidates. Is that any harder for the questioner than an answerer who may
lie only once in the whole game?

**Short answer: no, provided the stretch is at least twice the smaller side
of the split.** Then the questioner needs exactly as many questions as
against a single lie, for every number of candidates. With the stretch equal
to the smaller side, the questioner cannot win at all for seven small
numbers of candidates. The first statement is proved by hand apart from a
finite table of small positions and three small starting cases, which are
settled by program; the second is computed.

## The game

A secret x is one of 1 … n. The questioner asks "is x in S?" for sets S of
her choice, adaptively. The answerer is adversarial and need not fix x in
advance. Fix k ≥ 1.

A candidate is alive if some pattern of lies explains all the answers so far.
For a question S, let ell be the smaller of the numbers of alive candidates
inside and outside S. If the answer to question s is a lie (for a given
candidate), then for that candidate none of the next k·ell answers may be a
lie.

The questioner wins when one candidate is alive. Q_k(n) is the least number
of questions with which she can always win (infinite if she cannot). U1(n) is
the same for an answerer allowed one lie in the whole game.

P(n) is Pelc's formula for U1(n): the least q with n(q + 1) ≤ 2^q for even n,
and n(q + 1) + (q − 1) ≤ 2^q for odd n.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): is
Q_2(n) = U1(n) for every n ≥ 2?

## Results

| Claim | Status | Where |
|---|---|---|
| The position of the game is the multiset, over alive candidates, of how many more honest answers each still owes | proved by hand, twice; referee read both and checked it against the literal definition on 393,216 exhaustive histories | `solver-a/proofs.txt` P1, `solver-b/proofs.txt` Lemma 1 |
| Q_k(n) ≥ U1(n) ≥ P(n) for all k, n | proved by hand, twice; referee read both | `solver-a/proofs.txt` P2, P3 |
| For k ≥ 2, from any position with m alive candidates the questioner wins within 3·⌈log2 m⌉ questions | proved by hand by one follow-up instance; read by a second; I checked it by hand | `open-upper/proofs.txt` Lemma C |
| A state with a free candidates, b that can never lie again, b ≥ a − 1, a(j + 1) + b ≤ 2^j and at least 2j − 2 candidates (and at least 2) has a question after which no one can lie twice, and both outcomes again have b ≥ a − 1 and satisfy the weight bound with j − 1 | proved by hand by the same instance; read by a second; I checked it by hand | `open-upper/proofs.txt` Lemma S |
| Table T1: the 960 remaining small states per k (j ≤ 14, fewer than 2j − 2 candidates) are all wins, for k = 2 … 13 | computed by two programs written separately (the follow-up's and its reader's); after any lie at j ≤ 14 at most 13 questions remain, so for k ≥ 13 the game is the k = 13 game | `open-upper/table_out.txt`; `open-upper-referee/run1.txt` for k ≥ 4 and `open-upper-referee/rerun-t1-k2-k3.txt` for k = 2, 3 |
| Table T2: Q_2(3) ≤ 5, Q_3(3) ≤ 5, Q_2(5) ≤ 6, the three starting cases the general argument does not cover | computed by both solvers, the referee and the follow-up | `open-upper/table_out.txt`, `solver-a/strategies/` |
| **Q_k(n) = U1(n) = P(n) for every k ≥ 2 and n ≥ 2** | follows from the rows above. Does not use Pelc's theorem; reproves its upper bound | `open-upper/proofs.txt` Theorems 1, 2 |
| Q_2(n) = P(n) for 2 ≤ n ≤ 2805 | computed directly by solver A; solver B to 1100; referee to 400 | `reports/` |
| Q_1(n) is infinite for n = 2, 3, 4, 5, 6, 7, 9 | computed by three programs as a fixed point, not a depth cutoff; n = 2, 3 also by hand | `solver-b/certs/`, `reports/referee.md` |
| Q_1(n) = P(n) for n = 8 and 10 ≤ n ≤ 700 | computed (referee to 700, solvers to 314 and 317); beyond that open | `reports/referee.md`, `solver-a/out_k1_*.txt` |
| For k ≥ 2, states with b < a − 1 are wins exactly when the one-lie game is | computed for a + b ≤ 110, j ≤ 12, k = 2, 3; not proved, not needed | `reports/referee.md` |

## Why repeated lies do not help

The questioner asks balanced questions. If a question splits the alive
candidates into sides of size at least ell, anyone who lies to it owes k·ell
honest answers. With k ≥ 2 and a balanced split of enough candidates, that is
at least as many answers as remain in the game, so a liar never lies again
and the game is the one-lie game. Lemma S shows such a question exists, and
keeps the standard weight bound, whenever the position has at least 2j − 2
candidates with j questions left, is within the weight bound, and has at
least as many spent candidates as free ones, less one (b ≥ a − 1). The
questioner's first question puts her in such a position and Lemma S keeps
her there.

That leaves positions with few candidates and many questions left, where a
lie is cheap and can be repeated. There the questioner has time to spare. She
asks the same half up to three times: a liar owes at least two honest answers,
so he lies at most once in the three and the majority answer is true for
everyone still alive (Lemma C). That halves the candidates every three
questions, which is fast enough once j ≥ 15. For j ≤ 14 the finitely many
such positions are checked by program.

With k = 1 a lie to a question with ell = 1 costs one honest answer and the
three-question trick fails. For n = 2, 3, 4, 5, 6, 7 and 9 the answerer can
then keep two candidates alive for ever; for n = 8 and every n from 10 to 700
the questioner still wins in P(n).

## What to distrust

- **Everything is by one model family.** The upper bound was written by one
  instance and read by one other instance of the same model. Nothing is
  machine-checked as a proof.
- **The proof is not all by hand.** Table T1 is a search by two programs.
  Both work on the reduced position (the multiset of obligations), which is
  itself a hand-proved lemma, checked against the literal definition
  exhaustively for n ≤ 4 and on random histories for n ≤ 8.
- **The second reader's program has a real bug outside the table.** Its
  memo key stores each count in one byte (`open-upper-referee/ref.cpp`, the
  `key` function), so counts that differ by 256 collide. Table T1 has at most
  25 candidates and is not affected. Its extra sweep "Q_k(n) = P(n) for
  n ≤ 300" is not trustworthy above n = 255; the solvers' and the referee's
  sweeps over that range are separate programs. Found by the GPT reviewer.
- **Some logs are thinner than the claims.** The reader's saved log lost its
  k = 2, 3 table runs to an overwrite; I re-ran them with its program
  (`rerun-t1-k2-k3.txt`, 960 states each, none lost). The follow-up's
  `table_out.txt` is filtered output.
- **Pelc's formula** is taken from the question as the chain wrote it. The
  literature search matched it to Pelc (1987) through secondary sources and
  did not read the paper. Nothing here depends on that: the lower bound
  U1(n) ≥ P(n) is proved in `solver-a/proofs.txt`, and the upper bound comes
  out of Theorem 2.
- **Prior art** ([prior-art.md](prior-art.md)): liar games with a fixed
  window ("at most one lie in any r consecutive answers", Czyzowicz,
  Lakshmanan, Pelc 1994) are in print, with asymptotic results. The search
  did not find a window that depends on the question. It read no source cover
  to cover, and the rule has no standard name to search for.
- **The rule is artificial.** The chain invented it to make lying cost
  something that depends on the question. The result says the invention has
  no effect for k ≥ 2, which is a statement about this rule and not about
  liar games in general.
- **The "no win with one question fewer" columns** in the solvers' sweeps
  are decided by the proved weight bound, so they restate the proof instead
  of testing it (noted by the referee).

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files, with most of the
effort asked for on the upper-bound proof. All five confirmed the reduction
to obligations, Lemma C, Lemma S, Theorem 1, Theorem 2 and the lower bounds.
Their findings were about evidence and wording, and are applied above:

- The saved log of the second program did not contain the k = 2 and k = 3
  table runs it was cited for. Re-run and saved.
- The byte-wide memo key in the second reader's program (GPT).
- The results table omitted the three starting cases settled by program.
- My explanation dropped two hypotheses of Lemma S and said "more answers
  than remain" where the proof gives "at least as many".
- The k = 1 sentence read as if the answerer always wins.

Loose sentences in `open-upper/proofs.txt` are listed in a note at its top;
the body is left as written. The corrected wording has not been through a
second round.

## Files

- `chain/`: the seed and the three links, verbatim, with the question each
  link handed on.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`,
  `brief-open-upper.txt`: what each instance was told. The reader of the
  upper-bound proof was told to assume it wrong, follow every step, recompute
  the table with its own program, and confirm its tests can fail; that brief
  was not saved as a file.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: solvers, strategy trees, trap sets, checkers,
  proofs.
- `verifier/`: the referee's own programs.
- `open-upper/`: the upper-bound proof, its table and tests.
- `open-upper-referee/`: the second reader's programs and logs.
- `prior-art.md`: the literature search.

Binaries are left out.
