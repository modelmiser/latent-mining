/-
Late-thermostat game, History upper bound (Theorem A of open-h/proofs.txt):
for odd d = 2r+1 the "saturated sum" controller keeps |x| ≤ d.
Core Lean only (no Mathlib).

Indexing: real tick t is index t + d + 1, so the padding (x_i = 0 for i ≤ 0,
u_i = 0 for i < 0) occupies indices ≤ d+1 and < d+1.  The controller at index
m + d (m ≥ 1) sees sgn (x m), the level d ticks earlier.
-/

def sgn (a : Int) : Int := if a > 0 then 1 else if a < 0 then -1 else 0
def clamp (q : Int) : Int := if q ≥ 1 then 1 else if q ≤ -1 then -1 else 0

/-- `wsum f a k = f a + f (a+1) + ... + f (a+k-1)` -/
def wsum (f : Nat → Int) (a : Nat) : Nat → Int
  | 0 => 0
  | k+1 => wsum f a k + f (a+k)

theorem sgn_cases (a : Int) :
    (a > 0 ∧ sgn a = 1) ∨ (a = 0 ∧ sgn a = 0) ∨ (a < 0 ∧ sgn a = -1) := by
  unfold sgn; split
  · omega
  · split <;> omega

theorem clamp_cases (q : Int) :
    (q ≥ 1 ∧ clamp q = 1) ∨ (q = 0 ∧ clamp q = 0) ∨ (q ≤ -1 ∧ clamp q = -1) := by
  unfold clamp; split
  · omega
  · split <;> omega

theorem wsum_succ_left (f : Nat → Int) (a k : Nat) :
    wsum f a (k+1) = f a + wsum f (a+1) k := by
  induction k with
  | zero => simp [wsum]
  | succ k ih =>
    have e : a + 1 + k = a + (k+1) := by omega
    show wsum f a (k+1) + f (a+(k+1)) = f a + (wsum f (a+1) k + f (a+1+k))
    rw [ih, e]; omega

theorem wsum_zero (f : Nat → Int) (a k : Nat) (h : ∀ i, i < a + k → f i = 0) :
    wsum f a k = 0 := by
  induction k with
  | zero => rfl
  | succ k ih =>
    show wsum f a k + f (a+k) = 0
    rw [ih (fun i hi => h i (by omega)), h (a+k) (by omega)]; rfl

/-- The game: level, control, disturbance, for odd delay d = 2r+1. -/
structure Play (r : Nat) (x u w : Nat → Int) : Prop where
  step  : ∀ n, x (n+1) = x n + u n + w n
  x0    : ∀ n, n ≤ 2*r+2 → x n = 0
  u0    : ∀ n, n < 2*r+2 → u n = 0
  wb    : ∀ n, -1 ≤ w n ∧ w n ≤ 1
  rest  : ∀ n, w n ≠ 0 → w (n+1) = 0
  ctrl  : ∀ m, 1 ≤ m →
            u (m + (2*r+1)) = - clamp (((r:Int)+1) * sgn (x m) + wsum u m (2*r+1))

variable {r : Nat} {x u w : Nat → Int}

theorem telescope (h : Play r x u w) (a k : Nat) :
    x (a+k) = x a + wsum u a k + wsum w a k := by
  induction k with
  | zero => simp [wsum]
  | succ k ih =>
    show x (a+k+1) = x a + (wsum u a k + u (a+k)) + (wsum w a k + w (a+k))
    rw [h.step, ih]; omega

/-- Rest rule: 2m consecutive disturbances sum to at most m in size. -/
theorem wsum_pairs (h : Play r x u w) (a m : Nat) :
    -(m:Int) ≤ wsum w a (2*m) ∧ wsum w a (2*m) ≤ m := by
  induction m with
  | zero => simp [wsum]
  | succ m ih =>
    have e : wsum w a (2*(m+1)) = wsum w a (2*m) + w (a+2*m) + w (a+2*m+1) := rfl
    have b1 := h.wb (a+2*m)
    have b2 := h.wb (a+2*m+1)
    have rr := h.rest (a+2*m)
    rw [e]
    by_cases hz : w (a+2*m) = 0
    · omega
    · have := rr hz; omega

/-- z m = x m + (u m + ... + u (m+d)) stays within r. -/
theorem z_bound (h : Play r x u w) :
    ∀ m, (-(r:Int) ≤ x m + wsum u m (2*r+2) ∧ x m + wsum u m (2*r+2) ≤ r)
       ∧ (-(2*(r:Int)+1) ≤ x m ∧ x m ≤ 2*r+1) := by
  intro m
  induction m using Nat.strongRecOn with
  | _ m ih =>
    -- band at m
    have hx : -(2*(r:Int)+1) ≤ x m ∧ x m ≤ 2*r+1 := by
      by_cases hm : m ≤ 2*r+2
      · rw [h.x0 m hm]; omega
      · have hz := (ih (m - (2*r+2)) (by omega)).1
        have ht := telescope h (m - (2*r+2)) (2*r+2)
        have hp := wsum_pairs h (m - (2*r+2)) (r+1)
        have e1 : m - (2*r+2) + (2*r+2) = m := by omega
        have e2 : 2*(r+1) = 2*r+2 := by omega
        rw [e1] at ht; rw [e2] at hp
        omega
    refine ⟨?_, hx⟩
    cases m with
    | zero =>
      have : wsum u 0 (2*r+2) = 0 := wsum_zero u 0 (2*r+2) (fun i hi => h.u0 i (by omega))
      rw [this, h.x0 0 (by omega)]; omega
    | succ k =>
      have hz := (ih k (by omega)).1
      have el : wsum u k (2*r+2) = u k + wsum u (k+1) (2*r+1) := wsum_succ_left u k (2*r+1)
      have er : wsum u (k+1) (2*r+2) = wsum u (k+1) (2*r+1) + u (k+1+(2*r+1)) := rfl
      have hs := h.step k
      have hw := h.wb k
      have hc := h.ctrl (k+1) (by omega)
      rw [er, hc]
      rcases sgn_cases (x (k+1)) with ⟨hp, hsg⟩ | ⟨hp, hsg⟩ | ⟨hp, hsg⟩ <;>
        rw [hsg] <;>
        rcases clamp_cases (((r:Int)+1) * _ + wsum u (k+1) (2*r+1)) with ⟨hq, hcl⟩ | ⟨hq, hcl⟩ | ⟨hq, hcl⟩ <;>
        rw [hcl] <;> omega

/-- Theorem A: the saturated-sum controller keeps the level within d = 2r+1. -/
theorem history_upper (h : Play r x u w) (n : Nat) :
    -(2*(r:Int)+1) ≤ x n ∧ x n ≤ 2*r+1 := (z_bound h n).2

#print axioms history_upper
