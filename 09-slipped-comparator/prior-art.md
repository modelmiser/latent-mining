# Prior-art report: slip-proof adjacent-comparator words

The setting goes by two names: passive-fault-tolerant ("k-tolerant") sorting networks restricted to primitive (adjacent-comparator) networks; and, in Coxeter language, words Q in the 0-Hecke monoid of S_n with Demazure product w_0 such that every letter is a vertex of the subword complex Δ(Q, w_0). Deletion-proof = "no non-vertices".

## Closest prior work

1. **On Fault-Tolerant Networks for Sorting**, A. C. Yao and F. F. Yao, 1985, SIAM J. Comput. 14. https://scholar.xjtu.edu.cn/en/publications/fault-tolerant-networks-for-sorting/
   Defines the passive-fault (comparator removed) model; S(n) + k(2n-3) comparators suffice for k faults; an exact (k+1)(n-1) for minimum-finding. Differs: arbitrary comparators, upper bound only for sorting, no adjacent restriction, no slips.
2. **Subword complexes, cluster complexes, and generalized multi-associahedra**, Ceballos, Labbé, Stump, 2011. https://arxiv.org/abs/1108.1776
   Δ(c·w_0(c), w_0) is the cluster complex; the word has N + rank letters and every letter is a vertex (Lemma 5.5, as reported by the fetch). Differs: no minimality statement, no fault language.
3. **Brick polytopes of spherical subword complexes and generalized associahedra**, Pilaud, Stump, 2011. https://ar5iv.labs.arxiv.org/html/1111.3349
   Type A facets as pseudoline arrangements on sorting networks; root-independent complexes have |Q| = N + n. Differs: a count for one class, not a lower bound for all-vertex words.
4. **Multitriangulations, pseudotriangulations and primitive sorting networks**, Pilaud, Pocchiola, 2012. https://arxiv.org/abs/1009.5344v1
   Pseudoline arrangements with contacts supported by primitive sorting networks. Differs: enumeration and flips, no robustness question.
5. **Fault tolerance analysis of odd-even transposition sorting networks**, Salloum and Perrie, 1999 (and Salloum and Gala, 1997). https://www.bohrium.com/paper-details/fault-tolerance-analysis-of-odd-even-transposition-sorting-networks/812425996606111744-104696
   By simulation, single stuck-at faults in internal comparators of odd-even transposition networks are redundant. Differs: stuck-at model, standard round count, simulation only, no minimum size.
6. **Depth Optimal Sorting Networks Resistant to k Passive Faults**, Piotrów, 1996/2004. https://epubs.siam.org/doi/10.1137/S0097539799354308
   Depth Θ(log N + k) passive-fault-tolerant networks. Differs: asymptotic depth, arbitrary comparators.

## Verdicts

- **Upper bound S(n), A(n) ≤ (n-1)(n+2)/2 and the witness W_n: EASY COROLLARY** of item 2. By my own reading (not computed), W_n reversed is c·w_0(c) for c = c_1…c_{n-1}; "all letters are vertices" is exactly deletion-proofness, and reversal preserves it. The length N + n - 1 matches (rank = n-1).
- **Slip-proof ⇔ deletion-proof (T1); S(n) = A(n): NOT FOUND.** No slip/misconnected-comparator fault model found anywhere. The easy direction (inserting a comparator is harmless) is folklore monotonicity.
- **Lower bound S(n) ≥ N + n - 1 (T2 / L4 / L5), hence the headline D(n) = n-1: NOT FOUND.** No minimality statement for all-vertex words in the two subword-complex papers read; Yao–Yao's abstract gives no lower bound for sorting.
- **Up-slip variant u(n), u(3) = 4: NOT FOUND.**
- **Odd-even transposition with n+2 rounds is deletion-proof: NOT FOUND** (item 5 is nearest).
- **Sequences.** 5, 9, 14, 20, 27 is OEIS A000096, n(n+3)/2; the name list shows no sorting-network connection. 2, 24, 1912, 1210080 and 64, 3860, 2611264: looked up at oeis.org, "terms do not match anything in the table". The brute-force counts (2, 16, 58) are too short.

Queries run (about 19): fault-tolerant sorting network adjacent comparators one faulty; Yao Yao k faulty minimum size; Rudolph robust sorting network odd-even; primitive sorting network removal of one comparator N + n - 1; 0-Hecke Demazure product deleting any letter; OEIS terms (three direct lookups plus one web search); subword complex every letter a vertex c w_0(c); Schimmler Starke correction network; odd-even transposition n+2 stages; misrouted/misconnected comparator; subword complex non-vertices minimal letters; bubble/insertion network k-fault (k+1)(n-1); adjacent transpositions deletion-robust word; Piotrów passive faults; Knuth redundant comparator; spherical subword complex vertex lower bound; Salloum Perrie; 1-fault optimal size SAT; transposition networks fault-tolerant.

## Whom to ask

A Coxeter combinatorialist working on subword complexes (the lower bound may be a known remark there); second, a fault-tolerant sorting-network specialist from the Yao–Yao / correction-network line.

## Not checked

- Sources read in full: **0**. Items 2 and 3 were read through the first 100,000 characters via a summarising fetch; item 1 as an abstract page only; all others from search snippets.
- The Yao–Yao paper body (SIAM 403), Schimmler–Starke, the Leighton–Ma thesis (403) and Knuth 5.3.4 exercises were not readable; a lower bound could be in any of them.
- The identification of W_n reversed with c·w_0(c), and Lemma 5.5's wording, were not verified against the paper text directly.
