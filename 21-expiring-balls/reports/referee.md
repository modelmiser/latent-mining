# referee report

**Bottom line.** N(3) = 7 is confirmed. Combining the solvers gives **N(4) = 12**, which neither states. The upper half is a replayed explicit strategy, as good as a proof. The lower half is computed only, by two independently written programs that agree exactly. Confidence in N(4) = 12 is about 97%; the residual risk is all in g(11) = 3.

My code is in `verifier/`.

## Verdicts

| Claim | Verdict | How |
|---|---|---|
| g(n), n ≤ 8; N(1)=2, N(2)=4 | CONFIRMED | recomputed three ways (`ref.py` labelled n ≤ 5, `vf.cpp` forward, `vb.cpp` backward); verdicts and winning-state counts agree for every (n, L) |
| N(3) = 7 | CONFIRMED | recomputed; all four certificates accepted by my `wcheck.py` |
| g(9) = g(10) = 3 | CONFIRMED | `vb.cpp`; attractor sizes 34,970 and 3,675,805 equal B's |
| g(11) = 3, so N(4) ≥ 12 (B) | CONFIRMED, computation only | below |
| N(4) ≤ 12 (A's P6) | CONFIRMED | own replay, below |
| N(4) ≥ 11 via any-removal (A) | CONFIRMED | `rule_any_n10_L4.txt` accepted by `anyrem.py`; own solve gives g_any(10) = 3 |
| A(k) ≤ 1 + k(k+1)/2; A(k) = 2, 4, 7, 11 | CONFIRMED | proof read; g_any recomputed, n ≤ 10 |
| No-Expire value floor(log2 n) | CONFIRMED | proof read; recomputed n ≤ 8 |
| Greedy rule optimal, n ≤ 9 (A) | CONFIRMED n ≤ 8; UNSUPPORTED at 9 | `vf2` mode 2 (same 2,871 states at (6,3)); n = 9 not rerun |
| A(5) ≥ 14; g_any(11..13) computed (A) | UNSUPPORTED by me | not rerun |
| N(k) = k + 2^(k−1) (A's conjecture) | fits N(4) = 12, unproved | the rival 1 + k(k+1)/2 is refuted for FIFO at k = 4 |

## The two halves of N(4) = 12

**N(4) ≤ 12.** `replay12.py` is written from the prose of A's P5 and P6; it uses neither `verify12.py` nor A's `adv_n7_L3` table. The engine enforces the rules (offers only across components, FIFO expiry, win at load 4). All 3,521,600 Rule lines end with load 4, with at most 11 balls present. A's count (3,662,464) differs because my stage Ia follows the prose. I also read the argument by hand: no gap.

**N(4) ≥ 12.** I read `back2.cpp` line by line and found no error. Its correctness rests on:
- The attractor is closed under adding an older ball, so storing a core and testing age-suffixes is exact.
- Every attractor state is visited after it joins, so the offer rule fires from whichever answer joins later.
- The seed pruning is safe (at most n−k−2 more balls while two components survive).
- Labels and 10 balls fit the 128-bit key.

Evidence beyond reading:
- Rebuilt `back2.cpp`, reran n = 11: reproduces exactly (785 s).
- My `vb.cpp` (different canonical form, own hash table, incremental loads and components) gives **the same attractor size, 974,272,116, and the same 3,183,950 minimal seeds**; the empty state is outside (425 s).
- `vb` matches the forward solver's winning-set size for all n ≤ 8, including winning cases (138,141 at (7,3)), so it can answer "CAN".
- With pruning disabled, results are unchanged at (8,3), (8,4), (9,4).

Not ruled out: both programs share the core-plus-suffix idea and the pruning bound. There is no Rule certificate for n = 10 or 11 and no forward pass above n = 9.

## Disagreements between the solvers

- **N(4):** A {11, 12}, B {12..16}. Both right; the intersection is 12.
- **g(11):** A "3 or 4", B 3. Resolved as 3.
- **State counts:** A's 150,416 (n = 7) and 4,400,374 (n = 8) include terminal states (138,141 + 12,275; 4,358,443 + 41,931). No conflict with B.
- **Interpretation:** identical, and it is the problem as written.

## Proofs marked COMPLETE

- A-P1 / B-P2, N(k) ≤ 2^k by binary merging: no gap found.
- A-P2 / B-P1, N(k) ≥ 2k by the smaller-load rule: no gap found.
- A-P3, a rule safe against any removal is safe against FIFO: no gap found.
- A-P4 / B-P4, 1 + k(k+1)/2 bins suffice with free removal: no gap found.
- B-P3, no-Expire value is floor(log2 n): no gap found.
- A-P6, N(4) ≤ 12: no gap found; replayed.

## Errors and blemishes

- No wrong value in either report.
- A's "32 states" and B's "33 states" count file lines including terminal ones; 22 and 23 non-terminal states are reached.
- A lists g_any = 4 for n = 11–13 as computed; only n ≤ 10 has a certificate.
- B's N(4) ≥ 12 rested on one run of one program; now repaired.

## Broken witnesses, all rejected

- `wcheck.py`: wrong n (both kinds); both solvers' bad files; five of mine (deleted line, load-3 state added, move changed to Expire, rank raised, state removed).
- `replay12.py`: 11 bins; stage I′ skipped; junk ball not expired; engine expiring the newest ball.
- `anyrem.py`: the n = 10 set at n = 11; the n = 6 set at n = 7.

## Not checked

- The two `rule_n8_L4.txt` files (g(8) = 3 recomputed instead).
- `verify12.py`, `greedy.cpp` at n = 9, `capfifo`, `anyiso` at n ≥ 11, `rules.py`, B's forward pass at n = 9.

## Follow-ups

1. **Order-independent certificate for g(11) < 4.** Dump the final core C (about 3.7 M codes). A separate program checks, with C fixed: every seed has a suffix in C; for every q in the up-closure whose flipped newest ball gives a terminal or in-closure state, q minus that ball is in the closure; the empty state is outside.
2. **N(5) ≤ 21.** Give a strategy on 21 bins forcing a clean 4-star with 16 bins untouched, replayed against every Rule answer. Binary merging then gives load 5 and tests N(k) = k + 2^(k−1).
