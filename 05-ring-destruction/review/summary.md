<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-083548 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $3.387; this month $125.63 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.531, input 24 + cache write 65,405 + cache read 387,500 tokens — cache hit 86%
- claude:claude-opus-5-5: $0.856, input 16 + cache write 57,127 + cache read 146,469 tokens — cache hit 72%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q6** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| **Q8** (split) | CONFIRMED | REFUTED | REFUTED | CONFIRMED | REFUTED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q6, Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 13 MODERATE, 38 LOW
- **MODERATE** [claude:claude-fable-5-1] `README.md:74-75` — "Each destruction creates at most one new unit of delay" is false: an event sets v_p = v_{p−1}+1, which can jump by several units (solver A says so itself, `reports/solver-a.md:55`). What is bounded is the number of (piece, value) pairs. — high — "Each destruction creates at most one new (piece, value) pair, so after t rounds there are at most kt pairs."
- **MODERATE** [claude:claude-opus-5-5] README.md:74-75 — "Each destruction creates at most one new unit of delay… at most kt units" is false. One destruction can raise a piece's delay by several units; reports/solver-a.md:106 says exactly this and calls the original argument invalid. The README presents that invalid argument as fact — high — "Each destruction creates at most one new (piece, delay value) pair, so after t rounds there are at most kt pairs; pairs are closed downward along diagonals."
- **MODERATE** [claude:claude-opus-5-5] README.md:36 — "proved by hand, twice" is not supported. The referee calls solver B's proof "outline only" (reports/referee.md:16) and did not check it in detail (:45). B's Theorem L also assumes k ≤ n and T ≥ ⌈n/k⌉ (solver-b/proofs.txt:90), which excludes (4,3) — high — "proved by hand by solver A (referee read it); solver B found the same strategy, with a proof for k ≤ n, n ≥ 5 that the referee only simulated."
- **MODERATE** [claude:claude-opus-5-5] README.md:41 and README.md:12 — "two solvers and the referee agree" by "exhaustive search" for n ≤ 12 is overstated. Solver B's table stops at n = 11 (reports/solver-b.md:138-150), and B proved (11,4) rather than computing it. The lower bounds for (11,4), (12,3) and (12,4) come from the strategy, not from search (reports/solver-a.md:79; referee.md:13) — high — "exhaustive search for every upper bound; three lower bounds come from checked strategy witnesses; solver B computed only n ≤ 11."
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:55-70` — Theorem L’s proof is undefined for \(k=1\), although its bound is trivially true — confidence: high — restrict construction to \(k\ge2\).
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:148-150` — \(F(10,4)\) is not B’s first unresolved entry; B already cannot exclude \(F(5,4)=4\) — confidence: high — name \(F(5,4)\).
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:39` — attributes the unqualified \(k\ge3\) bound to B, whose proof requires \(k<n\) — confidence: high — add \(n>k\).
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:65,78-79` — delays need not have equal totals: `solver-a/witgen/g_7_3.txt:1-4` yields sums \([2,2,2,1,1,2,2]\) — confidence: high — say “each sum is at least \(D\).”
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:74-75` — current delay “units” can exceed \(kt\); that witness totals 12 with \(kt=9\) — confidence: high — say “at most \(kt\) historical piece-value pairs.”
- **MODERATE** [cursor:grok-4.7-medium] `README.md:65-67` — “same total” / “same margin” fails for the comb strategy: n=7,k=3, gaps 3,2,2, T=3, the eight-position delay sums are 1,2,2,2,2,2,1 — high — say the two delays sum to at least 2T−n+2 everywhere, not to one common value
- **MODERATE** [cursor:grok-4.7-medium] `README.md:74` — one hit sets v_p to v_{p−1}+1 and can raise that delay by more than 1, so the sum of delays need not be ≤kt; the kt bound is on pairs — high — “each destruction adds at most one (piece, value) pair; after t rounds there are at most kt pairs”
- **MODERATE** [cursor:grok-4.7-medium] `README.md:81-83` — value H is created only in round t when t is odd (2H−1=t); for even t it can appear in round t−1, and that is not why the even case is tight — high — restrict the “at most k of them” sentence to odd t, and point the even case at (E)
- **MODERATE** [cursor:gemini-3.8-flash-high] 05-ring-destruction/README.md:73-74 — Explanation asserts each destruction creates at most one unit of delay on one piece of information, but one destruction can increase a piece's delay by up to \(H\) (via \(v_p(t) = v_{p-1}(t-1) + 1\)); solver A counts distinct (piece, value) pairs \(|P_A| + |P_B| \le kt\), not delay units, explicitly noting the single-unit delay argument is invalid (reports/solver-a.md:54) — confidence: high — Replace "unit of delay on one piece of information" with "(piece, value) pair"
- **LOW** [claude:claude-fable-5-1] `README.md:65-67` — "add up to the same total"/"same margin": Theorem L proves only d+e ≥ 2T−n+2; equality is neither shown nor needed. — high — "add up to at least the required total".
- **LOW** [claude:claude-fable-5-1] `README.md:78-79` — "must add up to a fixed amount" → "to at least 2t−n+2". — high
- **LOW** [claude:claude-fable-5-1] `README.md:81-83` — top-value pairs are created only in the final round only when t is odd (`proofs.txt:51-52`). — high — add "when t is odd".
- **LOW** [claude:claude-fable-5-1] `README.md:69-70` — k ≤ 2 explanation ("too few chains … no delay worth having") is not the proof, which is a pair count (`proofs.txt:118-124`). — medium — "with k ≤ 2 the adversary makes ≤ n destructions by round ⌊n/2⌋, too few to give every process a delay".
- **LOW** [claude:claude-fable-5-1] `README.md:39` — B's bound stated "for k ≥ 3" without n > k; Lemma Φ (`solver-b/proofs.txt:55`) requires k < n. — high — add "n > k".
- **LOW** [claude:claude-fable-5-1] `README.md:36` — "proved by hand, twice": B's Theorem L assumes T ≥ g (`solver-b/proofs.txt:90`, fails at (4,3)) and the referee calls it "outline only", unread (`referee.md:16,45`). — high — "proved by hand by A (B gives the same strategy, read by nobody, under n ≥ 5)".
- **LOW** [claude:claude-fable-5-1] `README.md:12` — "exhaustive search agrees for n ≤ 12" holds only for k ≤ 4 (k ≤ 8 for n ≤ 8). — high
- **LOW** [claude:claude-fable-5-1] `README.md:8,66-67` — "until round n−⌈n/k⌉" is ambiguous; nobody is complete through round n−⌈n/k⌉−1 and someone is at n−⌈n/k⌉. — medium
- **LOW** [claude:claude-fable-5-1] `solver-a/proofs.txt:123` — "Take x with f(x)≥2": existence of some x with f>0 not stated (else apply the mirror argument to g). — high
- **LOW** [claude:claude-fable-5-1] `solver-a/proofs.txt:127-128`, `README.md:42` — even n, k ≥ 2n is actually proved there (all links on odd rounds + statement cap), yet tagged NOT PROVED / "computed". Understatement. — medium
- **LOW** [claude:claude-fable-5-1] `README.md:98-99` — referee item 8 (`referee.md:25`) I traced: d=e=n/2 at every position forces 2n top-value pairs all created in round n−1; correct. The "unreviewed" caveat can now be dropped. — high
- **LOW** [claude:claude-fable-5-1] `README.md:108-109` — "one paper … could not be opened": `prior-art.md:48` lists three unopened (Chlebus–Diks–Pelc, Dobrev et al., Godard–Peters). — high
- **LOW** [claude:claude-opus-5-5] README.md:64-67 — "spaced n/k apart" and "the two delays add up to the same total… same margin" are not what was proved. Theorem L shows only that the sum is ≥ D, with gaps ≤ ⌈n/k⌉ (solver-a:69) — high — "spaced at most ⌈n/k⌉ apart, so the two delays add up to at least 2t−n+2 at every process through round n−⌈n/k⌉−1."
- **LOW** [claude:claude-opus-5-5] README.md:8, 66-67 — "until round n−⌈n/k⌉" is off by one: survival lasts through T = n−⌈n/k⌉−1, and the first completion is at n−⌈n/k⌉ — medium — "someone first knows everything at round n−⌈n/k⌉."
- **LOW** [claude:claude-opus-5-5] README.md:69-70 — the k ≤ 2 explanation is hand-waving. For k = 2 the formula n−⌈n/2⌉ = ⌊n/2⌋ still holds. Through round ⌊n/2⌋−1, D ≤ 0, so no delay is needed at all. The upper bound is the count in §3 — high — "With k ≤ 2, kt destructions are too few to give every process a delay (solver-a §3); for k = 2 the general formula already equals ⌊n/2⌋."
- **LOW** [claude:claude-opus-5-5] README.md:79 — "add up to a fixed amount" should be "at least D = 2t−n+2" — high — as stated.
- **LOW** [claude:claude-opus-5-5] README.md:81-83 — the "at most k top-value pairs" step is needed only for odd t (solver-a:51-52, 92-99) — high — add "(for odd t)".
- **LOW** [claude:claude-opus-5-5] README.md:39 — leaves out solver B's hypotheses n > k and n ≥ 5 (solver-b:82, 111) — high — "for n > k ≥ 3".
- **LOW** [claude:claude-opus-5-5] README.md:42 and README.md:98-99 — understates what is proved:
- **LOW** [claude:claude-opus-5-5] README.md:108-109 — "one paper … could not be opened" understates the gap. prior-art.md:48 lists three full texts not opened (Chlebus–Diks–Pelc, Dobrev et al., Godard–Peters). "Did not find this formula" also needs a qualifier: OEIS has ⌊2n/3⌋ and ⌊3n/4⌋ (:34-36), just not in a network setting — high — "three full texts could not be opened; the k = 3, 4 rows match OEIS sequences with no network context."
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:108 — "For k≥4 true as above" does not follow from the even-case inequality — high — "(k−3)q+1 ≥ 2k−5 ≥ k−1 for k ≥ 4."
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:18` — L0’s induction invokes \(s-2\) at \(s=1\) without stating the immediate base case — confidence: high — add the base case.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:123-124` — the even C1 proof chooses positive \(f\) without handling empty f-support — confidence: high — add “otherwise exchange \(f,g\).”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:55-67` — Lemma Φ says merely “a function”; its telescoping identity requires integer-valued deficits — confidence: high — state \(A:\mathbb Z_n\to\mathbb Z_{\ge0}\).
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:36` — “strategy found independently” ignores that both solvers received the opposing-ramp hint at `brief-solver.txt:20-27` — confidence: high — say “independently completed from the shared hint.”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:12` — “exhaustive search agrees for \(n\le12\)” omits \(k\le4\); larger-k exhaustive data stops at \(n=8\) — confidence: high — state both ranges.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:63` — “never repeats a link” is false: rounds 1 and 5 repeat links in `solver-a/wit/w_8_4_T5.txt:2-6` — confidence: high — say “never in consecutive rounds.”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:81-83` — top values are forced into the final round only when \(t\) is odd — confidence: high — add that condition.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:69-70` — “too few chains” is hand-waving, not the \(k\le2\) proof — confidence: high — cite the pair-count/closure contradiction.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:91-93` — witness files are not byte-identical; only normalized schedules match — confidence: high — replace accordingly.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:17-18` — the writeup of b_{i+1}(s)≤b_i(s−1)+1 uses b_i(s−2) at s=1 — high — check s=1 directly (the value is 0 or 1); the arc claim itself is true
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:80` — “H<n” is not proved and fails for large t (t=2n gives H=n) — high — drop it; distinct f-values and |P_A|=Σ h_c do not need it
- **LOW** [cursor:grok-4.7-medium] `README.md:11` — “exhaustive search agrees for n≤12” covers cells A marked as strategy-plus-NO only (`reports/solver-a.md:28`, stars on (11,4),(12,3),(12,4)) and B has no n=12 search row (`solver-b/results.txt:1-11`) — high — say the printed k≤4 table matches A’s logs, B through n=11, and the referee’s recomputation
- **LOW** [cursor:grok-4.7-medium] `README.md:39` — B’s corollary is stated for k≥3, but the proof assumes n>k (`solver-b/proofs.txt:71,82`) — high — add “for n>k”
- **LOW** [cursor:grok-4.7-medium] `reports/referee.md:26` — “hence F=2⌊n/2⌋ for k≥2n” does not contain the achieving play; destroying every link on odd rounds does attain it, but that argument is only named in `solver-a/proofs.txt:128` — medium — cite that strategy in the same paragraph
- **LOW** [cursor:gemini-3.8-flash-high] 05-ring-destruction/README.md:41 — Table status claims entries computed by exhaustive search, but for (11,4) and (12,3-4) YES search timed out (1500s) and lower bounds were supplied by verified strategy witnesses (reports/solver-a.md:28, reports/solver-b.md:32, reports/referee.md:13-14) — confidence: high — Qualify status to note exhaustive search for upper bound NO and witness verification for lower bound YES where search timed out
- **LOW** [cursor:gemini-3.8-flash-high] 05-ring-destruction/solver-b/proofs.txt:147 — Claims F(10,4) is the first unproved entry left open by Theorem U, but Theorem U also fails to exclude \(T=3\) for (5,4) since \(2nh = 20 \le kT(2h+1-m) = 24\), making F(5,4) the true first open entry in B's table (reports/referee.md:33) — confidence: high — Change F(10,4) to F(5,4)
- **LOW** [cursor:gemini-3.8-flash-high] 05-ring-destruction/README.md:70-71 — Hand-waving explanation that \(k \le 2\) has too few chains for ramps to cover the ring; the formal obstruction is that the total pair budget \(2t\) cannot supply the positive deficits required at all \(n\) positions — confidence: high — Rephrase in terms of pair budget versus deficit requirement

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
