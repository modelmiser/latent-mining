<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of a Claude seat run on the plan login (one of the two on Claude Opus). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Claude seat 1: experiment 24 (two registers)

Paths are relative to `24-two-register-powers/`. All algebra below was redone by hand; numerics by my own throwaway scripts outside the repository. No repository program was run.

## Verdicts

**Q1. CONFIRMED** (`solver-b/proofs.txt:18-34`). Re-derived:
- A: M' = 2M, r' = r/2, dPhi = 1 + f(r/2) - f(r) <= 1.
- B, r <= 1/2: M' = M, r' = 2r, dPhi <= 1/4. B, r > 1/2: M' = 2rM, r' = 1/(2r), dPhi = 3/4 + (1/2) log r <= 3/4.
- C: M' = (1+r)M, r' = r/(1+r); r' >= 1/4 gives (3/4) log(1+r) <= 3/4; r' < 1/4 means r < 1/3 and dPhi <= log(4/3) = 0.415.
- D: M' = (1+r)M, r' = 1/(1+r); r <= 1/4 gives <= (3/4) log(5/4) + 1/2 = 0.7415; on [1/4, 1] h' has the sign of 2r - 1, endpoint maximum h(1) = 3/4.
- The four types are exactly the four moves on an unordered pair; at x = y all four give {x, 2x} with dPhi = 3/4, so either label is safe. Phi(1,1) = 0 and f >= -1/2, so (*) holds, start state included.

**Q2. CONFIRMED** (`solver-b/proofs.txt:35-41`). log X < log n <= log M <= j - k/4 + 1/2 gives k < 4(j - log X) + 2 <= 4 eta t + 2. A type-word determines the unordered state (each type is a function of {x <= y}), so counting states by words is legitimate, 2 integers per word. Binomial sum, 3^K and the entropy bound are right for K/t <= 1/2 (t >= 7). H(0.176) + 0.176 log2 3 = 0.950201. m <= N(D(m)): one representative n_r per residue with G(n_r) <= D(m), all distinct (`:10`). D(m) >= log2 m tends to infinity, so the bound follows for large m; 1/0.956 = 1.04603.

**Q3. CONFIRMED** (`solver-a/proofs.txt:15-37`). P4: induction is valid (ordered pairs, additions monotone and linear). P5: A and B commute as maps; 2^j C(k+2j+1, 2j+1) is right for 2(j+1) bins. P6: swap symmetry preserves (s, j). P7: both exponents are right; my bisection gives d* = 0.1206975, gamma = 0.9630958, 1/gamma = 1.038318; the crossing is the max-min.

**Q4. CONFIRMED for the proofs; the unlimited-memory sentence is true but its stated reason is insufficient** (finding 2). Easy bounds and equivalence, residue 0 included: `solver-a/proofs.txt:3-13`, `solver-b/proofs.txt:3-13`.

**Q5. CONFIRMED with one attribution error** (finding 3). My own BFS: N(0..8) = 1, 2, 4, 7, 12, 20, 34, 57, 98; D(2..40) equals solver B's list (`reports/solver-b.md:15`), so D(2..8) = 1, 2, 2, 3, 3, 4, 4. N(0..4) also by hand.

**Q6. CONFIRMED except one sentence** (finding 4). Every other sentence of `README.md:47-76` matches P2 or P4-P5.

**Q7. One number REFUTED** (finding 1), rest CONFIRMED. Counts check: 358 + 3738 + 3 = 4099; 558 + 37268 + 2173 = 39999; 254 = 23 + 231; 80199 certificate lines = sum of m for 2..400. Novelty claims do not exceed `prior-art.md:14-18`.

**Q8. CONFIRMED**, subject to findings 2 and 3.

## Findings

**1. MODERATE. "Only from t around 9,500" is wrong.** `README.md:82-84`, source `reports/referee.md:18`. Confidence: high on the numbers, medium on the cause.
The bound exactly as displayed at `solver-b/proofs.txt:39` is at most 2·2^(0.956t) for every t >= 5291 (last failure t = 5290; checked to 30,000, margin growing). With the binomial sum of `:37-38` in place of the entropy estimate, the last failure is t = 2466 (exact integers).
Likely cause, read from `verifier/lemmas.py:96-106` and not run: the first term is rounded up to `1<<int(0.956*t+1)`, and the grid above 3000 is multiples of 500, where 0.956t is an integer, so that term already equals 2^(0.956t+1) and the test can pass only once the second term vanishes in double precision. At t = 9500 the second term is 2^-39.7 of the first, which is about where that happens.
Replacement: "Solver B's explicit bound drops below 2·2^(0.956·t) only from t = 5291 (from t = 2467 if the binomial sum is kept instead of the entropy estimate); the referee's figure of about 9,500 is an artefact of rounding in its check." The qualitative caveat stands.

**2. LOW. Unlimited-memory justification.** `README.md:16-18`, top-level `README.md:52`. Confidence: high.
The claim (ratio 1) is true, but "almost every number" does not give a worst case over residues: that needs a short chain for some representative of every residue. The needed fact is Brauer's bound (1939), l(n) <= log2 n + (1 + o(1)) log2 n / log log n for every n, applied to n = r or n = m. Erdős's theorem is the matching lower bound for almost all n, as `prior-art.md:7` describes it. Brauer is not in `prior-art.md`; I cite it from memory.
Replacement: "With unlimited memory the corresponding ratio is 1: ordinary addition chains reach every n in log₂ n plus lower-order terms (Brauer, 1939)."

**3. LOW. Solver B's D table ends at 4096.** `README.md:40`; `reports/solver-b.md:15`; `solver-b/D_table.txt` last line is `4096 16`. Confidence: high.
Replacement: "computed by solver A (to 4100), solver B (to 4096) and, two ways, by the referee (to 4100); all agree where they overlap". The top-level "tables computed three times" is loose for the same reason, and because N(22) has two computations (`reports/solver-b.md:7`).

**4. LOW. "It carries no choice."** `README.md:67-68`. Confidence: medium.
This is not in P2 and is misleading: in the count, the largest factor, 2^(tH(K/t)) (0.671 of the 0.950 exponent), is the choice of which steps are type A. Also, "gains a full bit" is true of the larger register, not of Phi (A raises Phi by only 3/4 when r >= 1/2).
Replacement: "The step 'double the larger register' is the only one that always adds a full bit to the larger register. A sequence that reaches a large number can contain few other steps, and a sequence is fixed by where those steps sit and which of the three kinds each is; that is what makes such sequences rare."

**5. LOW. Unchecked sources understated.** `README.md:95-96` names only Knuth; `prior-art.md:37` also lists Bergeron–Berstel–Brlek, Floyd–Knuth "Addition machines", and the Herbaut and Sauerbrey–Dietel full texts (snippets only). Confidence: high.
Replacement: add "Also not seen: Bergeron–Berstel–Brlek, Floyd and Knuth's 'Addition machines', and the full texts of the Euclidean-chain and register-count papers (snippets only)."

## Checked and found correct

Both proofs (C1, C2), every Status cell other than the D(m) row, the N(t) list against `solver-a/nt22.txt`, `verifier/lev22.out` and `solver-b/gbfs21.out`, the 0.9502, 1.046 and 1.0383 constants, the D/log₂ m range (1.291 to 1.405 for 1000 <= m <= 4100 from `solver-a/D_all.txt`), the `potential_check.py` bullet (`solver-b/potential_check.py:24`), the m = 9069 onset (`verifier/levD.out`), the chain-error and process bullets.
