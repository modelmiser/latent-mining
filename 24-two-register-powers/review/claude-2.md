<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of a Claude seat run on the plan login (one of the two on Claude Opus). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Claude seat 2: experiment 24

All algebra redone by hand; numerics recomputed in my own scratch scripts (N(0..12), D(2..40), H values, gamma, the crossing point of P2's explicit bound). Paths are relative to `24-two-register-powers/` unless they say otherwise.

## Verdicts

**Q1: CONFIRMED.** solver-b/proofs.txt:24-34.
- **A:** M'=2M, r'=r/2, so ΔΦ ≤ 1.
- **B with r ≤ 1/2:** M'=M, r'=2r, so ΔΦ ≤ 1/4.
- **B with r > 1/2:** M'=2rM, r'=1/(2r). This gives ΔΦ = 3/4 + (1/2)log r ≤ 3/4.
- **C:** M'=(1+r)M, r'=r/(1+r). If r' ≥ 1/4, ΔΦ = (3/4)log(1+r). Otherwise r < 1/3, and since f is nondecreasing, ΔΦ ≤ log(4/3).
- **D:** M'=(1+r)M, r'=1/(1+r). The derivative of h has the sign of 2r−1, so h is largest at an endpoint: max(0.7415, 3/4) = 3/4.
- **The four types cover every move.** From {x ≤ y}, a move doubles the larger or the smaller register, or adds the smaller into the larger (C) or the larger into the smaller (D). When x = y, A coincides with B and C with D.
- **Inequality (\*) holds.** Φ(start) = 0 at {1,1}, f ≥ −1/2, so log M ≤ j − k/4 + 1/2.

**Q2: CONFIRMED.** solver-b/proofs.txt:35-41.
- **The bound on k:** log X < log n ≤ j − k/4 + 1/2 gives k < 4(j − log X) + 2 ≤ 4ηt + 2.
- **The word count:** Σ_{k≤K} C(j,k)3^k ≤ 3^K Σ_{k≤K} C(t,k) ≤ 3^K (K+1) 2^{tH(K/t)}. This needs K/t ≤ 1/2, which holds for t ≥ 7.
- **Counting states by type-words is legitimate.** Each type maps an unordered state to a single unordered successor. So a type-word determines the end state, and every path is realised by some type-word. Each state holds at most 2 integers. The factor (t+1) covers the lengths j.
- **The constant:** H(0.176) + 0.176·log₂3 = 0.950201.
- **Why m ≤ N(D(m)):** each residue r has a representative n_r with G(n_r) ≤ D(m), and representatives of different residues are different integers. So m ≤ N(D(m)) ≤ 2^{0.956D+1} once D(m) ≥ t₀, and D(m) ≥ ⌈log₂ m⌉ tends to infinity.

**Q3: CONFIRMED.** solver-a/proofs.txt:15-37.
- **P4:** the induction is valid. In (i), {min, max} ≤ {F_{j+1}, F_{j+2}}. In (ii), the entrywise invariant (a,b) ≤ 2^{k_so_far}(a',b') survives a doubling (the factor 2 is absorbed) and an addition (by linearity).
- **P5:** the maps a↦2a and b↦2b commute. The final state is therefore fixed by the addition letters (2^j choices) and the (#A,#B) counts in each of the j+1 gaps. Splitting k doublings into 2j+2 parts gives C(k+2j+1, 2j+1) ways.
- **P6:** register swap symmetry makes the a-values equal to all values.
- **P7:** at j = ds, the exponents are d + (1+d)H(2d/(1+d)) and 1 − d(1 − log₂φ). I get a max-min of 0.9630956 at d = 0.120698. f1 is strictly increasing on [0, 0.121] and f2 is decreasing, so this is the global max-min.

**Q4: CONFIRMED for the bounds and the mod-m equivalence; UNSUPPORTED for the stated reason behind "ratio 1".**
- **The bounds:** solver-a/proofs.txt:3-13 and solver-b/proofs.txt:8-13 prove them. The binary method costs ⌊log n⌋ + popcount(n) − 1 with the representative n ≤ m.
- **The equivalence:** solver-a P3 (:12-13) and solver-b P0 (:3-6) prove it. Residue 0 is handled automatically because all values are positive.
- **The "ratio 1" claim:** it is true, but README.md:16-17 justifies it with "almost every number … Erdős". A worst case over residues needs a bound for **every** n, namely l(n) ≤ log₂n + O(log n / log log n) (Brauer, 1939). An almost-all statement says nothing about a particular residue class.

**Q5: CONFIRMED, with one range slip.**
- **By hand:** I traced levels 0–4 myself and got N(0..4) = 1, 2, 4, 7, 12, with G(7) = 4 and G(14) ≥ 5. That gives D(2..8) = 1, 2, 2, 3, 3, 4, 4.
- **By script:** my BFS gives N(5) = 20, and N(0..12) agrees with the README.
- **Who computed what:** this matches referee.md:20-22 and the solver reports. The slip: README.md:40 says both solvers computed every m up to 4100, but solver B stopped at 4096 (reports/solver-b.md:15, `D_table.txt` ends at 4096).

**Q6: REFUTED in one sentence.** README.md:67-68 says "double the larger register is the only one that gains a full bit". That is false whenever x = y, a state that recurs (for example {2,2} at t = 2). There B, C and D each double M exactly, so the gain is a full bit, not "almost a full bit" as :70 says. The rest of :49-65 and :73-76 agrees with P2 and P4-P5.

**Q7: one number is wrong; attributions match.**
- **The ≈9,500 threshold is wrong** (README.md:83-84, referee.md:18). It is an artefact of `verifier/lemmas.py:96-106`:
  - That check replaces the first term by `1<<int(0.956t+1)`, which is exactly 2^{0.956t+1} on its t-grid (0.956·500 = 478 is an integer).
  - It then compares log₂ values truncated to about 60 bits (`lg2big`, :33-34).
  - So it reports "holds" only once the second term falls below the precision limit.
- **The correct threshold, recomputed:**
  - P2's displayed entropy-form bound (:39) is ≤ 2·2^{0.956t} for every t ≥ 5291 (the last failure is t = 5290).
  - The binomial-sum form, compared exactly in integers, holds from t ≈ 2380 (the last failure on a step-10 grid is 2370).
- **Other numbers and attributions match their sources:** the solvers' guesses (1.11–1.17 and 1.1–1.2), the gap-2 point m = 9069, the octave minima, the potential_check.py defects (potential_check.py:23 and :38-42), the 4 papers read, and the OEIS result.
- **Novelty:** the README claims only "did not find", which prior-art.md:14,18 supports.
- **Omission:** the README leaves out referee.md:52-54 ("programs not rerun"; the "no shorter path" side rests on exhaustive search).

**Q8: CONFIRMED.** The README.md:52 row (repository root) matches the folder. The constants, "at most 2", "proved two ways" and "computed three times" all agree. It repeats the "factor is 1" claim, so the Q4 caveat applies to it as well.

## Findings

1. **MODERATE**, confidence high. README.md:82-86 (and referee.md:18): the ≈9,500 threshold is an artefact of the referee's check, as traced under Q7.
   Replacement: "The explicit bound in solver B's P2 is below 2·2^(0.956·t) only from t ≈ 5,300 (t ≈ 2,400 if the binomial sum is kept exact), so D(m) ≥ (log₂ m − 1)/0.956 is proved only for m with D(m) at least that large. The referee's figure of about 9,500 came from a check that compared numbers at the limit of floating-point precision. The statement about the limit is unaffected …"

2. **LOW**, confidence high. README.md:67-71: the sentence is false at x = y.
   Replacement: "Only 'double the larger register' raises Φ by a full unit. A step of any of the other three kinds can raise the larger register by a full bit when the registers are equal, and by nearly a full bit when they are close. The second term of Φ charges for this in advance, so on average such steps gain at most 3/4 of a bit. The counting then pays a factor of 3 per such step and nothing for the default step."

3. **LOW**, confidence high. README.md:16-17 and the root README.md:52: the cited theorem does not cover a worst case over residues.
   Replacement: "With unlimited memory the corresponding ratio is 1: every n has an addition chain of length log₂ n + O(log n / log log n) (Brauer, 1939), and each residue has a representative n ≤ m."

4. **LOW**, confidence high. README.md:40: solver B's D(m) table stops at 4096.
   Replacement: "computed by solver A (m ≤ 4100), solver B (m ≤ 4096) and, two ways, by the referee; all agree on the common range".

5. **LOW**, confidence medium. README.md:38: the mod-m/integer equivalence is not cited anywhere in the README's table, and the referee's "not checked" list is not mentioned.
   Replacement: add `solver-a/proofs.txt P3; solver-b/proofs.txt P0` to the row. Add a bullet: "The solvers' programs were not rerun by the referee, and the 'no shorter path' side of every D(m) rests on exhaustive search, not a certificate."

The headline (L ≥ 1/0.956 > 1, and independently L ≥ 1.0383) stands.
