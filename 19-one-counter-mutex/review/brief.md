<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining experiment 19 (mutual exclusion with one shared counter)

Repository folder under review: `19-one-counter-mutex/` (present in the working tree; it is not committed yet). Read the files, not this summary. Assume the authors (instances of one AI model) are competent and wrong until you have traced each claim yourself. The problem is defined in `19-one-counter-mutex/chain/3-question.txt` (formal) and `README.md` ("The problem").

Claims made in `19-one-counter-mutex/README.md`:

- C1. No solution with b = 3 for n >= 3 and any number of states (`solver-b/proofs.txt` L0, L1, T3; `solver-a/proofs.txt` Theorem 2).
- C2. No solution with b = 2 for n >= 3 (T2 / Theorem 1).
- C3. g(3) <= 4 by the automata in `solver-b/w_n3_b4_s7.txt` and `solver-a/w_n3_b4_burns.txt` (formats in the reports and checker headers), accepted by model checkers `solver-a/checker.py`, `solver-b/check.cpp`, `solver-b/slowcheck.py`, `verifier/vcheck.py`.
- C4. g(2) = 2; g(4) <= 8; g(5) <= 14; the bounded-state search results.
- C5. The README's status wording, its two explanatory sections and "What to distrust" match what the files support.

Questions. Give a verdict for each: CONFIRMED / REFUTED / UNSUPPORTED, with file:line evidence.

- Q1. C1, L1 of `solver-b/proofs.txt`: check each of (a)-(d) against the exact safety and progress definitions in `chain/3-question.txt` (progress forbids only schedules in which every process that is active at some time >= T takes infinitely many steps; a process may sit in rem or crit forever). In particular: is "run p alone into crit and leave it there, then run q alone" a legitimate schedule for a SAFETY argument; in (c), is process j, parked mid-protocol, a problem for the argument?
- Q2. C1, T3 Step 1: do the L1 facts force E_k = {0} and a_j = e_i modulo 3? Is "multiply every add constant by 2" really a symmetry of the problem? Is "exit = add 2" forced?
- Q3. C1, T3 Step 2: check (i)-(iv). Is the helper's behaviour legitimate at each use (it must see exactly what it would see running alone; it leaves through crit; the third process stays in rem)? In (i), why may R not become 0? In (iii), check the sequence of counter values. In (iv), why is the next non-zero add equal to 1?
- Q4. C1, T3 Steps 3-6: is joint reachability right (does "p stands still" really change nothing for the constructions involving q and the helper, given that p is active and has contributed 0)? Are the two infinite schedules of Steps 4 and 5 genuine progress violations under the definition (who is active, who moves forever, is anybody ever in crit, is the third process ever active)? In Step 5, do both processes necessarily have the same sequence of level types, or does lockstep need more care when one chain's levels take different numbers of steps? Is the case split of Step 6 exhaustive?
- Q5. Try to break C1: describe three automata over Z_3 that you think might solve the problem, and say where the proof's schedule defeats them; or find a step of the proof that an adversarially designed automaton evades (for example one that reads many times, uses add 0, or has unreachable states).
- Q6. C3: read `verifier/vcheck.py` (or one of the other checkers) and say whether it checks safety over all reachable configurations and progress exactly as defined (a crit-free cycle or strongly connected set in which the active set is constant and every active process moves). Check the witness file is a legal set of automata (structural rules on rem and crit).
- Q7. Does the README's section "Why three values are not enough" say anything more than, or different from, the proof? Check every sentence. Does "Why four values are enough" claim anything not supported by the files?
- Q8. Check each Status cell and each bullet of "What to distrust" against `reports/referee.md`, `reports/solver-a.md`, `reports/solver-b.md` and `prior-art.md`; flag any number, count or attribution that differs from its source. Is the top-level `README.md` row for experiment 19 consistent with the folder?

For each defect give severity (CRITICAL: a headline claim is false or unproved; MODERATE: a stated claim is wrong or unsupported but the headline stands; LOW: wording), file:line, and the replacement wording you would use.
