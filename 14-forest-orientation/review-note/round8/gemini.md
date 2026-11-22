<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `14-forest-orientation/NOTE.md:578-591`, `709-712`; hand-checked rounds on concrete trees with ties left open by code rule all yield \(\Delta\Phi \ge 3k\) — none
Q2: CONFIRMED — `14-forest-orientation/lean/FO/Spec.lean:78`, `FO/UpperSpec.lean:13`, `FO/UpperMain.lean:213-233`, `lean/AXIOMS.txt:1-87`; zero sorry/axioms/shadowing found — none
Q3: REFUTED — `14-forest-orientation/README.md:186` claims upper bound 549 at \(n = 1000\), and `lean/SPEC.md:141` cites Section 5 instead of Section 6 — in `README.md:186` replace "549" with "333", and in `SPEC.md:141` replace "Section 5" with "Section 6"
Q4: REFUTED — `14-forest-orientation/README.md:186` states "against an upper bound of 549" at \(n = 1000\), contradicting Theorem 5's bound of 333 (`NOTE.md:894`) — in `14-forest-orientation/README.md:186` replace "against an upper bound of 549" with "against an upper bound of 333"
Q5: CONFIRMED — `14-forest-orientation/upper/third_check.py:27-101`, `third_paths.py:16-60`, `third_trees.py:15-83`, `third_pool.cpp:25-109`, `third_log.txt:1-58`; all counts and exit codes verified — none
Q6: CONFIRMED — `14-forest-orientation/NOTE.md:500-532`, `654-708`; `upper/ternary_family.py:35-43` confirms rule L breaks the ternary adversary in round 2 by selecting \(z\) instead of \(x\); all small cases bounded — none

F: MODERATE — `14-forest-orientation/README.md:186` — Stale upper bound 549 for \(n = 1000\) in lower-bound section contradicts Theorem 5 and `NOTE.md:894` (333) — high — Replace "against an upper bound of 549" with "against an upper bound of 333"
F: LOW — `14-forest-orientation/lean/SPEC.md:141` — Cites `NOTE.md` Section 5 for Theorem 5 and Lemmas 6–8, but they are in Section 6 — high — Replace "Section 5" with "Section 6"
F: LOW — `14-forest-orientation/lean/FO/UpperSpec.lean:6` — Docstring cites "Theorem 5 of ../NOTE.md, Section 5" instead of Section 6 — high — Replace "Section 5" with "Section 6"
F: LOW — `14-forest-orientation/lean/FO/UpperCore.lean:2` — Header comment refers to "NOTE.md, Section 5" instead of Section 6 — high — Replace "Section 5" with "Section 6"
F: LOW — `14-forest-orientation/lean/FO/UpperLemma6.lean:2` — Header comment refers to "NOTE.md (Section 5)" instead of Section 6 — high — Replace "Section 5" with "Section 6"

### Important matters not asked about

1. **Intermediate negative potential values.** In `NOTE.md:609`, a "cherry" (a vertex whose two out-neighbours each have 0 out-neighbours) has weight \(w = -1\). For instance, position `exD` on 8 vertices has \(\Phi = -1\). While `NOTE.md:802` notes \(\Phi\) starts at 0 and Lemma 6 shows \(\Phi \le n - 1\) on final trees, \(\Phi\) can dip below 0 during intermediate rounds. The telescoping argument remains valid because every round has \(\Delta\Phi \ge 3k\), but the text should explicitly clarify that \(\Phi \ge 0\) is not an invariant.
2. **Determinism of tie-breaking in Lean.** `NOTE.md` defines rule L non-deterministically when \(2h(x) + \varepsilon(x) = 2h(y) + \varepsilon(y)\) ("either"). In Lean, `round_gain` (`UpperRound.lean:441`) proves the round inequality for either choice, but `ruleL` (`UpperMain.lean:30`) fixes a deterministic choice by preferring \(u\) over \(v\), and steps along a canonical path via `spath`. This suffices for `upper_value`, which exhibits an Algorithm strategy.
3. **Tree pairs enumeration scope in `third_trees.py`.** In `third_trees.py:65-67`, the outer loops iterate over `na in range(1, NT + 1)` and `nb in range(1, NT + 1)`. When run as `third_trees.py 8 14`, pairs of trees where one component has size \(> 8\) (e.g. \(9 + 1 = 10\)) are omitted, even though their sum is \(\le 14\). The text in `NOTE.md:824` states "every pair of such trees with at most 14 vertices together", meaning each tree individually has \(\le 8\) vertices.
4. **Prior art.** No reference in the literature provides an upper bound with constant \(< 2\) for total flips on incremental forests with out-degree 2. The 2014 Gupta-Kumar-Stein analysis gives \(2n\), and Brodal-Fagerberg (1999) yields \(3(n-1)\). Thus \(n/3\) is novel.
