#!/usr/bin/env python3
"""Two-level ("quad") scheme.  n = 4*Qn + 1 cells: Qn quads of two pairs of
two cells each, plus one flag cell (the last).  See PROOF.md.

pair states: U = 00, L0 = 10 (first cell set, value 0), L1 = 01 (value 1), D = 11
"""
import sys
from checker import check, count_legal


def make(Qn, mutant=None):
    P = 2 * Qn
    n = 2 * P + 1
    FLAG = 1 << (2 * P)

    def pair(c, i):
        return (c >> (2 * i)) & 3          # 0 U, 1 L0, 2 L1, 3 D

    def live(c, i):
        return pair(c, i) in (1, 2)

    def val(c, i):
        return 0 if pair(c, i) == 1 else 1

    def kill(c, i):
        return c | (3 << (2 * i))

    def firstLL(c, start):
        for q in range(start, Qn):
            if live(c, 2 * q) and live(c, 2 * q + 1):
                return q
        return Qn

    def readB(c):
        """mode B reading: (jstar, O as list of pair indices, N as list of (quad, value))"""
        js = firstLL(c, 0)
        N = []
        for q in range(js):
            a, b = live(c, 2 * q), live(c, 2 * q + 1)
            if a and not b:
                N.append((q, 0))
            elif b and not a:
                N.append((q, 1))
        O = [i for i in range(2 * js, P) if live(c, i)]
        return js, O, N

    def Qf(c):
        if not c & FLAG:
            top = None
            for i in range(P):
                if live(c, i):
                    top = val(c, i)
            return 'E' if top is None else top
        js, O, N = readB(c)
        if N:
            return N[-1][1]
        if O:
            return val(c, O[-1])
        return 'E'

    def pushB(c, b):
        js = firstLL(c, 0)
        if js == Qn:
            return c                      # outside the proved budget
        j2 = firstLL(c, js + 1)
        if mutant != 'nowaste':
            for i in range(2 * js + 2, 2 * j2):
                if live(c, i):
                    c = kill(c, i)
        # value 0: first pair survives; value 1: second pair survives
        return kill(c, 2 * js + 1) if b == 0 else kill(c, 2 * js)

    def Ef(c, op):
        if not c & FLAG:
            if op == 'POP':
                for i in reversed(range(P)):
                    if live(c, i):
                        return kill(c, i)
                return c
            b = 0 if op == 'P0' else 1
            for i in range(P):
                if pair(c, i) == 0:
                    return c | ((1 if b == 0 else 2) << (2 * i))
            # freeze: set the flag, clear everything before the first LL quad
            js = firstLL(c, 0)
            c |= FLAG
            if mutant != 'nofreezeclean':
                for i in range(2 * js):
                    if live(c, i):
                        c = kill(c, i)
            return pushB(c, b)
        if op == 'POP':
            js, O, N = readB(c)
            if N:
                q, v = N[-1]
                return kill(kill(c, 2 * q), 2 * q + 1)
            if O:
                return kill(c, O[-1])
            return c
        return pushB(c, 0 if op == 'P0' else 1)

    return n, Qf, Ef


if __name__ == '__main__':
    Qn = int(sys.argv[1])
    t = int(sys.argv[2])
    mutant = sys.argv[3] if len(sys.argv) > 3 else None
    n, Qf, Ef = make(Qn, mutant)
    ok, info = check(n, t, Qf, Ef, strict=True)
    print('quad scheme Qn=%d n=%d t=%d mutant=%s: %s %s' % (
        Qn, n, t, mutant, 'SURVIVES' if ok else 'FAILS', info), flush=True)
