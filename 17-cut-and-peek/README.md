# Experiment 17: computing AND with cards, one cut and a peek

Each of n players holds a secret bit and lays cards face down according to
it. The row is cut once at a random place that nobody sees, and then a fixed
set of positions is turned face up. From what is seen, everyone must learn
the AND of the bits and nothing else. For two players this is the well-known
five-card trick. What about three?

A standalone write-up of the main result, with the proof and its relation to
the literature, is in [NOTE.md](NOTE.md).

**Short answer.**

- **If every card is turned up, three or more players cannot do it, with any
  number of cards.** This is proved by hand below. Papers of 2025 on these
  "single-cut full-open" protocols list the three-player case as an open
  question; it is settled, with stronger results in their one-pair-per-bit
  model, by Nuida, Shinagawa and Numata (see Prior work below). The proof
  here was written by a model and checked by other models and in Lean 4; no
  person has checked it.
- **If only some cards are turned up, three players can do it with 36
  cards, and not with 15 or fewer.** That some number of cards suffices was
  already known (Eriguchi and Shinagawa, 2025, as read by the same instance);
  the numbers 36 and 16 were not found in print. The least number is somewhere from 16 to 36.

The chain bet that three players can never do it. That is wrong for partial
opening and right for full opening.

**Prior work (added 23 November 2026).** For protocols that encode each
input bit by one pair of cards, the impossibility results on this page for
full opening are contained in stronger results of Koji Nuida, Kazumasa
Shinagawa and Yasuhide Numata: "Impossibility Results for Single-Cut Full-Open
Card-Based Protocols with Any Number of Cards", Cryptology ePrint Archive,
Paper 2026/2960 (https://eprint.iacr.org/2026/2960), posted on 20 November
2026. According to one of the authors, who wrote to us on 23 November 2026,
their result for symmetric functions was obtained in January 2026, and it
includes the cases we had described to them (AND of three or more bits,
equality of four or more). Their work has priority. This page was written
before we knew of it and is kept as a record of the experiment; where it says
that a question is open, or that an answer "would settle" it, read that as
the state of our knowledge on 19 November.

In their model, their Theorem 1 says that a function of three or more bits
has no single-cut full-open protocol, with any number of helping cards, as
soon as some input and three of its neighbours have the same value. That
hypothesis is weaker than the seven-of-eight hypothesis used here, and their
functions may have more than two output values. Their proof also shows that
the words in question cannot be made cyclically equal by inserting helping
cards, which answers the more specific questions of the 2025 papers as well.
They then determine, in that model, exactly which symmetric Boolean
functions, and which Boolean functions depending on all their variables, have
such a protocol.

How the two relate, as read by AI models from the paper's text and not
confirmed by its authors. Their protocols encode each input bit by one pair
of cards; multiple pairs per bit are, in their words, "outside the scope of
this paper". The definition on this page allows a bit to control any number
of cards. For those wider encodings the statements here (for functions with a
seven-of-eight restriction only) are not stated in their paper. Their
Theorem 1 as stated does not carry over to the definition here: a bit that
controls no card gives a trivial counterexample (the five-card trick, read as
a function of four bits). Whether it holds when every bit controls at least
one card, we have not checked. In their model the hypothesis of the seven-rows statement here (second row of the table under Results)
(seven rows that are rotations of one word) cannot occur. Their proof of
Theorem 1 uses polynomials over the two-element field. The proof here uses
the discrete Fourier transform (evaluation at all m-th roots of unity); their
paper evaluates at one root of unity for its Theorems 2 and 3. The
partial-opening results on this page and the Lean 4 formalisation have no
counterpart there.

## The protocol

Fix n ≥ 2 players and m cards in a row, positions 0 … m − 1 taken modulo m.
The public data are: which positions belong to which player and which to the
house (every player has at least one); for each player two words of ♣ and ♥
to lay on their positions, one for bit 0 and one for bit 1; a fixed word for
the house positions; and a non-empty set R of positions to reveal.

Each player lays the word for their bit, face down. A shift j is drawn
uniformly from 0 … m − 1, nobody learns it, and every card moves j places
forward round the row. Then the cards now at the positions in R are turned up.

For an input x, let M(x) be the collection, with repetition, of the m words
that can be seen as j varies. The protocol is valid if M(x) is the same for
every input other than all-ones (privacy), and no word seen on the all-ones
input is ever seen on the all-zeros input (correctness).

r(n) is the least m with a valid protocol, and f(n) the same when all cards
must be revealed (R is everything). The question, as the chain left it
([chain/3-link.md](chain/3-link.md)): find r(3).

This is the model of Definition 3.1 in Eriguchi and Shinagawa
(arXiv:2510.17308) with the shuffle fixed to one random cut and the function
fixed to AND, according to an instance that read that paper against this
definition (`prior-art-deep.md`). The "single-cut full-open" protocols of
Shinagawa and Nuida (arXiv:2507.04916), with one pair of cards per input and
helper cards of both colours placed anywhere, are the special case with two
positions per player and R everything.

## Results

| Claim | Status | Where |
|---|---|---|
| **f(n) = ∞ for every n ≥ 3:** with all cards revealed there is no valid protocol, for any m | proved by hand by solver B; read by the referee, by the instance that did the deep literature read, and by the organiser, with no gap found; the referee tested the key identity numerically. Solver A did not find it and left f(3) as a conjecture. Machine-checked in Lean 4 since 21 November, in the formulation of NOTE.md Section 1 (rows cyclically equal exactly when the outputs agree), which is equivalent to the one above by a short hand argument | `solver-b/proofs.txt` T2; written out below; `lean/` |
| If the rows for three players' inputs other than 1, 1, 1 (everything else fixed) are rotations of one word, the row for 1, 1, 1 is a rotation of it too | this is what T2's proof shows | `solver-b/proofs.txt` T2 |
| The same with any one of the eight inputs in place of 1, 1, 1, and its consequence for the equality function | restatement by the organiser; confirmed by all five seats of the review panel; machine-checked in Lean 4 for any corner, for AND of n ≥ 3 bits and for equality of n ≥ 4 bits | below; `lean/` |
| r(3) ≤ 36 | an explicit protocol, checked from the definition by solver A's checker and by the referee's own, which also rejected nine broken variants; also machine-checked in Lean 4 against the definition in `lean/CutPeek/PartialSpec.lean` | `solver-a/w_n3_m36.txt`, `lean/CutPeek/PartialMain.lean` |
| No valid three-player protocol with m ≤ 13 | computed exhaustively by both solvers and recomputed by the referee | `verifier/table_m13.txt` |
| None with m = 14, 15, so r(3) ≥ 16 | computed by both solvers, independently written but with the same normalisation; not rerun by the referee | `solver-a/fast_3_14_p*.txt`, `solver-a/fast_3_15_p*.txt`, `solver-b/fast2.out`, `solver-b/m15.part*.out` |
| r(2) = f(2) = 5, and a valid two-player protocol exists for every m ≥ 5 | the five-card trick; m ≤ 4 excluded by exhaustive search (three programs); the family for m ≥ 5 proved by solver A and checked to m = 25 by the referee | `solver-a/proofs.txt` L4 |
| r(n) ≥ max(2n, 2^n − 1) | proved by hand by solver B; read by the referee; the part 2^n − 1 is machine-checked in Lean 4 (`lean/CutPeek/PartialMain.lean`) | `solver-b/proofs.txt` L1, T1 |
| For n = 3 a valid protocol has 5 ≤ \|R\| ≤ m − 2, and every shifted copy of R contains a card of every player that depends on that player's bit | proved by hand (solver B; the covering statement by both solvers); read by the referee | `solver-b/proofs.txt` T4, C3, T5 |
| If a valid protocol exists with a cards, one exists with a·b cards for every b | proved by hand by solver A; read by the referee | `solver-a/proofs.txt` L5′ |
| If correctness only asks that the all-ones input be distinguishable in distribution, three players need exactly 12 cards | computed by both solvers and the referee | `solver-b/weak_n3.txt` |
| Within protocols built from interleaved strands, none smaller than 36 was found in the cases searched | solver A's searches only, partly with a floating-point solver; some cases below 36 were not searched, so this is not a minimality claim; the referee did not check any of it | `reports/solver-a.md` |

## Why full opening fails for three players

This is solver B's T2, written out.

Write a card as +1 or −1 and a row as a vector indexed by position. Fix all
players but three, holding 1, so that the AND is 1 exactly when the three
hold 1, 1, 1. Let W be the row when those three hold 0, 0, 0. Changing
player i's bit changes only cards on player i's own positions, so there are
vectors a₁, a₂, a₃, supported on disjoint sets, with

    row(x) = W − 2(x₁a₁ + x₂a₂ + x₃a₃)   for x in {0,1}³.

With every card revealed, M(x) is the set of rotations of row(x) with
repetition, so privacy says that the seven rows other than row(1,1,1) are
rotations of W. The claim is that row(1,1,1) is then a rotation of W as well,
which makes it indistinguishable from the others, against correctness.

Let z rotate a row by one place. By assumption there are shifts sᵢ and sᵢₖ
with W − 2aᵢ = z^(sᵢ)W and W − 2aᵢ − 2aₖ = z^(sᵢₖ)W. Substituting the first
into the second, and into row(1,1,1):

    (z^(sᵢ) + z^(sₖ) − 1 − z^(sᵢₖ)) W = 0   for each pair i, k,        (∗)
    row(1,1,1) = (z^(s₁) + z^(s₂) + z^(s₃) − 2) W.

Take the discrete Fourier transform over the m positions. Let ω be a
primitive m-th root of unity, and let r be a frequency at which the transform
of W is not zero; put θᵢ = r·sᵢ. Then (∗) says

    ω^(θᵢ) + ω^(θₖ) = 1 + ω^(r·sᵢₖ).

Two complex numbers of modulus 1 with a given non-zero sum are determined as
a pair. So for each pair i, k one of two things holds.

- The two sides are the same pair of numbers: one of θᵢ, θₖ is 0 and
  r·sᵢₖ = θᵢ + θₖ.
- Both sums are zero (possible only when m is even): θᵢ = θₖ + m/2 and
  r·sᵢₖ = m/2. If one of θᵢ, θₖ is 0
  this again gives r·sᵢₖ = θᵢ + θₖ. Otherwise call the pair exceptional;
  then r·(sᵢₖ − sᵢ − sₖ) = −2θₖ.

(All of this is modulo m.) Two pairs cannot both be exceptional at the same
frequency: any two of the three pairs share an index, so by symmetry take
(1,2) and (1,3); if both were exceptional, then θ₂ = θ₃ ≠ 0, and the pair (2,3)
would fit neither case. Put t = s₁₂ + s₁₃ + s₂₃ − s₁ − s₂ − s₃.

- No exceptional pair. Then no two of the θ are non-zero, so
  ω^(θ₁) + ω^(θ₂) + ω^(θ₃) − 2 = ω^(θ₁+θ₂+θ₃), and r·t = θ₁ + θ₂ + θ₃.
- One exceptional pair (k, l). Then the third index i has θᵢ = 0, and
  ω^(θₖ) = −ω^(θₗ), so the sum minus 2 is −1; and
  r·t = −2θₗ + θₖ + θₗ = θₖ − θₗ = m/2, so ω^(r·t) = −1 as well.

In both cases the transform of row(1,1,1) at r equals ω^(r·t) times that of
W; at frequencies where the transform of W vanishes both sides are zero. So
row(1,1,1) = z^t W, a rotation of W.

Nothing in the argument uses which corner of the cube is the odd one out:
replacing some xᵢ by 1 − xᵢ changes W and the sign of aᵢ and keeps the form
of the rows. So for any three players and any fixing of the rest, seven rows
that are rotations of one word force the eighth.

**The 2025 papers' open questions (answered by Nuida, Shinagawa and Numata;
see Prior work).**

