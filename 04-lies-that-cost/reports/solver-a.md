# solver-a report

All files are in `solver-a/`. P(n) denotes the Pelc expression from the problem statement.

## Headline

**Not settled for all n. No counterexample exists up to n = 2805: Q_2(n) = U1(n) = P(n) for every 2 <= n <= 2805.**
- Lower bound Q_k(n) >= U1(n) >= P(n): PROVED for all k, n (proofs.txt P2, P3).
- Upper bound Q_2(n) <= P(n): COMPUTED only, n <= 2805.
- "Yes for every n" is a GUESS; my confidence is about 85%. Conjecture: Q_k(n) = P(n) for all k >= 2, n >= 2.

## 1. Tables

Three independent programs agree wherever they overlap:
- `slow.py`: the definition itself, pairs (x, L), all subsets S, no memo. n=2,3 for k=1,2,3; n=4 for k=2.
- `mid.py`: labelled states, all 2^n subsets. n <= 7 (k=1,2), n <= 6 (k=3).
- `fast.cpp`: multiset-of-obligations states. Mode `fix`: bottom-up fixed point over all reachable unclipped states (exact, proves infinity). Modes `fast`/`ek`: top-down bounded search.

**Q_2(n), COMPUTED exhaustive:**

| n | 2 | 3 | 4 | 5–8 | 9–16 | 17–28 | 29–50 | 51–92 | 93–170 | 171–314 | 315–584 | 585–600 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Q_2 | 3 | 5 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 |

- Equal to P(n) throughout (`out_k2_fast_2_80.txt`, `out_k2_fast_81_600.txt`, compared by `pelc.py`: 0 mismatches).
- n = 601..2805: win at P(n) and no win at P(n)−1 (`out_k2_ek_601_4000.txt`; the run hit its timeout at 2805).
- Fixed point agrees for n <= 13 (`out_k2_fix_10_13.txt`).

**Other k, COMPUTED:**

| k | Range | Result | File |
|---|---|---|---|
| 3, 4, 5, 8 | 2 <= n <= 600 | Q_k(n) = P(n) for every n | `out_k{3,4,5,8}_ek_2_600.txt` |
| 1 | n in {2,3,4,5,6,7,9} | Q_1(n) = infinity (fixed point reached, start state never winning) | `out_k1_fix.txt` |
| 1 | n = 8 | Q_1(8) = 6 = P(8) | `out_k1_fix.txt` |
| 1 | 10 <= n <= 16 | Q_1(n) = 7 = P(n) | `out_k1_fix.txt` |
| 1 | 17 <= n <= 314 | Q_1(n) = P(n) | `out_k1_ek_2_600.txt`, `out_k1_ek_190_600.txt` (timeout at 314) |

So, as far as computed:
- E_1 ∩ [2,314] = {8} ∪ [10,314].
- E_2 ∩ [2,2805] is everything.
- E_k ∩ [2,600] is everything for k = 3, 4, 5, 8.

**U1:** brute force on (a,b) states equals P(n) for n <= 60 (`out_u1_bruteforce.txt`). For n <= 2805, U1(n) = P(n) also follows from P2 + P3 + the computed Q_2, without using Pelc's theorem.

**Pruning and why it is safe:**
- Obligations r >= questions left are clipped to "infinite"; such a candidate cannot lie again in time.
- Fail if a(j+1)+b > 2^j, where a = #(r=0), b = #(r>0), j = questions left (proved necessary, P3').
- Splits are taken up to swapping S with its complement.
- An unsafe weight prune would bias Q upward (toward a false "no").
- Prune off and clip off give identical values for k = 1, 2, 3, n <= 13, q <= 9 (`out_k*_noprune_*.txt`).

## 2. Witnesses

- **Upper bounds:** explicit questioner strategy trees in `strategies/k{K}_n{N}_q{Q}.txt`, one line per answer path (`path ? S`, `path = x`, `path X`).
  - k=2: every n = 2..48, plus n = 92, 93, 200.
  - k=1: n = 8, 10, 11, 16, 100.
  - k=3 and k=8: n = 20.
- **Checker:** `check_strategy.py FILE n k q` shares no code with the solver and re-derives C_t from the (x, L) definition along every legal answer sequence. All 57 pass (`check_log.txt`).
- **Deliberate failures, all rejected:** the k=2 n=8 strategy checked as k=1; the n=9 strategy checked with q=6; `strategies/BROKEN_k2_n8_q6.txt` (one element removed from a question).
- **Lower bounds:** the weight argument P3. For k=1 infinity: the fixed-point output, plus hand proof P4 for n = 2, 3.
- Strategies for n > 200 were not exported or independently checked; those values rest on `fast.cpp` alone.

## 3. Proofs (`proofs.txt`)

- P1 COMPLETE: the position is the multiset of remaining obligations (the problem's UNVERIFIED item 5 is true).
- P2 COMPLETE: Q_k(n) >= U1(n).
- P3 COMPLETE: U1(n) >= P(n), the easy half of Pelc. P3' COMPLETE: the same bound inside the Q_k game.
- P4 COMPLETE: Q_1(2) = Q_1(3) = infinity.
- P5 NOT PROVED: Q_2(n) <= P(n) for all n. Only a heuristic is given, labelled as a sketch. I have no induction on n.

## 4. Checks of the UNVERIFIED claims

- Q_2(2) = 3, Q_2(3) = 5: confirmed by all three programs.
- Pelc's formula: lower half proved here; upper half assumed for n > 2805.
- "E_1 omits 2 and 3": true but understated. E_1 also omits 4, 5, 6, 7 and 9 (Q_1 = infinity) and contains 8.

## 5. Interpretation

- ell_s counts alive candidates only, as written. Elements of S outside C_{s-1} are irrelevant.
- Questions with ell_s = 0 are allowed. A lie there carries no obligation, but it kills every candidate still under obligation. They are included in the search.
- An answer that would empty C is illegal for the responder; the other answer is then always legal.
- n is in E_k only if Q_k(n) is finite and equal to U1(n).
- No ambiguity found that changes an answer; no alternative reading computed.

## 6. Code → numbers

- `fast.cpp`: mode `fast` (Q_2 table), `ek` (E_k sweeps), `fix` (infinities, exact small values), `strat` (strategy files).
- `slow.py`, `mid.py`: small-n cross-checks and the U1 brute force. `pelc.py`: comparison with P(n). `check_strategy.py`: witness verification.
