<!-- Archived by the organiser on 2026-11-21. Report of a second-round reviewer (one Claude instance) on this experiment's page, taken from the session transcript. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

Second-round review, experiment 18 (`18-write-once-stack/README.md`, `quad/`, top-level row 18). No CRITICAL defect: every number and log citation checked is correct. There are two MODERATE and four LOW defects.

**What I recomputed (all agree with the page)**
- **Where the bound is best, n = 5..40:** 3·⌊(n − 1)/4⌋ − 1 strictly exceeds max(⌊n/2⌋ + 2, 7) exactly at n = 17, 21, 22, 23 and every n from 25 to 40. Ties are at n = 13, 18, 19, 24. "17 against 14 at n = 25" is right. The Short answer, Results row, Review and top-level row all give the same list.
- **Sequence counts:** `count_legal(17)` = 99,095,529 and `count_legal(18)` = 296,855,529, and they sit at the lengths the page says (3q − 1 and 3q at q = 6).
- **Which q in which file:** `run_quad.log` holds only q = 5, 6 for the unmodified scheme (t = 14/15/16 and 17/18/19); `organiser_rerun.txt` holds q = 1..4 at 3q − 1, 3q, 3q + 1. This matches both Results rows.
- **Rerun in a scratch copy, q = 1..5:** survive/fail pattern and every count reproduce the two logs. The `nofreezeclean` variant survives at q = 3 and fails at q = 4, as stated.
- **Out-of-memory claim:** the log does end with two MemoryError tracebacks and no command lines.

**Defects**

1. **MODERATE, README.md:111–115** — "The instance could not push the idea further: with more levels … 3 operations per 4 cells … That is its sketch" and "It reports that its attempt to compute R(9) with solver B's program ran out of time".
   - Why: the follow-up instance's report is not in the repository. `reports/` holds only `referee.md`, `solver-a.md`, `solver-b.md`, and `quad/` has no report. Neither statement appears in `quad/PROOF.md`, so a reader cannot check them.
   - Replacement: either add the report as `quad/REPORT.md` and cite it, or append "(from the instance's report, which is not stored in this folder)".

2. **MODERATE (stale), README.md:209–212, Files** — "`brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each instance was told." and "`reports/`: each instance's final report, verbatim."
   - Why: no longer true of "each instance". The follow-up instance's brief is `quad/BRIEF.md` and its report is absent.
   - Replacement: "…what each instance of the first version was told" and "`reports/`: the final reports of the two solvers and the referee, verbatim (the follow-up instance's report is not stored)".

3. **LOW, README.md:215–216, Files** — "`quad/`: … the proof of R(4q + 1) ≥ 3q − 1, the scheme, a checker written from the definition, logs."
   - Why: omits `BRIEF.md` and `check_qe.py`, and does not say one of the two logs is the organiser's.
   - Replacement: "…the brief (`BRIEF.md`), the proof, the scheme, a checker written from the definition (plus `check_qe.py`, a reader for solver A's file format), the instance's log and the organiser's rerun."

4. **LOW, README.md:107–109** — "Charging every operation to a quad gives s = 4q − |R| − w".
   - Why: stated as an unconditional equality. `PROOF.md:89` proves "s ≥ … (in fact equality)", and only once the flag is set; the page's own distrust bullet (line 121) says the equality needs no no-op operations.
   - Replacement: "…gives, once the flag is set, s = 4q − |R| − w (the proof needs only ≥)".

5. **LOW, README.md:121 and top-level README.md:49** — "The machine check covers n ≤ 25 only" / "its scheme checked by program for n ≤ 25".
   - Why: the scheme exists and was run only at n = 4q + 1, i.e. n = 5, 9, 13, 17, 21, 25.
   - Replacement: "covers n = 4q + 1 up to 25 (q ≤ 6) only".

6. **LOW, top-level README.md:49** — "three general lower bounds proved: ⌊n/2⌋ + 2, 7 from n = 8 on, …".
   - Why: ⌊n/2⌋ + 2 lacks its range. Unqualified it exceeds the upper bound n − 1 at n = 2, 3, 4. The page has "(for n ≥ 6)".
   - Replacement: "⌊n/2⌋ + 2 from n = 6 on".

**Sections that are clean**
- **Short answer:** clean.
- **Results rows for the lower bounds, including the two new rows:** clean on files, q ranges, lengths and counts.
- **"The lower bounds" outside the items above:** clean; line 88's maximum of three bounds is consistent with the Short answer, and the freeze, waste-kill and "two more entries than operations left" sentences match `PROOF.md` and `quad.py`.
- **"What to distrust" three-quarters bullet:** clean apart from item 5. Its remarks on `PROOF.md` hold: line 141 does cite `run_quad.log` for q = 1..4, lines 111–114 do misorder the destroyed entries, and `checker.py` tests monotonicity only along reachable plays.
- **Review, both parts:** internally consistent with the Results rows and the distrust bullet. The reviewers' reports are not stored in the folder, so who found what is unverifiable there (same as the first version). There is a small tension, disclosed in the same paragraph: "All five confirmed … the freeze" alongside "one sentence of the proof gives a wrong reason" at the freeze.
- **Stale sentences elsewhere:** none found; the "R(9) is 7 or 8" row and the open-question rows are unaffected by the new bound.

Nothing was written under `(workspace)`; the scratch copy is in `(scratch)/r2-exp18-q7x4k`.
