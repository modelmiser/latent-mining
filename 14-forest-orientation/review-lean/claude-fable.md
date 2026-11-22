<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Q1: CONFIRMED** — Spec.lean:29-32. Induction on the list: a shortest a–b walk in G+{u,v} uses the new edge at most once, so it is a walk in G, or a–u, uv, v–b, or a–v, vu, u–b; base case a==b. Holds for any list (repeats, loops: the loop clause collapses to conn D a u ∧ conn D u b ⇒ conn D a b). Reflexive by the first disjunct. Independently, Graph.lean:12,71,75,127 prove conn ↔ RT adj. — none

**Q2: CONFIRMED** — Spec.lean:35-37. u=v excluded since conn D u u = true. Both (u,v),(v,u) are listed; reorient (line 42) emits both head directions so reply sets, costs and values coincide → duplicate in maxL, no effect. From [] only non-connected pairs are added ⇒ forest, simple, no loops (Graph.lean:171,175). With j ≤ n−2 edges there are ≥2 components, so `moves` is never empty in play. — none

**Q3: CONFIRMED** — Spec.lean:19,22,40-47. reorient keeps every arc in place as (a,b) or (b,a): same edges, same count, nothing dropped/duplicated; replies = reorient((u,v)::D).filter valid gives both directions of the new edge and any change to any old edge. outdeg counts arcs with tail v; valid checks every v ∈ finRange n. — none

**Q4: CONFIRMED** — Spec.lean:50-56. Every reply is h :: D'' with D'' ∈ reorient D (same length, aligned); flips counts positions with differing arcs; (a,b)≠(b,a) when a≠b. Head excluded by `.tail`. SpecCheck.lean:32-33. — none

**Q5: CONFIRMED** — Spec.lean:59-78. Outer maxL over moves, inner minL over replies: Adversary first. minL [] cannot occur (any forest has a valid orientation; Full.lean:260, Reply.lean:43); if it did, 0 < +∞ makes formal T smaller — safe. maxL [] cannot occur (Q2); its 0 equals "no further payment". T 1 = value 0 [] = 0; T 0 = 0 by truncation, excluded by 1 ≤ n. Backward induction = history-strategy value for finite perfect-information games (Zermelo), unformalised, declared SPEC.md:59-61. — SPEC.md:55: "the minL convention can only make T smaller; the maxL convention equals 'game ends, pays 0'; neither position is reachable".

**Q6: CONFIRMED** — Spec.lean:86-87; Nat.log2 in v4.36.0 (Init/Data/Nat/Log2.lean:39) is ⌊log₂⌋ for n≥1, le_log2 at Lemmas.lean:1349. Divide by 3: T ≥ n/3+1−(7/3)⌊log₂(n+1)⌋ ≥ n/3−(7/3)log₂(n+1)+5/6. n=1: 4≤7; n=2: 5≤7; n=8: 11≤3+21; n=1000: T≥314 vs upper bound ≈990. Not vacuous, not trivial; SpecCheck.lean:55 shows constant 8 fails. — none

**Q7: CONFIRMED** — grep of FO/*.lean: no instance/@[/macro/notation/open/set_option/local/csimp; only new names Valid, Conn, Forest, Al (Prop-valued, capitalised). Main.lean:9 states the unfolded inequality; AXIOMS.txt `#check` prints it with FO.T. MainStatement is a transparent def, `main n hn` type-checks only by unfolding, so equivalent. Fin 0 and n−1 truncation only affect n=0, excluded. — none

**Q8: REFUTED (documentation only)** — SpecCheck.lean:57-69 lists 3 false variants (value, cost, Core cleanup) and promises "exact inputs and outputs" in FalseVariants.lean.txt; that file has 4 variants (value, cost, Type-lemma index, theorem with n+6), no outputs, and no cleanup variant; README.md:158 says "Four". FalseVariants.lean.txt:13 says "constant 4" but code uses n+6. SPEC.md:67 "T 5 = 0 by #eval" is unverifiable. Unlisted choices: `value` total on non-forest lists; `flips` silently truncates unequal lengths (Spec.lean:52). Sanity checks: #guard/decide ones can fail; line 11-12 is a restatement of a theorem (fails only if the proof breaks). — "Three variants in SpecCheck.lean, four inputs (no outputs) in FalseVariants.lean.txt; the Core cleanup variant is recorded only in the comment."

F: LOW — lean/FO/SpecCheck.lean:57-69 vs lean/FalseVariants.lean.txt — variant lists disagree (3 vs 4, different sets), promised error outputs absent — high — make the two lists identical and add the outputs
F: LOW — lean/FalseVariants.lean.txt:13 — comment "constant 4 in place of 3" but the statement uses n + 6 — high — "n + 6 in place of n + 3"
F: LOW — lean/SPEC.md:55-56 — "Both conventions can only make T smaller" is wrong for maxL [] = 0 (equal to the natural convention, larger than −∞); "neither happens in play" for moves is not formally proved — medium — wording above
F: LOW — lean/SPEC.md:67 — "T 5 = 0 … #eval" not reproducible from repository — high — drop or add a log
F: LOW — lean/SPEC.md:37-62 — unlisted choices: `value` is total on arbitrary lists (non-forests, loops); `flips` truncates unequal lengths; `valid` includes the new edge — medium — add as items 11-12
F: LOW — README.md:53 "positive only from n = 34" — the Lean integer form gives T(33) ≥ 1 (36 ≤ 3T+35) — medium — "the hand bound is positive from 34, the Lean form from 33"

Other observations: nothing else. The specification matches the informal game; the only gaps are documentation consistency and the unformalised (standard) equivalence of backward induction with the strategic value.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
