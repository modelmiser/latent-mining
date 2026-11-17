# Prior-art report: single deletion, one feedback bit at a fixed time t

Standard names for the setting: zero-error (adversarial) single-deletion-correcting codes; coding with noiseless feedback; "one-time" / "two-stage" / partial feedback; set packing in the single-deletion hypergraph.

## CLOSEST PRIOR WORK

1. **Feedback Insertion-Deletion Codes** — Maringer, Polyanskii, Vorobyev, Welter, 2020. https://arxiv.org/abs/2010.15928
   Shows: the maximal asymptotic rate of feedback codes against a fraction τn of adversarial insertions/deletions, by reduction to the substitution channel.
   Differs: full feedback after every symbol (the encoder sees the received segments), linear error budget, asymptotic rate only; no single deletion, no one-bit feedback, no finite-length sizes. It describes the feedback insertion-deletion problem as new.
2. **Correcting a single error in feedback channels** — Vorobyev, Deppe, A. Lebedev, V. Lebedev, 2022. https://www.mathnet.ru/eng/ppi2382
   Shows: optimal strategies for one-time feedback against a single error in a DMC; for the binary channel two feedbacks match complete feedback.
   Differs: substitution-type errors; deletions are not mentioned (abstract only read).
3. **Non-adaptive and two-stage coding over the Z-channel** — A. Lebedev, V. Lebedev, Vorobyev, Deppe, 2022. https://arxiv.org/abs/2202.00136
   Shows: two-stage (one feedback after n1 symbols) codes for one asymmetric error, with small-n tables.
   Differs: asymmetric substitutions, full feedback about the prefix, and the feedback position is fixed per example rather than optimised.
4. **Correcting One Error in Non-Binary Channels with Feedback** — Vorobyev, V. Lebedev, A. Lebedev, 2023. https://arxiv.org/abs/2305.06691
   Shows: q-ary single-error codes with one-time, two-time and complete feedback.
   Differs: q-ary symmetric (substitution) channel; no deletions.
5. **On Single-Deletion-Correcting Codes** — Sloane, 2002. https://arxiv.org/abs/math/0207197
   Shows: survey of VT codes and the small maximal sizes, matched to OEIS A000016.
   Differs: no feedback.
6. **The Tight Upper Bound for the Size of Single Deletion Error Correcting Codes in Dimension 11** — Nakasho, Hagiwara, Anderson, Nation, 2023. https://arxiv.org/pdf/2309.14736
   Shows: ILP proof that A(11) = 172; snippets state VT optimality was already known for n <= 10.
   Differs: no feedback; same set-packing/ILP method as the solvers used.

## VERDICTS

- **A(1..10) = 1,2,2,4,6,10,16,30,52,94**: FOUND. OEIS A000016 shifted by one (https://oeis.org/A000016, comment "size of Varshamov-Tenengolts code VT_0(n)"); optimality for n <= 10 per items 5-6 (from search snippets, not from a proof I read).
- **|D(x)| = r(x)**, the ingredient of the r(u)+r(a)-1 lemma: FOUND as classical (Levenshtein); stated in https://arxiv.org/pdf/2204.02201 per its snippet. The lemma itself (minimum over b, attained at b = u_t a'): LIKELY EASY COROLLARY of that fact; not found stated.
- **F(n,0) = F(n,n) = F(n,n-1) = A(n), F(n,t) >= A(n)**: EASY COROLLARY of the definition of single-deletion codes (the outcome set collapses to {x} ∪ D(x)).
- **F(n,1) = 2A(n-1); F(n,t) <= 2F(n-1,t-1) <= 2^t A(n-t)**: NOT FOUND; elementary prefix arguments, but the objects F(n,t) themselves were not found anywhere.
- **F(n) = (1+o(1)) 2^n/n**: NOT FOUND for this model; plausibly an EASY COROLLARY of Levenshtein's asymptotic bound plus VT codes, as solver-a says.
- **Headline NO (F(6,2) = 13 > 12), the table F(n,t) for n <= 8, F(9,3) = 67**: NOT FOUND.

Queries run (17): single-deletion codes + noiseless feedback zero-error; deletion channel one-bit/limited/partial feedback; OEIS A(n) terms + VT; OEIS "2,4,4,8,13,22,38"; adaptive deletion coding Berlekamp-style; interactive coding synchronization feedback fixed deletions; sender learns whether a deletion occurred + VT; one-time/partial feedback single error q-ary; Ulam-Rényi game with deletions; zero-error single deletion with causal encoder knowledge; deletion codes limited/bounded/delayed feedback; minimal noiseless feedback binary codes; set-packing/ILP bounds for single-deletion codes + feedback variant; deletion + one-time feedback + VT; deletion channel feedback ARQ/deletion detection; number of runs = single-deletion ball size; two-stage feedback single deletion; maximal single-deletion code sizes for lengths 9-11.

## Integer sequences (OEIS site search, done directly)

- A(n): A000016 (shifted). Column t=1 = 2A(n-1) "2,4,4,8,12,20,32,60": only A063776, an unrelated-looking subset-sum sequence — not investigated further.
- F(n) "4,4,8,13,22,38": no match. Column t=2 "2,2,4,8,13,21,37": no match. Column t=3 "2,4,8,12,22,38": no match.
- "13,22,38,67" matches A325473 and A395489 (unrelated definitions). F(5..8) = 8,13,22,38 agrees with A325473, but F(4) = 4 does not (A325473 has 5) and F(9) is only known to be >= 67; treat as coincidence.
- Columns t >= 4 have at most four or five terms; not searched.

## Who to ask

A coding theorist working on combinatorial feedback codes and insertion/deletion codes — the group of authors of items 1-4 is the exact intersection.

## What I could not check

- Sources read in full: 0. One paper (item 1) was read nearly whole via an HTML mirror through a summarising fetch tool (first 100,000 of 106,312 characters); item 3 likewise via summary; the rest are abstracts, OEIS pages and search snippets. Direct PDF fetches were unreadable.
- I could not search IEEE Xplore, MathSciNet, zbMATH or the Russian-language literature (Problemy Peredachi Informatsii), where a one-time-feedback deletion result would most likely sit.
- "Not found" here means not found by 17 web searches, not that the result is new. The model is a natural variant of the one-time-feedback line (items 2-4), so an unpublished or conference-only treatment is possible.
- No computation was run; none of the solvers' values were verified.
