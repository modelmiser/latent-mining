<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Q1: CONFIRMED** — `solver-a/proofs.txt:10-18`. Traced the three cases x_i+x_{i+1}∈{0,1,2}: new s_i = s_{i-1}+min(x_i,x_{i+1}) = max(s_{i-1}, s_{i+1}−1) in each; old s_i ≥ both arguments; max is monotone in both. 1^k0^(n−k) has s_j=min(j,k), the maximum; it is f_t of the reversed input (L0). (d) follows as written. — none

**Q2: CONFIRMED** — `solver-a/proofs.txt:20-27`. (⇐) slip = deletion + insertion, L1. (⇒) n≥3 gives ≥2 letters so every position has a slip; one fixed slip w^ is used for all x. Active ⇒ (y_i,y_{i+1})=(1,0); c_{i+1} sees (0,·), c_{i−1} sees (·,1): idle in both; i=1 uses only the first case, i=n−1 only the second. Idle ⇒ w− = w on x. Then L0. — none

**Q3: CONFIRMED** — `solver-a/proofs.txt:31-43`. Relative order of a pair changes only at a comparator on exactly that pair, which sorts it, so each initially inverted pair is exchanged exactly once. φ-relation persists because no value lies strictly between v and v+1. q_v exists else w− ends at φ(sorted); q_v is idle in w and its wires hold {v,v+1}, so distinct v give distinct positions, none active. — none

**Q4: CONFIRMED (with the one trivial gap)** — `solver-a/proofs.txt:45-60`. Traced (i) both y, including d=0 (empty ranges), (ii) j≤a and j>a, (iii), and all three deletion locations. The only uncovered case is x=1^m in (i) (`:53` "x sorted is the case c=0" needs ≥1 zero); y=0 then climbs to wire 1, y=1 trivial. — add to :53 "(x = 1^m: y=1 idle; y=0 climbs to wire 1)".

**Q5: CONFIRMED** — L5 `solver-b/proofs.txt:101-104`: classification `:72-75` correct; deleting a non-trajectory letter leaves t_k = first c_k after t_{k−1}; deleting t_{n−1} forces a c_{n−1} after t_{n−1}, read with M on wire n: wasted. A(1)=0 ⇒ A(n) ≥ n(n+1)/2−1. Independent of T2 (induction on max trajectory vs. idle-letter count). L4 `:62-99`: traced (1) both cases incl. i+1=k−1, and (2) both orderings of a vs t_{n−1}. Base: `solver-b/runup.txt:5-6` (fast, reversed input + pruning) and `verifier/out_lemmas.txt:31-40` (exhaustive, perm-confirmed). The cited `slow.py 4 9 up` log (`:5`) is **not in the repo**. — ":5: cite runup.txt and verifier/out_lemmas.txt; the slow.py run was not saved".

**Q6: CONFIRMED** — S=A (T1, n≥3) ≥ N+n−1 (T2) ≤ |W_n| (T3). n=2: S(2)=1, A(2)=2 (`verifier/out_lemmas.txt:8`); README never states a value for n=2 and scopes to n≥3 (`README.md:7,24,38`), though `README.md:9-10` is unqualified. — ":9 "For n ≥ 3, surviving a slip…"".

**Q7: CONFIRMED** — `README.md:47-52` matches T1 (active ⇒ slip ≡ deletion; idle ⇒ deletion ≡ original and insertion harmless). `:54-57`: u(3)=4 with 2112 (`runup.txt:1-2`, `out_lemmas.txt:30`); c_{n−1} has no up-slip. "last comparator" is ambiguous (alphabet, not position). `:59-66` matches T2. — ":56 "the letter c_(n−1) has no upward slip"".

**Q8: CONFIRMED with LOW overstatements** — "three independent programs" (`:39-40`): bfs.cpp, fast.cpp, dp.cpp all share one design (reversed-input image + one-slip image set + dominance) and one model family; counts match `out_n7_m0.txt:29`, `run7dom.txt:3`, `out_dp7.txt:3`. n=7 pruning remark `:82-84` matches `solver-a.md:28`, `solver-b.md:18`, `referee.md:18`. Prior-art paragraph hedges consistently with `prior-art.md:22,37,39`. `:34` "word by word for n ≤ 5" vs `referee.md:24` (n=5 only at size 14). — ":39 "three separately written programs of the same design"; :34 "n ≤ 4 at all lengths, n = 5 at size 14"".

F: LOW — `solver-b/proofs.txt:5` — cites `slow.py 4 9 up` as evidence for u(4)=9 but no slow.py output exists in `solver-b/`; base rests on fast (reversed input only) and the verifier's 0-1 enumeration — high — cite runup.txt:5-6 and verifier/out_lemmas.txt:31-40.
F: LOW — `solver-a/proofs.txt:53` — T3(i) omits x=1^m (needs a zero to be "c=0") — high — add the one-line case.
F: LOW — `README.md:36` — "resting on one computed base case" omits that n=3 in this route is the hand check L3, not L4 (which gives n≥4 only, `solver-b/proofs.txt:97`) — high — "resting on one computed base case (n = 4) and a hand check for n = 3".
F: LOW — `README.md:34` — "tested it word by word for n ≤ 5" overstates `referee.md:24` (n=5 only at size 14) — high — as above.
F: LOW — `README.md:39-40` — "independent" programs share design and model family; the README's own distrust section concedes the latter — medium — "separately written, same design".
F: LOW — `README.md:9-10` — "Surviving a slip … same as surviving the removal" unqualified; false at n=2 (`out_lemmas.txt:8`) — high — prefix "For n ≥ 3".
F: LOW — `README.md:56` — "the last comparator c_(n−1)" reads as positional — medium — "letters c_(n−1)".
F: LOW — `solver-b/proofs.txt:107` — stale "n = 3..6" vs `run7dom.txt:3,13` (already noted in referee.md:49) — high — "n = 3..7".
F: LOW — `solver-a/out_cross.txt:1-12` — paired runs are unlabeled, so "reduction off" vs "on" (`reports/solver-a.md:28`) cannot be told apart from the log — medium — label runs.

Not asked: the dominance pruning used for n=7 is justified by the proved L1, so the README's distrust item at `:82-84` is more cautious than needed; nothing substantive beyond the findings above. All headline claims C1–C6 hold as traced.
