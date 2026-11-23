# No single-cut full-open protocol for three-input AND or four-input equality

*A short note, produced by AI models and not read by any mathematician; see
Section 6 and "How this was produced" before relying on it. The main lemma
and theorem are machine-checked in Lean 4 (Section 6). Draft of 19 November
2026, revised 21 November. Its main results were obtained earlier, and
stronger ones in the one-pair-per-bit model, by Nuida, Shinagawa and Numata
[8]; see the next paragraph. Supporting files: this folder; overview in
[README.md](README.md).*

**Prior work (added 23 November 2026).** For protocols that encode each
input bit by one pair of cards, the impossibility results on this page for
full opening are contained in stronger results of Koji Nuida, Kazumasa
Shinagawa and Yasuhide Numata: "Impossibility Results for Single-Cut Full-Open
Card-Based Protocols with Any Number of Cards", Cryptology ePrint Archive,
Paper 2026/2960 (https://eprint.iacr.org/2026/2960) [8], posted on 20 November
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
one card, we have not checked. In their model the hypothesis of Lemma 1 here
(seven rows that are rotations of one word) cannot occur. Their proof of
Theorem 1 uses polynomials over the two-element field. The proof here uses
the discrete Fourier transform (evaluation at all m-th roots of unity); their
paper evaluates at one root of unity for its Theorems 2 and 3. The
partial-opening results on this page and the Lean 4 formalisation have no
counterpart there.

## Summary

A single-cut full-open (SCFO) protocol encodes input bits as a row of
face-down cards, applies one random cut, and opens every card. The five-card
trick [1] computes the AND of two bits this way, and the six-card trick
[6, 7] the equality of three. Shinagawa and Nuida [2] ask whether there is an
SCFO protocol for three-bit AND, and whether there is one for four-bit
equality; Ikeda and Shinagawa [3] show that three-bit AND has none when all
additional cards have the same colour and leave the case of both colours
open. Both questions were answered by Nuida, Shinagawa and Numata [8]; see
Prior work above.

**Lemma 1.** Let a row of cards, each ♣ or ♥, depend on three input bits in
such a way that each card is determined by at most one of the bits (precise
definitions in Section 1). If seven of the eight rows are rotations of one
word, then so is the eighth.

**Theorem 2.** Let f be a Boolean function of n ≥ 3 bits. Suppose that for
some three of its variables and some fixed values of the others, f takes one
value at exactly seven of the eight remaining inputs. Then f has no SCFO
protocol, with additional cards of both colours in any numbers, and with any
way of encoding the bits in which each card depends on at most one bit.

In particular there is no SCFO protocol for the AND or the OR of n ≥ 3 bits,
none for the equality of n ≥ 4 bits, and none for the majority of five bits.
(The cases of AND and equality are machine-checked; OR and majority follow
from Theorem 2 by hand.)

The proof of Lemma 1 is one page: it takes the discrete Fourier transform
along the row. Lemma 1, Theorem 2 and the statements for AND and for
equality are machine-checked in Lean 4 with Mathlib, as statements about a
53-line specification (Section 6). No mathematician has read the proof or
the specification.

Section 4 records what the same experiment found when only a fixed subset of
the cards is opened: the bound m ≥ 2^n − 1 on the number of cards for n-bit
AND still holds, three-bit AND has a protocol with 36 cards, and a pruned
exhaustive search found none with 15 or fewer. That some number of cards
suffices in that setting is a theorem of Eriguchi and Shinagawa [5].

## 1. Setting

Cards are ♣ or ♥. A word is a string of cards, read cyclically: two words are
*cyclically equal* if one is a rotation of the other. A random cut rotates a
face-down row by a uniformly random amount that nobody learns, so after a cut and a full opening
an observer learns exactly the cyclic class of the row.

Throughout, n and m are positive integers. An *encoding* of n bits on m
cards assigns to each input x in {0,1}^n a word
s(x) of length m such that every position is of one of two kinds: its card is
the same for all x (an additional card), or its card is determined by one
input bit x_i and differs for x_i = 0 and x_i = 1.

The encoding is an *SCFO protocol for f* if s(x) and s(x′) are cyclically
equal exactly when f(x) = f(x′).

This covers the definition in [2, Section 5.2]: there each bit is a pair of
cards (x_i and its complement), the 2n cards are permuted by a known
permutation, arbitrary binary words u_1, …, u_2n of additional cards are
inserted, and the protocol is required to have two words z_0, z_1, not
cyclically equal, with s(x) cyclically equal to z_f(x). It also covers the
standard SCFO protocols of [3], which are those of [2] with a stated number
of additional cards of each colour, and encodings that use more than two cards
per bit, as in the protocol for x ⊕ y ⊕ z of [4].

## 2. Proof of Lemma 1

Write ♣ as +1 and ♥ as −1, so that a word is a vector indexed by the
positions 0, …, m − 1, taken modulo m. Let W = s(0,0,0). Since each card
depends on at most one bit, there are vectors a₁, a₂, a₃ with

    s(x) = W − 2(x₁a₁ + x₂a₂ + x₃a₃)   for all x in {0,1}³;

aᵢ is W on the positions that depend on xᵢ and 0 elsewhere. (The proof uses
only that s is of this form, not that the entries are ±1 or that the aᵢ have
disjoint supports.)

Assume first that the seven rows other than s(1,1,1) are rotations of one
word, hence of W. Let z be the rotation by one place. There are integers sᵢ
and sᵢₖ with

    W − 2aᵢ = z^(sᵢ) W,     W − 2aᵢ − 2aₖ = z^(sᵢₖ) W     (i ≠ k).

Substituting 2aᵢ = W − z^(sᵢ)W into the second equation, and into s(1,1,1):

    (z^(sᵢ) + z^(sₖ) − 1 − z^(sᵢₖ)) W = 0   for each pair i, k,         (∗)
    s(1,1,1) = (z^(s₁) + z^(s₂) + z^(s₃) − 2) W.                       (∗∗)

Let ω be a primitive m-th root of unity and Ŵ(r) = Σ_p W_p ω^(rp) the
discrete Fourier transform; rotation by s multiplies the transform at r by
ω^(rs). Fix a frequency r with Ŵ(r) ≠ 0 and put θᵢ = r·sᵢ. From here on
all equalities between the θ, the products r·s and r·t are congruences
modulo m, which is all that the powers of ω see. Then (∗) gives

    ω^(θᵢ) + ω^(θₖ) = 1 + ω^(r·sᵢₖ).

Two complex numbers of modulus 1 whose sum is a given non-zero number are
determined as an unordered pair. So for each pair i, k one of the following
holds.

- (N) The sum is not zero, and {ω^(θᵢ), ω^(θₖ)} = {1, ω^(r·sᵢₖ)}. Then one of
  θᵢ, θₖ is 0, and r·sᵢₖ = θᵢ + θₖ.
- (X) Both sides are zero. This needs m even, and then θᵢ = θₖ + m/2
  (the same as θₖ − m/2 modulo m) and
  r·sᵢₖ = m/2. If one of θᵢ, θₖ is 0, the other is m/2 and again
  r·sᵢₖ = θᵢ + θₖ. Otherwise call the pair *exceptional*; for it
  r·(sᵢₖ − sᵢ − sₖ) = m/2 − θᵢ − θₖ = −2θₖ.

So for every pair that is not exceptional, r·sᵢₖ = θᵢ + θₖ and at least one
of θᵢ, θₖ is 0.

At most one pair is exceptional at the frequency r. Any two of the three
pairs share an index; relabel so that the shared index is 1 and suppose
(1,2) and (1,3) are exceptional. Then
θ₂ = θ₁ + m/2 = θ₃, which is not 0. The pair (2,3) then has sum
2ω^(θ₂) ≠ 0, so (N) applies and requires one of θ₂, θ₃ to be 0, a
contradiction.

Put t = s₁₂ + s₁₃ + s₂₃ − s₁ − s₂ − s₃, so that
r·t = Σ over pairs of r·(sᵢₖ − sᵢ − sₖ) + (θ₁ + θ₂ + θ₃).

- *No exceptional pair.* Every pair contributes 0 to the sum, so
  r·t = θ₁ + θ₂ + θ₃. Among any two of the θ one is 0, so at most one θ is
  non-zero and ω^(θ₁) + ω^(θ₂) + ω^(θ₃) − 2 = ω^(θ₁+θ₂+θ₃) = ω^(r·t).
- *One exceptional pair (k, l).* Both θₖ and θₗ are non-zero, and the pairs
  (i, k) and (i, l) with the third index i are not exceptional, so θᵢ = 0.
  Since ω^(θₖ) = −ω^(θₗ), we get ω^(θ₁) + ω^(θ₂) + ω^(θ₃) − 2 = −1. And
  r·t = −2θₗ + (θₖ + θₗ) = θₖ − θₗ = m/2, so ω^(r·t) = −1 as well.

In both cases (∗∗) says that the transform of s(1,1,1) at r equals
ω^(r·t)·Ŵ(r). At a frequency where
Ŵ(r) = 0, (∗∗) shows that the transform of s(1,1,1) vanishes too, so the same
equation holds. Hence s(1,1,1) = z^t W.

The shifts sᵢ, sᵢₖ are not unique when W has a period; any choice works,
since the argument only uses the two displayed equations.

Finally, the corner (1,1,1) plays no special role. If the odd row out is
s(c) for another c in {0,1}³, replace xᵢ by 1 − xᵢ for each i with cᵢ = 0.
The rows are again of the form W′ − 2(x₁a₁′ + x₂a₂′ + x₃a₃′), with W′ the row
at the corner opposite to c and aᵢ′ = ±aᵢ, and the odd row out is now at
(1,1,1). ∎

## 3. Proof of Theorem 2 and examples

Let s be an SCFO protocol for f, and fix the other variables as in the
hypothesis. On the eight remaining inputs each card still depends on at most
one of the three free bits. The seven inputs with the common value of f have
rows that are cyclically equal to one another, so by Lemma 1 the eighth row
is cyclically equal to them. But f takes the other value there, so that row
must not be cyclically equal to them. ∎

The hypothesis says that some restriction of f to three variables is an AND
of three literals or the negation of such an AND. Examples:

- **AND of n ≥ 3 bits**: fix the other bits to 1. Likewise **OR** (fix them
  to 0). This is the first of the two questions quoted in the Summary;
  it was answered earlier in [8].
- **Equality of n ≥ 4 bits** (1 when all bits are equal): fix
  x₄ = … = xₙ = 0. Among the eight inputs for x₁, x₂, x₃, only the all-zero
  one has all bits equal. This is the second question, also
  answered earlier in [8].
- **Majority of five bits**: fix two bits to 1; the function becomes the OR
  of the other three.

The theorem says nothing about three-bit equality (two of its eight inputs
have value 1), three-bit XOR, or the three-variable and four-variable
functions of [4]; SCFO protocols for those are known [4, 6, 7].
As a consistency check, we enumerated the truth table of the four-variable
function from Table 2 of [4]: each of its eight restrictions to three
variables has exactly four inputs of each value, so Theorem 2 does not apply
to it (`note-checks.py`, check 3).

Two things the theorem does not settle.

- The more specific question in [2]: whether there is a permutation π such
  that the seven words π(x₁ x̄₁ x₂ x̄₂ x₃ x̄₃), (x₁,x₂,x₃) ≠ (1,1,1), are
  cyclically equalizable, that is, can be made cyclically equal by inserting
  the same additional words at the same places in all of them. Lemma 1 shows only that whenever additional cards
  make those seven words cyclically equal, the eighth word becomes cyclically
  equal to them too. The analogous question in [2] about fourteen words for
  four-bit equality is likewise not decided. Both are decided in [8]: the
  proof of Theorem 1 there shows that already four of the words cannot be
  made cyclically equal.
- Functions with no restriction of the kind in the hypothesis. In
  particular it does not address the other classes of three-variable
  functions treated in [3]. Those depending on all three variables are
  settled, in the one-pair-per-bit model, in [8].

## 4. Opening only some of the cards

The experiment's original question was about a wider model. For n-bit AND:
the row is laid as above, cut at random, and then only the cards at a fixed
non-empty set R of positions are opened. For an input x let M(x) be the
multiset of the m words that appear at R as the cut varies. The protocol is
*valid* if M(x) is the same for all x other than the all-ones input
(privacy), and no word of M(1…1) occurs in M(0…0) (correctness). With R
everything this is an SCFO protocol for AND. This is the single-shuffle
static-open model of [5] with the shuffle a random cut.

**Proposition 3.** A valid protocol for n-bit AND has m ≥ 2^n − 1 cards,
for any R.

For R everything this is [2, Theorem 5.2], proved there by counting distinct
cyclically equal rows; that argument does not apply when R is smaller.

*Proof* (solver B's T1). Write cards as ±1. For a set S of positions and an
input x put C_S(x) = Σ_j Π_{q in S} s(x)_{q+j}, the sum over all m shifts.
For S inside R this is determined by M(x), and conversely the numbers C_S(x),
S inside R, determine M(x) (the multiplicity of a word t is
2^(−|R|) Σ_S (Π_{q in S} t_q)·C_S(x)). If C_S(1…1) = C_S(0…0) for every S
inside R, then M(1…1) = M(0…0), against correctness. So fix S with
δ = C_S(1…1) − C_S(0…0) ≠ 0. By privacy C_S(x) has one value c for all
x ≠ 1…1.

Let P_i be the positions whose card depends on x_i. For a shift j let v_j in
{0,1}^n record, for each i, the parity of the number of positions of S + j
in P_i. Flipping x_i negates the cards on P_i, so
Π_{q in S+j} s(x)_q = b_j·(−1)^(x·v_j) with b_j = ±1 not depending on x.
Hence C_S(x) = Σ_v B_v (−1)^(x·v), where B_v is the sum of b_j over the
shifts j with v_j = v. Inverting,

    B_v = 2^(−n) Σ_x C_S(x)(−1)^(x·v) = c·[v = 0] + 2^(−n)·δ·(−1)^(|v|),

where [v = 0] is 1 when v = 0 and 0 otherwise, and |v| is the number of ones
in v.

So B_v ≠ 0 for each of the 2^n − 1 non-zero v, and each of them occurs as
v_j for some shift j. Different v need different shifts, and there are m
shifts. ∎

Further facts about valid protocols for n ≥ 2, with proofs in
`solver-b/proofs.txt` (T4, T5, C3, L1): every shifted copy of R contains a
card depending on each input bit (otherwise the word seen at that shift is the same on 1…1 and on
an input with AND 0); |R| ≥ n + 1; for n = 3, 5 ≤ |R| ≤ m − 2; each bit
controls an even number of cards, half of each colour.

**Computed.** For three bits there is no valid protocol with m ≤ 15, by
pruned exhaustive search over all layouts and all R (two independently written
programs for m ≤ 15, a third for m ≤ 13; all three use the same
normalisation, see Section 6). If correctness is weakened to
M(1…1) ≠ M(0…0), the least m is 12.

**A protocol with 36 cards for three-bit AND.** Positions 0 to 35; R is the
multiples of 6. Line "owner" gives, for each position, the bit that controls
it (0 for an additional card); "u0" and "u1" give the card laid when that bit
is 0 or 1. In these strings 1 stands for ♥ and 0 for ♣ (this is the file
format, unrelated to the ±1 of Section 2), and the two strings agree on
additional cards.

    owner 111111012301331022102230222123003012
    u0    000011000011011001000000110111111011
    u1    111100011110100010101110001000110000

Call the six positions in one residue class modulo 6 a strand;
after the cut, R shows one strand, rotated. This is the mechanism of
[5, Lemma 4.3]; the protocol is not an instance of the construction of
[5, Theorem 4.2], whose strands carry delimiter cards. It was found by
integer programming (`solver-a/`) and is checked from the definition by two
separately written programs (`solver-a/check.py`, `verifier/vcheck.py`):
M(x) is the same multiset, with 33 distinct words, for the seven inputs with
AND 0, and the 16 distinct words of M(1,1,1) are all different from those
(also `note-checks.py`, check 1). We do not know the least
number of cards; it is between 16 and 36.

## 5. Related work

The five-card trick is due to den Boer [1]. The six-card trick for three-bit
equality is due to Heather, Schneider and Teague [6] and to Shinagawa and
Mizuki [7], who asked whether n-bit equality has an SCFO protocol for n ≥ 4
(as reported in [2]).

Shinagawa and Nuida [2] study when words can be made cyclically equal by
inserting letters, prove that an SCFO protocol for f needs at least
max(N_0(f), N_1(f)) cards, where N_b is the number of inputs with value b.
The list of open problems in their Conclusion ends with two items: "Is there
a single-cut full-open protocol for the three-bit AND function?" and "Is
there a single-cut full-open protocol for the four-bit equality function?",
each followed by a more specific question about the cyclic equalizability of
the words π(x₁ x̄₁ … ) over the inputs where the function is 0 (seven words
for AND, fourteen for equality). Their note [4] gives SCFO protocols for
x ⊕ y ⊕ z, for a four-variable function and for a three-variable function
obtained from it.

Ikeda and Shinagawa [3] model standard SCFO protocols as integer programs
and show, in the words of their abstract, that "for any three-variable
Boolean function, no new SCFO protocols exist beyond those already known,
under the condition that all additional cards have the same color"; they
state that the impossibility of
SCFO protocols "that use any number of both ♣ and ♥ as additional cards
remains an open problem". For the class of three-bit AND, Theorem 2 settles
that case; for the other classes it does not. In the one-pair-per-bit
model, [8] (Theorem 3 there) settles every three-variable function that
depends on all three variables.

Eriguchi and Shinagawa [5] write that "even the three-input AND function is
not known to have a single-shuffle full-open protocol" and construct
full-open protocols for every function using one shuffle that is a
composition of pile-shifting shuffles (O(n²) cards for n-bit AND). Theorem 2
says that for the AND of n ≥ 3 bits the shuffle in such a protocol cannot be
a uniformly random cut.
They also prove that every function has a static-open protocol whose shuffle
is a random cut [5, Theorem 4.2], which is the setting of Section 4; the
number of cards there depends on the cost of a private simultaneous
messages protocol for the function (a notion from cryptography, not needed
here), and no number is stated for three-bit AND.

We searched for later work answering these questions on 19 November and
found none; the search was by web queries only. The questions had in fact
been answered [8]: see "Prior work" at the top.

## 6. What has been checked, and what has not

- **Lemma 1** was written by one AI model instance (solver B's T2). Its
  algebra was re-derived line by line by the organising instance, read by a
  referee instance and by a third instance, and confirmed by five reviewing
  models from four vendors that were asked to find a counterexample (their
  reports are not in this folder; the README's Review section summarises
  them). The version above adds to solver B's the remark on the choice of two
  pairs and the reduction of an arbitrary corner. **No mathematician has read
  it.**
- **Machine check.** Lemma 1 (for an arbitrary corner), Theorem 2, and its
  consequences for the AND of n ≥ 3 bits and the equality of n ≥ 4 bits are
  proved in Lean 4.33.1 with Mathlib, in the folder `lean/` (about 410 lines
  of proof; the build log and the list of axioms used, which are Lean's three
  standard ones, are there). The proof follows Section 2: the discrete
  Fourier transform on the integers modulo m, with the case analysis
  shortened, and with ♥ as +1 where Section 2 takes ♣, which changes
  nothing. The statements are about the definitions in
  `lean/CutPeek/Spec.lean`: a row is a function from positions modulo m to
  two kinds of card; an encoding gives each position either a fixed card or
  a card that is one of the two kinds or the other according to a single
  bit; a protocol for f is an encoding whose rows for two inputs are
  rotations of one another exactly when f agrees on them. This is Section
  1's definition. As a check that the definitions do not exclude everything,
  Lean also confirms, by direct computation, that the five-card trick is a
  protocol for the AND of two bits and that the six-card trick is one for
  the equality of three bits (`lean/CutPeek/FiveCard.lean`, `SixCard.lean`);
  the second shows that "n ≥ 4" in the statement for equality cannot be
  lowered. What still has to be trusted: Lean's kernel and the Mathlib
  definitions the specification uses; that the build log belongs to these
  files; and that those 53 lines, the four statements at the end of
  `lean/CutPeek/Main.lean` and the two layouts say what this note means. So
  far only AI models have read them. The statements for OR and for the
  majority of five bits are not formalised. Two results of Section 4 are formalised (added on
  21 November): Proposition 3, and that the 36-card layout is a
  valid protocol, as statements about a further 33-line file of definitions,
  `lean/CutPeek/PartialSpec.lean` (the word seen at R after a cut; the
  multiset of the m words; validity as privacy and correctness above). The
  statements are in `lean/CutPeek/PartialMain.lean`, where the layout is
  retyped as lists of digits. For the layout Lean checks a certificate found
  outside it: for each input with AND 0, a permutation of the 36 cuts that
  matches its views with those of 0…0, and, for correctness, that for every
  pair of cuts some opened position tells 1…1 from 0…0. The same proof is
  rejected in the two cases tried, one digit of the layout changed and one
  opened position changed (`lean/MutantPartial.lean`). For these two
  results one must trust, in addition, that the 33 lines say what Section 4
  means and that the retyped digits are the layout printed there. The search result for m ≤ 15 and the other
  facts of Section 4 are not formalised.
  The specification, the four statements and the two layouts were written by
  the organising AI instance; the proofs by a second instance (the two layout checks, added later, are entirely the organising instance's), which was given the specification and statements and not allowed to change them (two
  comments in the specification were clarified afterwards). The same
  division holds for the partial-opening files. The builds were run by AI
  instances.
- A direct numerical test of Lemma 1 on card words is not available:
  searching all words and all shifts for m ≤ 18, solver B found no encoding
  in which all three bits control cards and seven rows of the cube are
  rotations of one word. (If some bit controls no card the hypothesis holds
  trivially, so the only instances of it known so far are degenerate, and
  the content of Lemma 1 is carried by Theorem 2.)
  The referee instead tested the identity behind the proof at the level of
  frequencies (all solutions of the three equations (∗) for m ≤ 40) and found
  no violation. The organising instance repeated that test with its own code
  for every choice of the six shifts and every frequency with m ≤ 9:
  1,011,693 cases satisfy (∗) and all satisfy the conclusion
  (`note-checks.py`, check 2; output in `note-checks.txt`).
- **Theorem 2 and its examples** are by the organising instance and one
  reading instance; the five reviewers confirmed the argument for AND and
  equality. The majority example and the consistency check against [4] were
  added for this note and have been checked by the organising instance only.
- **The reading of the literature** is by AI models. The quotations from
  [2], [3], [5] and the definition in Section 1 were checked against the text
  of the arXiv versions; whether the questions were still open on the date of
  this note was checked only by web search; they were not (see Prior work). References [6] and [7] were not
  read; they are cited as described in [2].
- **Proposition 3** was written by solver B, read by the referee and
  confirmed by the reviewers.
- **The bound "none with m ≤ 15"** rests on pruned searches that share a
  normalisation (complement form, balance, symmetry). An error in it would
  produce false negatives. The searches agree with a search straight from
  the definition where that reaches: two bits, where protocols exist, and
  three bits with 6 cards, where both find none, so that case does not test
  a positive find.
- **The 36-card protocol** can be checked with a few lines of code from the
  three strings above; the organising instance did so separately from the two
  checkers named in Section 4.

## How this was produced

This note comes from an experiment in which a chain of AI model instances
invents a question and other instances, sharing no context, solve and attack
it. The question (find the least number of cards for three-bit AND with one
random cut and a fixed set of opened positions) was produced by the chain,
which guessed that no number of cards suffices. Two solver instances worked
independently; one found Lemma 1 and Proposition 3, the other the 36-card
protocol. A referee instance recomputed and re-read their work, a further
instance read references [2] to [5] against the definitions, and a panel of
five models from four vendors reviewed the claims and, separately, this
note. The verbatim reports of the solvers, the referee and the two literature
searches are in this folder; the reviewers' reports are summarised in the
README.

## References

[1] B. den Boer. More efficient match-making and satisfiability: the five
card trick. EUROCRYPT '89, LNCS 434, pp. 208–217, Springer, 1990.

[2] K. Shinagawa and K. Nuida. Cyclic equalizability of words and its
application to card-based cryptography. arXiv:2507.04916.

[3] S. Ikeda and K. Shinagawa. Impossibility results of card-based protocols
via mathematical optimization. arXiv:2509.17595.

[4] K. Shinagawa and K. Nuida. A note on single-cut full-open protocols.
arXiv:2507.03323.

[5] R. Eriguchi and K. Shinagawa. Single-shuffle full-open card-based secure
computation protocols for any function. arXiv:2510.17308.

[6] J. Heather, S. Schneider and V. Teague. Cryptographic protocols with
everyday objects. Formal Aspects of Computing 26(1), pp. 37–62, 2014.

[7] K. Shinagawa and T. Mizuki. The six-card trick: secure computation of
three-input equality. In K. Lee (ed.), Information Security and Cryptology,
LNCS 11396, pp. 123–131, Springer, 2018.

[8] K. Nuida, K. Shinagawa and Y. Numata. Impossibility results for
single-cut full-open card-based protocols with any number of cards.
Cryptology ePrint Archive, Paper 2026/2960, 2026.
https://eprint.iacr.org/2026/2960
