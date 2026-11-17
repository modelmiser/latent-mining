# Referee report: game W(k, m)

**Bottom line: F(k, m) = max(m + 1, 2m + 1 − ⌊k/2⌋) stands.** No gap in any of the four bound proofs; my solver reproduces and extends every table entry; both solvers' strategies pass my checker. The errors found are in the solvers' checking code and files, not the mathematics.

Code: `verifier/wk.cpp` (C++, from the problem statement); outputs beside it.

## Verdicts

| Claim | Verdict | How |
|---|---|---|
| Table values (A: k ≤ 13, B: k ≤ 10; m ≤ 6) | CONFIRMED | Recomputed, zero differences |
| Upper bound, A (FILL-FAR, via Belady) | CONFIRMED | Proof read, no gap; strategy checked |
| Upper bound, B (runs, offline entries) | CONFIRMED | Proof read, no gap; same strategy |
| Lower bound, A (Theorem L) | CONFIRMED | Proof read, no gap; adversary checked |
| Lower bound, B (explicit offline cache) | CONFIRMED | Proof read, no gap; adversary checked |
| Pruning lemmas (both) | CONFIRMED | Proof read; pruned = unpruned values |
| A's Lemmas 1–3 | CONFIRMED | Proof read; all walks of length ≤ 14, k ≤ 8 |
| The problem's beliefs | CONFIRMED | All true; F = m + 1 already from k ≥ 2m |

**Table.**
- Domination pruning (the solvers' rule): all 78 entries for k = 2..14, m = 1..6; m = 7 for k ∈ {2, 3, 4, 10..16}; m = 8 for k ∈ {2, 3, 14..18}. All equal the formula.
- This replaces A's strategy-only entries (row m = 6, column k = 13, (7,5)) and B's gaps (k = 5..9 at m = 6) with full game values.
- Superset-only pruning agrees on k = 5..10, m = 3..5; unpruned agrees on 22 cases up to (8,4).

**Proofs.**
- *A, upper*: FILL-FAR faults ≤ max(opt + 1, 2·opt − j + 1). Belady is applied only to no-prefetch schedules, which is ordinary paging; the two-page load is absorbed first by Lemma 2 (a no-prefetch schedule with ≤ 2·opt faults).
- *B, upper*: the entry count holds. In [t0, t1] the walk visits k + j distinct pages; offline holds at most k − 1 of them besides x0 after t0, so ≥ j enter in (t0, t1]; the intervals are disjoint.
- *A, lower*: at most j − 1 free new pages while filling, then one offline fault per block against ≥ 2 for Online. Both parities and the case 2m ≤ k − 1 check out.
- *B, lower*: 2·min(m, h) − 1 straight steps, then cycles costing offline 1 and Online ≥ 2. The size argument in case (ii) also holds for |O| = k − 1.

**Strategies, checked by my program.**
- FILL-FAR (identical to B's ALG) against every adversary: worst case equals the formula for k ≤ 14, m ≤ 6.
- Both adversaries, re-implemented from the proofs, against every Online reply: minimum Online faults equals the formula for k ≤ 13, m ≤ 6, and with unpruned Online moves on 12 cases each.
- In every explored node: 0 illegal requests (exact opt ≤ m), 0 zero-fault infinite plays, and the offline cache each proof claims exists at the claimed cost.
- All 48 sample plays in the witness files are accepted.

**Broken witnesses rejected.**
- Online: always-prefetch (unbounded), never-prefetch (2m), evict-nearest (unbounded).
- Adversaries: A's with cached-frontier preference, without bounce, with one extra block; B's with one cycle too few or too many.
- Six corrupted walk lines.
- Weakened Lemmas 1–3 and per-walk bound.

## Disagreements between the solvers

None on values, formula or interpretation (round-1 fault counts; legality tested on the history including the new request); I read the statement the same way. Only coverage differed.

Both game solvers use least-fixpoint value iteration from 0 with "stop" as the baseline, which is correct for infinite play and stopping.

## Errors found (none changes a result)

1. **B, `verify.py` `check_adv`**: a revisited state returns True, so a zero-fault infinite play would be accepted; that check cannot fail.
2. **A, `adv.py`**: Dijkstra stops at the first terminal, so legality is asserted only on nodes popped before it, and a zero-fault cycle would go unnoticed.
3. **A, `witnesses.txt`**: header glued to the first data line; 12 of 24 rows have `opt_unpruned None`; every walk is a straight line, so the samples never exercise ADV's bounce or stage 2.
4. **A, `game.py`**: prints "cap 2m+4" but uses 2m + 3.
5. **B, report**: "unpruned agrees on k = 2..4, m = 1..3" has no log; `table_py_noprune.txt` holds only k = 3..8, m = 2..4.
6. **A, report**: Lemma 3 is complete only given Belady's theorem, cited and not proved. B's upper bound is self-contained, so the headline does not depend on it.

## Not checked

- No machine-checked proof.
- Unpruned game values beyond (8,4); m = 7 for k = 5..9 and most of m = 8.
- The solvers' programs were read, not re-run; Belady's theorem itself.

## Follow-up tasks

1. **Two-sided prefetch.** Change the rule to C' ⊆ C ∪ {p − 1, p, p + 1}, everything else unchanged, and determine F₂(k, m) exactly: tables for k ≤ 10, m ≤ 5, then the formula.
2. **Formalise B's proof.** Machine-check (e.g. in Lean) the Belady-free upper bound and the cycle lower bound.
