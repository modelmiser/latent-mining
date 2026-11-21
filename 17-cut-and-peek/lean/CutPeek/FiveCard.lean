/-
A check that the specification is not vacuous: the five-card trick is a single-cut full-open
protocol for the AND of two bits in the sense of `CutPeek/Spec.lean`.
Positions 0..4; bit 0 controls positions 0 and 3, bit 1 controls positions 2 and 4, position 1 is
an additional club. The rows are: (0,0) club club club heart heart; (1,0) heart club club club
heart; (0,1) club club heart heart club; (1,1) heart club heart club club. The first three are
rotations of one another and the fourth is not. This is den Boer's layout with the two suits
exchanged and the positions multiplied by 3 modulo 5.
-/
import CutPeek.Spec

namespace CutPeek

/-- The five-card trick as an encoding of two bits on five cards. -/
def fiveCard : Encoding 2 5 where
  owner := fun p => if p = 0 ∨ p = 3 then some 0 else if p = 2 ∨ p = 4 then some 1 else none
  base := fun p => decide (p = 3 ∨ p = 4)

/-- The five-card trick computes AND: the specification allows a protocol for two bits. -/
theorem five_card_trick : ∃ e : Encoding 2 5, IsSCFO e (andFn 2) :=
  ⟨fiveCard, by unfold IsSCFO CycEq; decide⟩

end CutPeek
