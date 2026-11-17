// Referee's own exhaustive search for the charged pebble game (written from the problem text).
// Usage: real c1 c2 ... cp     (initial charges; all equal k for C(p,k))
// No position cap: aborts loudly if a position would exceed 255 (encoding limit).
// State: board as map pos->charge, hand as count per charge. Canonical key: sorted list of
// (pos,charge) codes, pos=0 for hand, 12 bits each.
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;
typedef uint64_t u64;
int p;
struct St { int pos[5]; int ch[5]; };
u64 key(const St&s){ int c[5]; for(int i=0;i<p;i++) c[i]=s.pos[i]*16+s.ch[i]; sort(c,c+p); u64 k=0; for(int i=0;i<p;i++) k=k*4096+c[i]; return k; }
St unkey(u64 k){ St s; for(int i=p-1;i>=0;i--){ int c=k%4096; k/=4096; s.pos[i]=c/16; s.ch[i]=c%16; } return s; }
struct H{ size_t operator()(u64 x) const { x^=x>>31; x*=0x9E3779B97F4A7C15ULL; x^=x>>29; return x; } };
int main(int argc,char**argv){
  p=argc-1; if(p<1||p>5) return 2;
  St s0; for(int i=0;i<p;i++){ s0.pos[i]=0; s0.ch[i]=atoi(argv[i+1]); if(s0.ch[i]>15) return 2; }
  unordered_set<u64,H> seen; vector<u64> stack;
  u64 k0=key(s0); seen.insert(k0); stack.push_back(k0);
  int best=0, maxocc=0; long nF_edges=0;
  while(!stack.empty()){
    u64 k=stack.back(); stack.pop_back(); St s=unkey(k);
    int nb=0, top=0; for(int i=0;i<p;i++) if(s.pos[i]){ nb++; if(s.pos[i]>top) top=s.pos[i]; }
    if(top>maxocc) maxocc=top; if(nb==1 && top>best) best=top;
    auto occ=[&](int x){ if(x==0) return true; for(int i=0;i<p;i++) if(s.pos[i]==x) return true; return false; };
    for(int i=0;i<p;i++){
      if(s.pos[i]==0){
        // place pebble i on any empty node t>=1 with t-1 pebbled: t=1 or t=pos[j]+1
        bool dup=false; for(int j=0;j<i;j++) if(s.pos[j]==0&&s.ch[j]==s.ch[i]) dup=true; if(dup) continue;
        for(int j=-1;j<p;j++){ int t; if(j<0) t=1; else { if(s.pos[j]==0) continue; t=s.pos[j]+1; }
          if(occ(t) && t>=1) { bool o=false; for(int q=0;q<p;q++) if(s.pos[q]==t) o=true; if(o) continue; }
          if(t>255){ fprintf(stderr,"POSITION OVERFLOW\n"); return 3; }
          St n=s; n.pos[i]=t; u64 nk=key(n); if(seen.insert(nk).second) stack.push_back(nk); }
      } else {
        int x=s.pos[i]; St n=s; bool ok=false;
        if(occ(x-1)){ n.pos[i]=0; ok=true; }                      // R
        else if(x>=2 && s.ch[i]>=1){ n.pos[i]=0; n.ch[i]--; ok=true; nF_edges++; } // F
        if(ok){ u64 nk=key(n); if(seen.insert(nk).second) stack.push_back(nk); }
      }
    }
  }
  printf("charges"); for(int i=0;i<p;i++) printf(" %d",s0.ch[i]);
  printf(" : C=%d maxocc=%d states=%zu\n",best,maxocc,seen.size());
}
