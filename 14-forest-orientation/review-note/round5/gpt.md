<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: REFUTED — `THEOREM3-draft.md:29-36,63-64` only describes \(k≥1\); for \(k=0\), \(c_0\) is undefined and Step 2 excludes edges out of \(p=w_0\). The \(k≥1\) cases check out, including \(k=1,2\) and occupied \(w_k\) — “For \(k=0\), adding \(p→q\) cannot decrease any \(h\); old terms are nondecreasing and the new term is nonnegative.”

Q2: CONFIRMED — `THEOREM3-draft.md:68-80`; indegree sum is \(n-1\), an indegree-zero vertex is unreachable from other vertices, the one-/two-child cases are exhaustive after the \(n=1\) base, and all three maximum cases satisfy the required inequality — none

Q3: CONFIRMED — `THEOREM3-draft.md:84-86`; `14-forest-orientation/NOTE.md:40-47,324-329`; after repairing Q1’s \(k=0\) gap, merge induction gives cost \(\le Φ\), \(n-1\) legal merges leave one tree, positive cost forces \(Φ>0\) and hence \(H≥1\), every tie/path choice is covered, and the integer/max formulation handles small \(n\) — none

Q4: CONFIRMED — `14-forest-orientation/NOTE.md:14-26,254-258,300-309,332-343`; the lower bound and exact small values fit, while \(f\) is merely another upper bound and may be larger. Joining two outward-oriented 3-stars at their centers gives cost \(1=Φ\) on six vertices, not a counterexample — none

Q5: CONFIRMED — `THEOREM3-draft.md:88-89`; subtraction gives \(n/16>\log_2n-3/2\), whose exact first integer solution in the applicable range is \(n=76\); taking the minimum of simultaneously valid upper bounds is valid — “The new real-valued bound is smaller from \(n=76\); use the minimum of it and \(f(n)\).”

Q6: CONFIRMED — `allcheck.py:21-61` generates all stated tree classes, endpoint pairs and recursively all shortest paths, and computes the exact cap-2 potential; `lemma2.py:19-31` generates and checks every size \(2,\dots,N\). These are finite conclusion checks, not proofs of the lemmas — “Finite enumeration through the stated sizes found no violation.”

F: MODERATE — `THEOREM3-draft.md:29-36,63-64` — the \(k=0\) proof invokes inapplicable steps and omits \(p\)’s edges — confidence: high — add the direct argument above

F: MODERATE — `allcheck.py:42-62`; `lemma2.py:27-32` — both programs still exit successfully after violations, making automated “pass” detection unsound — confidence: high — exit nonzero when slack is negative or `bad>0`

F: LOW — `lemma2.py:19-31` — “all smaller ones” omits \(n=1\), although that base is trivial — confidence: high — check or explicitly exclude the base case

Unasked: `THEOREM3-draft.md:98-99` claims pool-search and exact-search results, but neither corresponding program nor output was attached, so those empirical claims remain unsupported.
