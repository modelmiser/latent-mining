# Prior-art search: walk-paging game W(k, m)

The setting is deterministic online paging in the **access graph model** (graph = infinite path) combined with **unit-cost prefetching of one adjacent page** (a "block-aware" / "granularity-change" fetch with overlapping blocks of size 2), measured as an exact minimax fault count against a bounded offline cost, not as a ratio.

## CLOSEST PRIOR WORK

1. **Competitive Paging with Locality of Reference** — Borodin, Irani, Raghavan, Schieber, STOC 1991 / JCSS 1995. https://research.ibm.com/publications/competitive-paging-with-locality-of-reference
   Shows: introduces the access graph model (requests are walks on a graph) and bounds LRU/FIFO per graph. Differs: demand paging only, ratios not exact values.
2. **Truly Online Paging with Locality of Reference** — Fiat, Mendel, FOCS 1997 / arXiv 2006. https://arxiv.org/abs/cs/0601127
   Shows: strongly competitive access-graph algorithms that do not know the graph. Differs: no prefetching; asymptotic ratios.
3. **Spatial Locality and Granularity Change in Caching** — Beckmann, Gibbons, McGuffey, 2022. https://arxiv.org/abs/2205.14543
   Shows: on a miss any subset of the item's block loads at unit cost; deterministic lower bound (a(k−h+1)+B(h−a))/(k−h+1) and a near-matching policy. Differs: blocks are a fixed disjoint partition, requests unrestricted, resource-augmented ratios.
4. **Competitive Algorithms for Block-Aware Caching** — Coester, Levin, Naor, Talmon, 2022. https://arxiv.org/abs/2205.12249
   Shows: block fetching/eviction models; ignoring blocks loses a factor O(β). Differs: disjoint blocks, arbitrary requests, asymptotic bounds.
5. **Strongly Competitive Algorithms for Caching with Pipelined Prefetching** — Gaysinsky, Itai, Shachnai, ESA 2001 / IPL 2004. https://link.springer.com/chapter/10.1007/3-540-44676-1_4
   Shows: prefetching with requests as walks on an access graph; an online algorithm within factor 2 of the best online one. Differs: objective is execution time with fetch delay, not fault count; no neighbour-only restriction.
6. **LRU Is Better than FIFO** — Chrobak, Noga, Algorithmica 1999. https://link.springer.com/article/10.1007/PL00009255
   Shows: LRU's ratio is at most FIFO's on every access graph. Differs: demand paging, ratios.

## VERDICTS

- **Headline F(k,m) = max(m+1, 2m−⌊k/2⌋+1), D = min(m, ⌊k/2⌋)−1: NOT FOUND.** Nothing found combines path access graph, neighbour prefetch and an exact bounded-opt game value.
- **Threshold F = m+1 exactly when k ≥ 2m: NOT FOUND.**
- **Upper- and lower-bound theorems (FILL-FAR / ALG, ADV): NOT FOUND.**
- **F ≤ 2m (Lemma 2): EASY COROLLARY.** On a path, far-end eviction is Belady's rule, and a fault loads at most 2 pages: the β = 2 case of the "ignore the blocks, lose O(β)" baseline in item 4. Blocks here overlap, so it is an analogue, not an instance.
- **opt ≥ ⌈distinct pages/2⌉ (Lemma 1): LIKELY FOLKLORE**, a counting argument; not found stated.
- **F(k,1) = 2, F(2,m) = 2m, F(3,2) = 4, F(4,2) = 3, and the tables: NOT FOUND.**

**Integer sequences (OEIS, looked up):** table by antidiagonals (2,4,3,6,5,4,8,7,6,5), no match; table by rows of a triangle (2,2,4,2,4,6,2,3,6,8,2,3,5,8,10), no match; row m = 4 (8,8,7,7,6,6,5,5,5,5), no match; column k = 6 extended by the formula (2,3,4,6,8,10,12,14), 41 matches, the first ten unrelated to paging, the other 31 not inspected. Rows and columns are piecewise linear, so OEIS results carry little weight.

**Queries run (17), by topic:** access-graph paging on path/line (4 variants, including LRU on trees and FAR); granularity-change caching (2); block-aware caching; free adjacent-page prefetch (3 variants); integrated prefetching and caching (2); exact faults under an adversary budget with empty cache; one-dimensional walk paging; batched paging, two pages per fault; Chrobak–Noga; paging minimax game value.

## WHO TO ASK

A specialist in competitive analysis of online algorithms: paging with locality of reference (access graphs) or block-aware caching.

## NOT CHECKED

- I read no source in full myself. Two full-text pages (item 3, and the first two-thirds of item 2) and three abstracts (2, 3, 4) were read through a summarising fetch tool; items 1, 5, 6 were judged from search snippets (the item 5 PDF did not parse).
- I could not confirm from any source that LRU is optimal on path or tree access graphs in Borodin et al.
- Paywalled journal versions, theses, and older systems literature on read-ahead were not searched.
