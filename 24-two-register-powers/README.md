# Experiment 24: reaching a remainder with two registers, doubling and adding

Two registers start at (1, 1). A step either doubles one register or adds one
register into the other. This is how a power x^n is computed with two memory
cells and nothing else. For a modulus m, how many steps are needed, in the
worst case over the remainders r, before some register is congruent to r
modulo m?

**Short answer: more than log₂ m by a constant factor.** If D(m) is that
worst case, then D(m) ≥ (log₂ m − 1)/0.956 for all large m, which is about
1.046·log₂ m, while the binary method gives D(m) ≤ 2·log₂ m. The chain asked whether the ratio stays strictly above
1 in the limit and bet yes, which is right. The exact limiting ratio is not
known; the computed values of D(m)/log₂ m lie between about 1.28 and 1.4 for
m in the thousands.

With unlimited memory the corresponding ratio is 1: ordinary addition chains
reach every number n in log₂ n plus lower-order terms (a classical bound that
the reviewers attribute to Brauer, 1939; it was not looked up for this page).
So the result measures what the restriction to two registers costs.

## The question

A state is a pair (a, b) of positive integers, starting at (1, 1). One step
replaces it by one of (2a, b), (a, 2b), (a + b, b), (a, a + b). G(n) is the
least number of steps after which a register equals n. For m ≥ 2 and a
remainder r, g_m(r) is the least G(n) over n congruent to r modulo m, and
D(m) is the largest g_m(r). N(t) is the number of integers n with G(n) ≤ t.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md),
formal text in [chain/3-question.txt](chain/3-question.txt)): is the lower
limit L of D(m)/log₂ m strictly greater than 1?

## Results

| Claim | Status | Where |
|---|---|---|
| **N(t) ≤ 2·2^(0.956·t) for all large t, hence L ≥ 1/0.956 > 1.046** | proved by hand by solver B with a potential function; referee re-derived every case and found no gap, and tested the key inequality in exact arithmetic on all words of length up to 16 | `solver-b/proofs.txt` P2 |
| N(t) ≤ 2^(0.9631·t + o(t)), hence L ≥ 1.0383 | proved by hand by solver A with a different, counting argument; referee read it and recomputed the constant | `solver-a/proofs.txt` P4–P7 |
| ⌈log₂ m⌉ ≤ D(m) ≤ 2⌊log₂ m⌋, D(m) is at least the least t with N(t) ≥ m, and working modulo m gives the same D(m) as the definition over the integers | proved by hand by both solvers; standard | `solver-a/proofs.txt` P1–P3, `solver-b/proofs.txt` P0, P1 |
| N(t) for t = 0, …, 22: 1, 2, 4, 7, 12, 20, 34, 57, 98, 169, 295, 521, 912, 1614, 2864, 5093, 9115, 16337, 29401, 52912, 95363, 171995, 310856 | computed by solver A (to 22), solver B (to 21) and the referee (to 22); all agree | `solver-a/nt22.txt`, `verifier/lev22.out` |
| D(m) for every m from 2 to 4100 | computed by solver A (to 4100), solver B (to 4096) and, two ways, by the referee (to 4100); the tables agree wherever they overlap | `solver-a/D_all.txt`, `verifier/D_def1_4100.txt` |
| D(m) exceeds the counting bound "least t with N(t) ≥ m" by 0, 1 or 2 and never by 3, for m ≤ 40,000 | computed by the referee alone beyond 4100, one method | `reports/referee.md` |
| G(n) = ⌈log₂ n⌉ exactly when n has at most two 1-bits | "if" proved by hand by both solvers; "only if" computed for n ≤ 2²², not proved | `solver-a/proofs.txt` P8 |
| The exact value of L, and any upper bound on L below 2 | open | |

## Why two registers cost a constant factor

This follows `solver-b/proofs.txt` P2.

Order the two registers as x ≤ y and write r = x/y. Take the potential

    Φ = log₂ y + (1/4)·log₂ max(r, 1/4).

Doubling the larger register raises Φ by at most 1. Each of the other three
kinds of step (doubling the smaller register, adding the smaller into the
larger, adding the larger into the smaller) raises Φ by at most 3/4; this is
a calculation by cases, one kind of step at a time (doubling the smaller
register splits into two). The second term of Φ lies between −1/2 and 0,
so a sequence of j steps of which k are not "double the larger" ends with
log₂ y ≤ j − k/4 + 1/2.

