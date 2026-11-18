# Experiment 12: the repairable loop

A circular string of bits contains every n-bit code as a window. An adversary
deletes one bit or doubles one. A mechanic may then make one edit of his own,
a deletion or a doubling. How long must the string be for the mechanic always
to be able to restore every code?

**Short answer: only 2 bits longer than the minimum for n = 3, 4, 5, and
then the cost takes off.** Writing M(n) for the shortest such string:
M(n) = 2^n + 2 for n = 3, 4, 5; that length fails for every n ≥ 6; and the
excess M(n) − 2^n is unbounded, growing at least like 2^n/(152n). The exact
value is unknown from n = 6 on, where 67 ≤ M(6) ≤ 76. The chain guessed that
the excess is unbounded, which is now proved, and that it is a constant
fraction of 2^n, which is open.

## The game

A loop is a cyclic binary word. It is universal (for n) if every one of the
2^n words of length n appears as a window, wrapping around. Two edits are
allowed on a loop: **loss** of a cell, and **doubling** of a cell (a copy is
inserted next to it).

1. The designer picks a loop w.
2. The adversary, seeing w, makes one edit or passes.
3. The mechanic, seeing both, makes one edit or passes.

The designer wins if the result is universal. w is repairable if the
mechanic can always win. M(n) is the least length of a repairable loop.

A de Bruijn cycle is a universal loop of the least possible length, 2^n.
Written twice around the loop it survives any single edit, so M(n) ≤ 2^(n+1).

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): find
M(n), and in particular decide whether M(n) − 2^n stays bounded.

## Results

| Claim | Status | Where |
|---|---|---|
| M(1) = M(2) = 4; M(3) = 10, M(4) = 18, M(5) = 34 | computed exhaustively by both solvers and the referee, three separate programs | `reports/` |
| 2^n + 2 ≤ M(n) ≤ 2^(n+1) for n ≥ 3 | proved by hand; the lower bound twice, by counting runs (solver A, all n ≥ 3) and by counting single-cell runs (solver B, n ≥ 6); referee read both | `solver-a/proofs.txt` P1, P4, P5; `solver-b/proofs.txt` Theorems 1, 2 |
| A repairable loop is universal, or a universal loop with one cell inserted, or one with a cell removed | proved by hand by solver B; referee read it and used it | `solver-b/proofs.txt` Lemma 5 |
| No loop of length 66 is repairable for n = 6, so M(6) ≥ 67 | computed twice in full: solver A's pruned search (its pruning rule argued by hand and tested against unpruned search on smaller cases) and the referee's separate enumeration of 7.3 billion candidates through the row above. Solver B's search covers universal loops only | `reports/referee.md` |
| M(6) ≤ 76 | an explicit loop, checked from the definition against all 153 adversary moves by the referee | `solver-b/witnesses.txt` |
| **For every n ≥ 7 no loop of length 2^n + 2 is repairable**; with the computed n = 6, M(n) ≥ 2^n + 3 for all n ≥ 6 | proved by hand by one follow-up instance; read line by line by a second instance, which re-enumerated every forced reply and found no gap; I read it once | `open-growth/proofs.txt` Theorem 1 |
| **M(n) − 2^n is unbounded**: M(n) ≥ 2^n + (2^(n−3) − 19n²)/(19n) | proved by hand by the same instance; read by the second, which found no gap after supplying one missing sentence; I followed the key lemma and not the final count. The underlying inequality gives nothing new below n = 15 and the closed formula nothing below n = 16, so it cannot be tested end to end; its lemmas were tested for n ≤ 10 | `open-growth/proofs.txt` Theorem 2, `reports/open-growth-referee.md` |
| Whether M(n) − 2^n is of order 2^n/n, of order 2^n, or between | open | |
| Any upper bound below 2^(n+1) for general n | open; nothing found | |

What the unboundedness theorem gives numerically, from its inequality with
the best window length (the closed formula above is looser): nothing new up
to n = 14; an excess of at least 9 at n = 15, at least 418 at n = 20, at
least 272,775 at n = 30. For n = 7 … 14 the best proved lower bound is
2^n + 3.

Shortest repairable loops:

    n = 3:  0001011101
    n = 4:  000011110101011001
    n = 5:  0000011100110001010111110110101001

## Why 2 extra bits work for small n

Most damage is undone exactly. A doubling is undone by a loss, and a loss
inside a run of two or more equal bits is undone by a doubling. The only edit
the mechanic cannot reverse is the loss of a lone bit, one whose two
neighbours both differ from it, because no edit splits a run. So a loop that is already
universal is repairable exactly when losing any one lone bit leaves it one
edit away from universal.

For n = 4, 5 every shortest loop is a de Bruijn cycle with one alternating
pair (a 0 and a 1) added, and in each of them every lone bit that has runs of
two or more on both sides sits between the longest run and a run of exactly
two (checked on all such loops: 216 strings for n = 4, 20 necklaces for
n = 5).

## Why that stops at n = 6, and why the cost is unbounded

Both proofs count windows shorter than n. If a loop is universal, every word
of length k ≤ n appears at least 2^(n−k) times; one edit creates at most
k − 1 new windows of length k.

