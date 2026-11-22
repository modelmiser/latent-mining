<!-- Archived by the organiser on 2026-11-21. Report of a second-round reviewer (one Claude instance) on this experiment's page, taken from the session transcript. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

Second-round review, experiment 22 (`22-counting-blind-squad/README.md`). One false sentence found; it errs toward understating the evidence. Everything numeric on the page checks out.

**Recomputed and confirmed**
- Re-running `enum6/aggregate.py` on `enum6/out/s6_*.txt` in scratch reproduces `aggregate_s6.txt` and `max_rules_s6.txt` byte for byte: 216 chunks, 6^14 rules, maximum 8, 336 rules.
- The six-state histogram equals the one at `reports/solver-a.md:25` at all eight lengths.
- The five-, four- and three-state outputs give 1,244, 232 and 36.
- 2,166 + 336 = 2,502; the time split is 312 / 24; chunks 39 (4 optimal rules) and 77 in `organiser_rerun.txt` match the recorded lines; 4 chunks = 1.45 × 10^9 rules.

**Defects**

1. **CRITICAL (false, but understates) — README.md:88-89.** "the 336 rules themselves could be compared only through one witness, because the searches kept no list."
   - `solver-a/dfs6.txt` does list the optimal rules: 16 `dfs L=8` lines, as partial rules with `-1` for entries never read.
   - I expanded them (every value for each unread entry, times the 6 renamings of the unnamed states) and got exactly 336 rules, set-equal to `enum6/max_rules_s6.txt`.
   - Two witnesses were also available, not one. `compare.py` used solver B's (`31310541211411`, in the 312 group); solver A's (`43230530150413`, in the 24 group) is in the list too.
   - Replacement: "the follow-up compared the 336 rules with the earlier work only through solver B's witness; solver A's search output (`solver-a/dfs6.txt`) does list the 16 optimal partial rules up to renaming, and expanding them gives the same 336 rules (checked by a second-round reviewer; no script in the folder)."
   - The script is only in my scratchpad (`(scratch)/r22/cmp2.py`); if the page claims this, add an equivalent to `enum6/`.

2. **MODERATE (unsupported) — README.md:80.** "(it was told not to read the earlier programs, and reports that it did not)."
   - The follow-up's report is not in the folder: `enum6/BRIEF.md` asks for a `REPORT.md` and there is none. The only in-folder support is the header comment at `enum.c:1` and the docstring of `compare.py`.
   - README.md:192 ("`reports/`: each instance's final report, verbatim") is therefore incomplete for the follow-up instance.
   - Fix: add the report as `enum6/REPORT.md`, or write "its source header says it was written from the question text only; its report is not included here".

3. **MODERATE (misleading) — README.md:18.** "an error in the simulator the searches share could move it either way."
   - The seven-state searches do not share one simulator. The solvers have their own programs, and README.md:107-108 says only "All the referee's programs share one simulator".
   - Replacement: "while an error common to the searches' simulators could move it either way."

4. **LOW (stale) — README.md:75-77.** "L(5) = 5 is the solid one: three separately written programs tried every rule…"
   - `enum.c` is now a fourth program, matching rule for rule, and L(6) is also fully enumerated, so "the solid one" no longer distinguishes it.
   - Suggested: "L(5) = 5 is the most checked: four separately written programs (the three original ones and the follow-up's) tried every rule…".
   - Same for top-level README.md:53, "five states by full enumeration of all rules, three times": make it "four times" or "at least three times".

5. **LOW — README.md:80.** "an odometer over all 14 free entries": the top 3 digits are fixed per chunk by the driver loop and the odometer runs over the low 11 (`enum.c:85-105`). Suggested: "all 14 free entries enumerated (3 fixed per chunk, an odometer over the other 11)".

6. **LOW — README.md:189-191.** Files lists `brief-*.txt` as "what each instance was told", but the follow-up's brief is `enum6/BRIEF.md`, which the `enum6/` entry does not mention. Add "brief" to the `enum6/` parenthesis.

**Clean sections**
- Results rows for L(6) and L(7): clean. The referee's verbatim "three search orders" and "No full enumeration exists for s = 6" are historical, and the page already corrects the first.
- "What to distrust" bullet after the L(6) one (L(7) and earlier L(6) evidence): clean.
- Last Review paragraph: consistent with the page and folder. The reviewers' own results (four chunks, all 2,502 rules) have no file in the folder and stand as reportage.
- Top-level row 22: consistent with the page apart from item 4.
- No binaries in `enum6/`.
