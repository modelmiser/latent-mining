# Experiment 25: 5h - 28 <= G(h) <= C(h) <= 5h - 10 for h >= 4

Every part is marked PROVED (complete proof here) or COMPUTED. Written by one instance on
2026-11-21; not machine-checked as a proof; nobody else has read it.

Game and notation as in `25-throttled-chips/chain/3-question.txt` and `solver-a/proofs.txt`:
H = h - 1; k_x = adds at x so far, F_x = firings at x so far, eta_x = chips on x,

    eta_x = k_x - 2 F_x + F_{x-1} + F_{x+1}.                                   (*)

A line "survives" if every height is <= H after every step.

---------------------------------------------------------------------------------------------

## Part A. Theorem A (PROVED): C(h) <= 5h - 10 for every h >= 4. Hence G(h) <= 5h - 10.

(The page had 5h - 6. C(h) = 5h - 10 was known as a computed value for 4 <= h <= 14, so the
bound cannot be improved for the constant list in that range.)

Let h >= 4, H = h - 1 >= 3, m = 5H - 5 = 5h - 10 >= 10, and take the list of m zeros. Suppose,
for contradiction, that some line keeps every height <= H after every step up to and including
step (1) of round m. Let P be the position right after step (1) of round m.

A1 (counting). Firings before P happen in step (2) of rounds 1..m-1. Round 1 is a forced pass
(one chip on the board). Let p >= 0 be the number of passes in rounds 2..m-1. The number of
firings before P is f = m - 2 - p = 5H - 7 - p.

A2 (potential). Q = sum over chips of (position)^2. An add at 0 leaves Q unchanged, a firing at x
raises it by (x-1)^2 + (x+1)^2 - 2x^2 = 2. So Q(P) = 2f = 10H - 14 - 2p.
For every integer x:  x^2 = 4 - 4[x = 0] - 3[|x| = 1] + w(x), with w(x) = 0 for |x| <= 2 and
w(x) = x^2 - 4 for |x| >= 3 (values 5, 12, 21, ...). P holds m chips, so

    Q(P) = 4m - 4 eta_0 - 3 (eta_1 + eta_{-1}) + E,     E = sum_{|x| >= 3} (x^2 - 4) eta_x.

Write eta_0 = H - d_0, eta_1 = H - d_1, eta_{-1} = H - d_{-1} (all d >= 0, integers, because
heights in P are <= H) and D = d_1 + d_{-1}. Then Q(P) = 4m - 10H + 4 d_0 + 3D + E
= 10H - 20 + 4 d_0 + 3D + E. Comparing the two expressions for Q(P):

    2p + 4 d_0 + 3D + E = 6.                                                   (A)

A3 (solutions of (A)). E is 0 or at least 5. If E >= 5 then 2p + 4d_0 + 3D <= 1, so it is 0 and
E = 6; but E is a sum of terms from {5, 12, 21, ...}, never 6. So E = 0: P has no chip at |x| >= 3.
Then 2p + 4d_0 + 3D = 6 forces D even, and the solutions are exactly
  (I)   p = 3, d_0 = 0, D = 0;
  (II)  p = 1, d_0 = 1, D = 0;
  (III) p = 0, d_0 = 0, D = 2.

A4 (only -1, 0, 1 ever fired). f >= 5H - 10 >= 5 > 0. The first firing of the game is at 0 (only
vertex 0 holds chips before it), so f_0 >= 1, where f_x = number of firings at x before P. Let
x1 = max{x : f_x >= 1} >= 0. By (*) at the vertex x1 + 1 >= 1 (no adds there, f_{x1+1} = f_{x1+2}
= 0): eta_{x1+1} = f_{x1} >= 1 in P. As E = 0, x1 + 1 <= 2. Symmetrically on the left. So f_x = 0
for |x| >= 2, and by (*) in P:

    eta_2 = f_1,  eta_{-2} = f_{-1},  eta_1 = f_0 - 2 f_1,  eta_{-1} = f_0 - 2 f_{-1}.

Hence d_1 - d_{-1} = eta_{-1} - eta_1 = 2 (f_1 - f_{-1}), and, all m chips being on [-2, 2],
f_1 + f_{-1} = eta_2 + eta_{-2} = m - (3H - d_0 - D) = 2H - 5 + d_0 + D.

A5 (the opening). In round 2 the board is two chips at 0 after the add; the only possible firing
is at 0, giving one chip at each of -1, 1. If Firer fires in round 2, then after the add of round 3
the heights are 1, 1, 1 and round 3 is a forced pass. So rounds 2 and 3 are not both firing rounds:
p >= 1, and if p = 1 the single pass is in round 2 or round 3 (m - 1 >= 3, so both rounds count).

