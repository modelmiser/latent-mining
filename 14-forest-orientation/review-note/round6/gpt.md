<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — The invariant, both rounds, recurrence, vertex count, and \(H=j\) follow from `14-forest-orientation/NOTE.md:467-492`; replay gives \(c_2=2,c_3=10\) — none

Q2: REFUTED — the bounds follow from `NOTE.md:496-501`, but “uses one tie” is false recursively (`NOTE.md:468-470,502-503`), \(c_0\) is undefined, and no output preserves the numerical claim at `NOTE.md:505-506` — “Define \(c_0=0\); each base copy and recursive assembly uses one consequential adversarial tie; the first merge and shortest-path choices are immaterial. The source implements the reported exhaustive search, but its output is not archived.”

Q3: CONFIRMED — every Lemma 4 case, including \(k=0\), is valid (`NOTE.md:357-416`); Lemma 5’s induction and \(E\)-cases are valid (`NOTE.md:419-441`); the smaller-tree example has old \(\Phi=7/2\), new \(\Phi=5\), cost 2 (`NOTE.md:340-344`) — none

Q4: REFUTED — adversarial tie-breaking does not exclude a better fixed tie rule (`NOTE.md:32-35,503-504`); `compare_bounds.py:10-14` checks only through \(N\), not “every \(n\ge94\)” as `NOTE.md:461-464` states. No stale smaller-tree attribution or current \(1/3\le L\le9/16\) claim remains (`14-forest-orientation/README.md:56-58`, `README.md:45`) — “Theorem 4 gives \(n/2-O(\log^2 n)\) under adversarial tie-breaking; beating \(1/2\) needs another algorithm or an unanalysed fixed tie rule. The comparison was checked through 4000.”

Q5: REFUTED — the main enumerators and pool rounds are genuine (`lemma4_check.py:46-67`, `pool_search.cpp:52-66`), but `ternary_family.py:31` fabricates \(A_1\) instead of replaying its rounds, `lemma5_check.py:20-34` omits \(n=1\), and several quoted run limits have no saved output — “The programs implement the intended checks for documented positive parameters, except that the ternary base and Lemma 5 singleton are not replayed; archive the quoted runs.”

Q6: CONFIRMED — \(j=2\) performs two cost-1 merges and \(j=3\) adds \(3\cdot2+2\cdot2=10\) (`NOTE.md:477-489`); exact \(T\)-values concern an optimal algorithm, whereas \(G\) concerns rule R (`NOTE.md:496-505,517-522`), so there is no contradiction — none

F: CRITICAL — `upper/compare_bounds.py:2,10-14` — the documented optional \(N<130\) causes an out-of-range traceback — confidence: high — validate \(N\ge130\) or bound diagnostics by \(N\)

F: CRITICAL — `upper/pool_search.cpp:40-46,69` — \(K=0\) dereferences an empty vector, causing undefined behavior — confidence: high — reject nonpositive \(K\)

F: MODERATE — `NOTE.md:32-35`; `14-forest-orientation/README.md:17,237-241` — adversarial ties do not prove that every fixed tie rule has rate \(1/2\) — confidence: high — add the fixed-tie alternative

F: MODERATE — `NOTE.md:461-464`; `upper/compare_bounds.py:10-14` — finite computation is presented as an unbounded comparison — confidence: high — restrict to 4000 or add the 16-step induction

F: MODERATE — `upper/greedy_exact.cpp:3-5,321-337` — \(S<HI\) silently activates abstraction while still printing ostensibly exact values — confidence: high — enforce \(S\ge HI\)

F: LOW — `NOTE.md:500-503`; `14-forest-orientation/README.md:57` — tie count and \(c_0\) are misstated/undefined — confidence: high — define \(c_0\) and describe recursive ties

F: LOW — `upper/ternary_family.py:22-40` — base rounds are skipped and all verification disappears under Python `-O` — confidence: high — replay \(A_1\) and use explicit failures

F: LOW — `upper/lemma5_check.py:20-34` — “all trees up to N” omits the singleton — confidence: high — check \(n=1\)

Unasked: no additional issues.