- The conclusion of Shinagawa and Nuida (arXiv:2507.04916) asks: "Is there a
  single-cut full-open protocol for the three-bit AND function?" The answer
  is no: in their model by Theorem 1 of Nuida, Shinagawa and Numata, and in
  the wider one, if the proof here is right, by the argument on this page.
  Their more specific question, whether a certain seven words can be made
  cyclically equal by inserting helper words, is not answered by the argument
  here, which only shows that whenever they are, the eighth word is
  cyclically equal to them too. The proof of that Theorem 1 answers it:
  already four of the seven words cannot be made cyclically equal.
- The same conclusion asks the question for four-bit equality. For n-bit
  equality with n ≥ 4, fix x₄ = … = xₙ = 0: among the eight inputs for x₁,
  x₂, x₃ only the all-zero one has all bits equal, so the
  other seven rows must be rotations of one word, the eighth then is too, and
  correctness fails. So no single-cut full-open protocol for n-bit equality
  with n ≥ 4 exists either; in the one-pair-per-bit model this is Corollary 2
  of Nuida, Shinagawa and Numata. Three-bit equality is not covered (two of
  its eight inputs have all bits equal), and a protocol for it is known. This consequence was noticed by the instance
  doing the deep read; it is not in either solver's report and the referee
  did not see it.
