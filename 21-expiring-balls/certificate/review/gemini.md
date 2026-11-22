<!-- Archived by the organiser on 2026-11-21. Report of the Gemini seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `21-expiring-balls/certificate/PROOF.md:6-37`, `certificate/check.cpp:50-56,60-63,71-76`. Prefixes of canonical states preserve canonical labeling because labels appear in order 0, 1, 2... from the start; induction over moves (Expire via Property 3, Offer via Property 4, load bounds via Property 2, start state via Property 1) proves the Rule maintains load below L indefinitely. — none

Q2: CONFIRMED — `21-expiring-balls/certificate/PROOF.md:20-27,40-45`. Follows directly: the empty state has no non-empty prefix so is not in A and thus in S; S is non-final by definition; if Expire(q) were in A, q would be in A by the extension fact, so Expire(q) must be in S. — none

Q3: CONFIRMED — `21-expiring-balls/certificate/PROOF.md:66-71`, `certificate/check.cpp:98-106`. Flipped \(c_1\) has \(c_2\) as a prefix since \(\text{length}(c_1) \ge \text{length}(c_2)\), so it is in A; \(T^+\) fires at \(c_1\) and forces \(\text{dropNewest}(c_1) \in A\), placing \(q \in A\) by extension; for \(j = 1\), \(\text{dropNewest}(c_1)\) is the empty state (never in A), so any certificate with \(j = 1\) and flipped in A is rejected by the checker; equal length is symmetric. — none

Q4: CONFIRMED — `21-expiring-balls/certificate/PROOF.md:48-52,60-64,72-85`, `certificate/check.cpp:111-123,125-132`. F1, F2, F3 exhaust all pairs of bad answers \(\{(\text{in } A, \text{in } A), (\text{in } A, \text{load } L), (\text{load } L, \text{load } L)\}\); in F2, `walkMin` from \(c\) to \(f\) encounters no intervening lines of C (since \(c\) is the longest) and no load \(L-1\) on bin 1 before \(f\) (since \(f\) is the shortest), reaching \(f\) where \(T\) fires; in F3, `walkSeeds` hits minimal seed \(q_0\) and enforces \(q_0 \in A\). — none

Q5: CONFIRMED — `21-expiring-balls/certificate/PROOF.md:98-106`, `certificate/check.cpp:114-120,129`. A forest on \(n\) bins with \(\ge 2\) components has at most \(n-2\) edges; distinct witness bins in separate components require disjoint sets of balls, requiring \(\ge k + (L - 1 - \text{ld}_e(x)) + (L - 1 - \text{ld}_e(y))\) total balls; P2 bounds extensions by \(n-1\) edges. Neither pruning skips states required by the proof. — none

Q6: CONFIRMED — `21-expiring-balls/certificate/check.cpp:83-93,98-106,194-201`, `certificate/PROOF.md:57-85`. Code rejects empty/illegal/overloaded states and non-canonical lines; \(T/T^+\) matches PROOF.md; `older()` explores all canonical children with untouched bins assigned \(m, m+1\); `canon()` is applied consistently; `inA()` inspects all prefixes \(1 \le j \le k\); no divergence found between code and proof. — none

Q7: CONFIRMED — `21-expiring-balls/certificate/out/difftest.log:1-49,80-85`, `certificate/out/count_check.txt:1-31`, `certificate/check.cpp:197-201`. Validation cases cover positive and negative ground truths across \(n = 4 \dots 9\); orbit sums match the Cayley recurrence; because the mathematical reduction is sound and the checker implements it faithfully, the checker cannot accept a certificate if the Adversary actually wins. — none

Q8: REFUTED — `21-expiring-balls/README.md:46,90-93,106,116`, `certificate/check.cpp:1-217`. Line count is 217, not 218; `certificate/out/` logs contain no sha256 or diff proving byte-for-byte identity of the 11-bin reduced files; and the 1-bin expiry penalty remains an unverified assertion because any-removal on 11 bins lacks a certificate (`README.md:141-143`). — "In `21-expiring-balls/README.md`, change '218-line' to '217-line', note that byte-for-byte reduction identity was verified out-of-repo, and clarify that the 1-bin cost assumes solver A's uncertified any-removal value."

F: MODERATE — `21-expiring-balls/certificate/out/difftest.log:50-79` — Contains 29 spurious lines logging `MISMATCH` for the state count test against Cayley labelled forests, directly contradicting `certificate/out/count_check.txt` where all rows match — confidence: high — Strip the broken comparison run from `difftest.log` or replace it with the clean output from `count_check.txt`.
F: MODERATE — `21-expiring-balls/README.md:46,106` — Claims the two programs' winning sets reduce byte-for-byte to the same certificate (`sha256 35f5203d…e99d`), but no reduction run, diff, or checksum log for 11 bins exists in `certificate/out/` — confidence: high — Add a reproduction log showing the minimization and sha256 hash comparison to `certificate/out/`.
F: MODERATE — `21-expiring-balls/README.md:90-93` — Unhedged statement "for k = 4 the order of expiry costs the adversary one bin" is stated as fact, despite any-removal on 11 bins resting solely on an uncertified computation by solver A — confidence: high — Add qualification: "assuming the uncertified 11-bin value for the any-removal game holds".
F: LOW — `21-expiring-balls/README.md:116` — Text claims a "218-line checker", but `certificate/check.cpp` has 217 lines — confidence: high — Update line count to 217.
F: LOW — `21-expiring-balls/certificate/PROOF.md:69-70` — For Case F1 with \(j = 1\), \(\text{dropNewest}(c_1)\) is the empty state which is never in A, rendering the prose deduction technically incomplete; soundness holds because the checker rejects any certificate containing such a 1-ball line whose flipped counterpart is in A — confidence: medium — Clarify in PROOF.md that 1-ball lines with flipped partners in A are explicitly rejected by \(T^+\) at load time.

### Important points not asked about

1. **Damage test accepts redundant additions (`certificate/out/damage_11.log:23`):**
   `damage_11.log` shows line `added line: 01 02 03 45 46 47 58` resulting in `ACCEPT`. Because that state was already in \(A\), adding it to \(C\) left \(A\) and \(S\) unchanged. The brief implies damage tests always reject; redundant line insertions do not invalidate a certificate.
2. **Multi-process partition safety (`certificate/check.cpp:10,212-214`):**
   The partitioned execution mode splits certificate lines across parts (`i % parts == part`), but `walkSeeds` is run only by `part == 0`. Complete validation relies on external tooling ensuring that every partition from `0` to `parts - 1` outputs `ACCEPT`.
3. **Hardcoded limit on bins (`certificate/check.cpp:173`):**
   `check.cpp` enforces `n <= 12` because keys pack 4-bit bin indices into a 128-bit `Key`. Attempting to verify certificates for \(n \ge 13\) requires modifying the key packing and data types.
