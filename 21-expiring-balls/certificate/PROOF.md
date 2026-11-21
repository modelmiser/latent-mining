# Why "check accepts" implies the Rule keeps every load below L on n bins

This is the soundness argument for `check.cpp` (modes `min` and `full`). It uses only the game definition
in `chain/3-question.txt`. Nothing about how the certificate was produced is used.

## 1. Objects

A **state** is a sequence of balls b_1, ..., b_k, newest first, each ball a pair (receiver, other bin), taken
up to renaming of bins, such that the offer graph is a forest on at most n bins. It is **non-final** if every
load is at most L-1. The **prefix of length j** of a state is the state made of its j newest balls. Two facts
from the rules:

- Expire turns a state into its prefix of length k-1.
- Answering an offer puts a new ball in front. So for a state q and a legal offer {u, v}, writing q+u for
  "u receives": the prefix of length j of q+u is (the same new ball) in front of the prefix of length j-1 of q.

Every prefix of a non-final state is a non-final state (deleting balls keeps a forest and lowers loads).

The certificate is a set **C** of non-empty non-final states (the checker refuses a file containing the empty
state, an illegal state, a state with a load of L or more, or a line not written in canonical labelling). Define

- **A** = the non-final states that have at least one prefix in C;
- **S** = the non-final states that are not in A.

*Extension fact.* If p is in A, p is a prefix of r, and r is non-final, then r is in A. (A prefix of p is a
prefix of r.)

Claim: if the checker accepts, S has the four properties

1. the empty state is in S;
2. every state in S has all loads at most L-1;
3. S is closed under Expire;
4. for every q in S and every legal offer {u, v} at q (u, v in different components of the offer graph, among
   all n bins), at least one of q+u, q+v is in S.

These give a strategy for the Rule (always answer so as to stay in S), under which the play stays in S for
ever by induction on the number of moves, so no load ever reaches L.

## 2. Properties 1, 2, 3

1. The only prefix of the empty state is itself, and it is not in C. So it is not in A; it is non-final.
2. By definition of S.
3. Let q be in S and q' its prefix of length k-1. q' is non-final. If q' were in A, then q would be in A by
   the extension fact. So q' is in S.

## 3. Property 4: reduction to three finite families

Call an answer x in {u, v} **bad** at q if it does not lead into S, that is, if load_q(x) = L-1 (the new ball
makes a load L) or q+x is in A. Suppose property 4 fails: q is in S, the offer {u, v} is legal at q, and both
answers are bad. One of three cases holds. In each, a test made by the checker shows that q is in A, a
contradiction.

In canonical labelling the newest ball of q+u reads "0 receives, 1 is the other bin": bin 0 is u and bin 1
is v. So load of bin 1 in q+u equals load_q(v), and exchanging receiver and other bin in the newest ball of
q+u gives q+v. The checker's test T(e), for a non-final state e:

> T(e): if bin 1 of e has load L-1, or (variant T+ only) e with its newest ball flipped is in A, then e
> without its newest ball must be in A; otherwise reject.

**Case F3: both answers make a load L.** Then u and v have load L-1 and are in different components: q is a
*seed*. Let q0 be the shortest prefix of q that is a seed. `walkSeeds` starts at the empty state and steps
from a state to each state with one more older ball, stopping at seeds; so it reaches every non-final state
none of whose proper prefixes is a seed, in particular q0 (pruning: see P1). It requires q0 in A. By the
extension fact q is in A.

**Case F1: both q+u and q+v are in A.** Let c1 be a prefix of q+u in C and c2 a prefix of q+v in C, and
suppose length(c1) = j is at least length(c2) (otherwise exchange u and v). c1 is a line of the certificate,
and the checker applies T+ to every line. c1 with its newest ball flipped is the prefix of length j of q+v;
it is non-final and has c2 as a prefix, so it is in A. T+ therefore requires c1 without its newest ball,
which is the prefix of length j-1 of q, to be in A. By the extension fact q is in A.

