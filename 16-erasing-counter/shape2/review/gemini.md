<!-- Archived by the organiser on 2026-11-21. Report of the Gemini seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `shape2/PROOFS.md:15-41`, `shape2/cex.txt:2,4`, `shape2/out_cex.txt:1-12`, `shape2/root1.txt:2`, `shape2/out_root1.txt:1` — none
Q2: CONFIRMED — `shape2/PROOFS.md:62-136`, `shape/PROOFS.md:72-128` — none
Q3: CONFIRMED — `shape2/PROOFS.md:184-234`, `shape/PROOFS.md:48-69` — none
Q4: CONFIRMED — `shape2/PROOFS.md:237-314` — none
Q5: CONFIRMED — `shape2/PROOFS.md:316-350`, `shape2/out_lemma_t.txt:1` — none
Q6: CONFIRMED — `shape2/PROOFS.md:353-370`, `16-erasing-counter/README.md:64-67,153-158`, `shape2/out_nc.txt:2`, `shape2/out_tail.txt:2`, `shape2/out_lemma_t.txt:1` — none

F: LOW — `shape2/out_root1.txt:5-9` — dirty artifact: output file contains extraneous runs and an unhandled Python traceback (`AssertionError` in `root1.py:7`) from executing against `cex_mut.txt` trees after `root1.txt` — confidence: high — re-run `python3 root1.py root1.txt > shape2/out_root1.txt` to keep only the clean single-tree output

### Important Items Not Asked About

1. **Unproved candidate replacement \(\hat{L}(T)\) (`shape2/PROOFS.md:364-370`)**:
   Section 6 proposes \(\hat{L}(T) = \mu_1 \dots \mu_k + \sum_{n=1}^k \mu_1 \dots \mu_n\) (sorting subtree values \(\mu_1 \ge \dots \ge \mu_k\)) as a candidate replacement for \(L(T)\). The text is candid that it is NOT proved and that empirical support is weak (only tested on random chain machines of \(\le 12\) indices and depth \(\le 7\); `lhat_search.py` controls found nothing). It should remain explicitly marked as open/conjectural.

2. **Negative controls and mutation checks (`shape2/PROOFS.md:372-386`)**:
   All five structural properties and counting bounds are backed by mutation outputs (`out_cex_mut.txt`, `out_md_mut*.txt`, `out_nc_mut*.txt`, `out_tail_mut*.txt`, `out_lemma_t_mut*.txt`) that intentionally violate hypotheses (shared data, missing (EC) filter, inflated bounds, or illegal trees) and confirm that the checkers reject them rather than passing vacuously.

3. **Asymptotic divergence of cycle length from \(L(T)\) (`shape2/PROOFS.md:42-45`)**:
   In \(T(q, K)\), taking \(c = q \to \infty\) yields \(\text{cycle}/L(T) = (q+2)q/(6q) = (q+2)/6 \to \infty\). This demonstrates that \(L(T)\) does not merely fail by a small additive gap (like \(52 > 50\)), but can undercount the actual cycle length by an arbitrarily large multiplicative factor.
