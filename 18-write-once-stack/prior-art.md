# Prior-art search: write-once stack-top schemes, R(n)

No paper found on this setting: a rewriting code for write-once memory (WOM) whose data evolves as a stack and whose decoder reads only the top. The nearest framework is "generalized rewriting" over a data graph.

## CLOSEST PRIOR WORK

1. **Universal Rewriting in Constrained Memories**, Jiang, Langberg, Schwartz, Bruck, ISIT 2009. https://people.engr.tamu.edu/ajiang/universalWrite.pdf
   - Shows: defines rewriting over an arbitrary data graph D with worst-case rewrite count t(C); proves t(C) <= n(q-1) and Theta(nq) for bounded out-degree D.
   - Differs: D is finite and strongly connected, the decoder must return the whole data value, every rewrite changes that value, and results hold only up to constant factors.
2. **Trajectory Codes for Flash Memory**, same authors, arXiv 1012.5430 (IEEE Trans. IT 2013). https://ar5iv.labs.arxiv.org/html/1012.5430
   - Shows: journal version; same model, more upper bounds, all asymptotic.
   - Differs: no stack, pop, undo, infinite graph or partial decoding; no exact result for binary cells.
3. **Buffer Coding for Asymmetric Multi-Level Memory**, Bohossian, Jiang, Bruck, 2007. https://authors.library.caltech.edu/26113
   - Shows: codes remembering the last r bits of a stream; for r = 2 the abstract gives t = (q-1)(n-2)+1, "strictly optimal" at q = 2, which is n-1.
   - Differs: a queue (sliding window) with no pop, and the decoder outputs all r bits. The matching n-1 is suggestive; I found no reduction either way.
4. **How to reuse a "write-once" memory**, Rivest, Shamir, 1982. https://people.csail.mit.edu/rivest/pubs/RS82c.bib.txt
   - Shows: the original WOM codes (complete data graph).
   - Differs: any value may follow any value; no history to return to.

## VERDICTS

- **Headline (R(n) = n-1 for all n >= 2; open for n >= 9):** NOT FOUND, as a question or as a result.
- **Upper bound R(n) <= n-1 (n >= 2), including the Case B repair:** NOT FOUND as stated. It is in the spirit of the trivial counting bound t(C) <= n(q-1) (item 1, Section III-D), but that bound gives n, not n-1, and assumes every rewrite changes the decoded value, which fails here.
- **floor(n/2) pair scheme:** NOT FOUND for stacks. It resembles the "record each step in a fresh register" idea of trajectory codes (item 1) but is not a corollary.
- **floor(n/2)+2 for n >= 6; false "every non-last operation changes the configuration" claim and its corrected condition; two readings coincide for t >= 3; M(w, r) = r+1 for r = 3..6:** NOT FOUND.
- **Sequences:**
  - R(n) = 1,1,2,3,4,5,6,7 and 0,1,...,7: I queried OEIS; 10 hits, all generic (e.g. A028310, A054055). Too generic to carry evidence.
  - Stack counts 31, 63, 127, 255 are 2^k - 1. No other sequence has four terms.

Queries run (16), by topic: generalized WOM with DAG transitions; WOM stack push/pop; floating and buffer codes; WOM with undo; sequential objects on write-once bits; tree labeling in the Boolean lattice; stack/undo data graphs; automata on write-once bits; buffer coding of last r symbols; write-once data structures; write-once up-down counters; exact small-n WOM by SAT; generalized write-once memories 1984; WOM rollback/LIFO; monotone encodings of operation trees; write-once abstract data types.

## WHO TO ASK

A coding theorist working on rewriting codes for flash and write-once memory (WOM, floating, buffer and trajectory codes).

## WHAT I COULD NOT CHECK

- **Sources read in full: none.** I read the main body (Sections I-III) of item 1 directly. Item 2 I know only through an automated page summary. Items 3-4 are abstract or snippet only; item 3's PDF text extraction was garbled, so its q = 2 optimality claim is from the abstract alone.
- Seen but not listed: Aspnes, Censor-Hillel, Yaakobi, "Concurrent use of write-once memory" (https://cs.yale.edu/homes/aspnes/papers/concurrent-wom-abstract.html), abstract only; no stack result there.
- Not obtained: Fiat-Shamir "Generalized write-once memories" (1984), Fu-Vinck (1999).
- Not searched: papers citing the trajectory or buffer code papers; IEEE Xplore.
- A negative web search is weak evidence.
