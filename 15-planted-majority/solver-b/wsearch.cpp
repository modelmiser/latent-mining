// Minimal |W| with 1,K in W, W subset [1..K], such that
//  (S) every w>=2 in W is a sum of two elements of W, and
//  (C) for all x>y in W, z=x-y: z in W, or z=w1+w2 (w1,w2 in W), or [if MIXED] exists x'<x in W with x'-z in W.
// Usage: wsearch K MIXED maxsize
#include <bits/stdc++.h>
using namespace std;
int K,MIXED; vector<int> W; bool in[512];
bool ok(){
  for(int w:W){ if(w<2) continue; bool s=false; for(int a:W) if(a<w && in[w-a]){s=true;break;} if(!s) return false; }
  for(int x:W) for(int y:W){ if(y>=x) continue; int z=x-y; if(in[z]) continue; bool s=false; for(int a:W) if(a<z && in[z-a]){s=true;break;}
    if(!s&&MIXED){ for(int a:W) if(a<x && a>z && in[a-z]){s=true;break;} }
    if(!s) return false; }
  return true;
}
long long cnt=0;
bool rec(int next,int left){ // choose 'left' more elements from [next..K-1], increasing
  if(left==0){ cnt++; return ok(); }
  for(int v=next; v<=K-left; v++){
    // cheap necessary pruning: elements are added in increasing order, so v must already be splittable by smaller chosen ones
    bool s=false; for(int a:W) if(a<v && in[v-a]){s=true;break;} if(!s) continue;
    W.push_back(v); in[v]=true; bool r=rec(v+1,left-1); if(r) return true; W.pop_back(); in[v]=false;
  }
  return false;
}
int main(int argc,char**argv){ K=atoi(argv[1]); MIXED=atoi(argv[2]); int mx=atoi(argv[3]);
  for(int sz=2; sz<=mx; sz++){ W.clear(); memset(in,0,sizeof in); W.push_back(1); in[1]=true; in[K]=true; W.push_back(K);
    // K itself must be splittable: checked in ok()
    if(rec(2,sz-2)){ sort(W.begin(),W.end()); printf("K=%d mixed=%d min|W|=%d W=",K,MIXED,sz); for(int w:W) printf("%d,",w); printf("\n"); return 0; } }
  printf("K=%d mixed=%d none up to size %d\n",K,MIXED,mx); }
