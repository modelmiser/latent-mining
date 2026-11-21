/-
Algebra of complex numbers of absolute value 1 used in the Fourier proof of Lemma 1.
-/
import Mathlib.Analysis.Complex.Basic

namespace CutPeek

open ComplexConjugate

/-- If `a, b, c, d` have absolute value 1 and `a + b = c + d`, then the unordered pairs agree or
`a + b = 0`. -/
theorem unit_pair {a b c d : ℂ} (ha : ‖a‖ = 1) (hb : ‖b‖ = 1) (hc : ‖c‖ = 1) (hd : ‖d‖ = 1)
    (h : a + b = c + d) : (a = c ∧ b = d) ∨ (a = d ∧ b = c) ∨ a + b = 0 := by
  by_cases hS : a + b = 0
  · exact Or.inr (Or.inr hS)
  have ha0 : a ≠ 0 := by rintro rfl; simp at ha
  have hb0 : b ≠ 0 := by rintro rfl; simp at hb
  have hc0 : c ≠ 0 := by rintro rfl; simp at hc
  have hd0 : d ≠ 0 := by rintro rfl; simp at hd
  have hconj : a⁻¹ + b⁻¹ = c⁻¹ + d⁻¹ := by
    rw [Complex.inv_eq_conj ha, Complex.inv_eq_conj hb, Complex.inv_eq_conj hc,
      Complex.inv_eq_conj hd, ← map_add, ← map_add, h]
  have hprod : a * b = c * d := by
    have h1 : (a + b) * (c * d) = (c + d) * (a * b) := by
      field_simp at hconj
      linear_combination hconj
    rw [← h] at h1
    have h2 : (a + b) * (c * d - a * b) = 0 := by linear_combination h1
    rcases mul_eq_zero.mp h2 with h3 | h3
    · exact absurd h3 hS
    · linear_combination -h3
  have hfac : (a - c) * (a - d) = 0 := by linear_combination a * h - hprod
  rcases mul_eq_zero.mp hfac with h3 | h3
  · left
    have hac : a = c := sub_eq_zero.mp h3
    exact ⟨hac, by rw [hac] at h; exact add_left_cancel h⟩
  · right; left
    have had : a = d := sub_eq_zero.mp h3
    exact ⟨had, by rw [had] at h; linear_combination h⟩

/-- The form used for a pair of shifts: `u + u' = 1 + v`. -/
theorem unit_pair_one {u u' v : ℂ} (hu : ‖u‖ = 1) (hu' : ‖u'‖ = 1) (hv : ‖v‖ = 1)
    (h : u + u' = 1 + v) :
    (u = 1 ∧ v = u') ∨ (u' = 1 ∧ v = u) ∨ (u = -u' ∧ v = -1) := by
  rcases unit_pair hu hu' (by simp) hv h with ⟨h1, h2⟩ | ⟨h1, h2⟩ | h1
  · exact Or.inl ⟨h1, h2.symm⟩
  · exact Or.inr (Or.inl ⟨h2, h1.symm⟩)
  · refine Or.inr (Or.inr ⟨by linear_combination h1, ?_⟩)
    rw [h1] at h
    linear_combination -h

/-- The three pair equations force the triple identity. -/
theorem unit_triple {u1 u2 u3 v12 v13 v23 : ℂ}
    (h1 : ‖u1‖ = 1) (h2 : ‖u2‖ = 1) (h3 : ‖u3‖ = 1)
    (k12 : ‖v12‖ = 1) (k13 : ‖v13‖ = 1) (k23 : ‖v23‖ = 1)
    (e12 : u1 + u2 = 1 + v12) (e13 : u1 + u3 = 1 + v13) (e23 : u2 + u3 = 1 + v23) :
    (u1 + u2 + u3 - 2) * (u1 * u2 * u3) = v12 * v13 * v23 := by
  have n2 : u2 ≠ 0 := by rintro rfl; simp at h2
  by_cases a1 : u1 = 1
  · subst a1
    have w12 : v12 = u2 := by linear_combination -e12
    have w13 : v13 = u3 := by linear_combination -e13
    subst w12 w13
    linear_combination v12 * v13 * e23
  by_cases a2 : u2 = 1
  · subst a2
    have w12 : v12 = u1 := by linear_combination -e12
    have w23 : v23 = u3 := by linear_combination -e23
    subst w12 w23
    linear_combination v12 * v23 * e13
  by_cases a3 : u3 = 1
  · subst a3
    have w13 : v13 = u1 := by linear_combination -e13
    have w23 : v23 = u2 := by linear_combination -e23
    subst w13 w23
    linear_combination v13 * v23 * e12
  exfalso
  have p12 : u1 = -u2 := by
    rcases unit_pair_one h1 h2 k12 e12 with ⟨a, _⟩ | ⟨a, _⟩ | ⟨a, _⟩
    · exact absurd a a1
    · exact absurd a a2
    · exact a
  have p13 : u1 = -u3 := by
    rcases unit_pair_one h1 h3 k13 e13 with ⟨a, _⟩ | ⟨a, _⟩ | ⟨a, _⟩
    · exact absurd a a1
    · exact absurd a a3
    · exact a
  have p23 : u2 = -u3 := by
    rcases unit_pair_one h2 h3 k23 e23 with ⟨a, _⟩ | ⟨a, _⟩ | ⟨a, _⟩
    · exact absurd a a2
    · exact absurd a a3
    · exact a
  apply n2
  have : 2 * u2 = 0 := by linear_combination p12 + p23 - p13
  simpa using this

end CutPeek
