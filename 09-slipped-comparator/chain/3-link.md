## KNOWN

Words over adjacent comparators ("primitive" networks, Knuth TAOCP vol. 3, section 5.3.4) sort all inputs iff they sort the reversed input; the minimum fault-free size is n(n-1)/2. Fault-tolerant sorting networks are studied mainly for comparators that fail to act (Yao and Yao; Rudolph's robust network; Schimmler and Starke's correction networks) — exact citations for adjacent-only minimum sizes: unsure. The received question is well posed but has three headlines. Its phrase "adversary knows the input" adds nothing (the condition is universal over inputs). Its question 3 is half easy: I believe inserting an adjacent comparator into a sorting word never breaks it, so a slip is a deletion plus a harmless insertion, giving S(n) <= A(n) at once; only strictness is open. The claimed S(3) = 5 looks right (every slip of c_1c_2c_1c_2c_1 contains c_1c_2c_1 or c_2c_1c_2 in effect). I kept one headline: the exact excess of S(n) over n(n-1)/2.

## FINAL QUESTION

Fix an integer n >= 3 and wires numbered 1..n from top to bottom. For 1 <= i <= n-1 the adjacent comparator c_i acts on a vector of n distinct integers by putting the smaller of the values on wires i and i+1 on wire i and the larger on wire i+1; all other wires are unchanged. A word w = c_{i_1} c_{i_2} ... c_{i_m} over the alphabet {c_1, ..., c_{n-1}} is applied left to right; its size is m. No other comparators exist.

A slip of w is a word obtained from w by choosing exactly one position p and replacing the letter c_i at p by c_{i-1} or by c_{i+1}, where the replacement must be in the alphabet (so c_1 can only become c_2, and c_{n-1} can only become c_{n-2}). A slip has the same size as w. A word sorts if it maps every arrangement of n distinct values to increasing order on wires 1..n.

Call w slip-proof if w sorts and every slip of w sorts. (Equivalently: an adversary who sees w and the input, and may make at most one slip, cannot prevent sorting.) Let S(n) be the least size of a slip-proof word on n wires, and put D(n) = S(n) - n(n-1)/2.

HEADLINE. Find the exact function D(n) for all n >= 3. Precisely: is D(n) = n - 1 for every n >= 3, that is, S(n) = (n-1)(n+2)/2? Answer yes with proof, or no with the least n at which it fails and the true value of S(n) there. Small instances to settle by computation: S(4), S(5), S(6).

UNVERIFIED beliefs (do not rely on them):
(a) A word over adjacent comparators sorts every input if and only if it sorts the single input n, n-1, ..., 1; so each slip needs one test.
(b) S(3) = 5, attained by c_1 c_2 c_1 c_2 c_1.
(c) S(n) >= max(n(n-1)/2, 2(n-1)): the unslipped word must undo n(n-1)/2 inversions, and on the input that is sorted except for wires j, j+1 exchanged, slipping the first c_j acts as a deletion, so c_j must occur at least twice.
(d) Inserting an adjacent comparator anywhere in a sorting word leaves it sorting (monotonicity of adjacent comparators on 0-1 inputs under dominance). Hence a slip is never worse than deleting the letter, and S(n) <= A(n), where A(n) is the least size of a word that still sorts after deletion of any one letter.
(e) Odd-even transposition sort run for n+2 rounds has size (n-1)(n+2)/2 and may be deletion-proof, which would give D(n) <= n - 1.

GUESS (a bet, not a claim): yes, D(n) = n - 1, because on nearly sorted inputs a slipped comparator does nothing, so slips behave like deletions and each of the n-1 letters seems to need one spare occurrence; the real doubt is whether the misplaced comparator can do useful work on the reversed input and save letters for larger n.

## WHY IT IS PROBABLY UNCHARTED

Fault models in the literature are comparators that do nothing, are stuck, or reverse; a comparator that works but on a neighbouring pair of wires is, as far as I know, not studied, and adjacent-only networks get little exact-size attention. Doubts: if belief (d) holds and the slipped comparator never helps, the problem collapses to one-deletion tolerance for bubble-type networks, which may be solved in the 1980s robust-network literature or be a routine exercise. The formula guess rests on n = 3 alone.

## HOW TO CHECK AN ANSWER

For fixed n, test a word by applying it and each slip (at most 2m of them) to all n! inputs; do not assume belief (a) unless you also verify it exhaustively for that n. To find S(n), enumerate words by increasing size m, or run breadth-first search over states (image of every input under the unslipped prefix, set of images under once-slipped prefixes); the first m with a slip-proof word is S(n), and exhaustion below m is the lower-bound certificate. n = 4 is immediate (3^m words, m <= 9); n = 5, 6 need the state search. A general claim needs an explicit slip-proof family of the claimed size for every n and a matching lower-bound proof valid for all n.
