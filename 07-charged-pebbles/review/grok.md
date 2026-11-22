<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: REFUTED — `open-upper/proofs.txt:62-68` defines `T_i` only on `(a_i, a_{i+1})` and uses “every configuration contains A” with no argument (same assumption at L5, `:70`); W(iii) `:25-30`, L1 `:42-50`, L2 `:51-55`, L3 `:56-58`, and L6 `:72-77` do check — Add `a_{m+1}=∞`, `T_m=S∩(a_m,∞)`, and that a reduction deletes only non-anchors so the L1-reverse keeps A.
Q2: CONFIRMED — `open-upper/proofs.txt:97-101`: the split e=z / pred≠z / e=succ(z) is exhaustive; `:98-99` has `2^c=2^((q-1)-|A''|)`; `V=S0−w` is in `comp(A)` by L1 — none.
Q3: CONFIRMED — `:109-119`: (3b) has `u'>u` because `s−u>2^φ≥s−u'`, `e≠s` because that gap exceeds `2^φ`, and `0<s−u'≤2^φ=2^(q−(k−1)−1)`; `|A|≤1` is used only where `:103` and `:112-115` say so — none.
Q4: CONFIRMED — `:150-151` gives `4U−2^(r'−1)` and `3U`; `:154-159` and `:161-168` match `(f+3)U` and `(f+4)U−2^(r'−1)`; `:138` gives J(f)⇒J(f+1) for r≥1. Reachable boards never have m=q (`:138-139`); the proof does not treat r=0 — none.
Q5: CONFIRMED — `:171-175`: n≤2U is `(f+2)U`; if n>2U then A={n} and, for f=0, J(0) forces A empty so that branch is impossible; (b) is `a_m+2^r−1≤(f+4)U−1`. `f≤pk` is `solver-b/proofs.txt:6-7` — none.
Q6: CONFIRMED — `solver-a/proofs.txt:5-16`: L(q) places `g+2^(q−1)`, and pebble `(j−1) mod p` has been forgotten `floor((j−1)/p)≤k−1` times; `solver-b/proofs.txt:13-32`: Clean/Frontier is the same advance, and the `{c,c−1}` invariant keeps the base charge ≥1 for exactly pk rounds — none.
Q7: CONFIRMED — `solver-a/proofs.txt:20-30` covers every p=2 move; `solver-b/proofs.txt:59-66` shows the predecessor of the lowest y≥3 leaves by an F-move, so y≤2+f — none.
Q8: REFUTED — `README.md:73-75` says the shift is forward; `open-upper/proofs.txt:119` retreats. `README.md:92-93` says prior strategies reach about half as far per erasure; `prior-art.md:12` is `(f+1)·2^(p−2)` against `(f+2)·2^(p−2)`, and `prior-art.md:7`’s `m·2^(n−1)` with `2m−1` erasures is about `2^(n−2)` per erasure, the same coefficient — Say “shifted either way by at most `2^(q−k−1)`” and “slightly less per erasure, `(f+1)` against `(f+2)`”.

F: MODERATE — `open-upper/proofs.txt:62` — L4 never bounds the ray above `a_m`, but `:161`, `:165`, and `:174` cite L4 for it; as written, “every element ≤ `a_m+2^r−1`” does not follow — high — define `T_m=S∩(a_m,∞)` and apply W with c=r.
F: MODERATE — `open-upper/proofs.txt:66` — L4/L5 assume every configuration on the P/R path contains A; that freezing fact is not proved — high — note that deleting an anchor would make `red` drop it, contradicting uniqueness.
F: MODERATE — `README.md:92` — “about half as far per erasure” contradicts the reach formulas in `prior-art.md:7` and `:12` — high — replace with the `(f+1)` versus `(f+2)` comparison.
F: LOW — `README.md:74` — “sliding one pebble forward” omits the backward retreat at `open-upper/proofs.txt:119` — high — say the shift has absolute value at most `2^(q−k−1)`.
F: LOW — `open-upper/proofs.txt:87` — H_A requires `u'<succ_V(u)`; for q=3, A=∅, S0={3,4}, z=3, V={3} has no successor, while |4−3|=1 equals `2^(q−k−1)` — high — add “if a successor exists, else no upper neighbor”.
F: LOW — `open-upper/proofs.txt:101` — the last case of Lemma N writes `w<succ_V(z)` in that same situation — high — same proviso.
F: LOW — `open-upper/proofs.txt:138` — “Always m≤q−1” is false: for q=2 the set {2,4} is reduced and has m=2; it is true for boards reachable from empty — high — restrict the sentence to reachable boards.
F: LOW — `open-upper/proofs.txt:172` — for f=0 the text reads the numeric clause of J, which is only stated for f≥1; the real reason is that J(0) forces A empty — medium — say that.
F: LOW — `open-upper/proofs.txt:28` — W(iii) does not say why nodes >b form a play with ≤c−1 pebbles (it is W(i)) — high — add that sentence.
F: LOW — `open-upper/proofs.txt:115` — “e≠s” is asserted, not tied to `s−u>2^φ` — high — add “so s is not reducible in V”.
F: LOW — `open-upper/proofs.txt:176` — `f≤pk` is one line; the charge sum is only in `solver-b/proofs.txt:6-7` — high — copy that justification.

The Part 3 arithmetic (`:146-175`) matches the claimed bounds, including the tight steps `a_m+2^(r−1)` and `a+2U−2^(r'−1)`. I do not have a counterexample to the Main Theorem. The status table, the “each forgetting buys `2^(p−2)`” sentence, and “How the strategy works” match the solver proofs and Theorem I. The same-model-family limit and the unproved stronger anchor ranking J' are already disclosed in `README.md:84-90` and `open-upper/proofs.txt:188-192`.
