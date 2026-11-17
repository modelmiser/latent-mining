# solver-b report

All files are in `solver-b/`.

## 1. Tables

F(n,t), columns t = 0..n. Every entry for n <= 8 is COMPUTED exactly.

| n | F(n,t), t = 0..n | F(n) | 2A(n-1) |
|---|---|---|---|
| 2 | 2 2 2 | 2 | 2 |
| 3 | 2 4 2 2 | 4 | 4 |
| 4 | 4 4 4 4 4 | 4 | 4 |
| 5 | 6 8 8 8 6 6 | 8 | 8 |
| 6 | 10 12 **13** 12 12 10 10 | 13 | 12 |
| 7 | 16 20 **21 22** 20 20 16 16 | 22 | 20 |
| 8 | 30 32 **37 38 38 35 34** 30 30 | 38 | 32 |

n = 9 is partial: F(9,1) = F(9,7) = 60 (COMPUTED, one solver); 67 <= F(9,4) <= 68 (solver hit its time limit; 67 is a verified witness, 68 is only the solver's floating-point bound). F(9,6) was not obtained (run timed out).

How each size was computed:
- **n <= 4**: four programs agree — naive backtracking over full outcome sets (`slow.py`), set-packing ILP over all strategies with full outcome sets (`fast.py full`), the same ILP over inclusion-minimal length-(n-1) sets (`fast.py min`), and an LP-free memoised branch on the lowest free point (`dp.cpp`).
- **n = 5, 6**: the last three agree. `slow.py` timed out at n = 5.
- **n = 7**: both ILP modes agree. `dp.cpp` confirmed only t = 0; it timed out at t = 1 and I stopped the rest.
- **n = 8**: `fast.py min` only (HiGHS, proved optimal, gap 0).

So A(1..8) = 1, 2, 2, 4, 6, 10, 16, 30 is confirmed (A(1) by hand). A(9) = 52 and A(10) = 94 were not checked.

Only pruning (min mode): duplicate and non-minimal sets are dropped — safe, since a packing member can be swapped for any nonempty subset. The unpruned `full` mode agrees through n = 7.

## 2. Witnesses

- **Lower bounds**: `wit_min_n{n}_t{t}.txt` for every entry above (and `wit_full_*` for n <= 7), one strategy `u a b` per line. `verify.py n t file` checks them by simulating the channel and shares no code with the solvers. All 78 pass; a corrupted copy (`BAD_wit_n6_t2.txt`) is rejected.
- **Upper bounds**: `cert_n{n}_t{t}.txt` holds integer weights on {0,1}^(n-1) (scale K = 10^6) such that every strategy's outcomes weigh >= K and the total is < (F+1)K. `certcheck.py n t F file` checks this in exact integers over all strategies. 33 certificates pass; a claim of F-1 and a corrupted certificate (`BAD_cert_n6_t1.txt`) are both rejected.
- **No checkable upper-bound certificate** exists where the LP bound is not tight: (6,2), (7,0), (7,3), (7,4), (7,6), (7,7), (8,1), (8,2), (8,4). Those upper bounds rest on solver agreement only (for n = 8, on HiGHS alone).

The counterexample, (n,t) = (6,2), 13 strategies `u a b`:

```
00 0000 0000   00 1111 0111   00 1100 0100
11 0000 1000   11 1111 1111   11 0011 1001   11 1100 1100
01 0000 0101   01 1111 0011   01 0011 0001   01 1100 1010
10 0000 1101   10 1100 0010
```

## 3. Headline

**NO.** The least n is 6, with t = 2: F(6,2) >= 13 > 12 = 2A(5). Confidence is very high, because the refutation needs only two independently checked facts: the 13-strategy witness above, and A(5) <= 6 (`cert_n5_t0.txt`).

Minimality of n = 6 is certified: `cert_n{3,4,5}_t*` give F(n,t) <= 2A(n-1) for n = 3, 4, 5 and all t.

Feedback after the first bit is also suboptimal at n = 7 (22 vs 20, t = 3), n = 8 (38 vs 32, t = 3 or 4) and n = 9 (>= 67 vs 60, t = 4, taking A(8) = 30 from my own computation).

No formula conjectured. Guess only: F(n) > 2A(n-1) for all n >= 6, best t near n/2.

## 4. Proofs

Written out in `proofs.txt`. All COMPLETE unless marked.

- **P1**: outcome sets are disjoint iff the strategies are distinct and their length-(n-1) parts S = uD(a) ∪ D(u)b are disjoint.
- **P2**: F(n,0) = F(n,n) = A(n).
- **P3**: F(n,t) >= A(n), taking b = a.
- **P4**: F(n,1) = 2A(n-1).
- **P5**: F(n,n-1) = A(n).
- **P6**: F(n,t) <= 2F(n-1,t-1), hence F(n,t) <= 2^t A(n-t). Strip the first bit of u: c·S(u',a,b) ⊂ S(cu',a,b). This gives F(5,2) <= 8.
- **P7**: for 1 <= t <= n-1, min over b of |S| is r(u)+r(a)-1, attained at b = u_t a'. Also brute-forced for n <= 8 (`misc.py`).
- **P8**: headline NO, given the checked witness and certificate.
- **S1 (SKETCH, not written out)**: F(n) = (1+o(1)) 2^n/n via P7 and Levenshtein-style counting.

Status of the problem's UNVERIFIED beliefs:
- First three bullets, and the run-count formula in the fourth: true and proved (P3, P4, P5, P7).
- The asymptotic in the fourth: sketch only.
- A(1..8): confirmed. A(9), A(10): unchecked.
- F(3,1) = 4, F(4,t) = 4, F(5,2) <= 8 (in fact = 8): confirmed.
- The "Guess" is false. The n = 5 pattern does not persist: the n = 6 witness uses a = 0011 and 1100 with u = 01, 10, 11.

## 5. Interpretation

- No real ambiguity found; I used the formal definition literally.
- t = 0 and t = n are degenerate (feedback carries nothing); both give A(n).
- The equality F(n) = 2A(n-1) also holds at n = 2, using A(1) = 1.

## 6. Code

`common.py` (definitions); `slow.py` (n <= 4); `fast.py` (every table entry and witness; logs `fast_*.log`); `dp.cpp` (cross-check n <= 6 and (7,0); `dp.log`, `dp7.log`); `mkcert.py` (certificates); `verify.py`, `certcheck.py` (independent checkers; `verify.log`, `certcheck.log`); `misc.py` (P7 brute force). All runs niced, pinned to cores 2-3, memory-capped.
