<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: REFUTED — code ordering and Lemma 8’s consequence are correct (`NOTE.md:579-590,709-712`), and Lemmas 6–8/Theorem 5 follow; but equal h and ε=0 force identical slots `(h−1,h−1)`, so lexicographic order cannot decide that tie. For concrete ε=1 ties, let X have slots `(0,1)` and Y have `(0,r)` using a leaf plus complete binary out-trees, r=2,3,4: Φ changes respectively `2→5/6`, `7→10/12`, `20→23/25` when flipping from X/Y, always gaining ≥3 for k=1, by the table (`NOTE.md:599-607`) — “Lexicographic ordering decides some ε=1 code ties; ε=0 ties remain ties.”

Q2: CONFIRMED — both statements use exactly `FO.T` from `Spec.lean:73-78` (`Main.lean:7-10`; `UpperSpec.lean:13-15`); current status/diff leaves `Spec.lean` unchanged; the requested-token search found only the harmless tactic macro `ifs_omega` (`UpperCore.lean:21`), no shadowed specification names; and empty-position instantiation is correct (`UpperMain.lean:214-220`) — none

Q3: REFUTED — weights and the one-neighbour case match (`UpperCore.lean:26-43`), ε matches (`UpperPhi.lean:21-29`), and `round_gain` assumes precisely a valid forest, disconnected endpoints, any normal shortest path of length hh, and code order (`UpperRound.lean:441-444`; `Normal.lean:309-313`); however the documentation repeatedly calls Theorem 5 “Section 5” (`lean/SPEC.md:141`) although it is Section 6 — replace “Section 5” with “Section 6”; otherwise the audited upper-bound descriptions are accurate

Q4: REFUTED — the headline equalities, deficit bounds, `314≤T(1000)≤333`, small values, supersession, and tie-breaking comparison follow from `NOTE.md:16-24,40-45,527-531,891-896,1017-1022`; novelty wording is appropriately qualified (`NOTE.md:50-54,908-913,982-984`), and I know no contrary reference; but `prior-art.md:20` still says the current bounds are `1/3≤L≤1/2` — replace with “L=1/3; the deficit remains open”

Q5: REFUTED — generated rounds use the correct table/code, all four programs return nonzero on detected failure, and every recorded figure matches `third_log.txt:1-60`; but `third_pool.cpp:65-68,94-100` tests only one endpoint per `(h,slots)` signature, contrary to its “all replies” wording (`third_pool.cpp:3-6`). Two rule-L-built vertices can both have slots `(0,1)` while their shortest h=0 children have ε=0 and ε=1; connecting them by incoming arcs preserves the signature, and pairing with a code-3 endpoint leaves one real round skipped — “all shortest paths from one representative per signature class”

Q6: CONFIRMED — in Theorem 4’s second round, x has code `2j−1` while z has `2j−2`, so rule L flips from z rather than the adversarially chosen x (`NOTE.md:490-503,579-590`); the known lower values fit upper ceilings 9,10,21. For an off-path test, attach v→x,v→t with t a depth-2 binary out-tree and three vertices pointing into v: v’s slots change `(1,2)→(2,2)`, its weight stays 5, and total Φ changes `10→13` for cost 1 (`NOTE.md:599-607,785-800`). Total code order always supplies an endpoint, and shortest-path replies are legal (`UpperMain.lean:131-151`) — none

F: MODERATE — `prior-art.md:20` — stale `L≤1/2` contradicts the proved `L=1/3` — confidence: high — update the bound

F: MODERATE — `upper/third_pool.cpp:65-68` — endpoint signature reduction skips distinct real rounds while the header says all replies are generated — confidence: high — enumerate every endpoint or narrow the claim

F: LOW — `lean/SPEC.md:141`, `lean/FO/UpperSpec.lean:7`, `UpperCore.lean:2`, `UpperLemma6.lean:2` — wrong NOTE section — confidence: high — change 5 to 6

F: LOW — `lean/PROGRESS.md:2,80-82` — still says the specification has had no independent audit, contradicting the recorded panel audit — confidence: high — mark these as historical or update them

No additional proof defect found.
