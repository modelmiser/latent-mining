# Referee report: charged pebbles, clean delivery

Files: `verifier/` (`real.cpp`, `relax.cpp`, `relaxj.cpp`, `wcheck.py`, `ccheck.py`, `runw.py`, and `*.log`).

**Bottom line:** nothing either solver claims was refuted. The headline stays "YES in every case computed, not proved": the upper bound for p ≥ 3, k ≥ 1 is open, and both solvers say so.

## Verdicts

| Claim | Verdict | How |
|---|---|---|
| C(p,k) ≥ (pk+2)·2^(p-2), all p ≥ 2, k ≥ 0 (A's P1, B's Theorem 1) | CONFIRMED | Both proofs read, no gap; 39 move lists replayed by my checker |
| C(2,k) = 2k+2 (A's P2, B's Theorem 2) | CONFIRMED | Both proofs read, no gap; recomputed k ≤ 10 and 15 |
| C(p,0) = 2^(p-1) (B's Lemma W) | CONFIRMED | Proof read, no gap; recomputed p ≤ 5 |
| Relaxed game (pooled budget f = pk) is an upper bound on the real game | CONFIRMED | Proof read: P and R are identical, and each F-move lowers the total charge by one, so every real play is a legal relaxed play |
| Real table: p=2 (k ≤ 10, 15), p=3 (k ≤ 8, 15), p=4 (k ≤ 4), p=5 (k ≤ 1) | CONFIRMED | My uncapped search, 29 runs; value, highest node and state count identical to the solvers' |
| C(4,5)=88, C(4,6)=104, C(5,2)=96 | CONFIRMED by another route | My relaxed search (upper) plus the proved strategy (lower); their 39M–118M-state runs not rerun |
| C(3,k ≤ 20), C(4,k ≤ 10), C(5,k ≤ 5) (B) | CONFIRMED | My relaxed search: q=2 f ≤ 100, q=3 f ≤ 60, q=4 f ≤ 40, q=5 f ≤ 25; clean and top match conjecture R at every f; state counts match B's |
| C(6,1) = 128 (B) | UNSUPPORTED by me | Lower bound replayed. Upper bound rests on B's one run of 121,871,315 states, not rerun |
| Highest node = C + 2^(p-1) − 1 | CONFIRMED | Every case I ran |
| A's seven unequal-charge values | CONFIRMED | Recomputed |
| B's conjecture J on the data | CONFIRMED, extended | Zero violations for q=3 f ≤ 40, q=4 f ≤ 25, q=5 f ≤ 25 |
| B: {2,6,7} needs one F-move | CONFIRMED | My search: minimum 1 |
| Headline for all p ≥ 3, k ≥ 1 | UNSUPPORTED | No proof |

## Proofs marked COMPLETE

- **A's P1** claims cyclic rotation reaches (pk+2)·2^(p-2). No gap found. The reversed pass touches only nodes whose state is unchanged; the base pebble in round j has been forgotten floor((j−1)/p) ≤ k−1 times.
- **A's P2** claims C(2,k) ≤ 2k+2 by an invariant on lone pebbles and pairs. No gap found; all move cases checked.
- **A's P4** claims the position cap is harmless when node N is never occupied. No gap found; my search has no cap and agrees.
- **B's Theorem 1** claims the same lower bound with a maximal-charge hand pebble placed on top. No gap found; the {c, c−1} invariant holds in both cases.
- **B's Lemma W** claims q pebbles above a fixed base keep the lowest ≤ base + 2^(q-1) and all ≤ base + 2^q − 1. No gap found.
- **B's Theorem 2** claims the lowest pebble is ≤ 2 + (F-moves so far) for two pebbles. No gap found: with both pebbles down no placement is possible, so y−1 leaves by an F-move.

## Disagreements between the solvers

None on values, formulas or interpretation.

- A proves the k=0 upper bound only for p=2; B proves it for all p.
- Only B has the relaxed game; only A has unequal charges.
- Their strategies differ (cyclic rotation against maximal charge); both are valid.
- Confidence 90% against 95%; neither figure rests on more than the tables.

## Errors and blemishes

- No wrong constant or misstated lemma found.
- B's "the only binding constraint is the total pk" holds for equal charges only. A's data (charges 3,0,0 give 6, not 10) shows the per-pebble limit can bind.
- A's `slow.py` counts states with pebble identities (3,086 for charges 1,1,1) against 530 in the main search; "where comparable" covers this but is easy to misread.
- A's folder holds a stray file named `-`, a copy of a (4,1) move list.
- Both ship a certificate over the 300 kB guideline, and say so.

## Witness and certificate checks

- All 20 of A's and 19 of B's move lists pass `wcheck.py` at n = formula; each is rejected at n+1.
- Rejected on purpose: A's two BAD files, B's two negative witnesses, and nine of mine (F relabelled R, R relabelled F, k=0, deleted line, empty predecessor, double placement, p=2, truncation, F on a charge-0 pebble).
- All 15 closed-set certificates pass `ccheck.py`: start present, closed, maximum clean = formula.
- Rejected on purpose: both solvers' broken certificates and four of mine (wrong k, last line dropped, middle line dropped, clean-at-10 states removed).

## Reading of the problem

Both took the text literally: F only where the predecessor is empty, node 1 always removed by R, charge per pebble. I found no second reading that changes a value.

## Not checked

- Real-game searches for (4,5), (4,6), (5,2); relaxed q=6 at any f.
- The solvers' code line by line, beyond `relaxed.cpp` and part of `fast.cpp`.
- `conj.py` (J retested with my own program instead).

## Follow-up tasks

1. **Prove the relaxed bound for three pebbles.** With 3 uncharged pebbles and at most f F-moves, no clean node exceeds 2f+4. Look for an invariant on the three positions by fitting linear inequalities to the full reachable set (18,798 sets at f ≤ 60) and testing each move.
2. **Find the unequal-charge function for p=3.** Give the exact clean reach for charges (a,b,c), by search for a+b+c ≤ 12 plus a fitted formula. It shows what an induction must track beyond total charge.
