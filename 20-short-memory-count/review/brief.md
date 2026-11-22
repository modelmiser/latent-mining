<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 20 (counting the last w bits with w + 1 states)

Repository folder under review: `20-short-memory-count/` (present in the working tree; it is not committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The object is defined in `20-short-memory-count/chain/3-question.txt` (formal) and `README.md` ("The question").

Claims made in `20-short-memory-count/README.md`:

- C1. f(w) <= floor((w-1)/2) for every w >= 3 (`solver-b/proofs.txt` Theorem A for odd w and Theorem D for even w; `solver-a/proofs.txt` P1).
- C2. f(w) >= 1 for w >= 2 (Theorem C / P2) and "bound s forces at least w - s + 1 reachable states" (Theorem B / P3).
- C3. Lemma 1 of `solver-b/proofs.txt`: g(M) <= s iff the reachable states of spread > s induce an acyclic graph; and the safety of the pruning (Lemmas 2, 3).
- C4. The computed values f(1..10) = 0,1,1,1,2,2,3,3,4,4 with the stated provenance.
- C5. The README's status wording, its description of the machines and its "What to distrust" section match what the files support. The general formula is stated as a CONJECTURE.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C3: is Lemma 1 right against the definition of g(M) in `chain/3-question.txt` (max over simple cycles of the min spread on the cycle; cycles of reachable states only; self-loops)? Is the README's game description ("lit states", "infinitely often") equivalent?
- Q2. C1, Theorem A: check the bounds L(k) for odd w (both cases of the lower bound on a, the symmetry for the upper bound) and that the machine has at most w + 1 states. Does every state have spread <= floor(w/2)?
- Q3. C1, Theorem D: check the accounting identity a = t + (k - h - e0 + eK)/2, the parity fact, the symmetry remark (is it legitimate to transfer bounds when the start state changes?), each of the four cases for state 0, and both cases for odd k, including the integrality step a' >= (k-1)/2. Check that every step from an even inner level leads to an odd level, so the over-wide states have no cycle. If you can, verify the w = 4 or w = 6 machine by hand from `solver-b/witnesses.txt` (format in `reports/solver-b.md`).
- Q4. C2: read Theorem C and Theorem B. Is each a proof? In Theorem B, is "a is reached by 0^n for arbitrarily large n, so 0 is in C(a)" right, and is the count of distinct states right?
- Q5. C4: is the pruning rule (drop a partial machine once its already-defined transitions contain a cycle of states whose spread, computed from the defined part, exceeds s) safe, i.e. can it only lose machines that are really bad? Could an error make a reported f too small, or only too large? Is the README right about which values were checked how many times and by whom (compare `reports/referee.md`)?
- Q6. Is the README's description of the two machine families accurate (state counts, the "jump two levels" rule, which levels are reliable), and is the statement that solver A's family equals solver B's for even w supported?
- Q7. Check each Status cell and each bullet of "What to distrust" against `reports/referee.md`, `reports/solver-a.md`, `reports/solver-b.md` and `prior-art.md`. Flag any number, count or attribution that differs from its source.
- Q8. Is the top-level `README.md` row for experiment 20 consistent with the folder, and does anything present the conjecture as more settled than it is?

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
