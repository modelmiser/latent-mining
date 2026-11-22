<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 18 (a stack in write-once memory)

Repository folder under review: `18-write-once-stack/` (present in the working tree; it is not committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `18-write-once-stack/chain/3-question.txt` (formal) and `README.md` ("The game").

Claims made in `18-write-once-stack/README.md`:

- C1. R(n) <= n - 1 for n >= 2 under the literal reading and for n >= 1 under the strict reading; R(1) = 1 literal (`solver-b/proofs.txt` P1; `solver-a/proofs.txt` section 1).
- C2. R(n) = n - 1 for n = 2..8 (schemes in `solver-a/witness_n*`, `solver-b/s_n*`; checkers `solver-a/check.py`, `solver-b/check.py`, `verifier/vcheck.py`).
- C3. The two readings agree for t >= 3 (`solver-b/proofs.txt` P2) and the reduction P0.
- C4. R(n) >= floor(n/2) (pair scheme, P3) and R(n) >= floor(n/2) + 2 for n >= 6 (P4, relying on `solver-b/mwit/`).
- C5. The README's status wording, its two explanatory sections and its "What to distrust" section match what the files support. The headline question (R(n) = n - 1 for all n) is stated as OPEN.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1: read P1 line by line. Is "the top differs, so Q differs, so the configuration differs, so a new cell is set" sound at every step from the second on? Is the case where the first push leaves 0^n closed correctly for n >= 3 (does PUSHb,PUSHb really stay at 0^n; are PUSHb,PUSHb,POP and PUSHb,POP both legal and within t = n)? Is the n = 2 case check complete (both b, and c1 = 11)? Is R(1) = 1 right under the literal text of `chain/3-question.txt`?
- Q2. C2: pick `solver-a/witness_n4_t3.txt` (or a larger one) and check it yourself against the definition for several adversary sequences including ones with POP on a non-empty stack after mixed pushes; say whether the file format lets a checker verify monotonicity (c <= E(c, op)) and whether `verifier/vcheck.py` actually enumerates every legal sequence of length <= t and checks Q after every operation.
- Q3. C3: is P0's "hosts" reduction an equivalence, and is "only the top r + 1 entries matter" right, including legality of POP? Is P2 sound?
- Q4. C4, P3: is the pair-scheme invariant right (touched pairs form a prefix; live pairs read left to right give the stack), including POP on a stack whose pushes and pops interleave, and is the count of operations right?
- Q5. C4, P4: is the hand-over from the pair phase to the computed scheme well defined as a function of the configuration alone? Check: that "no untouched pair" plus the pair contents determine the stack and the number of operations done; that r = t - p - d <= 5; that the computed scheme's start condition Q(all-zero) = top(w) is compatible with what Q returned before; what happens if the pairs run out exactly when no operations remain; sequences that never exhaust the pairs; odd n. Does t = floor(n/2) + 2 follow for every n >= 6?
- Q6. Does the README's sentence "If that held for every r it would give R(n) = n - 1" follow? Does anything in the README present the open headline as more settled than it is?
- Q7. Check each Status cell and each bullet of "What to distrust" against `reports/referee.md`, `reports/solver-a.md`, `reports/solver-b.md` and `prior-art.md`. Flag any number, count or attribution that differs from its source.
- Q8. Is the top-level `README.md` row for experiment 18 consistent with the folder, and is the README's "The game" the same game as `chain/3-question.txt`?

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
