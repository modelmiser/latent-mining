<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED — solver-a/proofs.txt:16-22. The bound b_{i+1}(s) ≤ b_i(s−1)+1 makes each union of integer intervals contiguous, so K_t(i) is an arc. Completeness means a+b ≥ n−1, which is d+e ≤ 2t−n+1, so D = 2t−n+2 is right (:26). With i = p−t, the recursion min(d_i+1, d_{i+1}) becomes min(v_{p−1}+1, v_p) (:31-32). The two events in L2 both hit the A-link into p−t (:40-41) — none

Q2: CONFIRMED — :44-52. (i) holds because each event sets one value and so creates at most one pair. (ii) holds because the event that first gives value y reads v_{p−1}(r−1) = y−1. (iii): if (p−1, y−1) were first created in round r−1, that would share a link with round r, which L2 forbids; so it was created by r−2, and induction gives r ≥ 2y−1. For odd t, H = (t+1)/2 can only be created in round t, so there are at most k such pairs — none

Q3: CONFIRMED — :79-85. Closure keeps each diagonal p−y fixed, so P_A is a union of initial segments of diagonals. f(x) ≡ x+t−c (mod n) and H < n, so f-values on one diagonal are distinct and lie in [1, h_c]. The least f is at most h_c−δ_c+1, which gives (1). Σδ_c = n because the X_c partition Z_n — none

Q4: CONFIRMED — :87-99, re-derived. Even t: g ≤ H gives δ_c ≤ h_c−s, and h_c ≤ H gives δ_c ≤ w; summing over both directions and using L3(i) gives (E). Odd t: g ≤ H1+β_c and h_c ≤ H1+ε_c give both bounds. E counts the top-value pairs, so E ≤ k. Each A-diagonal's x* is a different position and carries a different top-value B-pair, and vice versa, so B ≤ E; hence (O) — none

Q5: CONFIRMED — :103-113, re-derived:
- **Even t0:** s = ((k−3)q+2−r)/2 ≥ 0, w = q−1, the ceiling is ≥ k+1, and the condition reduces to q(k−3)+2k+2−3r ≥ (q−1)(k−3)+2 > 0.
- **Odd t0:** D′ ≥ H ⇔ (k−3)q+1 ≥ r. For k = 3, r = 2 the parity forces t0 even, and q = 2 gives w = 0. The right side of (O) does not increase in B or E, so its minimum is at B = E = k, which leaves q(k−3)+4k+3−5r ≥ 2k−1 > 0 for k ≥ 4, and ≥ 10 for k = 3 (where r = 1).
- **Minor gap:** "true as above" (:108) does not follow from the even case; a direct check, (k−3)q+1 ≥ 2k−5 ≥ k−1, does.

Proved for every n > k ≥ 3 — none

Q6: CONFIRMED — :55-70. At most m ≤ k links per round, and odd rounds use only A-links while even rounds use only B-links. The j-th event of each chain lands on piece P_c+j−1 with value ≥ j. β_c − α_c = 1, the intervals [α_c+1, α_{c′}] partition the ring, and the sum is ≥ T−δ+1 = D. This covers k ≥ n (m = n, δ = 1) and gaps up to ⌈n/m⌉. The choice of P_c is only stated in reports/solver-a.md:95 — none

Q7: CONFIRMED — solver-a:117-124 and solver-b:119-127 both cover both parities of n. Solver-b Lemma Φ (:55-67), Theorem U (:71-80) and Corollary U′ (:82-85) check out under their hypotheses (k < n; n > k) — none

Q8: REFUTED (several overstatements, listed below) — README:36, 41, 64-67, 74-75 — see F lines

F: MODERATE — README.md:74-75 — "Each destruction creates at most one new unit of delay… at most kt units" is false. One destruction can raise a piece's delay by several units; reports/solver-a.md:106 says exactly this and calls the original argument invalid. The README presents that invalid argument as fact — high — "Each destruction creates at most one new (piece, delay value) pair, so after t rounds there are at most kt pairs; pairs are closed downward along diagonals."

