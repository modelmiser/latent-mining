# Prior-art report

Setting's standard names: incremental (insertion-only) edge orientation in forests with bounded outdegree; cost = recourse / edge flips (Brodal–Fagerberg line, dynamic graph algorithms). Adjacent: cuckoo hashing with bucket size 2, graph balancing with reassignment.

## CLOSEST PRIOR WORK

1. **Dynamic Representations of Sparse Graphs** — Brodal, Fagerberg, 1999. https://cs.au.dk/~gerth/papers/wads99.pdf (unreadable to me; lemma taken from restatements in items 5 and https://ar5iv.labs.arxiv.org/html/1802.09515). Shows: if an offline δ-orientation sequence uses f flips over |U| updates, the reset-all-out-edges algorithm with threshold Δ makes at most (|U|+f)(Δ+1)/(Δ+1−2δ) flips. Differs: asymptotic competitiveness, no exact game value; restatements disagree on whether Δ = 2δ is allowed (Kaplan–Solomon: Δ′ ≥ 2Δ; Hiken–Wein: Δ > 2δ).
2. **Incremental Edge Orientation in Forests** — Bender, Kopelowitz, Kuszmaul, Porat, Stein, 2021. https://arxiv.org/abs/2107.02318. Shows: outdegree 3 with O(log log n) flips per insertion w.h.p.; notes outdegree 2 gives O(log n) worst-case flips because an unsaturated vertex lies within O(log n) directed steps. Differs: per-insertion asymptotics against a fixed sequence; no flip lower bound, no totals with constants.
3. **Dynamic Edge Orientation via Random Walks: From Trees to Outerplanar Graphs and Beyond** — Marques Domingues, Nguyen, Solomon, 2026. https://arxiv.org/abs/2608.24776. Shows: "folklore" outdegree-2 forest algorithm flipping a directed path from the overfull vertex to a vertex of outdegree ≤ 1; outdegree 2 on outerplanar graphs needs Ω(n) amortized flips even offline. Differs: explicitly says no worst-case lower bound follows for forests from the empty graph; no amortized forest lower bound.
4. **A Simple Greedy Algorithm for Dynamic Graph Orientation** — Berglin, Brodal, 2018. https://cs.au.dk/~gerth/papers/algorithmica18.pdf. Shows: worst-case flip/outdegree trade-offs; cites BF's Ω(n) worst-case flips at outdegree α. Differs: fully dynamic, asymptotic.
5. **Dynamic Edge Orientation on Low-Arboricity Graphs: Breaking the log n Barrier** — Hiken, Wein, 2026. https://arxiv.org/abs/2611.10161. Shows: O(α log log n) amortized flips, first improvement even for dynamic forests; states no non-trivial lower bound is known. Differs: fully dynamic, outdegree 11α.

## VERDICTS

- **T(n) ≤ 3(n−1) (solver A, P5): EASY COROLLARY** of the BF lemma with δ = 1, f = 0 (root the final tree), Δ = 2: factor (Δ+1)/(Δ+1−2δ) = 3; same algorithm and potential. Caveat: needs the Δ = 2δ boundary case.
- **Path normal form / cost = directed distance to an unsaturated vertex (A's P1–P2, B's Lemma 1): LIKELY FOLKLORE** — the augmenting-path repair in items 2 and 3 (the latter attributes it to BF99 Lemmas 2–3). Optimality of that normal form for the game: not found stated.
- **Superadditivity, monotonicity, limit exists (Fekete): EASY COROLLARY** (standard; not found stated for this game).
- **T(n) ≤ n − log₂(n+1) (B's Lemma 4): NOT FOUND.** *(Superseded: Gupta, Kumar and Stein, SODA 2014, was read later; its recurrence gives the bound n − 1 − log₂ n. See the addendum to prior-art-deep.md.)*
- **T(n) ≥ ⌊n/4⌋−1, B's Lemma 2 formula, any positive amortized lower-bound constant for incremental forests at outdegree 2: NOT FOUND.** Items 3 and 5 suggest no such lower bound is recorded.
- **Exact values T(1..16); T ≠ B with T(30) ≥ 7, T(32) ≥ 8, T(64) ≥ 18; limit bounds 9/32 ≤ L ≤ 1; W_j games: NOT FOUND.** *(The bounds are now 1/3 ≤ L ≤ 9/16; see NOTE.md.)*

Queries run (19): incremental edge orientation forests outdegree flips; Brodal Fagerberg dynamic representations amortized reorientations; lower bound flips outdegree 2 forest insertions adversary; OEIS 0,0,0,1,1,2,3,5,5,6,7,9; amortized recourse exact constant trees insertion-only; "flipping game" Kaplan Solomon; Kowalik adjacency queries; online load balancing recourse two machines tree; cuckoo hashing bucket size 2 adversarial evictions; amortized lower bound incremental forest exact minimax; Ghaffari Kopelowitz Stein trees outdegree 2; BF lemma "(k + r)" formula; exact value game adversary forest outdegree 2 n/4; outdegree 1 total flips n log n; BF 1999 lemma δ-orientation; Efron Patel Stein 2411.07553; outdegree 2 "from the empty graph" open problem; BF wads99 lemma 1 (extended); plus one OEIS site search.

## SEQUENCES

- Cap-1 model 0,0,0,1,1,2,3,5,5,6,7,9: looked up on OEIS directly — "terms do not match anything in the table".
- T(n) = 0×7, 1×4, 2×4, 3: equals ⌊n/4⌋−1 on the computed range; not looked up (a match would say nothing).
- Lower bounds at n = 30, 32, 36, 40, 64 are not consecutive terms; not searchable.

## WHO TO ASK

A dynamic graph algorithms specialist working on low-outdegree orientation / recourse (the authors of items 2, 3, 5).

## NOT CHECKED

- The original BF99 text (PDF unreadable); the Δ = 2δ case rests on secondhand restatements.
- Cuckoo-hashing and load-balancing literatures: snippets only.
- Paywalled venues, theses, non-English sources.
- Sources read in full: 0. Read in part: 5 (item 3 pages 1–6 and 29–30; item 4 pages 1–5; items 2, 5 and Kaplan–Solomon via a summarising fetch of the first ~100k characters).
