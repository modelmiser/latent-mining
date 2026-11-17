**Basis:** 24 web searches plus fetches. I read only one paper in full text (Cohen–Petrank PLDI'13, pp. 1–3). Everything else is from abstracts or search snippets. Robson's 1971, 1974 and 1977 papers, Luby–Naor–Orda, and Bendersky–Petrank were paywalled or blocked (403/WAF).

## CLOSEST PRIOR WORK

1. **Robson, "An estimate of the store size necessary for dynamic storage allocation", JACM 1971** — https://projects.csail.mit.edu/jacm/References/robson1971:416.html
   - Shows: the exact store for sizes {1,2} only, plus a logarithmic lower bound for general n (per snippet summaries). An unofficial, self-marked-unverified TAOCP 2.5 ex. 38 mirror gives N(n,2) = ⌊(3n−1)/2⌋ (https://brain-taocp.pages.dev/vol1/2.5/38/).
   - Differs: sizes {1,2}, not {1,2,3}; no restricted-free or move variants.
2. **Robson, "Bounds for some functions concerning dynamic storage allocation", JACM 1974** — https://dl.acm.org/doi/10.1145/321832.321846
   - Shows: a tight bound M(½·log₂n + 1) − n + 1 for power-of-two sizes, as quoted in Cohen–Petrank §2.2, which I read.
   - Differs: powers of two only; {1,2,3} is not covered exactly.
3. **Cohen, Petrank, "Limitations of partial compaction: towards practical bounds", PLDI 2013** — https://csaws.cs.technion.ac.il/~erez/Papers/compaction-pldi13.pdf
   - Shows: lower and upper bounds for c-partial managers, which may move at most s/c words in total after s words allocated (Bendersky–Petrank's model, POPL'11/TOPLAS'12, https://dl.acm.org/doi/10.1145/2362389.2362392).
   - Differs: the budget is earned on allocation, is cumulative (bankable), and the results are asymptotic in n. F is earned on free, is not bankable, and is strictly less than the freed size.
4. **Farach-Colton, Kuszmaul, Sheffield, Westover, "A nearly quadratic improvement for memory reallocation", SPAA 2024** — https://arxiv.org/abs/2405.12152
   - Shows: update cost defined as (size moved)/(size of the item inserted or deleted), with bounds as a function of load factor 1−ε.
   - Differs: this is the nearest cost normalisation to F, but the cost applies to inserts too, is expected or amortised, and is ≫1. It never studies the cost <1, delete-only, worst-case regime.
5. **Berndt, Jansen, Klein, "Fully dynamic bin packing revisited", APPROX 2015** — https://arxiv.org/abs/1411.0960
   - Shows: migration factor (repacked size / size of the arriving or departing item) of poly(1/ε) for ratio 1+ε, with an Ω(1/ε) lower bound.
   - Differs: bins not contiguous addresses; factor ≫1; applies on arrivals too.
6. **Bender, Conway, Farach-Colton, Komlós, Kuszmaul, Wein, "Tight bounds for memory allocation with and without request fragmentation", arXiv Aug 2026** — https://arxiv.org/abs/2608.28462
   - Shows: asymptotic Θ(log M) for the classical problem and Θ(log log M) with request splitting.
   - Differs: asymptotic only; no small fixed size sets.

## VERDICTS

- **Claim 1 (exact table, M = 3..13): NOT FOUND.** No exact small-M values for {1,2,3} surfaced in any variant.
- **Claim 2 (h^O = 5M/3 + O(1)): NOT FOUND as a statement; LIKELY EASY COROLLARY in technique.** The lower bound is the direct analogue of the {1,2} argument behind Robson 1971 / Knuth ex. 36–38: keep 1s on one residue class, then allocate the large size, giving M + M/2 there and M + 2M/3 here. That analogy is my inference, not a cited result. The "only smallest blocks freeable" model itself was not found anywhere.
- **Claim 3 (7M/4 ≤ h^N ≤ 43M/18): NOT FOUND.** Neither constant appeared.
- **Claim 4 (F model and 5/3 conjecture): NOT FOUND.** No model with a per-deletion, sub-deleted-size, non-bankable move right turned up. Items 3–5 are the nearest neighbours and all differ materially.

## NO-MOVE WORST CASE FOR {1,2,3}

Nothing I saw states it. What is visible:
- {1,2} is exact at about 3M/2.
- Powers of two are exact; {1,2,4} gives 2M − 3 from the formula above.
- For non-power-of-two sizes, Cohen–Petrank say only to round up to powers of two and double the upper bound.

So the visible literature implies h^N ≥ 3M/2 − O(1) by monotonicity and only a generic upper bound. Robson 1971 may contain sharper general-n bounds; I could not read it.

## WHO TO ASK

- An online-algorithms / memory-allocation theorist: William Kuszmaul, Michael Bender, Martín Farach-Colton.
- For the partial-compaction angle: Erez Petrank.
- For the classical side: J. M. Robson himself, or someone who knows TAOCP §2.5 ex. 36–41 answers well.

## NOT CHECKED

- Full text of Robson 1971, 1974 and 1977 (Computer Journal). This is the biggest gap: 1971 may bound general n, including n = 3.
- Knuth's printed ex. 38–41 answers.
- Luby–Naor–Orda beyond the abstract's first line.
- Bendersky–Petrank's own text.
- Naor–Teague and Kuszmaul FOCS'23 (titles only).
- Woodall's "bay restaurant" paper (search returned nothing on it).
- "Insertion and compaction algorithms in sequentially allocated storage" (https://epubs.siam.org/doi/10.1137/0213037), which a snippet says lists generalising block sizes as open.
- "Upper bounds for dynamic memory allocation", IEEE TC 2010 (blocked).

Web search will not surface theses, older Computer Journal / BIT / Acta Informatica papers, or computer-search folklore.

## QUERY THEMES RUN

- Robson 1971/1974/1977 bounds and N(M,n) for n = 3
- TAOCP 2.5 ex. 38–41
- {1,2,3} exact or open
- Luby–Naor–Orda
- Bendersky–Petrank and Cohen–Petrank
- move on deallocation / per-free budget
- memory reallocation (Kuszmaul)
- cost-oblivious reallocation
- history-independent allocation
- migration factor / fully dynamic bin packing
- strip packing with migration
- online interval colouring with bandwidth
- permanent large blocks / only small blocks freed / "5/3"
- dynamic bin packing without repacking
- Woodall's bay restaurant
- Wilson's survey
