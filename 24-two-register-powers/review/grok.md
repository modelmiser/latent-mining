<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — Cases A, B (r≤1/2 and r>1/2), C, D all give ΔΦ≤1, ≤1/4, ≤3/4, ≤3/4, ≤3/4; at x=y the four labels coincide and still obey those caps; (*) holds from Φ(start)=0 and −1/2≤f≤0 (`solver-b/proofs.txt:18-34`) — none

Q2: CONFIRMED — n>X and G(n)=j≤t forces k<4(j−log₂X)+2≤4ηt+2; Σ_{k≤K} C(t,k)3^k≤(K+1)2^{tH(K/t)}3^K for K/t<1/2; a type-word on the ordered pair determines the state, so counting words only overcounts; ≤2 integers per word; H(0.176)+0.176·log₂3=0.95020; m≤N(D(m)) because the m residues need m distinct positive integers of cost ≤D(m) (`solver-b/proofs.txt:10-11,35-41`) — none

Q3: CONFIRMED — P4’s addition induction and the 2^k scaling hold; A and B commute and the state depends only on the addition word plus (#A,#B) in each of the j+1 gaps, hence 2^j C(k+2j+1,2j+1); the max-min for γ matches those two bounds and evaluates to 0.963096 at d=0.1207 (`solver-a/proofs.txt:15-37`, `solver-a/analysis.txt:38`) — none

Q4: CONFIRMED — Easy bounds and the mod-m lift, including residue 0 via n=m, are proved (`solver-a/proofs.txt:6-13`, `solver-b/proofs.txt:3-13`). Every ordinary chain length is log₂n+o(log n), so the unlimited-memory worst-case ratio is 1 — none

Q5: REFUTED — N(0..5)=1,2,4,7,12,20 and D(2..8)=1,2,2,3,3,4,4 match a hand BFS and `solver-a/nt22.txt:1-6`, `solver-a/D_all.txt:1-7`; A to t=22, B to t=21, referee to t=22 agree. B’s D table stops at 4096 (`solver-b/D_table.txt:4095`, `reports/solver-b.md:15`), so “both solvers, every m≤4100” (`README.md:40`) is false — “D(m) for m=2..4100 by solver A and the referee (two ways); solver B through 4096; they agree on the overlap.”

Q6: CONFIRMED — Lines 49–65 match P2’s Φ, the 1 and 3/4 caps, (*), η=0.044, 0.9502, and the factor 2 (`README.md:49-65`). Lines 67–76 add the “full bit / no choice” gloss and a correct sketch of A’s P4–P5; they do not contradict P2 — none

Q7: REFUTED — The D(m)≤4100 “both solvers” cell is false (Q5). The other status cells match the reports. Distrust bullets on the factor 2, t≈9500, Erdős/Knuth/OEIS, the 1.1–1.2 guess, octave minima, `potential_check.py`, the powers-of-two error, and `rm -f /dev/null` match `reports/referee.md:14-18,38-44`, `prior-art.md:7-18,36`, and `reports/solver-a.md:28,73`. “Did not find L>1” does not outrun `prior-art.md:14`, and Knuth unread is disclosed (`README.md:95-96`). The “test copy” over 300 kB (`README.md:111-112`) is not in those sources; only `paths_m400.txt` is (`reports/solver-a.md:37`, `reports/referee.md:30`) — “One omitted file: solver A’s paths_m400.txt (80199 lines); the referee accepted it.”

Q8: CONFIRMED — The top-level row matches the model, L≥1/0.956>1.046 with liminf≤2, two hand proofs, and an open constant (`README.md:52`, `24-two-register-powers/README.md:36-43`) — none

F: MODERATE — `24-two-register-powers/README.md:40` — “both solvers” computed D(m) for every m≤4100, but solver B stops at 4096 — high — attribute 4097..4100 to A and the referee only
F: MODERATE — `solver-b/proofs.txt:15` — the header states N(t)≤2^{0.956 t}; the proof only reaches 2·2^{0.956 t} (`solver-b/proofs.txt:41`) — high — put the factor 2 in the header
F: MODERATE — `24-two-register-powers/README.md:16-18` — Erdős’s almost-all theorem (`prior-art.md:7`) does not prove the worst-case unlimited-memory ratio is 1; that needs l(n)=log₂n+o(log n) for every n — medium — cite a uniform bound
F: MODERATE — `solver-b/potential_check.py:24` — `assert abs(phi-P)<1e-6 or True` is always true, and the recursive call never updates P (`potential_check.py:30`) — high — delete `or True` and pass the updated potential
F: LOW — `24-two-register-powers/README.md:111-112` — a second omitted file, “a test copy” over 300 kB, is not documented; the reports name only paths_m400.txt — medium — drop the test-copy sentence
F: LOW — `24-two-register-powers/README.md:53-55` — “four cases” skips the split of type B into r≤1/2 and r>1/2 (`solver-b/proofs.txt:25-27`) — high — “four types, five calculations”
F: LOW — `24-two-register-powers/README.md:67-68` — “gains less on average” is a uniform ≤3/4 cap, not an average (`solver-b/proofs.txt:23`) — medium — “raises Φ by at most 3/4”
F: LOW — `README.md:52` — “tables computed three times” fails for N(22) and for D(m), m=4097..4100, which B did not compute — medium — “N(t) three times through 21; D(m) three times through 4096”
