# budget protocol from the problem text. states: weights K..1 -> idx 0..K-1 (qA=+K idx0), -K..-1 -> idx K..2K-1 (qB=-K idx K), wa=2K, wb=2K+1
import sys
K=int(sys.argv[1]); s=2*K+2
def idx(v):
    if v=='wa': return 2*K
    if v=='wb': return 2*K+1
    return K-v if v>0 else K+(K+v)
print(s); print(idx(K),idx(-K)); print('A'*K+'B'*K+'AB')
def rule(p,q,p2,q2):
    print(idx(p),idx(q),idx(p2),idx(q2)); print(idx(q),idx(p),idx(q2),idx(p2))
for i in range(1,K+1):
    for j in range(1,K+1):
        if i>j: rule(i,-j,i-j,'wa')
        elif i<j: rule(i,-j,'wb',-(j-i))
        else: rule(i,-j,'wa','wb')
    rule(i,'wb',(i-1) if i>1 else 'wa','wa')
    rule(-i,'wa',-(i-1) if i>1 else 'wb','wb')
