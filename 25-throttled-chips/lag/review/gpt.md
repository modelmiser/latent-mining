<!-- Archived by the organiser on 2026-11-21. Report of the GPT seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — add/firing effects follow the definition; (b), (c), and integrality give readiness when M>3 (`solver-a/proofs.txt:21-31`; `lag/PROOFS.md:11-18`) — none

Q2: CONFIRMED — both induction cases, the 1b averaging inequality, harmonic algebra, base, 2.6 add bound, n≤3m, support V, and arbitrary tie-breaking check out (`lag/PROOFS.md:21-40`) — none

Q3: CONFIRMED — the height bound and G inequality follow (`lag/PROOFS.md:43-52`); independently, f(199)=58.93994<f(200)=59.14996 and f(397)=99.92037<f(398)=100.12540, matching `corollary_out.txt:8-9`; h=28 is the first strict improvement (`corollary_out.txt:15-19`), and the 10⁵ crossover is cautiously estimated — none

Q4: CONFIRMED — the finite reservoir gives interior level 5 and η*≥1; integrality proves R1/R2 readiness (`lag/PROOFS.md:68-81`) — none

Q5: CONFIRMED — pads, both B-adds, and both C-adds have the stated levels; the final N firings remove N distinct level-8 flanks, yielding S(8,N) (`lag/PROOFS.md:84-107`) — none

Q6: CONFIRMED — all phase-2 cases are exhaustive, flushing preserves an alive flank, phase 4 permits adversarial firing among all raised flanks, 16m′≤⌊m/2⌋−1, and the recurrence and length sums are valid (`lag/PROOFS.md:108-150`) — none

Q7: UNSUPPORTED — the original firer knows the full list (`chain/3-question.txt:3-6`), while Theorem L explicitly excludes future-aware ties (`lag/PROOFS.md:54-65`); additionally its displayed lower envelope needs t≥108 (`lag/PROOFS.md:58-60`) — “For each fixed history-only tie rule, a rule-dependent list gives lag 0.2log₃₄t−O(1), while every tie rule has lag at most ln t+4.1; future-aware lower bounds remain open.”

Q8: REFUTED — the standard 2.2 and greedy 2.6/3.6/4.0 runs and deliberate failures are supported (`gen_out_39304.txt:10-29`; `replay_out_300000.txt:1-4`; `neg_out.txt:1-6`), and factors 34 and ≈18 are correct; but “0.2 to 0.8 per doubling” is false because 32→64 and 128→256 both increase by 0 (`constant/lagadv2_out.txt:2-5`; `README.md:49`) — “earlier-search changes ranged from 0 to 0.8 per doubling, with plateaus.”

F: MODERATE — `README.md:47`, `README.md:56` — summaries blur ∀rule∃rule-dependent-list into an unconditional logarithmic-lag claim, unsupported for legal future-aware ties — confidence: high — state the causal quantifier explicitly

F: MODERATE — `README.md:49` — claimed 0.2–0.8 increase per doubling has concrete zero-increase counterexamples — confidence: high — change to 0–0.8

F: LOW — `lag/PROOFS.md:58-60` — logarithmic lower-envelope formula is undefined for t≤84 and not derived for 85≤t<108 — confidence: high — add “for t≥108”

F: LOW — `lag/REPORT.md:21` — “every point,” “every lag 1,” and “every later top-lag point is ready” overstate the finite-interior reservoir and R2 readiness condition — confidence: high — say “each point of a finite interval; interior lag 1; an interior maximum with a lower neighbour is ready”

Unasked: no additional material defect found.No follow-up needed.
