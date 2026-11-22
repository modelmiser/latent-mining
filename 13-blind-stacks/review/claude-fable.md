<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

Everything traced. Verdicts:

Q1: CONFIRMED — `solver-b/proofs.txt:4-20`: refused IN/SHIFT depend only on counts (line 4-5); refused OUT on a at min m′ ⇒ value(a)≠m′, implied once m′ leaves (lines 7-8), so clearing flags is sound; flags are kept per position bottom-to-top (line 11), so buried flagged items are tracked; OUT is generated for every stack i (`solver-b/fast.py:119-124`, `prove_k2.py:13-16`); successors partition the consistent set (line 15-16), so max-over-attempt-of-sum is the decision-tree optimum; measure (items, positions, −flags) strictly drops (line 18). Independently tested at every one of 1,907,390 literal-rule states, set size (t−f)(t−1)! and value, `verifier/deflemma.cpp:48-57`, `verifier/lemma_out.txt:72`. — none (only: say "positions, with unarrived counted as position 0" at line 18).

Q2: CONFIRMED — `solver-b/proofs.txt:23-30`; compaction keeps ≤1 per stack and frees S_1 when ≤k−1 items are in; all-refused case: S_1 holds one item so IN is legal, new top is 1, OUT succeeds, k items one per stack (line 29-30). Checks for k=1,n=2 by hand. — none.

Q3: CONFIRMED — `solver-b/proofs.txt:33-36` (deterministic sorter ⇒ play in phase m fixed by designation; never-output case handled line 35-36); Lemma 0 line 41 justifies late designation; C(1) line 42-43; C(j) line 44-48: suffix S_{i+1..k} holds the j−1 old items plus t (none leaves, none moves back, nothing unarrived) when b leaves, so ≥j items; Step A gives n−1≥k+1 items (line 37-38). — none.

Q4: CONFIRMED — `prove_k2.py:51-57` builds W_r by one r-independent policy in topological order (linear map A); `:72-77` check slack(1..9)≥0, u(r)=0 for r=8..128 (121 terms, ≥49 needed by Cayley–Hamilton on slack(r)=cᵀA^{r−1}W_0), slack(9)−β·slack(8)≥0 exactly via squaring (`:58-64`, correct case split); g(r+1)=α·g(r) with α+β=αβ=5 gives slack≥0 for all r; `:83` asserts V=W directly to r=130; Python ints only. Re-run: "OK: 36 slack sequences certified". — none.

Q5: CONFIRMED — `prove.py 2 5 8` (`solver-a/prove.py:107-127`): for all 49 states and all actions, Q(s,a,u)=5Q(u−1)−5Q(u−2) for u=5..8 and one fixed a* with d(4..8)≥0, d(u)≥λ₂d(u−1) for u=5..8; re-run gives failures=0 (4 failures on 4..7, as reported). Induction `proofs.txt:56-61` is valid: d(u+1)=5d(u)−5d(u−1)≥(5−λ₁)d(u)=λ₂d(u)≥0. The closing remark at u=5 needs d(3)≥0 (unchecked), but the empty state has only IN (`prove.py:87-92`), so N(2,n)=Q(empty,IN,n) and (i) alone gives the theorem for all n≥5. — replace line 61 with "for every s and u≥6, and for s=empty at u≥5".

Q6: CONFIRMED — hand check: 3·5^{j−1}F_{2j+1} = 6,75,975,12750; 3·5^{j−2}L_{2j} = 21,270,3525,46125; (1−5x+5x²)·G = x−3x²+x³+x⁴ gives a₁..a₄ = 1,2,6,21 then the recurrence; `flag_out.txt:79` to n=300. Formulas fail at n=1,2 (`solver-b/proofs.txt:68` says so; README line 69 omits the range). — add "for n ≥ 3" in README line 69.

Q7: CONFIRMED — rows match `verifier/def_out.txt:15-62` (k=2 n≤10 incl. 46125; k=3 n≤8; k=4 n=5..7; k=5 n=6,7), `verifier/flag_out.txt:93-97`, `solver-a/table_out.txt`, `solver-b/fast_small.txt`. Literal-rule runs for k=4 stop at n≥5 and k=5 at n≥6 (smaller n are n! by Thm 1). — "k = 4 for n = 5..7, k = 5 for n = 6, 7".

Q8: CONFIRMED with LOW wording issues — Status cells match reports; both explanatory sections track the proofs. Exceptions: README:136 "three times OEIS A052936" omits the shift (prior-art.md:10: 3·A052936(n−2)); README:123-124 "the one sign condition" omits the nine integer sign checks slack(1..9)≥0 (`prove_k2.py:74`); README:86-90 omits compaction before each IN (`proofs.txt:24-26`). — see fixes below.

F: LOW — README.md:69 — closed forms stated without range; N(2,2)=2 ≠ 3·5^{−1}·L₂, N(2,1)=1 ≠ 3·5^{−1}·F₁ — high — add "(n ≥ 3)".
F: LOW — README.md:136 — "three times OEIS A052936" drops the index shift; prior-art.md:10 says N(2,n)=3·A052936(n−2) — high — "three times A052936 shifted by two".
F: LOW — README.md:123-124 — "decides the one sign condition … in Q(√5)" understates: nine integer non-negativity checks (r=1..9) are also needed and done (`prove_k2.py:74`) — high — "checks slack ≥ 0 for r = 1..9 in integers and the one Q(√5) inequality slack(9) ≥ β·slack(8)".
F: LOW — README.md:71 — "k = 4, 5 up to n = 7" implies literal-rule runs for all smaller n; `def_out.txt` has k=4 only n=5..7, k=5 only n=6,7 — high — state the actual ranges.
F: LOW — solver-a/proofs.txt:61 — closing remark "for every s and u ≥ 5" needs d(3) ≥ 0, never checked (referee already notes; theorem unaffected) — high — "for every s and u ≥ 6; for s = empty from u = 5".
F: LOW — README.md:86-90 — "Enough room" omits shifting items right to free S_1 before each IN, which the proof needs (`solver-b/proofs.txt:24-26`) — medium — add "shift items right so S_1 is free before each arrival".
F: LOW — README.md:70 — "proved by solver A by the same method" for k=1: `solver-a/proofs.txt:62` is one sentence; the k=1 induction (d(u+1)=2d(u)) is not written out, only the script run (I re-ran: failures=0) — medium — "by the same script; the k = 1 induction is not written out".
F: LOW — solver-b/proofs.txt:18 — well-founded measure "(#items left, positions, −#flags)": IN leaves items-left unchanged, so "positions" must count unarrived as position 0; not stated — low — add that clause.

Not asked but worth noting: the whole headline rests on Lemma 0, which only a machine-tested hand proof supports; I found no gap in it. No CRITICAL or MODERATE defects.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
