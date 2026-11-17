# Prior-art search: repairable universal loops, M(n)

Standard names: de Bruijn sequences / universal cycles, covering sequences, and synchronization-error coding (deletion + sticky insertion = the racetrack-memory shift-error model). I found no name for the game itself.

## CLOSEST PRIOR WORK

1. **Constrained de Bruijn Codes: Properties, Enumeration, Constructions, and Applications** — Chee, Etzion, Kiah, Vardy, Vu, Yaakobi, 2020. https://arxiv.org/abs/2005.03102 (ISIT 2019 version: https://yaakobi.net.technion.ac.il/files/2020/07/91_Bounded_De_Bruijn_ISIT19.pdf). Shows that sequences whose length-h windows are distinct within any b consecutive positions give codes correcting deletions and sticky insertions. Differs: same error model, but the goal is decoding a codeword, not keeping one cycle universal; no minimum-length question.
2. **Covering Sequences for l-Tuples** — Marcovich, Etzion, Yaakobi, 2022. https://arxiv.org/abs/2206.03711. Studies sequences containing every l-tuple at least once (non-minimal universal words): counts, rates, encoders. Differs: no edits, no robustness.
3. **Maximal State Complexity and Generalized de Bruijn Words** — Gabric, Holub, Shallit, 2019. https://arxiv.org/abs/1903.05442. Shows circular words with the maximum number of distinct factors exist at every length. Differs: static objects; no edits.
4. **Multi de Bruijn Sequences** — Tesler, 2017. https://arxiv.org/abs/1708.03654. Counts cyclic sequences containing every k-mer exactly m times. Differs: the doubled de Bruijn cycle of the upper bound is an m=2 instance, but robustness to edits is not discussed (abstract only).
5. **Robust positioning patterns** — Berkowitz, Kopparty. https://www.Math.Toronto.Edu/swastik/robustpositioning.pdf. Sequences whose windows identify position despite read errors. Differs: substitution errors in the window, not edits to the sequence (search snippet only; year not seen).
6. **The Existence and Structure of Universal Partial Cycles** — https://arxiv.org/pdf/2310.13067 (authors not seen). States the run property of de Bruijn cycles (a^(n-r) runs of length r per letter, non-maximal convention). Differs: the classical fact behind the run-count lemmas, at length 2^n only.

## VERDICTS

- **M(n) as defined; M(3)=10, M(4)=18, M(5)=34; M(n) >= 2^n+2 for n >= 3; the M(6) bounds; "shortest repairable loops are universal"; solver-b's Theorem 3 and Lemma 5:** NOT FOUND.
- **2^n <= M(n) <= 2^(n+1):** EASY COROLLARY of the definition of a de Bruijn cycle; not found stated.
- **Run counts (2^(n-1) runs, 2^(n-2) singletons in a de Bruijn cycle):** LIKELY FOLKLORE at length 2^n (Golomb-type run distribution; item 6 and a snippet from https://arxiv.org/abs/2404.16313). The extensions to length 2^n+1 and the bound 2^(n-2) <= S <= 2^(n-2)+l-2^n were NOT FOUND; they look like the same counting of occurrences of short factors (my inference, not a source).
- **The reversibility lemma (only deleting a length-1 run is irreversible):** NOT FOUND; elementary.

Queries run (17, paraphrased): de Bruijn robust to single deletion/insertion; de Bruijn + sticky insertion/duplication; shortest cyclic word universal after one deletion; robust positioning + racetrack; run counts in universal cycles; OEIS 4,4,10,18,34; generalized de Bruijn words; fault-tolerant covering sequence 2^n+2; multi de Bruijn; Golomb run property; bit-slip-tolerant position encoding; tandem duplication + de Bruijn; deletion-robust universal word; de Bruijn adversary/repair game; run-length encoding of de Bruijn sequences; error-tolerant de Bruijn; cycles of length 2^n+1, 2^n+2.

## INTEGER SEQUENCES (OEIS, searched directly)

- 4, 4, 10, 18, 34 (M(n)): no match.
- 12, 24, 62, 308, 1172 (n=4 necklace counts): no match.
- 216, 456, 1240, 6468, 25784 (n=4 string counts): no match.
- 30, 110, 300, 910 (n=3 string counts): no match.
- Shorter sequences not searched.

## WHO TO ASK

A combinatorics-on-words specialist in de Bruijn sequences and universal cycles, or a coding theorist working on constrained de Bruijn codes.

## WHAT I COULD NOT CHECK

- Sources read in full: **zero**. I read five arXiv abstract pages and four OEIS result pages; everything else is search snippets. No paper body was opened, so a remark inside one on edit-robust single cycles cannot be excluded.
- No paywalled literature (IEEE, older Golomb/Fredricksen work), no citation-graph walk.
- "NOT FOUND" means not found by these queries, not novel.
- The reports disagree on n=6 (67 <= M(6) <= 78 vs 66 <= M(6) <= 76); not examined.
