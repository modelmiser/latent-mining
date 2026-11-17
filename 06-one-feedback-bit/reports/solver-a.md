# solver-a report

Files: `solver-a/`.

## 1. Tables

F(n,t), t = 0..n; every entry for n <= 8 COMPUTED (exhaustive, optimal). Bold = exceeds 2A(n-1).

| n | t=0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 2A(n-1) |
|---|---|---|---|---|---|---|---|---|---|---|
| 2 | 2 | 2 | 2 | | | | | | | 2 |
| 3 | 2 | 4 | 2 | 2 | | | | | | 4 |
| 4 | 4 | 4 | 4 | 4 | 4 | | | | | 4 |
| 5 | 6 | 8 | 8 | 8 | 6 | 6 | | | | 8 |
| 6 | 10 | 12 | **13** | 12 | 12 | 10 | 10 | | | 12 |
| 7 | 16 | 20 | **21** | **22** | 20 | 20 | 16 | 16 | | 20 |
| 8 | 30 | 32 | **37** | **38** | **38** | **35** | **34** | 30 | 30 | 32 |

A(1..8) = 1, 2, 2, 4, 6, 10, 16, 30 (column t=0), matching the stated list; A(9), A(10) not computed.

n = 9 (partial): F(9,3) = 67 COMPUTED (ILP optimal, one solver, not cross-checked). 67 <= F(9,4) <= 68 (ILP stopped at 600 s; 68 is the solver's uncertified bound). Both exceed 60 = 2A(8). Other t not run.

Support per row:
- n <= 4: definition-level max-independent-set (`slow.py`), plus the below. `slow.py` did not finish n = 5 in 900 s.
- n <= 6: ILP (HiGHS via scipy) with and without dominance pruning agree. Independent C++ branch-and-bound `bb.cpp` (no dominance) agrees for n <= 5 and n = 6 at t = 0, 3, 4, 5, 6; it timed out at n = 6, t = 1, 2. `bb2.cpp` (same search plus dominance) agrees on all of n = 6.
- n = 7: ILP with and without dominance agree. `bb2` agrees for t = 0, 1, 4, 5, 6, 7; it timed out (180 s) at t = 2, 3, which rest on HiGHS alone.
- n = 8: ILP with dominance only, one solver.

Pruning: L1 (strategies conflict iff their length-(n-1) outcome sets S = uD(a) ∪ D(u)b meet), merging identical sets, dropping any set containing another. All value-preserving (proofs.txt L1).

## 2. Witnesses

- `wit_n_t.txt` for 0 <= t <= n <= 8, plus `wit_9_3.txt`, `wit_9_4.txt`: lines `u a b`. All 44 pass `check.py` (string definition of O only).
- F(6,2) >= 13 (`wit_6_2.txt`):
  (00,0000,0000) (00,1100,0100) (00,1111,0111) (01,0000,0101) (01,0011,0001) (01,1100,1010) (01,1111,0011) (10,0000,1101) (10,1100,0010) (11,0000,1000) (11,0011,1001) (11,1100,1100) (11,1111,1111)
- Upper bounds: `cert_n_t.txt` are LP-dual certificates (non-negative rational weights on words of length n-1, weight >= 1 on every strategy's S, floor(total) = F), verified by `checkcert.py` over all strategies in exact fractions. They exist for all (n,t) with 3 <= n <= 5; n = 6 except t = 2; n = 7 at t = 1, 2, 5; n = 8 at t = 0, 3, 5, 6, 7, 8 (30 files, all pass).
- Elsewhere the LP is not tight, so the upper bound is exhaustive search only: F(6,2) <= 13 (LP gives 14), n = 7 at t = 0, 3, 4, 6, 7, n = 8 at t = 1, 2, 4.
- Deliberate failures, all rejected: `bad2_wit_6_2.txt` (extra conflicting strategy), `bad_cert_6_1.txt` (one weight zeroed), `cert_6_1.txt` applied to t = 2.

## 3. Headline

**NO.** Least n = 6, t = 2: F(6,2) = 13 > 12 = 2A(5). Confidence very high; the refutation needs only certificate-checked facts:
- 13 disjoint strategies: `wit_6_2.txt`.
- A(5) = 6: `wit_5_0.txt`, `cert_5_0.txt`.
- n = 3, 4, 5 are not counterexamples: `cert_n_t.txt` give F(n,t) <= 2A(n-1) for every t.

F(n) = 4, 4, 8, 13, 22, 38 for n = 3..8; F(9) >= 67. Best t: 1; any; 1–3; 2; 3; 3–4.

Guess, not proved: F(n) > 2A(n-1) for all n >= 6, optimal t growing slowly. No formula conjectured.

Statement's beliefs:
- F(n,t) >= A(n); F(n,1) = 2A(n-1); F(n,n-1) = A(n); minimum short-outcome count r(u)+r(a)-1: all true, proved.
- F(3,1) = 4, F(4,t) = 4, F(5,2) <= 8 (equals 8): true.
- "Interior t forced onto constant-run strings, hence yes": false from n = 6.
- F(n) ~ 2^n/n: true (P6), but does not make t = 1 optimal.

## 4. Proofs (`proofs.txt`), all COMPLETE

- L1: reduction to set packing in {0,1}^(n-1); dominance.
- P1: F(n,0) = F(n,n) = A(n).
- P2: F(n,n-1) = A(n), since S = D(ub) independent of a.
- P3: F(n,t) >= A(n) via (u,a,a).
- P4: F(n,1) = 2A(n-1).
- P5: min over b of |S| = r(u)+r(a)-1, attained iff b ∈ u_t D(a). Also brute-forced for n <= 9 (`p5check.py`).
- P6: F(n,t) <= (1+o(1)) 2^n/n, given two classical ingredients not re-proved: Hoeffding's bound and A(n) >= 2^n/(n+1) (VT codes).
- Headline refutation: certificate-checked (section 3).

No sketches presented as proofs. No general proof that the excess persists for n >= 6.

## 5. Interpretation

- Strategies are triples; two with the same (u,a) and different b share ua and conflict, so messages are distinct words ua.
- t = 0 and t = n make the feedback vacuous, giving A(n); included because the text says 0 <= t <= n.
- The headline also holds at n = 2 (F(2) = 2 = 2A(1)).
- No other reading seemed plausible enough to compute separately.

## 6. Code

- `slow.py`: definition-level solver (n <= 4).
- `fast.py`: ILP, all tables and `wit_*`. Logs `log_fast7.txt`, `log_fast8.txt`, `log_fast9.txt`; without dominance `log_nodom.txt`, `log_nodom7.txt`.
- `bb.cpp`, `bb2.cpp`: independent exact search. Logs `log_bb.txt`, `log_bb2_7.txt` (the n = 6 `bb2` run went to the terminal only).
- `lp.py`: writes `cert_*` (`log_lp.txt`).
- `check.py`, `checkcert.py`: standalone checkers (`log_check.txt`, `log_checkcert.txt`).
- `p5check.py`: lemma brute force.

Runs used nice 15, at most 2 pinned cores, 8 GB ulimit, timeouts. Nothing deleted.
