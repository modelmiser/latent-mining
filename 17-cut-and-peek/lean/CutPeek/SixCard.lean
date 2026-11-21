/-
A second check that the specification is not vacuous, this time with three bits: the six-card
trick is a single-cut full-open protocol for the equality of three bits in the sense of
`CutPeek/Spec.lean`. Together with `no_scfo_eq` this shows that the bound "four or more bits" in
that theorem cannot be lowered to three.
Positions 0..5; bit 2 controls positions 0 and 3, bit 1 controls 1 and 4, bit 0 controls 2 and 5.
The row for input (0,0,0) is club heart club heart club heart.
-/
import CutPeek.Spec

namespace CutPeek

/-- The six-card trick as an encoding of three bits on six cards. -/
def sixCard : Encoding 3 6 where
  owner := fun p => if p = 0 ∨ p = 3 then some 2 else if p = 1 ∨ p = 4 then some 1 else some 0
  base := fun p => decide (p = 1 ∨ p = 3 ∨ p = 5)

/-- The six-card trick computes three-bit equality. -/
theorem six_card_trick : ∃ e : Encoding 3 6, IsSCFO e (eqFn 3) :=
  ⟨sixCard, by unfold IsSCFO CycEq; decide⟩

end CutPeek
