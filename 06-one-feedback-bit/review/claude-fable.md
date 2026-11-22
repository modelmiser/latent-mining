<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Q1: CONFIRMED** — I recomputed all 13 outcome sets with my own code (and read them off: the 13 sets cover 31 of the 32 length‑5 words plus 13 distinct length‑6 words, no repeats; `solver-a/wit_6_2.txt:2-14`). Prefix counts 3/4/2/4. My own exhaustive search gives A(5)=6; `solver-a/cert_5_0.txt:2-13` is nonnegative, every D(w), |w|=5, has weight ≥1, total 6; checker logic `checkcert.py:22-30` is sound weak duality in `Fraction`. — none.

**Q2: CONFIRMED** — `README.md:16-25`, `chain/3-question.txt:1-5`. No deletion → ua; deletion in first t → "yes", b sent intact (at most one deletion), receiver sees D(u)b; deletion in a → "no", uD(a). Disjoint O ⇔ every observation identifies the message ⇔ zero error. t=0: D(ε)=∅, O={a}∪D(a); t=n: a=b=ε, O={u}∪D(u) (`solver-a/proofs.txt:17`). — none.

**Q3: CONFIRMED** — `solver-a/proofs.txt:6-13`. Same (u,a) ⇒ ua shared and, for t≤n−1, uD(a)≠∅ shared, so S-disjointness suffices; equal S from distinct strategies intersect (nonempty), so not both packable. Dominance replacement preserves disjointness; implementation dedups before pruning proper subsets (`verifier/ref3.cpp:30-32`), referee's `ref dp` uses no dominance for n≤6 (`reports/referee.md:9`). — none.

**Q4: CONFIRMED** — P4 `solver-a/proofs.txt:23-27`: S⊇uD(a) gives ≤A(n−1) per first bit; b=ua′ gives S=uD(a). P2/B‑P5 `solver-a/proofs.txt:18-22`, `solver-b/proofs.txt:17-19`: {u}∪D(u)b=D(ub) (delete last vs. earlier symbol), same ub ⇒ both contain u. — none.

**Q5: CONFIRMED** — `solver-a/proofs.txt:29-34`. A common word w=ua′=u′b forces u′=u[1..t−1] (length), then b₁=u_t, b[2..]=a′; conversely b=u_t a′ gives ua′∈D(u)b. So |S|=r(u)+r(a)−1 iff b∈u_t D(a). Solver B's P7 (`solver-b/proofs.txt:26-29`) proves only "attained by", not "exactly". — none.

**Q6: CONFIRMED** — B‑P6 `solver-b/proofs.txt:21-24`: cD(u′)⊆D(cu′) so c·S(u′,a,b)⊆S(u,a,b); stripped strategies distinct. A‑P6 `solver-a/proofs.txt:36-43`: distinct w=ua; |S|≥r(w)−1; Hoeffding at s=√(n ln n)−½ gives exp(−2s²/(n−1))=(1+o(1))n⁻², not ≤n⁻² (`verifier/lemmas.log:2`: 1.65n⁻² at n=9); only o(2^n/n) needed, so the conclusion stands. — replace "at most n^−2" with "at most (1+o(1))·n^−2".

**Q7: CONFIRMED** — `solver-a/checkcert.py:18-30`: weights ≥0, every (u,a,b) incl. t=0/t=n via `product(repeat=0)` has S‑weight ≥1, exact `Fraction`, k ≤ Σy ⇒ k ≤ floor. I re‑ran it: F(3,t)≤4, F(4,t)≤4, F(5,t)≤8 for all t; A(2)=2, A(3)=2, A(4)=4 lower bounds `wit_{2,3,4}_0.txt`. C2 follows. — none.

**Q8: CONFIRMED with LOW defects** — status cells match `reports/referee.md:7-34`; "five at n=7, three at n=8" without certificates matches the cert files present (7: t=1,2,5; 8: t=0,3,5,6,7,8); n=9 row matches `referee.md:19-23` (I re‑checked `wit_9_3` (67), `wit_8_0` (30), `cert_8_0` floor 30); prior‑art paragraph matches `prior-art.md:7-18,50-52`; "3+4+2+4" is right. Overstatements: see F‑lines below.

F: LOW — `README.md:83-84` — "0011 and 1100 become usable behind the prefixes 01, 10 and 11": in the witness 1100 also sits behind 00 (`wit_6_2.txt:3`), and 0011 only behind 01, 11; also one of 375,360 optimal packings, so the mechanism is illustrative, not established — high — "In this packing 1100 is used behind every prefix and 0011 behind 01 and 11; this is one of 375,360 optimal packings."

F: LOW — `README.md:43` — "A(5) = 6 by exhaustive search (same four)": solver A's exhaustive log stops at n=4 (`solver-a/log_slow5.txt`), its A(5)≤6 is by certificate; the README author's "script of mine" (`README.md:12,43`) is not in the folder — high — "A(5) ≤ 6 by certificate (both solvers) and exhaustive search (referee); my own check script is not included."

F: LOW — `README.md:42` — "reached exactly when … proved by hand, twice": the "exactly" direction is proved only by solver A (`solver-a/proofs.txt:29-34`); solver B's P7 states "attained by" (`solver-b/proofs.txt:26`) — high — "proved by hand (the 'exactly' part by one solver); brute-forced for n ≤ 9".

F: LOW — `README.md:79` — "the b-words can be chosen so that a deletion inside the prefix lands on outcomes no other strategy uses" restates disjointness; not an explanation of why t=2 beats t=1 — medium — mark as "a description of the packing, not a mechanism".

F: LOW — `README.md:10` — "from there the best moment drifts towards the middle" rests on n=7,8 only (n=9 is lower bounds) — medium — "for n = 7, 8 the best moment is t = 3 or 4".

F: LOW — `README.md:81-83` — "as happens at n = 5": the (5,2) optimum already uses non‑constant‑run a=110 (`solver-a/wit_5_2.txt:8`), so the chain's premise was already shaky at n=5; the README reports it without noting this — medium — add "(already not quite true at n = 5: one optimal packing uses a = 110)".

Not asked but notable: the n=7 entries t=0,6,7 and n=8 t=1 rest on A(7)=16 by the referee's exhaustive search only (no certificate in either solver folder); `README.md:45` ("certificate where one exists, otherwise exhaustive search") is accurate but a reader might assume A(7) is certified. Nothing CRITICAL: the headline (13 > 12) is fully traceable and holds.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
