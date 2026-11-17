# solver-b report: charged pebbles, clean delivery

**Headline: YES as far as anyone here can tell, but NOT proved.** No counterexample exists for any (p,k) I could reach; C(p,k) = (pk+2)·2^(p-2) in every computed case. The lower bound is proved for all (p,k). The upper bound is proved only for p=2 (all k) and k=0 (all p); for p≥3, k≥1 it is computed, not proved.

## 1. Tables

**Real game, exhaustive BFS over all reachable states** (state = multiset of (node, charge), hand charges included; `fast.cpp`). Board truncated at N; every run had max-pebbled-node < N, so the truncation never bound. All entries COMPUTED and all equal (pk+2)·2^(p-2).

| p | k range | C(p,k) | largest state count |
|---|---|---|---|
| 2 | 0..10 | 2,4,6,…,22 | 3,025 |
| 3 | 0..8 | 4,10,16,22,28,34,40,46,52 | 661,890 |
| 4 | 0..6 | 8,24,40,56,72,88,104 | 112,202,419 |
| 5 | 0..2 | 16,56,96 | 117,951,562 |

In every case the largest node ever pebbled was C(p,k) + 2^(p-1) − 1.

`fast.cpp` agrees with the slow direct implementation `slow.py` (clean value, max node and state count) on 11 cases: (2,0),(2,1),(2,2),(2,3),(2,5),(3,0),(3,1),(3,2),(3,3),(4,0),(4,1).

**Relaxed game** (q uncharged pebbles, a total budget of f F-moves; `relaxed.cpp`, cross-checked against `relaxed.py` on q=3 f≤5 and q=4 f≤4 by state count). Every real play is a relaxed play with f=pk, so this is an upper bound on C. COMPUTED exhaustively: max clean node = (f+2)·2^(q-2) exactly, for q=2 f≤100, q=3 f≤60, q=4 f≤40, q=5 f≤25, q=6 f≤6 (121,871,315 states). Combined with the proved lower bound this gives, beyond the table above:

- C(3,k) for k≤20, C(4,k) for k≤10, C(5,k) for k≤5 (e.g. C(5,3)=136, C(5,5)=216)
- **C(6,1) = 128** (one pebble count beyond the real-game BFS)

These are tagged COMPUTED (upper, relaxed) + PROVED (lower).

## 2. Witnesses

- `wit_bfs_P_K.txt` for (2,1),(2,2),(3,1),(3,2),(4,1): shortest move lists from the BFS.
- `wit_strat_P_K.txt` for 14 pairs up to (6,2)→224, (7,1)→288, (8,1)→640: the general strategy, from `gen_strategy.py`.
- All pass `check_witness.py p k n file`, an independent replay with individual charges.
- Upper-bound certificates `cert_P_K.txt` for (2,1),(2,2),(3,1),(3,2),(4,1): the full reachable set. `check_cert.py` verifies the start state is present, the set is closed under every legal move, and no state is clean beyond n. All five pass. (`cert_4_1.txt` is 546 kB — over the 300 kB guideline, kept because it is a certificate.)
- Deliberate failures, all rejected (`neg/`): witness with a line deleted; witness with an inflated charge; correct witness claimed for n+1; correct witness with k=0; certificate with one state deleted ("not closed"); certificate claimed for n−1; certificate for the wrong k.

Larger cases have no compact impossibility certificate: the reason is the exhaustive search itself.

## 3. Headline answer and confidence

YES, C(p,k) = (pk+2)·2^(p-2); confidence about 95%. The residual doubt is entirely the unproved upper bound for p≥3 at sizes beyond the tables.

The "end-game trick" the problem worried about does not appear. The relaxed game, where charge is a pooled budget and no pebble can ever be stuck with zero charge, gives the same values, so the only binding constraint is the total pk.

Conjectures (computed, not proved):
- **R**: in the relaxed game the max clean node is (f+2)·2^(q-2) and the max node pebbled is (f+4)·2^(q-2) − 1.
- **J**: after f F-moves, a pebble with exactly j pebbles strictly below it sits at ≤ 2^q − 2^(q-1-j) + f·2^(q-2). Exhaustive for q=2,3 (f≤5) and q=4 (f≤4), `conj.py`. J is true on the data but not inductive: {2,6,7} with q=3 satisfies it at f=0 yet needs one F-move.

## 4. Proofs (`proofs.txt`)

- **Theorem 1, COMPLETE**: C(p,k) ≥ (pk+2)·2^(p-2) for all p≥2, k≥0. The problem's strategy, with the rotation made precise: always put a hand pebble of maximal charge on top. Invariant: all charges lie in {c, c−1} and the base pebble has c, so all pk units get used.
- **Lemma W, COMPLETE**: reversible window lemma (q pebbles above a base: lowest ≤ base+2^(q-1), all ≤ base+2^q−1). Gives C(p,0) = 2^(p-1).
- **Theorem 2, COMPLETE**: C(2,k) = 2k+2. The lowest pebble is ≤ 2 + (number of F-moves so far), by induction on time.
- **Upper bound for p≥3, k≥1: NOT PROVED**, and I offer no sketch as a proof. `proofs.txt` records where the induction breaks: clearing below the lowest pebble is not reversible once F-moves occur, and the per-pebble invariant J is too weak to carry it.

## 5. Interpretation

- No real ambiguity found. Rule F's "i ≥ 2" is automatic, since node 1's predecessor is always pebbled.
- The formula is not meaningful for p=1 (C(1,k)=1); the statement excludes it.
- Other reading, reported because it was cheap: a pooled budget of pk F-moves instead of per-pebble charges gives identical values wherever both were computed.
- Checked beliefs from the statement: C(2,1)=4 exact; C(2,k)=2k+2 exact (proved); C(3,1)=10 exact (not merely ≥10); k=0 gives 2^(p-1) (proved).
- "Bennett-style gains less" is correct for p≥3 (2^(p-1)−1 nodes for p−1 F-moves against (p−1)·2^(p-2)); for p=2 the two coincide.

## 6. Code → numbers

All in `solver-b/`.

| File | Produces |
|---|---|
| `slow.py` | reference BFS |
| `fast.cpp` | real-game tables (`real_table.txt`, `real_table_big.txt`), BFS witnesses, certificates |
| `relaxed.py`, `relaxed.cpp` | relaxed tables (`rel_q2.txt` … `rel_q6.txt`) |
| `conj.py` | conjecture J test |
| `gen_strategy.py` | strategy witnesses |
| `check_witness.py`, `check_cert.py` | independent checkers |
| `proofs.txt` | proofs |
