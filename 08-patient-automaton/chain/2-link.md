# KNOWN

Adaptive synchronization games (players alternate letters, full information) have been studied; I believe the paper is Fominykh, Martyugin and Volkov, "P(l)aying for synchronization", and that deciding the winner is polynomial via a pair-merging argument. The exact reference details: unsure. Blind winning words are decidable by a search over families of subsets (the set of images the Spoiler can have produced), which gives only a doubly exponential length bound; I know no better bound, and no named theorem for B(n). The question is not ill-posed: singleton images stay singleton, so winning is monotone in length. For n = 2 part (3) is empty (an adaptively winnable two-state automaton has a constant letter, which is a blind word of length 1). Weakness: parts (1)-(3) are three loosely linked sequences, and blind versus adaptive are just the two ends of a scale the question never names.

# SUCCESSOR QUESTION

Let A be a complete deterministic finite automaton with state set Q = {1, ..., n} and input letters {a, b}; for a word v, Q·v is the image of Q under v. Fix an integer d >= 0 (the delay). Two players build an infinite word in rounds t = 1, 2, 3, ...: in round t the Synchronizer chooses a letter x_t, then the Spoiler chooses a letter y_t; the played word is x_1 y_1 x_2 y_2 ....

Information. Both players know A and d. The Synchronizer sees the Spoiler's letters d rounds late: x_t may depend only on x_1, ..., x_(t-1) and on y_1, ..., y_(t-1-d) (on no Spoiler letter at all while t <= d+1). The Synchronizer's strategy is deterministic. The Spoiler knows that strategy and sees every letter played so far.

Winning. A is d-winnable if there are a Synchronizer strategy and a number T such that |Q·(x_1 y_1 x_2 y_2 ... y_(T-1) x_T)| = 1 for every way the Spoiler can play. (The final state may depend on the Spoiler; the Synchronizer need not know when the image became a single state.) A is blind-winnable if there is one fixed word x_1 ... x_T, chosen before play, with the same property for all 2^(T-1) choices of y_1 ... y_(T-1); this is the case "d infinite".

Let W_d(n) be the set of d-winnable automata among the n^(2n) automata on Q (labelled: two automata are different if their transition tables differ), and W_inf(n) the set of blind-winnable ones. Clearly W_0(n) ⊇ W_1(n) ⊇ W_2(n) ⊇ ... ⊇ W_inf(n). Define s(n) as the least d with W_d(n) = W_inf(n), and s(n) = infinity if there is no such d.

Sanity check: s(2) = 0, since a two-state automaton is 0-winnable only if some letter is a constant map, and then that letter alone is a blind winning word.

(1) For n = 3 and n = 4 (and n = 5 if feasible) give the numbers |W_0(n)|, |W_1(n)|, |W_2(n)|, ..., up to the first d at which the number equals |W_inf(n)|, together with |W_inf(n)| and s(n).

(2) Is s(n) finite for every n? If yes, give s(n) as a formula in n (or matching upper and lower bounds) and describe a family of automata that are (s(n)-1)-winnable but not blind-winnable. If no, give the least n with s(n) = infinity and an explicit automaton that is d-winnable for every finite d but not blind-winnable.

(3) For each d >= 0 let N(d) be the least n with W_d(n) ≠ W_(d+1)(n) (infinity if none). Determine N(0), N(1), N(2), and say whether N is eventually given by a formula in d.

# WHY IT IS PROBABLY UNCHARTED

Games with delayed observation exist in the literature (delay games, delayed control), but I know of no work applying a delay to synchronization games, and none on where the delay hierarchy collapses to the blind case. Whether "winnable at every finite delay" implies "blind-winnable" is a compactness-style statement that does not follow from an obvious argument, because the Synchronizer's position space grows with d. Doubts: the hierarchy may collapse almost immediately (s(n) at most 1 or 2) for a simple reason, making the sequence dull; and if s(n) is infinite for some small n, part (1) there has no finite table.

# HOW TO CHECK AN ANSWER

For fixed d, d-winnability is a reachability game on positions (S, p): S is the image under the prefix the Synchronizer knows, p its last d own letters; the Synchronizer adds a letter, the Spoiler reveals the oldest hidden one; the Synchronizer wins iff it forces |S| = 1. Blind-winnability is a breadth-first search over families of subsets. Enumerate all n^(2n) tables (729, 65536, about 9.8 million), tabulate the counts, and compare. Check the claimed counts are non-increasing in d and invariant under renaming states and swapping letters. Claims about s(n) = infinity or formulas need proof plus agreement with the tables.
