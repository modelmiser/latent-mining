<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED. I checked (a)–(d) at solver-b/proofs.txt:15-26 against chain/3-question.txt:12-13. (a) The schedule p,p,p,… leaves p active and moving, and nobody else is ever active. (b) Safety covers every schedule, so "run p alone into crit and leave it there, then run q alone" is a legitimate schedule prefix. (c) j, parked mid-protocol, does no harm: safety does not care who is active, and before its add j has left R at 0. (d) holds. — none

Q2: CONFIRMED. Since 0 ∈ E_k and a_j ≠ 0, a_j = −e_i would put e_i + a_j = 0 into E_k, so a_j = e_i. Then e_i + a_j = −e_i, and both nonzero values are excluded from E_k, so E_k = {0} (proofs.txt:40-41). Multiplying by 2 is an automorphism of Z_3, so the "×2, re-index read branches" map works on whole runs. Exit = add 2 is forced: a read at crit or any other constant would leave an all-rem configuration with R = 1 or 2, which L1(d) forbids (proofs.txt:44-45). — Say explicitly that a read at crit is excluded by the same argument.

Q3: CONFIRMED. In (i), R reaching 0 while h is in crit lets q run sigma_q(0) into crit. In (iii) the counter goes 0 →(p) 1 →(h) 2 → reads → (p adds 2) 1. At that point h sits at F_0(h) with R = 1, exactly as in sigma_h(0); it enters crit at 1 and exits to 0, while q stays in rem. In (iv), a = 2 would give R = 0 with h in crit (proofs.txt:53-71). — none

Q4: CONFIRMED. Every helper and q construction (proofs.txt:75-79) is driven only by R, and p contributes 0 and takes no steps. Step 4: p and q are active and move forever, h is never active, and nobody is in crit (by (i)). Step 5: the segments run one after another, p then q, so it does not matter how many steps each level takes. Both chains are infinite, so both have every level m. A crit before rho's first nonzero add is impossible, because R would be 0 there. Step 6 is exhaustive: at each level (ii) gives hold or yield, and after a yield (iv) always yields the next level. — Step 5: replace "lockstep" with "level by level, segments run one after the other".

Q5: CONFIRMED (no break found). Extra reads see a constant value, so they are absorbed. add 0 is not a "nonzero add" and is absorbed too. Unreachable states never enter any run. Example: Burns-style 1,1,2 weights reduced mod 3 make process 3's first add 2 ≠ 1, which Step 1 already rules out. Flag-like hold automata (verifier's pool of 940) deadlock as in Step 4. — none

Q6: CONFIRMED. vcheck.py:156-168 tests safety on every reachable configuration. vcheck.py:169-189 tests progress exactly: for each active set M, it looks for a crit-free SCC with that active set in which every member of M has a step inside the SCC. Structural rules are at :87-92. When I ran it, w_n3_b4_s7 gave OK with 182 configurations and states [7,7,7]; w_n3_b4_burns gave OK with 104 and [3,7,6]. Each solver checker accepts only its own author's format (checker.py crashes on solver B's file, slowcheck.py on solver A's). — Brief C3 should read "each accepted by its author's checker(s) and vcheck.py" (the README already says this).

Q7: REFUTED. README.md:94 says the decoding "is not written out by either solver". But solver-a/proofs.txt:102-103 gives it ("sums 0,1,2,3 are distinct…"), and solver-b/proofs.txt:106-110 proves the decoding for weights 1,1,2,…, which for n = 3 is exactly 1,1,2 mod 4. The section on three values matches the proof, apart from two loose phrases: "reading 2" (:71) and "reads 2 forever" (:80). The process may also add 0. — "Each solver gives a short decoding argument; correctness of the whole algorithm rests on model checking."

Q8: REFUTED. Details are in F lines 2–3. The other cells match their sources: 569,600 = 312,000 + 249,600 + 8,000; 104/182; and the prior-art wording. The top-level README.md:49 row is consistent with the folder. — see F

F: MODERATE — README.md:94 — The claim that the decoding is "not written out by either solver" is false (solver-a/proofs.txt:102-103; solver-b/proofs.txt:106-110) — high — use the wording in Q7.
F: MODERATE — README.md:47 — "checked by program for n ≤ 5" is wrong for solver A's 2^n − 1 bound. Its only checked instances are b = 7 (n = 3) and b = 15 (n = 4) (reports/solver-a.md:133; solver-a has no n = 5 b = 31 file). Solver B's bound does have n = 2..5 (w_n5_b16_s7.txt). The referee's wording (referee.md:29) is just as loose — high — "B's checked for n = 2–5; A's for n = 3, 4".
F: MODERATE — README.md:45 — "computed by exhaustive search by the solvers" is wrong for n = 3, b = 4, s = 4: solver A timed out (solver-a.md:104) and solver B left no log (referee.md:46). "One with 7" is a witness, not a search result — high — "no solution with 4 states (solver B, no log; recomputed by the referee); a 7-state witness exists".
F: LOW — README.md:112-113 — "no search … addresses four processes with b between 4 and 7" is too strong. By L0, the s = 3 result "no two-process solution for b ≤ 8" (solver-b.md:185; referee.md:23) already rules out n = 4, s = 3 — medium — "except the trivial s = 3 case".
F: LOW — README.md:40 — The "Where" cell omits solver-a/proofs.txt Theorem 1 — high — add it.
F: LOW — README.md:71,80 — "reading 2" / "reads 2 forever": the process may also take add 0 steps, so "reads" is too narrow — high — "sees R = 2 / never changes R".
F: LOW — README.md:89-92 — "Same construction, lowering subtracts" does not fit solver A's witness, which is a hand-written variant: its process 1 never lowers except at exit — medium — "both solvers use variants of…".
F: LOW — referee.md:37 / README.md:39 — The 8,000 hand-built trials (verifier/chains.py) have no saved log; only the 561,600 are logged — medium — note this.
F: LOW — solver-b/proofs.txt:44-45 — The proof does not explicitly rule out crit being a read, although it is forced — high — add one clause.

Other points the questions did not ask about:
- The b = 3 proof also depends on L1(d) being applied at all-rem configurations that the pumping step creates. I checked this and it holds.
- vcheck.py lets s go unchecked in solver B's format (s = None at :59). This does no harm here because the state counts are printed.