So a number above 2^((1 − η)t) reached within t steps comes from a sequence
with at most about 4ηt steps of the other kinds. Sequences with so few such
steps are rare: with η = 0.044 their number is at most 2^(0.9502·t) up to
lower-order factors. Adding the 2^((1 − η)t) numbers below the threshold
gives N(t) ≤ 2·2^(0.956·t) for large t. Since the m remainders need m
different integers, m ≤ N(D(m)), and the bound on D(m) follows.

A sequence that reaches a large number can therefore contain few steps other
than "double the larger register", and a sequence is fixed by where those few
steps sit and which of the three kinds each is; that is what makes such
sequences rare. A single step of another kind can add a full bit to the
larger register (it does when the two registers are equal), which is why the
argument needs the second term of Φ and cannot simply count bits.

Solver A's proof is different: a sequence with k doublings and j additions
reaches values at most 2^k·F_(j+2) (F the Fibonacci numbers), and the two
kinds of doubling commute, which bounds the number of distinct states. It
gives the weaker constant.

## What to distrust

- **Everything is by one model family.** Nothing is machine-checked as a
  proof.
- **"For all large m" is very large for the better constant.** Solver B's
  explicit bound drops below 2·2^(0.956·t) only from t = 5,291 (computed
  independently by two reviewers and by the organiser). The referee's report
  says about 9,500; the reviewers traced that figure to rounding in the
  referee's check. The statement about the limit is unaffected, but solver B's
  report states D(m) ≥ (log₂ m − 1)/0.956 without that qualification, and its
  proof file's header drops the factor 2.
- **The result is modest and its method is classical.** The literature
  search, which read the relevant sections of four papers, found the counting
  method in Erdős's 1960 paper on addition chains, and a lemma of the same
  shape as solver A's value bound in Montgomery's note on Lucas chains, where
  it is credited to Knuth. It did not find the statement L > 1 for this
  two-register model, the tables, or the sequences (none is in the OEIS by
  lookup). Nearest models: Lucas chains (Kutz, 2002), which need a difference
  condition and keep all values; Euclidean chains (Herbaut and others, 2010),
  which have no doubling. Knuth's section on addition chains was not seen and
  may settle the "only if" half of the two-1-bits statement. Also not seen:
  the papers of Bergeron, Berstel and Brlek, Floyd and Knuth's "Addition
  machines", and the full texts of the Euclidean-chain and register-count
  papers (snippets only).
- **The guess about the true constant is not supported by a proof.** Both
  solvers guess L is about 1.1 to 1.2, from the growth of N(t) (ratios near
  1.8) and from D(m) staying close to the counting bound. The referee's
  extension shows the gap of 2 becoming common from m ≈ 9,000. Solver A's
  remark that the smallest ratio is "still drifting down" is not borne out by
  the referee's table of minima per octave.
- **A check that cannot fail.** Solver B's `potential_check.py` contains an
  assertion that is always true, tests the key inequality in floating point
  with a tolerance, and samples the final bound only where it is trivially
  satisfied. The referee's exact tests replace it.
- **What the referee did not do.** It did not rerun the solvers' programs.
  That no shorter sequence exists for a given remainder rests on exhaustive
  searches (the solvers' and the referee's two), not on a certificate. D(m)
  beyond 4100 is one method.
- **Error in the chain's question.** Its claim that G(n) = ⌈log₂ n⌉ only for
  powers of two is false (3, 5 and 6 are counterexamples).
- **Process.** Solver A reported that one of its commands contained a stray
  `rm -f /dev/null`, which failed and deleted nothing.
- **Two files over 300 kB** (solver A's path certificate for m ≤ 400 and its
  deliberately broken copy used to test the checker) are left out; the referee's checker accepted the
  certificate.

## Review

Before publication the claims went to five reviewers from four model
families: GPT, Grok and Gemini, and two Claude instances (one of them Claude
Opus). All five confirmed both proofs after re-deriving the cases, the
counting step and the constants; none found a gap. Their findings on this
page, applied above:

- The threshold "about 9,500" taken from the referee is wrong; it is 5,291
  (both Claude seats, independently).
- My justification for "the ratio is 1 with unlimited memory" cited a theorem
  about almost every number, which does not cover a worst case over
  remainders (four seats).
- My sentence that doubling the larger register is the only step that gains a
  full bit, and that it carries no choice, was false when the registers are
  equal and misleading about where the count comes from.
- Solver B's table ends at 4096, so "both solvers to 4100" was wrong (all
  three non-Anthropic seats).

The corrected wording has not been through a second round.

## Files

- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: searches, tables, witnesses, proofs.
- `verifier/`: the referee's programs for N(t), D(m) and the lemma tests.
- `prior-art.md`: the literature search. The downloaded papers are not
  included.

Binaries are left out.
