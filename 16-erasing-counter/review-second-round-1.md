<!-- Archived by the organiser on 2026-11-21. Report of a second-round reviewer (one Claude instance) on this experiment's page, taken from the session transcript. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

Second-round review, experiment 16 (`16-erasing-counter/README.md`). No CRITICAL defect. One MODERATE, three LOW.

Besides the folder, I read the panel records in `(staging)/panel/`: both Claude seat reports, `dispositions.md` and `panel-r1.log`. I did not open the three cursor seat reports.

## Defects

**1. MODERATE — README.md:225-229 (Review, last paragraph).**
"Both Claude seats also wrote their own programs: over 8,000 and 18,500 random trees … they found no cycle with two empty steps, no violation of the product bound for disjoint subtrees, and no cycle longer than L"
- Only seat 2 (8,000 trees) reports "no cycle longer than L(T)" (`claude-seat2.md:3`).
- Seat 1 (18,500 trees) did not test the L bound on its random trees. It reports Theorem E, the construction of F, Theorem RD on 1,500 disjoint trees, and Q(T) on 264 read-once shapes (`claude-seat1.md:5-10`).
- This matters because "length ≤ L" for trees that reuse positions is the open conjecture, and the sentence doubles its independent evidence.
- Replacement: "…they found no cycle with two empty steps and no violation of the product bound for disjoint subtrees; the seat with 8,000 trees also found no cycle longer than L; the other built the auxiliary tree from the proof's text for 1,934 cycles and found it to behave as claimed each time."

**2. LOW — README.md:60 (Theorem E row).**
"two of them tested the theorem and the construction with their own programs"
- This contradicts the Review paragraph ("one built the auxiliary tree") and the seat reports: only seat 1 tested the construction.
- Replacement: "two of them tested the theorem with their own programs, and one of those the construction of the auxiliary tree".

**3. LOW — README.md:141.**
"The upper-bound results are one day old"
- They are dated 21 November, the same day as the page, and the phrase goes stale either way.
- Replacement: "The upper-bound results date from 21 November and have one author."

**4. LOW — top-level README.md:47 (row 16).**
"r = 4 by one design of search run twice"
- The follow-up's `lazy2.cpp` is a third run of the same design (`upper/out_lazy2_r4.txt`, same node count 1,890,054,694 as `verifier/lazy_r4.out`).
- Replacement: "r = 4 by one design of search (three programs)".
- Optional: page row 53 names only solver B and the referee, and the "What to distrust" bullet already covers the third program.

## Clean sections

- **Short answer:** clean. The factor 2^r/(r + 1) and the bound 2^(2^(r−1)) are correct.
- **Results rows, Theorem RD to the conjecture rows:** clean.
  - Theorem G values recomputed: 2, 6, 64, 10240, 402,653,184.
  - Counts 383,161, 5,478,539 and the depth-4 conjecture check match `out_brute3_r3.txt` and `out_lazy2_r4.txt`.
  - The depth-4 check is against L of the used part of the tree, which is at most L(T), so the row is supported.
  - The "depth at most r − 5" wording matches PROOFS.md:110-115.
- **"Towards an upper bound":** clean.
  - I replayed the depth-2 example: 01 → 00 → 10 → 01, product bound 2, L = 6.
  - The five strengthenings match PROOFS.md:168-172, and all five have non-zero violation counts at depth 3 (42, 1502, 1, 29958, 112579).
- **"What to distrust" bullet:** clean apart from defect 3.
  - The 40,000 trees are two runs of 20,000 in `out_checkF.txt`.
  - "Left as the instance wrote it" holds: `upper/PROOFS.md` is byte-identical to the instance's working copy.
  - The three clarifications match the seats' findings and PROOFS.md:53, 68-69, 96-97.
- **Rest of the Review paragraph:** consistent with the panel log. Questions 1 to 8 were confirmed by all five seats, and the attributions of the Theorem RO and "r − 5" findings are right.
- **Files:** clean. `upper/` holds PROOFS.md, programs, outputs, BRIEF.md and REPORT.md, with no binaries.
- **Stale-sentence check on older text:** nothing found. Rows 55-57, the "exact function is open" bullet and the D(4) bullet remain true alongside the new results.
