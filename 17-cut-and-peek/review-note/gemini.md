<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `17-cut-and-peek/NOTE.md:47-63`, `17-cut-and-peek/prior-art-deep.md:10-14`; cyclic equivalence partitions inputs into the level sets of \(f\) for any non-constant Boolean function, making the relation equivalent to [2], while the encoding definition allows arbitrary card counts per bit and subsumes [2]'s 2-card commitments — none
Q2: CONFIRMED — `17-cut-and-peek/NOTE.md:66-137`, `17-cut-and-peek/solver-b/proofs.txt:43-64`, `17-cut-and-peek/reports/referee.md:19,34`; the affine reduction, DFT step, modulus-1 pair determination, exceptional-pair classification, \(r\cdot t\) identity, null-frequency consistency, and corner change all hold line by line without relying on \(\pm 1\) values or disjoint supports beyond the affine decomposition — none
Q3: CONFIRMED — `17-cut-and-peek/NOTE.md:140-175`, `17-cut-and-peek/prior-art-deep.md:12-18,33-35`; Theorem 2 follows directly from Lemma 1, all four examples correctly yield 7-to-1 restrictions, 7-to-1 functions are precisely conjunctions or negated conjunctions of 3 literals, and both unsettled limitations match the preprints — none
Q4: CONFIRMED — `17-cut-and-peek/NOTE.md:193-235`, `17-cut-and-peek/solver-b/proofs.txt:23-34,66-68,88-124`, `17-cut-and-peek/solver-a/w_n3_m36.txt:1-8`; Walsh-Fourier inversion and non-zero parity counts complete Proposition 3, further properties match T4/T5/C3/L1, and the owner, u0, and u1 vectors match `w_n3_m36.txt` character for character — none
Q5: CONFIRMED — `17-cut-and-peek/NOTE.md:9-40,248-280`, `17-cut-and-peek/prior-art-deep.md:4-28`; claims accurately distinguish SCFO from equalizability of specific words, AND from other function classes in [3], and random cuts from composite shuffles in [5], with literature status properly hedged — none
Q6: CONFIRMED — `17-cut-and-peek/NOTE.md:3-5,31-33,284-330`, `17-cut-and-peek/reports/referee.md:8-70`, `17-cut-and-peek/solver-b/proofs.txt:64`, `17-cut-and-peek/prior-art-deep.md:3-56`; all check logs, solver histories, frequency checks, and literature reviews match repo records, and AI origin without mathematician review is displayed prominently in four separate locations — none
Q7: CONFIRMED — `17-cut-and-peek/NOTE.md:26,102,132,148,217,238`; five localized terminology and scoping ambiguities were identified at lines 26, 132, 148, 217, and 238 — none

F: LOW — `17-cut-and-peek/NOTE.md:26` — "no card depends on two bits" literally permits cards depending on \(\ge 3\) bits, whereas the proof requires each card to depend on at most one bit — high — replace with "in which each card depends on at most one bit"
F: LOW — `17-cut-and-peek/NOTE.md:148` — "the AND of three literals or the negation of one" is syntactically ambiguous between negating one literal versus negating the 3-literal conjunction — high — replace with "the conjunction of three literals or the negation of such a conjunction"
F: LOW — `17-cut-and-peek/NOTE.md:132` — "the exceptional row" collides with "exceptional pair" defined at line 102 for a frequency zero-sum pair — high — replace with "the unique row taking the minority value of f"
F: LOW — `17-cut-and-peek/NOTE.md:217` — "player's bit" introduces the undefined term "player"; the note uses "input bit" or "variable" elsewhere — medium — replace with "input bit"
F: LOW — `17-cut-and-peek/NOTE.md:238` — "strand" is specialized jargon from [5] used without explicit definition — low — replace with "strand (sub-sequence of cards at a fixed residue class modulo 6)"

### Important points not asked about

1. **Proposition 3 holds under weak correctness (`17-cut-and-peek/NOTE.md:188-214` vs `solver-b/proofs.txt:116`):**
   Section 4 notes that weakening correctness to \(M(1\dots 1) \ne M(0\dots 0)\) lowers the required card count to \(m=12\), but omits that Proposition 3's lower bound \(m \ge 2^n - 1\) remains valid under weak correctness. The proof only requires \(\delta = C_S(1\dots 1) - C_S(0\dots 0) \ne 0\), which holds whenever \(M(1\dots 1) \ne M(0\dots 0)\).

2. **Unsearched candidate parameters between 16 and 35 (`17-cut-and-peek/NOTE.md:245` vs `reports/referee.md:68-71`):**
   The note states that the minimal card count for 3-bit AND with partial opening is between 16 and 36. However, the upper bound 36 comes from an incomplete search over interleaved strand families: factorizations \(m = ab < 36\) including \((a,b) = (7,5)\), \((11,2)\), \((11,3)\), \((12,2)\), and \(13 \le a \le 17\) with \(b=2\) were not exhaustively resolved.

3. **Card balance gives a generic lower bound \(m \ge 2n\) (`17-cut-and-peek/NOTE.md:219-220` vs `solver-b/proofs.txt:16-21`):**
   The note lists as a further fact that each bit controls an even number of cards, half of each colour. Because every player must control at least two non-constant cards (\(|P_i'| \ge 2\)) to preserve privacy and correctness, this immediately implies \(m \ge 2n\) for any valid protocol with partial opening, which is tighter than \(2^n - 1\) for \(n=2\).
