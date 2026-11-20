// N(t), G(n): level-by-level enumeration of ALL integer states reachable in exactly t steps,
// stored as unordered pairs (the move set is symmetric under swapping registers, so this is safe),
// deduplicated within a level only (sort+unique). No other pruning.
// usage: nt tmax  -> prints "t N(t) levelsize", writes G table for n<=2^min(tmax,16) to nt_G.txt
#include <bits/stdc++.h>
using namespace std; typedef uint64_t u64;
int main(int argc,char**argv){
  int T=atoi(argv[1]); vector<signed char> G(((size_t)1<<T)+1,-1); vector<u64> cur{((u64)1<<32)|1},nx; u64 N=0;
  for(int t=0;t<=T;t++){
    for(u64 s:cur){u64 a=s>>32,b=s&0xffffffffu; if(G[a]<0){G[a]=t;N++;} if(G[b]<0){G[b]=t;N++;}}
    printf("%d %llu %zu\n",t,(unsigned long long)N,cur.size()); fflush(stdout);
    if(t==T)break;
    if(t==T-1 && argc>2){ // streaming last level: only mark values, do not store states
      for(u64 s:cur){u64 a=s>>32,b=s&0xffffffffu; u64 v[3]={2*a,2*b,a+b}; for(u64 x:v) if(G[x]<0){G[x]=T;N++;}}
      printf("%d %llu streamed\n",T,(unsigned long long)N); break; }
    nx.clear(); nx.reserve(cur.size()*4);
    auto put=[&](u64 a,u64 b){ if(a>b)swap(a,b); nx.push_back((a<<32)|b); };
    for(u64 s:cur){u64 a=s>>32,b=s&0xffffffffu; put(2*a,b);put(a,2*b);put(a+b,b);put(a,a+b);}
    vector<u64>().swap(cur); sort(nx.begin(),nx.end()); nx.erase(unique(nx.begin(),nx.end()),nx.end()); cur.swap(nx);
  }
  FILE*f=fopen("nt_G.txt","w"); size_t lim=(size_t)1<<min(T,14); for(size_t n=1;n<=lim;n++)fprintf(f,"%zu %d\n",n,(int)G[n]); fclose(f);
  // histogram of excess G(n)-ceil(log2 n) among n with G(n)<=T, and list of non-powers of 2 with excess 0 (first 40)
  FILE*h=fopen("nt_excess.txt","w"); map<int,u64> hist; int shown=0;
  for(size_t n=1;n<=((size_t)1<<T);n++) if(G[n]>=0){int c=0; while(((size_t)1<<c)<n)c++; hist[G[n]-c]++; if(G[n]==c&&(n&(n-1))&&shown<40){fprintf(h,"excess0_nonpow2 n=%zu G=%d\n",n,(int)G[n]);shown++;}}
  for(auto&p:hist)fprintf(h,"excess %d count %llu\n",p.first,(unsigned long long)p.second); fclose(h);
}
