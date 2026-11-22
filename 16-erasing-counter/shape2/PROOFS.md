# Experiment 16, attempt B: Conjecture P1 is false; a bound for mode-disjoint trees

Model and notation as in `16-erasing-counter/chain/3-question.txt` and `upper/PROOFS.md` (cited as
[U]): tree text `(i T0 T1)`, `[]`, `[i]`; L(leaf) = 1, L(node) = L(1-child)·(L(0-child)+1);
W_0 = 1, W_r = W_(r-1)(W_(r-1)+1); M(G) = longest cycle of f_G; D(r) = max of M over depth ≤ r.
Used from [U]: facts (F1), (F2) and Theorem E (a cycle has at most one empty step), all trees.

For a tree T let Z = (z_1..z_k) be the indices on the all-zero path, λ0 its leaf, S_i the subtree at
the 1-child of the node z_i. Unrolling the definition of L along the all-zero path, with ℓ_i = L(S_i):

    L(T) = ℓ_1 + ℓ_1ℓ_2 + ... + ℓ_1···ℓ_k + ℓ_1···ℓ_k.                                   (U)

(Induction on k: L(T) = ℓ_1 (L(T') + 1), T' the 0-child of the root; L(λ0) = 1 = empty sum + empty
product.)

## 1. Proposition X: Conjecture P1 is false, and cycle/L(T) is unbounded

Let K be any tree of the model with a cycle of length c, and q ≥ 1. Let z_1..z_(q+1) be indices not
in idx(K) and let T(q, K) be the tree whose all-zero path is z_1..z_(q+1), with

    λ0 = [z_q];   S_1 = (z_(q+1) [z_(q+1)] []);   S_i = [z_(i-1)] for 2 ≤ i ≤ q;   S_(q+1) = K.

T(q, K) is a tree of the model: no index repeats on a path (z_(q+1) is read in S_1 below z_1 only;
K has other indices), every leaf writes nothing or an index of its path (λ0: z_q; S_1: z_(q+1), read
just above; S_i: z_(i-1), on the all-zero path above; leaves of K as in K). Its depth is
q + 1 + depth(K).

**Claim.** T(q, K) has a cycle of length (q+2)·c, and L(T(q, K)) = 2q + 4·L(K).

Proof. Let y_0, ..., y_(c-1) be the cycle of K (states restricted to idx(K)). Write (i, y) for the
state with z_i = 1 as the only 1 on Z and K-memory y, and (0, y) for the state with Z all zero.
From the definition of a step:
(0, y) → (q, y) (all-zero path, λ0 writes z_q);
(i, y) → (i-1, y) for 2 ≤ i ≤ q (reads z_1..z_i, erases z_i, the leaf S_i writes z_(i-1));
(1, y) → (q+1, y) (reads z_1 = 1, then z_(q+1) = 0, writes z_(q+1));
(q+1, y) → (0, f_K(y)) (reads z_1..z_q = 0, z_(q+1) = 1, then walks K on y; the leaf of K writes
nothing or an index of K; z_(q+1) is erased).
So the orbit of (0, y_0) is (0,y_0), (q,y_0), ..., (1,y_0), (q+1,y_0), (0,y_1), ...: q + 2 states for
each y_t, all pairwise distinct, returning to (0, y_0) after c rounds. Length (q+2)c.
L: by (U) with ℓ_1 = 2, ℓ_2 = ... = ℓ_q = 1, ℓ_(q+1) = L(K): q terms equal to 2, then 2L(K) twice. ∎

Take K read-once with a cycle of length c = L(K) (exists for every shape, [U] Theorem RO). Then
(q+2)c > 2q + 4c iff (c-2)q > 2c: e.g. c = 6, q = 4: cycle 36 > L = 32 (depth 7); c = 4, q = 5:
28 > 26. With c = q → ∞ the ratio cycle/L is (q+2)q/(6q) → ∞. So P1 fails, with unbounded ratio.

Smaller-depth counterexamples (same mechanism, mode 1 carrying a counter as well; verified by
`sim.py`, a checker written from the definition, output `out_cex.txt`):

    A (depth 5): (1 (2 (3 [2] (6 (7 [7] [6]) (8 [8] []))) [1]) (3 (4 (5 [5] [4]) [3]) []))   cycle 60, L = 56
    B (depth 5): (1 (2 (3 [2] (6 [6] (7 [7] []))) [1]) (3 (4 [4] (5 [5] [3])) []))           cycle 52, L = 50

Depth 5 is the least possible, since [U] computed P1 for all trees of depth ≤ 4.

Why P1 fails: L charges the subtree S_i at position i of the all-zero path by the product
ℓ_1···ℓ_i, i.e. it assumes the modes are used in the order k, k-1, ..., 1 (deepest most often).
A leaf may jump to a *deeper* mode after reading that mode's index, at the price of one extra unit
in its ℓ. So a cheap chain of "pass-through" modes (ℓ = 1) placed shallow can be traversed once per
step of a big counter placed deep; L charges them ℓ_1 each instead of ℓ_1·L(K) each.
None of this exceeds W_depth: the counterexamples are thin, deep trees.

## 2. Theorem MD: trees whose modes have disjoint data

Data indices: idx(T) \ Z. Put D_i = idx(S_i) \ Z. (S_i can contain z_j only for j > i.)
G_i is S_i with every node whose index is in Z replaced by its 0-child (repeatedly), and then every
leaf that writes an index of Z made empty. G_i is a tree of the model with idx(G_i) ⊆ D_i (paths of
G_i are subsequences of paths of S_i; a leaf of G_i writes nothing or a data index of its S_i-path,
which is kept), and depth(G_i) ≤ depth(S_i). g_i = f_(G_i).

**Hypothesis (MD):** the sets D_1, ..., D_k are pairwise disjoint.
(Read-once trees satisfy it. So do example (b) of [U] §5 and all trees of §1 when K ∩ nothing is
shared; (MD) allows any reuse of the indices z_j inside the S_i, and any sharing inside one S_i.)

**Theorem MD.** Let T satisfy (MD) and let C be a cycle of f_T of length ≥ 2. Then either
(A) no state of C has all Z-bits 0, the data bits are constant on C, and |C| ≤ 2^k; or
(B) there are p ≥ 1 pairwise distinct i_1, ..., i_p in {1..k} and cycles C_(i_t) of g_(i_t), of
lengths c_t = |C_(i_t)| ≤ M(G_(i_t)), such that, with n_j = number of states of C whose only 1 on Z
is z_(i_j) and H = number of states of C with Z all zero:

    H ≤ c_1 c_2 ··· c_p,     n_j ≤ c_j c_(j+1) ··· c_p   (1 ≤ j ≤ p),
    |C| = H + n_1 + ... + n_p ≤ c_1···c_p + Σ_(j=1..p) c_j···c_p.

Proof.
(A) Suppose no state of C has Z(s) = 0. As in [U] Theorem E Case A and Theorem G: no step ends at
λ0, so by (F1) no step raises the weight, the weight is constant, and every step reads exactly one 1
and writes one bit. The 1 it reads is the first 1 on Z; Z(s) cannot increase (F2) and is periodic,
hence constant, so the written bit is on Z. All data bits read are 0 and stay 0, none is written:
the data are constant on C, the states differ on Z only, |C| ≤ 2^k.

(B) Suppose some state of C has Z = 0. As in [U] Theorem E Case B: λ0 writes some z_(j0); every
state of C has at most one 1 on Z; a state is *home* if Z = 0 and *in mode i* if its only 1 on Z is
z_i. Bits outside idx(T) never change. So a state of C is given by its mode i ∈ {0, 1..k} (0 = home)
and x = (x_1, ..., x_k), x_i its restriction to D_i; by (MD) these restrictions are independent
coordinates covering all data bits.

Step 1 (what a step does). A home step changes no data and leads to mode j0. Consider a state in
mode i ≥ 1. Its step reads z_1..z_i and walks in S_i. Every z_j met there has j ≠ i, so is 0, and
the walk takes its 0-child; every other node met has its index in D_i. Hence the walk in S_i visits
exactly the nodes that survive in G_i along the walk of G_i on x_i, and ends at the leaf of S_i
corresponding to the leaf of G_i reached by x_i. That leaf is of one of three kinds:
 (α) it writes some z_j: the next state is in mode j; the leaf is empty in G_i;
 (β) it writes a data index (in D_i, on the path): the next state is home; not empty in G_i;
 (γ) it writes nothing: the next state is home; empty in G_i.
In all three cases the data read are erased and the leaf of G_i acts as in G_i, so the next state has
x_i replaced by g_i(x_i) and all other coordinates x_l (l ≠ i) unchanged (they are disjoint from
D_i, and a z written is not a data bit).

Step 2 (each coordinate runs on one cycle of g_i). Call mode i *visited* if C has a state in mode i;
let v_i ≥ 1 be their number. By Step 1, x_i changes only at mode-i steps, and there by g_i. Going
once round C from any state, g_i^(v_i)(x_i) = x_i. So every value of x_i on C lies on one cycle C_i
of g_i; c_i := |C_i| ≤ M(G_i). For a mode not visited, x_i is constant on C.

Step 3 (overflow). By Theorem E applied to the tree G_i, at most one state e_i of C_i has its
g_i-step ending at an empty leaf. So a mode-i state of C with x_i ≠ e_i is of kind (β): the next
state is home. A mode-i state with x_i = e_i (an *overflow*) always reaches the same leaf of S_i
(the walk depends on x_i only), so it is always followed by the same mode a(i) ∈ {0, 1..k}, and
afterwards x_i = g_i(e_i).

Step 4 (the chain). Let s be a state of C in mode i ≥ 1, and go back along C to the last home state
h before s. The states strictly between h and s are in modes μ_1, ..., μ_(n-1), and s in mode μ_n = i,
with μ_1 = j0 and, since each of these states is followed by a mode state, each is an overflow and
μ_(l+1) = a(μ_l). So the sequence μ is the forward orbit of j0 under a. Define i_1 = j0 and
i_(j+1) = a(i_j) as long as a(i_j) is defined (mode i_j visited with an overflow state on C), nonzero,
and different from i_1..i_j; this gives pairwise distinct i_1, ..., i_q. Then μ_l = i_l for l ≤ q, and
every later μ_l is one of i_1..i_q. Consequences:
 (a) every visited mode is some i_j; and if i_j is visited then i_1, ..., i_(j-1) are visited. So the
     visited modes are exactly i_1, ..., i_p for some 1 ≤ p ≤ q.
 (b) if s is in mode i_j, then for every t < j the mode i_t occurs among μ_1..μ_(n-1) (the orbit
     reaches i_j only after i_1..i_(j-1)). The last mode-i_t state between h and s was an overflow,
     which set x_(i_t) = g_(i_t)(e_(i_t)), and x_(i_t) has not changed since (only mode-i_t steps
     change it). So in every mode-i_j state of C:  x_(i_t) = g_(i_t)(e_(i_t)) for all t < j.

Step 5 (counting). Distinct states of C in the same mode differ in x. In a mode-i_j state the
coordinates of unvisited modes have their constant value, the coordinates i_t, t < j, are fixed by
(b), and each coordinate i_t, j ≤ t ≤ p, lies in C_(i_t). So n_j ≤ c_j···c_p. In a home state every
coordinate i_t lies in C_(i_t) and the others are constant: H ≤ c_1···c_p. Every state of C is home
or in a visited mode. ∎

Remarks. (i) The bound is attained: for tree A of §1 the chain is 2, 1, 3 with c = 1, 3, 6 and
18 + (18 + 18 + 6) = 60. (ii) Without (MD) Step 1 fails (a mode-i step changes data another mode
reads), and Steps 2–3 fail with it; `mdcheck.py` with MUT=1 shows this happening.

## 3. Corollaries

Lemma S. Let d_1 ≥ d_2 ≥ ... ≥ d_r ≥ 1 be integers, i_1..i_p pairwise distinct in {1..r} and
1 ≤ c_t ≤ d_(i_t). Then
    c_1···c_p + Σ_(j=1..p) c_j···c_p  ≤  d_1···d_r + Σ_(n=1..r) d_1···d_n.
Proof. The j-th product runs over n = p-j+1 distinct indices a_1 < ... < a_n of {1..r}; a_l ≥ l, so
d_(a_l) ≤ d_l and the product is ≤ d_1···d_n. Different j give different n ≤ p ≤ r, and all
omitted terms are ≥ 0. The first product is ≤ d_1···d_p ≤ d_1···d_r. ∎

Lemma V. W_r = W_(r-1)W_(r-2)···W_0 + Σ_(n=1..r) W_(r-1)···W_(r-n).
Proof. Call the right side V_r. V_0 = 1. Factoring W_(r-1) out: V_r = W_(r-1)(V_(r-1) + 1). ∎

**Corollary MD-r.** Let T have depth ≤ r and satisfy (MD), and suppose M(G_i) ≤ W_(r-i) for every i
(true in particular if D(ρ) = W_ρ for all ρ < r, since depth(G_i) ≤ depth(S_i) ≤ r - i and D is
non-decreasing). Then every cycle of T has length ≤ W_r.
Proof. Cycles of length 1: clear. Case (A): 2^k ≤ 2^r ≤ W_r (W_r ≥ 2W_(r-1)). Case (B): Lemma S
with d_m = W_(r-m), then Lemma V. ∎

**Corollary K (unconditional, recursive class).** Define the class K: a leaf is in K; a tree T is in
K if it satisfies (MD) and every G_i is in K. Then every tree in K of depth ≤ r has all cycles of
length ≤ W_r. K contains all read-once trees (for a read-once tree, (MD) holds and G_i is S_i with
some leaves emptied, again read-once), and K is strictly larger: it contains example (b) of [U] §5
and trees A, B of §1, none read-once.
Proof. Induction on r: G_i ∈ K has depth ≤ r - i < r, so M(G_i) ≤ W_(r-i); apply Corollary MD-r. ∎

**Corollary K4 (uses the repository's computation D(ρ) = W_ρ for ρ ≤ 4).** The same holds for the
larger class K4: T ∈ K4 if depth(T) ≤ 4, or T satisfies (MD) and every G_i ∈ K4. In particular every
tree of depth ≤ 5 satisfying (MD) has all cycles ≤ W_5 = 3263442 (here every G_i has depth ≤ 4).

**Corollary K\* (combining with [U] Theorem RD; also uses D(ρ) = W_ρ for ρ ≤ 4).** Let K\* be the
smallest class such that T ∈ K\* if (1) depth(T) ≤ 4, or (2) T satisfies (MD) and every G_i ∈ K\*, or
(3) T = (a T0 T1) with idx(T0) ∩ idx(T1) = ∅ and T0°, T1° ∈ K\* (T0°, T1° as in [U] §2: leaves writing
a made empty). Then every tree in K\* of depth ≤ r has all cycles ≤ W_r.
Proof. Induction on the construction. (1) computed. (2) Corollary MD-r. (3) T0°, T1° have depth
≤ r-1, so by Theorem RD M(T) ≤ M(T1°)(M(T0°)+1) ≤ W_(r-1)(W_(r-1)+1) = W_r. ∎
K\* is strictly larger than K and than K4: e.g. T = (a T0 []) with T0 of depth 4 in which two
subtrees off its all-zero path share a data index is in K\* by (3), and fails (MD) at the root.
This is a modest enlargement: rule (3) only tolerates data shared between S_i, S_j (i, j ≥ 2) when
S_1 is index-disjoint from the rest, and rule (1) only at depth ≤ 4.

Relation to [U] Theorem RD: neither contains the other. RD needs idx(T0) ∩ idx(T1) = ∅ at the root
only (so S_1 may not read any z_j, but S_2..S_k may share data among themselves); (MD) lets every S_i
read the z_j but needs the data of all S_i disjoint.

## 4. Theorem NC: the same bound for every cycle on which the modes do not communicate through data

Theorem MD is a statement about a class of trees. Its proof uses less: only what happens on the cycle.
Let T be ANY tree and C a cycle of length ≥ 2 containing a home state (so, as in §2 (B), every state
of C is home or in a mode). For a mode i visited by C define
  R_i  = the data indices at nodes visited by the mode-i steps of C,
  ch_i = the data indices whose value is changed by some mode-i step of C.
**Hypothesis (NC) on C:** ch_i ∩ R_j = ∅ for all visited modes i ≠ j.
("No mode changes a data bit that another mode reads, on this cycle." The subtrees S_i may share any
indices; (MD) implies (NC) for every cycle, since then ch_i ⊆ D_i and R_j ⊆ D_j.)

Let G_i^R be S_i with every node whose index is not in R_i replaced by its 0-child (repeatedly), and
then every leaf whose written index is not in R_i made empty. It is a tree of the model on R_i (paths
are subsequences of paths of S_i; a leaf that still writes, writes a data index u ∈ R_i, which sits at
a node of S_i on its path, and that node is kept), of depth ≤ depth(S_i).

**Theorem NC.** If C satisfies (NC), the conclusion (B) of Theorem MD holds for C, with C_(i_t) a
cycle of f_(G^R_(i_t)): there are distinct visited modes i_1..i_p and c_t ≤ M(G^R_(i_t)) with
H ≤ c_1···c_p, n_j ≤ c_j···c_p, |C| ≤ c_1···c_p + Σ_j c_j···c_p.

Proof. Write y_i for the restriction of a state to R_i, g_i for the map of G_i^R.
Step 0. A home step changes no data bit. A mode-i step changes only bits it reads, so ch_i ⊆ R_i.
Hence a data bit outside R = ∪ R_i is constant on C, and by (NC) a bit of R_i is changed only by
mode-i steps.
Step 1. Let s ∈ C be in mode i. The nodes its step visits inside S_i carry an index of Z, read as 0
(z_i does not recur, all other z are 0), where the walk takes the 0-child, or a data index, which
then is in R_i by definition. So the walk in S_i is the walk of G_i^R on y_i and ends at the
corresponding leaf. That leaf (α) writes some z_j — next state in mode j, the leaf is empty in
G_i^R; or (β) writes a data index, which is on the visited path, so in R_i — next state home, leaf
not empty in G_i^R; or (γ) writes nothing — next state home, leaf empty. In all cases the next state
has y_i replaced by g_i(y_i) (the data read are erased; the write, if in R_i, is that of the leaf of
G_i^R), and by Step 0 every y_j, j ≠ i, is unchanged.
Steps 2–4 are now word for word Steps 2–4 of Theorem MD with x_i, G_i replaced by y_i, G_i^R:
y_i changes only at mode-i steps, there by g_i, so its values lie on one cycle C_i of g_i; by
Theorem E (for G_i^R) at most one state e_i of C_i overflows, an overflow is always followed by the
same mode a(i) and leaves y_i = g_i(e_i), a non-overflow is followed by home; the modes between a
home state and a later state follow the orbit of j0 under a; the visited modes are a chain
i_1, ..., i_p, and in every mode-i_j state y_(i_t) = g_(i_t)(e_(i_t)) for t < j.
Step 5. Two distinct states of C in the same mode differ in some data bit, hence (Step 0) in some
bit of R, hence in some y_i with i visited. So n_j ≤ c_j···c_p and H ≤ c_1···c_p as before. ∎

**Corollary NC-r.** Let T have depth ≤ r and suppose D(ρ) ≤ W_ρ for all ρ < r. Then every cycle of T
that has no home state, or that satisfies (NC), has length ≤ W_r. (Proof as Corollary MD-r:
depth(G_i^R) ≤ r - i.) With the repository's D(ρ) = W_ρ, ρ ≤ 4: **in a tree of depth 5, a cycle longer
than W_5 = 3263442 must pass through a home state and contain two modes i ≠ j and a data index that
a mode-i step changes and a mode-j step reads.** By induction on r the same localisation holds at
the first depth r at which D(r) > W_r, if there is one.

Mechanical check: `nccheck.py` (`out_nc.txt`; failing on purpose: `out_nc_mut1.txt`, `out_nc_mut2.txt`).

## 5. Theorem T: private prefix modes that may jump into a tail with arbitrary internal sharing

Fix 1 ≤ i ≤ k. The *tail* Θ is the subtree of T reached from the root by i 0-edges (its all-zero path
is z_(i+1)..z_k, its all-zero leaf is λ0; Θ = λ0 if i = k). Modes 1..i are *prefix* modes, modes > i
*tail* modes. D_m = idx(S_m) \ Z for m ≤ i, D_Θ = idx(Θ) \ Z. G_m as in §2.

**Hypothesis (MD_i) on T:** D_1, ..., D_i, D_Θ are pairwise disjoint.
(Nothing is assumed inside Θ: S_(i+1), ..., S_k may share and exchange data freely. The prefix S_m
may read any spine index, also those of Θ. i = k is (MD). For i = 1, (MD_1) is weaker than the
hypothesis of [U] Theorem RD: S_1 may read spine indices of T0.)
**Hypothesis (EC) on a cycle C through home:** there is one tail mode j* such that every step of C
from a home state or a prefix-mode state to a tail-mode state goes to mode j*.
(Tree-level sufficient condition: λ0 and the leaves of S_1..S_i write at most one index among
z_(i+1)..z_k.)

Θ* is Θ with λ0 replaced by [z_(j*)] and every leaf that writes some z_m, m ≤ i, made empty. It is a
tree of the model on idx(Θ) (z_(j*) is on the all-zero path of Θ; every other leaf writes nothing or
an index of its path inside Θ), depth(Θ*) ≤ depth(Θ). A state of Θ* is *Θ-home* if all of
z_(i+1)..z_k are 0.

**Theorem T.** Let T satisfy (MD_i), and let C be a cycle of length ≥ 2 through a home state that
satisfies (EC). Then there are pairwise distinct *units* u_1, ..., u_p, each a prefix mode or the
symbol Θ, with: for a prefix unit u a number c_u ≤ M(G_u); if Θ is a unit (u_τ = Θ), a cycle C_Θ of
f_(Θ*) with c_Θ = |C_Θ| ≤ M(Θ*), of which H_Θ states are Θ-home, 2H_Θ ≤ c_Θ; and, putting w_t = c_(u_t)
for prefix units and w_τ = H_Θ:
    #home states ≤ w_1···w_p;   #states in prefix mode u_j ≤ w_j···w_p;
    #states in tail modes ≤ (c_Θ − H_Θ)·w_(τ+1)···w_p   (0 if Θ is not a unit).

Proof. Every state of C has at most one 1 on Z (as in §2 (B)); it is home, in a prefix mode, or in a
tail mode. Besides z_1..z_i, a state is given by y_m = restriction to D_m (m ≤ i) and θ = restriction
to idx(Θ); by (MD_i) these are independent coordinates. θ is Θ-home unless the state is in a tail
mode. λ0 writes z_(j0).

Step 1 (steps).
(h) Home: writes z_(j0), nothing else changes. If j0 ≤ i the next state is in prefix mode j0 and θ
is unchanged. If j0 > i then j0 = j* by (EC), and θ, which was Θ-home, gets z_(j*) = 1: this is
f_(Θ*)(θ) (Θ* reads its all-zero path, all 0, and writes z_(j*)).
(p) Prefix mode m: exactly as Step 1 of Theorem MD, the walk in S_m is the walk of G_m on y_m, y_m
becomes g_m(y_m), the other y are unchanged. The spine bits of Θ read on the way were 0 and stay 0,
and no bit of D_Θ is read (MD_i). The leaf writes: some z_j, j ≤ i (next state in prefix mode j, θ
unchanged); or z_j with j > i, then j = j* by (EC) and θ becomes f_(Θ*)(θ) as in (h); or a data
index of D_m, or nothing (next state home, θ unchanged). In the first two cases the leaf is empty
in G_m, in the third not.
(t) Tail mode j > i: the step reads z_1..z_i = 0 and then is the step of the tree Θ on θ: it reads
only indices of Θ, so no y_m changes. Its leaf is not λ0. If it writes an index of Θ or nothing, θ
becomes f_Θ(θ) = f_(Θ*)(θ); the next state is in a tail mode or home. If it writes z_m, m ≤ i, then θ
becomes θ with the bits read erased, which is f_(Θ*)(θ) (that leaf is empty in Θ*), and is Θ-home;
the next state is in prefix mode m.
So y_m changes exactly at mode-m steps, by g_m; θ changes exactly at tail-mode steps and at steps
that enter a tail mode, each time by f_(Θ*).

Step 2 (cycles and overflow). As in §2: the values of y_m (m visited) lie on one cycle C_m of g_m,
c_m = |C_m|; at most one e_m ∈ C_m overflows; a mode-m state with y_m ≠ e_m is followed by home; one
with y_m = e_m is always followed by the same thing a(m) — home, a prefix mode, or a tail mode (then
j*) — and leaves y_m = g_m(e_m).
Suppose C has a tail-mode state. θ is periodic and moves by f_(Θ*), so its values on C form one
cycle C_Θ of f_(Θ*); let H_Θ be the number of Θ-home states on it. A Θ-home state is followed on C_Θ
by a state with z_(j*) = 1, so c_Θ ≥ 2H_Θ. By Theorem E for Θ*, at most one state ε of C_Θ has an
empty Θ*-step. A *tail visit* is a maximal run of consecutive tail-mode states of C. If the state
after a tail visit is not home, it is in a prefix mode (the run is maximal), so the last step was of
the third kind in (t), an empty step of Θ*: the last state of the visit has θ = ε, the leaf is the
one reached from ε, so the following prefix mode is always the same, a(Θ), and afterwards
θ = f_(Θ*)(ε) =: ε⁺.

Step 3 (chain). Call a prefix mode, or Θ, a unit; a prefix-mode state belongs to its mode, a
tail-mode state to Θ. Between a home state h and a later state s (no home state in between) the
states run through units μ_1, μ_2, ..., μ_n (consecutive states of one tail visit counted once), μ_1
the unit of j0, and μ_(l+1) = a(μ_l) by Step 2, every unit before the last being left by an overflow.
As in §2 Step 4: with u_1 = unit of j0, u_(j+1) = a(u_j) while this is defined, not home and new, the
units met on C are exactly u_1..u_p for some p, and in every state that belongs to u_j the coordinate
of each earlier unit u_t (t < j) has its post-overflow value: y = g(e) for a prefix unit, θ = ε⁺ for
Θ (its last visit before s was left by an overflow, and the coordinate has not moved since).

Step 4 (counting). Distinct states of C of the same kind (home; a given prefix mode; tail modes)
differ in (y_1..y_i, θ). Coordinates of units not met are constant on C. Home: y_u ∈ C_u for prefix
units, θ a Θ-home state of C_Θ: at most w_1···w_p. Prefix mode u_j: earlier units fixed (Step 3),
later prefix units in C_u, and if Θ is later, θ a Θ-home state of C_Θ: at most w_j···w_p. Tail
modes (u_τ = Θ): earlier units fixed, θ one of the c_Θ − H_Θ states of C_Θ that are not Θ-home, later
units free: at most (c_Θ − H_Θ)w_(τ+1)···w_p. ∎

**Lemma T.** Let r ≥ i ≥ 1, d_m = W_(r-m), ω = W_(r-i), and in Theorem T let c_u ≤ d_u for prefix
units and c_Θ ≤ ω. Then the sum X of the bounds of Theorem T is ≤ W_r.
Proof. Put P_n = d_1···d_n. Iterating W_ρ = W_(ρ-1) + W_(ρ-1)W_(ρ-1) i times: W_r = ωP_i + Σ_(n=1..i) P_n.
(*) A product of d_m over n distinct m ∈ {1..i} is ≤ P_n (Lemma S), and ω ≤ d_m for every m ≤ i.
Let q ≤ i be the number of prefix units. If Θ is not a unit, X ≤ P_q + Σ_(n≤q) P_n by Lemma S, and
P_q ≤ ωP_i. Let u_τ = Θ, A = product of the c of the a prefix units after Θ, B_j = product of the c of
the units u_j..u_(τ-1) (j < τ). Then
  X = H_Θ·B_1·A + Σ_(j<τ) H_Θ·B_j·A + (c_Θ − H_Θ)·A + Σ_(j>τ) c_(u_j)···c_(u_p).
The last sum has a terms, products over 1, 2, ..., a distinct modes: ≤ P_1 + ... + P_a.
If τ = 1: the rest is c_Θ·A ≤ ωP_a = ωP_q. Done.
If τ ≥ 2: the two terms with B_1 give 2H_Θ·B_1·A ≤ c_Θ·B_1·A ≤ ωP_q. The term (c_Θ − H_Θ)A is
≤ ω·A ≤ d_(u_1)·A, a product over a + 1 distinct modes, ≤ P_(a+1). For 2 ≤ j < τ, H_Θ·B_j·A ≤ ω·B_j·A
≤ d_(u_1)·B_j·A, a product over a + τ − j + 1 distinct modes (u_1 is not among u_j..u_(τ-1) or the
later ones), ≤ P_(a+τ-j+1); these indices are a+2, ..., a+τ−1 = q. Altogether
X ≤ ωP_q + Σ_(n≤q) P_n ≤ W_r. ∎   (Numerical check: `lemma_t.py`, `out_lemma_t.txt`.)

**Corollary T-r.** Let T have depth ≤ r and satisfy (MD_i) for some i ≥ 1, and suppose D(ρ) ≤ W_ρ
for all ρ < r. Then every cycle of T through home that satisfies (EC) has length ≤ W_r.
(depth(G_m) ≤ r − m, depth(Θ*) ≤ r − i.) Unconditionally for r = 5, by the repository's D(ρ) = W_ρ
for ρ ≤ 4. In particular, for i = 1 and r = 5: if the data of
S_1 are disjoint from idx(T0), a cycle longer than W_5 would have to enter T0's modes from outside at
two different modes (λ0 writing one tail index and S_1 another, or S_1 two).

**Corollary K\*\* (recursive class).** Add to the rules of K\* the rule (4): T satisfies (MD_i) for
some i ≥ 1, λ0 and the leaves of S_1..S_i write at most one index z_(j*) among z_(i+1)..z_k, every G_m
(m ≤ i) is in the class, and, if such an index z_(j*) is written at all, Θ* is in the class (if none
is written, no cycle through home ever reaches a tail mode and Θ plays no role). Then every tree of the class of depth ≤ r has all cycles ≤ W_r.
Proof. Cycles without a home state have length ≤ 2^k ≤ W_r (§2 (A) holds for all trees). For a
cycle through home, (EC) holds by the leaf condition; apply Theorem T, the induction hypothesis
(depths r − m and r − i are < r) and Lemma T. ∎
Rule (4) contains rule (2) (take i = k). It is strictly more than K\*: e.g. a tree with spine z_1, z_2, z_3, S_1 = (z_3 [z_3] []) (a relay
into tail mode 3), λ0 = [z_1], and S_2, S_3 of depths 3 and 2 sharing a data index is in K\*\* by
rule (4) with i = 1 (Θ* has depth 4), and satisfies neither (MD) nor root-disjointness.

Mechanical check of Theorem T: `tailcheck.py` (`out_tail.txt`; failing on purpose `out_tail_mut*.txt`).

## 6. Not proved, and what the computations say

- D(r) = W_r for all trees. Not covered by §2–§5: (i) a cycle on which a prefix mode and another mode
  exchange information through a data bit (one changes a bit the other reads); (ii) a tail entered
  from outside at two different modes. At depth 5 (given D(ρ) = W_ρ for ρ ≤ 4) a cycle longer than
  W_5 would need (i) or (ii) for every split i ≥ 1.
- Bounds per node, charged by depth, are false: "the number of states of a cycle with root bit 1 is
  ≤ W_(r-1)" fails at depth 4: the tree in `root1.txt` (chain 1 → 2, a 21-state counter in S_1 whose
  overflow path is short, a 6-state counter in S_2) has a cycle of length 258 with 126 > W_3 = 42
  states with root bit 1 (`out_root1.txt`). Only the total is bounded.
- A replacement for P1. Candidate, NOT proved:
  Lhat(leaf) = 1, Lhat(T) = μ_1···μ_k + Σ_n μ_1···μ_n where μ_1 ≥ ... ≥ μ_k are the values
  Lhat(S_i) sorted. Lhat ≥ L, and Lhat ≤ W_r for depth ≤ r (Lemmas S, V and induction).
  Evidence (weak: narrow generators, ≤ 12 indices, depth ≤ 7): no cycle longer than Lhat in 150,000
  random chain machines with identified indices (`lhat_rand.py`, `out_lhat_shared_Lhat.txt`) and
  up to 4,000,000 hill-climbing evaluations (`lhat_climb.py`, `out_climb_Lhat.txt`); the same runs with L as
  the measure find violations of P1 (63 and 53 trees; `out_lhat_shared_L.txt`, `out_climb_L.txt`).
  `lhat_search.py` is an earlier climber whose control found nothing; its output means nothing.

## 7. Mechanical checks (all watched failing on a wrong input)

- `sim.py` (from the definition): `out_cex.txt` (4 counterexamples to P1), `out_cex_mut.txt` (three
  near-misses that satisfy P1, one illegal tree rejected).
- `mdcheck.py`: Theorem MD, steps (1)–(7), 40,000 trees, 83,005 cycles through home: `out_md.txt`;
  failing: `out_md_mut1.txt` (overlapping data), `out_md_mut2.txt`, `out_md_mut3.txt` (wrong bounds).
- `nccheck.py`: Theorem NC on trees with heavy sharing, 10,000 trees, 20,592 cycles satisfying (NC),
  15,121 of them in trees violating (MD): `out_nc.txt`; failing: `out_nc_mut1.txt` (no (NC) filter),
  `out_nc_mut2.txt` (wrong products).
- `tailcheck.py`: Theorem T, 19,934 trees, 82,420 cycles satisfying (EC), 39,896 with the tail in the
  chain, 3,862 with a prefix mode jumping into the tail, 11,294 in trees whose tail subtrees share
  data: `out_tail.txt`; failing: `out_tail_mut1.txt` (no (EC) filter), `out_tail_mut2.txt` (tail data
  shared with a prefix mode), `out_tail_mut3.txt` (wrong bound).
- `lemma_t.py`: Lemma T, 35,387 exhaustive cases (r ≤ 5) and 165,091 random (r ≤ 8), max X/W_r = 1:
  `out_lemma_t.txt`; failing: `out_lemma_t_mut1.txt` (H_Θ up to c_Θ − 1), `out_lemma_t_mut2.txt`.
These checks test the structure and the counting on small trees (≤ 12 indices); they do not test the
W_r conclusion at depth ≥ 5, which rests on the proofs.
