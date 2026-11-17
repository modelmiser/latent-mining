// Referee program: written from the problem definition. Modes:
//  pairs n t      : brute-force check, over all pairs of distinct strategies, that O-disjoint <=> S-disjoint
//  dp n t         : exact F(n,t) by memoised branching on the lowest free short word (no bound, no dominance)
//  sets n t       : dump inclusion-minimal S sets (for LP weight generation)
//  bb n t target wfile : verify weights (w(S)>=K for ALL strategies), then exhaustive search for a packing of
//                        'target' sets; prints FOUND or NONE.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
struct BS{ u64 w[4]; BS(){w[0]=w[1]=w[2]=w[3]=0;}
  void set(int i){w[i>>6]|=1ULL<<(i&63);} bool get(int i)const{return w[i>>6]>>(i&63)&1;}
  void clr(int i){w[i>>6]&=~(1ULL<<(i&63));}
  bool meets(const BS&o)const{return (w[0]&o.w[0])|(w[1]&o.w[1])|(w[2]&o.w[2])|(w[3]&o.w[3]);}
  bool sub(const BS&o)const{return !((w[0]&~o.w[0])|(w[1]&~o.w[1])|(w[2]&~o.w[2])|(w[3]&~o.w[3]));}
  void minus(const BS&o){for(int i=0;i<4;i++)w[i]&=~o.w[i];}
  int cnt()const{return __builtin_popcountll(w[0])+__builtin_popcountll(w[1])+__builtin_popcountll(w[2])+__builtin_popcountll(w[3]);}
  bool operator<(const BS&o)const{for(int i=3;i>=0;i--)if(w[i]!=o.w[i])return w[i]<o.w[i];return false;}
  bool operator==(const BS&o)const{return w[0]==o.w[0]&&w[1]==o.w[1]&&w[2]==o.w[2]&&w[3]==o.w[3];}
};
int n,t;
// word x of length L (first symbol = most significant bit); delete symbol at position i (0 = first)
static inline int del(int x,int L,int i){int r=L-1-i; return ((x>>(r+1))<<r)|(x&((1<<r)-1));}
// S(u,a,b) = u D(a) ∪ D(u) b, as set of words of length n-1
BS Sset(int u,int a,int b){BS s;int m=n-t;
  for(int i=0;i<m;i++){int a2=del(a,m,i); s.set((u<<(m-1))|a2);}
  for(int i=0;i<t;i++){int u2=del(u,t,i); s.set((u2<<m)|b);}
  return s;}
vector<BS> allS(bool minimal){ vector<BS> v; int m=n-t;
  for(int u=0;u<(1<<t);u++)for(int a=0;a<(1<<m);a++)for(int b=0;b<(1<<m);b++){ if(t==0&&b)continue; v.push_back(Sset(u,a,b)); }
  sort(v.begin(),v.end()); v.erase(unique(v.begin(),v.end()),v.end());
  if(!minimal) return v;
  vector<BS> r; for(size_t i=0;i<v.size();i++){bool dom=false; for(size_t j=0;j<v.size()&&!dom;j++) if(i!=j&&v[j].sub(v[i]))dom=true; if(!dom)r.push_back(v[i]);}
  return r;}
// ---------- dp
vector<vector<u64>> byLow; unordered_map<u64,signed char> memo; 
int dp(u64 fr){ if(!fr)return 0; auto it=memo.find(fr); if(it!=memo.end())return it->second;
  int p=__builtin_ctzll(fr); int best=dp(fr&(fr-1));
  for(u64 s:byLow[p]) if((s&~fr)==0){int v=1+dp(fr&~s); if(v>best)best=v;}
  memo[fr]=best; return best;}
