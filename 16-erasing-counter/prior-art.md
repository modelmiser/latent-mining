# Prior-art search: destructive single-write incrementer D(r)

Standard names: **decision assignment tree (DAT)**, **bit-probe counters**, **quasi-Gray codes**. The setting is a DAT of read depth r with a non-standard write rule.

## CLOSEST PRIOR WORK

1. **Observations on the Complexity of Generating Quasi-Gray Codes**, Fredman, 1978. https://typeset.io/papers/observations-on-the-complexity-of-generating-quasi-gray-4v9zyu8t3z — Introduces the DAT model; per items 2 and 3, gives length 2^(cn) with O(log n) reads and an Omega(log d) read lower bound for full-length Gray codes. Differs: non-destructive reads.
2. **Optimal Quasi-Gray Codes: The Alphabet Matters**, Chakraborty, Das, Koucký, Saurabh, 2017. https://arxiv.org/abs/1712.01834 — Binary codes of length >= 2^n - 20n with read 6 + log n, write 2; its Table 1 collects earlier trade-offs. Differs: non-destructive; no exact extremal value at fixed read depth.
3. **Improved Methods For Generating Quasi-Gray Codes**, Jansens, Bose, Carmi, Maheshwari, Morin, Smid, 2010. https://arxiv.org/abs/1010.0905 — A "composite code" in which one counter steps a second when it wraps. Differs: the same cascade idea as the solvers' construction, but with power-of-two periods and non-destructive reads; every write-1 entry in its table reads d bits in the worst case.
4. **Integer Representation and Counting in the Bit Probe Model**, Rahman, Munro, 2007/2010. https://unpaywall.org/10.1007%2FS00453-008-9247-2 — Length 2^(n-1) with log n + 4 reads, 4 writes (per item 2's table). Differs: non-destructive, 4 writes.
5. **Integer Representations towards Efficient Counting in the Bit Probe Model**, Brodal, Greve, Pandey, Rao, 2011. https://tildeweb.au.dk/au121/papers/jda14.pdf — Space-optimal counter with n - 1 reads, 3 writes. Differs: non-destructive.
6. **A linear lower bound for incrementing a space-optimal integer representation in the bit-probe model**, Raskin, 2016. https://arxiv.org/abs/1607.00242 — A full-length (2^n) counter must read linearly many bits. Differs: says nothing on maximum length at depth r.

## VERDICTS

- **The model (destructive reads, one write-back): NOT FOUND** outside hardware patents.
- **log2 D(r) not polynomial in r: NOT FOUND for this model.** The non-destructive analogue (length 2^(Theta(d)) at about log d reads) is established (items 1, 2, 4), so it is unsurprising, but not a corollary: those constructions do not erase what they read.
- **D(r) >= a_r, a_r = a_(r-1)(a_(r-1)+1): NOT FOUND.** The cascade technique is standard (item 3); the periods a, a+1 and the Sylvester link were not found. OEIS A007018 (https://oeis.org/A007018) has no counter, Gray-code or cycle-length comment.
- **D(2) = 6, D(3) = 42, D(4) = 1806; conjecture D(r) = a_r: NOT FOUND.**
- **Weight lemma, D(r) <= 2^d - 1, B(r,m) composition rule, "at most one empty leaf": NOT FOUND** (model-specific).

## SEQUENCES (OEIS)

- 2, 6, 42, 1806, 3263442: A007018, as reported.
- 8, 12, 20, 30, 42 (B(3,m)): one hit, A300414 = 1, 2, 4, 8, 12, 20, 30, 42, 62, ... The reports' composition rule, applied by hand, gives B(3,0..2) = 1, 2, 4, so the first eight terms agree. Probably coincidence: the r = 4 row does not match.
- 42, 36, 26, 16, 11, 7, 3, 1; 1806, 1764, 1272, 900, 610, 400, 262, 172; 40, 72, 108, 180, 270, 420: no match.
- 2, 4, 6: too short.

## WHO TO ASK

A bit-probe / data-structure lower-bounds specialist (quasi-Gray code literature).

## LIMITS

- Sources read in full: **0**. Read in part: item 3 (results table, previous-work and composite-code sections), item 2 (introduction and table, via a summarising fetch), abstracts of item 6 and arXiv 2602.11791, the A007018 page. Items 1, 4, 5 judged from snippets and other papers' summaries.
- Fredman's 1978 paper, said to contain a read/write trade-off theorem, was not read; it is the likeliest place for an overlooked overlap.
- Not checked: pre-1978 switching-theory literature on core-memory counters.

Queries (19 web, 6 OEIS lookups), by topic: quasi-Gray code / DAT read-write trade-offs and length bounds (7); bit-probe counters (3); destructive read, read-and-reset, fetch-and-clear, one-time memory counters (5); Sylvester terms with counters, Gray codes, coprime periods (3); longest cycle of a depth-r decision-tree map (1).
