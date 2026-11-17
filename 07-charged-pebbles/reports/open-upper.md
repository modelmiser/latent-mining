# open-upper report: the upper bound is proved

## What is now proved (exact statements; all marked COMPLETE in `open-upper/proofs.txt`)

**Main Theorem.** In the relaxed game with q >= 2 pebbles, after f F-moves:
(a) a board clean at n has n <= (f+2)·2^(q-2);
(b) every pebbled node is <= (f+4)·2^(q-2) − 1.
Hence C(p,k) <= (pk+2)·2^(p-2) for all p >= 2, k >= 0. With the solvers' lower bound, **C(p,k) = (pk+2)·2^(p-2); the headline answer is YES.** Both halves of solver B's Conjecture R (upper bounds) follow.

The route (U = 2^(q-2)):

1. **Reversible components are "anchor sets".** Call x reducible in S if x − pred(x) <= 2^(q−|S|). Deleting reducible elements is confluent, each step is a P/R path, and P/R moves do not change the reduced form red(S). So P/R-components are indexed by reduced sets A (all gaps > 2^(q−|A|)); every configuration of the component contains A, and its other pebbles sit within 2^(q−|A|) − 1 above an anchor (Lemma W of solver B, plus a rank version W(iii)).
2. **Invariant J(f)** on A = red(board) = {a_1<…<a_m}: A is empty, or f >= 1, a_1 <= (f+2)U and a_m <= (f+3)U − 2^(q−m−1). Theorem I: deleting any pebble from any configuration satisfying J(f) (reachable or not) gives J(f+1). This is an inductive invariant, not just a true one. Clean at n > 2^(q-1) means A = {n}, so n <= (f+2)U.
3. For m >= 2 the step is elementary (crude position bounds plus a truncation lemma). For m <= 1 it needs a **Shape Lemma**: if S is in comp(A), A empty or {a}, then red(S − z) is empty or is a configuration of comp(A) with one pebble shifted by at most 2^(q−k−1) (k its size) without passing a neighbour. Its proof uses a **normal form**: reduce S − z in the (q−1)-pebble game; the path lifts to the q-game with z kept, which shows every deletion is equivalent to "advance one pebble by at most half the free reach". A short case analysis shows this shape survives further reduction steps (the shifted pebble may become a retreating one).

## What was tested mechanically, and how (all in `open-upper/`, niced, 2 cores max, capped)

- `induct2.py J2`: for every anchor set in a window (q=2 N=16, q=3 N=26, q=4 N=30, q=5 N=44: 64,272 anchor sets, 696,545 configurations, 2,597,483 F-moves), reachable or not, every F-move from every configuration of the component raises the least f with J(f) by at most 1. 0 violations. BFS components equal fibres of red: 0 mismatches. Three mutants (either bound tightened by 1; a_1 bound alone) are reported violated, so the test can fail.
- `lemmas.py` / `lemmas2.py`: Shape Lemma for every S and every z (q <= 5: comp(empty) and all {a} with 2^(q-1) < a <= 2^q+3; q = 6: comp(empty), 9.2M cases, plus {33}, {34}); its closure form (every one-shift configuration reduces to a one-shift configuration or to empty); the lifting identity; the truncation lemma. 0 failures. Mutant with the shift bound lowered by 1 fails.
- `reach_check.py`: exact F-cost of every configuration by 0-1 BFS (q=2..5) satisfies J and (b); both bounds of J are met with equality.

## What failed and why

- Solver B's Conjecture J on raw configurations is not invariant under P/R, so it cannot be inductive. Moving it to anchor sets with the same offsets still fails: at q=4, {3,6,x} loses two ranks with one deletion.
- "A free pebble below z, or in another interval, cannot become an anchor": false ({4,6,7} minus 6 at q=3 has anchors {4,7}).
- Exchange identities red(red_q(S−z)+z) = red(S) and red(A+A'+z) = A: false (`ep.py`, `ep2.py`, `ep3.py`). The correct version uses red in the (q−1)-game.

## What remains open

- **J'** (tested, not proved): after f >= 1 F-moves the j-th lowest anchor is <= (f+3)U − 2^(q−1−j). Inductive on all anchor sets in the windows for q <= 5; proved only for the first F-move. Not needed for the headline.
- Nothing was checked in a proof assistant.

## Confidence

- Part 1 (components, Lemmas L1–L6): high.
- Part 2 (normal form, Step, Shape): high for the statements, which were checked exhaustively up to q = 6; the written case analysis is short and I re-derived it twice, but nobody else has read it.
- Part 3 (invariant and Main Theorem): high; elementary inequalities, and the inductive claim was machine-checked for q <= 5.
- Overall that the headline is YES for all (p,k): about 95%. The residual risk is an unnoticed slip in the general-q case analysis of Lemma Step or Lemma N; an independent reader should check those two first.

Proofs: `open-upper/proofs.txt`; programs and logs are in the same folder.
