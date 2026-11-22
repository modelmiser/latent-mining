# Experiment 16, attempt A: Conjecture P1 is false; what holds instead

Model, notation and the facts F1, F2, Theorem E, Case A / Case B as in
`16-erasing-counter/upper/PROOFS.md` (called [U] below). "Tree" = any finite tree of the model.
W_0 = 1, W_r = W_(r-1)(W_(r-1)+1). L(leaf) = 1, L(node) = L(1-child)(L(0-child)+1). M(G) = longest cycle of G.

Spine notation, for a tree T that is not a leaf: z_1, ..., z_k (k >= 1) are the indices on the all-zero
path, λ0 its leaf, S_i the subtree at the 1-child of the spine node z_i, Z = {z_1..z_k}.
A state is *home* if all spine bits are 0; it is *in mode i* if z_i is its only 1 on the spine.
Unrolling the definition of L along the spine, with ℓ_i = L(S_i) and P_i = ℓ_1 ℓ_2 ... ℓ_i:

    L(T) = P_1 + P_2 + ... + P_k + P_k  >=  k + 1.                                   (L-spine)

(The subtree at the 0-child of spine node z_i has L = ℓ_(i+1)(L(next)+1), and L(λ0) = 1.)

## 1. Counterexample to P1 (depth 5, 7 indices)

    T* = (1 (2 (3 [2] (4 [4] (5 [5] []))) [1]) (3 (6 [6] (7 [7] [3])) []))

Spine 1, 2, 3; λ0 = [2]; S_1 = (3 (6 [6] (7 [7] [3])) []), S_2 = [1] (a leaf), S_3 = (4 [4] (5 [5] [])).
Depth 5 (path 1, 2, 3, 4, 5). L(S_3) = 2·2 = 4, L(S_2) = 1, L(S_1) = 1·(4+1) = 5, so by (L-spine)
L(T*) = 5 + 5 + 20 + 20 = 50. The orbit of the all-zero state is a cycle of length **52 > 50**.

By hand: write x = bits (6,7), y = bits (4,5). Both (6 [6] (7 [7] ·)) and (4 [4] (5 [5] [])) step a
2-bit memory through 00 → 10 → 01 → 11 → 00, and reach their last leaf only from 11.
- home (x, y) → mode 2, same data (λ0 writes 2);
- mode 2 → mode 1, same data (the leaf [1] erases bit 2 and writes bit 1);
- mode 1 reads 1, 3 (which is 0), then steps x; from x ≠ 11 it writes 6 or 7 → home;
  from x = 11 it writes 3 → mode 3 with x = 00;
- mode 3 reads 1, 2, 3 and steps y → home.
One round of x: 4·3 states (home, mode 2, mode 1) + 1 state in mode 3 = 13 states, and y advances once
per round: 4 rounds, 52 pairwise distinct states (distinct mode or distinct (x, y)).

Mechanically: `check.py cex_small.txt` (from the definition) and `allcycles.py` (second program, full
functional graph, 128 states): one cycle, length 52. Same family, verified the same way:
`cex_mid.txt` 114 > 98 (9 indices), `cex_big.txt` 762 > 602 (13 indices), both depth 5.

General form: data trees with cycles c_1 (under S_1, behind a node reading z_3, 1-child a leaf) and c_3
(S_3), both tight for L: N = c_3(3c_1 + 1), L = (c_1 + 1)(2c_3 + 2), N − L = c_1c_3 − 2c_1 − c_3 − 2.
Why L undercounts: the leaf S_2 = [1] has L = 1 but is used once per home state. It costs one level of
depth for everything below it on the spine, which L does not see and W_depth does.
P1 is computed true for depth <= 4 in [U]; in this family depth 4 forces c_3 <= 2 and N − L < 0.

P1 is therefore FALSE. The headline conjecture D(r) = W_r is not affected: 52 << W_5, and Theorem Z
below shows that the whole class these examples live in obeys the W_depth bound.

## 2. Lemma A (all trees): cycles that avoid the home states are short

**Lemma A.** Let T be any tree with spine length k >= 1, and C a cycle of f_T of length N >= 2 with
no home state. Then N <= k (indeed N <= k − c + 1, c the number of 1s on the spine). In particular
N < L(T), by (L-spine).

