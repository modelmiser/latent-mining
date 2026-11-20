# Referee report

**Headline: r(3) is finite, 16 ≤ r(3) ≤ 36.** Solver A's 36-card witness is valid under the problem's definition; solver B's guess r(3) = ∞ is refuted. The exact value is open, and the lower bound 16 rests on the solvers' m = 14, 15 runs, which I did not repeat.

My code and logs are in `verifier/`.

## Verdict per claim

| claim | verdict | how |
|---|---|---|
| E(3,36) = 1 (A, `w_n3_m36.txt`) | CONFIRMED | witness checked with my own checker `vcheck.py` |
| E(3,132) = 1, E(2,5) = 1 (A) | CONFIRMED | witnesses checked |
| r(3) = ∞ (B's guess, 55%) | REFUTED | the 36-card witness |
| r(2) = f(2) = 5; E(2,m) = 1 for m ≥ 5 | CONFIRMED | raw recompute m ≤ 6; A's family checked m = 5..25; proof read |
| min \|R\| for n = 2, m = 5..12 (B) | CONFIRMED | recomputed |
| E(3,m) = 0, m ≤ 13 | CONFIRMED | recomputed |
| E(3,m) = 0, m = 14, 15 | UNSUPPORTED by me | both solvers agree; not rerun |
| weak-reading r(3) = 12 | CONFIRMED | recomputed; 216 and 1152 pairs at m = 12, 13 equal B's |
| f(n) = ∞ for n ≥ 3 (B T2) | CONFIRMED | proof read; identity tested numerically |
| r(n) ≥ max(2n, 2ⁿ−1) (B T1) | CONFIRMED | proof read; checked on the witness |
| \|R\| ≥ 5 for n = 3 (B T4); \|R\| ≤ m−2 (B C3) | CONFIRMED | proofs read |
| E(4,m) = 0, m ≤ 14 | CONFIRMED | follows from T1; recomputed m ≤ 11 |
| E(3,36k) = 1 (A L5') | CONFIRMED | proof read; tested on the five-card trick, b ≤ 5 |
| A's strand-family exclusions | UNSUPPORTED | logs match the report; nothing recomputed; the a ≤ 5 part is floating-point LP |

## The witness

- For m = 36, M(x) is identical on the seven AND-0 inputs (33 distinct words); M(111) has 16 distinct words, none shared.
- Nine broken variants are rejected: flipped player card, flipped house card, R minus one element, R with one element moved, R = Z₃₆, changed owner, constant player, empty R, u0/u1 swapped.

## Disagreements and resolution

1. **Is r(3) finite?** A is right, by the witness.
2. **f(3).** A conjectures f(3) = ∞; B proves f(n) = ∞ for n ≥ 3. B is right. A's L6 stalls on the "antipodal" frequency type because it uses the shift a₁+a₂+a₃; B's shift s₁₂+s₁₃+s₂₃−s₁−s₂−s₃ absorbs it. My numeric test (m ≤ 40) found 3519 frequency solutions, 1140 antipodal, no violation.
3. **r(4).** A says ≥ 14 (computed), B says ≥ 15 (T1). B's bound is correct and stronger.
4. **\|R\|.** A proves ≥ n, B proves ≥ n+1 and ≥ 5 for n = 3. No conflict; the witness has \|R\| = 6.
5. **Reading.** No disagreement: both took correctness as disjoint supports and reported the weak reading separately.

## Recomputed table

- `raw.cpp` (definition, no reduction) and `red.cpp` (complement form, balance, subset-inherited privacy) agree on valid and weak counts for m ≤ 6.
- I differ from neither solver on any entry.

## Proofs marked COMPLETE

No gap found in any of them.

- **A L1 / B L0**: complement form is WLOG.
- **A L2 / B L1**: flipping sets are even and balanced, so m ≥ 2n.
- **A L3 / B T5**: every translate of R meets every flipping set. Holds on the witness.
- **A L4**: E(2,m) = 1 iff m ≥ 5.
- **A L5, L5'**: strand decomposition; multiples of a valid size are valid.
- **A L6**: for R = Z_m, w(111) is homometric to w(000).
- **B T1**: some S ⊆ R realises every non-zero parity vector, so m ≥ 2ⁿ−1.
- **B T2**: seven AND-0 rotations force w(1ⁿ) to be a rotation.
- **B C3**: \|R\| ≤ m−2 for n ≥ 3.
- **B T4**: \|R\| ≥ n+1, and ≥ 5 for n = 3. I re-derived both sign systems.

## Errors and weak checks

- B's `fullR.cpp` test of T2 cannot fail for n = 3: it finds zero triples. B says so.
- B's stated obstruction (zero-error correctness) is wrong, along with the guess.
- A understates r(4) and leaves f(3) open although a short proof exists.
- All three exhaustive programs use the same normalisation, so equal leaf counts confirm the enumeration, not the pruning.

## Not checked

- E(3,14) and E(3,15).
- A's strand-family minimality claims, including "no a = 6 solution with b ≤ 5".
- Anything for m = 16..35, and n = 4 beyond T1.

## Follow-up tasks

1. **Strand family below 36.** Decide whether a valid n = 3 protocol exists with m = ab < 36 and R inside the subgroup of order a, for the unsearched cases: (a,b) = (7,5); (11,2), (11,3), (12,2) with partial R; (13,2) through (17,2). Outcome: a witness below 36, or "36 is the strand-family minimum".
2. **E(3,16), exhaustive.** About 8× the m = 15 cost (roughly 6 CPU-hours at B's speed), with \|R\| ≥ 5 and \|R\|·\|D_i\| ≥ m as extra pruning. This raises the lower bound to 17 or finds a non-strand protocol.
