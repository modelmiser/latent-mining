# Prior art: two-register double/add chains (G, N(t), D(m), L)

## CLOSEST PRIOR WORK

1. **M. Kutz, "Lower Bounds for Lucas Chains", SIAM J. Comput. 31 (2002)** — http://www.mpi-inf.mpg.de/~mkutz/pubs/Kutz_LucasChains.pdf. Theorem 4: only O(N^(1-rho)) of n in [N, 2N-1] have a Lucas chain shorter than (1-eps) log_phi n. Model does not match: Definition 1 requires "a_k = a_j + a_i and a_j - a_i in {0, a_0, ..., a_{k-1}}" with unlimited memory; here there is no difference condition and only two values are kept.
2. **P. L. Montgomery, "Evaluating recurrences ... via Lucas chains" (1983/1992, unpublished)** — https://cr.yp.to/bib/1992/montgomery-lucas.pdf. Lemma 4: "If the Lucas chain has exactly d doubling steps, then n <= 2^(d-1) F_(r-d+3)", cited to Knuth vol. 2 p. 448; the same shape as solver-a's P4. Differs: "doubling" means a_i = 2a_(i-1), in increasing chains with full memory.
3. **P. Erdős, "Remarks on number theory III. On addition chains", Acta Arith. 6 (1960)** — https://users.renyi.hu/~p_erdos/1960-13.pdf. Almost all n have l(n) = log2 n + (1+o(1)) log n / log log n, by counting chains with few non-doubling steps. Differs: unrestricted memory, so the constant is 1; both solvers' proofs are this counting method on a smaller class.
4. **De Koninck, Doyon, Verreault, "On the minimal length of addition chains", arXiv:2504.07332 (2025); "On the distribution of the minimal length of addition chains", arXiv:2609.28374 (2026)** — https://arxiv.org/abs/2504.07332, https://arxiv.org/pdf/2609.28374. Bound F(m, r) = #{n in [2^m, 2^(m+1)) : l(n) <= m + r}. Differs: the nearest published analogue of N(t), but for ordinary chains.
5. **Herbaut, Liardet, Méloni, Téglia, Véron, "Random Euclidean Addition Chain Generation and Its Application to Point Multiplication" (2010)** — https://hal.archives-ouvertes.fr/hal-00674251 (snippet only). Pairs from (1,1) with (u,v) -> (u,u+v) or (v,u+v). Differs: no doubling moves.
6. **Sauerbrey, Dietel, "Resource requirements for the application of addition chains in modulo exponentiation", EUROCRYPT '92** — https://link.springer.com/doi/10.1007/3-540-47555-9_15 (snippet only). Treats register count as the resource; binary method needs two registers. Differs: heuristics, no counts.

## VERDICTS

- **L > 1 (N(t) <= 2^(0.963t), resp. 2^(0.956t+1); L >= 1.038 / 1.046)**: NOT FOUND; technique is that of items 2–3.
- **Solver-a P4 (value <= 2^k F_(j+2))**: LIKELY EASY COROLLARY of Knuth 4.6.3 Theorem A as quoted in Montgomery's Lemma 4; doubling notions differ, so not literal.
- **ceil(log2 m) <= D(m) <= 2 floor(log2 m); N(t) <= 2^t**: FOLKLORE (binary method; Montgomery: "L(n) <= L_b(n) <= 2 lg n ... L(n) >= l(n) >= lg n").
- **G(n) = ceil(log2 n) iff at most two 1-bits**: "if" is folklore. "Only if" (computed only) probably follows from G(n) >= l(n) plus Knuth's Theorem 4.6.3C, whose text I did not see: UNCHECKED. Schönhage's bound (log2 n + log2 nu(n) - 2.13) alone is too weak.
- **D(m) table, non-monotonicity, D - T(m) in {0,1,2}, lambda near 1.8**: NOT FOUND.

## SEQUENCES (OEIS search; Fibonacci control returned A000045)

- N(t) = 1,2,4,7,12,20,34,57,98,169,295,521,912: no match.
- D(m) = 1,2,2,3,3,4,4,4,4,5,5,5,5,6,6,5,6,...: no match.
- First m with D = d (11,15,27,39,71,119,191,369) and last m (17,26,41,73,131,209,389,561): no match.
- G(n), computed by me for n <= 39: 22 terms match only A014701 (binary-method cost); 32 terms match nothing. G(23) = 6, A014701(23) = 7.

## QUERIES (21)

OEIS terms; "(2a,b)" "(a+b,b)" two registers; restricted-memory chains; Lucas/binary chain counts; Euclidean chains with doubling; width-2 straight-line programs; Kutz; x^n in two registers; Erdős 1960; registers/pebbling; matrix semigroup diag(2,1); Montgomery; in-place x:=x+y, x:=2x mod m; Cayley diameter Z_m; Herbaut; Bergeron–Berstel–Brlek; Sauerbrey–Dietel; two memory cells; Knuth 4.6.3 nu(n) >= 3; residue-class l(n); reversed binary GCD.

## WHO TO ASK

An addition-chain number theorist (De Koninck, Doyon, Verreault), or the Lucas-chain / ECM community.

## NOT CHECKED

Knuth TAOCP 4.6.3 text and exercises; Bergeron–Berstel–Brlek papers; Floyd–Knuth "Addition machines"; Springer and HAL full texts (downloads returned HTML). Read cover to cover: 0. Downloaded to deep/: 5 PDFs; relevant sections read in 4 (Kutz, Montgomery, Erdős, arXiv:2504.07332), abstract and introduction only for arXiv:2609.28374.