- According to the deep read, Ikeda and Shinagawa (arXiv:2509.17595) show
  by integer programming that
  three-input AND has no such protocol when all helper cards have one colour,
  and state the case of both colours as open. For AND the argument here
  covers both colours. Theorem 3 of Nuida, Shinagawa and Numata settles, in
  their model, every three-variable function that depends on all three
  variables; functions with redundant variables are left open there.

According to the deep read, Eriguchi and Shinagawa do build a full-open
protocol for n-input AND, but
with a shuffle that is not a single random cut, so there is no conflict.

## Thirty-six cards with partial opening

Solver A's protocol uses six strands of six cards each, interleaved: strand
s holds positions s, s + 6, s + 12, …, and R is the multiples of 6. After the
cut, R shows exactly one strand, rotated. So the protocol is a list of six
6-card rows per input, with the requirement that each player's cards sit at
fixed places; the collection of rotations of the six rows must be the same
for the seven inputs with AND 0, and share nothing with those for 1, 1, 1.
Solver A found such a list by linear and integer programming.

The mechanism of interleaving piles and opening equally spaced positions is
Lemma 4.3 of Eriguchi and Shinagawa. Their general theorem, instantiated for
three-input AND by the deep-read instance, gives 2,816 cards by the stated
formula and 2,200 by the count in its proof; they state no number for this
function. The 36-card protocol is not an instance
of their construction (its strands have no delimiter cards).

