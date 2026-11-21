/-
The core of Lemma 1 over the complex numbers, by the discrete Fourier transform on `ZMod m`.
-/
import Mathlib.Analysis.Fourier.ZMod
import CutPeek.Unit

namespace CutPeek

open ZMod

variable {m : ℕ} [NeZero m]

theorem norm_stdAddChar (j : ZMod m) : ‖(stdAddChar j : ℂ)‖ = 1 := by
  rw [stdAddChar_apply]; exact Circle.norm_coe _

/-- Shift rule for the discrete Fourier transform. -/
theorem dft_shift (W : ZMod m → ℂ) (s k : ZMod m) :
    𝓕 (fun p => W (p - s)) k = stdAddChar (-(s * k)) * 𝓕 W k := by
  rw [dft_apply, dft_apply, Finset.mul_sum]
  refine Fintype.sum_equiv (Equiv.subRight s) _ _ (fun j => ?_)
  simp only [Equiv.subRight_apply, smul_eq_mul]
  rw [← mul_assoc, ← AddChar.map_add_eq_mul]
  congr 2
  ring

/-- Fourier form of a pair equation. -/
theorem pair_freq (W : ZMod m → ℂ) (a b c : ZMod m)
    (h : ∀ p, W (p - a) + W (p - b) - W p = W (p - c)) (k : ZMod m) (hD : 𝓕 W k ≠ 0) :
    (stdAddChar (-(a * k)) : ℂ) + stdAddChar (-(b * k)) = 1 + stdAddChar (-(c * k)) := by
  have hf : (fun p => W (p - a)) + (fun p => W (p - b)) - W = fun p => W (p - c) := by
    funext p; simpa using h p
  have := congrFun (congrArg 𝓕 hf) k
  simp only [map_add, map_sub, Pi.add_apply, Pi.sub_apply, dft_shift] at this
  apply mul_right_cancel₀ hD
  linear_combination this

/-- Core of Lemma 1: three single shifts and three pair shifts force the triple shift. -/
theorem core (W : ZMod m → ℂ) (s1 s2 s3 s12 s13 s23 : ZMod m)
    (h12 : ∀ p, W (p - s1) + W (p - s2) - W p = W (p - s12))
    (h13 : ∀ p, W (p - s1) + W (p - s3) - W p = W (p - s13))
    (h23 : ∀ p, W (p - s2) + W (p - s3) - W p = W (p - s23)) (p : ZMod m) :
    W (p - s1) + W (p - s2) + W (p - s3) - 2 * W p
      = W (p - (s12 + s13 + s23 - s1 - s2 - s3)) := by
  have key : (fun p => W (p - s1)) + (fun p => W (p - s2)) + (fun p => W (p - s3)) - (2 : ℂ) • W
      = fun p => W (p - (s12 + s13 + s23 - s1 - s2 - s3)) := by
    apply (dft (N := m) (E := ℂ)).injective
    funext k
    simp only [map_add, map_sub, _root_.map_smul, Pi.add_apply, Pi.sub_apply, Pi.smul_apply, dft_shift,
      smul_eq_mul]
    by_cases hD : 𝓕 W k = 0
    · simp [hD]
    have e12 := pair_freq W s1 s2 s12 h12 k hD
    have e13 := pair_freq W s1 s3 s13 h13 k hD
    have e23 := pair_freq W s2 s3 s23 h23 k hD
    have tri := unit_triple (norm_stdAddChar _) (norm_stdAddChar _) (norm_stdAddChar _)
      (norm_stdAddChar _) (norm_stdAddChar _) (norm_stdAddChar _) e12 e13 e23
    have ht : (stdAddChar (-((s12 + s13 + s23 - s1 - s2 - s3) * k)) : ℂ)
        * (stdAddChar (-(s1 * k)) * stdAddChar (-(s2 * k)) * stdAddChar (-(s3 * k)))
        = stdAddChar (-(s12 * k)) * stdAddChar (-(s13 * k)) * stdAddChar (-(s23 * k)) := by
      simp only [← AddChar.map_add_eq_mul]
      congr 1
      ring
    have hne : (stdAddChar (-(s1 * k)) * stdAddChar (-(s2 * k)) * stdAddChar (-(s3 * k)) : ℂ)
        ≠ 0 := by
      intro h0
      have := congrArg norm h0
      simp at this
    have hu : (stdAddChar (-(s1 * k)) : ℂ) + stdAddChar (-(s2 * k)) + stdAddChar (-(s3 * k)) - 2
        = stdAddChar (-((s12 + s13 + s23 - s1 - s2 - s3) * k)) := by
      apply mul_right_cancel₀ hne
      rw [tri, ht]
    rw [← hu]
    ring
  simpa using congrFun key p

end CutPeek
