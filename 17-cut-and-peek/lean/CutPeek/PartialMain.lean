/-
Target statements for partial opening. Do not change the statements.
-/
import CutPeek.PartialSpec
import CutPeek.PartialBound
import CutPeek.PartialCheck

namespace CutPeek

/-- Proposition 3 of the note: a valid partial-open protocol for the AND of `n` bits needs at
least `2^n - 1` cards, whatever the opened set. -/
theorem partial_and_card_bound {n m : ℕ} [NeZero m] (e : Encoding n m) (R : Finset (ZMod m))
    (h : IsPartialAnd e R) : 2 ^ n - 1 ≤ m :=
  partial_and_card_bound_aux e R h

/-- The 36-card layout of the note. Position `p` is controlled by bit `i` when the `p`-th digit of
`owner36` is `i + 1`, and is an additional card when it is `0`; its card for bit value `false` is
the `p`-th digit of `u0_36` (`1` = heart). -/
def owner36 : List ℕ := [1,1,1,1,1,1,0,1,2,3,0,1,3,3,1,0,2,2,1,0,2,2,3,0,2,2,2,1,2,3,0,0,3,0,1,2]
def u0_36 : List ℕ := [0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,0,1,1]

def enc36 : Encoding 3 36 where
  owner := fun p => match owner36.getD p.val 0 with
    | 1 => some 0
    | 2 => some 1
    | 3 => some 2
    | _ => none
  base := fun p => decide (u0_36.getD p.val 0 = 1)

/-- The opened positions: the multiples of 6. -/
def R36 : Finset (ZMod 36) := {0, 6, 12, 18, 24, 30}

/-- The 36-card layout is a valid partial-open protocol for the AND of three bits. -/
theorem thirty_six_cards : IsPartialAnd enc36 R36 :=
  partial3_of_checks enc36 R36 ⟨0, by decide⟩ perm36 (by decide) (by decide) (by decide)

end CutPeek
