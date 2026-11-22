<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — arc, survival, moving-piece recursions, and L2 indices follow at `solver-a/proofs.txt:11-41` — none  
Q2: CONFIRMED — pair accounting, closure, earliest round \(2y-1\), and odd-round top cap follow at `solver-a/proofs.txt:43-52` — none  
Q3: CONFIRMED — diagonals are initial segments, f-values are distinct, (1) follows, and the \(X_c\) partition gives \(\sum\delta_c=n\) at `solver-a/proofs.txt:76-85` — none  
Q4: CONFIRMED — independently deriving \(s,w\) gives (E), while occupied odd diagonals give \(E\le k\), \(B\le E\), and (O) at `solver-a/proofs.txt:87-99` — none  
Q5: CONFIRMED — both parity calculations, \(D'\ge H\), exceptional cases, and minimization at \(B=E=k\) are correct at `solver-a/proofs.txt:102-114` — none  
Q6: REFUTED — for \(k=1\), \(m=1,\delta=n,T=-1\), so “survives \(T\)” is undefined at `solver-a/proofs.txt:55-70`; all \(k\ge2\), including \(k\ge n\) and nondivisible cases, check out — “The construction proves the bound for \(k\ge2\); for \(k=1\), \(F\ge0\) is trivial.”  
Q7: CONFIRMED — both parities of C1 follow at `solver-a/proofs.txt:117-124` and `solver-b/proofs.txt:119-128`; B’s upper bound is valid on its stated \(n>k\ge3\) domain at `solver-b/proofs.txt:55-85` — “B proves \(F<n-n/k+1\) for \(n>k\ge3\).”  
Q8: REFUTED — README claims at `README.md:12,36,39,63-83,91-93` exceed or misdescribe the cited evidence — “Apply the scoped and inequality-based wording below.”

F: LOW — `solver-a/proofs.txt:18` — L0’s induction invokes \(s-2\) at \(s=1\) without stating the immediate base case — confidence: high — add the base case.

F: MODERATE — `solver-a/proofs.txt:55-70` — Theorem L’s proof is undefined for \(k=1\), although its bound is trivially true — confidence: high — restrict construction to \(k\ge2\).

F: LOW — `solver-a/proofs.txt:123-124` — the even C1 proof chooses positive \(f\) without handling empty f-support — confidence: high — add “otherwise exchange \(f,g\).”

F: LOW — `solver-b/proofs.txt:55-67` — Lemma Φ says merely “a function”; its telescoping identity requires integer-valued deficits — confidence: high — state \(A:\mathbb Z_n\to\mathbb Z_{\ge0}\).

F: MODERATE — `solver-b/proofs.txt:148-150` — \(F(10,4)\) is not B’s first unresolved entry; B already cannot exclude \(F(5,4)=4\) — confidence: high — name \(F(5,4)\).

F: LOW — `README.md:36` — “strategy found independently” ignores that both solvers received the opposing-ramp hint at `brief-solver.txt:20-27` — confidence: high — say “independently completed from the shared hint.”

F: MODERATE — `README.md:39` — attributes the unqualified \(k\ge3\) bound to B, whose proof requires \(k<n\) — confidence: high — add \(n>k\).

F: LOW — `README.md:12` — “exhaustive search agrees for \(n\le12\)” omits \(k\le4\); larger-k exhaustive data stops at \(n=8\) — confidence: high — state both ranges.

F: LOW — `README.md:63` — “never repeats a link” is false: rounds 1 and 5 repeat links in `solver-a/wit/w_8_4_T5.txt:2-6` — confidence: high — say “never in consecutive rounds.”

F: MODERATE — `README.md:65,78-79` — delays need not have equal totals: `solver-a/witgen/g_7_3.txt:1-4` yields sums \([2,2,2,1,1,2,2]\) — confidence: high — say “each sum is at least \(D\).”

F: MODERATE — `README.md:74-75` — current delay “units” can exceed \(kt\); that witness totals 12 with \(kt=9\) — confidence: high — say “at most \(kt\) historical piece-value pairs.”

F: LOW — `README.md:81-83` — top values are forced into the final round only when \(t\) is odd — confidence: high — add that condition.

F: LOW — `README.md:69-70` — “too few chains” is hand-waving, not the \(k\le2\) proof — confidence: high — cite the pair-count/closure contradiction.

F: LOW — `README.md:91-93` — witness files are not byte-identical; only normalized schedules match — confidence: high — replace accordingly.

No result-changing defect was found: the exact \(n>k\ge3\) formula and \(c(k)=1-1/k\) remain proved.