A6 (the three cases).
 (III) p = 0 contradicts A5.
 (I)   d_1 = d_{-1} = 0, so f_1 = f_{-1} by A4, and f_1 + f_{-1} = 2H - 5 is odd. Contradiction.
 (II)  The only pass among rounds 2..m-1 is in round 2 or 3, so rounds m-2 and m-1 are firing
       rounds (m - 2 >= 8 >= 4), at vertices of {-1, 0, 1} by A4. In P the heights at -1, 0, 1 are
       H, H-1, H, so after step (2) of round m-1 they are H, H-2, H.
       Round m-1 fired v. If v = +-1, then right after step (1) of round m-1 vertex v held H + 2
       chips: not allowed. So v = 0, and right after step (1) of round m-1 the heights were
       H-1, H, H-1; after step (2) of round m-2 they were H-1, H-1, H-1.
       Round m-2 fired v'. If v' = 0, vertex 0 held (H-1) + 2 = H+1 right after step (1) of round
       m-2; if v' = +-1, vertex v' held (H-1) + 2 = H+1 then. Not allowed. Contradiction.

So no line survives through step (1) of round m: the list of 5h - 10 zeros wins.  QED

Where it needs h >= 4: case (II) needs m - 2 >= 4. For h = 3 (m = 5) the statement is false:
C(3) = 7.

COMPUTED cross-check (`solve.cpp`, mode C, written from the definition; `out_C.txt`): the least
winning number of zeros is exactly 5h - 10 for the h listed there, and 7 for h = 3.

---------------------------------------------------------------------------------------------

## Part B. Theorem B (PROVED): under the lag rule every height is at most t/5 + 5.6 at every step of round t, against every list. Hence every list of length <= 5h - 29 loses and G(h) >= 5h - 28.

(The page had 5h - 176, and 5h - 26 - 10 H_15h. The new bound beats 2h - 1 from h = 10 on.)

Rule (unchanged from `solver-a/proofs.txt` Theorem 2; it does not look at the rest of the list):
T_x = (3 k_x + k_{x-1} + k_{x+1})/5, lag l_x = T_x - F_x. In step (2) Firer fires a ready vertex of
largest lag among the ready vertices, provided that lag is >= 1 (ties broken in any way, even
with knowledge of the list); otherwise it passes.

Facts from the earlier files, re-derived:
 (a) l_x >= 0 always (T never decreases; a firing needs lag >= 1 and lowers that lag by 1, and
     changes no other lag). l_x <= T_x. 5 l_x is an integer.
 (b) eta_x = eta*_x + 2 l_x - l_{x-1} - l_{x+1}, with eta*_x = (k_{x-2} + ... + k_{x+2})/5
     (substitute F = T - l in (*) and expand).
 An add at x raises the lags of x-1, x, x+1 by 1/5, 3/5, 1/5 (total 1) and no other.

### B1. Lemma (readiness from lag 6/5). If, right after an add, the largest lag M is >= 6/5, then some vertex of lag M is ready; so the vertex fired in that round has lag exactly M.

(The earlier files had "every vertex of lag M is ready" from M >= 6, then from M > 3. Here only
SOME vertex of lag M is claimed ready, which is all the later arguments use.)
Proof. Let x have lag M, and s = k_{x-2} + ... + k_{x+2} = 5 eta*_x. Since l_x is a maximum,
2 l_x - l_{x-1} - l_{x+1} >= 0 and eta_x >= eta*_x by (b).
 Case s >= 6: eta_x >= 6/5, an integer, so eta_x >= 2: x is ready.
 Case s <= 5: write k = k_x, F = F_x, nb = k_{x-1} + k_{x+1} <= s - k. Then
     6 + 5F <= 5M + 5F = 5 T_x = 3k + nb <= 2k + s <= 2k + 5,
 and by (*)  eta_x = k - 2F + F_{x-1} + F_{x+1}.
   F >= 2: 2k + 5 >= 16, so k >= 6 > s >= k. Impossible.
   F = 1, k >= 4: eta_x >= k - 2 >= 2: x is ready.   (k <= 2 is impossible: 2k + 5 >= 11.)
   F = 1, k = 3: 9 + nb >= 11 and nb <= s - 3 <= 2, so nb = 2, s = 5 (so k_{x-2} = k_{x+2} = 0),
     5 T_x = 11, M = 6/5. If a neighbour of x has fired, eta_x >= 3 - 2 + 1 = 2: x is ready.
     Otherwise take a neighbour u with k_u >= 1; its other neighbour has no adds and F_u = 0, so
     l_u = T_u = (3 k_u + 3)/5 <= M = 6/5 forces k_u = 1 and l_u = 6/5 = M; and by (*)
     eta_u >= k_u - 2 F_u + F_x = 2. So u is a ready vertex of lag M.
   F = 0, k >= 2: eta_x >= k >= 2: x is ready.       (k = 0 is impossible: 2k + 5 >= 6.)
   F = 0, k = 1: 3 + nb >= 6, so nb >= 3. If a neighbour of x has fired, eta_x >= 1 + 1 = 2: x is
     ready. Otherwise take a neighbour u with k_u >= 2 (nb >= 3 on two vertices); F_u = 0, so
     l_u = T_u >= (3*2 + k_x)/5 = 7/5, and eta_u >= k_u - 2 F_u = 2 by (*): u is ready. As
     l_u <= M, and M = (3 + nb)/5 <= (3 + s - 1)/5 <= 7/5, we get l_u = M. So u is a ready vertex
     of lag M.
 In every case a vertex of lag M >= 6/5 >= 1 is ready, so the rule fires, and the vertex it fires
 has the largest lag among ready vertices, which is M.  QED
