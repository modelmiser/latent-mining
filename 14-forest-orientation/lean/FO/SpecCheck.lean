/-
Sanity checks of the definitions in Spec.lean.  Each of them can fail.
`decide` runs in the kernel; `#guard` runs the compiled definitions and stops the build if the
Boolean is false.
-/
import FO.Graph
namespace FO

/-! The computable `conn` is the reflexive-transitive closure of adjacency: `FO.conn_iff`
(proved in Graph.lean). -/
example {n : Nat} (D : Pos n) (a b : Fin n) :
    conn D a b = true ↔ RT (fun x y => (x, y) ∈ D ∨ (y, x) ∈ D) a b := conn_iff D a b

/-! Small values, known independently: T(n) = 0 for n ≤ 7 (problem statement; exhaustive
solvers of the experiment). -/
example : T 0 = 0 := by decide
example : T 1 = 0 := by decide
example : T 2 = 0 := by decide
#guard T 3 == 0
#guard T 4 == 0

/-! Moves and replies. -/
-- with no edges every ordered pair of different vertices is legal
#guard (moves ([] : Pos 3)).length == 6
-- after the edge 0-1, the pairs inside {0,1} are no longer legal
#guard moves ([(0, 1)] : Pos 3) == [(0, 2), (1, 2), (2, 0), (2, 1)]
-- path 0-1-2 on 4 vertices: only pairs with the isolated vertex 3 remain
#guard moves ([(1, 2), (0, 1)] : Pos 4) == [(0, 3), (1, 3), (2, 3), (3, 0), (3, 1), (3, 2)]
-- all four orientations of a path with two edges are valid
#guard (replies ([(0, 1)] : Pos 3) 1 2).length == 4
-- cost counts OLD edges only: reversing the old edge costs 1, the new edge is free
example : cost ([(0, 1)] : Pos 3) [(1, 2), (1, 0)] = 1 := by decide
example : cost ([(0, 1)] : Pos 3) [(2, 1), (0, 1)] = 0 := by decide
-- out-degree 3 is rejected: star with centre 0 and three out-arcs
example : valid ([(0, 1), (0, 2), (0, 3)] : Pos 4) = false := by decide
example : valid ([(0, 1), (0, 2), (3, 0)] : Pos 4) = true := by decide
-- joining the centre 0 of a cherry (saturated) to 3: of the 8 orientations only the one with
-- three arcs out of 0 is invalid
#guard (replies ([(0, 1), (0, 2)] : Pos 4) 0 3).length == 7
#guard !(replies ([(0, 1), (0, 2)] : Pos 4) 0 3).contains [(0, 3), (0, 1), (0, 2)]

/-! A tiny play with a forced payment.  Two cherries 1 ← 0 → 2 and 4 ← 3 → 5 on 6 vertices.
Adversary names the two saturated centres; whichever way the new edge points, its tail needs
a free slot, so one old edge must be reversed.  One round is worth exactly 1. -/
#guard value 1 ([(0, 1), (0, 2), (3, 4), (3, 5)] : Pos 6) == 1
-- the best reply to the move (0, 3) costs 1 ...
#guard minL ((replies ([(0, 1), (0, 2), (3, 4), (3, 5)] : Pos 6) 0 3).map
  (fun D' => cost [(0, 1), (0, 2), (3, 4), (3, 5)] D')) == 1
-- ... and naming a leaf instead costs nothing
#guard minL ((replies ([(0, 1), (0, 2), (3, 4), (3, 5)] : Pos 6) 1 3).map
  (fun D' => cost [(0, 1), (0, 2), (3, 4), (3, 5)] D')) == 0

/-! The final inequality is not vacuous: at n = 1 it reads 4 ≤ 7 and fails with 8 in place
of 3. -/
example : ¬ (1 + 8 ≤ 3 * T 1 + 7 * Nat.log2 (1 + 1)) := by decide

/-! DELIBERATELY FALSE variants, refused by Lean (kept as comments with the error):

1.  `#guard value 1 ([(0, 1), (0, 2), (3, 4), (3, 5)] : Pos 6) == 2`
    error: Expression ... did not evaluate to `true`

2.  `example : cost ([(0, 1)] : Pos 3) [(1, 2), (1, 0)] = 0 := by decide`
    error: Tactic `decide` proved that the proposition ... is false

3.  In Core.lean, `cleanup` with a loss of 1 instead of 2 per cleanup merge, i.e. hypothesis
    `3 * g + F3 a + rest.length ≤ Phi (a :: rest) + 2`: the final `omega` fails with
    "omega could not prove the goal: a possible counterexample may satisfy the constraints ..."
    (the Type Lemma allows F3 j = F3 b + 2).
See FalseVariants.lean.txt for the exact inputs and outputs. -/

end FO
