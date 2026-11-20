// Unbounded level BFS on integer states, unordered pairs (a<=b) [safe: move set is symmetric under swap].
// States deduplicated per level only (no cross-level visited set needed for correctness: G(n) set at first appearance).
// usage: gbfs T   prints N(t), states per level
#include <bits/stdc++.h>
using namespace std; typedef uint64_t u64;
int main(int c,char**v){int T=atoi(v[1]); vector<int8_t> G((size_t)1<<T|1,-1); G[1]=0; u64 N=1;
 vector<u64> fr={((u64)1<<32)|1},nf; printf("t 0 N 1 states 1\n");
 for(int t=1;t<=T;t++){nf.clear(); nf.reserve(fr.size()*4);
  for(u64 s:fr){u64 a=s>>32,b=s&0xffffffffu; u64 x[4]={2*a,a,a+b,a},y[4]={b,2*b,b,a+b};
   for(int k=0;k<4;k++){u64 p=x[k],q=y[k]; if(p>q)swap(p,q); nf.push_back(p<<32|q);}}
  sort(nf.begin(),nf.end()); nf.erase(unique(nf.begin(),nf.end()),nf.end());
  for(u64 s:nf){u64 a=s>>32,b=s&0xffffffffu; if(G[a]<0){G[a]=t;N++;} if(G[b]<0){G[b]=t;N++;}}
  fr.swap(nf); printf("t %d N %llu states %zu\n",t,(unsigned long long)N,fr.size()); fflush(stdout);}
 printf("G1..64:"); for(int n=1;n<=64&&n<=(1<<T);n++)printf(" %d",G[n]); puts("");
}
