<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED — `Spec.lean:29-32`. A path in G+uv that uses the new edge can be shortened so it uses it once. So a~b in G+uv exactly when a~b in G, or a~u and v~b, or a~v and u~b, which is what the recursion says. The base case `a == b` gives reflexivity and the recursion keeps it, so `conn D a a` is always true. This holds for any arc list. A self-loop (u,u) or a repeated edge adds nothing, because a~u and u~b already gives a~b, which matches the undirected graph. — none

Q2: CONFIRMED — `Spec.lean:35-37`. `moves` lists every ordered (u,v) with `conn D u v = false`. Because `conn` is reflexive, u = v is never included. Both orders appear, but they give the same set of replies (`reorient` produces both head arcs), so the maximum is unchanged. Nothing legal is missing. Every round joins two different components and play starts from `[]` (`:78`), so every position reached is a simple forest. — none

Q3: CONFIRMED — `Spec.lean:40-47`. `reorient` keeps or reverses each arc in place, giving all 2^|D| orientations of the same edge sequence: no edge is dropped, duplicated or changed. Both directions of the new edge are included, and `filter valid` keeps exactly the valid ones. `outdeg` (`:19`) counts arcs whose tail is v, and `valid` (`:22`) checks every vertex in `Fin n`. — none

Q4: CONFIRMED — `Spec.lean:50-56`. Every reply has the form `x :: R` with R aligned position by position with D (`:42`), so `D'.tail = R`. An arc and its reversal differ unless it is a self-loop, which never occurs in play. The new edge is the head and is never compared. — none

Q5: CONFIRMED — `Spec.lean:71-78`. The Adversary maximises outside and the Algorithm minimises inside, so the Adversary moves first each round. After k rounds there are n−k components, so whenever a round remains there are at least 2 and `moves` is non-empty. Every forest has a valid orientation (orient each edge toward a root), so `replies` is non-empty too. Both conventions (`maxL [] = 0`, `minL [] = 0`) could only lower T, and neither happens in play. T 1 = value 0 [] = 0, and T 0 = 0 only through truncation, which is excluded by 1 ≤ n. The game is finite with perfect information and the payment depends only on the position, so backward induction gives the same value as strategies that see the history (Zermelo/Kuhn). This equivalence is not formalised, and `SPEC.md:59-61` says so. — none

Q6: CONFIRMED — `Spec.lean:86-87`, `Main.lean:9`. From 3T ≥ n+3−7⌊log⌋ ≥ n+3−7·log₂(n+1), so T ≥ n/3+1−(7/3)log₂(n+1), which is at least the informal bound. Hand checks:
- n=1: 4 ≤ 7, true.
- n=2: 5 ≤ 7, true.
- n=8: 11 ≤ 3T+21, trivially true.
- The statement first has content at n=33: 36 vs 7·5=35, so T(33) ≥ 1.
- n=2^20: it needs T ≥ about (2^20−137)/3. That is consistent with T ≤ n−log₂(n+1), so the statement is not trivially true.
— none

Q7: CONFIRMED — My grep of `FO/*.lean` found no redefinition, attribute, notation, instance or macro on any name in the specification. Graph.lean and Normal.lean define the capitalised `Conn` and `Valid`, which are different names. The only `open` is in `FalseVariants.lean.txt`, which is not built. The `#check` in `AXIOMS.txt` shows `T_lower_bound` has the stated type over `FO.T`. `Fin 0` is excluded by 1 ≤ n. The specification uses no `partial`, `extern` or `implemented_by`. `main : MainStatement` unfolds definitionally to `T_lower_bound`. — none

Q8: REFUTED — `SpecCheck.lean:69` says `FalseVariants.lean.txt` holds "the exact inputs and outputs", but the file (18 lines) has inputs only. The two lists of false variants also disagree (details in the findings). The checks of T 0, T 1 and T 2 cannot fail. — fixes in the findings

F: MODERATE — `SpecCheck.lean:57-69` vs `FalseVariants.lean.txt` — The comment's variant 3 (`cleanup` in Core.lean with loss 1) is not in `FalseVariants.lean.txt`. That file's variants 3 (Type Lemma, index always rises) and 4 (constant 4 in place of 3) are not in the comment. The file contains no outputs, despite "exact inputs and outputs". `README.md:158-159` says "Four ... that Lean refuses are recorded", but the refusals are not recorded anywhere for variants 3 and 4. — high — Make the two lists the same and store the actual error output, or say "inputs only".

F: LOW — `SpecCheck.lean:2,16-18`; `README.md:154`; `SPEC.md:66` — "Each of them can fail" is false for T 0, T 1 and T 2:
- T 0 and T 1 equal `value 0 _ = 0` by definition.
- T 2 = `value 1 []`, where every cost is `flips [] _ = 0`.

None of these depends on `moves`, `replies`, `valid` or `outdeg`. Only T 3 onward and the cherry check (`:45`) test the game. — high — Say that T 0–T 2 hold by construction.

F: LOW — `SpecCheck.lean:11-12` — The `conn_iff` "check" only restates the lemma, so it cannot fail on its own. It is listed as a check that can fail (`README.md:157-158`). — high — Call it a lemma, not a check.

F: LOW — `SpecCheck.lean:53-55` — "Not vacuous" is shown only at n=1, where T 1 = 0 by definition. The theorem first constrains T at n=33, and no check evaluates anything near that size. — medium — State the n ≥ 33 threshold. Treat Q6's large-n reasoning as the argument that the statement has content.

F: LOW — `SPEC.md:37-62` (unlisted choice) — A state is the ordered arc list, so it records the order edges were added, which is more than (G,D). Values of states with the same (G,D) are never identified. This is harmless because the game tree rooted at `[]` is the real game tree, but it is a choice the list leaves out. — medium — Add item 11: "the state also records the order edges were added; the value is computed on that richer tree, which equals the game tree."

F: LOW — `FalseVariants.lean.txt` (last example) — The "constant 4" variant is not shown to be false. At n=1 it holds (7 ≤ 7). It is only not derivable from `key` by that proof. Calling it "deliberately false" is inaccurate. — medium — Call it "not derivable from `key`".

F: LOW — `README.md:144-145` — "Trusting ... those 89 lines and the Lean kernel" leaves out the core-library definitions they use (`Nat.log2`, `List.finRange`, `countP`, `flatMap`). — low — Add "and the core definitions they use".

F: LOW — `SPEC.md:56` — "Neither happens in play" is proved only for the Algorithm's side. The Adversary always having a move is argued in the text (counting components) but not proved. This is harmless, because an empty maximum only lowers T. — medium — Say "proved for replies; for moves by component counting".

Not asked: `Main.lean:12-15` and `SPEC.md:28-34` match the arithmetic. The README's "fourteen supporting" theorems matches `AXIOMS.txt`, which lists 15 including the final theorem. `FO.nb` uses only `propext`. None of the problems I found weakens the implication from the formal theorem to the informal one.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
