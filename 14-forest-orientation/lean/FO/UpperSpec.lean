/-
UPPER BOUND, statement only.  Uses the trusted definitions of FO/Spec.lean unchanged.
-/
import FO.Spec
namespace FO

/-- The statement of the upper bound (Theorem 5 of ../NOTE.md, Section 6), in integers:
    `3 T(n) + 1 ≤ n`   for every `n ≥ 1`.
Relation to the real inequality `T(n) ≤ (n - 1)/3`: for `n ≥ 1` the natural-number inequality
`3 T(n) + 1 ≤ n` is the same as `3 T(n) ≤ n - 1` (in the integers), i.e. `T(n) ≤ (n - 1)/3`
(in the rationals), and since `T(n)` is an integer this is `T(n) ≤ ⌊(n - 1)/3⌋`.  Nothing is
lost or gained.  It is false at `n = 0` (`T 0 = 0`, and `1 ≤ 0` fails), hence the hypothesis. -/
def UpperStatement : Prop :=
  ∀ n : Nat, 1 ≤ n → 3 * T n + 1 ≤ n

end FO
