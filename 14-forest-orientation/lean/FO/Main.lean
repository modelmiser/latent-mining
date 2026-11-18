/-
The final theorem, spelled out, and its form without logarithms.
-/
import FO.Full
namespace FO

/-- FINAL THEOREM (Theorem 1 of open-limit/proofs.txt for the full game).
For every `n ≥ 1`:   `n + 3 ≤ 3 T(n) + 7 ⌊log₂(n+1)⌋`. -/
theorem T_lower_bound (n : Nat) (hn : 1 ≤ n) : n + 3 ≤ 3 * T n + 7 * Nat.log2 (n + 1) :=
  main n hn

/-- The same without logarithms:  `2^(2n+6) ≤ 2^(6 T(n)) (n+1)^14`.
Taking `log₂` this is `2n + 6 ≤ 6 T(n) + 14 log₂(n+1)`, i.e.
`T(n) ≥ n/3 - (7/3) log₂(n+1) + 1`, which is the inequality of the paper with `1` in place
of `5/6`. -/
theorem T_lower_bound_pow (n : Nat) (hn : 1 ≤ n) :
    2 ^ (2 * n + 6) ≤ 2 ^ (6 * T n) * (n + 1) ^ 14 := by
  have h := main n hn
  have hl : 2 ^ Nat.log2 (n + 1) ≤ n + 1 := (Nat.le_log2 (by omega)).mp (Nat.le_refl _)
  have h1 : 2 ^ (2 * n + 6) ≤ 2 ^ (6 * T n + Nat.log2 (n + 1) * 14) :=
    Nat.pow_le_pow_right (by omega) (by omega)
  have h2 : 2 ^ (6 * T n + Nat.log2 (n + 1) * 14)
      = 2 ^ (6 * T n) * (2 ^ Nat.log2 (n + 1)) ^ 14 := by rw [Nat.pow_add, Nat.pow_mul 2 (Nat.log2 (n + 1)) 14]
  rw [h2] at h1
  exact Nat.le_trans h1 (Nat.mul_le_mul_left _ (Nat.pow_le_pow_left hl 14))

end FO
