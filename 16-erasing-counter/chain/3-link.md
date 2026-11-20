## KNOWN

The received question is well-posed. D(r,0) = 1 is right (weight never increases, so no 1 is ever read on a cycle). Two points the text leaves loose: unmentioned indices never change, so d = 2^r - 1 is enough even with short paths; and index reuse across different branches is allowed and may matter, so "one bit per tree node" is not without loss.

The unrestricted case D(r,r) is the classical bit-probe counter / quasi-Gray-code setting (Fredman's decision assignment trees; later work by Bose et al. and by Raskin, exact statements: unsure). I believe log2 D(r,r) = Theta(2^r) follows from that literature, which makes parts 1 and 3 of the received question mostly a matter of bookkeeping. The part with no visible answer is w = 1: the weight argument only says that, averaged over a cycle, at most one 1 is read per step. I fix w = 1 and ask for that alone.

## FINAL QUESTION

Fix an integer r >= 1 and put d = 2^r - 1. Memory is a string s in {0,1}^d with positions 1..d.

A destructive single-write incrementer of depth r is a finite rooted binary tree T such that:
- every internal node carries an index in {1..d} and has exactly two children, a 0-child and a 1-child;
- no index occurs twice on one root-to-leaf path (the same index may occur in different branches);
- every root-to-leaf path has at most r internal nodes (paths may be shorter, and of different lengths; T may be a single leaf);
- every leaf carries a set R that is either empty or one index occurring on that leaf's own root-to-leaf path.

T defines f: {0,1}^d -> {0,1}^d. To compute f(s): start at the root; at an internal node with index i move to the s_i-child; stop at a leaf. Let P be the set of indices on the path followed and R that leaf's set. Then f(s)_i = 1 if i is in R, f(s)_i = 0 if i is in P \ R, and f(s)_i = s_i if i is not in P. In words: every bit read is erased; the step may then set one of the bits it just read to 1, chosen as a function of everything read; unread bits keep their value. There is no state other than s. f need not be injective.

A cycle of length N >= 1 is a state s with f^N(s) = s and f^k(s) != s for 0 < k < N. Define

D(r) = the largest N such that some destructive single-write incrementer of depth r has a cycle of length N.

(Larger d gives nothing: T mentions at most 2^r - 1 indices and never changes the others.)

HEADLINE: find the exact function D(r) for all r >= 1. An answer must include the values D(2) and D(3), and must settle whether log2 D(r) is bounded above by a polynomial in r.

UNVERIFIED:
- D(1) = 2 (toggle the single bit).
- 6 <= D(2) <= 7. Lower bound: root a, 0-child b, 1-child c; on (a,b)=(0,0) write b; on (0,1) write a; on (a,c)=(1,0) write c; on (1,1) write nothing; this gives abc = 000, 010, 100, 001, 011, 101, back to 000. Upper bound: for r >= 2 the all-ones state has no preimage, so D(r) <= 2^d - 1.
- For r >= 2 a step whose path reads k ones lowers the weight by at least k - 1, and only the unique all-zero path can raise it, always by writing the same position.

GUESS: I would bet log2 D(r) is polynomial in r, because after each step the bits just read hold at most one 1, so only about log2(r+1) bits of control pass from one step to the next and the stock of unread 1s can be reached only through that channel.

## WHY IT IS PROBABLY UNCHARTED

Bit-probe counter work lets a step write arbitrary values to the bits it reads, and optimises reads, writes or space. Here reading erases and one write must both carry control and restore data; that combination I have not seen. Doubts: destructive readout is old in hardware (core memory, DRAM, FeRAM), and write-restricted models exist (write-once memory, cell-probe with bounded writes), so a paper on "counting with destructive reads" may exist. The answer may also turn out easy: a short argument could give log2 D(r) = Theta(r log r) or similar.

## HOW TO CHECK AN ANSWER

Fixed r: enumerate trees lazily. Start from a state s, simulate, and assign an index or leaf set to a tree node only when the run first reaches it; backtrack over all choices and all start states, recording the longest closed orbit. Quotient by permutations of {1..d}. r = 2 is immediate (d = 3); r = 3 has d = 7, 128 states. Confirm with an independent brute force for r = 2 over all trees and all 8 states. Include short paths and repeated indices across branches; omitting either is a different problem.

General claim: an explicit tree family with a proved cycle length (lower bound), and an upper-bound proof valid for every tree, including reused indices. Formula must reproduce computed D(1), D(2), D(3).
