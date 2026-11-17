#!/usr/bin/env python3
"""Independent replay checker for a move list in the REAL game (individual charges).
Usage: check_witness.py p k n file
File lines: 'P i c' place a hand pebble of charge c on node i
            'R i c' reversible removal of the pebble (charge c) on node i
            'F i c' forgetful removal of the pebble on node i, charge c BEFORE the move
Lines starting with # are ignored. Exit 0 and print OK iff every move is legal by the
problem's rules and the final board is exactly {n}."""
import sys


def check(p, k, n, lines):
    board = {}            # node -> charge
    hand = [k] * p        # multiset of charges
    nF = 0
    for ln, line in enumerate(lines, 1):
        line = line.strip()
        if not line or line[0] == '#':
            continue
        m, i, c = line.split()
        i = int(i)
        c = int(c)
        pred = (i - 1 == 0) or (i - 1 in board)
        if m == 'P':
            if i < 1 or i in board or not pred or c not in hand:
                return "line %d illegal: %s" % (ln, line)
            hand.remove(c)
            board[i] = c
        elif m == 'R':
            if board.get(i) != c or not pred:
                return "line %d illegal: %s" % (ln, line)
            del board[i]
            hand.append(c)
        elif m == 'F':
            if i < 2 or board.get(i) != c or pred or c < 1:
                return "line %d illegal: %s" % (ln, line)
            del board[i]
            hand.append(c - 1)
            nF += 1
        else:
            return "line %d bad move type" % ln
        assert len(board) + len(hand) == p
    if sorted(board) != [n]:
        return "final board %s is not clean at %d" % (sorted(board), n)
    return "OK clean at %d, %d F-moves" % (n, nF)


if __name__ == "__main__":
    p, k, n = map(int, sys.argv[1:4])
    r = check(p, k, n, open(sys.argv[4]).read().split("\n"))
    print(r)
    sys.exit(0 if r.startswith("OK") else 1)
