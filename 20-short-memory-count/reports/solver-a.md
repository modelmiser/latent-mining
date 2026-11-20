# solver-a report

All files are in `solver-a/`.

## 1. Tables

| w | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|
| f(w) | 0 | 1 | 1 | 1 | 2 | 2 | 3 | 3 |
| e(w) (all states lit) | 0 | 1 | 1 | 2 | 2 | 2 | 3 | 3 |
| canonical machines with g = f | 2 | 61 | 77 | 5 | 3573 | 91 | not counted | not counted |

Every entry is COMPUTED, exhaustively. Sizes reached: w = 8. w = 9 was not run to completion.

- **w = 1..3:** `slow.py brute`, the definition coded directly, over all labelled machines.
- **w = 1..6:** `fast.cpp min`, no pruning, exact g for every canonical machine (262,279,982 at w = 6). Canonical: states numbered breadth-first from the start state, at most w+1 reachable states.
- **w = 2..8:** `fast.cpp dec S all`, a pruned search for "some M has g(M) <= S".
  - w = 7: none at S = 2 (18,357,815 nodes); 143,863 machines at S = 3.
  - w = 8: none at S = 2 (258,801,970 nodes, 8.5 min); S = 3 is achieved by the family below.
- **e(w):** `fast_e.cpp dec S e`, same search, criterion "every state with non-empty C has spread <= S".

**Pruning.** A partial machine is cut when its defined transitions contain a cycle of states whose spread, computed from defined transitions only, already exceeds S. Adding transitions only enlarges each C(q), so the cut is safe. An unsafe cut would bias f upward.

**Cross-checks.**
- Pruned and unpruned searches give identical counts for w = 2..6.
- The multi-word bitset code used for w >= 7 was checked only pointwise: 600 random machines at w = 5, 7, 8, 9 against independent Python (`crosscheck.py`), 0 mismatches. The w = 7 and w = 8 "none found" results have no unpruned confirmation.
- Watched failing: `slow.py check` rejects `bad_w6_s1.txt` (true witness, s lowered), `bad_w6_plain7.txt` (7-state plain counter, g = 3) and `bad_w4_lit.txt` (wrong lit set).

## 2. Witnesses

`witness_w1.txt` … `witness_w15.txt`. Format: w, s, number of states n, n lines "d(q,0) d(q,1)" with start state 0, then the lit states. Check with `python3 slow.py check FILE`.

- **w = 1, 2:** the "last bit" machine.
- **w >= 3:** the split-end counter S_w (`family.py split`).
  - Levels 0..w-2; bit 1 moves up and sticks at the top, bit 0 moves down and sticks at the bottom.
  - The bottom level is two states, "just arrived" and "stayed", with identical outgoing transitions; the top level likewise. Total w+1 states.
  - g(S_w) = floor((w-1)/2), computed for w = 3..15.
  - For w = 4 the middle level has spread 2, is unlit, and has no self-loop. For every other w all states are within the bound.
- **Odd w:** the plain saturating counter with (w+1)/2 + 1 states also achieves the value; for even w it does not.
- **Next better value impossible:** exhaustive search only (w <= 8); no compact certificate. To re-check, run `fast<w> dec <f-1> all` and expect `found=0`.

## 3. Headline

**Conjecture: f(w) = floor((w-1)/2) for all w except w = 2, where f(2) = 1.**

- **Proved:** f(w) <= floor((w-1)/2) for all w >= 3; f(w) >= 1 for w >= 2.
- **Computed:** equality for w <= 8.
- **Guess:** the matching lower bound for w >= 9, confidence about 85%.

The author's guess f(w) = floor(w/2) is false for even w >= 4: f(4) = 1, f(6) = 2, f(8) = 3. It holds for odd w <= 7 and w = 2.

The author's other beliefs were all confirmed: f(1) = 0, f(2) = 1, f(3) = 1; the (w+1)-state counter has g = floor(w/2) and the trailing-ones machine g = w-1 (both checked for w = 1..10); f(w) >= 1 for w >= 2 (proved); e(2) = 1.

## 4. Proofs (`proofs.txt`)

- **P1, COMPLETE.** f(w) <= floor((w-1)/2) for w >= 3 via S_w, by counting bits wasted at the ends of the level range, per state. Independent of the start state.
- **P2, COMPLETE.** f(w) >= 1 for all w >= 2.
- **P3, COMPLETE (lemma only).** In any legal design, a state reached with at least s+1 trailing zeros starts a path of 1s with at least w-s+1 distinct states; symmetrically for ones. I could not turn this into the general bound.
- **P4, NOT PROVED.** f(w) >= floor((w-1)/2) in general. No sketch is offered.

## 5. Interpretation

- **Number of states.** "|Q| = w+1" is read as allowing fewer reachable states. Unreachable padding states have empty C and lie on no cycle, so this is equivalent; optimal machines with fewer states occur (w = 5, 7).
- **Game reading.** Equivalent to the definition: condition (1) holds exactly when the reachable unlit states contain no cycle.
- No degeneracy found; w = 2 is the only exception to the formula.

## 6. Code

- `slow.py`: brute force for w <= 3; witness checker.
- `fast.cpp` (binaries `fast<w>`, built with `-DWW=w`): f table, counts, pruned searches; `eval` mode for cross-checks.
- `fast_e.cpp`: the e(w) row.
- `family.py`: S_w, plain and trailing-ones counters, witness files, g for w <= 15.
- `crosscheck.py`: the 600-machine comparison.
- `run_w7_w8.log`: raw output of the w = 7 and w = 8 runs.