(6/5 cannot be lowered: after the list x-1, x, x+1 the lags are 4/5, 1, 4/5 and nothing is
ready. `test_rule.py` records the largest M at which no vertex of lag M was ready: it is 1.)

### B2. Lemma (top-k sums, after Theorem U of `lag/PROOFS.md`, with a set that depends on the targets).

Fix a round t0 and let Wset = {z : T_z >= 6/5 after round t0}, n = |Wset|. Then after every round
t <= t0:
  (i)  every vertex outside Wset has lag <= 1;
  (ii) if n >= 1, for k = 1..n the sum S_k(t) of the k largest lags among the vertices of Wset
       satisfies S_k(t) <= B_k := k (1 + H_n - H_k),   H_j = 1 + 1/2 + ... + 1/j.
In particular after round t0 every lag is at most max(1, H_n) (at most 1 if n = 0).
Proof. (i) For z outside Wset and any moment up to round t0: l_z <= T_z <= T_z(t0) < 6/5, and
5 l_z is an integer.
(ii) Induction on t; t = 0 is trivial (all lags 0). Let l, l', l'' be the lags after round t,
after the add of round t+1 <= t0, and after round t+1; M = max over all vertices of l'. For any
set A of vertices, sum_A l' <= sum_A l + 1.
 Case 0, M <= 1: l'' <= l' <= 1 everywhere, so S_k(t+1) <= k <= B_k.
 Case 1, M >= 6/5: by B1 the round fires a vertex j with l'_j = M; l''_j = M - 1 and nothing else
 changes. j is in Wset, because T_j(t0) >= T_j(now) >= l'_j >= 6/5. Fix k and let A be a set of k
 vertices of Wset carrying the k largest values of l'' in Wset.
  1a, j in A: sum_A l'' = sum_A l' - 1 <= sum_A l <= S_k(t) <= B_k.
  1b, j not in A (so k < n): A + j is a set of k+1 vertices of Wset, so
      sum_{A+j} l'' = sum_{A+j} l' - 1 <= S_{k+1}(t) <= B_{k+1}.
      Every a in A has l''_a = l'_a <= M, so l''_j = M - 1 >= max_A l'' - 1 >= (sum_A l'')/k - 1.
      Hence (1 + 1/k) sum_A l'' <= B_{k+1} + 1, that is
      sum_A l'' <= (k/(k+1)) (B_{k+1} + 1) = k (1 + H_n - H_{k+1} + 1/(k+1)) = B_k.
 (M is 5M-integral, so cases 0 and 1 are exhaustive.) For k = 1: every lag in Wset is
 <= B_1 = H_n.  QED

### B3. Lemma (far vertices cost far chips). Fix a vertex y and a round t. Let e = number of the first t adds that are not in [y-2, y+2], and j = number of z in Wset (taken at t0 = t) with |z - y| >= 4. Then 6j <= 5e; moreover j = 2 implies e >= 4, and j = 4 implies e >= 6.