## What to distrust

- **Everything is by one model family, and no mathematician has checked the
  full-opening proof.** It is short and uses only the discrete Fourier
  transform, but it has been read only by models. Since 21 November it is
  machine-checked in Lean 4 with Mathlib (folder `lean/`), together with the
  general theorem and its consequences for AND and for equality, as
  statements about a 53-line specification. That removes the risk of an
  error in the algebra; it leaves the question whether the specification and
  the four statements say what is meant, which only models have judged. Of
  the partial-opening results it covers only the bound 2^n − 1 and the
  validity of the 36-card protocol, as statements about a further 33-line
  specification; the search for m ≤ 15 is not machine-checked.
  Solver B's own machine test of the statement cannot fail for three players
  (it finds no configuration that satisfies the hypothesis for m ≤ 18); the
  referee's numerical test of the identity over frequencies is a real test.
- **The account of the 2025 papers' open questions rests on reading four
  preprints.**
  The deep-read instance read them from the PDF text and quotes the
  definitions; I checked the wording of the open questions in the second
  paper's text myself. They had been answered: one of the authors told us so
  on 23 November 2026 (see Prior work). The paper's ePrint record is dated
  20 November, the day after the web search.
- **The lower bound 16 rests on pruned searches that share a
  normalisation.** The referee's recomputation stops at 13 cards and uses the
  same normalisation as the solvers' programs. An error in it would produce
  false "none" answers. The searches were validated against brute force only
  where brute force reaches: two players, where valid protocols exist, and
  three players at 6 cards, where the comparison is zero against zero. Solver
  B also matches its leaf counts against a closed formula, which checks the
  enumeration and not the pruning.
- **There is no exhaustive result between 16 and 35 cards.** Solver A
  excludes some strand-family cases there; that is unchecked, and other cases
  were not searched.
- **Existence for partial opening is not new** (per the deep read), and
  neither is the bound
  2^n − 1 in the full-open case (Shinagawa and Nuida, Theorem 5.2, by a
  different argument that does not cover partial opening).
- **The solvers disagreed on the headline.** Solver B guessed that no
  three-player protocol exists at any size (55 per cent) and gave a reason
  the witness refutes. Solver A understated its bound for four players and
  missed the short proof for full opening.
- **Errors in the chain's question.** Its guess was wrong. Its belief (f) is
  false as worded for two players (the five-card trick is of the excluded
  form). Its belief (e) leans on a reconstruction theorem recalled from
  memory, which turns out not to be needed.
- **The referee's test script is weak in form.** `verifier/tests.py` prints
  results without asserting them, and one of its ten "broken" variants swaps
  two identical cards and is rightly accepted; the nine real ones are
  rejected. `verifier/vcheck.py` prints the declared number of players but
  does not check it against the file.
- **Process.** Solver A reported one ten-minute run started without the
  required limits.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files, with most of the
brief aimed at breaking the full-opening proof. All five confirmed its
set-up, each step of the algebra, the extension to any corner and to
equality, the 36-card protocol, and solver B's other proofs; none produced a
counterexample. Their findings on this page, applied above:

- The written-out proof did not say that the other players are fixed to 1,
  which the argument needs when there are more than three (Grok, Claude
  Opus).
- It did not say that the zero-sum case needs m even, nor why checking one
  choice of two pairs is enough.
