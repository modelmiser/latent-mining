<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `14-forest-orientation/NOTE.md:467-493` and `14-forest-orientation/upper/ternary_family.py:21-41` — none
Q2: CONFIRMED — `14-forest-orientation/NOTE.md:495-506` and `14-forest-orientation/upper/greedy_exact.cpp:165-221,300-318` — none
Q3: CONFIRMED — `14-forest-orientation/NOTE.md:341-344,356-416,418-441` — none
Q4: CONFIRMED — `14-forest-orientation/NOTE.md:20-36,461-466`, `14-forest-orientation/README.md:15-18,55-58`, `README.md:45`, and `14-forest-orientation/upper/compare_bounds.py:1-14` — "so an upper bound on L below 1/2 needs a different algorithm (or an algorithmic tie-breaking rule for rule R)"
Q5: CONFIRMED — `14-forest-orientation/upper/ternary_family.py:21-41`, `14-forest-orientation/upper/pool_search.cpp:52-66,84`, `14-forest-orientation/upper/lemma4_check.py:46-66`, `14-forest-orientation/upper/lemma5_check.py:28-33`, and `14-forest-orientation/upper/compare_bounds.py:10-14` — none
Q6: CONFIRMED — `14-forest-orientation/NOTE.md:16-24,467-493,505-506`, `14-forest-orientation/README.md:10-18,44-58`, and `14-forest-orientation/upper/ternary_family.py:30-41` — none

F: LOW — `14-forest-orientation/NOTE.md:34` — Summary states "an upper bound on L below 1/2 needs a different algorithm", but line 504 concedes fixed tie-breaking for rule R remains undetermined and Theorem 4 relies on adversarial tie-breaking in round 2 — confidence: high — reword to state that rule R with adversarial tie-breaking cannot beat 1/2, or that beating 1/2 requires a different algorithm or an algorithmic tie-breaking rule.
F: LOW — `14-forest-orientation/upper/greedy_exact.cpp:322` — Usage error message prints `"usage: hybrid S D lo hi [CAP] [MEM_GB] [MINUTES]\n"`, retaining the old program name from which it was adapted — confidence: high — replace `hybrid` with `greedy_exact`.

### Additional Review Notes

1. **Tie-breaking scope in Theorem 4 vs Summary claims**:
   In `NOTE.md:483-485`, round 2 explicitly relies on the Adversary breaking the tie between \(x\) and \(z\) in favor of \(x\). If ties were broken by an algorithmic rule favoring the smaller tree or component index, this exact sequence would not proceed identically. `NOTE.md:504` acknowledges this ("We have not determined the worst case of rule R under a fixed tie-breaking rule"), making the claim in `NOTE.md:34` and `14-forest-orientation/README.md:17` ("so doing better needs a different one") technically an overclaim regarding rule R with algorithmic tie-breaking.

2. **Counterexample for the smaller-tree rule**:
   The computation in `NOTE.md:341-344` was re-verified by hand:
   - \(U\) is complete binary of depth 2 (\(|U|=7\)), with \(\Phi(U) = 1\), \(h(u) = 2\).
   - \(V\) has root \(v\) with two leaves and five degree-1 in-neighbours (\(|V|=8\)), with \(\Phi(V) = 5/2\), \(h(v) = 1\).
   - Smaller-tree rule flips from \(u\) at cost 2. In the resulting 15-vertex tree, \(u\) contributes 1, the path child contributes 1, the path leaf contributes 1/2, untouched vertices in \(U\) contribute 0, and \(V\) still contributes 5/2.
   - Total new \(\Phi = 5\), giving \(\Delta\Phi = 5 - 3.5 = 1.5 = 3/2 < 2\), exactly as stated.

3. **Replay of Theorem 4 for \(j = 2\) and \(j = 3\)**:
   - \(j = 2\) (\(n = 9\)): 3 copies of \(A_1\) (cost 0, \(h = 1\)). Round 1 \(\{x, y\}\) costs 1 and leaves \(x\) with out-neighbours \(\{c, y\}\) with \(h(c)=0, h(y)=1\), so \(h(x)=1\). Round 2 \(\{x, z\}\) forces shortest path through \(c\) of length 1, cost 1. Resulting root has out-neighbours \(\{y, z\}\) of depth 1, so \(h = 2\), total cost \(2 = (9-1)/2 - 2\).
   - \(j = 3\) (\(n = 27\)): 3 copies of \(A_2\) (cost \(3 \times 2 = 6\), root \(h = 2\)). Round 1 costs 2, leaving \(h(c)=1, h(y)=2 \implies h(x)=2\). Round 2 forces path through \(c\) of length 2, cost 2. Total cost \(6 + 2 + 2 = 10 = (27-1)/2 - 3\), with root \(h = 3\).
   - Both match the closed form and Lemma 5 equality \((n-1)/2 - H\).
