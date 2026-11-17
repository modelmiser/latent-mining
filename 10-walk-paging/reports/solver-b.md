# solver-b report: F(k,m) for the walk-paging game W(k,m)

**Headline (PROVED, both directions; proofs.txt): F(k,m) = max(m+1, 2m+1−⌊k/2⌋) for all k≥2, m≥1; D(k,m) = min(m, ⌊k/2⌋) − 1.** The statement's candidate formula is exact. Confidence: high (two complete elementary proofs, plus exhaustive agreement on 50+ instances by a game solver and by an independent strategy checker).

## 1. Tables (all COMPUTED, exhaustive)
Method (`game.py`): state = (offline work function, Online cache), translated to the current page; reachable states enumerated; value = least fixpoint of "Adversary max / Online min" value iteration (handles infinite play). `prune=0` implements the definition directly (all sub-caches for offline, all legal Online moves); `prune=1` uses the dominance lemma (proofs.txt, complete proof).

F(k,m), prune=1 (`table_py.txt`, `table_py_extra.txt`):

| m\k | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 |
| 2 | 4 | 4 | 3 | 3 | 3 | 3 | 3 | 3 | 3 |
| 3 | 6 | 6 | 5 | 5 | 4 | 4 | 4 | 4 | 4 |
| 4 | 8 | 8 | 7 | 7 | 6 | 6 | 5 | 5 | 5 |
| 5 | 10 | 10 | 9 | 9 | 8 | 8 | 7 | 7 | 6 |
| 6 | 12 | 12 | 11 | – | – | – | – | – | 8 |

Also F(12,6)=7. "–" = not computed (Python timed out at 120 s). Every entry equals the formula. Unpruned (`prune=0`, `table_py_noprune.txt`) agrees on k=3..8, m=2..4 (18 cases) and k=2..4, m=1..3. Asked instances: F(k,1)=2, F(2,m)=2m (m≤6), F(3,2)=4, F(4,2)=3 — all confirmed.

## 2. Witnesses
- **Online strategy ALG** (upper bound): cache is an interval window W. Round 1 load {0,1}. On a fault at p just outside W on side d: if |W| ≤ k−2 load p and p+d, evict nothing; otherwise load p only and, if the window would exceed k, evict the far end.
- **Adversary strategy ADV** (lower bound): walk straight 2·min(m,⌊k/2⌋)−1 steps away from Online's first prefetch; then, holding an explicit offline cache O = interval, repeat: if Online lacks a page of O, walk to the nearest one (free Online fault); else step out past an end b to b+1, then to b+2 if Online did not load it, otherwise back to the nearest page of O that Online dropped. Each such cycle costs offline 1 fault, Online ≥2.
- `verify.py` (shares no code with game.py; brute-force unpruned opt): `alg k m` runs ALG against every legal adversary (all reachable states) and reports worst faults; `adv k m` runs ADV against every Online move sequence, asserting legality of every request and Online faults ≥ bound. Logs `verify_log1.txt` (k=2..5, m=1..3), `verify_log2.txt` (ALG: (6,3),(6,4),(7,4),(8,4),(4,5),(3,5); ADV: (6,3),(6,4),(4,4),(3,4),(7,4)) — all OK, ALG worst = bound exactly.
- Checks watched failing: ALG variant "always prefetch" → VIOLATION (17 faults vs bound 5 at (4,3)); ADV asked for bound+1 → FAIL at (4,3), (3,2), (6,3).
- `witness_plays.txt`: for k=2..7, m=1..4 the explicit walk of ADV vs ALG with ALG's faults (= F) and opt (≤ m), recomputable by any program.

## 3. Headline answer
F(k,m) = max(m+1, 2m+1−⌊k/2⌋). So F = m+1 exactly when k ≥ 2m; D = ⌊k/2⌋−1 for m ≥ k/2 (bounded in m, as guessed: "about k/2"); F/m → 2. All UNVERIFIED beliefs are true; one is not sharp: F = m+1 already for k ≥ 2m, not only k ≥ 2m+2.

## 4. Proofs (proofs.txt)
- Upper bound F ≤ max(m+1, 2m+1−h): **COMPLETE**. Pair phase: window has 2·f1 pages, ≥2·f1−2 visited ⇒ ALG ≤ opt+1. Slide phase: group single faults into same-direction runs; the first run forces ≥2h−1+j0 distinct visited pages; each later run of j faults forces the walk across k+j distinct pages after a time when offline holds ≤k, so ≥j new offline entries in disjoint time intervals; a fault gives ≤2 entries ⇒ opt ≥ ⌈(2h−1+f2)/2⌉ ⇒ ALG = h+f2 ≤ 2·opt+1−h.
- Lower bound: **COMPLETE**. ADV as above with the offline service constructed cycle by cycle (choice between loading {b+1,b+2} or {b+1} made after seeing Online's reply, which is legitimate for opt). Odd k handled by a first cycle with |O| = k−1.
- Dominance lemma for pruning: **COMPLETE**.
- No sketches are relied on. Proofs were not machine-checked; they are corroborated by the exhaustive checks above for k ≤ 8, m ≤ 5 only.

## 5. Interpretation
- Round 1's fault on page 0 counts in the payoff; Online may prefetch ±1 there.
- |C'| ≤ k allows voluntarily smaller caches, and any evictions; a hit changes nothing (no prefetch on hits).
- Legality is checked on the history including the new request (opt ≤ m), so at most 2m distinct pages are ever visited.
- F exists (≤ 2m) so "least N" is well defined; infinite plays occur (hits forever) and are handled by the least fixpoint. I found no ambiguity that changes the answer; no alternative reading computed.

## 6. Code
`game.py` (tables; args k m prune), `pv.py` (optimal-play dump, exploratory), `verify.py` (strategy checker), `witness.py` (→ witness_plays.txt), `proofs.txt`. All in solver-b/.
