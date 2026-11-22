<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `solver-a/proofs.txt:12-18` — none
Q2: CONFIRMED — `solver-a/proofs.txt:22-27` — none
Q3: CONFIRMED — `solver-a/proofs.txt:32-43` — none
Q4: REFUTED — `solver-a/proofs.txt:49-53` — the only gap is `x = 1^m` (c = 0 still contains a zero); (ii), displaced (iii), and the deletion-site split at `58-60` hold. Add: "If `x = 1^m`, `y = 1` stays `1^{m+1}`; `y = 0` is bubbled by `c_m…c_1` to `0 1^m`. A sorted vector, including `1^n`, is fixed by every letter of (iii)."
Q5: CONFIRMED — `solver-b/proofs.txt:101-104` and `:68-97`; base `verifier/out_lemmas.txt:31-40`, `solver-b/runup.txt:5-6` — none
Q6: CONFIRMED — `solver-a/proofs.txt:63-64`; at n = 2, `c_1` is slip-proof and not deletion-proof (`verifier/out_lemmas.txt:8`), so S(2) = 1 while A(2) = (n−1)(n+2)/2 = 2. `README.md:5` states the equality only for n ≥ 3 — none
Q7: REFUTED — slip paragraph `README.md:47-57` matches T1, and n = 3 with upward slips only has size 4 (`solver-b/runup.txt:1`, word `2 1 1 2`). `README.md:61-66` never uses deletion-proofness, so it does not force the later idle letter. Insertion sort on the reversed input has N active letters and no idle ones. Replace with: "On a deletion-proof word, the reversed input swaps each pair once. Delete the swap of v+1 with v. The runs differ by renaming v and v+1 until a later letter meets that pair; otherwise the deleted word ends unsorted. That letter is idle in the intact run. Distinct pairs use distinct letters."
Q8: REFUTED — counts `7144675968` agree in `solver-a/out_n7_m0.txt:29`, `solver-b/run7dom.txt:3`, `verifier/out_dp7.txt:3`; the n = 7 pruning sentence matches unpruned agreement only through n = 6 (`verifier/out_dp.txt:19-22`) and dom = 1 at n = 7 (`verifier/out_dp7.txt:1`); `prior-art.md:22-24,36-39` is the same hedge as `README.md:72-78` — none

F: MODERATE — `solver-b/proofs.txt:39-45` — L2(c) is false: `[1,4,5,2,6]` is 2-low, and `I_6` on `y = 3` yields `[1,2,4,5,3,6]`, neither sorted nor 2-low — high — replace the `y > x` case; the same `x` need not survive
F: MODERATE — `README.md:61-66` — idle-letter existence is asserted without deletion-proofness; length-N insertion sort is a counterexample — high — use the Q7 replacement
F: LOW — `solver-a/proofs.txt:49-53` — (i) skips `x = 1^m`, and (iii) never says a sorted vector is idle — high — add the Q4 sentence
F: LOW — `verifier/lemmas.py:150-154` — `xlow` is existential, so the 0 failures in `out_lemmas.txt:50-54` do not test L2(c) — high — track the same displaced value
F: LOW — `solver-a/proofs.txt:17-18` — (d) is proved only on 0-1 inputs; the lift is L0 at line 8, which B states and A does not — high — add "hence all inputs, by L0"
F: LOW — `solver-a/proofs.txt:42` — "`q_v` are distinct" has no sentence; one letter holds one pair — high — add that clause
F: LOW — `solver-b/proofs.txt:108` — count equality is written "n = 3..6" while `run7dom.txt:3` and `:13` both give `7144675968` — high — write n = 3..7
F: LOW — `solver-b/proofs.txt:5` — cited `slow.py 4 9 up` has no saved stdout — high — cite `verifier/out_lemmas.txt:39`
F: LOW — `README.md:39` — "exhaustively" for n = 7 sits above the admission that dominance pruning was not rerun unpruned there (`README.md:82-84`) — medium — say "exhaustive under dominance pruning"

Unasked, and the serious one: B's second upper-bound proof does not work. L2(c) fails on the counterexample above, so `solver-b/proofs.txt:48-51` does not prove `W_n` is deletion-proof. The construction is still deletion-proof in the checked range `solver-a/out_check.txt:1-10` (n ≤ 12), and A's T3 still proves it once the all-ones case is added. T2, T1, and L5 do establish the matching lower bound, so the headline equality is not false.