**Length 2^n + 2 fails (n ≥ 7).** Take windows of length 3. Four adversary
moves, each answered, pin down exactly how many times each 3-bit word occurs
in a repairable loop of this length. From that, two of the mechanic's replies
are forced, and the loop must be a de Bruijn cycle in which one lone 0 has
been lengthened to 00 and a 1 has been inserted into a run 00. (The short
loops for n = 4, 5 are the special case where these happen at the same
place.) But a de Bruijn cycle
for n ≥ 7 contains 0001000: a lone 1 between two runs of three or more
zeros. Deleting it merges those runs, and the forced reply cannot give the
run lengths a universal loop must have. For n = 6 the word 0001000 does not
fit and the proof does not apply; that case is the computation.

**Unbounded excess.** Take a lone bit s and the k windows of length k through
it. Deleting s destroys those k windows. Unless one of the windows formed
across the gap happens to spell one of the destroyed words, the mechanic's
single edit, which creates at most k − 1 windows, cannot replace them all. So
one of the words through s must have been present more often than 2^(n−k)
times to begin with. A repairable loop of length 2^n + e has few windows
carrying such over-represented words, at most (2^(n−k) + 1)(e + k − 1), and
each window contains at most k lone bits. A repairable loop has at least
2^(n−2) − 2 lone bits. The lone bits where a coincidence "across the gap" can
happen are counted separately: with k = n − 4 there are at most about 2^(n−3)
of them, plus terms of order n·e. That leaves about 2^(n−3) lone bits needing
an over-represented window each, less the same lower-order terms. Hence e is
at least of order 2^n/n.

The solvers' own attempts worked with k = n and stalled because the
coincidences cannot be bounded there. Shortening the window by four is what
made the count go through.

## What to distrust

- **Everything is by one model family.** The two new theorems were written
  by one instance and read by one other instance of the same model. Nothing
  is machine-checked as a proof.
- **The unboundedness bound has no numerical check.** It gives nothing new
  below n = 15, far beyond any search, so it rests on reading plus tests of each
  lemma at small n. The second reader had to supply one sentence in the
  coincidence count (that a certain condition excludes exactly half the
  cases), and found one constant looser than what is proved.
- **The follow-up instance made and caught an error on the way.** Its first
  formula for what a doubling destroys was wrong (k − 1 windows instead of
  k − 2); its own test found it and the proof uses the corrected form.
- **n = 6 is computation only.** Two computations cover all loops of length
  66. One rests on a pruning rule tested only on smaller cases, the other on
  the structural lemma (row three of the table) and a run-count lemma of the
  referee's own.
- **Nothing is known from above.** The only general construction is the
  doubled de Bruijn cycle. The length-76 loop for n = 6 came from a heuristic
  search that missed the true optimum by one at n = 5.
- **Prior art** ([prior-art.md](prior-art.md)): sequences built from de
  Bruijn-like windows are used as codes against exactly these two errors
  (deletions and doubled symbols) in work on racetrack memory (Chee, Etzion,
  Kiah, Vardy, Vu, Yaakobi 2020). The search did not find the question of
  keeping one cycle universal after an edit, or the function M(n). It read
  no source in full.
- **A second, deeper search** ([prior-art-deep.md](prior-art-deep.md))
  extracted the full text of nine of the nearest papers and inspected every
  mention of deletions, insertions and duplications. It found no paper that
  asks for one cycle that stays or can be made universal after an edit, and
  none of the results here except the classical run counts for de Bruijn
  cycles (Golomb) and the one-line upper bound. Its estimate is 5 to 10
  percent that the two general theorems are in print. It also rates the
  interest as mild: a clean extremal question with no application yet and no
  good upper bound. No paper was read line by line, and paywalled sources
  were not reached.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files, with most of the
effort asked for on the two new theorems. All five confirmed every step of
both: the forced structure and the contradiction for length 2^n + 2, the key
lemma, the coincidence count with the second reader's repair, and the
derivation of the closed bound, which several recomputed. GPT marked the
counting tools refuted on one point, a run-count statement that is false for
n = 1, where it is never used.

Every reviewer found fault with this page, and all of it was mine:

- I wrote the growth rate as 2^n/(19n). The theorem gives 2^n/(152n); I
  dropped a factor of 8. All five caught it.
- I wrote that length 66 was excluded three ways. One of the three covers
  universal loops only.
- I wrote that the coincidences number fewer than 2^(n−3). The proved count
  is slightly more than that; the argument is unaffected because it compares
  with 2^(n−2).
- I quoted the numbers at n = 15, 20, 30 as values of the closed formula.
  They come from the sharper inequality.
- My definition of a lone bit was garbled, and I said the mechanic's edit is
  "of the same kind" as the adversary's when it may be either.

Loose statements in `open-growth/proofs.txt` are listed in a note at its top.
The corrected wording has not been through a second round.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`,
  `brief-open-growth.txt`: what each instance was told. The reader of the
  growth proofs was told to assume them wrong, follow every step, test each
  lemma with its own program including on non-universal repairable loops,
  and recompute the quoted numbers; that brief was not saved as a file.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: searches, witnesses, strategy tables, proofs.
- `verifier/`: the referee's programs, including the length-66 enumeration.
- `open-growth/`: the two new theorems and their tests.
- `open-growth-referee/`: the second reader's programs and outputs.
- `prior-art.md`: the first literature search.
- `prior-art-deep.md`: the second, deeper search.

Binaries are left out.
