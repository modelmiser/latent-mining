# Prior-art search: glimpse cop/robber on complete binary trees

I found no publication of the glimpse-budget game, of d(h) = h-1, of e(h) = floor(h/2), or of h* = 3. Only the zero-glimpse facts look like corollaries of known work, and I could not confirm that at the primary source.

## CLOSEST PRIOR WORK

1. **The complexity of zero-visibility cops and robber** — Dereniowski, Dyer, Tifenbach, Yang, 2015 (Theor. Comput. Sci.). https://pub.pg.edu.pl/pl/download/135137-the-complexity-of-zero-visibility-cops-and-robber
   - Shows: characterises trees by zero-visibility copnumber, with a linear-time algorithm (from snippets and citing papers; the page itself would not load).
   - Differs: counts cops with no information at all; here there is one cop and the resource is a glimpse budget.
2. **Limited Visibility Cops and Robbers** — Clarke, Cox, Duffy, Dyer, Fitzpatrick, Messinger, 2017. https://arxiv.org/abs/1708.07179
   - Shows: the l-visibility copnumber of a tree is the largest k for which it contains a member of a recursive family (three smaller members joined by paths to a common vertex); the robber may pass.
   - Differs: visibility is by distance and free every round, not a budget of chosen queries; no direction-only information.
3. **Cop and robber games when the robber can hide and ride** — Chalopin, Chepoi, Nisse, Vaxès, 2011. https://pageperso.lis-lab.fr/jeremie.chalopin/publis/CCNV-SIAM-JDM11.pdf
   - Shows: studies Clarke's "witness version", where one cop is told the robber's position every k steps, and characterises graphs winnable for all k.
   - Differs: sightings are periodic and unlimited, not chosen and counted; the question is which graphs, not how many sightings.
4. **The Cat and the Noisy Mouse** — Rautenbach, Schneider, 2017. https://ar5iv.labs.arxiv.org/html/1707.02442
   - Shows: a cat receiving coarse distance information every round catches an invisible mouse exactly on forests.
   - Differs: the cat jumps to any vertex, the mouse must move, and information is free every round.
5. **An Efficient Noisy Binary Search in Graphs via Median Approximation** — Dereniowski, Łukasiewicz, Uznański, 2020. https://arxiv.org/abs/2005.00144
   - Shows (snippet only): search where a query at a vertex returns the neighbour towards the target, the same oracle as game D.
   - Differs: the target is static and the searcher does not walk.
6. **Approximating pathwidth for graphs of small treewidth** — authors not shown in what I read, 2020. https://arxiv.org/abs/2008.00779
   - Shows (snippet only): cites as known that the complete binary tree of height h has pathwidth ceil(h/2).
   - Differs: a width parameter, not this game; it resembles e(h) = floor(h/2), but I know of no derivation linking them.

## VERDICTS

- **d(h) = e(h) = 0 for h <= 2; a blind cop loses for h >= 3:** LIKELY EASY COROLLARY of the zero-visibility tree characterisation (items 1 and 2). It holds if the base forbidden tree is the three-legged spider with legs of length 2, which T_3 contains and T_2 does not. I could not confirm that leg length: item 1 was unreachable and the summary of item 2 reported inconsistent path lengths.
- **Upper bounds d(h) <= h-1, e(h) <= floor(h/2):** NOT FOUND, though they are elementary descent arguments.
- **d(h) = h-1 for h >= 3; e(h) = floor(h/2); h* = 3; d - e unbounded; projection and blind lemmas in their glimpse form:** NOT FOUND.
- **Sequences (OEIS searched directly):**
  - e = 0,0,0,1,2,2,3,3,4,4 returned six results. Only A278959 (length of the concatenation of primes below n) begins this way, which is a coincidence.
  - d = 0,0,0,2,3,4,5,6,7,8 returned two results (A108498, A331375), neither beginning with it and neither about graphs.

Queries run (24), by topic:
- zero-visibility copnumber of trees and complete binary trees; one-cop caterpillar characterisation; limited-visibility tree characterisation
- witness version (three phrasings); Scotland Yard with chosen reveals
- limited position queries for an invisible robber; glimpse/peek/ping budget; number of observations, sightings or probes; exact-location queries with floor(h/2)
- direction oracle with hints; direction-only cop (bearing, compass); binary search in graphs with a moving target; mobile hidden target with edge or vertex queries
- hunter and rabbit on trees; cost of visibility
- advice complexity of pursuit; observation budgets in partially observable reachability games
- pathwidth of complete binary trees
- the two OEIS sequences

## WHO TO ASK

A graph-searching / pursuit-evasion specialist who works on limited-visibility Cops and Robbers; the authors of items 1 and 2 are that community.

## NOT CHECKED

- The primary zero-visibility tree paper and Clarke's original witness paper: not read.
- Paywalled journals, theses and non-English literature: not searched.
- Whether floor(h/2) follows from pathwidth: not established.
- Sources read in full: one, a two-page course homework sheet (https://perso.ens-lyon.fr/edouard.bonnet/graphDecompositions2/dm1.pdf) that sets, without answer, the invisible-robber cop number of the complete binary tree in the helicopter model. I read three pages of item 3 directly. Items 2 and 4, one abstract, a 2025 paper (https://arxiv.org/abs/2509.05024) and the OEIS pages came through a summarising fetch. Items 1, 5 and 6 were judged from search snippets.