Proof. By [U] Case A the weight is constant on C and every step reads at least one 1; by F1 a step
reading c' ones changes the weight by 1 − c' or −c', so every step reads exactly one 1 and writes one
bit. The 1 it reads is the first 1 on the spine, z_i (the walk follows the all-zero path up to it).
By F2 the number Z(s) of spine ones never increases along C, so it is constant, = c >= 1; the step
erases z_i, so the bit it writes is a spine bit z_j, and it was 0 or j = i (a written index was read,
and the only 1 read is z_i); j = i would make s a fixed point. No data bit changes. So with
B(s) = {i : s_(z_i) = 1}, |B| = c, each step removes min B and adds one j not in B, and the states of
C are determined by B.
Let p be the largest position that occurs in any B on C. If c >= 2, a step can remove p only when
p = min B, and then the other elements of B exceed p, contradicting the choice of p. So once p is in B
it stays; C is periodic, so p is in every B and is never removed. Then B' = B \ {p} has min B' = min B
and obeys the same rule (remove the minimum, add an element not present), with |B'| = c − 1. Repeat:
after c − 1 rounds a fixed set F of c − 1 positions lies in every B, and B = F ∪ {q} with q the moving
element. The states of C are distinct, so the values of q are distinct and avoid F: N <= k − c + 1. ∎

Consequence: any violation of P1, or of the W_depth bound (k <= depth <= W_depth), is a cycle through
a home state.

## 3. Theorem Z: spine sharing with disjoint data

Hypothesis (H) on T: the *data sets* D_i = idx(S_i) \ Z, i = 1..k, are pairwise disjoint.
(S_i may read spine indices z_j, j > i, and its leaves may write any spine index on their path. The
S_i need not be read-once.) Let S_i° be S_i with every node whose index is in Z replaced by its
0-child (repeatedly), and every leaf that writes an index of Z made empty. S_i° is a tree of the model
on D_i: paths keep distinct indices, and a leaf that still writes, writes a data index of its path,
which pruning keeps. g_i = f_(S_i°) on {0,1}^(D_i).

**Theorem Z1.** Let T satisfy (H) and let C be a cycle of length N >= 2 containing a home state.
Then there are distinct modes m_1, ..., m_l and cycles C_(m_u) of g_(m_u), of lengths c_u, with

    N <= 2·C_1 + C_2 + ... + C_l,   where C_t = c_t c_(t+1) ... c_l.

Moreover m_1 is the mode written by λ0, and all steps of C that go from mode m_u directly to a mode
start from one and the same value of the data of S_(m_u) and go to the same mode (m_(u+1) if u < l).

Proof. By [U] Case B, λ0 writes some z_(j0) and every state of C is home or in a mode; in a mode-i
state every spine bit except z_i is 0. Bits outside idx(T) are constant. Write a state as (μ, x),
μ = 0 (home) or the mode, x = (x_1..x_k), x_i its restriction to D_i.

(1) Steps. (0, x) → (j0, x): the step reads only the spine, all 0, and writes z_(j0).
Let (i, x) be a state of C. Its step reads z_1..z_i and walks in S_i. At a node of S_i with a spine
index the bit is 0 (z_i does not recur below itself) and the walk takes the 0-child; at a data node it
reads a bit of x_i. So the walk is the walk of S_i° on x_i and ends at the corresponding leaf. All
bits read are erased: z_i, spine bits that were 0, and the data bits on the S_i°-path. If the leaf
writes a data index or nothing, the next state is (0, x') ; if it writes z_j, the next state is (j, x')
(j = i is possible); in each case x'_i = g_i(x_i) and x'_u = x_u for u ≠ i, by (H): S_i reads no bit
of D_u. A leaf writing z_j is empty in S_i°. So the next mode is ν_i(x_i), a function of x_i alone,
nonzero only if the S_i°-step from x_i is empty.

(2) Cycles C_i. Call i *visited* if C has a mode-i state. x_i changes only at mode-i steps, by g_i.
Going round C repeatedly, the values of x_i at successive mode-i states form a periodic sequence
a, g_i(a), g_i²(a), ..., so they lie on one cycle C_i of g_i, and x_i is in C_i at every state of C.
If i is not visited, x_i is constant on C.

(3) One jump state per mode. By Theorem E of [U] applied to S_i° and C_i (trivially if |C_i| = 1) at
most one state e_i of C_i has an empty S_i°-step. So ν_i is nonzero at most at e_i. If it is, call i
a *jumper*, j(i) = ν_i(e_i), x*_i = g_i(e_i).

