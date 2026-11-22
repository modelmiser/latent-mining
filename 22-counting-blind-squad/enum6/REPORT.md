<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the follow-up instance that enumerated all six-state rules, verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

# Experiment 22: L(6) by complete enumeration — report

Note: the harness refused the write of `REPORT.md` ("subagents should return findings as text"), so this text is the only copy of the report.

## Strongest statement

**COMPUTED: L(6) = 8, attained by exactly 336 rules** (56 classes up to renaming the three unnamed states). Every one of the 6^14 = 78,364,164,096 six-state rules was simulated; none synchronises all of 2..9. This agrees with the page.

Histogram over all six-state rules (N = largest N with every length 2..N synchronised; N = 1 cannot occur; "0" = fails length 2):

| N | rules |
|---|---|
| 0 | 71,265,267,072 |
| 2 | 6,666,570,006 |
| 3 | 418,310,088 |
| 4 | 10,046,130 |
| 5 | 3,939,300 |
| 6 | 28,998 |
| 7 | 2,166 |
| 8 | 336 |
| ≥ 9 | 0 |

## Parts

- **COMPUTED — enumeration.** `enum.c`, written from `chain/3-question.txt` only; I read no program in solver-a/, solver-b/ or verifier/, before or after. It is an odometer over all 14 entries (equivalent to 14 nested loops) with the simulator called for every rule, trying lengths 2, 3, … until the first failure. No entry is fixed lazily, no symmetry is used, nothing is skipped. Outputs: `out/s6_000_054.txt`, `out/s6_054_108.txt`, `out/s6_108_162.txt`, `out/s6_162_216.txt` (216 chunks = the first three entries). Four runs, all exit 0, about 34 minutes wall on cores 6,7; the sample beforehand gave 38 ns per rule, about 50 minutes of core time.
- **COMPUTED — coverage.** `aggregate.py` → `aggregate_s6.txt`: chunk ids 0..215 each present once, each with 6^11 rules visited, total equal to 6^14, each chunk's histogram summing to its visited count, and the number of listed rules equal to the histogram for N = 7 and 8.
- **Never-fires detection.** A run is deterministic, and a run without F on n cells has at most 5^n configurations, so it must repeat one; after a repeat it is periodic and never fires. `enum.c` uses Brent's cycle detection on the exact configuration (3 bits per cell in a 64-bit word). It says "never fires" only when two configurations at different times are equal, so there are no false verdicts, and it must terminate within a small multiple of 5^n steps. No step bound is used as a verdict. Lengths are capped at 20 by the word size; no rule came near it.
- **COMPUTED — second simulator.** `check.py` (Python, separately written, keeps the set of all configurations seen instead of Brent) recomputed N for all 2,502 rules with N ≥ 7 (`check_s6_ge7.txt`) and for the 336 (`check_s6_max.txt`, with firing times): 0 mismatches. It rechecks only the rules the C program listed, not the rejections.
- **The 336 rules:** `max_rules_s6.txt` (digits = entries (Q,1),(Q,2),(G,0),…,(U3,2); 0 = Q, 1 = G, 2 = F, 3..5 unnamed). 312 of them fire lengths 2..8 at times 7, 9, 9, 15, 17, 13, 13; the other 24 at 8, 10, 10, 19, 21, 20, 20.
- **COMPUTED — comparison** (`compare.py` → `compare_out.txt`; solver-b files opened only after the run was complete). `witness_s6.txt` holds one rule, not a list: it is in my 336 (`31310541211411`) with the same firing times. The page's count 336 matches; solver-b's `lazy6_nosym_out.txt` also says 336. Solver-b has no list of the 336 to compare one by one. My list is closed under the 6 renamings.
- **COMPUTED — validation with the same program.**
  - Five states: all 48,828,125 rules, L(5) = 5 with 1,244 rules (`out/s5.txt`, `aggregate_s5.txt`); the set equals `solver-b/all_L5_rules_s5.txt` rule for rule. Histogram for N = 0, 2, 3, 4, 5: 43,920,625 / 4,663,756 / 239,212 / 3,288 / 1,244.
  - Four states: L(4) = 3, 232 rules of 65,536 (57,920 / 7,384 / 232).
  - Three states: L(3) = 2 (207 / 36).
  - The README's example rule gives times 5, 8, 7, 10 and fails at 6.
- **Checks seen failing:**
  - `aggregate.py` with one run file left out (54 missing chunks, total mismatch) and with one chunk line deleted (missing chunk 77).
  - `check.py` with one rule's N altered and one digit altered (2 mismatches of 3).
  - `compare.py` with one letter changed in a copy of the L5 list (sets differ).

Nothing is PROVED in the mathematical sense; nothing is SKETCH or FAILED.

## What failed or went wrong

- **Two breaches of the brief, both mine.**
  - `check.py` on the 1,244 five-state rules ran for a few seconds while both enumeration processes were alive, so three compute processes existed at once.
  - One scratch file was written to `/tmp/claude-1000/x.txt`, outside the folder; I moved it into `scratch/`.
- My waiting loops returned earlier than I assumed, and I briefly took normal progress for a stall. No run was affected.
- The README's other rule (the n = 9 one) fails length 2 as a whole rule, so running it tested nothing.
- `git status` in `github/latent-mining` shows modified `README.md`, `16-erasing-counter/README.md` and an untracked `16-erasing-counter/upper/`. I wrote nothing there; those are not from this task.

## What a reader must still trust

- That `enum.c` implements the definition (about 100 lines; the simulator is `simulate`). The independent evidence is the exact five-state agreement with the earlier programs and the Python recheck of the listed rules only.
- That the compiler and machine ran it correctly: each chunk was run once, not twice.
- One model family wrote everything, including this check.
- The agreement on 336 is a count plus one witness, not a list-against-list comparison.

## Files (all in (staging)/)

- Programs: `enum.c`, `aggregate.py`, `check.py`, `compare.py`
- Raw output: `out/s5.txt`, `out/s6_*.txt` (with `.exit` files), `out/sample6.txt`
- Results: `aggregate_s5.txt`, `aggregate_s6.txt`, `max_rules_s5.txt`, `max_rules_s6.txt`, `check_s6_ge7.txt`, `check_s6_max.txt`, `compare_out.txt`
- `scratch/`: the deliberately broken inputs

Build with `gcc -O2 -march=native -DS=6 -o enum6 enum.c`; run with `./enum6 3 FIRST LAST 7`. Total disk use is 408K.
