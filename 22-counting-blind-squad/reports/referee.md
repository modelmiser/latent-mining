# Referee report: counting-blind firing squad

My code and outputs: `verifier/` (`ref.cpp`, `ref2.cpp`, `chk.py`, `classes7.py`, `l7_8_*.txt`, `o7_24_*.txt`). All runs ended by 02:22, at most two at a time.

## Verdicts

- **L(3) = 2 (36 rules): CONFIRMED.** Recomputed (every rule; lazy search); B's hand proof read, no gap.
- **L(4) = 3 (232 rules): CONFIRMED.** Recomputed (every rule; lazy search; other length orders).
- **L(5) = 5 (1,244 rules): CONFIRMED.** Recomputed: all 48,828,125 rules enumerated, each run on n = 2..10; none synchronises 2..6.
- **L(6) = 8 (336 rules): CONFIRMED.** Recomputed without symmetry breaking, three search orders.
- **L(7) = 11 (4 classes): CONFIRMED.** Recomputed without symmetry breaking, two search orders: 96 = 4 × 24 rules.
- **Histograms (s = 3..6), single-length counts and minimum times (s = 4, 5): CONFIRMED.** Every entry recomputed.
- **All ten witnesses: CONFIRMED.** My checker; all firing times match.
- **B's list of 1,244 rules: CONFIRMED.** All distinct, all pass 2..5 and fail 6; 23 time vectors.
- **Poser's "t ≥ 2n − 2" is false: CONFIRMED.** 2,487,460 (rule, n) counterexamples at s = 5.
- **General question (is L(s) ever infinite): UNSUPPORTED.** Neither solver offers anything; open.

## L(6) and L(7): how far independent confirmation goes

The two solvers' lazy searches are the same tree: their s = 7 leaf counts are identical (162,484,625). My ascending search has that shape too (same leaf counts as B at s = 5, 6), so I added different designs.

- **No "lowest unused label" rule, s = 7.** Every branch tries all 7 values. Weights sum to exactly 7^17; 3,899,515,471 leaves (23.9993 × the solvers' count). 96 rules synchronise 2..11; none goes further.
- **Different search order, s = 7.** Lengths in the order 3,2,5,4,7,6,9,8,11,10,12, cells scanned right to left, values in reverse, no symmetry breaking. 0 rules synchronise all of 2..12; exactly 96 synchronise 2..11 and fail 12. Weights sum to 7^17.
- **s = 6.** Ascending without symmetry gives 336. Descending order (n = 9 first; 301 M leaves against 6.7 M) gives 0 rules for 2..9 and 336 for 2..8.

Limit: for s ≥ 6 every program of mine is still a lazy backtracking search sharing one simulator (Brent cycle detection). No full enumeration exists for s = 6 (7.8 × 10^10 rules) or s = 7.

## Disagreements between the solvers

None on any value, count or reading. Their witnesses for s = 3..6 differ (all valid; the s = 7 witness is the same rule). Lemma 2 is A's only and the hand proof of L(3) = 2 is B's only; both hold.

## Proofs marked COMPLETE

- **A Fact 0 / B P2:** cell j is Q before time j − 1, so t ≥ n − 1. No gap found; 0 violations over all rules, s = 3, 4, 5.
- **A Lemma 1 / B P1:** a rule synchronising n ≥ 3 has d(Q,1) ∉ {Q, F}. No gap found; 0 violations, s = 3, 4, 5.
- **A Lemma 2:** if d synchronises n and m > n, then t(n) ≥ 2n − 1 or t(n) = t(m). No gap found. At s = 5: 549,534 pairs, 0 violations, 320 in the equal-time branch.
- **B P3:** L(3) = 2. No gap found; each case re-derived.

## Errors and weak checks

- **A's Lemma 2 check is weaker than stated.** "Zero violations for s = 3, 4" is vacuous at s = 3 (no rule synchronises two lengths) and at s = 4 never reaches the equal-time branch (0 of 511 pairs). Only s = 5 exercises it.
- **B's P2 remark is loose.** "witness_s4-type rules fire n = 3 at t = 3": B's own s = 4 witness fires n = 3 at t = 7; other 4-state rules do reach 3.
- **"4 classes" at s = 7** is correct for relabelling unnamed states. All four share one firing-time vector, and classes 1 and 2 have identical space-time diagrams from time 1 under a renaming that moves G. Classes 3 and 4 do not match class 1 that way.

My checker rejected eight broken witnesses (changed entry, wrong time, understated L, d(Q,0) ≠ Q, wrong state count, d(Q,1) = F, omitted length, entry at F) and accepted the unbroken one.

One bug of my own: the chunked permuted-order program first counted shallow leaves in every chunk. The weight-sum check caught it; fixed before the s = 7 run.

## Problem as written

Both solvers answered the stated problem under the literal reading (k counts existing neighbours only). I see no ambiguity. Nobody, including me, computed the reading where a missing neighbour counts as different.

## Not checked

- s = 8; full enumeration for s = 6 or 7.
- A's `check.py` and B's `slow.py` line by line (I read the C++ searches).
- Whether the four s = 7 classes are one dynamics under some later-time renaming.

## Follow-ups

1. **Compute L(8) exactly.** Lazy search with symmetry breaking, chunked across cores; cross-check its s = 7 behaviour against `l7_8_*.txt` (96 rules at prefix 11). Expect hours: my unpruned s = 7 took 27 CPU-minutes, the pruned ones 70–164 s.
2. **Prove or refute sup L(s) = ∞.** Either give an explicit family of rules, with s(N) states, synchronising every n in 2..N, or prove a uniform bound.
