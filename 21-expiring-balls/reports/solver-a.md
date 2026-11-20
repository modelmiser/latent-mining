# solver-a report

**Headline: N(3) = 7.** g(6) = 2 and g(7) = 3, so FIFO expiry helps and the problem's guess (8) is wrong. Both halves are exhaustive computations with independently checked certificates. For the general form I only have N(4) ∈ {11, 12}; I could not decide 11.

Files are in `solver-a/` (index: `README.txt`).

## 1. Tables

FIFO game, as in the text:

| n | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 |
|---|---|---|---|---|---|---|---|---|----|----|----|----|
| g(n) | 1 | 1 | 2 | 2 | 2 | 3 | 3 | 3 | 3 | 3 or 4 | 4 | 4 |

- **n ≤ 8: COMPUTED, exhaustive** (`fast.cpp`): every state up to bin renaming, Adversary's forcing set as a least fixed point. 150,416 states at n=7; 4,400,374 at n=8.
- **n ≤ 5** agrees with `slow.py`, a direct labelled implementation.
- **n = 9, 10: COMPUTED** by relaxation: Rule keeps loads below 4 even if Adversary may remove any ball. n = 9 is confirmed separately by `greedy.cpp`.
- **n = 12, 13:** explicit 12-bin strategy (P6) plus the relaxation. **n = 11: open.**

So N(1) = 2, N(2) = 4, N(3) = 7, N(4) ∈ {11, 12}. The problem's unverified small values are confirmed.

Any-removal reading, `anyiso.cpp`, COMPUTED exhaustive up to isomorphism:

| n | 2–3 | 4–6 | 7–10 | 11–13 |
|---|-----|-----|------|-------|
| g_any(n) | 1 | 2 | 3 | 4 |

So A(k) = 2, 4, 7, 11 for k ≤ 4, and A(5) ≥ 14. The n = 14 run hit the 1500 s timeout. Agrees with `fast.cpp` mode 1 (n ≤ 8).

## 2. Witnesses

Plain text; `check.py` shares no code with the solvers.

- `adv_n7_L3.txt`: Adversary strategy for g(7) ≥ 3; 32 states, at most 12 moves, strictly decreasing rank. `check.py adv 7 3`: OK.
- `rule_n6_L3.txt`: Rule safe set for g(6) ≤ 2; 3843 states, closed under Expire, a safe answer to every legal offer. `check.py rule 6 3`: OK.
- `adv_n4_L2.txt`, `rule_n3_L2.txt`: the same for N(2) = 4. OK.
- `rule_n8_L4.txt`: g(8) ≤ 3; 2,318,587 states, OK (64 MB).
- `rule_any_n10_L4.txt`: any-removal safe set on 10 bins (32,844 classes), giving N(4) ≥ 11. `check_any.py` (different canonical form): OK.
- `verify7.py`, `verify12.py`: replay the hand-written strategies against every Rule answer (200 and 3,662,464 lines). OK.

Deliberate failures, all rejected: witnesses checked with the wrong n (both kinds); `bad_rule_n6_L3_missing_state.txt`; `bad_adv_n7_L3_wrong_move.txt`; `verify12_bad11.py`.

One real bug was caught by cross-checking: the first `anyiso.cpp` mis-found tree centres and disagreed with `fast.cpp` at n=7; fixed before any number above.

## 3. Headline and conjectures

- **N(3) = 7**, confidence about 99%.
- **N(4) ∈ {11, 12}**, both bounds certified.
- **Guess**: N(4) = 12 (roughly 60%), with N(k) = k + 2^(k−1). The rival 1 + k(k+1)/2 fits k ≤ 3 equally and gives 11.
- **Conjecture** (other reading): A(k) = 1 + k(k+1)/2. Upper bound proved; equality computed for k ≤ 4.
- **Rule strategy**: "give the ball to the endpoint with the smaller load; on a tie, to the endpoint appearing earlier in the age-ordered list of present balls". COMPUTED optimal for every n ≤ 9 (`greedy.cpp`; 45,439,595 reachable states at n=9). The tie-break matters: "later bin" and "smaller component" lose at n=6. Untested at n = 10, 11.
- Why 11 is undecided: about 10^11 states. A handicapped Adversary (no offer once 8 balls are present) does not win on 11–13 bins (`capfifo.cpp`); that proves nothing.

## 4. Proofs (full text: `proofs.txt`)

- **P1 COMPLETE**: N(k) ≤ 2^k, binary merging without Expire.
- **P2 COMPLETE**: N(k) ≥ 2k, smaller-load rule.
- **P3 COMPLETE**: N(k) ≥ A(k); a strategy safe against any removal is safe against FIFO.
- **P4 COMPLETE**: A(k) ≤ 1 + k(k+1)/2. Force load k−1 at x, strip all but x's balls, force load k−1 on the untouched 1 + k(k−1)/2 bins, offer.
- **P5 COMPUTED** (no hand proof of g(6) ≤ 2): N(3) = 7. The 7-bin strategy:
  1. Build a load-2 bin x on 4 bins.
  2. Offer two fresh bins (f receives), then {x, f}. Rule must give f the ball, or x reaches 3. f now holds the two youngest balls.
  3. Expire everything older: a clean 2-star on 3 bins, 4 bins untouched.
  4. Build a second load-2 bin there without Expire; offer it against f.
- **P6 COMPLETE given P5's certificate, machine-replayed**: N(4) ≤ 12. The same forcing trick twice gives a clean 3-star on 4 bins; then build load 3 on the 8 untouched bins without Expire.
- **SKETCH only**: N(k) ≤ k + 2^(k−1) for k ≥ 5. Not proved that the clean (k−1)-star fits in that many bins.

## 5. Interpretation

- No real ambiguity found. "Has load L" is read as "≥ L" (equivalent). Infinite play is a Rule win.
- The other reading (Adversary picks which ball leaves) gives the same answer for k ≤ 3.

## 6. Code per number

Named inline above: `fast.cpp` / `slow.py` / `check.py` (FIFO, n ≤ 8), `anyiso.cpp` / `check_any.py` (any-removal), `verify7.py`, `verify12.py`, `greedy.cpp`, `rules.py`, `capfifo.cpp`.

## Process notes

- For about a minute around 23:29 three of my compute processes were alive at once, breaking the two-process rule; my mistake.
- No run touched the 04:50–05:45 window.
