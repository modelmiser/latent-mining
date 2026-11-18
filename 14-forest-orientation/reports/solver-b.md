# solver-b report

Files: `solver-b/`.

## 1. Tables

| n | 1–7 | 8–11 | 12–15 |
|---|---|---|---|
| T(n) | 0 | 1 | 2 |

All COMPUTED, exhaustive over both players, and all equal B(n). The beliefs "T(n)=0 for n≤7" and "T(8)=1" are true.

- n ≤ 10: `slow.py`, the definition taken literally (explicit directions, Algorithm may re-orient the whole forest; only isomorphic positions are merged).
- n ≤ 14: `fast.cpp`, exact minimax over all Adversary pairs, with Algorithm restricted to the normal form of Lemma 1. Its mode 1 (every valid orientation of the merged tree) agrees for n ≤ 9.
- n = 15 (and 14 again): `fast3.cpp` refutes "Adversary forces B(n)+1" exhaustively (4.5 million positions at n=15); T ≥ B is Lemma 2. Pruning used: Lemma 1 (proved) and a cutoff by the proved lower bound of Lemma 2, which can only return "Adversary wins".
- n = 16 was not reached (memory cap).
- `exactv.cpp`: for n ≤ 11 the exact value of every reachable position equals the Lemma 2 formula P−1.

Lower bounds only (NOT exact; Adversary restricted to a subfamily of moves, all Algorithm normal-form replies searched):

| n | 30 | 32 | 36 | 40 | 64 |
|---|---|---|---|---|---|
| T(n) ≥ | 7 | 8 | 9 | 10 | 18 |
| B(n) | 6 | 7 | 8 | 9 | 15 |

n = 36 and 40 are search output only (no certificate file). By monotonicity T(n) > B(n) for every 30 ≤ n ≤ 43.

Not settled: 16 ≤ n ≤ 29. The restricted Adversary families (`fast2.cpp` ADV=3 and 4) fail to beat B(n) there; that is evidence, not a bound. A search for T(128) ≥ 39 timed out; above n=43 only n=64 was checked.

## 2. Witnesses

- **Lower bounds:** `cert_n30_k7.txt` (140 positions), `cert_n32_k8.txt`, `cert_n64_k18.txt` (10 MB), plus small `cert_n8_k1.txt` and `cert_n12_k2.txt`. Each line gives a position and the pair Adversary names. `check_cert.py` (separate Python code, explicit orientations) replays every normal-form reply; all five verify. These rely on Lemma 1.
- **Upper bounds:** `upper_n7.txt`, `upper_n8.txt`, `upper_n11.txt`, `upper_n12.txt`, `upper_n13.txt` list positions with bounds. `check_upper.py` verifies that for every listed position and every Adversary pair some legal reply stays inside the list within budget. This needs no lemma. All five verify (T(13) ≤ 2 over 270,048 positions). n = 14 and 15 have no certificate file, only the exhaustive runs.
- **Checks seen to fail:** `cert_n30_k7_MUTANT.txt` (one named vertex changed) and `upper_n8_MUTANT.txt` (lines deleted) are rejected; asking the n=30 certificate for k=8 is rejected.
- **Format:** a tree is `(digit children)` with digit = 2 − outdegree; see `README.txt`.

## 3. Headline

**Is T(n) = B(n) for every n? NO** (confidence about 97%; it rests on Lemma 1 and the independently checked certificate). T(30) ≥ 7 > 6 = B(30).

- **Least deviating n:** somewhere in 16 ≤ n₀ ≤ 30. My guess is n₀ = 30, at about 55%. The problem's guess of 16–24 is not confirmed: 16–29 remain open.
- **Mechanism:** the problem's guess is right in kind. A paid merge of saturated x, y leaves x→y with both saturated. Naming two such x in different trees costs 1 and makes one "bad" (both out-neighbours saturated), or costs 2. Naming two bad vertices costs 2. Bad trees can be forced on 16 vertices at cost 3 or on 14 at cost 2, and 16+14 gives 3+2+2 = 7.
- **Formula:** not found.
- **Limit:** L = lim T(n)/n exists and equals sup T(n)/n. Proved: 9/32 ≤ L ≤ 1, the lower bound from T(64) ≥ 18. The greedy recursion in `gdp.py` gives T(n) ≤ 0.558n for n ≤ 3000 (numeric only).
- **Conjecture (guess):** T(4^k) ≥ (4^k−1)/3 − k, hence L ≥ 1/3. It holds exhaustively over Algorithm for k=3. Whether L = 1/3 I do not know (about 35%).

## 4. Proofs (`proofs.txt`)

- **Lemma 1, COMPLETE.** Algorithm loses nothing by either flipping nothing and orienting the new edge out of an unsaturated endpoint, or flipping one directed path from a saturated endpoint through saturated vertices to the first unsaturated one.
- **Lemma 2, COMPLETE.** With s = components containing a saturated vertex, q = other components of size ≥ 2, g = singletons, Adversary forces at least s + ⌊(q+⌊g/2⌋)/2⌋ − 1 from any position. So T(n) ≥ B(n).
- **Lemma 3, COMPLETE.** T is monotone and T(a+b) ≥ T(a)+T(b).
- **Lemma 4, COMPLETE.** T(n) ≤ n − log₂(n+1), by the greedy shortest-path Algorithm. So T(n) ≤ Cn holds with C = 1.
- **Lemma 5, COMPLETE.** The limit exists (Fekete).
- **SKETCH only:** the depth recursion behind L ≥ 1/3. A naive "cost + depth" induction fails when Algorithm overpays by 1 in one subtree.

The two beliefs about outdegree-0 vertices are true.

## 5. Interpretation

I found no ambiguity that changes values. I read the rules as: payment counts old edges only; Algorithm may flip edges in any component (shown irrelevant by Lemma 1); Adversary may name any pair in different components, singletons included.

## 6. Code per number

- Table: `slow.py`, `fast.cpp`, `fast3.cpp` (output `run_exact_14_15.txt`), `exactv.cpp`.
- Lower bounds: `fast2.cpp` / `fast3.cpp` (ADV=3 for n=30, 36, 40; ADV=7 for 32, 64), `certgen.cpp`, `check_cert.py`.
- Upper certificates: `upgen.cpp`, `check_upper.py` (outputs `run_check_upper11.txt`, `run_check_upper13.txt`).
- Gadget hints: `fast3bonus.cpp`. Greedy bound: `gdp.py`.
