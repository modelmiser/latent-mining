<!-- Archived by the organiser on 2026-11-21. Report of a second-round reviewer (one Claude instance) on this experiment's page, taken from the session transcript. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

Second-round review of `16-erasing-counter/README.md` (read-only; nothing written, no scripts needed).

No CRITICAL defects. Nothing on the page states outright that D(r) = W_r for every r, but four sentences lean that way or drop a hypothesis.

**MODERATE**

1. README.md:153 — "what is proved is that a cycle longer than W_r must, for every way of splitting the path, either … or enter the lower part at two different modes; and, separately, that on such a cycle some two modes must exchange information…"
   - Corollaries T-r and NC-r (`shape2/PROOFS.md`:227-232, 332-337) assume D(ρ) ≤ W_ρ for all ρ < r. So this is proved only at depth 5 (given the computed values) or at the least depth where the conjecture fails, not for general r.
   - "Share a position" should also be "a position off the path"; spine positions may be shared.
   - Replacement: "…what is proved is that at depth 5 (given the computed values for depth ≤ 4), and likewise at the least depth where D(r) > W_r if there is one, a cycle longer than W_r must, for every way of splitting the path, either have one of the upper subtrees share a position off the path (in the tree) with …"

2. README.md:153 — "so a bound, if there is one, can only be on the total".
   - This still generalises from one example, which is the fault the Review (lines 278-280) says was fixed. One failed per-root bound does not exclude other per-node charges.
   - "A bound, if there is one" is also loose, since Theorem G is a bound.
   - Replacement: "so this way of charging W_r to the root fails; if the bound W_r holds, an induction through the root needs a joint statement that nobody has found."

3. README.md:158 — the bullet lists what to discount in `shape2/PROOFS.md` §6 but omits §6 line 361, "Only the total is bounded". That sentence asserts the open statement, and the Review lists it as a finding.
   - Add: "its sentence 'Only the total is bounded' (§6) asserts the open bound and should read 'only a bound on the total can hold'".

4. README.md:163 — "Theorems RD and RO depend on it" is stale. Theorems Z1/Z, MD, NC and T all use Theorem E (`shape/PROOFS.md` §4; `shape2/PROOFS.md` Step 3 of §2, §4, §5).
   - Replacement: "Theorems RD and RO, and Theorems Z, NC and T of the later follow-ups, depend on it."

5. README.md:111 — "its last paragraph follows `shape/PROOFS.md`" is stale; the last paragraph's second half follows `shape2/PROOFS.md`.
   - Replacement: "its last two paragraphs follow `shape/PROOFS.md` and `shape2/PROOFS.md`".

**LOW**

6. README.md:69 — "so D(r) = W_r is not affected" reads as if it were established. Replace with "so the conjecture D(r) = W_r is not affected".
7. README.md:158 — "stated more narrowly than what is proved" is ambiguous. Replace with "claims more than is proved (it leaves fewer cases open than the proofs do)".
8. README.md:140 — the five strengthenings are described as strengthenings of "length ≤ L(T)" with no note here that it is itself false. Add "(itself false, see next paragraph)".
9. README.md:66 — "private positions" should be "private positions off the path"; (MD_i) lets the subtrees read spine positions.
10. README.md:153 — "the Sylvester value of the next depth" should be "of depth one less (W_(r−1))".
11. README.md:64-71 — row order: rows 64-65 mention "the failure of the shape bound" and "Theorem Z's counting" before rows 69 and 71 introduce them, and "that instance" changes referent between rows 65-67 and 70-71. Move rows 69-71 above 64, or name the instances.
12. README.md:297 — "the follow-up" should be "the first follow-up".
13. Short answer (9-19) and top-level README.md:47 are accurate, with the open status explicit. Neither mentions that the shape bound was refuted or that the depth bound now covers a larger class (Theorem Z). Optional one clause.

**Clean**

- **Test counts and values** match the output files: 6,003 trees (`shape/out_testZ.txt`), 20,592 (`out_nc.txt`), 82,420 (`out_tail.txt`), 35,387 and 165,091 (`out_lemma_t.txt`), 60/56 and 52/50 (`shape2/out_cex.txt`), 52/50, 114/98 and 762/602 (`shape/out_*.txt`, `organiser_check_out.txt`), 258/126/42 (`out_root1.txt`; the stale file does end in a traceback).
- **"Systematic boundary cases"** is right; `lemma_t.py` samples three values per c, so the file's own "exhaustive" is the overstatement.
- **Theorem G** values and the factor 2^r/(r + 1) recompute correctly.
- **"About an hour"** matches the commits (15:52 to 17:08).
- **Rows** for Lemma A, Theorem Z, the unbounded ratio, Theorem NC and the depth-4 example match the proof files.
- **Row 66's** "apart from trees whose tail is never entered, the enlargement relies on the computed values" holds: without rule (1), rule (4) collapses to (MD).
- **§6 as corrected** on the page matches what Corollaries T-r and NC-r prove, apart from item 1.
- **Files** entries exist.

**Not checkable from the folder:** the reviewer figures (485,059 cycles of 147,992 trees; about 8,000 qualifying cycles; 1.3 million further cases; "one tested it"). No reviewer reports are stored there.
