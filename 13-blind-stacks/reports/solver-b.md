# solver-b report

## 1. Tables (all COMPUTED exhaustively)

Method: exact DP over Sorter's information states (`fast.py`; state = per-stack flags "refused since last output" + number unarrived; justified by Lemma 0). No pruning except skipping attempts already known to be refused. Cross-checked against `slow.py` (literal definition, explicit sets of consistent permutations, no state abstraction) for k=1 n≤6, k=2 n≤6, k=3 n≤5: identical.

| k \ n | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256 | 512 | 1024 | 2048 |
| 2 | 1 | 2 | 6 | 21 | 75 | 270 | 975 | 3525 | 12750 | 46125 | 166875 | 603750 |
| 3 | 1 | 2 | 6 | 24 | 116 | 604 | 3188 | 16948 | 90460 | 483924 | 2592108 | 13894468 |
| 4 | 1 | 2 | 6 | 24 | 120 | 715 | 4781 | 33419 | 235814 | 1673633 | 11922892 | 85148125 |
| 5 | 1 | 2 | 6 | 24 | 120 | 720 | 5034 | 39808 | 340774 | 3004976 | 26681636 | 238004439 |
| 6 | 1 | 2 | 6 | 24 | 120 | 720 | 5040 | 40313 | 361959 | 3567592 | 37283949 | 398035330 |
| 7 | 1 | 2 | 6 | 24 | 120 | 720 | 5040 | 40320 | 362872 | 3627260 | – | – |

Further reached: k=2 to n=130, k=3 to n=70, k=4 to n=90. Sanity checks passed: N ≤ n!, monotone in k and n.

## 2. Witnesses

- `strategy_k{k}_n{n}.txt` (lines `r | flags per stack | MOVE`): optimal strategies for (k,n) = (1,5),(2,4),(2,8),(3,5),(3,7),(4,6),(4,7),(5,7). `check_strategy.py` plays each against all n! orders by the literal rules (no shared code); beaten counts 16, 21, 3525, 116, 3188, 715, 4781, 5034 (`witness_log.txt`).
- Upper bounds: `cert_k{k}_n{n}.txt` lists the value of every reachable information state; `check_cert.py` (own move generator) verifies the Bellman equation at each. All eight accepted. Caveat: the certificate presupposes Lemma 0 (the state abstraction); only `slow.py` is free of it.
- Lost orders: `lost_k2_n4.txt` = 2341, 3241, 4231; `lost_k3_n5.txt`, `lost_k4_n6.txt`, `lost_k5_n7.txt`: k+1 orders each, all with 1 arriving last.
- Failure on purpose: `BAD_strategy_k2_n4_a.txt` and `_c.txt` score 0/24; `BAD_cert_k2_n4.txt` (root value 22) is REJECTED. `BAD_strategy_k2_n4_b.txt` still scores 21 (equivalent move; not a rejection test).
- Oversize: `cert_k5_n7.txt` is 569 kB (certificate). `lost_k2_n8.txt` (588 kB) was overwritten with a one-line note.

## 3. Headline

**N(2,n) = 5·N(2,n−1) − 5·N(2,n−2) for all n ≥ 5, with N(2,1..4) = 1, 2, 6, 21.**

Equivalently Σ_{n≥1} N(2,n)xⁿ = (x − 3x² + x³ + x⁴)/(1 − 5x + 5x²), and

- N(2,2j+1) = 3·5^(j−1)·F_{2j+1} (j ≥ 1),
- N(2,2j) = 3·5^(j−2)·L_{2j} (j ≥ 2)

(Fibonacci, Lucas). Growth rate (5+√5)/2 ≈ 3.618, so the poser's guess (exponential, rational generating function) is right.

Status: PROVED for all n, computer-assisted (Theorem 3), conditional on Lemma 0; also COMPUTED directly to n=130. Confidence: high (~97%); the residual risk is the hand argument of Lemma 0, which `slow.py` confirms only on small cases.

Conjectures (computed only, not proved):
- N(k,k+2) = (k+2)! − (k+1), verified for k=1..7.
- N(3,n) = 11a(n−1) − 40a(n−2) + 58a(n−3) − 30a(n−4) for n ≥ 9, verified to n=70; characteristic polynomial (x−3)(x³−8x²+16x−10).
- N(4,n) satisfies an order-10 recurrence (coefficients in `proofs.txt`) for n ≥ 17, verified to n=90.

## 4. Proofs (full text in `proofs.txt`)

- **Lemma 0** (the game reduces to the flag-state DP) — COMPLETE, by hand.
- **Theorem 1**: N(k,n) = n! for n ≤ k+1 — COMPLETE. Keep one item per stack; if all k tops are refused, the unarrived item is 1.
- **Theorem 2**: N(k,n) < n! for every n ≥ k+2 and every k — COMPLETE. Adaptive adversary: 1 := the last arrival; then by induction on j, "at a phase start with nothing unarrived and ≥ j+1 items in the last j stacks, Sorter can be defeated": designate as next minimum the buried item of the first stack of that suffix; unburying it pushes its cover into the last j−1 stacks, which then hold j items.
- **Corollary**: N(k,n) = n! exactly when n ≤ k+1 — confirmed, for all k.
- **Theorem 3** (headline) — COMPLETE, COMPUTER-ASSISTED (`prove_k2.py`). A fixed r-independent policy on the 49 states gives W_r = A·W_{r−1}; each of the 36 "policy minus alternative" slack sequences is checked to satisfy x²−5x+5 on 121 consecutive terms (≥ 49 suffices by Cayley–Hamilton) and to stay ≥ 0 by an exact Q(√5) sign test; induction then gives V = W for all r.
- **N(1,n) = 2^(n−1)** — SKETCH only; computed to n=12, not proved.

## 5. Interpretation

No material ambiguity found. Readings used: endless refused attempts count as failure; IN/SHIFT refusals carry no information (Sorter can count). "The three lost orders have 1 arriving last" is true of the optimal strategy I extracted; Theorem 2 shows every strategy loses some order with 1 last. No alternative reading was computed.

## 6. Code → numbers (all in `solver-b/`)

- `fast.py`: all tables. `slow.py`: independent small cases.
- `explore.py`, `bm.py`: recurrence fitting.
- `prove_k2.py`: Theorem 3, closed form, N(2,n) to n=130.
- `dump.py`, `check_strategy.py`, `check_cert.py`: witnesses and checkers.
