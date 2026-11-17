#!/usr/bin/env python3
# Referee witness checker. usage: wcheck.py p k n file
# Lines: "P node charge" | "R node [charge]" | "F node [charge]"; '#' comments ignored.
import sys
def check(p,k,n,lines):
    hand=[k]*p; board={}
    for ln,l in enumerate(lines,1):
        l=l.split('#')[0].split()
        if not l: continue
        t=l[0]; i=int(l[1]); c=int(l[2]) if len(l)>2 else None
        if i<1: return "REJECT line %d: node<1"%ln
        pred = (i==1) or (i-1 in board)
        if t=='P':
            if c is None: return "REJECT line %d: P needs charge"%ln
            if i in board: return "REJECT line %d: node occupied"%ln
            if not pred: return "REJECT line %d: predecessor empty"%ln
            if c not in hand: return "REJECT line %d: no hand pebble of charge %d"%(ln,c)
            hand.remove(c); board[i]=c
        elif t=='R':
            if i not in board: return "REJECT line %d: no pebble"%ln
            if not pred: return "REJECT line %d: R with empty predecessor"%ln
            if c is not None and c!=board[i]: return "REJECT line %d: charge mismatch"%ln
            hand.append(board.pop(i))
        elif t=='F':
            if i not in board: return "REJECT line %d: no pebble"%ln
            if i<2 or pred: return "REJECT line %d: F with pebbled predecessor"%ln
            if c is not None and c!=board[i]: return "REJECT line %d: charge mismatch"%ln
            if board[i]<1: return "REJECT line %d: F on charge 0"%ln
            hand.append(board.pop(i)-1)
        else: return "REJECT line %d: unknown move"%ln
        if len(hand)+len(board)!=p or min(hand+list(board.values()))<0: return "REJECT internal"
    if list(board)!=[n]: return "REJECT final board %s not clean at %d"%(sorted(board),n)
    return "OK"
if __name__=="__main__":
    p,k,n=map(int,sys.argv[1:4]); print(check(p,k,n,open(sys.argv[4]).read().split("\n")))
