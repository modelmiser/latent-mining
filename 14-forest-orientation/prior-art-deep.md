# Deep prior-art report

Method: PDFs downloaded and text-extracted locally. "Scanned" means the whole extracted text was keyword-searched (lower bound, Ω, forest, tree, incremental, out-degree 2, capacity) and every hit read in context. Nothing was computed.

## Sources read

1. Brodal, Fagerberg, "Dynamic Representations of Sparse Graphs", 1999. https://cs.au.dk/~gerth/papers/wads99.pdf. Sections 3–5 read in full (Lemmas 1–3, Theorems 2–4, open problems).
2. Bender, Kopelowitz, Kuszmaul, Porat, Stein, "Incremental Edge Orientation in Forests", 2021. https://arxiv.org/abs/2107.02318. Introduction and Remark 12 read; rest scanned.
3. Marques Domingues, Nguyen, Solomon, 2026. https://arxiv.org/abs/2608.24776. Sections 1, 6.3, 7 read; rest scanned.
4. Hiken, Wein, 2026. https://arxiv.org/abs/2611.10161. Section 1.3 read; rest scanned.
5. Kowalik, "Adjacency Queries in Dynamic Sparse Graphs", 2007. https://www.mimuw.edu.pl/~kowalik/papers/adjacency.pdf. Scanned.
6. Kopelowitz, Krauthgamer, Porat, Solomon, 2014. https://arxiv.org/abs/1312.1382. Scanned.
7. Berglin, Brodal, 2018. https://cs.au.dk/~gerth/papers/algorithmica18.pdf. Introduction read; rest scanned.
8. Bernstein, Holm, Rotenberg, "Online Bipartite Matching with Amortized O(log² n) Replacements". https://arxiv.org/abs/1707.06063. Sections 1 and 6.1 read; rest scanned.
9. Chaudhuri, Daskalakis, Kleinberg, Lin, "Online Bipartite Perfect Matching With Augmentations", 2009. https://people.csail.mit.edu/costis/matching.pdf. Introduction, related work, capacity remark read; rest scanned.
10. Efron, Patel, Stein, "A Simple Algorithm for Dynamic Carpooling with Recourse", 2024. https://arxiv.org/abs/2411.07553. Lemma 5 and proof read.
11. Kaplan, Naori, Sadeh, "Dynamic Edge Coloring of Forests", 2026. https://arxiv.org/abs/2605.09711. Scanned (adjacent problem).
12. Scanned, nothing relevant: arXiv 1802.09515, 2310.18146, 2203.06039, 2608.22524, 1704.02093, 2111.06308.

**Not opened:** Gupta, Kumar, Stein (SODA 2014; paywalled, abstract only); He, Tang, Zeh (ISAAC 2014; paywalled); Grove, Kao, Krishnan, Vitter (WADS 1995; 403), known only through items 8 and 9; Awerbuch et al., Westbrook, Andrews et al. (not attempted; item 9 describes them as load/competitive-ratio results). Cuckoo hashing with bucket size 2 under adversarial insertions: search snippets only. Forward citations: Semantic Scholar listed 7 citers of Bender et al. (three read, one scanned, three by title); the Brodal–Fagerberg listing was rate-limited and not obtained.

## Strongest prior statements on lower bounds

- Brodal–Fagerberg Theorem 4 (fully dynamic, out-degree exactly the arboricity c): an algorithm "can be forced to change the orientation of Ω(n/c²) edges, even when considering amortized complexity." Berglin–Brodal: "It is not hard to see that this bound holds even for α = 1." Needs deletions and out-degree 1.
- Marques Domingues et al. Theorem 1.4: out-degree 2 on outerplanar graphs, "even offline, the amortized recourse is Ω(n)". On forests: the two-ternary-trees example "does not, however, imply an Ω(log n) worst-case recourse lower bound for a dynamic algorithm starting from the empty graph."
- Hiken–Wein: "there is no non-trivial lower bound; it is possible that an O(1)-orientation can be maintained on constant-arboricity graphs in constant update time. Moreover, there has been no progress on closing this gap even for dynamic forests."
- Matching view, capacity 1 (out-degree 1): Bernstein et al. report that Grove et al. "showed matching upper and lower bounds of Θ(n log n) replacements for the case where each client has degree two"; Chaudhuri et al.: "any online algorithm must have switching cost Ω(n log n), even when G is restricted to be a forest."
- Bender et al.: no lower bound of any kind.
- Ours: out-degree 2, insertion-only forest, at least ⌊n/4⌋−1 flips in total, and at least 9n/32 asymptotically.

## Verdicts

1. **T(n) ≥ ⌊n/4⌋−1: NOT FOUND AFTER READING** (items 1–11). Not implied by the matching bounds: the capacity reduction in items 8 and 9 (split a server into unit copies) transfers upper bounds; the Θ(n log n) instances are capacity-1 paths with no slack, and a forest at capacity 2 has n+1 spare slots that no forest gadget can use up. In update-time terms the claim is Ω(1) per insertion, which this literature calls trivial; its content is only that total flips cannot be o(n).
2. **Exact values for n ≤ 15: NOT FOUND.** No source computes any exact game value.
3. **T(30) ≥ 7, T(64) ≥ 18, limit ≥ 9/32: NOT FOUND.**
4. **Upper bound.** The algorithm is FOUND: Efron et al. Lemma 5, "find the shortest path in H from u to a vertex w with out-degree at most 1 and flip every edge along the path", attributed to Gupta–Kumar–Stein for trees, with O(log n) worst-case recourse; also Bender et al. footnote 1 and Brodal–Fagerberg Lemmas 2–3. T(n) ≤ 3(n−1) is a COROLLARY OF Brodal–Fagerberg Lemma 1: take δ = 1, Δ = 2, r = 0 (root the final tree), giving (k+r)(Δ+1)/(Δ+1−2δ) = 3k; the proof needs only Δ+1−2δ ≥ 1 (the relation symbol in the proviso was lost in extraction). The bound n − log₂(n+1) for the shortest-path greedy: NOT FOUND, with the caveat that Gupta–Kumar–Stein was not read.
5. **Path normal form: not stated as a theorem; a standard argument.** Marques Domingues et al. use it in one instance: "By conservation of outdegree changes, the unit lost at x must be transferred along a path of flipped edges". Optimality over the whole game: NOT FOUND.

## Chance already known

- (1): about 20% that it is written down somewhere I did not reach (Gupta–Kumar–Stein, Grove et al., a thesis); about 80% that a specialist would call it an easy exercise once posed. Who would know: the authors of items 2, 3, 4 and 11.
- (3): under 5%. Nothing found treats exact constants for this game.

## Interest

Kaplan–Naori–Sadeh prove an Ω(1/Δ) amortized recourse bound for incremental forest edge colouring, so constant amortized lower bounds on incremental forests are publishable next door. Here, a proof that out-degree 2 on incremental forests cannot have o(1) amortized flips, with a constant provably above the natural 1/4, would be a modest note; the exact limit would be the interesting part.
