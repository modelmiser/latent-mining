#!/usr/bin/env python3
# Independent witness checker. usage: check.py p k n file
# file lines: "P node charge" | "R node" | "F node"; '#' comments. Accepts iff all moves legal and final config is clean at n.
import sys
p,k,n=map(int,sys.argv[1:4])
hand=[k]*p; board={}
ln=0
for line in open(sys.argv[4]):
    line=line.split('#')[0].split()
    if not line: continue
    ln+=1; op=line[0]; i=int(line[1])
    pred = (i==1) or (i-1 in board)
    if op=='P':
        c=int(line[2])
        if i<1 or i in board or not pred or c not in hand: sys.exit("REJECT move %d: illegal P %s"%(ln,line))
        hand.remove(c); board[i]=c
    elif op=='R':
        if i not in board or not pred: sys.exit("REJECT move %d: illegal R %s"%(ln,line))
        hand.append(board.pop(i))
    elif op=='F':
        if i not in board or i<2 or pred or board[i]<1: sys.exit("REJECT move %d: illegal F %s"%(ln,line))
        hand.append(board.pop(i)-1)
    else: sys.exit("REJECT bad op")
    assert len(hand)+len(board)==p
if set(board)!={n}: sys.exit("REJECT final board %s not clean at %d"%(sorted(board),n))
print("ACCEPT p=%d k=%d clean at %d in %d moves, F-moves used leave charges %s"%(p,k,n,ln,sorted(hand+list(board.values()))))
