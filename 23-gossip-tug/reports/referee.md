# Referee report: Delayer/Hastener gossip game

My code is in `verifier/`: `g6.cpp` (exact minimax, threshold search, witness checkers; `g*.cpp` are variants), `slow.py`, `alt.py`. Written from the problem text; I read the solvers' code only for file formats and, afterwards, the core of B's `ab2.cpp`.

## Verdicts

| Claim | Verdict | How |
|---|---|---|
| Headline NO; first failure n = 4; A(4) = 6 | CONFIRMED | recomputed (Python and C++); B's hand proof read, no gap; certificates for both players accepted by my checker |
| A(2..7) = 1, 3, 6, 9, 12, 16 | CONFIRMED | recomputed, with A's position counts (905,168 raw at n = 6; 49,045 classes at n = 7); B's `table4..7` equal my values entry by entry; A's D and H certificates accepted |
| A(8) = 19 | CONFIRMED | recomputed exactly (2,923,259 classes, A's count); threshold search with and without lemma cut-offs; both Delayer certificates accepted |
| A(9) = 23 | CONFIRMED | my threshold search: YES at 23, NO at 24, several move orders, once without lemma cut-offs. Computation only |
| A(10) ≥ 27 | CONFIRMED | my search, two engine variants |
| A(10) = 27 (B only) | CONFIRMED | my search answers NO at 28 twice, with different reply orders (168M and 203M nodes, 1261 s and 1316 s). Both runs use the Lemma 1–3 cut-offs |
| A(11) ≥ 30 (B only) | UNSUPPORTED by me | my program confirms ≥ 29 (two runs); four attempts at 30 hit the 1500 s limit undecided |
| A: P0 and P1 (A(n) ≥ 2n−3, all n ≥ 2) | CONFIRMED | read line by line, no gap; the shortest continuation from the chain position is exactly n−2 for n ≤ 6 |
| B: Lemmas 1, 2, 3 | CONFIRMED | no gap; as static bounds, violated at no reachable position for n ≤ 8 |
| B: Theorem A (hand proof, A(4) = 6) | CONFIRMED | every round checked: legal replies, forced calls and sets are as stated |
| B: V(m,r) table | CONFIRMED in part | all entries with m + r ≤ 10 that B ran are equal, except V(4,6), where I have only ≥ 24; also V(9,2), V(8,3), V(7,4) |

How far my own program goes: A(8) by full minimax; A(9) and A(10) by threshold search, both sides.

## Disagreements between the solvers

- **Values:** none for n = 2..9. They differ in reach only. For n = 10, B is right; n = 11 is not resolved.
- **Proofs:** only A proves A(n) ≥ 2n−3. B says no general-n statement is proved. The task note that both prove it is wrong about B.
- **Interpretation:** both read the rules literally and identically.

## Errors found

- **B, `bad5.txt`:** byte-identical to `table5.txt`. It sits with the deliberately corrupted files but is not corrupted; the value test that fails is `bad5_value.txt`.
- **B, certificates:** called "explicit Delayer strategies", but they are proof DAGs indexed by (position, threshold). The same position recurs with different thresholds (6, 435 and 17,284 repeated lines at n = 6, 7, 8). Read as "position → agent" with the first line winning, they guarantee only 11, 14 and 15. Taking the highest-threshold line per position, my checker accepts all five.
- **B, `badcert5_root.txt`:** only the root's claimed number is changed; the strategy inside is still valid, and my checker accepts it at 9.
- **B, identity A(n) = (m−1) + V(m, n−m):** the position "m agents knowing exactly m secrets" cannot be reached in m−1 calls for m ≥ 3, so this is a numerical coincidence. B labels it a guess.
- **A:** no errors found.

## Witness checker tests (mine)

Rejected:
- claim 10 on A's n = 5 Delayer file, and claim 8 on its Hastener file;
- 23 of 24 single-line deletions and 59 of 96 agent changes in the Delayer file;
- all 62 line deletions and 840 of 1112 reply changes in the Hastener file;
- 187 of 198 line deletions in B's n = 6 certificate;
- A's `bad_n4_D`, `bad_n4_H`, `bad_n5_H` and B's `badcert5_agent`.

The mutations that pass are alternative optimal moves or unused lines.

## Problem as written

Both answered the stated problem. One neighbouring reading matters. The text lets Delayer name any active agent, including a fully informed one, and B's proof of A(4) = 6 uses that in round 5. If Delayer may name only non-full agents, I get 5, 8, 10, 14, 16 for n = 4..8. That reproduces the problem author's "A(4) = 5"; the headline is still NO, failing first at n = 5 (8 against 7).

## Not checked

- A(11) ≥ 30; V(6,5), V(5,6), V(6,6) and the n = 12, 13 entries (my program stops at n = 11).
- A lemma-free upper bound at n = 10; any upper-side certificate for n ≥ 8 (none exists).
- A's `rule.cpp` claim; B's restricted-Hastener runs; the solvers' engines beyond B's core.

## Follow-up tasks

1. Prove A(n) ≥ 3n − 6 for all n ≥ 4 (true for n = 4..10) by an explicit Delayer rule, machine-checked as a rule for n ≤ 10; or find the first n where the best such rule falls short.
2. Decide A(11) exactly with two independent engines (30 or 31?), and produce a Hastener-side certificate or a lemma-free run for A(10) ≤ 27.
