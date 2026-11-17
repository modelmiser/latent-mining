# referee report

Code and logs: `verifier/`.

## Verdicts

**Headline NO, least n = 6, t = 2 — CONFIRMED** (recomputed, witness checked).
- The 13-strategy witness for (6,2) passes my string-level checker `chk.py` (full outcome sets, from the definition).
- A(5) = 6, F(6,2) = 13 exactly, and F(n,t) <= 2A(n-1) for n = 3, 4, 5: my `ref dp` (memoised branch on the lowest free short word; all distinct sets, no dominance, no bound, no LP).

**Table F(n,t), n <= 8 — CONFIRMED, every entry. I differ from neither solver anywhere.**
- n <= 6: all 25 entries by `ref dp`.
- n = 7: t = 0, 6, 7 by `ref dp` (A(7) = 16). F(7,3) <= 22 and F(7,4) <= 20 by my branch-and-bound `ref bb`. t = 1, 2, 5 by the solvers' certificates, rechecked.
- n = 8: t = 0, 3, 5, 6, 7, 8 by certificates, rechecked. F(8,1) = 2A(7) = 32 by P4. F(8,2) <= 37 and F(8,4) <= 38 by `lpbb.py` (7,129 and 56,813 nodes).
- Lower bounds: all 122 witness files pass `chk.py` at the claimed sizes. All 63 certificates pass `certs.py`.

Caveat on "not an ILP call": (6,2), A(5) and A(7) are LP-free. `ref bb` and `lpbb.py` use HiGHS only as an LP oracle proposing weights; my code certifies each pruning in exact integers, so a wrong LP answer cannot close a node wrongly. No MILP call, but the same library remains in the loop as a hint source.

**n = 9**
- F(9) >= 67 > 60 = 2A(8): CONFIRMED (witnesses checked; A(8) <= 30 by certificate).
- F(9,1) = 60: CONFIRMED (P4 plus A(8) = 30).
- F(9,3) = 67 (a) and F(9,7) = 60 (b): lower bounds confirmed; upper bounds UNSUPPORTED (one MILP run each, no certificate).
- F(9,4) <= 68: UNSUPPORTED (floating-point solver bound).

**Proofs marked COMPLETE**
- L1 / b-P1 (strategies conflict iff same (u,a) or their S meet; dominance): no gap found.
- P1 / b-P2 (F(n,0) = F(n,n) = A(n)): no gap found.
- a-P2 / b-P5 (F(n,n-1) = A(n)): no gap found; S = D(ub) brute-forced for n <= 9.
- P3 (F(n,t) >= A(n)): no gap found; brute-forced.
- P4 (F(n,1) = 2A(n-1)): no gap found; matches the table for n = 3..8.
- a-P5 / b-P7 (min over b of |S| is r(u)+r(a)-1, attained iff b in u_t D(a)): no gap found; a's "exactly when" brute-forced for n <= 9 (347,480 cases, 0 violations).
- b-P6 (F(n,t) <= 2F(n-1,t-1)): no gap found; inclusion brute-forced. Loose in a detail: the corollary F(n,t) <= 2^t A(n-t) at t = n needs A(0) = 1 and the stated range reaches F(1,.), both outside the problem's n >= 2. Harmless.
- a-P6 (F(n,t) <= (1+o(1)) 2^n/n): right in outline, wrong in a stated detail. Hoeffding at m = n/2 - sqrt(n ln n) gives (1+o(1)) n^-2, not "at most n^-2" (1.65 n^-2 at n = 9). The exact tail is below 0.32 n^-2 for 3 <= n <= 3000 and only o(2^n/n) is needed, so the conclusion stands.
- Headline proofs (a, b-P8): no gap found.

## Disagreements between the solvers

- Values: none for n <= 8. At n = 9 they computed different entries; no conflict.
- Asymptotic: a says COMPLETE, b says SKETCH. a's argument holds apart from the constant above.
- Interpretation: none.

## Errors found

- The two "independent" (6,2) witnesses are the same 13 triples. My count (`cnt`) gives 375,360 maximum packings, so this reflects a shared deterministic library, not uniqueness. Their agreement on ILP values was weaker evidence than it looked.
- a's report mislabels `bad2_wit_6_2.txt` as "extra conflicting strategy": it holds 8 lines (7 good, 1 conflicting). Both bad files are rejected.
- b's `dp.log` prints "memo 0" on every line (argument evaluation order).
- My own `ref pairs` test is nearly vacuous (it only exercises "same (u,a) implies S meet"); I do not rely on it.
- No check that cannot fail was found in the solvers' checkers. Mine rejected 7 broken witnesses and 4 broken certificates, and my searches find a packing when the target is lowered by one (tested at (5,0), (6,2), (7,3); not at n = 8).

## Problem as written

Both answered the stated problem (triples, 0 <= t <= n, F(n) = max over t); no easier neighbour was substituted. The formal form matches the prose.

## Not checked

- A(9) = 52, A(10) = 94.
- Any n = 9 upper bound other than F(9,1).
- The solvers' solver code line by line (I read their checkers and drivers).
- The conjecture F(n) > 2A(n-1) for all n >= 6.

Process notes: one smoke test briefly overlapped two running searches (three cores for a few seconds). A failed n = 8 attempt wrote a 29 MB intermediate; I stopped my two processes by PID and removed the file.

## Follow-up tasks

1. Determine F(9,t) for 2 <= t <= 8 with machine-checkable upper bounds (a dual certificate, or a branch tree whose leaves carry integer weights). In particular decide whether F(9,4) is 67 or 68.
2. Prove or refute: F(n,2) >= 2A(n-1) + 1 for every n >= 6. A refutation needs an n with a certified F(n,2) <= 2A(n-1).
