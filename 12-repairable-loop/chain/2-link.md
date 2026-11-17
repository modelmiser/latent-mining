## KNOWN

The question is well-posed; I found no flaw. De Bruijn cycles of every order exist (standard). The lower bound L(n) >= 2^n + 1 is immediate. The upper bound 2^(n+1) from a doubled de Bruijn cycle checks out by thought: the undamaged copy plus the wrap supplies every window. L(1) = 4 I re-derived; L(2) = 6 I did not recheck. Useful reformulation: either damage changes the length of one maximal run by 1, and a loss in a run of length 1 merges its neighbours. A de Bruijn cycle is never robust (too short after a loss). I expect L(n) to sit far below 2^(n+1), so the "infinitely often 2^(n+1)" clause is probably false, but that is a guess. Related literature on deletion- or insertion-tolerant de Bruijn-type sequences exists in coding theory; exact statements: unsure.

## SUCCESSOR QUESTION

The tape loop with a mechanic.

Fix an integer n >= 1. A loop is a cyclic binary word v = v_0 ... v_{m-1} of length m >= 1 (indices mod m). It covers a word c in {0,1}^n if some index i has v_{(i+j) mod m} = c_j for j = 0..n-1 (windows may wrap, several times if m < n). A loop is universal if it covers all 2^n words of length n.

Two kinds of edit apply to a loop v of length m:
- loss at cell k (legal only if m >= 2): remove v_k, giving a loop of length m-1;
- doubling at cell k: insert a second copy of v_k immediately after v_k, giving a loop of length m+1.
Neither edit can create a symbol that is not a copy of an adjacent one. (Equivalently: an edit changes the length of one maximal run by 1; a loss in a run of length 1 deletes that run and merges its two neighbours, and no edit can ever split a run.)

The game has three moves, all with perfect information.
1. The designer commits to a loop w of length L.
2. The adversary, seeing w, performs one edit (loss or doubling at a cell of its choice) or passes. Call the result w'.
3. The mechanic, seeing w' (and w), performs one edit on w' (loss or doubling at a cell of its choice, of either kind, independent of what the adversary did) or passes. Call the result w''.
The designer wins if w'' is universal. There are no further moves.

Call w repairable if the designer wins against every adversary move, that is, if for each of the 2L+1 possible w' (2L when L = 1) there is at least one mechanic move with w'' universal. Let M(n) be the least length of a repairable loop, and R(n) the number of repairable loops of length M(n), counted up to rotation.

Question. Determine M(n) and R(n) for n = 1, 2, 3, 4, 5, and determine M(n) for all n by a closed formula or a theorem. In particular:
(a) for which n is M(n) = 2^n, i.e. for which n does there exist a de Bruijn cycle of order n that is repairable;
(b) is M(n) - 2^n bounded as n grows, and if so what is its eventual value;
(c) for n >= 2, is every repairable loop of length M(n) obtained from a de Bruijn cycle of order n by doublings?

Anchors to be re-derived, not trusted. Since w'' has length at least L - 1 + 1 after the worst loss and must have length at least 2^n, M(n) >= 2^n. By hand M(1) = 4 (loop 0011; the loop 01 fails because a loss leaves a one-symbol loop that doubling cannot diversify) and M(2) = 4 (loop 0011: after the loss of a 0 the mechanic doubles the remaining 0). For n = 3 both de Bruijn cycles appear to fail: the adversary deletes a run of length 1 (the 1 in 00010111 becomes 0000111), the mechanic must return to length 8 by one doubling, and neither 00000111 nor 00001111 is universal; so M(3) >= 9 appears to hold. A loop all of whose single-edit damages are already universal is repairable with the mechanic passing, and writing a de Bruijn cycle twice around the loop is such a loop, so M(n) <= 2^(n+1).

## WHY IT IS PROBABLY UNCHARTED

Error-tolerant de Bruijn sequences are studied, but there the decoder corrects; here the repairer is confined to the same crippled edits as the attacker and cannot recreate a destroyed run. That asymmetry (run count never increases) is what kills the de Bruijn cycle at n = 3 and makes singleton runs the battleground. I know no theorem covering it. Doubts: M(n) may simply equal the robust length minus a constant, making the game cosmetic; and sticky-insertion/deletion coding literature might contain an equivalent statement I do not recall.

## HOW TO CHECK AN ANSWER

For each L from 2^n upward, enumerate binary necklaces of length L, keep the universal ones, and for each of the 2L+1 adversary results test all mechanic moves (at most 2L+3) for universality; the first L with a survivor is M(n), the survivor count is R(n). Run-level edits cut the branching to the number of runs. Feasible for n <= 4 in seconds and n = 5 with pruning. A claimed formula must match these values; a general proof needs a construction plus a lower bound driven by singleton-run deletion.
