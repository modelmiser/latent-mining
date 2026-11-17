## 1. KNOWN

A de Bruijn sequence of order n, unrolled to 2^n + n - 1 bits, is the shortest string containing every n-bit code; this is textbook. The seed is well-posed once "may" is read as "may also abstain". Easy facts: removing any bit of a run gives the same string, so the adversary really picks a run; unrolling a de Bruijn cycle twice gives an upper bound of 2^(n+1) + n - 1 for both versions, since the two occurrences of a code are 2^n apart and one fault reaches only one of them. "Every code must occur twice" is NOT a valid lower bound: a run 0^(n+1) keeps 0^n after a loss, and 0^n 1 keeps 0^(n-1) 1. So the true value lies strictly between the obvious bounds and depends on boundary effects. Single-deletion-correcting codes (Levenshtein, Varshamov-Tenengolts) concern decoding, not window coverage. Exact values: unsure.

## 2. SUCCESSOR QUESTION

The damaged tape loop.

Fix an integer n >= 1. A loop is a cyclic binary word w = w_0 w_1 ... w_{L-1} of length L >= 1 (indices mod L). A cyclic word v of length m covers a word c in {0,1}^n if there is an index i with v_{i+j mod m} = c_j for all j = 0..n-1 (windows may wrap around, more than once if m < n). Call v universal if it covers all 2^n words of length n.

A lock with a secret n-bit code reads the loop round and round forever and opens as soon as the last n bits read equal the code. Before reading starts, an adversary who knows w and the code may damage at most one cell k of the loop, permanently, in one of two ways:
- loss: cell k is skipped on every lap, so the lock reads the cyclic word of length L-1 obtained from w by removing w_k;
- doubling: cell k is read twice on every lap, so the lock reads the cyclic word of length L+1 obtained from w by inserting a second copy of w_k immediately after w_k.
The adversary may also do nothing. There are no further moves: the designer commits to w first, the adversary then picks (k, loss), (k, doubling), or nothing, with full knowledge of w and the code.

Call w robust if all 2L+1 resulting cyclic words (w itself, the L loss-words, the L doubling-words) are universal. Equivalently, the lock opens whatever the code and whatever the adversary does.

Let L(n) be the least length of a robust loop, and N(n) the number of robust loops of length L(n), counted up to rotation.

Question. Determine L(n) and N(n) for n = 1, 2, 3, 4, and determine L(n) for all n: give a closed formula, or failing that the exact value of lim L(n)/2^n (and prove the limit exists). In particular decide whether L(n) = 2^(n+1) for infinitely many n, and whether L(n) - 2^n is bounded by a polynomial in n.

Anchors to be re-derived, not trusted: a loss-word must itself be universal, so L(n) >= 2^n + 1; writing a de Bruijn cycle of order n twice around the loop appears to give L(n) <= 2^(n+1); by hand, L(1) = 4 (loop 0011) and L(2) = 6 (loop 000111, while no loop of length 5 is robust).

## 3. WHY IT IS PROBABLY UNCHARTED

Making the fault live in the loop removes the "+ n - 1" boundary and the "wait for the second copy" answer: the damaged loop must itself be a universal cycle, of a different length. That asks for a cyclic word all of whose one-step run-shortenings and run-lengthenings are universal, which I cannot match to any theorem. The gap between 2^n + 1 and 2^(n+1) is a factor of two, and n = 2 already sits strictly inside it. Doubts: robust positioning sequences and deletion-tolerant de Bruijn-like codes are studied; a paper there may contain this, and the limit may simply be 1 or 2.

## 4. HOW TO CHECK AN ANSWER

For n <= 3, enumerate every binary necklace of length up to 16, form its 2L+1 variants, and test universality directly: seconds. That fixes L(n) and N(n) and tests any formula. For n = 4 (lengths 17 to 32), use backtracking over necklaces with pruning, or a SAT encoding per length; unsatisfiability below the claimed L(4) plus one explicit robust loop confirms it. A general claim needs a construction verified by the same checker for n up to 5 or 6, plus a written lower-bound proof.
