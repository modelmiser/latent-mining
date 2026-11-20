# h=3: every list of length 4 over positions 0..12 loses (covers all gap patterns up to 4; larger gaps never interact:
# 4 chips cannot move a chip more than 1 site from where it was added before the game ends).
import itertools, slow
bad=[l for l in itertools.product(range(13),repeat=4) if not slow.survives(l,3)]
print("h=3: winning lists of length 4 in window 13:",len(bad))