F: MODERATE — README.md:36 — "proved by hand, twice" is not supported. The referee calls solver B's proof "outline only" (reports/referee.md:16) and did not check it in detail (:45). B's Theorem L also assumes k ≤ n and T ≥ ⌈n/k⌉ (solver-b/proofs.txt:90), which excludes (4,3) — high — "proved by hand by solver A (referee read it); solver B found the same strategy, with a proof for k ≤ n, n ≥ 5 that the referee only simulated."

F: MODERATE — README.md:41 and README.md:12 — "two solvers and the referee agree" by "exhaustive search" for n ≤ 12 is overstated. Solver B's table stops at n = 11 (reports/solver-b.md:138-150), and B proved (11,4) rather than computing it. The lower bounds for (11,4), (12,3) and (12,4) come from the strategy, not from search (reports/solver-a.md:79; referee.md:13) — high — "exhaustive search for every upper bound; three lower bounds come from checked strategy witnesses; solver B computed only n ≤ 11."

F: LOW — README.md:64-67 — "spaced n/k apart" and "the two delays add up to the same total… same margin" are not what was proved. Theorem L shows only that the sum is ≥ D, with gaps ≤ ⌈n/k⌉ (solver-a:69) — high — "spaced at most ⌈n/k⌉ apart, so the two delays add up to at least 2t−n+2 at every process through round n−⌈n/k⌉−1."

F: LOW — README.md:8, 66-67 — "until round n−⌈n/k⌉" is off by one: survival lasts through T = n−⌈n/k⌉−1, and the first completion is at n−⌈n/k⌉ — medium — "someone first knows everything at round n−⌈n/k⌉."

F: LOW — README.md:69-70 — the k ≤ 2 explanation is hand-waving. For k = 2 the formula n−⌈n/2⌉ = ⌊n/2⌋ still holds. Through round ⌊n/2⌋−1, D ≤ 0, so no delay is needed at all. The upper bound is the count in §3 — high — "With k ≤ 2, kt destructions are too few to give every process a delay (solver-a §3); for k = 2 the general formula already equals ⌊n/2⌋."

F: LOW — README.md:79 — "add up to a fixed amount" should be "at least D = 2t−n+2" — high — as stated.

F: LOW — README.md:81-83 — the "at most k top-value pairs" step is needed only for odd t (solver-a:51-52, 92-99) — high — add "(for odd t)".

F: LOW — README.md:39 — leaves out solver B's hypotheses n > k and n ≥ 5 (solver-b:82, 111) — high — "for n > k ≥ 3".

F: LOW — README.md:42 and README.md:98-99 — understates what is proved:
- Even n with k ≥ 2n is proved by the all-links-on-odd-rounds strategy plus the statement's cap (solver-a:127-128).
- I checked the referee's argument (referee.md:25): at t = n−1, D = n and H = n/2, so all 2n top-value pairs are needed, all in round n−1. It is correct.

high — "even n, k ≥ 2n: proved; n ≤ k < 2n: referee's argument, checked by one outside reviewer."

F: LOW — README.md:108-109 — "one paper … could not be opened" understates the gap. prior-art.md:48 lists three full texts not opened (Chlebus–Diks–Pelc, Dobrev et al., Godard–Peters). "Did not find this formula" also needs a qualifier: OEIS has ⌊2n/3⌋ and ⌊3n/4⌋ (:34-36), just not in a network setting — high — "three full texts could not be opened; the k = 3, 4 rows match OEIS sequences with no network context."

F: LOW — solver-a/proofs.txt:108 — "For k≥4 true as above" does not follow from the even-case inequality — high — "(k−3)q+1 ≥ 2k−5 ≥ k−1 for k ≥ 4."

Not asked about: the headline C3/C4 holds. I re-derived every step of L0–L3, Theorem U and the Corollary and found no gap. What is left is how the README describes its evidence and explains the ideas, not the result itself.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
