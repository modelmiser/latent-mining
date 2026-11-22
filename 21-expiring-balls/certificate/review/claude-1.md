<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1: certificate for 11 bins (experiment 21)

Paths are relative to `21-expiring-balls/`. I traced PROOF.md and check.cpp line by line and wrote my own
small test of the theorem (not of check.cpp): for n = 4, 5, 6 with L = 2, 3, about 7,500 random and
perturbed sets C, "three families pass" never held without the literal properties 1 to 4 holding
(n = 6, L = 3: 692 accepted of 1,500; no counterexample). I ran nothing from the repository.

## Verdicts

- **Q1 CONFIRMED.** PROOF.md:13-17. First-appearance labelling of the first j balls depends only on those
  balls, so a prefix of a canonical state is canonical (check.cpp:50-57; the key prefix is the low 8j bits,
  check.cpp:74). Induction: start in S (property 1), Expire stays in S (3), an offer has an answer in S (4),
  S has loads at most L-1 (2).
- **Q2 CONFIRMED.** PROOF.md:41-44.
- **Q3 CONFIRMED.** PROOF.md:66-70. q+u and q+v differ only in the orientation of the newest ball, so the
  flipped c1 is the length-j prefix of q+v; it is non-final because q+v is in A, and c2 (length at most j)
  is a prefix of it. j = 1: the state before is empty, never in A, so the checker rejects
  (check.cpp:104-105, loop at :74 is empty). Equal lengths: flipped c1 is c2 itself.
- **Q4 CONFIRMED.** PROOF.md:60-64, 72-83. F2b: the chain child is produced by `older` in canonical form,
  is not in C (c is longest), bin 1 is below L-1 before f, and P2 cannot cut (f has at most n-1 balls).
  T at f gives a prefix of q. F3: seed test precedes pruning (check.cpp:111-113). Exhaustive and exclusive:
  an answer that makes load L gives a final state, which is not in A.
- **Q5 CONFIRMED.** PROOF.md:98-105 against check.cpp:113-121, 129. All pair types (two untouched, one
  untouched, two touched in different components) are in `need`.
- **Q6 CONFIRMED.** Empty state: check.cpp:194; load: :199; forest, bins, canonical: :197-200. T/T+:
  :98-106. `older`: :83-92, all (m+1)^2 choices. Membership by every prefix: :72-76. I found no place where
  code and proof differ in substance. The first textual difference: PROOF.md:108 cites `REPORT.md`, which
  does not exist.
- **Q7 UNSUPPORTED in part.** The count test (out/count_check.txt, n = 6 to 9) and the difftest verdicts
  are meaningful. But `brute` shares `analyse`, `canon`, `keyOf`, `inA` and `older` with `min` and `full`,
  so agreement does not test those; the only independent literal checker, `explicit.py`, has no output in
  `out/`. I found no way to obtain a wrong ACCEPT other than through the trusted parts (wrong n or L on the
  command line, not all parts run).
- **Q8 UNSUPPORTED in part.** Supported: 3,334,554 lines, 40 s, 974,272,116, 3,183,950
  (out/check_min_11_4.log, check_full_11_4.log, b2_11_4.log, vb_11_4.log); 218 lines; 3,722,933 and
  3,723,730. Not supported by any file: "identical", the sha256, "byte for byte" for 11 bins (no minimise
  or comparison log). I reduced the small dumps myself: b2 and vb give the same lists for (6,3), (8,4),
  (9,4), equal to `cert/`. "The organiser recompiled..." is worded as the organiser's own statement.
  "Costs one bin" is justified as far as the page's own rows go, but the parenthesis misdescribes the 11.

## Findings

1. **MODERATE, confidence high.** out/difftest.log:50-79 holds 30 lines ending `MISMATCH` (a count
   comparison that failed, apparently on format) inside a log cited as evidence. Replace the log with a
   clean rerun, or add: "lines 50-79 are a first comparison script that compared differently formatted
   lines; the valid comparison is count_check.txt".
2. **MODERATE, high.** README.md:104-106 and row :47: no log records the 11-bin comparison. Add the
   minimise outputs and the `sha256sum` of both files to `out/`, or write "identical according to the
   follow-up instance (comparison not logged here); the small cases can be recomputed from `out/`".
3. **MODERATE, high.** PROOF.md:124-126 says `explicit.py` agrees on damaged certificates; `difftest.py`
   runs only check.cpp modes (difftest.py:44) and no explicit.py output exists. Replacement: "(b) `brute`
   mode checks properties 1-4 literally and agrees with `min` and `full` on 1,560 certificates for
   n = 4 to 8; it shares the state code with them. `explicit.py`, which shares nothing, was run on
   [cases], logs in out/" and add those logs, or delete the mention.
4. **LOW, high.** PROOF.md:107-108: `REPORT.md` is absent, and no pruned small-case log exists to compare
   with the NOPRUNE lines (difftest.log:80-84). Point to real logs.
5. **LOW, high.** README.md:113-115 "rejects ... damaged 11-bin certificates": out/damage_11.log has one
   ACCEPT (added line `01 02 03 45 46 47 58`, correctly, since it extends a listed seed). Write "rejects
   every damaged 11-bin certificate tried except the addition of a redundant line, which it rightly accepts".
6. **LOW, high.** README.md:116-117 "tested against a literal check ... up to 9 bins": at 9 bins only the
   intact certificate (out/check_brute_9_4.log); damaged ones stop at 8 bins (60 cases). Say so. Bin names
   `a`, `b` occur only from 11 bins on and have no literal test; the full-mode count is the check there.
7. **LOW, medium.** README.md:91-93: by rows :53 and :143-144, 11 bins suffice by a hand proof and 10 do
   not by a certificate the referee checked. Write "the 11 on a hand proof and, for 10 bins not sufficing,
   a certificate checked by the referee".
8. **LOW, medium.** README.md:117-119 names one pruning; `min` also uses P2 (covered only by the proof).
   Write "two pruning bounds proved in the same file, one shared with the two programs".
9. **LOW, medium.** README.md:157-177 (Review) predates the certificate; "the certificate checker" at :162
   reads as the new one. Add "this section predates `certificate/`".

No CRITICAL finding.
