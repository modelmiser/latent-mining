# Expiring-balls game: general bounds (experiment 21 follow-up)

Game, g(n), N(k) as in `chain/3-question.txt`. "Target k" means: Adversary wants a bin of load k.
A bin is **untouched** if no present ball was offered to it (it is then alone in its component).
Two facts used throughout:

- (W) The present balls are always exactly the balls offered since some moment; `Expire` m times
  removes the m oldest. So if p balls are present at some moment and further balls are added, then
  p expiries remove exactly those p balls and nothing else.
- (F) The offer graph is a forest, so the balls received by a bin have distinct other ends.

## Theorem U (upper bound). N(5) <= 21, and N(k) <= 5·2^(k-3) + 1 for every k >= 5.

Put b_1 = 2, b_2 = 4, b_3 = 7, b_4 = 12 and b_k = 5·2^(k-3) + 1 for k >= 5 (21, 41, 81, ...).
**P(k):** on b_k bins, from the empty position, Adversary can force a position in which some bin
has load k. (Stopping at the first such moment, that bin has load exactly k and all other loads
are at most k-1 before the last ball.) More bins do no harm: the extra bins stay untouched.

### Lemma T (tree). 
Let B be a set of 2^j untouched bins. Offering only inside B and never expiring, Adversary reaches
a position with a bin of B of load exactly j; the 2^j - 1 new balls all lie inside B.

Proof. j = 0: nothing to do. j >= 1: split B in halves, apply the lemma to each (the second half
is still untouched after the first run), get u, v of load exactly j-1 in different halves, hence
in different components; offer {u, v}. The receiver has load exactly j. ∎

### Lemma C (feeding).
Target k, 1 <= j <= k-1. Suppose the position has bins c_j, c_(j+1), ..., c_(k-1) and f, all in
pairwise different components, with load(c_i) = i. Then, with offers only and no
Expire, Adversary forces: either a bin of load k, or the position that differs from the present
one exactly by the k-j newest balls f<-c_j, f<-c_(j+1), ..., f<-c_(k-1) (f receives each).

Proof. For i = j, ..., k-1 in turn offer {f, c_i}. This is legal: so far f's component has
absorbed only c_j, ..., c_(i-1), and c_i is in a different one. If f receives, go on. If c_i
receives, call it g; it has load i+1. Now for m = i+1, ..., k-1 offer {g, c_m}: g's component
contains only f's and those of c_j..c_(m-1), so the offer is legal; g and c_m both have load m,
so the receiver has load m+1; rename the receiver g. After m = k-1 (or at once if i = k-1) a bin
has load k. ∎

### Proof of P(k) by induction.
P(1): one offer. P(2): Lemma T with j = 2 on 4 bins. Let k >= 3 and assume P(k-1). n = b_k bins.

**Round 0.** Play the strategy of P(k-1) inside a fixed set B0 of b_(k-1) bins until the first
moment a bin x has load k-1 (the Rule may answer anything; before that moment all loads are at
most k-2, which is all the strategy of P(k-1) assumes). Let p be the number of balls present.
All of them lie inside B0.

**Round 1.** Take a set B1 of 2^(k-2) untouched bins outside B0. By Lemma T get y in B1 of load
k-2. Offer {x, y} (different components). If x receives it has load k. Otherwise expire p times:
by (W) exactly the balls of Round 0 go. Position R1: the 2^(k-2) - 1 tree balls inside B1 and the
ball y<-x; load(y) = k-1; touched bins: B1 and x, that is s1 = 2^(k-2) + 1 bins.
Bins needed: b_(k-1) + 2^(k-2).
If k = 3, R1 consists of two balls, both received by y: a clean 2-star F = y on 3 bins; go to Final.

**Round 2 (k >= 4).** Write Y = y. Take disjoint sets B2, B3 of 2^(k-2) and 2^(k-3) untouched
bins. Lemma T in B2 gives a with load k-2. Let q be the number of balls now present. Lemma T in
B3 gives w with load k-3. a, Y, w are in three different components and load(a) = k-2,
load(Y) = k-1, so Lemma C applies with j = k-2, c_(k-2) = a, c_(k-1) = Y, f = w: either load k
appears or w receives the two newest balls w<-a, w<-Y. Expire q times: exactly the balls of R1
and of the tree in B2 go. Position R2: the 2^(k-3) - 1 tree balls inside B3, and w<-a, w<-Y;
load(w) = k-1; touched bins: B3, a, Y, that is s2 = 2^(k-3) + 2.
Bins needed: s1 + 2^(k-2) + 2^(k-3) = 5·2^(k-3) + 1.
If k = 4, B3 has two bins and one ball, received by w; so R2 is a clean 3-star F = w on 4 bins;
go to Final.