- Statements about the outside papers were made as facts in several places
  where the only source is an instance's reading.
- "36 is the least size" within the strand family was stated as a fact
  although cases were not searched (two seats).
- I implied that the referee's recomputation to 13 cards was independent of
  the solvers' normalisation; it is not.
- The weaknesses of the referee's test script (GPT).

The corrected wording has not been through a second round.

The standalone note went to the same five reviewers separately. All five
confirmed its definitions, its written-out proof of the lemma line by line,
and the theorem with its examples (AND, OR, equality, majority of five). GPT
marked the section on partial opening refuted because the list of further
facts did not say n ≥ 2. Most other findings were about evidence: three
checks the note cited (the 36-card protocol, a test of the proof's identity,
and a consistency check against a published protocol) had no script in the
folder, and the note said the full record was in the folder although the
reviewers' reports are not. A script with those checks and its output were
added, and the wording was corrected. The corrected note has not been through
a second round.

The Lean formalisation went to the same five reviewers with a different
question: not whether the proofs are right, which Lean checks, but whether
the definitions and the four statements say what the note means. All five
confirmed that they do, including the handling of m = 0, the meaning of the
protocol definition when the function is constant, and the five-card layout,
which several worked out by hand. Their findings, applied: the note credited
"an AI model instance" with the Lean files, hiding that the specification
was written first and separately from the proofs; the list of what must be
trusted was incomplete; the proof and the note use opposite sign
conventions; a comment in the specification needed "for injective i"; and a
pre-existing sentence about a search finding "no encoding at all" was wrong
as worded. A reviewer suggested the six-card check, which was added. The
changes made after the round have not been reviewed again.

The partial-opening formalisation (21 November) went to the same five
reviewers with the same kind of question. All five confirmed that the 33
lines of definitions say what Section 4 of the note means, neither weaker
nor stronger, that the bound is stated as in the note, and that the digits
in the Lean file are the note's layout; both Claude seats recomputed the
layout's validity with their own programs. Their findings, applied: two
sentences still said the partial-opening results were not formalised (all
five); "a clean full build" rested on a one-line log, so the project's
modules were rebuilt from scratch and that log kept; the failure-test log
named a different file; the note's account left out the correctness check
and the added trust assumptions. Not changed: three inaccurate remarks in the proving instance's own files (a comment in `PartialCheck.lean` that also covers the unused all-true case, "three" for four uses of `decide` in its log, and that log's reference to `BUILD-partial.log`, which is now the organiser's rebuild). A second-round reviewer (one Claude instance) then checked every sentence
about Lean in this page, the note and the Lean README against the files;
its corrections (the two layouts were added after the proofs, not written
first) are applied.

## Files

- `review/`: the review of this experiment before publication (16 to 20 November), archived on 21 November verbatim with absolute paths shortened: the brief given to the reviewers, each seat's report, the machine-made summary, and the organiser's note of what was done with the findings; `review-note/` is the same for `NOTE.md` (the reviews of the emails to the papers' authors are not archived). The reports were written about the files as they stood before the findings were applied, so their line numbers and quotations may not match the current files.
- Reviewers' reports of 21 November, archived verbatim with absolute paths shortened: `lean/review-full-opening/`, `lean/review-partial/`. Each `review*/` folder holds the brief the reviewers were given, the reports of the GPT, Grok and Gemini seats with a machine-made summary, the reports of the Claude seats (one of the two ran on Claude Opus), and the organiser's note of what was done with the findings; `review-second-round-1.md` is the later recheck of the Lean-related text by a single Claude instance. The reviews of the two emails to the papers' authors are not archived here. The reports were written about the files as they stood before the findings were applied, so their line numbers and quotations may not match the current files.
- `lean/REPORT-full-opening.md`, `lean/REPORT-partial.md`: the final reports of the two proving instances, archived from the session transcript on 21 November, verbatim with paths shortened.
- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: searches, witnesses, checkers, proofs.
- `verifier/`: the referee's checker, brute force, reduced search and tests.
- `NOTE.md`: the standalone note. `note-checks.py`, `note-checks.txt`: three
  small checks it cites, and their output.
- `lean/`: the Lean 4 formalisation of the full-opening results and, since
  21 November, of two partial-opening results (the bound 2^n − 1 and the
  validity of the 36-card protocol), with its own README, build logs and
  axiom lists.
- `prior-art.md`: the first literature search. `prior-art-deep.md`: the
  second, which read the four closest papers against this definition. The
  papers themselves are not included.

Binaries are left out.
