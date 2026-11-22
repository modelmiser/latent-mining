<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `04-lies-that-cost/chain/3-question.txt:4-9`, `04-lies-that-cost/solver-a/proofs.txt:8-19`, `04-lies-that-cost/solver-b/proofs.txt:7-21` — none
Q2: CONFIRMED — `04-lies-that-cost/open-upper/proofs.txt:16-27` — none
Q3: CONFIRMED — `04-lies-that-cost/open-upper/proofs.txt:30-52` — none
Q4: CONFIRMED — `04-lies-that-cost/open-upper/proofs.txt:68-76` — none
Q5: REFUTED — T1/T2 definitions and the k≥13 reduction hold (`04-lies-that-cost/open-upper/proofs.txt:54-59`), and `04-lies-that-cost/open-upper/table_out.txt:1-15,24-29` covers all cases, but referee log `04-lies-that-cost/open-upper-referee/run1.txt:1-13` omits t1 for k=2,3 (starts at k=4) and `04-lies-that-cost/open-upper-referee/run2.txt:1-9` omits Q_2(5) — "T1 and T2 are verified by `table_out.txt`; referee log `run1.txt` logs T1 only for k≥4 and `run2.txt` omits Q_2(5)"
Q6: CONFIRMED — `04-lies-that-cost/open-upper/proofs.txt:84-100` — none
Q7: CONFIRMED — `04-lies-that-cost/solver-a/proofs.txt:21-42`, `04-lies-that-cost/solver-b/proofs.txt:23-41` — none
Q8: REFUTED — `04-lies-that-cost/README.md:46` claims `run1.txt` covers T1 for k=2..13 (refuted by `04-lies-that-cost/open-upper-referee/run1.txt:1-13`), and `04-lies-that-cost/README.md:48` claims headline Theorem 2 follows from table rows above while omitting Table T2 (`04-lies-that-cost/open-upper/proofs.txt:59`) — "Include Table T2 in the results table and qualify that referee's `run1.txt` logs k=4..13 with k=2,3 covered in `refcmp_out.txt`"

F: MODERATE — 04-lies-that-cost/open-upper-referee/run1.txt:1 — `run1.txt` omits T1 runs for k=2 and k=3 (starts at k=4 due to output redirection overwrite), leaving T1 unlogged for k=2,3 in the referee's primary log file — high — re-run `./ref t1 2 >> run1.txt` and `./ref t1 3 >> run1.txt` to restore missing logs
F: MODERATE — 04-lies-that-cost/README.md:48 — Results table asserts headline Theorem 2 follows from rows above but omits Table T2 (P(3) for k=2,3 and P(5) for k=2), without which root exceptions are unproved from the table — high — add Table T2 row to README results table citing `open-upper/proofs.txt:59` and `open-upper/table_out.txt:24-29`
F: LOW — 04-lies-that-cost/open-upper-referee/run2.txt:2 — `run2.txt` omits literal check for Q_2(5) <= 6, leaving one of three T2 base cases unlogged in the referee's literal run — high — add `literal Q_2(5)` to `open-upper-referee/lemmas.py` and re-run into `run2.txt`
F: LOW — 04-lies-that-cost/open-upper/proofs.txt:30 — Lemma S statement omits explicit hypothesis `ell >= 1`, which is needed to ensure liars get obligation k·ell >= 2 (holds implicitly from m>=2, 2·ell>=j-1) — high — add `ell >= 1` explicitly to Lemma S statement
F: LOW — 04-lies-that-cost/open-upper/proofs.txt:62 — Proof text cites 1310 states per k for T1, whereas T1 as defined (a+b <= 2j-3, j <= 14) has 960 states (1310 corresponds to envelope a+b <= 2j) — high — clarify in `proofs.txt` that 960 is the exact count for T1 and 1310 is the tested superset

### Important aspects not asked about (most severe first)

1. **Lemma C is never executed along any root trajectory in Theorem 2 (MODERATE)**
   Theorem 2 only evaluates states reachable from \((n, 0)\). As confirmed in `04-lies-that-cost/reports/open-upper.md:29` and `04-lies-that-cost/open-upper/strat.py`, any state with \(m\) candidates reachable from the root maintains \(m \ge 2j - 2\) until \(j \le 14\), where it transitions directly into Table T1. Lemma C is mathematically necessary to close the universal induction over all nice states in Theorem 1, but is completely bypassed when deriving the headline \(Q_k(n) = P(n)\).
   - Replacement wording: "Note in README §'Why repeated lies do not help' that Lemma C is needed for the general induction of Theorem 1, but root plays for \(Q_k(n)\) transition directly from Lemma S into Table T1 without invoking Lemma C."

2. **Monotonicity of honesty obligations is unproved (LOW)**
   `04-lies-that-cost/reports/open-upper.md:36` observes that simulating a game where obligations are increased fails because \(\ell\) depends on the current alive set (fewer alive candidates can decrease \(\ell\), lowering future lie penalties). The proof circumvents this by strictly tracking states with obligations \(r \ge j\) (permanently blocked), but general obligation monotonicity remains an unproved heuristic.
   - Replacement wording: "Acknowledge in `open-upper/proofs.txt` under OPEN that monotonicity in obligations is unproved."

3. **Equivalence for non-nice states (\(b < a - 1\)) remains open (LOW)**
   Referee's `04-lies-that-cost/reports/referee.md:54` verified on 74,556 states (\(a + b \le 110, j \le 12, k \in \{2, 3\}\)) that \(Q_k\) winnability matches the one-lie game. Theorem 1 only proves this for nice states (\(b \ge a - 1\)). Non-nice states are never reached after the root question, but the full game equivalence remains a conjecture.
   - Replacement wording: none (already marked OPEN in `open-upper/proofs.txt:102`).

4. **Table T1 state count discrepancy across files (LOW)**
   `04-lies-that-cost/README.md:46` states 960 states per \(k\), `04-lies-that-cost/open-upper/proofs.txt:62` cites 1310 states, and `04-lies-that-cost/open-upper/table_out.txt:1` reports 1884 states. These reflect different search envelopes (\(a+b \le 2j-3\) vs \(2j\) vs \(2j+3\)).
   - Replacement wording: "Clarify in `proofs.txt` that 960 is the exact count for \(a+b \le 2j-3\), while 1310 and 1884 are widened search domains."