**Case F2: q+u is in A and v has load L-1 in q** (or the same with u and v exchanged). Put e = q+u; bin 1
of e has load L-1. Let f be the shortest prefix of e in which bin 1 has load L-1 (every prefix of
length at least 1 contains the newest ball and so names bins 0 and 1 the same way).

- If some prefix c of e that is in C has length(c) >= length(f): bin 1 has load L-1 in c. T+ at the line c
  requires c without its newest ball, a prefix of q, to be in A. So q is in A.
- Otherwise every prefix of e that is in C is a proper prefix of f, and there is one because e is in A. Let
  c be the longest. `walkMin` starts at the line c and steps to each state with one more older ball, skipping
  a child only if that child is itself in C, and stopping at a state where bin 1 has load L-1. Along the
  chain of prefixes from c to f no state after c is in C (c is the longest) and in none before f does bin 1
  have load L-1 (f is the shortest). So the walk reaches f and applies T there, which requires f without
  its newest ball, a prefix of q, to be in A. So q is in A.

That finishes property 4. The families examined are: F1/F2-roots, the lines of C; F2, the extensions of
each line up to the first moment bin 1 reaches load L-1; F3, the minimal seeds.

## 4. The walks reach what the proof says

*Children.* `older(e)` must produce every non-final state whose prefix of length k is e (k = length of e).
Such a state is e plus one older ball. Each end of that ball is either one of the m bins e touches or a bin
e does not touch; untouched bins are interchangeable, and canonical labelling names them m (and m+1 if
both ends are untouched, receiver first). The function runs over all (m+1)^2 choices "touched bin a or
untouched" for the two ends, and keeps those that are legal: at most n bins, the two ends distinct and in
different components (an untouched bin is a component by itself), receiver's load at most L-1 afterwards.
Different choices give different canonical states. This is tested independently in section 6.

*Pruning P1 (seeds).* Let e have k balls and let d be a seed with e as a prefix, with witnesses x, y. x and
y are in different components of d, hence of e (or untouched by e). d is a forest with at least two
components, so it has at most n-2 balls. Each ball of d not in e raises one load by one, so d has at least
k + (L-1-load_e(x)) + (L-1-load_e(y)) balls. If that exceeds n-2 for every pair x, y in different
components of e (untouched bins counting as load 0), no extension of e is a seed, and the walk may stop.

*Pruning P2 (F2).* A state reached below e in which bin 1 has load L-1 has at least k + (L-1-load_e(1))
balls, and a forest on n bins has at most n-1.

Both prunings can be switched off (`NOPRUNE=1`); the verdicts and the seed counts were compared with and
without on small cases (REPORT.md).

## 5. Mode `full`

`walkFull` starts at every line of C, steps to every child that is not itself a line of C, and applies T+ at
every state it meets. Every state e of A is met (start from its longest prefix in C). Soundness without the
reduction: if q in S has an offer with both answers bad and it is not case F3, then some answer leads to a
state e of A; e is met; the other answer is bad, so T+ fires and requires q in A. The number of states met
is exactly the number of states of A, which can be compared with the two search programs' counts.

## 6. What the argument does not cover

- That `check.cpp` implements the definitions above: canonical relabelling, loads, components, the forest
  test, the 128-bit key (injective: at most 11 balls, bin names below 16, a ball byte is never zero), the
  hash set. Tests of this, none of them a proof: (a) `count` mode enumerates all states with `older` and
  compares, for each number of balls k, the sum over states of n!/(n-m)! with the number of labelled
  forests with k edges times k! 2^k, computed by an independent formula (`forestcount.py`); (b) `brute`
  mode and `explicit.py` check properties 1-4 literally, without sections 3-5, on small n, and agree with
  `min` and `full` on several hundred damaged certificates (`difftest.py`).
- The compiler and the machine.
