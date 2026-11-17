## KNOWN

Fault-tolerant sorting networks with passive ("stuck-pass") faults go back to Yao and Yao (1985) and were continued by Assaf–Upfal, Leighton–Ma, Schimmler–Starke and Piotrów (correction networks); I am confident of the names, unsure of exact bounds. The 0-1 principle survives passive faults, because monotone maps commute with both working and dead comparators. An easy lower bound follows: on a 0-1 input with a single adjacent inversion at wires i, i+1, only a comparator joining exactly those two wires changes anything, so every pair (i, i+1) must be joined at least f+1 times; hence S(n, f) >= (f+1)(n-1), and the last layers must be dense in adjacent comparators. Exact values of S(n, f) for small n: unsure whether tabulated. The seed is well posed but has four moving parts (n, f, size, depth) and unknown fault-free values S(n) beyond small n, so exact formulas are hopeless as stated.

## SUCCESSOR QUESTION

Fix n >= 2 wires, numbered 1..n from top to bottom. An adjacent comparator c_i (1 <= i <= n-1) acts on wires i and i+1: it puts the smaller of the two values on wire i and the larger on wire i+1. A primitive network is a finite word w = c_{i_1} c_{i_2} ... c_{i_m} over the alphabet {c_1, ..., c_{n-1}}, applied left to right; its size is m. No comparator between non-adjacent wires is allowed. A fault deletes one letter of the word (that comparator passes both values through unchanged). Call w f-robust if, for every input of n distinct values and every way of deleting at most f letters, the remaining word leaves the values in increasing order from wire 1 to wire n. The adversary sees the input before choosing which letters to delete.

Define P(n, f) as the least size of an f-robust primitive network on n wires. (P(n, 0) = n(n-1)/2 is classical. By hand, P(2, f) = f+1 and P(3, 1) = 5, attained by c_1 c_2 c_1 c_2 c_1.)

Questions.
1. Determine the sequence A(n) = P(n, 1) for n = 2, 3, 4, 5, 6, 7, and give a closed formula for A(n) with proof. In particular: is A(n) = n(n-1)/2 + (n-1) for all n, or is the price of the single fault eventually smaller or larger than n-1 comparators?
2. Determine P(n, f) for all n and f. Is the marginal price P(n, f+1) - P(n, f) independent of f? Is it independent of n beyond some point?
3. For each n <= 6, count the 1-robust words of size exactly A(n), and say whether every one of them is obtained from some fixed family (for example, a truncation of the alternating odd-even transposition pattern, or a bubble-sort triangle with one extra diagonal) by commuting letters c_i c_j with |i - j| >= 2.

## WHY IT IS PROBABLY UNCHARTED

Restricting to adjacent comparators removes the unknown S(n) and makes the fault-free answer exact, so the fault price becomes an exact quantity rather than an asymptotic one. The problem is equivalent to: shortest words in the 0-Hecke monoid whose Demazure product stays the longest element after any f deletions. I know of no such "deletion-robust Hecke word" result. Doubts: robustness of odd-even transposition sort has been studied, and the answer may be a quick corollary (n(n-1)/2 + f(n-1) is the obvious guess); question 3 is likelier to be new than question 1.

## HOW TO CHECK AN ANSWER

A primitive network sorts every input iff it sorts the reversed input n, n-1, ..., 1 (de Bruijn); this applies to each faulted word separately, so f-robustness needs one input per deletion set. For n <= 5 enumerate all words of length up to the claimed A(n) (at most 4^14 words) and test all single deletions; for n = 6, 7 use a breadth-first search over states (set of reachable permutations under one pending deletion). A claimed formula needs a matching construction plus a lower-bound proof; any smaller robust word found by search refutes it.
