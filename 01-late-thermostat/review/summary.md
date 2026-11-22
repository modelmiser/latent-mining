<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261116-181046 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $3.720; this month $97.25 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.861, input 36 + cache write 84,670 + cache read 709,148 tokens — cache hit 89%
- claude:claude-opus-5-5: $0.859, input 22 + cache write 63,728 + cache read 352,786 tokens — cache hit 85%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q7** (split) | CONFIRMED | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED |
| **Q8** (split) | REFUTED | REFUTED | REFUTED | REFUTED | CONFIRMED |
| **Q9** (split) | CONFIRMED | REFUTED | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q7, Q8, Q9
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 9 MODERATE, 25 LOW
- **MODERATE** [claude:claude-opus-5-5] README.md:32 — the headline's status "partly machine-checked, one part open" describes the experiment, not the headline; the headline is entirely hand-proved, with nothing open — high — "proved by hand (one model family); not machine-checked".
- **MODERATE** [claude:claude-opus-5-5] 01-late-thermostat/README.md:28 — "certificate-checked … for d ≤ 13" overstates d=1,3,5, which have no certificates — high — reword as in Q8.
- **MODERATE** [claude:claude-opus-5-5] 01-late-thermostat/README.md:67 and open-c/ — the certificate-size policy is inconsistent: the 2.16 MB KR3 certificate is included, while the exact d=7 certificate (the only one checked by both checkers) and the d=9–13 certificates are missing; readers cannot re-check — high — ship cert_7_7.txt, or state which files are missing and the regeneration commands.
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:44-48` — claimed inequality fails at `d=3,l=1` — confidence: high — replace case II with Solver B’s rest-sensitive argument
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:27` — d=3,5 are described as certificate-checked without certificates/checker logs — confidence: high — distinguish exhaustive and certified cases
- **MODERATE** [cursor:grok-4.7-medium] `solver-a/proofs.txt:30` — “only 3 maps are bounded, with values floor(3d/2)+1, +2, +2” has no upper bound for the f(0)=±1 relays; (c) is one play — high — delete the exact +2 claim or prove a matching upper bound
- **MODERATE** [cursor:grok-4.7-medium] `solver-a/proofs.txt:34,46` — Theorem 2 is tagged complete, but for odd d and s+d even the inequality ⌈(d+1+l)/2⌉−⌊l/2⌋ ≤ ⌊d/2⌋+1 is false (d=1, l=1) — high — case on the parity of l, or point the odd-d upper bound at solver-b P2
- **MODERATE** [cursor:grok-4.7-medium] `README.md:32` — the within-1 headline is marked partly machine-checked and partly open; Lean does not prove it, and it is not open — high — use the Q8 replacement
- **MODERATE** [cursor:gemini-3.8-flash-high] `01-late-thermostat/solver-a/proofs.txt:47` — Case II of Theorem 2 asserts \(\lceil(d+1+l)/2\rceil - \lfloor l/2\rfloor \le \lfloor d/2\rfloor + 1\), which fails for odd \(d\) and odd \(l\) (e.g. \(d=1, l=1\) yields \(2 \le 1\); \(d=3, l=1\) yields \(3 \le 2\)) — confidence: high — Replace with solver-b P2's Phase 2 parity pairing and boundary redo analysis.
- **LOW** [claude:claude-fable-5-1] 01-late-thermostat/README.md:28 — "certificate-checked for d ≤ 13" overstates d=1,3,5 (no certificates/check logs for them) — high — reword as in Q8.
- **LOW** [claude:claude-fable-5-1] README.md:67 vs open-c/proofs.txt:200-206 — files listed in proofs.txt (cert_7_7.txt, cert_9_9*.txt, cert_11_11_KR3.txt, cert_13_13_KR3.txt, mut_cert_*) absent; 2.1 MB cert_7_7_KR3.txt contradicts "above 1.5 MB left out" — high — fix the sentence and the file list.
- **LOW** [claude:claude-fable-5-1] solver-a/proofs.txt:70-72 — T0 not constrained to ≥τ0=2d+2 though Step 2's validity needs Q=P on ticks<2d+2 — medium — add "T0 ≥ 2d+2".
- **LOW** [claude:claude-fable-5-1] solver-a/proofs.txt:46 — false inequality for odd d, odd l (d=1,l=1) — high — repair as in Q3 or delete case II for odd d and cite P2.
- **LOW** [claude:claude-fable-5-1] solver-a/proofs.txt:30 — "only 3 maps are bounded, with values …+2,+2": boundedness/exact value of (+1,±1,-1) only computed — high — tag as computed.
- **LOW** [claude:claude-fable-5-1] solver-a/proofs.txt:62 — "(legal: V rests on odd ticks)" stated for arbitrary V; holds only for the V actually used (P,Q) — high — say "for V ∈ {P,Q}".
- **LOW** [claude:claude-fable-5-1] lean-h/ — no lakefile/toolchain file or build log; reproducibility depends on reader's Lean version (passes on 4.36.0) — medium — add lean-toolchain and the #print axioms output.
- **LOW** [claude:claude-fable-5-1] open-c/proofs.txt:38-53 / README:52-68 — the soundness of the nogood method and the two checkers are themselves same-family hand/code artefacts, not mentioned under "What to distrust" — medium — add a bullet.
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:46 — the inequality fails for odd d and odd l (it is off by 1); the conclusion survives the e-split — high — patch as in Q3.
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:70 — T0 ≥ 2d+2 is not required, but Step 2 needs it — high — add it.
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:30 — "only 3 maps bounded, values +2" is unproved; only lower bounds are given — high — reword as in Q4.
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:112-116 — the "NOT PROVED / not attempted" notes are superseded by open-h and open-c but not marked so — high — add a pointer.
- **LOW** [claude:claude-opus-5-5] solver-a/run_c5.txt — logs a MemoryError for d=5, which looks like a failed run; the d=5 evidence is actually run_clock_cpp_d5.txt — medium — annotate.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:30` — exact values for the two nonoptimal bounded maps lack a general upper-bound proof — confidence: high — add the bound or delete that sentence
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:70-81` — Step 3 should explicitly choose `T0≥2d+2`; even `λ=1` needs the empty-omission convention — confidence: high — state both conventions
- **LOW** [cursor:gpt-5.6-sol-xhigh] `lean-h/HistoryUpper.lean:1-124` — no pinned Lean toolchain or recorded build is included — confidence: high — add reproducible checking instructions
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:64-66` — prior-art conclusions cannot be audited because the report and citations are absent — confidence: high — include the report
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:70` — T0 is not required to be ≥ 2d+2, so the “valid” induction is not literally started — medium — add that lower bound on T0
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:86-87` — the Step 6 switch never says even ticks stay 0, though the −2 count assumes it — high — state w = 0 on even ticks ≥ σ
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:92` — the corollary header says the two-sided gap follows from Theorems 2 and 3; the left inequality is H ≤ C — high — cite the class inclusion
- **LOW** [cursor:grok-4.7-medium] `open-c/proofs.txt:22` — the odd-d clock upper bound is credited to solver-a Theorem 2, whose odd-d writeup is the broken case II — high — cite solver-b P2
- **LOW** [cursor:gemini-3.8-flash-high] `01-late-thermostat/open-c/proofs.txt:75-77` — Certificates `cert_11_11_KR3.txt` and `cert_13_13_KR3.txt` are omitted from the repo due to size, preventing direct execution of `ngcheck.cpp` without regenerating them via `ngclock3` (~7 min for \(d=13\)) — confidence: high — Add a note or helper script documenting certificate regeneration before running checks.
- **LOW** [cursor:gemini-3.8-flash-high] `01-late-thermostat/open-c/check_d11_KR3_python.log:5` — Python checker `ngcheck.py` was aborted at 80,000 / 199,469 nogoods on \(d=11\) due to runtime, leaving only the C++ checker complete — confidence: high — Note in `open-c/proofs.txt` that the Python check was partial for \(d=11\).
- **LOW** [cursor:gemini-3.8-flash-high] `01-late-thermostat/solver-a/proofs.txt:87` — Step 6 states \(2 \cdot \#\{\text{odd ticks in } [\sigma, \sigma^*)\} = 1 - (d+2)\), which conflates the odd tick count \((d+2)/2\) with the subtracted disturbance \(2 \cdot ((d+2)/2) = d+2\) — confidence: high — Explicitly write \(2 \cdot ((d+2)/2) = d+2\).
- **LOW** [cursor:gemini-3.8-flash-high] `01-late-thermostat/solver-a/run_rh.txt:33` — Exact belief-graph enumeration in `solver-a/rh.py` hits Python `MemoryError` at \(d=8\) — confidence: high — Add memory limit checks or reference solver-b's C++ implementation.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
