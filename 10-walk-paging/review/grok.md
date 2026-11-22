<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `solver-b/proofs.txt:9-34` — Round 1 is the special case load `{0,1}`; after that a fault is the outside neighbour of the interval. Pair phase gives `|W|=2·f1≤2h` and `opt≥f1−1`. For `f2≥1`, `f1=h` (even: `|W|=k`; odd: first single fault grows `k−1` to `k`). After each run the window is the `k` pages from the fault back along the walk; the next run is the opposite end, so `[t0,t1]` meets `k+j` pages. `x0` is cached at `t0`, hence at most `k−1` of the other pages are, so `≥j` entries fall in `(t0,t1]`. Those intervals are disjoint from each other and from the first run, and `h+f2≤2·opt+1−h`.

Q2: CONFIRMED — `solver-b/proofs.txt:37-61` — Stage 0 loads `g=min(m,h)` pairs on `2g` pages (`2g≤k`); the `2g−1` new pages cost online `≥g`, and `m<h` stops at `≥m+1`. The cycle’s `(i)`/`(ii)` choice is one hindsight schedule: `|O|=k` and `|O|=k−1`, both parities, each cycle `+1` offline and `≥2` online, prefixes `≤m`, total `≥2m+1−h`.

Q3: CONFIRMED — `solver-a/proofs.txt:6-47` — Lemmas 1–2 hold (no-prefetch `N` faults at most twice). Lemma 3 invokes Belady only for demand paging on an interval. Theorem U follows: phase 1 is `≤f+1`; after `t*` the Belady rule on a superset of `N`’s cache costs `≤2f−2j+1`.

Q4: CONFIRMED — `solver-a/proofs.txt:57-89` — Zeros sit in `{3,…,k−1}`, no two adjacent, so at most `⌈(k−3)/2⌉=j−1` of the first `2j` ranks, and at most `m−1` when `2m≤k−1`. Each later block is `≥2` online and `1` offline under the pair tiling; both parities give `≥max(m+1,2m−j+1)`.

Q5: REFUTED — `solver-b/proofs.txt:71-72` and `solver-a/proofs.txt:102` versus `solver-a/game.py:15-34` — On a virtual hit the text claims `|V\R|` falls by 1, but a full real cache that evicts a page of `V` leaves `|V\R|` unchanged and the credit invariant fails. A’s “(b) always prefetch” is not the prune the solver uses: `maximal_containing` may drop `q` whenever `|C∪{p,q}|>k`.

Q6: CONFIRMED — `chain/3-question.txt:3-9` matches both proofs: round 1 is page 0, hits do not change the cache, legality is `opt` of the extended history, the adversary may stop, infinite play is allowed, and `F` is the least such `N`.

Q7: CONFIRMED — `verifier/table_mode2.txt:1-65`, `verifier/table_m6.txt:1-13`, `README.md:49-56` — All 78 cells `k=2..14`, `m=1..6` are logged `EQ` to the formula, and the printed `k≤12` rows match.

Q8: REFUTED — `README.md:42,73-74,86-87` versus `solver-b/proofs.txt:38` and `verifier/wk.cpp:9-11` — The replays are mode 1 (maximal caches only), not every legal reply. The adversary walks toward an uncached neighbour of 0, not “the side prefetched on the first fault” (the first cache may be `{0}`). “Had to drop pages” is stronger than the proof: one missing page suffices when `|O|=k−1`.

F: MODERATE — `solver-b/proofs.txt:71-72` — Virtual-hit case states `|V\R|−1` for an unspecified real fault; evicting a page of `V` breaks `credit≥|V\R|`. — high — Keep `V∩R`, add `p`, evict only from `R\V`.
F: MODERATE — `solver-a/proofs.txt:102` — “Always prefetch an uncached neighbour” is not implied by the domination test and is not what `online_options` does once eviction is required. — high — Say online may be limited to inclusion-maximal caches.
F: MODERATE — `solver-b/proofs.txt:75` — Claims unpruned agreement also for `k=2..4`, `m=1..3`; `solver-b/table_py_noprune.txt` has only the 18 cases `k=3..8`, `m=2..4`. — high — Delete the clause, or add the log.
F: LOW — `solver-a/proofs.txt:80` — “`2m≥k` (i.e. `m≥j`)” is false for odd `k` (`k=5`, `m=2`). — high — Drop the “i.e.”.
F: LOW — `solver-a/proofs.txt:61-62` — `B:=j` is written only in the even branch; the odd loop uses `B` anyway. — medium — Set `B:=j` before the parity split.
F: LOW — `solver-a/proofs.txt:90` — A frontier request is outside `O` and outside the online cache, so it is not a hit inside both caches. — high — Say only the walk up to the frontier stays in `O`.
F: LOW — `README.md:73-74` — “Prefetched on its first fault” excludes a first cache of `{0}`. — high — “toward a neighbour of 0 that round 1 did not cache”.
F: LOW — `README.md:86-87` — “Had to drop pages … to make room” covers only the full-window counting case. — high — “then at least one window page is missing (at least two if `|O|=k`)”.
F: LOW — `README.md:42` — “Every online reply” does not match mode-1 logs (`verifier/advb.txt`, `adva.txt`, `wk.cpp:10`). — high — “every inclusion-maximal reply”.
F: LOW — `README.md:109-113` — Omits `prior-art.md:15-16` (Gaysinsky et al.: access-graph walks plus prefetch, different objective). — high — Cite it as the nearest partial combination.
F: LOW — `verifier/table_mode2.txt:1` — The `k≤14`, `m≤6` table is mode 2 (domination pruning); unpruned `table_mode0.txt` is only a sparse set through `(8,4)`. — high — Say “exact under the domination prune”.