Proof. Call the adds outside [y-2, y+2] far adds; for this proof k_x counts far adds only (for
|x - y| >= 3 that is all adds at x). For |z - y| >= 4 the vertices z-1, z, z+1 lie outside
[y-2, y+2], so 5 T_z = 3k_z + k_{z-1} + k_{z+1} counts only far adds. One far add at x contributes
to 5 T_z only for z in {x-1, x, x+1}, with weights 1, 3, 1: at most 5 in total. Summing 5 T_z >= 6
over the j vertices: 6j <= 5e.
Each of the j vertices z has 3 k_z + k_{z-1} + k_{z+1} >= 6, so one of:
   k_z >= 2;   or k_z = 1 and k_{z-1} + k_{z+1} >= 3;   or k_z = 0 and k_{z-1} + k_{z+1} >= 6.
j = 2, e <= 3: the third kind needs 6 far adds, the second kind 4. So both vertices are of the
 first kind: 4 far adds. Contradiction; so e >= 4.
j = 4, e <= 5: no vertex is of the third kind, so each of the four has k_z >= 1, and their k's
 sum to 4 or 5. Not all four have k_z >= 2. If the sum is 4, all four have k_z = 1 and at most one
 far add is elsewhere, so every vertex has at most 1 add on each neighbour: k_{z-1} + k_{z+1} <= 2,
 no vertex qualifies. If the sum is 5, one vertex z* has k = 2, the other three have k = 1, and
 there are no other far adds; each of the three needs neighbours with k summing to >= 3, which
 with the available values (2 once, 1 otherwise, 0 elsewhere) means z* is its neighbour; but z*
 has only two neighbours. Contradiction; so e >= 6.  QED

### B4. Lemma (local lag bound). After every round t, for every vertex y, with e as in B3:   2 l_y - e/5 <= 24/5.

Proof. Wset at t0 = t has at most 7 vertices within distance 3 of y, so n <= 7 + j with j as in B3.
If y is not in Wset, l_y <= 1 and the claim holds. Otherwise n >= 1 and by B2,
l_y <= H_n <= H_{7+j}; since 5 l_y is an integer, l_y <= lam_j := the largest multiple of 1/5 that
is <= H_{7+j}. By B3, e >= e_j, where e_j = ceil(6j/5) except e_2 = 4 and e_4 = 6.

    j          0      1      2      3      4      5      6      7      8      9
    H_{7+j}    2.5929 2.7179 2.8290 2.9290 3.0199 3.1032 3.1801 3.2516 3.3182 3.3807
    lam_j      2.4    2.6    2.8    2.8    3.0    3.0    3.0    3.2    3.2    3.2
    e >=       0      2      4      4      6      6      8      9      10     11
    2lam-e/5   4.8    4.8    4.8    4.8    4.8    4.8    4.4    4.6    4.4    4.2

For j >= 10: 2 l_y - e/5 <= g(j) := 2 H_{7+j} - 6j/25; g(10) = 4.479 and
g(j+1) - g(j) = 2/(8+j) - 6/25 < 0, so g(j) < 4.8.  (`arith.py` recomputes the table exactly, with
fractions, and checks j up to 2000 term by term; `out_arith.txt`.)  QED

### B5. Proof of Theorem B.

After round t (t >= 0), for every y: by (b), (a) and eta*_y = (t - e)/5,
    eta_y <= eta*_y + 2 l_y = t/5 + (2 l_y - e/5) <= t/5 + 24/5        (B4).
Right after the add of round t >= 1 a height is at most its value after round t-1 plus 1:
    eta_y <= (t-1)/5 + 24/5 + 1 = t/5 + 28/5.
So at every step of round t every height is <= t/5 + 5.6. If the list has length m <= 5h - 29,
then for t <= m: eta_y <= (5h - 29)/5 + 28/5 = h - 1/5 < h, so eta_y <= h - 1: this line survives
and the list loses. Hence G(h) >= 5h - 28.  QED

Tests (COMPUTED, `test_rule.py`, outputs `out_rule_main.txt`, `out_rule_controls.txt`): B1, B2
(with Wset recomputed at each checked round), the identity (b), and the height bounds
5 eta <= t + 24 after a round and <= t + 28 after an add, asserted along every play. The runs are
far from making the height bound binding (see the report); they test the bookkeeping of B1 and
B2 (the top-k bound is met with equality), not B3, B4 or the theorem.

---------------------------------------------------------------------------------------------

## Corollary (PROVED, from A and B, with P2 of `solver-b/proofs.txt`)

For every h >= 4:   max(2h - 1, 5h - 28) <= G(h) <= C(h) <= 5h - 10.

So the constant c in G(h) = 5h - c lies between 10 and 28 for every h >= 4 (it is 10 for h = 4, 5
by the earlier exhaustive searches). NOT established here: any exact value of G(h) for h >= 6.
