#!/usr/bin/env python3
"""certtool.py -- turn a dumped core into the certificate file (NOT part of the checker; nothing here is trusted).
usage: certtool.py minimize IN OUT      relabel each line canonically (newest ball first, first appearance),
                                        drop duplicates, drop every line that has a proper prefix in the set,
                                        sort.  The result depends only on the set A that the dump describes.
       certtool.py drop IN OUT I        copy IN without its line number I (0-based)     [damage test]
       certtool.py add IN OUT "LINE"    copy IN plus one extra line                      [damage test]
Line format: balls newest first, 'wo' two hex digits (receiver, other bin); '-' = empty state."""
import sys
def canon(line):
    line = line.strip()
    if line == '-' or line == '': return '-'
    mp = {}; out = []
    for tok in line.split():
        for ch in tok:
            if ch not in mp: mp[ch] = '%x' % len(mp)
        out.append(mp[tok[0]] + mp[tok[1]])
    return ' '.join(out)
cmd = sys.argv[1]
if cmd == 'minimize':
    S = set(canon(l) for l in open(sys.argv[2]))
    if '-' in S: keep = ['-']
    else:
        keep = []
        for s in S:
            # proper prefixes: first j balls, j = 1 .. k-1  (each ball is 2 chars + a space)
            if not any(s[:3 * j - 1] in S for j in range(1, (len(s) + 1) // 3)): keep.append(s)
    keep.sort(key=lambda s: (len(s), s))
    open(sys.argv[3], 'w').write(''.join(s + '\n' for s in keep))
    print(f'{len(S)} distinct lines in, {len(keep)} minimal lines out')
elif cmd == 'drop':
    i = int(sys.argv[4]); ls = open(sys.argv[2]).read().split('\n')
    print('dropped line:', ls[i]); del ls[i]; open(sys.argv[3], 'w').write('\n'.join(ls))
elif cmd == 'add':
    open(sys.argv[3], 'w').write(open(sys.argv[2]).read() + canon(sys.argv[4]) + '\n')
