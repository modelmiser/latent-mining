# Solver A report: planted majority against an unfair scheduler

Files are in `solver-a/`; proofs in `proofs.txt`, witnesses in `witnesses.txt`.

## 1. Tables

Exact values. COMPUTED = exhaustive search over all protocols (after the reduction of Lemma R).

| s | G(s) | status |
|---|------|--------|
| 2 | 1 | COMPUTED, also with no reduction (`search_ord 2 2`) |
| 3 | 3 | COMPUTED: a protocol wins N=3, none wins N=4 (`search`, `search2`) |
| 4 | 3 | COMPUTED: none wins N=4 (`search2 4 4`, 638,017,216 search nodes; replicated with a second checker) |
| 5 | >= 3 | NOT computed. 770 capped random-restart searches for N=4 found nothing; not an exhaustion |

Lower bounds from explicit protocols. "checked" = every instance verified by `check.cpp`.

| s | 8 | 10 | 11 | 15 | 2m+1 | 4M-1 | 8M-5 |
|---|---|----|----|----|------|------|------|
| G(s) >= | 5 | 7 | 9 | 15 | K(m)-1 | M^2-1 | M^3-1 |
| | checked | checked | checked | checked | proved | proved | proved |

K(m) is the largest maximum of a closed set of m integers (section 4), COMPUTED exhaustively (`closed.py`),
m = 1..13: 1, 2, 4, 6, 10, 14, 18, 24, 32, 42, 50, 62, 75.

The statement's claims:
- G(2) = 1: true.
- The 3-state protocol wins N=3 and fails on (1,3,C): true. G(3) is exactly 3.
- G is non-decreasing: true (Lemma M).
- Budget protocol with K = N+2: holds for K <= 6, where it wins exactly up to N = 2*ceil(K/2)+1 (`budget.py`).
- G unbounded: true. GUESS "G = Theta(s)": false.

## 2. Witnesses

- G(3) >= 3: `brief3.txt`. G(4) >= 3: `found.txt`. Both verified by `check.cpp` and `slow.py`.
- G(3) < 4, G(4) < 4: the searches; logs `s4N4.out`, `s4N4.err`, `s4N4_pp_masks*.out`.
- Weight protocols from `family.py` (`fam_*.txt`, `fam1w_*.txt`): S={1,2,3}, 8 states, wins exactly to N=5;
  {1,2,3,4}, 10 states, to 7; {1,2,4,5,10}, 11 states, to 9; {1,2,3,4,8,12,16}, 15 states, to 15.

## 3. Headline

**No.** No constant C gives G(s) <= C*s: G(4M-1) >= M^2-1 and G(8M-5) >= M^3-1 for every M. Proved
(Theorem 1', Corollaries 1a, 1b); confidence about 97%.
The "no" is not because G is infinite: **G(s) is finite for every s** (Theorem 2, proved, no explicit bound;
confidence about 90%).
The exact function is not found. Established: s^3/512 - O(s^2) <= G(s) < infinity. Whether G is polynomially
bounded is open here; GUESS only: it is not. The obvious fourth level of my family is not closed (`sets.py`),
and a greedy repair (`closure.py`) does worse than cubic.

## 4. Proofs (written out in `proofs.txt`)

- Lemma R, COMPLETE: one outcome per unordered pair of states and no swaps loses nothing.
- Lemma M, COMPLETE: G(s+1) >= G(s).
- Theorems 1, 1', COMPLETE. S is *closed* if every u >= 2 in S is a sum of two elements of S, and for u > v
  in S either u-v is in S, or u-v is a sum of two elements, or u-c and v-c are in S for some 0 < c < v.
  Then G(2|S|+1) >= max S - 1. States: tokens +u, -u (u in S) and one weightless state; qA = +K, qB = -K.
  Opposite tokens net exactly, so the signed weight sum is conserved. A token of weight >= 2 meeting a
  weightless agent that shows the wrong letter splits its weight onto it. Each move lowers
  (sum of |weights|, number of weightless agents), so every play ends. The sum favours the majority by at
  least K whatever the plant is, so a stuck configuration with a wrong weightless agent would have only
  weight-1 tokens, which needs K <= a+b.
  The budget protocol spends weight on a conversion; this one splits it, so the weights can be sparse.
  {1..M} with the multiples of M up to M^2 is closed (1a); adding M^2-j and the multiples of M^2 up to M^3
  keeps it closed (1b; by hand, and by machine for M <= 7).
- Theorem 2, COMPLETE. Suppose one protocol wins every N. (1) No instance has a cycle: add qB agents until
  the majority flips; the cycle would have to be good for both majorities. (2) A zero-sum combination of moves
  would give a cycle in a large population, so by Gordan's theorem a linear potential exists and plays on n
  agents have O(n) moves. (3) The scheduler activates agents one at a time in the order B,A,A,B,B,A,A,...,
  letting the active ones get stuck each time. When the active counts differ by one, the stuck configuration
  is terminal in an instance, so all active agents show that letter. From k A's and k B's, adding 2m+1 agents
  forces k*m moves on 2k+2m+1 agents, contradicting (2) for k = m large.
- Not done: an explicit upper bound on G(s); a lower bound above cubic.

## 5. Interpretation

- A cycle is a cycle of the multiset graph; a swap d(p,q) = (q,p), p != q, is a legal move and a self-loop.
- The plant ranges over all states, including states no play reaches.
- No ambiguity found that changes an answer.

## 6. Code

- `slow.py`: the definition, directly. `pp.h`, `check.cpp`: fast checker. `xcheck.py`: 2 x 600 random or
  mutated protocols, fast and slow agree. `redu.py`: Lemma R on 500 random ordered protocols, no violation.
- `search2.cpp` (own checker); `search.cpp`, `search_m.cpp` (`pp.h` checker): exhaustive searches. Solution
  counts agree for s=3 (6). Solution and node counts agree for s=4, N=2, one output map (5761 solutions), and
  for s=4, N=4, all six output maps (NONE each).
- `search_ord.cpp` (no reduction): done for s=2; timed out for s=3, N=4, so that cross-check is missing.
- `family.py`, `sets.py`, `closed.py`, `closure.py`, `budget.py`: protocols and closed sets.
- Failures seen on purpose: mutated protocols rejected by both checkers; `family.py` refuses {1,2,4,8}.
