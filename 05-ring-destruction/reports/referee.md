# Referee report — first to know everything, destruction budget k

Files: `verifier/`.

## Main ruling

**Solver A is right.** A's Theorem U is complete, so F(n,k) = n − ⌈n/k⌉ is proved for all n > k ≥ 3. B's bracket is correct but weaker than what is provable; F(10,4) = 7 is proved by A, not open.

## Verdicts

1. **Table, n = 3..12, k = 1..4** — CONFIRMED, recomputed. My layered BFS over knowledge sets reproduces every entry of both solvers; no differences.
   - n ≤ 8: full value search; agrees with a no-pruning, no-symmetry run for n ≤ 6.
   - n = 9..12: exhaustive NO at T = F for every k ≤ 4 (including (12,3,8), (12,4,9)); YES from checked witnesses, and from my own search for (9,3), (9,4), (10,4).
   - Large k: nine entries recomputed (n ≤ 8, including (4,7), (6,11), (7,7)); all agree.
2. **Witnesses** — CONFIRMED with my checker. All 367 files (A `wit/`, `witgen/`; B `surv_`, `witness_`, `sawtooth_`) are legal and survive as claimed. Six broken variants were rejected: repeated link, over budget, non-link, understated k, illegal extra round, one hit removed.
3. **Lower bound F ≥ n − ⌈n/min(k,n)⌉** — CONFIRMED. Both strategies, implemented from their written descriptions, give first completion exactly at the formula for 3 ≤ n ≤ 70, 2 ≤ k ≤ 14 (884 pairs each, including k ≥ n). A's proof: read, no gap. B's proof: outline only; its hypothesis T ≥ g is not needed in practice.
4. **A's Theorem U and corollary (F ≤ n − ⌈n/k⌉, n > k ≥ 3)** — CONFIRMED, read line by line, no gap found.
   - I redid the residue algebra (n = kq − r). Even t0 reduces to (q−1)(k−3)+2 > 0; odd t0 to q(k−3)+4k+3−5r > 0. The exceptions (k = 3, r = 2 forces t0 even; q = 2 gives w = 0) are handled correctly.
   - Mechanical test: every lemma and intermediate inequality checked at every surviving prefix of 11,700 plays (48,800 prefixes; Theorem U applicable at 5,900): all solver witnesses, 4,348 optimal plays from my solver (n ≤ 8), both strategies for n ≤ 40, 6,000 random plays. Zero failures.
   - The tester can fail: eight mutants were all caught.
   - My own evaluation of (E)/(O) never excludes a survivable t for any computed entry, including k ≥ n, and excludes t0 but not t0 − 1 for all 21,983 pairs with 3 ≤ k ≤ 40, k < n ≤ 600.
5. **B's Lemmas E, D, Φ, Theorem U, Corollary U′** — CONFIRMED, read with no gap and tested on the same plays. They give c(k) = 1 − 1/k independently of A.
6. **F(n,1) = F(n,2) = ⌊n/2⌋** — CONFIRMED. Both proofs read, no gap; recomputed to n = 11.
7. **c(k) = 1 − 1/k for k ≥ 3** — CONFIRMED; the limit exists by either solver's bounds.
8. **k ≥ n** — A proves odd n only; B only computes. I close the gap with A's L3. For even n and t = n − 1, survival needs d + e ≥ n with d, e ≤ H = n/2, so all 2n pairs of value H exist; all are created in round n − 1, needing 2n destructions in one round. Hence F = n − 1 for n ≤ k < 2n and F = 2⌊n/2⌋ for k ≥ 2n, matching every computed entry. This is my own argument, not reviewed by anyone else.

## Disagreements

- **Exact formula versus bracket.** Resolved for A. B's odd-T case lets both directions reach height ⌈T/2⌉; A adds that pairs of the top value are created only in the last round, so there are at most k of them.
- No disagreement on values or on the reading of the problem.

## Errors found

- B: "first entry the proofs leave open: F(10,4)" is wrong. B's Theorem U also fails to exclude T = 3 for (5,4): 2nh = 20 ≤ kT(2h+1−m) = 24. F(5,4) is the first open entry in B's table; (7,5) and (7,6) are open too, and 155 pairs for 3 ≤ k ≤ 12, k < n ≤ 60.
- The solvers' search witnesses are byte-identical where compared ((8,4), (9,3), (10,4)), presumably from the same lexicographic search order. Valid, but not independent evidence.
- A's third "must-fail" test is only a wrong expected number against a legal file.

## Problem reading

Both took per-directed-link budget and no-repeat rule, completeness tested after round t's deliveries, F = 1 + longest survivable prefix. That is the statement as written; no easier neighbour was substituted.

## Not checked

- The solvers' C++ line by line; their large-k tables beyond those nine entries.
- B's Theorem L proof in detail (simulation only); B's Section P (my solver uses only useful-links and half-speed pruning, re-derived myself).
- My YES search for (10,3,5) timed out at 1500 s; that YES rests on checked witnesses.

## Follow-ups

1. Cross-family review or formalisation of A's Theorem U plus verdict 8. Statement: for all n ≥ 3, F(n,k) = ⌊n/2⌋ for k ≤ 2; n − ⌈n/k⌉ for 3 ≤ k < n; n − 1 for n ≤ k < 2n; 2⌊n/2⌋ for k ≥ 2n.
2. Last completion: G(n,k) = maximum over legal sequences of the least t at which all processes are complete. Compute for n ≤ 10, k ≤ 4 and find lim G/n.
