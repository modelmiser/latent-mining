# solver-a report

## 1. Tables
N(k,n), all COMPUTED exhaustively by `fast.py` (exact DP over knowledge states; relies on Lemma 1, proofs.txt). Independently COMPUTED by `slow.py` (direct definition, sets of consistent orders, no normal form) for k=1 n<=7, k=2 n<=7, k=3 n<=6, k=4 n=6: identical.

| k \ n | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256 | 512 |
| 2 | 1 | 2 | 6 | 21 | 75 | 270 | 975 | 3525 | 12750 | 46125 |
| 3 | 1 | 2 | 6 | 24 | 116 | 604 | 3188 | 16948 | 90460 | 483924 |
| 4 | 1 | 2 | 6 | 24 | 120 | 715 | 4781 | 33419 | 235814 | 1673633 |
| 5 | 1 | 2 | 6 | 24 | 120 | 720 | 5034 | 39808 | 340774 | 3004976 |
| 6 | 1 | 2 | 6 | 24 | 120 | 720 | 5040 | 40313 | 361959 | 3567592 |
| 7 | 1 | 2 | 6 | 24 | 120 | 720 | 5040 | 40320 | 362872 | 3627260 |
| 8 | 1 | 2 | 6 | 24 | 120 | 720 | 5040 | 40320 | 362880 | 3628791 |

k<=4 additionally computed to n=70 (k=2 to n=300) for recurrence fitting (`fit.py`, `table.py`).
All four UNVERIFIED beliefs are TRUE (the last two proved for all k); the guess (exponential, rational GF) is TRUE for k=2. n=11,12 are in `table_out.txt`.
Extra, COMPUTED only (k<=8): N(k,k+2) = (k+2)! - (k+1).

## 2. Witnesses
- `policy_k{K}_n{N}.txt`: optimal strategies as plain-text tables (flag-state, unarrived count -> move; format in the header of `witness.py`). 20 instances, k<=6, n<=9.
- `check_strategy.py` (stand-alone, shares no code with the solver) plays each table against all n! orders under the raw rules; all 20 reproduce the claimed count.
- `lost_k{K}_n{N}.txt` (n<=6): orders not beaten by that table. k=2,n=4: 2341, 3241, 4231 (all have 1 last, as the problem believed; which three is strategy-dependent, but at least 3 of the 6 orders with 1 last are lost by any strategy, proofs.txt Thm 2).
- Upper bounds: exhaustive search (two independent programs), plus the adversary proof for n=k+2, plus Theorem 3 for k=2.
- Check seen failing: `BAD_policy_k2_n6.txt` (one move altered) -> 150 vs claimed 270, MISMATCH, exit 1. `prove.py 2 3 6` and `2 4 7` fail (16 and 4 failures) while `2 5 8` passes.

## 3. Headline
**N(2,n) = 5·N(2,n-1) - 5·N(2,n-2) for all n >= 5, with N(2,1..4) = 1, 2, 6, 21.**
Generating function: sum_{n>=1} N(2,n) x^n = x(1 - 3x + x^2 + x^3) / (1 - 5x + 5x^2).
Closed form (n>=4): N(2,n) = 21·U(n-3) - 30·U(n-4), U(j) = (λ1^j - λ2^j)/√5, λ = (5±√5)/2; growth rate λ1 = √5·φ ≈ 3.618.
Status: PROVED (computer-assisted induction, Theorem 3) and independently COMPUTED to n=300 by the DP; n<=7 by the definition-level brute force. Confidence: high (~97%); residual risk is an error in Lemma 1 or in my reading of the rules, both of which the brute force tests only up to n=7.

Conjectured (fitted, checked to n=70, NOT proved):
- k=3: a(n) = 11a(n-1) - 40a(n-2) + 58a(n-3) - 30a(n-4) for n>=9.
- k=4: order-10 recurrence, coefficients 23, -216, 1089, -3245, 5866, -6173, 3032, 300, -923, 245, for n>=17 (`fit_out.txt`).

## 4. Proofs (all in `proofs.txt`)
- Lemma 1, COMPLETE: probing every top after every change loses nothing (simulation with ghost items). Justifies the DP.
- Theorem 2, COMPLETE: N(k,n) = n! iff n <= k+1. (a) one-item-per-stack strategy; the only doubling is the last arrival, which is then necessarily 1. (b) adversary: 1 arrives last; then some suffix S_j..S_k holds more items than stacks, and an "overfull suffix" lemma (declare the bottom of the last overfull double stack to be the minimum; overfullness moves right until S_k) forces failure.
- Theorem 3, COMPLETE but COMPUTER-ASSISTED: the k=2 recurrence for all n>=5. Induction on the number of unarrived items over all 49 flag states and all actions; base window verified exactly by `prove.py 2 5 8`; the step uses d(u+1) = 5d(u) - 5d(u-1) >= λ2·d(u) to keep one fixed action optimal forever. The same script proves N(1,n) = 2^(n-1) (`prove.py 1 3 6`).
- NOT proved (no sketch offered as proof): k>=3 recurrences; N(k,k+2) = (k+2)! - (k+1).

## 5. Interpretation
No material ambiguity. Readings: OUT is allowed from any stack (as written); a refused attempt carries information only for OUT (IN/SHIFT legality is computable by Sorter from the shape); "beats" counts fixed arrival orders against one deterministic adaptive strategy, so N is the best sum over the decision tree; a successful OUT cannot be declined, which Lemma 1 shows is harmless. k=1 has no SHIFT. I did not compute any alternative reading.

## 6. Code -> numbers
- `fast.py`: DP, all table entries. `table.py` (`table_out.txt`): table, k=2 recurrence/closed-form check to n=300, N(k,k+2) deficits.
- `slow.py` (`slow_out.txt`): definition-level cross-check.
- `fit.py` (`fit_out.txt`): recurrence fitting k<=4, n<=70.
- `prove.py`: finite check for Theorem 3 and for k=1.
- `witness.py`: policy tables; `check_strategy.py`: independent verifier and lost-order lists.