**Round 3 (k >= 5).** Write Y' = w. Take disjoint sets D_(k-2), ..., D_2, D_1 of untouched bins
with |D_i| = 2^i (in total 2^(k-1) - 2 bins). Lemma T in D_i gives c_i with load i, for
i = k-2, ..., 2. Let q' be the number of balls now present. Offer the two bins of D_1 and call the
receiver f. The bins c_2, ..., c_(k-2), Y', f lie in pairwise different components and
load(c_i) = i, load(Y') = k-1, so Lemma C applies with j = 2, c_(k-1) = Y': either load k appears
or f receives the k-2 newest balls. Expire q' times: exactly the balls of R2 and of the trees go.
What remains is the ball of D_1 and the k-2 fed balls, all received by f: a clean (k-1)-star
F = f on k bins.
Bins needed: s2 + 2^(k-1) - 2 = 5·2^(k-3).

**Final.** A clean (k-1)-star at F occupies k bins and everything else is untouched. Lemma T on
2^(k-1) untouched bins gives z of load k-1 in another component. Offer {z, F}: the receiver has
load k. Bins needed: k + 2^(k-1).

**Count.** n must be at least the maximum of the four "bins needed" that apply:
- k = 3: max(4+2, 3+4) = 7 = b_3.
- k = 4: max(7+4, 11, 4+8) = 12 = b_4.
- k = 5: max(12+8, 21, 20, 5+16) = 21 = b_5.
- k >= 6: b_(k-1) + 2^(k-2) = 9·2^(k-4) + 1 <= 5·2^(k-3) + 1; and k + 2^(k-1) <= 5·2^(k-3) + 1
  because k - 1 <= 2^(k-3) for k >= 5. So the maximum is 5·2^(k-3) + 1 = b_k. ∎

Remarks. (i) For k = 3, 4 this is the known 7- and 12-bin strategy (Round 2 at k = 4 is step I' of
`solver-a/proofs.txt` P6). (ii) b_5 = 21 = 5 + 2^4 equals the fit k + 2^(k-1); for k >= 6 the
theorem gives 41, 81, ... against the fit's 38, 71, ...; the binding step is Round 2 (a forcer on
2^(k-2)+1 bins, a tree of 2^(k-2) and a tree of 2^(k-3) present together). (iii) The previous
general bound N(k) <= N(k-1) + 2^(k-1) gives 28, 60, 124, ... (about 2^k); the theorem gives about
(5/8)·2^k. (iv) Mechanical confirmation for k <= 6: `gen.py` writes this strategy out as a table
of positions and moves, and `check_adv.py`, written from the game definition, accepts the tables
(see REPORT.md). The proof does not depend on these runs.

## Theorem L (lower bound, not specific to FIFO). N(k) >= floor((k+1)^2/4) + 1 for every k >= 1.

More precisely: if 1 <= r <= k and n <= r(k-r+1), the Rule below keeps every load at most k-1 on
n bins, **even if Adversary may remove any present ball it likes** (so the bound also holds for
the any-removal number A(k), which is at most 1 + k(k+1)/2). Taking r = ceil(k/2) gives the
statement. This is the "Robin Hood" argument of the literature on load balancing with departures
(Azar, Broder, Karlin and successors) adapted to forests; it is not new, and it does not use the
order of expiry. It beats the bound N(k) >= 2k from k = 6 on (13, 17, 21, 26, ... against 12, 14,
16, 18, ...).

Rule R_r. A bin is *rich* if its load is at least r, otherwise *poor*. When a ball raises a bin's
load from r-1 to r, the bin gets that ball's arrival index as its *stamp*; it keeps the stamp
while it stays rich and loses it when its load drops below r. Different rich bins have different
stamps (one ball raises one load). On an offer {u, v}: if one of them is poor, give the ball to
a poor one; if both are rich, give it to the one with the later stamp.

Proof. (a) At any moment let s be the number of rich bins. They hold at least s·r balls, and by
(F) the present balls are at most n-1. So s <= (n-1)/r.

(b) Consider a moment T and a bin i that is rich at T, with stamp t0. Let S be the set of bins
that were rich immediately after ball t0 arrived (i is in S). Bin i has been rich from t0 to T.
Its present balls are of two kinds. Those that arrived up to t0: at most r, because immediately
after ball t0 the load of i was exactly r and removals only delete balls. Those that arrived at
a time t > t0: such a ball was offered on {i, v} and given to i while i was rich, so by the rule v
was rich at time t with a stamp earlier than t0; v was rich from its stamp to t, in particular
immediately after t0; so v is in S, and v != i. By (F) these balls have distinct other ends, so
there are at most |S| - 1 of them. Hence load(i) <= r + |S| - 1 <= r + floor((n-1)/r) - 1 by (a)
applied at time t0.

(c) A poor bin has load at most r-1 <= k-1. If n <= r(k-r+1) then floor((n-1)/r) <= k-r and a
rich bin has load at most r + (k-r) - 1 = k-1. So load k needs n >= r(k-r+1) + 1 for every r,
and max over r of r(k-r+1) is floor((k+1)^2/4). ∎

`rh_test.py` plays this rule against a random any-removal adversary and asserts (a), the
containment used in (b), and the load bound after every move; with the tie-break reversed the
assertions fail (REPORT.md). That is a test of the argument's steps, not a proof.

## Not proved

- No lower bound that uses the FIFO order, and nothing superpolynomial from below. The picture
  the upper-bound proof suggests (SKETCH, not a theorem): under FIFO the only way found to delete
  clutter while keeping a loaded bin is to *feed* the bin its balls last, and each feeder
  component can feed a given bin once, and only while a chain of loaded bins of all higher loads
  stands in separate components (Lemma C). Whether the Rule can enforce that this is the only way
  is exactly what a lower bound would have to show.
- Whether N(5) = 21, and whether N(k) <= k + 2^(k-1) for k >= 6.
