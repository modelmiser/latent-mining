# Prior-art report

## Closest prior work

1. **Achieving Competitiveness in Online Problems**, M. S. Ozdayi, 2017/2020, https://arxiv.org/pdf/2001.03015 (read: section 2).
   - Shows: Lemma 2.1, "If the given edge set is acyclic and if the reorientation of the edges are not allowed, the best competitive ratio … is Θ(log n)"; halving adversary, smaller-in-degree rule, "any sequence that achieves k in-degree requires at least 2^k − 1 edges".
   - Differs: matches the no-Expire game exactly, but has no deletions.
2. **Incremental Edge Orientation in Forests**, Bender, Kopelowitz, Kuszmaul, Porat, Stein, 2021, https://arxiv.org/pdf/2107.02318 (read: introduction).
   - Shows: a warm-up algorithm that "never flips any edges", orients out of the smaller component, maximum out-degree ⌈log n⌉.
   - Differs: insertion-only; main results allow flips.
3. **Temporary Tasks Assignment Resolved**, Armon, Azar, Epstein, Regev, c. 2002, https://cims.nyu.edu/~regev/papers/unrelated_online.pdf (read: sections 1, 3).
   - Shows: Theorem 3.1, an adversary where each unit job "is admissible to two machines: Aj and Bp" forces load √m while "OFF maintains a load of one". It attributes the Ω(√m) unknown-duration bound to Azar–Broder–Karlin and the matching O(√m) "Robin-Hood" algorithm to Azar et al.
   - Differs: departures are adversary-chosen and nested, not FIFO; no forest requirement (its live jobs do form a forest, apart from one single-machine job); asymptotic only.
4. **On-line restricted assignment of temporary tasks with unknown durations**, Armon, Azar, Epstein, Regev, IPL 85 (2003), https://cris.haifa.ac.il/en/publications/on-line-restricted-assignment-of-temporary-tasks-with-unknown-dur/ (abstract only).
   - Shows: greedy is Ω(m^(2/3)) and is optimal for m ≤ 5.
   - Differs: arbitrary admissible sets and competitive ratio, not pairs on a forest. The only exact-small-value work found.
5. **A Simple Algorithm for Dynamic Carpooling with Recourse**, Efron, Patel, Stein, 2024, https://arxiv.org/pdf/2411.07553 (read: introduction, Lemma 4).
   - Shows: edges "arrive and depart online … according to an adaptive adversary"; on trees, discrepancy ≤ 3 with O(log n) recourse.
   - Differs: recourse allowed; discrepancy objective.
6. **Fairness in scheduling**, Ajtai, Aspnes, Naor, Rabani, Schulman, Waarts, 1995/1998, https://www.wisdom.weizmann.ac.il/~naor/PAPERS/carpool_abs.html (abstract only).
   - Shows: online carpool/orientation fairness.
   - Differs: discrepancy objective, no departures.

## Verdicts

- **No-Expire value floor(log2 n), N(k) ≤ 2^k:** FOUND (items 1, 2).
- **N(k) ≥ 2k:** EASY COROLLARY of the smaller-load argument in item 1; exact statement not found.
- **A(k) ≤ 1 + k(k+1)/2 (any removal):** the order is FOUND: Θ(√m) is tight for temporary tasks even with two admissible machines (item 3). The exact constant and the conjecture A(k) = 1 + k(k+1)/2 are NOT FOUND; item 3's construction uses about k² machines.
- **N(k) ≥ A(k):** trivial.
- **N(3) = 7, g(6) = 2, g(7) = 3, N(4) bounds, FIFO helping the adversary, the age-order tie-break rule:** NOT FOUND.

Note: the solver reports disagree at n = 11 (solver-a: open; solver-b: g(11) = 3).

## Sequences (OEIS JSON queries)

- No-Expire 1,1,2,2,2,2,3: A000523, floor(log2 n).
- g and g_any, 1,1,2,2,2,3,3,3,3,4,4,4: A003056 (inverse triangular), shifted. FIFO g agrees only on computed terms.
- 2,4,7,11: A000124. 2,4,7,12: A005126.
- No fetched entry mentions orientation, load balancing or bins.

## Queries run (17 web searches, plus OEIS)

Forest edge orientation; carpool; AKPPW; two machines with deletions; two-choice adversarial deletions; orientation-game exact values; ABK √n; FIFO edge expiry; orientation without recourse; departures on trees; k + 2^(k−1); triangular forcing; Ma–Plotkin; ABK construction; Robin Hood; FIFO departures; Armon et al. constant.

## Whom to ask

A specialist in online load balancing and competitive analysis (the Azar / Epstein / Regev line), or in dynamic edge orientation (Kuszmaul / Kopelowitz).

## Not checked

- ABK94, AKPPW97, Ma–Plotkin and the IPL 2003 paper were not obtained (paywalled). The √m attributions and "m ≤ 5" rest on item 3's text and abstracts.
- Whether the Robin Hood constant is consistent with 1 + k(k+1)/2.
- No source on FIFO departures was found: absence of evidence, not proof of novelty.
- Relevant sections read in 3 PDFs (items 1, 3, 5); item 2 introduction only; two others skimmed. Downloads are in `deep/`.
