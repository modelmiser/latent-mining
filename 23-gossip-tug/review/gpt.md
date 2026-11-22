<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — legality and all forced replies follow `23-gossip-tug/chain/3-question.txt:1-5` and the traced states in `solver-b/proofs.txt:23-39`; symmetry preserves agents 1,2, and full-but-active agents may be named — none

Q2: CONFIRMED — distinct partners, the exact n−1-call phase, chain invariant, exactly two experts, and the n−2 continuation bound—including n=2,3—are established in `23-gossip-tug/solver-a/proofs.txt:10-23` — none

Q3: CONFIRMED — Lemmas 1–3 give valid arbitrary-position bounds and Lemma 3 supplies a strategy against every naming choice (`23-gossip-tug/solver-b/proofs.txt:5-19`); a false lower cutoff would bias upward and a false upper cutoff downward (`reports/solver-b.md:20`) — none

Q4: REFUTED — the strategy guarantee from the initial position is sound, including relabelling, but `solver-a/check.py:52-76` traverses only reachable rows, not every listed row; moreover several reported certificates are absent (`reports/solver-a.md:33-38`, `solver-a/check.log:7-9`). The n=9 result has a lemma-free threshold run, while n=10’s upper bound uses Lemmas 1–3 (`reports/referee.md:12-15`) — “The checker validates every position reachable from the initial state under the stored strategy; accepted but omitted files are listed separately.”

Q5: REFUTED — the games agree (`chain/3-question.txt:1-7`; `README.md:22-32`), but the unqualified shortest-run statement at `23-gossip-tug/README.md:34-35` is false for n=2,3: the recorded minima are 1 and 3, not 0 and 2 (`solver-a/sanity.log:1-3`) — “For n≥4 the shortest run is 2n−4; for n=2,3 it is 1,3. The longest is n(n−1)/2.”

Q6: CONFIRMED — every proof-retelling sentence, including the one-new-knower bound and the chain-qualified one-new-expert bound, follows `solver-b/proofs.txt:5-8,23-39` and `solver-a/proofs.txt:10-23` — none

Q7: REFUTED — the values, increments, solver attributions, variant values and OEIS remark match `reports/referee.md:8-18,48-55` and `prior-art.md:21-28`, but README says the V(m,r) positions “cannot arise in play” (`README.md:110-112`), whereas the source only says they cannot arise in m−1 calls (`reports/referee.md:34`); for m=3, calls {1,2},{1,3},{2,3} do reach one — “those positions cannot be reached in m−1 calls.”

Q8: UNSUPPORTED — the folder carefully limits exactness to computed cases (`23-gossip-tug/README.md:42-52`), but the top-level row’s unqualified “more than that from four people on” (`README.md:54`) asserts the unproved all-n claim expressly disclaimed in `solver-a/proofs.txt:36-37` — “more than 2n−3 for every computed n from 4 through 10.”

F: MODERATE — `README.md:54` — unqualified all-n overclaim — confidence: high — restrict it to computed n=4…10

F: MODERATE — `23-gossip-tug/README.md:34-35` — shortest-run formula has the wrong scope — confidence: high — add n≥4 and the n=2,3 exceptions

F: MODERATE — `23-gossip-tug/README.md:110-112` — “cannot arise” is concretely false — confidence: high — say “cannot arise in m−1 calls”

F: MODERATE — `23-gossip-tug/README.md:131-133` — only two omitted files are disclosed, but `solver-a/cert_n7_H.txt` and `cert_n8_D.txt` are also reported and absent — confidence: high — enumerate all omitted witnesses

F: LOW — `23-gossip-tug/solver-a/check.py:52-76` — unused listed rows are not validated — confidence: high — document reachable-row semantics or validate every row

F: LOW — `23-gossip-tug/README.md:46-47` — file wording obscures that in-tree two-sided witnesses stop at n=6 — confidence: high — distinguish produced/accepted files from shipped files

Unasked: none material beyond these findings.
