# solver-a report: game W(k, m)

**Headline: F(k, m) = max(m + 1, 2m − ⌊k/2⌋ + 1) for all k ≥ 2, m ≥ 1, so D(k, m) = min(m − 1, ⌊k/2⌋ − 1).** The candidate formula in the problem is exact. I have a written proof of both bounds (`proofs.txt`); the upper bound uses Belady's theorem as a black box. Every computed value agrees.

## 1. Tables

F(k, m), all entries COMPUTED and all equal to the formula:

| m \ k | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2* |
| 2 | 4 | 4 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3* |
| 3 | 6 | 6 | 5 | 5 | 4 | 4 | 4 | 4 | 4 | 4 | 4 | 4* |
| 4 | 8 | 8 | 7 | 7 | 6 | 6 | 5 | 5 | 5 | 5 | 5 | 5* |
| 5 | 10 | 10 | 9 | 9 | 8 | 8* | 7 | 7 | 6 | 6 | 6 | 6* |
| 6* | 12 | 12 | 11 | 11 | 10 | 10 | 9 | 9 | 8 | 8 | 7 | 7 |

Three methods, with the sizes each reached:

- **Unpruned definitional solver** (`game.py … slow`: full work function over all caches, every Online move, least fixed point over the reachable state graph): the 13 cases (k,m) = (2,1..4), (3,1..4), (4,1..3), (5,2..3). Output in `table_slow.txt`.
- **Pruned exact solver** (`game.py … fast`, domination pruning, Lemma D): m ≤ 5, k ≤ 12, except (7,5), which timed out at 200 s. It agrees with the unpruned solver on all 13 shared cases. Output in `table_py.txt`.
- **Two fixed strategies, each checked exhaustively against every opponent behaviour**: Online strategy FILL-FAR gives the upper bound for k ≤ 13, m ≤ 6; adversary ADV gives the lower bound for k ≤ 13, m ≤ 6. The starred entries (row m = 6, column k = 13, and (7,5)) rest on these two matching bounds only, not on the full game solver.

Pruning safety: an unsafe work-function pruning could only raise opt and bias F down; an unsafe Online-move pruning could only bias F up.

Checks that could fail:
- Prefetching outward forever (`outward`) is unbounded for k ≤ 2m (tested k ≤ 9, m ≤ 4).
- Plain LRU gives 2m, above F once k ≥ 4 and m ≥ 2.
- A sabotaged adversary (`BREAK=1`, steps onto cached frontier pages) scores exactly one below F everywhere (`adv_break.txt`).

## 2. Witnesses

- **Upper bound, FILL-FAR** (`strat.py fill_far`): at a fault at p, if |C| ≤ k − 2 load p and its uncached neighbour (+1 if neither is cached), otherwise load p only; when over capacity evict the page farthest from p. Worst case over all legal walks is in `strat_fill_far.txt`.
- **Lower bound, ADV** (`adv.py`, rule stated in `proofs.txt`, Theorem L):
  - Bounce: while Online lacks a page of the region both caches should hold, walk to the nearest such page.
  - Fill stage: step onto a frontier page Online has not cached, if one exists.
  - After k − 1 pages are visited: fix the direction of an uncached frontier and proceed in blocks. Request a new page; if Online then holds the whole region, request the next new page too, otherwise bounce.
  - `adv.py` asserts every move is legal by exact opt and takes the minimum over all Online replies (`adv_12_5.txt`, `adv_k13.txt`, `adv_m6.txt`; with unpruned Online moves for k ≤ 5, m ≤ 3 in `adv_slow.txt`).
- **Sample plays** ADV versus FILL-FAR for k ≤ 7, m ≤ 4, with opt recomputed unpruned for k ≤ 5, are in `witnesses.txt` (written by `mkwit.py`).

## 3. Headline answer and the stated beliefs

F(k, m) = max(m + 1, 2m − ⌊k/2⌋ + 1). Confidence is high: two written proofs, and both strategies were machine-checked on every case reached.

- True as stated: F ≤ 2m; F(k,1) = 2; F(2,m) = 2m; F(3,2) = 4; F(4,2) = 3; F/m → 2 for fixed k.
- F = m + 1 for k ≥ 2m + 2: true, but the exact threshold is k ≥ 2m; F(2m − 1, m) = m + 2.
- The guess about D: true, D = ⌊k/2⌋ − 1 once m ≥ ⌊k/2⌋.
- "Always prefetch outward" is right only while filling; afterwards it loses without bound.

## 4. Proofs (`proofs.txt`)

- **Lemma 1, COMPLETE**: opt ≥ ⌈(distinct pages)/2⌉.
- **Lemma 2, COMPLETE**: a no-prefetch schedule with at most 2·opt faults exists, holding only requested pages.
- **Lemma 3, COMPLETE given Belady's furthest-in-future theorem** (cited, not reproved): from an interval cache, far-end eviction is a Belady execution.
- **Theorem U (upper bound), COMPLETE given Lemma 3**: FILL-FAR faults at most max(opt + 1, 2·opt − j + 1), j = ⌊k/2⌋. The first j faults each add two pages; after that it is Belady from a cache containing every visited page, compared with the Lemma 2 schedule, which has already paid at least 2j − 1 faults.
- **Theorem L (lower bound), COMPLETE**: ADV forces max(m + 1, 2m − j + 1).
  - In the fill stage a new page is a free hit only if both frontier pages are cached, and never twice in a row, so at most j − 1 are free among the first 2j ranks.
  - Afterwards each block costs offline one fault and Online at least two.
  - An explicit offline schedule with m faults shows legality.
- **Lemma D (domination pruning), COMPLETE**.

None of these proofs has been read by anyone but me. The likeliest place for a slip is the bookkeeping at the switch from fill stage to block stage (even versus odd k); the exhaustive ADV runs cover k ≤ 13 there.

## 5. Interpretation

- A hit changes nothing (no prefetch, no eviction); a fault may load fewer pages than allowed.
- Infinite play with infinitely many faults counts as payoff ∞; this never binds.
- No ambiguity changes the answer, so no alternative reading is reported.

## 6. Files

All in `solver-a/`:

| File | Produces |
|---|---|
| `game.py` | exact solver → `table_slow.txt`, `table_py.txt` |
| `strat.py` | fixed Online strategies → `strat_fill_far.txt` |
| `adv.py`, `adv6.py` | adversary ADV → `adv_12_5.txt`, `adv_k13.txt`, `adv_m6.txt`, `adv_slow.txt`, `adv_break.txt` |
| `mkwit.py` | `witnesses.txt` |
| `proofs.txt` | the proofs |
