<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 22 (a firing squad whose cells can only count)

Repository folder under review: `22-counting-blind-squad/` (present in the working tree; it is not committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The problem is defined in `22-counting-blind-squad/chain/3-question.txt` (formal) and `README.md` ("The problem").

Claims made in `22-counting-blind-squad/README.md`:

- C1. L(3) = 2, L(4) = 3, L(5) = 5 by full enumeration (`solver-a/fsq.cpp` brute mode, `solver-b/brute.cpp`, `verifier/ref.cpp`), with 36, 232 and 1,244 optimal rules.
- C2. L(6) = 8 and L(7) = 11 by lazy backtracking searches (`solver-a/fsq.cpp` dfs, `solver-b/lazy.cpp`, `verifier/ref.cpp`, `verifier/ref2.cpp`).
- C3. The hand-proved facts: firing time >= n - 1; d(Q,1) not in {Q, F} for a rule synchronising some n >= 3; solver A's Lemma 2; solver B's proof of L(3) = 2.
- C4. The example five-state rule in the README and its firing times 5, 8, 7, 10 for n = 2..5, failing at n = 6.
- C5. The README's status wording and "What to distrust" match what the files support.

Questions. Verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C4: simulate the README's example rule by hand for n = 2, 3 and 4 from the definition in `chain/3-question.txt` (compute k for every cell at every tick, including end cells, which have one neighbour) and report the firing times you get, and what happens at n = 6 if you can. Is the README's table the same rule as the line `0 3 3 3 1 0 2 4 1 2 1 0` in `solver-a/witnesses.txt` under the stated numbering?
- Q2. C1: read one of the brute-force programs and say whether it implements the definition: the visible quantity k; simultaneous update; "fails if some cell is in F while another is not"; "fails if no cell ever fires", decided exactly (cycle detection) and not by a step cap; the constraint d(Q,0) = Q; the count s^(3s-4) of rules. Does "L(s) = largest N such that one rule synchronises every n in 2..N" match what the programs maximise?
- Q3. C2: is lazy assignment sound (a run that never reads a table entry behaves the same for every value of it), is the "lowest unused label" symmetry rule sound, and does the referee's weight-sum check (cases add up to 7^17) show that the search without symmetry breaking covers every rule exactly once? Could an error make a reported L too large, or only too small? Is the README right about what the three instances' searches share?
- Q4. C3: check the proof of "t >= n - 1" and of "d(Q,1) not in {Q, F}" (note the end cell with k = 1, and what happens on n = 3 if d(Q,1) = F), solver A's Lemma 2, and solver B's P3 (L(3) = 2). State the first step that does not follow, if any. Is the README's paraphrase of each statement accurate?
- Q5. Is the README's "The problem" the same problem as `chain/3-question.txt` (what a cell sees; end cells; the initial configuration; what counts as synchronising; the definition of L)? Is the claim that "a counting rule is a restricted ordinary rule" correct, and does the README use it only where it applies?
- Q6. Check each Status cell and each bullet of "What to distrust" against `reports/referee.md`, the solver reports and `prior-art.md`; flag any number, count or attribution that differs from its source (for example who enumerated what, 96 versus 4 classes, the minimum firing times, the counterexample count).
- Q7. Does anything in the README present the computed values as more settled, or the open question as more answered, than the files support? Is the top-level `README.md` row for experiment 22 consistent with the folder?

For each defect give severity (CRITICAL: a headline claim is false or unsupported; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
