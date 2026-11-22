# R(4q+1) >= 3q - 1: the two-level ("quad") scheme

Definitions are those of `chain/3-question.txt`. The scheme below is
implemented in `quad.py`; `checker.py` tests it from the definition.

## Theorem

For every q >= 1 there is a top-scheme on n = 4q + 1 cells that survives
t = 3q - 1 operations (with Q(0^n) = EMPTY, so under both readings).
Hence, ignoring up to three cells, **R(n) >= 3*floor((n-1)/4) - 1 for all
n >= 5**, which is at least (3n - 16)/4.

## Layout

Cells 1..4q are cut into 2q *pairs* (cells 2i-1, 2i); pairs 2j-1, 2j form
*quad* j (j = 1..q). The last cell is the *flag*. A pair is

- U (untouched) if it reads 00,
- L (live) with *static value* 0 if it reads 10, static value 1 if it reads 01,
- D (dead) if it reads 11.

"Killing" a live pair means setting its remaining cell (L -> D). A quad
whose pairs are both L is called LL; a quad with exactly one L and one D is
*mixed*, with *position value* 0 if the first pair is the live one (LD) and
1 if the second is (DL); DD is a quad with both pairs dead.

## Mode A (flag = 0): the pair scheme

- Q: static value of the rightmost live pair; EMPTY if there is none.
- PUSH b, some pair is U: write b's pattern into the leftmost U pair.
- POP: kill the rightmost live pair.
- PUSH b, no pair is U ("freeze"): set the flag; kill every live pair in the
  quads to the left of the first LL quad; then do the mode-B PUSH b below.

## Mode B (flag = 1; every pair is L or D)

Reading of a configuration. Let j* be the index of the first LL quad
(j* = q + 1 if there is none). The *left zone* is quads 1..j*-1, the *right
zone* quads j*..q.

- N = the position values of the mixed quads of the left zone, left to right.
- O = the static values of the live pairs of the right zone, left to right.
- R = O followed by N (N on top).
- Q: last entry of R (EMPTY if R is empty).

Operations.

- PUSH b (j* <= q): let j** be the first LL quad after j* (q + 1 if none).
  Kill every live pair in quads j*+1..j**-1 ("waste kills"); in quad j* kill
  the second pair if b = 0, the first if b = 1 (quad j* becomes mixed with
  position value b).
- POP, N non-empty: kill the live pair of the last mixed quad of the left zone.
- POP, N empty: kill the rightmost live pair of the right zone.
- Anything else (PUSH with j* = q + 1, POP with R empty): stay.

Q and E are functions of the configuration alone and E only sets cells.

## Proof

Let S be the true stack, s the number of operations done, r = t - s.

**Mode A.** Invariant: the touched pairs form a prefix and the static values
of the live pairs, left to right, spell S bottom to top. PUSH writes to the
right of all touched pairs; POP removes the rightmost live pair, which is
the top. So Q is right as long as the flag is 0. (This is solver B's P3.)

**Charging.** Give each operation to one quad: a mode-A PUSH to the quad of
the pair it writes; a POP (either mode) to the quad of the pair it kills; a
mode-B PUSH (the freeze PUSH included) to quad j*, the quad it converts.
Waste kills are not operations. So s = sum over quads of charge(quad).

In mode B every pair has been touched, so every quad carries 2 charges from
its two mode-A pushes, plus one charge for each of its pairs killed by a POP
or by a conversion, and none for a pair killed by a waste kill. A waste kill
always kills every live pair of its quad, and never hits an LL quad (the
quads strictly between j* and j**, and the quads left of the first LL quad
at the freeze, are not LL). Call a quad *wasted* if it received a waste
kill; a wasted quad is DD and was mixed when it was wasted. By final state:

| quad | charge | entries of R it holds |
|---|---|---|
| LL (necessarily right zone) | 2 | 2 |
| mixed (either zone), never wasted | 3 | 1 |
| DD, not wasted | 4 | 0 |
| DD, wasted | 3 | 0 |

Hence, with w the number of wasted quads,

  (1)  s >= 4q - |R| - w   (in fact equality).

The quad converted by the freeze PUSH is never wasted (later waste kills lie
to the right of the then-current j*, which never moves left because left-zone
quads never become LL), so w <= q - 1 once mode B has begun, *provided an LL
quad existed at the freeze*.

**An LL quad exists at the freeze.** Before the freeze PUSH, s = 2q + d
where d is the number of dead pairs (mode A: 2q pushes touched all pairs,
each dead pair is one POP). Without an LL quad every quad has a dead pair,
d >= q, s >= 3q > t - 1: the freeze PUSH would be operation number
s + 1 > t. So within the budget there is one.

**Invariants in mode B**, after every operation:

- (I1) R is a suffix of S (same order, same values);
- (I2) |R| >= r + 2.

(I2) follows from (1): |R| >= 4q - w - s >= 4q - (q - 1) - s = 3q + 1 - s
= r + 2, using t = 3q - 1. It is a statement about the configuration reached,
so it holds after every mode-B operation, the freeze included.

(I1) at the freeze: before it, S is the list of live pairs. The freeze kills
the live pairs left of j* and those between j* and j** (the bottom-most
elements of S), converts quad j* (its two elements are the next
bottom-most), so afterwards the left zone is quads 1..j**-1, containing the
one mixed quad j* and otherwise DD quads: N = (b), O = live pairs from j**
on, R = (a suffix of S) + b. Good.

(I1) is preserved by a mode-B PUSH b: by (1) and the argument above a PUSH
with j* = q + 1 cannot occur within the budget (then R = N, w <= number of
DD quads, s >= 4q - (number of mixed) - (number of DD) = 3q > t - 1). With
j* <= q: the quads j*..j**-1 hold the bottom-most entries of O; they become
one mixed quad of position value b followed by DD quads, and the left zone
becomes 1..j**-1. New N = N + (b), new O = O minus bottom entries. Good.

(I1) is preserved by a POP. Before the POP r >= 1, so |R| >= 3 by (I2).
If N is non-empty its last quad becomes DD: N loses its top, j* unchanged.
If N is empty the rightmost live pair of the right zone is killed. Since
|O| = |R| >= 3 that pair lies in a quad j' > j* (quad j* holds only two
entries and everything right of the rightmost live pair is dead). Quad j*
stays LL, so the zones do not move; O loses its top. Good. (The excluded
case, the POP hitting quad j* itself, is the one place where the reading
would break: the quad would turn mixed and be re-read by position.)

**Conclusion.** By (I1) and (I2) (|R| >= 2 > 0) the last entry of R is the
top of S and S is non-empty; in mode B Q returns it. In mode A Q is right by
the pair invariant, and Q(0^n) = EMPTY. So the scheme survives every legal
sequence of at most t = 3q - 1 operations.  QED

## What the checker says

`quad.py` with `checker.py` (every legal sequence, see `run_quad.log`):
survives t = 3q - 1 **and** t = 3q for q = 1..6 (n = 5..25; at q = 6 that
is all 296,855,529 legal sequences of length <= 18), fails at
t = 3q + 1 for q = 1..6 (by q more pushes after the 2q pair pushes, then a
push that finds no LL quad). The proof gives 3q - 1; that the same scheme
survives 3q is COMPUTED for those q only. q = 7 ran out of the 6 GB memory
cap and is not checked.

Two mutants are rejected: without the waste kills (`nowaste`), and without
the cleaning at the freeze (`nofreezeclean`, rejected at q = 4, not at
q = 3).
