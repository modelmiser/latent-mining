<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — Expanding every deletion position and indexing each resulting O finds no shared outcome; the exact checker does this at `solver-a/check.py:8-31` and records all 13 disjoint at `solver-a/log_check.txt:21`; the six-word lower witness is `solver-a/wit_5_0.txt:2-7`, while nonnegative rational weights cover every triple and total 6 (`solver-a/checkcert.py:14-31`, `solver-a/cert_5_0.txt:2-13`), proving A(5)=6 by weak duality — none

Q2: CONFIRMED — The three possible histories are no deletion `{ua}`, deletion after feedback `uD(a)`, and deletion before feedback `D(u)b` (`README.md:15-27`); overlap is exactly decoder ambiguity, while disjoint sets define a decoder; t=0 gives `{a}∪D(a)` and t=n gives `{u}∪D(u)`, correctly making feedback vacuous — none

Q3: CONFIRMED — Length separates `{ua}` from S; for equal `(u,a)` with t<n, both S contain nonempty `uD(a)`, while at t=n equal `(u,a)` means the identical strategy; replacing S′ by S⊆S′ preserves every disjointness relation (`solver-a/proofs.txt:3-13`) — none

Q4: CONFIRMED — For t=1, grouping by u gives at most A(n−1) per prefix and choosing `b=ua′` attains it (`solver-a/proofs.txt:24-29`); for t=n−1, `S={u}∪D(u)b=D(ub)`, yielding A(n) (`solver-a/proofs.txt:15-19`) — none

Q5: CONFIRMED — The two parts have sizes r(a), r(u) and can overlap only in `uD(a)∩D(u)b`; equality forces the deleted-u result to be `u[1..t−1]` and hence exactly `b∈u_tD(a)` (`solver-a/proofs.txt:30-36`); exhaustive checks report zero violations (`verifier/lemmas.log:1`) — none

Q6: CONFIRMED — Prefix stripping gives `cS(u′,a,b)⊆S(cu′,a,b)`, so each first-bit class has size at most F(n−1,t−1) (`solver-b/proofs.txt:20-24`); solver A’s asymptotic counting is valid after replacing its tail estimate (`solver-a/proofs.txt:38-47`) — “Hoeffding gives `(1+o(1))n⁻²=o(1/n)`, not necessarily at most `n⁻²`.”

Q7: CONFIRMED — The checker uses exact `Fraction`s, rejects negative weights, tests every triple, and floors total weight only after establishing weight ≥1 on every S (`solver-a/checkcert.py:14-31`); all n=3,4,5 certificates pass (`solver-a/log_checkcert.txt:1-15`), and the required A(2), A(3), A(4) lower witnesses are in `solver-a/wit_2_0.txt:2-3`, `wit_3_0.txt:2-3`, and `wit_4_0.txt:2-5` — none

Q8: CONFIRMED — Every n≤8 entry is independently supported (`reports/referee.md:10-17`); uncertified cells are correctly identified as search-based, n=9 upper claims are explicitly marked unsupported (`reports/referee.md:20-23`), the prefix counts are indeed 3+4+2+4 (`solver-a/wit_6_2.txt:2-14`), and the literature caveats match `prior-art.md:1-55` — none

F: CRITICAL — `verifier/ref.cpp:35-38`, `verifier/dp_7.log:7-16` — interior n=7 DP exhausts memory and aborts — confidence: high — guard unsupported sizes or use branch-and-bound

F: MODERATE — `solver-a/proofs.txt:41-42` — the cited Hoeffding bound does not imply “at most n⁻²”; its factor reaches 1.6486 at n=9 (`verifier/lemmas.log:2`) — confidence: high — use `(1+o(1))n⁻²`

F: LOW — `solver-b/proofs.txt:20-22` — the stated n≥2 range invokes undefined F(1,·), and the corollary at t=n needs undefined A(0) — confidence: high — restrict the range or define lengths 0 and 1

F: LOW — `solver-b/dp.cpp:21` — argument evaluation makes every memo-size diagnostic print zero (`solver-b/dp.log:1-25`) — confidence: high — evaluate `f(0)` before `printf`

Unasked: both solvers’ 13-strategy witnesses are identical (`verifier/runchk.log:10`), so they are not independent evidence; the README does disclose this.
