// Slow, direct brute force on permutations (no thresholds, no dedupe, no pruning).
// DFS over ALL words of size <= L on n wires; at each node keeps image of reversed input under
// the word and under every slip (mode 0) / every deletion (mode 1) of the word.
// A word passing on the reversed input is then re-checked on ALL n! inputs straight from the definition.
// usage: brute n mode L
#include <bits/stdc++.h>
using namespace std;
int n, mode, L; long long cntRev[40], cntAll[40]; int word[40];
typedef array<signed char,8> P;
static inline void cmp(P&p,int i){ if(p[i-1]>p[i]) swap(p[i-1],p[i]); }
static inline bool sorted(const P&p){ for(int i=0;i+1<n;i++) if(p[i]>p[i+1]) return false; return true; }
bool fullcheck(int len){
  vector<int> v(n); iota(v.begin(),v.end(),1);
  do{
    // unslipped
    { P p; for(int i=0;i<n;i++) p[i]=v[i]; for(int t=0;t<len;t++) cmp(p,word[t]); if(!sorted(p)) return false; }
    for(int pos=0;pos<len;pos++){
      if(mode==0){ for(int d=-1; d<=1; d+=2){ int j=word[pos]+d; if(j<1||j>n-1) continue;
          P p; for(int i=0;i<n;i++) p[i]=v[i]; for(int t=0;t<len;t++) cmp(p, t==pos? j: word[t]); if(!sorted(p)) return false; } }
      else { P p; for(int i=0;i<n;i++) p[i]=v[i]; for(int t=0;t<len;t++) if(t!=pos) cmp(p,word[t]); if(!sorted(p)) return false; }
    }
  } while(next_permutation(v.begin(),v.end()));
  return true;
}
void dfs(int d, P p, vector<P>& Q){
  bool ok=sorted(p); if(ok) for(auto&q:Q) if(!sorted(q)){ ok=false; break; }
  if(ok){ cntRev[d]++; if(fullcheck(d)) cntAll[d]++; }
  if(d==L) return;
  size_t sz=Q.size();
  for(int i=1;i<n;i++){
    word[d]=i;
    for(size_t k=0;k<sz;k++){ Q.push_back(Q[k]); cmp(Q.back(),i); }
    if(mode==0){ if(i>1){ Q.push_back(p); cmp(Q.back(),i-1);} if(i<n-1){ Q.push_back(p); cmp(Q.back(),i+1);} }
    else Q.push_back(p);
    P p2=p; cmp(p2,i);
    vector<P> Q2(Q.begin()+sz,Q.end()); Q.resize(sz);
    dfs(d+1,p2,Q2);
  }
}
int main(int c,char**a){ n=atoi(a[1]); mode=atoi(a[2]); L=atoi(a[3]);
  P p; for(int i=0;i<n;i++) p[i]=n-i; vector<P> Q; dfs(0,p,Q);
  for(int d=0;d<=L;d++) printf("n=%d mode=%d size=%d words_ok_on_reversed=%lld words_ok_on_all_inputs=%lld\n",n,mode,d,cntRev[d],cntAll[d]);
}
