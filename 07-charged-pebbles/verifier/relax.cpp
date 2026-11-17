// Referee's relaxed game: q uncharged pebbles; value[set] = min number of F-moves to reach set.
// 0-1 shortest path (deque). Usage: relax q fmax [probe positions...]
// Prints, for each f<=fmax: max clean node and max occupied node among sets with dist<=f, count.
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <deque>
#include <algorithm>
#include <unordered_map>
using namespace std;
typedef uint64_t u64;
int q;
u64 enc(vector<int> v){ sort(v.begin(),v.end()); u64 k=0; for(int x:v) k=k*256+x; return k*8+v.size(); }
vector<int> dec(u64 k){ int n=k%8; k/=8; vector<int> v(n); for(int i=n-1;i>=0;i--){ v[i]=k%256; k/=256; } return v; }
struct H{ size_t operator()(u64 x) const { x^=x>>31; x*=0x9E3779B97F4A7C15ULL; x^=x>>29; return x; } };
int main(int argc,char**argv){
  q=atoi(argv[1]); int fmax=atoi(argv[2]); if(q>6) return 2;
  unordered_map<u64,int,H> dist; deque<pair<u64,int>> dq;
  dist[enc({})]=0; dq.push_back({enc({}),0});
  vector<int> clean(fmax+1,0), top(fmax+1,0); vector<long> cnt(fmax+1,0);
  while(!dq.empty()){
    auto [k,d]=dq.front(); dq.pop_front(); if(dist[k]!=d) continue;
    vector<int> v=dec(k); cnt[d]++;
    if(!v.empty()){ if(v.back()>top[d]) top[d]=v.back(); if(v.size()==1&&v[0]>clean[d]) clean[d]=v[0]; }
    auto has=[&](int x){ return x==0 || binary_search(v.begin(),v.end(),x); };
    auto relax=[&](const vector<int>&w,int nd){ if(nd>fmax) return; u64 nk=enc(w); auto it=dist.find(nk);
      if(it==dist.end()||it->second>nd){ dist[nk]=nd; if(nd==d) dq.push_front({nk,nd}); else dq.push_back({nk,nd}); } };
    if((int)v.size()<q){ for(int j=-1;j<(int)v.size();j++){ int t=(j<0)?1:v[j]+1; if(has(t)&&t!=0){ if(binary_search(v.begin(),v.end(),t)) continue; }
        if(t>255){ fprintf(stderr,"OVERFLOW\n"); return 3; } vector<int> w=v; w.push_back(t); relax(w,d); } }
    for(size_t i=0;i<v.size();i++){ vector<int> w=v; w.erase(w.begin()+i); if(has(v[i]-1)) relax(w,d); else if(v[i]>=2) relax(w,d+1); }
  }
  int c=0,t=0; long n=0; bool allok=true;
  for(int f=0;f<=fmax;f++){ c=max(c,clean[f]); t=max(t,top[f]); n+=cnt[f]; long fc=(long)(f+2)<<(q-2), ft=((long)(f+4)<<(q-2))-1;
    bool ok=(c==fc&&t==ft); if(!ok) allok=false;
    if(!ok||f==fmax||f<2) printf("q=%d f=%d clean=%d top=%d states=%ld formula_clean=%ld formula_top=%ld %s\n",q,f,c,t,n,fc,ft,ok?"EQ":"DIFF"); }
  printf("q=%d f<=%d ALL %s\n",q,fmax,allok?"EQ":"SOME DIFF");
  if(argc>3){ vector<int> w; for(int i=3;i<argc;i++) w.push_back(atoi(argv[i])); auto it=dist.find(enc(w)); printf("probe min F = %d\n", it==dist.end()?-1:it->second); }
}
