# open-upper referee report

**Overall: CONFIRMED (given the table).** I followed every step of `open-upper/proofs.txt` by hand and found no gap. My own program recomputed T1 and T2 with no lost state. Q_k(n) <= P(n) for all k >= 2, n >= 2 stands; with the refereed lower bounds, Q_k(n) = U1(n) = P(n).

Files: `open-upper-referee/` (`ref.cpp`, `lemmas.py`, `run1.txt`, `run2.txt`).

## Verdict per statement

**Lemma C — CONFIRMED** (win within 3·ceil(log2 m) from any position, k >= 2).
- S is re-asked only while both sides are alive, so ell >= 1 at every asking; ell = 0 never occurs.
- A liar gets obligation >= 2, which covers the rest of the round whether it lied first or second. Earlier obligations only forbid more lies. Changes in ell between askings do not matter.
- m = 1, 2, 3 give 0, 3, 6. Survivors of three answers lied at most once, so they are on the majority side, of size <= ceil(m/2).

**Lemma S — CONFIRMED** (nice, w <= 2^j, m >= max(2, 2j−2): a split with 2·ell >= j−1 and nice outcomes of weight <= 2^(j−1)).
- Even a, including a = 0: weight difference 0 or 1; ell = floor(m/2) >= 1; m >= j for j = 1 and j >= 2.
- Odd a: niceness gives b >= j−1 >= d. d < 0 needs j = 1, where odd a forces (1,0), excluded. Weight difference (j−1)−d is 0 or 1. 2·ell >= j for j >= 3 and >= 1 for j = 2, so ell >= 1.
- Off-by-one: a liar at question s is barred from s+1..s+k·ell; j−1 questions remain; k·ell >= j−1 covers them all.

**Theorem 1 — CONFIRMED given T1.**
- The three ranges partition all m >= 2, j >= 1.
- j = 15: m <= 27, 3·5 = 15 <= 15. j = 14 would need 15 > 14, so the table is needed up to exactly 14. j = 18: 33 <= 64. The step j -> j+3 needs 2j >= 9.
- Lemma C allows arbitrary obligations, so its use is legitimate.

**Reduction k >= 13 to k = 13 (j <= 14) — CONFIRMED.** After any lie at most 13 questions remain; ell >= 1 gives k·ell >= 13, ell = 0 gives 0 for every k.

**Theorem 2 — CONFIRMED given T1, T2.**
- The non-nice root is handled separately and correctly.
- P(n) <= n+1 (even n) and P(n) <= n (odd n >= 7): bases and induction steps check.
- Outcome weights are half the defining expression of P; the odd-n term appears in the "yes" branch (b = a−1).
- P(3) = 5, P(5) = 6; the exceptions are exactly n = 3 (k = 2, 3) and n = 5 (k = 2).

**Corollary 1a** — its converse directions rest on solver-a P3/P3', not re-read.

## Errors found

None in the mathematics. Small notes:
- T1 as stated has 960 states per k; the proof's "1310" and "1884" are wider windows, not recounted.
- Lemma S never states ell >= 1. It is true (above) and is needed to read "obligation k·ell" as "blocked".

## Mechanical tests (my own code; I never opened the author's programs)

- **T1:** memoised search over obligation multisets, every question enumerated. k = 2..14, 20, 1000: 960 states each, 0 lost.
- **T2:** Q_2(3) = 5, Q_3(3) = 5, Q_2(5) = 6. Also Q_k(n) = P(n) exactly for n = 2..300, k = 2, 3, 4, 7.
- **Wider table:** all (a,b;j), j <= 12, a+b <= 30, k = 2, 3, 5, 13: nice states are winnable iff w <= 2^j (3510 each).
- **Literal definition** (explicit lie sets, rule (ii) checked directly): Q_2(2) = 3, Q_2(3) = 5, Q_3(3) = 5, Q_2(4) = 5, Q_3(4) = 5; Q_1(2), Q_1(3) not won in 6.
- **Lemma C:** forced strategy, obligations 0..6, m <= 7, every S, every responder, k = 2, 3, 5: 0 failures in 3431 cases each.
- **Lemma S:** all 3,067,392 qualifying states with j <= 13 plus 45,881 boundary samples up to j = 257, k = 2, 3, 13, under the true obligation transition: 0 failures.
- **Theorem 2:** first-question claims for n <= 200000 plus samples to 2^119, k = 2, 3, 4, 9.
- **k-reduction:** transitions of k = 13 against k = 50 from j = 14: 9100 comparisons, 0 differences.

**Deliberately false variants — all failed as required:**
- T1 with w <= 2^j + 1 (2 lost); T1 with k = 1 (60 lost).
- Lemma C with k = 1; with the bound minus 1; with two askings.
- Lemma S with k = 1; without niceness; with m >= j; with d's parity swapped.
- Threshold from j = 14 (1 failure).
- Theorem 2 without exceptions (exactly n = 3, 5 fail); with k = 1; with the even-n formula for odd n.
- Transitions k = 13 against 12 at j = 14, and 13 against 14 at j = 15 (134 differences each).

## Not checked

- The author's `table.cpp`, `strat.py`, `lemmaC.py` and outputs.
- Solver-a P2/P3/P3' (taken as refereed).
- T1 rests on machine search only. My solver and the author's both use the refereed abstraction P1; my definition-level check covers only n <= 4.
