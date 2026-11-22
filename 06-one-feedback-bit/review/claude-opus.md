<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED. The 13 lines in `solver-a/wit_6_2.txt:2-14` are the same as the README listing (README:69-72). I built every O(u,a,b) with my own script, straight from the definition at README:25, and no two sets overlap. I found A(5) = 6 two ways: an exact branch-and-bound search of my own (no LP), and both certificates. `cert_5_0.txt` totals 6 and passes `checkcert.py`. `cert_n5_t0.txt` totals 6000008 out of 7·10⁶ and passes `certcheck.py`. `wit_5_0.txt` gives the matching lower bound. — none

Q2: CONFIRMED. The formal form matches the prose (chain/3-question.txt:1-5). If the deletion falls after bit t, the sender was told "no" and sent a, so the receiver sees u a′. If it falls inside u, the sender sent b, so the receiver sees u′b. A deletion can't land in b, because b is only sent after one has already happened. "At most one" is covered because ua itself is in O. When t = 0 the prefix u is empty, D(u) is empty, so O = {a} ∪ D(a). When t = n there is nothing after the feedback, so O = {u} ∪ D(u). The receiver can decode with zero error exactly when the outcome sets are pairwise disjoint. — none

Q3: CONFIRMED. `solver-a/proofs.txt:6-12`: ua is the only length-n outcome. If two strategies share (u,a), their S sets share the non-empty set uD(a) whenever t < n. Dominance pruning is sound: if S ⊆ S′ then S can stand in for S′, and S can't already be in the packing because it is non-empty and meets S′. — none

Q4: CONFIRMED. P4 (`a:24-28`, `b:12-14`): with t = 1, S = uD(a) ∪ {b}, and choosing b = u a′ gives S = uD(a). P2/P5 (`a:16-19`, `b:16-18`): {u} ∪ D(u)b = D(ub), and S doesn't depend on a. — none

Q5: CONFIRMED for solver A (`a:30-35`). A common element must be u[1..t−1]·b = u a′, which forces b = u_t a′. Brute-force checks agree: `p5check.py:16-18`, and `verifier/lemmas.log:1` (347,480 cases). Solver B's P7 (`b:25-28`) only proves "attained by", not the converse. — README:42: "proved by hand twice (the 'exactly when' direction by solver A only)"

Q6: CONFIRMED. B's P6 holds because c·D(u′) ⊆ D(u) (`b:20-22`). A's P6 counting is right; the Hoeffding bound gives (1+o(1))·n⁻², not n⁻², and the conclusion still stands. — none (the README already discloses the constant)

Q7: CONFIRMED. `checkcert.py:22-31` loops over every (u,a,b), uses exact Fractions, and takes the floor. `certcheck.py:49-59` uses exact integers and requires total < (F+1)·K. Since disjoint O sets imply disjoint S sets, weak duality applies. I re-ran all certificates for n = 3, 4, 5 at bound 2A(n−1), plus those for n = 6, 7, 8 (A's checker at their own floors); all pass. Together with the witnesses for A(2), A(3), A(4), C2 follows. — none

Q8: REFUTED (minor points only; the headline stands). Details are in F2–F6 below. The arithmetic 3+4+2+4 = 13 is correct (prefixes 00, 01, 10, 11). The prior-art paragraph matches `prior-art.md:7-18, 50-51`.

F: MODERATE — README:42 — The "exactly when" part is labelled "proved by hand, twice", but solver B's P7 (`solver-b/proofs.txt:25`) proves only that the minimum is attained. Only A proves the converse, and only A's script (p5check.py) and the referee's lemmas.py check it (n ≤ 9); B's misc.py checks only the minimum (n ≤ 8). — high — fix: "proved by hand by solver A; solver B proved the minimum only".
F: LOW — README:83-84 — "0011 and 1100 become usable behind the prefixes 01, 10 and 11" doesn't match the witness. 1100 also follows 00 (`wit_6_2.txt:3`), and 0011 never follows 10 (only lines 6 and 12, under 01 and 11). — high — fix: "1100 is used behind all four prefixes, 0011 behind 01 and 11".
F: LOW — README:76-79 — The mechanism sentence ("b-words chosen so a deletion inside the prefix lands on unused outcomes") just restates disjointness; nothing in the folder analyses why 13 is possible. — medium — fix: label it "an informal description, not an explanation".
F: LOW — README:12, 43 — "a script of mine" and "(same four)": no such script is in the folder. Also, the solvers established A(5) ≤ 6 with LP certificates, not exhaustive search (`a:49-50`, `b:31`). — high — fix: "A(5) = 6 by exact-arithmetic dual certificates (both solvers) and the referee's exhaustive search"; drop "mine" or add the script.
F: LOW — README:47 — The Claim column states "F(9,3) = 67" as a fact while the Status column says "not established". The referee's F(9,7) = 60 (referee.md:22) is left out. — high — fix: "F(9,3) ≥ 67 and F(9,4) ≥ 67, with search bounds of 67 and 68 that are not certified".
F: LOW — README:9-10 — "from there the best moment drifts towards the middle" generalises from n ≤ 8; n = 9 has lower bounds only. — medium — fix: "for n = 6 to 8 the best t moves from 2 to 3–4".
F: LOW — `solver-a/wit_6_2.txt:1` — The header says "dualbound=13.0", but the LP bound is 14 (`log_lp.txt:18`); the 13 is the MILP bound. A reader could think a tight dual certificate exists for (6,2). — medium — fix: rename the field to "milp_bound".
F: LOW — README:95-96 — "Both solvers used the same solver library for those" leaves out solver A's independent C++ search `bb2.cpp`, which agrees on all of n = 6 (`reports/solver-a.md:25`). — medium — fix: mention it.
F: LOW — `solver-b/proofs.txt:20` — The corollary F(n,t) ≤ 2^t·A(n−t) relies on A(0) = 1 and F(1,·), both outside n ≥ 2. The referee flagged this as harmless. — high — fix: state the convention A(0) = 1.
F: LOW — `solver-a/proofs.txt:41` — The n⁻² constant should be (1+o(1))·n⁻²; the README already discloses this. — high — fix: as the referee says.

Other things the questions didn't ask about:
- README:45 says "two solvers and the referee agree on every entry", but for n = 7 at t = 1, 2, 5 and much of n = 8, the referee only re-checked the solvers' certificates rather than computing the values. That is still a valid check, but it isn't three independent computations.
- I recomputed F(n,t) for n ≤ 6 with my own exact search, and every entry matches the README table. I checked all solver-A witnesses for n ≤ 9 at their claimed sizes; none has a conflict.
- I did not independently re-derive the n = 7–8 bounds that rest on search only: (7,0), (7,3), (7,4), (7,6), (7,7), (8,1), (8,2), (8,4). Those rest on the referee's logs.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
