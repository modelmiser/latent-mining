# Deep prior-art check: repairable universal loops, M(n)

**Bottom line:** I found no source that asks for one cyclic sequence that stays (or can be made) universal after a deletion or duplication, and none that bounds its length. Claims (1) lower bound, (2), (3), (4) were not found; (5) is mostly classical. Limit of this check: "read" below means full text extracted locally, every occurrence of deletion/insertion/sticky/duplicat/robust inspected, plus the named sections read. No paper was read line by line.

## Sources read

1. Chee, Etzion, Kiah, Vardy, Vu, Yaakobi, "Constrained de Bruijn Codes: Properties, Enumeration, Constructions, and Applications", 2020, https://arxiv.org/abs/2005.03102. Abstract, intro, Sec. II, Sec. VI (Thm 12), Sec. VII (Thms 13-16), all 60-odd edit-error mentions.
2. Marcovich, Etzion, Yaakobi, "Covering Sequences for l-tuples", 2022, https://arxiv.org/abs/2206.03711. Abstract, intro, definitions, all "insertion" hits (these are an encoder step, not errors).
3. Chee, Kiah, Vardy, Vu, Yaakobi, "Coding for Racetrack Memories", https://arxiv.org/abs/1701.06874. Intro and all theorem statements.
4. Gabric, Holub, Shallit, "Maximal State Complexity and Generalized de Bruijn Words", 2019, https://arxiv.org/abs/1903.05442. Secs. 1-2 through Corollary 1.
5. Tesler, "Multi de Bruijn Sequences", 2017, https://arxiv.org/abs/1708.03654. Abstract, intro, Sec. 2.1.
6. Berkowitz, Kopparty, "Robust positioning patterns", 2015, https://www.math.toronto.edu/swastik/robustpositioning.pdf. Intro and definitions.
7. Chee, Dao, Kiah, Ling, Wei, "Robust Positioning Patterns with Low Redundancy", https://arxiv.org/abs/1912.13301. Abstract, Sec. 2 definitions.
8. Fillmore, Goeckner, Kirsch, Martin, McGinnis, "The Existence and Structure of Universal Partial Cycles", https://arxiv.org/abs/2310.13067. Sec. 7 (run property).
9. Nellore, Ward, "Arbitrary-length analogs to de Bruijn sequences", 2021, https://arxiv.org/abs/2108.07759. Abstract, Sec. 1.
10. Keyword-skimmed only: Ye, Ge, "Deletion-Correcting Codes for the l-Symbol Read Channel", https://arxiv.org/abs/2606.26434; Chee, Etzion, Ta, Vu, "On de Bruijn Covering Sequences and Arrays", https://arxiv.org/abs/2404.13674 (covering radius, a different notion); "Maximum Length RLL Sequences in de Bruijn Graph", https://arxiv.org/abs/2403.01454; "Simple and Robust Binary Self-Location Patterns", https://arxiv.org/abs/1112.4253; Lenz, Rashtchian, Siegel, Yaakobi, "Covering Codes using Insertions or Deletions", https://arxiv.org/abs/1911.09944 (codes, not one sequence); Fleischmann et al., "The Edit Distance to k-Subsequence Universality", 2020, https://arxiv.org/abs/2007.09192 (abstract only).
11. OEIS: 4,4,10,18,34 gives no match; 10,18,34,66 gives only A052548 (2^n+2) and unrelated entries.
12. Forward citations: 29 titles citing the two versions of source 1 and 6 citing source 5. Titles only, except the first and third papers of item 10. None suggests the question.

**Could not open:** the ISIT 2019 version of source 1 (host blocks automated clients); Chee, Le, Vu, "Robust Locally Positioning Sequences and Codes" (ISIT 2022, paywalled; search summary only); citations of sources 2 and 4 (index empty or rate-limited); Golomb's book, Lempel 1971, Yoeli 1962; IEEE journal versions. Searches found no puzzle or olympiad source with a faulty keypad.

## Nearest prior statements

- Source 1, Thm 12: "The code C'DB(n, b, k) can correct any number of sticky-insertions and any number of bursts of deletions ... whose length is at most b − 2 ... in the ℓ-symbol read channel." Same two errors, but the goal is recovering a codeword from a set whose windows are locally distinct. Ours: one loop whose window set stays complete. No covering requirement, no minimum length.
- Source 2: "a sequence is an ℓ-tuples covering sequence if it contains each q-ary ℓ-tuple as a window at least once", with "for the rest of this paper we refer only to acyclic sequences". No errors anywhere.
- Source 5: "a cyclic or linear sequence that contains every k-mer ... exactly m times". The doubled de Bruijn cycle is an m = 2 instance. Enumeration only.
- Sources 6, 7: windows "far apart from each other in Hamming distance". Substitutions inside a window, not edits to the sequence.
- Source 10, Fleischmann et al.: "the minimal number of edit operations (insertion, deletion, substitution) one needs to apply to a given word in order to reach the set of k-subsequence universal words". The closest in spirit (edits against universality), but for scattered subsequences, and it is an algorithmic question about a given word.

## Verdicts

1. **2^n + 2 ≤ M(n) ≤ 2^(n+1).** Lower bound: NOT FOUND. Upper bound: COROLLARY OF the definition (source 5 covers the object): in a de Bruijn cycle written twice each n-word occurs at two places 2^n apart, one edit disturbs windows near one place only, so the mechanic passes. Not found stated.
2. **M(n) = 2^n + 2 for n = 3, 4, 5.** NOT FOUND; no OEIS match.
3. **Length 2^n + 2 fails for n ≥ 6.** NOT FOUND.
4. **Unbounded excess, about 2^n/(152n).** NOT FOUND. Nothing of this shape appears in any source opened.
5. **Auxiliary facts.**
   - Every k-word at least 2^(n−k) times: classical one-liner. Source 4: "every word of length i occurs as a prefix of some word of length n, and every word of length n is guaranteed to appear in w". Source 9 studies the exact-balance version (floor or ceiling of L/K^m occurrences).
   - Run counts at length 2^n: CLASSICAL. Source 8: "Run (R-2) For every r ∈ [n] and every letter ℓ ∈ A, the word ℓ^r appears exactly a^(n−r) times in w", with "All De Bruijn cycles are known to satisfy the R-1 and R-2 properties", attributed to Golomb. Counts of maximal runs and singletons follow by applying the same count to 010 and 101.
   - Run and singleton identities at length 2^n + e: NOT FOUND; short corollaries of the previous two items.
   - Eulerian circuit plus balanced extra edges: standard. Source 5 states the m-fold case ("Any Eulerian cycle in this graph gives a cyclic sequence in C(m, q, k)"); source 4 treats circuits of length N "that visits every vertex", a different condition (distinct (n+1)-factors). The general statement was not found verbatim.

## Are (3) and (4) already known?

My estimate is 5-10% that they exist in print. The question itself seems not to have been posed; the coding literature wants unique decodability, not preserved coverage. This rests on searches plus partial reads, with no paywalled literature. The people who would know are the authors of sources 1 and 2 (Etzion, Yaakobi, Kiah, Vu) and, on the words side, Shallit.

## Would it interest them?

Mildly. It is a clean extremal question in a model that community already uses, and the jump from "+2" to an excess of order 2^n/n is a real finding. But it has no application yet and no upper bound below 2^(n+1), so it reads as a short note or open problem, not a major result.
