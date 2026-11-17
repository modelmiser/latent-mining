# solver-a report — first to know everything, destruction budget k

Files: `solver-a/`.

## Headline

**c(k) = 1 − 1/k for every k ≥ 3** (c(3) = 2/3, c(4) = 3/4). More precisely:

- **F(n,k) = n − ⌈n/k⌉ = ⌊n(k−1)/k⌋ for all n > k ≥ 3** — PROVED (proofs.txt, Theorems L and U).
- **F(n,1) = F(n,2) = ⌊n/2⌋ for all n** — PROVED.

Both agree with every computed value. The poser's bet c(4) = 5/8 is wrong; "c(k) = 1 − O(1/k)" is right. Confidence about 95%: the residual risk is an error in my own proof that small cases do not expose.

## 1. Tables

F(n,k), COMPUTED exhaustively by `fast.cpp` (decision "can the adversary keep everyone incomplete through round T", T = 1, 2, … until NO):

| k \ n | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
|---|---|---|---|---|---|---|---|---|---|---|
| 1, 2 | 1 | 2 | 2 | 3 | 3 | 4 | 4 | 5 | 5 | 6 |
| 3 | 2 | 2 | 3 | 4 | 4 | 5 | 6 | 6 | 7 | 8* |
| 4 | 2 | 3 | 3 | 4 | 5 | 6 | 6 | 7 | 8* | 9* |
| 5 | 2 | 3 | 4 | 4 | 5 | 6 | | | | |
| 6 | 2 | 3 | 4 | 5 | 5 | 6 | | | | |
| 7 | 2 | 3 | 4 | 5 | 6 | 6 | | | | |
| 8 | 2 | 4 | 4 | 5 | 6 | 7 | | | | |

`*`: upper bound by exhaustive NO; lower bound by the explicit strategy (`gen.py`) verified with `check.py`, because the search did not find the YES in time.

Cross-checks:
- `slow.py` (sets, straight from the definition, no pruning) agrees for n ≤ 5, k ≤ 4 and (6,1), (6,2).
- `fast` with optional pruning off agrees for n ≤ 7 (k ≤ 4) and n = 8 (k ≤ 2); with at most the chain-potential pruning, for all n ≤ 10, k ≤ 4.

Prunings, all safe (they cannot turn YES into NO; an unsafe one would bias F downward): never destroy a link whose message is redundant; memoise NO states up to dihedral symmetry; chain-potential bound; needed-event count ≤ k × rounds left. Logs: `log_p*.txt`, `out_*.txt`.

## 2. Witnesses

- `wit/w_<n>_<k>_T<t>.txt`: 192 sequences found by the search, each surviving round t.
- `witgen/g_<n>_<k>.txt`: the explicit strategy for n = 3..13, 30, 60 and k = 1..5; first completion is n − ⌈n/min(k,n)⌉ every time.
- `check.py`: separate definition-level checker (legality, first completion). `verify_log.txt`: all 65 + 192 files pass; three deliberately wrong inputs are rejected (`bad_repeat.txt`, `bad_budget.txt`, a wrong expected value).
- `sweep.py`: the strategy simulated for 3 ≤ n ≤ 80, 1 ≤ k ≤ 12 (936 pairs), no failures.
- Next value impossible: the NO lines in the logs for the table, Theorem U in general. `ub2range.py` evaluates Theorem U's inequality: it rules out t = n − ⌈n/k⌉ and not t − 1 for all 10,738 pairs with k < n ≤ 400, 3 ≤ k ≤ 30.

**The strategy** ("two opposing combs"). Let m = min(k,n), δ = ⌈n/m⌉, T = n − δ − 1, P_c = ⌊cn/m⌋, Q_c = P_c − T for c = 0..m−1.
- Odd round 2j−1: destroy link (x+1 → x) at x = P_c − j, for each c.
- Even round 2j: destroy link (x−1 → x) at x = Q_c + j, for each c.

Each chain follows information moving at speed one and hits every second round, so it builds delays 1, 2, 3, … on consecutive pieces. The k ramps in each direction overlap (spacing n/k, height T/2), and the opposite slopes make the summed delay at least T − (δ − 1) everywhere, which is what surviving round T needs.

## 3. Proofs (`proofs.txt`)

- Model lemmas L0–L3: COMPLETE. Knowledge is always an arc; delays; "pieces" and the set P of (piece, value) pairs with |P| ≤ kt; a pair of value y cannot exist before round 2y − 1 (this is where the no-repeat rule enters).
- Theorem L, F ≥ n − ⌈n/min(k,n)⌉: COMPLETE.
- Theorem U (count pairs along diagonals; even and odd t separately) and corollary F ≤ n − ⌈n/k⌉ for n > k ≥ 3: COMPLETE, algebra for all residues included.
- k = 1, 2: COMPLETE. The poser's counting argument is invalid as stated (one destruction can create more than one unit of delay), but its conclusion holds.
- NOT proved: even n with n ≤ k < 2n. Computed F = n − 1 there (n = 4, 6, 8). For odd n and k ≥ n, F = n − 1 is proved. F(n,k) = n for even n and k ≥ 2n.

## 4. Statement

- No ambiguity affected the answer. I read "D_t disjoint from D_{t−1}" per directed link, and F as 1 + the last round the adversary can keep everyone incomplete.
- Beliefs checked: F(n,1) = F(n,2) = ⌊n/2⌋ true; F(n,2n) = 2⌊n/2⌋ true; k = n gives n − 1 (odd n proved, even n computed to 8); F(5,3) = 3 true, though F(3,3) = 2 > 1 already; c(4) ≥ 5/8 true but not tight; 1/2 < c(3) < c(4) < 1 and c(k) → 1 true.

## 5. Code

- `fast.cpp`, `drive.sh`: tables. `slow.py`: reference. `gen.py`: strategy. `check.py`: checker.
- `finalize.sh` → `verify_log.txt`. `sweep.py`, `ub2.py`, `ub2range.py`: bound checks.
