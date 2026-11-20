# solver-b report

**Headline: N(3) = 7.** g(6) = 2 and g(7) = 3, so FIFO expiry helps the Adversary and the problem's guess (N(3) = 8) is wrong. One size beyond: 12 <= N(4) <= 16; the exact value is open.

## 1. Tables

FIFO game (the problem as stated). All entries COMPUTED by exhaustive game solving unless tagged.

| n | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
|---|---|---|---|---|---|---|---|---|---|---|
| g(n) | 1 | 1 | 2 | 2 | 2 | 3 | 3 | 3 | 3 | 3 |

- n <= 8: `fast.cpp`, forward enumeration of every state reachable from empty, then least-fixpoint Adversary-win set.
- n <= 5: also `slow.py`, a naive labelled implementation with no symmetry reduction; agrees with `fast` on all 36 cases (n 2..5, targets 1..3, three variants).
- n = 9, 10, 11 (g < 4): backward Adversary attractor from the states with two load-3 bins in different components (`back.cpp`, `back2.cpp`). Attractor sizes 250 (n=8), 34,970 (n=9), 3,675,805 (n=10), 974,272,116 (n=11); the empty state is not in it.
  - n = 8, 9: additionally confirmed by an independent forward pass (`back v`) that builds a Rule-safe closure (73,593,200 states at n=9) and asserts every offer is answerable.
  - n = 10: two implementations agree on the attractor size; no forward pass.
  - n = 11: `back2` only, one run (786 s). Weakest entry in the table.
- g(n) >= 3 for n >= 8 is by monotonicity; n=8 also computed directly.

N(1) = 2, N(2) = 4, N(3) = 7, N(4) in {12,...,16} (upper bound proved, lower bound computed).

Variants (`fast`, third argument), n <= 8:
- No Expire: g = 1,1,2,2,2,2,3 for n = 2..8, i.e. floor(log2 n); load 3 not forceable at n=7, forceable at n=8; load 4 not at n=8.
- Adversary removes any ball it likes: load 3 first forceable at n=7 (not at 6); load 4 not at n=8.

## 2. Witnesses (plain text, in `solver-b/`)

State format: age-ordered balls `ro` (receiver, other), bins relabelled by first appearance. All verified by `check.py`, which has its own move generation over all n bins.

- `adv_n7_L3.txt` — 33 states, Adversary strategy with a rank that strictly decreases on every Rule answer: g(7) >= 3.
- `rule_n6_L3.txt` — 3,843 states: contains empty, loads <= 2, closed under Expire, every legal offer has an answer inside: g(6) < 3.
- `rule_n5_L3.txt` (246), `rule_n3_L2.txt` (4), `adv_n4_L2.txt` (5).
- `rule_n8_L4.txt` — 2,318,587 states, g(8) < 4. This file is 48 MB, far over the 300 kB guideline; I did not delete it because the brief forbids deleting.
- No-Expire: `rule_n7_L3_noexpire.txt`, `adv_n8_L3_noexpire.txt`.
- No certificate file for n = 9, 10, 11 (too large); rerun `back 9 4 v`, `back2 10 4`, `back2 11 4`.
- Checker seen failing on purpose: `BAD_rule_n6_L3.txt` (one line removed) and `BAD_adv_n7_L3.txt` (moves altered) are rejected.

The winning position (two 2-stars, centres offered) already fits in 6 bins but is unreachable there; the seventh bin lets Adversary pad and expire until the junk edge is gone.

## 3. Headline and conjecture

N(3) = 7. Confidence high: two solvers (forward and backward) agree, and both directions have small certificates checked by separate code.

No closed formula. The natural candidate 1 + k(k+1)/2 (2, 4, 7, 11) fits k <= 3 but fails at k = 4 if the n=11 computation is right. N(k) = 2k is false (k=3) and N(k) = 2^k is false (k=3). I have no supported guess for N(4) within 12..16, and no optimal Rule strategy in closed form.

## 4. Proofs (full text in `solver-b/proofs.txt`)

- P1 COMPLETE: N(k) >= 2k. Rule gives the ball to the lower-load endpoint; a ball creating load k needs two load-(k-1) bins in different components, each of at least k vertices.
- P2 COMPLETE: N(k) <= 2^k (recursive halving, no Expire).
- P3 COMPLETE, no-Expire game only: value is floor(log2 n) (smaller-component rule, invariant load <= log2 of component size). Expire breaks the invariant.
- P4 COMPLETE, free-removal variant only: 1 + k(k+1)/2 bins suffice (build clean stars recursively). No matching lower bound.
- P5 computer-assisted: N(3) = 7 from the two certificates above.
- No proof of anything general for the FIFO game beyond P1 and P2.

All UNVERIFIED beliefs in the statement check out (the k^2/2 one only as an upper bound); the GUESS does not.

## 5. Interpretation

- Rule may in principle use history; this is a finite reachability game, so positional strategies suffice and I solved it positionally.
- A state is taken up to relabelling of bins, keeping age order and receivers; every age-ordered oriented forest is reachable from empty.
- The statement is otherwise unambiguous. The alternative reading "Adversary chooses which ball leaves" is in section 1.

## 6. Code

- `fast.cpp` — all n <= 8 numbers, all three variants, all witness files.
- `slow.py` — reference for n <= 5.
- `check.py` — witness checker.
- `back.cpp` — backward attractor, n <= 10, and the forward verification pass for n <= 9 (`out_9_4_verify.txt`).
- `back2.cpp` — memory-light backward attractor; n=10 and n=11 (`out_11_4_back2.txt`). `out_11_4.txt` is an abandoned `back` run that hit the 1500 s timeout.
