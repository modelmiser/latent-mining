<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED. The description of the consistent orders in `solver-b/proofs.txt:6-11` is right. A refused OUT says only "this item is not m′", which is implied once m′ leaves from another item, so flags can be cleared. IN and SHIFT legality depends only on counts the sorter already knows (:4-5), so a refused one carries no information. A buried item can be flagged (probe the top of S_1, then IN on top of it), and the per-stack bottom-to-top flag lists keep that. "N = best sum over the decision tree" is right because the leaves are single, fully pinned orders and the branches split the consistent set. The `rank` assertion in `prove_k2.py:19-27` checks that every attempt from any stack strictly decreases the recursion's measure, so it is well founded. OUT from any stack is handled (:14-16 of the code, proofs :14). The flag-DP matches the literal rules at every state, 1,907,390 for k=2, n=10 (`verifier/lemma_out.txt`). Replacement: in :18, say "positions" means total items in the stacks, then items in S_1.

Q2: CONFIRMED. In `solver-b/proofs.txt:23-30`, with ≤ k items left and something unarrived, at most k−1 items sit in the stacks, so S_1 can be emptied. At n=k+1, if all k tops are refused, the only unarrived item must be 1. IN is legal because S_1 holds one item, and OUT 1 then succeeds. If one top succeeds, k items are left, one per stack. — none

Q3: CONFIRMED. The sorter is deterministic, and within a phase every answer depends only on which item is m (:33-36), so the adversary may choose late. In C(j) (:44-48), maximality forces |S_i|=2 and exactly j−1 items in the last j−1 stacks. When b is output, those stacks hold at least j items: nothing else leaves and items only move forward. If the sorter never reaches b, the order fails. If n ≥ 2k+2, the item carrying 1 never arrives. Replacement: in :47, "≥ j items", not "j items".

Q4: CONFIRMED. W_r = A·W_{r−1} and each slack is a linear functional of A^{r−1}W_0, so Cayley–Hamilton gives the order-≤49 recurrence. That turns 121 consecutive zeros (code :75-76) into zeros for all r ≥ 8. The quantity slack(r+1) − β·slack(r) gets multiplied by α each step. `ge_beta` (:58-64) is correct exact arithmetic, and the induction order covers every attempt, not only PI's (:27). I re-ran it: "36 slack sequences certified", n checked to 130. — none

Q5: CONFIRMED. `prove.py 2 5 8` takes all 49 states (a state with an unflagged top has only the forced probe), all actions, and u=5..8. It checks f = max Q, and that each Q obeys the recurrence using Q at u−2 = 3. It also checks one fixed a* with d(4..8) ≥ 0 and d(u) ≥ λ2·d(u−1). I re-ran it: 204 checks, failures=0. The step from (S(u−1), S(u)) to S(u+1) is valid for u ≥ 6 (`proofs.txt:56-60`). The theorem survives the referee's point: the empty state has the single action IN, so f = Q(IN) at every u, and d(3) is never needed. Replacement: in :61, "for every s and u ≥ 6 (u ≥ 5 for the empty state)".

Q6: CONFIRMED with domain limits. The generating function expands to 1, 2, 6, 21, 75 and then follows 5a−5a′. The closed forms give 6, 21, 75, 270, 975. `verifier/flag_out.txt` checks them to n=300. The formulas only hold for j ≥ 1 (odd n) and j ≥ 2 (even n): at n=2 the even formula gives 9/5. Replacement: give the README formulas with "(j ≥ 1)", "(j ≥ 2)", and "N(2,1)=1, N(2,2)=2".

Q7: CONFIRMED. The table matches `verifier/flag_out.txt` and the definition-level runs in `def_out.txt`/`def_out2.txt`: k=2 to n=10, k=3 to n=8, k=4 and k=5 to n=7, k=1 to n=10 (that last one is left out of the README). — none

Q8: REFUTED, on wording only. The findings below are a few small overstatements, none to a headline.

F: MODERATE — `README.md:69` — The closed forms are stated with no range of j. Taken literally, N(2,2)=3·5⁻¹·L_2=9/5, and the odd form gives N(2,1)=3/5. — high — Add "j ≥ 1 / j ≥ 2; N(2,1)=1, N(2,2)=2".
F: LOW — `README.md:69` — "Checked to n=300" is credited to `solver-b/proofs.txt`, but B's code checks only to 130. The 300 check is in `verifier/flag_out.txt`. — high — Cite `verifier/flag_out.txt`.
F: LOW — `README.md:67` — The row says "twice", but the Where cell omits `solver-a/proofs.txt` Theorem 2. — high — Add it.
F: LOW — `README.md:123-124` — "the one sign condition needed at the start" understates the check: B also checks slack ≥ 0 for r=1..9 in integers (`prove_k2.py:74`). — high — Say "checks slack ≥ 0 for r ≤ 9 and decides slack(9) ≥ β·slack(8) exactly".
F: LOW — `README.md:110-112` — Fixing a rule gives a 49-dimensional linear recurrence, not x²−5x+5. That characteristic polynomial is a checked fact. A's proof does not use the matrix view at all. — medium — Say "…a fixed linear function…; computation shows the values satisfy x²−5x+5".
F: LOW — `README.md:116-119` — Solver A's induction also carries forward the recurrence for every action value, in the within-level order. It also needs the margin at u−2. — medium — Add "and every action value obeys the recurrence".
F: LOW — `README.md:100-101` — "has learned nothing" is not the real reason. The sorter is deterministic, its answers within a phase depend only on which item is the minimum, and flags clear at each phase start. — medium — Use that wording.
F: LOW — `README.md:136` — "three times A052936" leaves out the offset. — high — Write "N(2,n) = 3·A052936(n−2), n ≥ 3" (`prior-art.md:10`).
F: LOW — `README.md:92-93` — "When it is finally output, at least k+1 items sit in k stacks" is only true if 1 arrives at all. For n ≥ 2k+2 it never does, so the sorter fails outright. — high — Add "(if ever)".
F: LOW — `README.md:71` — The literal-rule list leaves out k=1 to n=10. — high — Add it.
F: LOW — `solver-a/proofs.txt:44-45` — "after IN and OUT of 1" does not say that SHIFTs before that OUT only add items to S_2..S_k. The conclusion holds, but the step is unstated. — medium — State that items only move forward.
F: LOW — `solver-b/proofs.txt:55` — "topological order" reads as PI's order. Optimality needs an order that covers every attempt, which `rank` does provide. — medium — Name `rank` as the order.

Not covered by the questions:
- The README's closed-form row attributes checks to the wrong files (above).
- The verifier's memo keys are 128-bit hashes (`referee.md:38`). The 1.9-million-state literal-rule comparison therefore carries a tiny hash-collision risk, and the README does not disclose it.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
