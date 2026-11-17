import sys
s=set()
for l in sys.stdin:
    w=l.strip()
    if not w or w[0] not in "01": continue
    s.add(min(w[i:]+w[:i] for i in range(len(w))))
for w in sorted(s): print(w)
