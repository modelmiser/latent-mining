# Prior art: counting-blind firing squad

Standard names: firing squad synchronization problem (FSSP), state lower bounds, partial solutions, symmetric solutions. I found no paper using the model here (own state plus count of differing neighbours, no border marker).

## CLOSEST PRIOR WORK

1. **Two barriers for minimal-time firing squads, with applications to four and five states** — K. K. Giam, 2026 (arXiv v1, unrefereed). https://arxiv.org/abs/2611.11080. Defines the same kind of quantity: "a largest Nk ... such that some k-state rule synchronizes the lines of lengths 2, ..., Nk in minimal time"; "Exhaustive searches give N4 = 8 (Yunès [10]; Theorem 6.2 (a))", "N5 ≥ 14". Differs: rules map triples (x, y, z) with a visible border ∗ and must fire at exactly 2n − 2; the model does NOT match, so L(s) is not N_s.
2. **An Overview of Recent Solutions to and Lower Bounds for the Firing Synchronization Problem** — Correa, Gustavo, Lemos, Settle, 2017. https://arxiv.org/abs/1701.01045. Survey; reports Yunès 2008: "it is impossible to synchronize any line of length n ≥ 5 with only 3 states". Differs: ordinary three-cell neighbourhood.
3. **A family of smallest symmetrical four-state FSSP protocols for ring arrays** — Umeo, Kamikawa, Yunès, 2009; read only through item 2, §3.4.4. Computer search of symmetric 4-state tables, n = 2..32; 17 solutions for rings of length 2^k. Differs: rings; "symmetric" hides left/right but not neighbour states.
4. **Massively parallel search for transition-tables of polyautomata** — Sanders, 1994. https://ae.iti.kit.edu/documents/people/sanders/papers/parcella94.pdf. Exhaustive search: no 4-state minimal-time solution. Differs: full neighbourhood, border state "#", minimal time.
5. **Essential metrics for Life on graphs** — Rollier et al., 2025. https://arxiv.org/abs/2506.21226. Count-based (outer-totalistic) binary automata on a task it calls FSSP. Differs: random start, target is global alternation; unrelated.

## VERDICTS

- **L(5) = 5; L(3..7) = 2, 3, 5, 8, 11; counts 36, 232, 1244, 336; histograms; witnesses; minimum single-length times** — NOT FOUND.
- **Three states synchronise only n = 2, 3** — for n ≥ 5 an EASY COROLLARY of Yunès 2008 as quoted in item 2, since a counting-blind rule is a restriction of an ordinary radius-1 rule. n = 4 and L(3) = 2: NOT FOUND.
- **Firing time ≥ n − 1; d(Q,1) ∉ {Q, F}** — LIKELY FOLKLORE (signal-speed argument, as in item 1's introduction).
- **Lemma 2 (2n − 1, or t(n) = t(m))** — EASY COROLLARY in method of the classical 2n − 2 argument; this statement NOT FOUND.
- **Is L(s) infinite for some s** — NOT FOUND. Symmetric full solutions exist (Szwerinski, 10 states; Umeo–Maeda–Hongyo, 6 states, per item 2), but those cells see neighbour states; nothing found that transfers.

Queries (20), all on FSSP: totalistic few states; 4-state partial solutions exhaustive search; symmetric left/right-blind; Sanders 4-state search; count of differing neighbours; Yunès 4-state; outer-totalistic; 1-bit communication; 5-state enumeration longest line; 3-state impossibility; no sense of direction; Umeo–Kamikawa–Yunès rings; Szwerinski (two); beeping/stone-age path; Settle–Simon; Berthiaume ring; 2^k − 1 rings; own-state-plus-count brute force; Umeo–Maeda–Hongyo.

## OEIS (first 10 hits each)

- 2, 3, 5, 8, 11 and 1, 3, 5, 6, 7: unrelated hits only; too short to discriminate.
- 1, 3, 4, 6, 7, 9, 10 (five-state minimum times, n = 2..8): equals A032766, numbers ≡ 0 or 1 mod 3, i.e. ⌊3(n − 1)/2⌋. A seven-term coincidence, not prior art; a conjecture worth testing.
- 36, 232, 1244(, 336) and both histograms: no results.

## WHO TO ASK

A cellular-automata specialist in FSSP state complexity: J.-B. Yunès, H. Umeo or N. Kamikawa (computer-searched partial solutions), or A. Settle (lower bounds).

## NOT CHECKED

- Sources read in full: 0. Relevant sections read from downloaded text of items 1, 2, 4, 5; item 3 second-hand.
- HAL refused automated access: Yunès's 2008 papers and thesis (source of N4 = 8) unread, so the 3-state result is second-hand.
- Szwerinski 1982 and Umeo–Maeda–Hongyo 2006 not obtained; their symmetry definitions are unverified.