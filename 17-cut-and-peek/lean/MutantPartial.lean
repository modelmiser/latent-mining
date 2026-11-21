import CutPeek
namespace CutPeek
-- organiser's failure test: the note's layout with ONE base digit changed (position 0: 0 -> 1). Expected: the proof term is rejected.
def u0_bad : List ℕ := [1,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,0,1,1]
def encBad : Encoding 3 36 where
  owner := enc36.owner
  base := fun p => decide (u0_bad.getD p.val 0 = 1)
example : IsPartialAnd encBad R36 :=
  partial3_of_checks encBad R36 ⟨0, by decide⟩ perm36 (by decide) (by decide) (by decide)
-- and the true layout with the opened set shifted off the strand structure
def RBad : Finset (ZMod 36) := {0, 6, 12, 18, 24, 31}
example : IsPartialAnd enc36 RBad :=
  partial3_of_checks enc36 RBad ⟨0, by decide⟩ perm36 (by decide) (by decide) (by decide)
end CutPeek
