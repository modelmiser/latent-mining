<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 23 (gossip with one player stalling and one hurrying)

Repository folder under review: `23-gossip-tug/` (present in the working tree; it is not committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The game is defined in `23-gossip-tug/chain/3-question.txt` (formal) and `README.md` ("The game").

Claims made in `23-gossip-tug/README.md`:

- C1. A(4) = 6 (hand proof: `solver-b/proofs.txt` Theorem A; retold in the README section "Why four people need all six calls").
- C2. A(n) >= 2n - 3 for every n >= 2 (`solver-a/proofs.txt` P0, P1; retold in the README).
- C3. The three static bounds of `solver-b/proofs.txt` (Lemmas 1-3), which the larger searches use to cut the search.
- C4. The computed values A(2..10) = 1, 3, 6, 9, 12, 16, 19, 23, 27 with the stated provenance, and the strategy files (`solver-a/cert_n*_D.txt`, `cert_n*_H.txt`, checker `solver-a/check.py`; `solver-b/table*.txt`, `cert_n*_ge*.txt`, checkers `check_table.cpp`, `check_cert.py`).
- C5. The README's status wording, its two explanatory sections and "What to distrust" match what the files support.

Questions. Verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1: play the README's argument by hand from the rules in `chain/3-question.txt`. At each round list the legal partners of the named agent (a call {a, b} is legal iff a is not in K_b or b is not in K_a) and check: that after {1,2} and naming 1 the reply is a new agent; that after naming 2 the legal replies are exactly 3 and 4; in the branch "reply 3", that three more calls are needed; in the branch "reply 4", that naming 2 again forces {2,3}, that 3 is then fully informed yet still active, that its only legal partner is 4, and that a sixth call is still needed. Is "up to symmetry" used soundly? Is naming a fully informed agent allowed by the rules as written?
- Q2. C2: check P1 line by line: that while agent 1 is active every call naming it is with an agent that has not called yet (why can 1 not call an earlier partner again?), that the phase lasts exactly n - 1 calls whatever the Hastener does, the chain structure at its end, that exactly two agents are fully informed, that the chain property is preserved by every later call, and that a call makes an agent fully informed only if the other already was. Does the bound hold whatever the Delayer does in phase 2, and for n = 2 and 3?
- Q3. C3: are Lemmas 1-3 proved, and are they valid as bounds on the game value at an arbitrary position (lower bound from Lemma 1 and 2: for every play, hence for the value; upper bound from Lemma 3: is there really a Hastener strategy achieving it against every Delayer choice)? Could using them as cut-offs make a search answer wrongly, and in which direction?
- Q4. C4: read `solver-a/check.py` (or `verifier/g6.cpp`'s checker) and say whether a Delayer file is checked correctly (for every Hastener reply from every listed position the play continues inside the file, and the number of calls is at least the claim) and a Hastener file likewise (for every agent the Delayer may name there is a listed reply). Is working with positions up to relabelling of agents sound? Is the README right about which values have files for which player, and about what the n = 9 and n = 10 values rest on?
- Q5. Is the README's "The game" the same game as `chain/3-question.txt`? Are the remarks "the shortest possible run has 2n - 4 calls" and "the longest n(n-1)/2" stated as known facts with the right scope (n >= 4 for the first)?
- Q6. Does the README's retelling of the two proofs say anything more than, or different from, the proof files? Check every sentence, including "a call adds at most one new knower of a given secret" and "each call adds at most one fully informed agent".
- Q7. Check each Status cell and each bullet of "What to distrust" against `reports/referee.md`, the solver reports and `prior-art.md`; flag any number, count or attribution that differs from its source (for example the variant values 5, 8, 10, 14, 16; the differences 2, 3, 3, 3, 4, 3, 4, 4; which solver proved what; the OEIS remark).
- Q8. Does anything in the README present the computed values or the open questions as more settled than the files support? Is the top-level `README.md` row for experiment 23 consistent with the folder?

For each defect give severity (CRITICAL: a headline claim is false or unsupported; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
