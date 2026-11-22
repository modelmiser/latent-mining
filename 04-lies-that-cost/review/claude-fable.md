<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Verdicts**

Q1: CONFIRMED — `solver-a/proofs.txt:9-18`, `solver-b/proofs.txt:8-21`. (i) forces L uniquely; adding t to L only adds the condition t ∉ ∪ windows ⇔ r_x(t−1)=0; windows look forward so s=t's own window is vacuous; ell_t counts only C_{t−1} (`:12`, `:16`); ell=0 lie → r=0. Blocked: r ≥ j with j left means D ≥ t+j, window ⊇ {t+1..t+j}; r<j leaves t+r+1 lie-able. No off-by-one. — none

Q2: CONFIRMED — `open-upper/proofs.txt:18-26`. Re-ask only while alive set meets both sides ⇒ ell ≥ 1; k·ell ≥ 2 covers the remaining ≤2 askings; prior obligations only forbid more lies; majority of 3 with ≤1 lie is true; ⌈log2⌈m/2⌉⌉=⌈log2 m⌉−1 (m≥2). Referee re-tested exhaustively m≤7 (`run2.txt:59-64`). — none

Q3: CONFIRMED — `proofs.txt:37-51`. I re-derived: diff=(a1−a2)(j−1)+(b1−b2); odd a: b ≥ j−1 from 2b+1 ≥ m ≥ 2j−2, d∈{j−1,j−2}, d<0 only at j=1 which forces (1,0) (`:46`); 2·ell=m−|d−1| ≥ j for j≥3, ≥1 for j=2. (ii): liar obligation k·ell ≥ 2ell ≥ j−1 = questions left after, exactly "blocked" (obligation ≥ questions left, `:8-9`). (iii) a2 ≥ a1−1, a1 ≥ a2. My own script: 43,066,705 states j≤15, 0 failures. — none

Q4: CONFIRMED — `proofs.txt:70-76`. Cases m≥2j−2 / m≤2j−3∧j≤14 / m≤2j−3∧j≥15 partition; j≤2 gives 2j−3≤1 so only case 1 applies. I recomputed 3⌈log2(2j−3)⌉≤j: false at j=14, true for 15..39; induction j→j+3 fine. Empty outcomes are illegal answers (`chain/3-question.txt:8`), both solvers skip them (`table.cpp:29`, `ref.cpp:55-56`). — none

Q5: CONFIRMED (claim) — T1 stated `proofs.txt:55-58`; I count 960 states/k, 1884 for J=16,EX=3 (`table_out.txt:1-15`). Reduction: after any question at j≤14 at most 13 remain, k·ell ≥ 13 for ell≥1, 0 for ell=0 (`run1.txt:35` trans test). But `run1.txt:1-13` starts at k=4: k=2,3 lines are absent from the cited log. I compiled `ref.cpp` and ran t1 for k=2,3,13: 960 states, 0 lost each. — "run1.txt covers k=4…14,20,1000; k=2,3 by table.cpp and verifier/ms (refcmp_out.txt)"

Q6: CONFIRMED — `proofs.txt:87-99`. n(n+2)≤2^{n+1} (8≤8, step n²≥3) ⇒ P(even n)≤n+1; n²+2n−1≤2^n from n=7 (62≤128, step n²≥4) ⇒ P(odd n≥7)≤n. Yes-weight ((n+1)/2)q+(n−1)/2=(n(q+1)+(q−1))/2 matches odd-n P; no-branch smaller by q−1; even: n(q+1)/2. P(3)=5, P(5)=6; k·1≥4 iff k≥4, 2k≥5 iff k≥3; T2 roots in `table_out.txt:24-28`, reconfirmed by my `ref root 2 60`. — none

Q7: CONFIRMED — `solver-a/proofs.txt:21-38`, `solver-b/proofs.txt:23-39`. B's Lemma 2 avoids determinacy (A's P2 uses it tacitly, referee noted `reports/referee.md:33`). Odd refinement: |(2x−n)(q−1)| ≥ q−1, heavier ≥ (n(q+1)+q−1)/2, must be ≤ 2^{q−1}. — none

Q8: CONFIRMED with LOW wording defects — status cells match `reports/referee.md:9-10,15-24,56`, `reports/solver-a.md:7,26`, `reports/solver-b.md:14`, `prior-art.md:5,31-33`; defects listed below. — see F lines

**Findings**

F: MODERATE — `README.md:46`, `reports/open-upper-referee.md:42` — cites `open-upper-referee/run1.txt` as second-program evidence for k=2…13, but the file contains no `t1 k=2`/`t1 k=3` lines (starts at k=4); the k=2,3 portion of "two programs" is unsupported by the saved log (true on rerun: 0 lost) — high — regenerate run1.txt or cite refcmp_out.txt/verifier ms for k=2,3

F: LOW — `README.md:57-58` — "more answers than remain in the game": Lemma S gives k·ell ≥ j−1, i.e. at least as many, equality allowed — high — "at least as many answers as remain"

F: LOW — `README.md:71-72` — "two candidates can be kept alive for ever" reads as all n for k=1; true only for n∈{2..7,9}, false for n=8 and 10..700 — medium — "…for ever when n is one of 2…7, 9"

F: LOW — `README.md:45` — "both outcomes are again such states": outcomes need not have ≥2(j−1)−2 candidates; "such" must mean nice with weight ≤2^{j−1} only — medium — "both outcomes are again nice with the weight bound at j−1"

F: LOW — `open-upper/proofs.txt:107` — "1310 states" per k conflicts with 960 (T1) and 1884 (table_out); third unexplained count (referee noted) — high — state which window gives 1310 or delete

F: LOW — `open-upper/proofs.txt:30-36` — Lemma S never states ell ≥ 1; true (at j=1 the only state is (0,2) with ell=1) and only cosmetically needed — high — add "ell ≥ 1 in all cases"

F: LOW — `README.md:80-82` — "checked against the literal definition only for n ≤ 4" understates: referee also ran 4,000 random histories n≤8 and literal game n≤5 (`reports/referee.md:10-13`) — high — "exhaustively for n ≤ 4, by sampling to n ≤ 8"

**Not asked**

- Positive, unstated: a "win" verdict in `table.cpp`/`ref.cpp` never depends on the weight-pruning (pruning only returns false, `table.cpp:38`, `ref.cpp:42`), so T1 does not rest on Lemma 4/P3′. Worth saying in `proofs.txt:61`.
- `README.md:51` non-nice row: referee's 74,556 states (`referee.md:56`) are all (a,b), not only b<a−1; README's restriction is a correct sub-statement but the count/scope differs from the source.
- Both T1 programs implement the same abstraction and share the author's reading (ell=0 allowed, dead elements ignored); the only definition-level checks are n≤5. Acknowledged in README, but the headline's dependence on P1 at j≤14 states with intermediate obligations (not just 0/blocked) is never literal-checked.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
