# negative control: staircase with every window one tick longer than its work (not saturated)
import sys
from rules import Staircase, run
class Loose(Staircase):
    def __init__(self, P):
        Staircase.__init__(self, P)
        self.Len = [x + 1 for x in self.Len]
        self.T = self.Len[0]
for P in range(3, 7):
    run("control staircase/loose windows", lambda: Loose(P), P - 1)
