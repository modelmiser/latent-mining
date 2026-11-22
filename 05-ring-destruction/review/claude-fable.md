<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Verdicts**

Q1: CONFIRMED — `solver-a/proofs.txt:11-41`. I re-derived: b_{i+1}(s) ≤ b_i(s−1)+1 by induction gives the left end of K_{t−1}(i+1) ≥ i−b_i(t−1), so the union is an arc with extents (b_i(t−1), max(a_i(t−1), a_{i+1}(t−1)+1)); complete iff a+b ≥ n−1 ⇔ not complete iff d+e ≥ 2t−n+2 (line 25). The piece recursion d_i(t)=min(d_i(t−1)+1, d_{i+1}(t−1)) → v_p(t)=min(v_p(t−1), v_{p−1}(t−1)+1) is right (line 31). L2: piece p round t and piece p−1 round t−1 both hit the A-link into p−t; B: q+t for both (line 40-41). — none

Q2: CONFIRMED — `solver-a/proofs.txt:43-52`. (i) one destruction = one event on one piece = ≤1 new pair; (ii) first attainment of y is via an event with v_{p−1}(s−1)=y−1; (iii) induction with L2 forces the (p−1,y−1) creation ≤ s−2, so s ≥ 2y−1; t odd ⇒ H=(t+1)/2 needs round 2H−1=t, ≤ k destructions. — none

Q3: CONFIRMED — `solver-a/proofs.txt:79-86`. Closure walks down a diagonal, so P_A∩diag c = {(c+y,y): y ≤ h_c}. f(x) ≡ x+t−c (mod n) with f ∈ [1,H] ⊂ [1,n−1] forces distinct f on one diagonal; least f ≤ h_c−δ_c+1; (*) gives (1). Σδ_c = n because f ≥ 1 everywhere (needs D′ ≥ H, line 77). — none

Q4: CONFIRMED — `solver-a/proofs.txt:87-99`. Even: w = n−t−1, n ≤ |P_A| − s·m_A, both directions summed with L3(i) gives (E). Odd: g ≤ H1+β, h ≤ H1+ε give both bounds; E ≤ k by L3; B ≤ E since x*_c are distinct positions each carrying a distinct B-pair of value H (and vice versa). (O) follows. — none

Q5: CONFIRMED — `solver-a/proofs.txt:103-113`. Redone: even s = ((k−3)q+2−r)/2 ≥ 0 (k=3 ⇒ r ≤ 2), w=q−1, ⌈n/w⌉ ≥ k+1, residual q(k−3)+2k+2−3r = (q−1)(k−3)+2+3(k−1−r) > 0. Odd: D′ ≥ H ⇔ (k−3)q+1 ≥ r; k=3,r=2 gives t0=2q−2 even; q=2 ⇒ w=0 ⇒ 2n ≤ 2k contradiction; q ≥ 3: RHS of (O) is decreasing in B and E so min at B=E=k; residual q(k−3)+4k+3−5r ≥ 2k−1 (k ≥ 4) or 15−5r ≥ 10 (k=3). My own implementation of (E)/(O) excludes t0 and never t0−1 for 3 ≤ k ≤ 24, k < n ≤ 260. Proved for all n > k ≥ 3. — none

Q6: CONFIRMED — `solver-a/proofs.txt:57-70`. m ≤ k links, distinct positions; A-links odd rounds, B-links even, and an A-link never equals a B-link for n ≥ 3. β_c − α_c = 1 so intervals [β_c, α_{c′}] partition the ring; d+e ≥ J+J′−(α_{c′}−β_c) ≥ T−δ+1 = 2T−n+2. My own simulator of the written strategy gives first completion exactly n−⌈n/min(k,n)⌉ for 3 ≤ n ≤ 44, 2 ≤ k ≤ 2n+1 (k=1 gives the trivial bound 0). — none

Q7: CONFIRMED — `solver-a/proofs.txt:117-124` (odd: n pairs vs ≤ n−1; even: |P| = n forces a descending chain to value 1; the unstated symmetric case "all g>0" is trivial). `solver-b/proofs.txt:55-85`: Lemma Φ (needs k<n), Theorem U and U′ re-derived; T even gives T ≤ n−1−n/k, odd gives n−T > n/k. — none

Q8: CONFIRMED with wording defects — Status cells match `reports/referee.md:11-25`; table matches `verifier/table_small.txt`, `verifier/out_s1.txt`, `out_s2.txt` (NO at T=F for n=9..12, k=3,4), `solver-a/log_p2.txt:242` (12,1), witgen first completions; prior-art paragraph matches `prior-art.md:30,47-48`. Explanatory prose has hand-waves listed below. — see F lines

**Findings**

F: MODERATE — `README.md:74-75` — "Each destruction creates at most one new unit of delay" is false: an event sets v_p = v_{p−1}+1, which can jump by several units (solver A says so itself, `reports/solver-a.md:55`). What is bounded is the number of (piece, value) pairs. — high — "Each destruction creates at most one new (piece, value) pair, so after t rounds there are at most kt pairs."
F: LOW — `README.md:65-67` — "add up to the same total"/"same margin": Theorem L proves only d+e ≥ 2T−n+2; equality is neither shown nor needed. — high — "add up to at least the required total".
F: LOW — `README.md:78-79` — "must add up to a fixed amount" → "to at least 2t−n+2". — high
F: LOW — `README.md:81-83` — top-value pairs are created only in the final round only when t is odd (`proofs.txt:51-52`). — high — add "when t is odd".
F: LOW — `README.md:69-70` — k ≤ 2 explanation ("too few chains … no delay worth having") is not the proof, which is a pair count (`proofs.txt:118-124`). — medium — "with k ≤ 2 the adversary makes ≤ n destructions by round ⌊n/2⌋, too few to give every process a delay".
F: LOW — `README.md:39` — B's bound stated "for k ≥ 3" without n > k; Lemma Φ (`solver-b/proofs.txt:55`) requires k < n. — high — add "n > k".
F: LOW — `README.md:36` — "proved by hand, twice": B's Theorem L assumes T ≥ g (`solver-b/proofs.txt:90`, fails at (4,3)) and the referee calls it "outline only", unread (`referee.md:16,45`). — high — "proved by hand by A (B gives the same strategy, read by nobody, under n ≥ 5)".
F: LOW — `README.md:12` — "exhaustive search agrees for n ≤ 12" holds only for k ≤ 4 (k ≤ 8 for n ≤ 8). — high
F: LOW — `README.md:8,66-67` — "until round n−⌈n/k⌉" is ambiguous; nobody is complete through round n−⌈n/k⌉−1 and someone is at n−⌈n/k⌉. — medium
F: LOW — `solver-a/proofs.txt:123` — "Take x with f(x)≥2": existence of some x with f>0 not stated (else apply the mirror argument to g). — high
F: LOW — `solver-a/proofs.txt:127-128`, `README.md:42` — even n, k ≥ 2n is actually proved there (all links on odd rounds + statement cap), yet tagged NOT PROVED / "computed". Understatement. — medium
F: LOW — `README.md:98-99` — referee item 8 (`referee.md:25`) I traced: d=e=n/2 at every position forces 2n top-value pairs all created in round n−1; correct. The "unreviewed" caveat can now be dropped. — high
F: LOW — `README.md:108-109` — "one paper … could not be opened": `prior-art.md:48` lists three unopened (Chlebus–Diks–Pelc, Dobrev et al., Godard–Peters). — high

**Not asked:** nothing further of substance; the headline C3/C4 holds under my independent trace and numeric checks.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
