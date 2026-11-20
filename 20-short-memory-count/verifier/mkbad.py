#!/usr/bin/env python3
"""Write deliberately broken witnesses (referee's own) into verifier/bad/."""
import os
d = os.path.join(os.path.dirname(os.path.abspath(__file__)), "bad")
os.makedirs(d, exist_ok=True)
def wa(name, w, s, tr, lit):
    with open(os.path.join(d, name), "w") as f:
        f.write("%d\n%d\n%d\n" % (w, s, len(tr)))
        for a, b in tr: f.write("%d %d\n" % (a, b))
        f.write(" ".join(map(str, lit)) + "\n")
S6 = [(0, 2), (0, 2), (1, 3), (2, 4), (3, 5), (4, 6), (4, 6)]
S4 = [(0, 2), (0, 2), (1, 3), (2, 4), (2, 4)]
wa("a1_s_lowered.txt", 6, 1, S6, range(7))                 # true machine, s one too small
wa("a2_w4_all_lit.txt", 4, 1, S4, range(5))                # middle state (spread 2) lit
wa("a3_w4_unlit_cycle.txt", 4, 1, S4, [1, 2, 3])           # self-loops at 0 and 4 unlit
wa("a4_one_edge_changed.txt", 6, 2, [(0, 2), (0, 2), (1, 3), (2, 4), (3, 5), (4, 6), (5, 6)], range(7))
wa("a5_too_many_states.txt", 4, 1, [(0, 1), (2, 3), (0, 3), (4, 5), (1, 3), (5, 5)], range(6))  # 6 reachable states, w=4
wa("a6_wrong_w.txt", 8, 2, S6, range(7))                   # S_6 offered for w=8, s=2
with open(os.path.join(d, "b_bad.txt"), "w") as f:
    f.write("6 7 0 2 0 2 1 3 2 4 3 5 4 6 4 6 1\n")          # B_6 with g claimed 1
    f.write("8 9 0 2 0 2 1 3 2 4 3 5 4 6 5 7 6 8 6 8 2\n")  # B_8 with g claimed 2
    f.write("7 9 0 1 2 3 0 4 4 5 1 3 3 6 3 7 5 8 6 7 2\n")  # solver-b's w7n9 with last edge changed
    f.write("5 4 0 1 0 2 1 3 2 9 2\n")                      # transition out of range
