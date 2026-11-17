# Referee report

## Verdicts

- **Tables** — CONFIRMED, recomputed. My definition-level solver (`verifier/defsolve.cpp`: explicit sets of consistent arrival orders, items named by arrival index, no flags) gives k=1 n≤10, **k=2 n≤10** (46125), k=3 n≤8, k=4 n≤7, k=5 n≤7; every entry equals both tables. For k=2 that is three sizes beyond the solvers' own check. My own flag DP reproduces A's rows k≤8, n≤12 and all of B's. No entry differs from either solver.
- **Reduction lemma (A Lemma 1, B Lemma 0)** — CONFIRMED, proofs read with no gap, and tested at every state, not only the root (`deflemma.cpp`). At each definition-level knowledge state reached (1,907,390 for k=2 n=10; 514,448 for k=3 n=7; 519,518 for k=4 n=6) the consistent set has exactly (t−f)·(t−1)! orders and its value equals the flag-DP value: 0 mismatches. A mutant flag DP that does not clear flags after an output gives 1706 mismatches at k=2 n=5. A's text is thinner than B's: Lemma 1 proves only "probing is free"; that the consistent set is "all bijections with m off flagged items" is one asserted sentence. True; it is B's Lemma 0.
- **Headline N(2,n) = 5N(2,n−1) − 5N(2,n−2), n ≥ 5** — CONFIRMED, both proofs read with no gap, both finite checks re-run with my own code (`flagcheck.py k2`). Conditional on the lemma (hand-proved, machine-tested to n=10).
  - A's finite computation: S(u) for u=5..8 over 49 states. Every action value Q(s,a,u) satisfies the recurrence, and one fixed action a*(s) has d(u−1) ≥ 0 and d(u) ≥ λ₂·d(u−1). It covers all n because the dominance margin is part of the inductive statement: once every Q at level u+1 satisfies the recurrence, d(u+1) = 5d(u) − 5d(u−1) ≥ λ₂·d(u) ≥ 0. My rerun: 0 failures on 5..8, 4 on 4..7, 16 on 3..6, matching A's failing runs.
  - B's finite computation: for a fixed policy with W_r = A·W_{r−1}, 36 slack sequences with slack ≥ 0 for r=1..9, x²−5x+5 annihilating each on r=8..128, and slack(9) ≥ β·slack(8) exactly. It covers all n because a sequence obeying a monic order-≤49 recurrence with 121 consecutive zeros stays zero, after which slack(r+1) − β·slack(r) is multiplied by α > 0 each step. My rerun: 36 pairs, no negative slack to r=300; annihilation in fact holds from r ≥ 2.
  - The gap I was asked to look for is absent in both: neither argues "optimal on the terms checked, hence always".
- **Closed forms** — CONFIRMED to n=300: A's 21U(n−3) − 30U(n−4), B's 3·5^(j−1)·F_{2j+1} and 3·5^(j−2)·L_{2j}, and the generating function.
- **N(k,n) = n! iff n ≤ k+1** — CONFIRMED, proofs read with no gap (A Thm 2, B Thms 1–2); both use the same adversary. Mechanical test (`overfull.py`): every phase-start state with an overfull suffix has value < t!, k≤4, 325 state-and-level pairs, 0 violations.
- **N(1,n) = 2^(n−1)** — CONFIRMED to n=10 at definition level and n=40 by flag DP. A's proof (`prove.py 1 3 6`) has the same shape; I did not re-run that window.
- **Conjectures (unproved by both)** — recomputed: N(k,k+2) = (k+2)! − (k+1) for k≤8; the k=3 recurrence holds for n≥9 to n=70 (fails at 5, 7, 8); the k=4 order-10 recurrence holds for n≥17 to n=90.

## Witnesses

Checked with `verifier/wcheck.py` (raw rules, own code). All 20 of A's policies and all 8 of B's strategies reproduce the claimed counts. Every lost-order file equals my complement. All 8 of B's certificates satisfy the Bellman equation.

Rejections observed: four broken policies or claims of mine (12, 12, claimed 22, lost file short by one), A's BAD policy (150 vs 270), B's BAD_a and BAD_c (0 of 24), my altered B strategy (3150 vs 3525), and three broken certificates (altered leaf, missing state, B's BAD cert).

## Disagreements between the solvers

No numeric disagreement. Differences of status only:
- N(1,n): A says proved, B says sketch. A's claim is defensible.
- A's DP forces a probe; B's leaves OUT optional. I ran both: forced probing attains the maximum in all 49 states for u ≤ 300.
- B states the headline as conditional on Lemma 0; A lists that as residual risk. B's wording is the accurate one.

## Errors found

- A's `table_out.txt` prints numerator coefficients "−4, 2, 1, 1", not the reported 1, −3, 1, 1, because `table.py` uses N(2,0)=1. The reported generating function is correct.
- A, Theorem 3: the closing claim "f(s,u) satisfies the recurrence for every s and u ≥ 5" does not follow at u=5 for states with several actions, since d(3) ≥ 0 is never checked. It is numerically true for all 49 states, and the theorem needs only the empty state.
- B's strategy files carry no claimed count, so a broken one is detectable only against `witness_log.txt`. B's BAD_b is not a rejection test (disclosed).

## Not checked

- Definition-level values beyond the sizes listed. My k=6 n=7 run was stopped unfinished through the harness; without `ps` I could not confirm the process exited, but it carries `timeout 1500`.
- My memo keys are 128-bit hashes of the explicit set, not the set itself.
- My solver, like the solvers' slow programs, skips attempts refused for every consistent order.
- I did not run the solvers' programs and computed no alternative reading. I found no material ambiguity; both took the same reading.

## Follow-up tasks

1. Prove N(k,k+2) = (k+2)! − (k+1) for all k: sharpen the overfull-suffix adversary to k+1 forced losses among orders with 1 last, and give an explicit strategy losing only those.
2. Prove N(3,n) = 11a(n−1) − 40a(n−2) + 58a(n−3) − 30a(n−4) for all n ≥ 9 by B's method on the 343 flag states: fixed policy, slacks annihilated by (x−3)(x³−8x²+16x−10), and an exact eventual-sign argument from the dominant root.