(4) Stages. Since C has a home state, every non-home state s of C is reached from the last home state
h before it by t >= 1 steps: call s a stage-t state. Stage 1 is in mode m_1 = j0. If a stage-t state
is in mode μ and stage t+1 exists, the step was not to home, so by (3) x_μ = e_μ, μ is a jumper and
stage t+1 is in mode j(μ). Hence stage t is in mode m_t, where m_(t+1) = j(m_t), and every earlier
stage jumped. Let l be the largest number with m_1, ..., m_l distinct and defined; every stage is in a
mode m_τ with τ <= l (take the first occurrence of its mode in the sequence), so exactly the modes
m_1..m_l are visited and x_i is constant on C for the other i.

(5) Claim: a stage-t state whose mode first occurs at position τ has x_(m_u) = x*_(m_u) for all u < τ.
Stage u (u < τ <= t) was in mode m_u and jumped, which set x_(m_u) = g(e_(m_u)) = x*_(m_u). Between
stage u and stage t, x_(m_u) changes only at stages u' with u < u' < t in mode m_u; each of these
jumped, so it found x_(m_u) = e_(m_u) and left it at x*_(m_u) again. Stage t itself is in mode
m_τ ≠ m_u. ∎(claim)

(6) Count. So every non-home state of C lies in one of the sets
Ω_τ = {(m_τ, x) : x_(m_u) = x*_(m_u) for u < τ, x_(m_u) in C_(m_u) for τ <= u <= l, other x_i fixed},
τ = 1..l, of size C_τ, and every home state in {(0, x) : x_(m_u) in C_(m_u), other x_i fixed}, of
size C_1. N <= 2C_1 + C_2 + ... + C_l. ∎

**Definition (class 𝒞).** A leaf is in 𝒞. A tree T with k >= 1 is in 𝒞 if it satisfies (H) and every
S_i° is in 𝒞. (Well founded: S_i° has fewer nodes.)

**Theorem Z.** Every tree in 𝒞 of depth <= r has all cycles of length <= W_r. More generally, if T
has depth <= r, satisfies (H), and M(S_i°) <= W_(depth(S_i°)) for all i, then M(T) <= W_r.

Proof. Induction on the number of nodes; a leaf has only fixed points. Let T have depth <= r, so
k <= r, and S_i (its root has i ancestors) has depth <= r − i; pruning does not increase depth. So
|C_i| <= M(S_i°) <= W_(r−i) =: w_i (induction for 𝒞, hypothesis otherwise; W is increasing), with
w_1 > w_2 > ... > w_k >= 1. A cycle of length 1 is fine; one without home state has length <= k <= r
<= W_r by Lemma A. Otherwise Theorem Z1 gives N <= 2C_1 + C_2 + ... + C_l. Each C_t is a product of
l − t + 1 numbers c_u <= w_(m_u) with distinct m_u, hence at most the product of the l − t + 1 largest
of w_1..w_k:  C_t <= w_1 w_2 ... w_(l−t+1). Therefore, as l <= k and all w >= 1,

    N <= Σ_(q=1..l) w_1...w_q + w_1...w_l  <=  Σ_(q=1..k) w_1...w_q + w_1...w_k.

Unrolling W_j = W_(j−1) + W_(j−1)·W_(j−1) k times:
W_r = Σ_(q=1..k) w_1...w_q + (w_1...w_k)·W_(r−k), and W_(r−k) >= 1. So N <= W_r. ∎

**𝒞 strictly contains the read-once trees.** If T is read-once the sets idx(S_i) are pairwise
disjoint and contain no spine index, so (H) holds, and S_i° is S_i with some leaves emptied, again
read-once; induction. Not read-once but in 𝒞: examples (a) and (b) of [U] Section 5
((1 (2 [1] []) (2 [2] [2])): k = 2, D_1 = D_2 = ∅; (b): D_1 = {4}, D_2 = {3}) and T* above
(D_1 = {6,7}, D_2 = ∅, D_3 = {4,5}; the S_i° are read-once). So on 𝒞 the bound W_depth is proved
while the shape bound L(T) is false.

With the computation in [U] (M <= W_depth for every tree of depth <= 4), the "more generally" clause
covers every tree of depth <= r satisfying (H) whose S_i° have depth <= 4 or lie in 𝒞, whatever sharing
happens inside those S_i°. That clause rests on the computation; Theorem Z for 𝒞 does not.

Remark (order of the chain). In the extremal construction the chain is m = (k, k−1, ..., 1): only
jumps to smaller modes, which need no extra node. A jump from mode i to j > i needs a node reading z_j
inside S_i. T* uses the chain (2, 1, 3).

## 4. What is used from [U]

F1, F2, the Case A / Case B dichotomy, and Theorem E (for the S_i° in step (3) only).
Nothing else. Theorem RD/RO are not used.