// ---------- bb
int NP; vector<BS> F; vector<long long> W; long long K; int target; u64 nodes=0; bool found=false; vector<int> sol;
const long long SC=720720; long long inv[300];
void rec(BS fr,const vector<int>&av,int chosen){
  if(found)return; nodes++;
  if(chosen>=target){found=true;return;}
  static int cntA[64][256]; static int mnA[64][256];
  int depth=min(chosen,63); (void)depth;
  vector<int> cnt(NP,0), mn(NP,1000);
  for(int s:av){int sz=F[s].cnt(); for(int k=0;k<4;k++){u64 x=F[s].w[k]; while(x){int p=(k<<6)+__builtin_ctzll(x); x&=x-1; cnt[p]++; if(sz<mn[p])mn[p]=sz;}}}
  long long Wl=0,Dl=0; int bp=-1;
  for(int p=0;p<NP;p++) if(cnt[p]>0){ Wl+=W[p]; Dl+=inv[mn[p]]; if(bp<0||cnt[p]<cnt[bp]||(cnt[p]==cnt[bp]&&W[p]>W[bp]))bp=p; }
  if(bp<0)return;
  if(chosen+Wl/K<target)return;
  if(chosen+Dl/SC<target)return;
  for(int s:av) if(F[s].get(bp)){ vector<int> av2; av2.reserve(av.size()); for(int q:av) if(!F[q].meets(F[s]))av2.push_back(q);
     BS f2=fr; f2.minus(F[s]); sol.push_back(s); rec(f2,av2,chosen+1); if(found)return; sol.pop_back(); }
  vector<int> av2; for(int q:av) if(!F[q].get(bp))av2.push_back(q);
  BS f2=fr; f2.clr(bp); rec(f2,av2,chosen);
}
string wstr(int x,int L){string s; for(int i=L-1;i>=0;i--)s+=char('0'+((x>>i)&1)); return s;}
int main(int argc,char**argv){ string mode=argv[1]; n=atoi(argv[2]); t=atoi(argv[3]); NP=1<<(n-1); int m=n-t;
  if(mode=="pairs"){ // full outcome sets from the definition: universe = words of length n (index 2^(n-1)+x... ) and n-1
    struct St{BS S; int ua;}; vector<St> v;
    for(int u=0;u<(1<<t);u++)for(int a=0;a<(1<<m);a++)for(int b=0;b<(1<<m);b++){ if(t==n&&b)continue; v.push_back({Sset(u,a,b),(u<<m)|a}); }
    // note: for t==0 strategies with different b are distinct strategies with equal O; keep them all.
    u64 bad=0,pairs=0; for(size_t i=0;i<v.size();i++)for(size_t j=i+1;j<v.size();j++){pairs++; bool Od=!(v[i].ua==v[j].ua||v[i].S.meets(v[j].S)); bool Sd=!v[i].S.meets(v[j].S); if(Od!=Sd)bad++;}
    printf("pairs n=%d t=%d strategies=%zu pairs=%llu mismatches(O-disjoint vs S-disjoint)=%llu\n",n,t,v.size(),pairs,bad); return 0;}
  if(mode=="dp"){ vector<BS> v=allS(false); byLow.assign(NP,{}); for(auto&s:v){ if(s.cnt()==0){printf("EMPTY SET\n");return 1;} byLow[__builtin_ctzll(s.w[0])].push_back(s.w[0]); }
    u64 full= NP==64? ~0ULL : ((1ULL<<NP)-1); int r=dp(full); printf("dp n=%d t=%d distinct_sets=%zu F=%d memo=%zu\n",n,t,v.size(),r,memo.size()); return 0;}
  if(mode=="sets"){ vector<BS> v=allS(true); for(auto&s:v){ bool f=true; for(int p=0;p<NP;p++) if(s.get(p)){printf(f?"%d":" %d",p);f=false;} printf("\n"); } return 0;}
  if(mode=="bb"){ target=atoi(argv[4]); W.assign(NP,0); FILE*f=fopen(argv[5],"r"); if(!f){printf("no wfile\n");return 1;} if(fscanf(f,"%lld",&K)!=1)return 1; {int p; long long w; while(fscanf(f,"%d %lld",&p,&w)==2){ if(p<0||p>=NP||w<0){printf("BAD WEIGHT FILE\n");return 1;} W[p]=w; }} fclose(f);
    vector<BS> all=allS(false); long long tot=0; for(auto x:W)tot+=x; u64 viol=0; for(auto&s:all){ long long ws=0; for(int p=0;p<NP;p++) if(s.get(p))ws+=W[p]; if(ws<K)viol++; }
    printf("bb n=%d t=%d weights: total/K=%.6f  sets_all=%zu violated=%llu\n",n,t,(double)tot/K,all.size(),viol); if(viol){printf("WEIGHTS INVALID\n");return 1;}
    for(int i=1;i<300;i++) inv[i]=(SC+i-1)/i;
    F=allS(true); vector<int> av(F.size()); iota(av.begin(),av.end(),0); BS fr; for(int p=0;p<NP;p++)fr.set(p);
    rec(fr,av,0);
    printf("bb n=%d t=%d target=%d minimal_sets=%zu nodes=%llu result=%s\n",n,t,target,F.size(),nodes,found?"FOUND":"NONE");
    if(found){ // re-verify disjointness
      BS acc; bool ok=true; for(int s:sol){ if(acc.meets(F[s]))ok=false; for(int k=0;k<4;k++)acc.w[k]|=F[s].w[k]; } printf("found packing size %zu disjoint=%d\n",sol.size(),ok); }
    return 0;}
  return 1;}
