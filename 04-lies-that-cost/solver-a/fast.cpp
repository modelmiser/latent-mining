// Fast solver on multiset states.  State = sorted list of (r,count), r = remaining honesty obligation.
// Modes:
//   fast K NMIN NMAX QMAX [prune=1] [clip=1]   : top-down bounded search, prints least q<=QMAX with a win
//   fix  K NMIN NMAX                            : bottom-up fixed point on all reachable unclipped states (proves infinity)
//   strat K N Q                                 : prints a questioner strategy tree (labelled) winning within Q
#include <bits/stdc++.h>
using namespace std;
typedef vector<pair<int,int>> St; // (r,count) sorted by r, counts>0
int K; bool PRUNE=true, CLIP=true;
static string enc(const St&s,int q){ string k; k.reserve(s.size()*2+1); k.push_back((char)q); for(auto&p:s){k.push_back((char)p.first);k.push_back((char)p.second);} return k;}
static void norm(map<int,int>&m, St&out){ out.clear(); for(auto&p:m) if(p.second>0) out.push_back(p); }
static int total(const St&s){int t=0; for(auto&p:s) t+=p.second; return t;}
// apply split: a[i] of group i go to side A. truthA: side A truthful. Returns new state (may be empty).
static St apply(const St&s,const vector<int>&a,bool truthA){
  int A=0,m=0; for(size_t i=0;i<s.size();i++){A+=a[i]; m+=s[i].second;}
  int ell=min(A,m-A); map<int,int> o;
  for(size_t i=0;i<s.size();i++){
    int r=s[i].first; int t = truthA? a[i] : s[i].second-a[i]; int l = s[i].second - t;
    if(t) o[max(r-1,0)]+=t;
    if(l && r==0) o[K*ell]+=l;
  }
  St out; norm(o,out); return out;
}
static St clip(const St&s,int q){ map<int,int> o; for(auto&p:s) o[min(p.first,q)]+=p.second; St out; norm(o,out); return out; }
unordered_map<string,bool> memo;
unsigned long long nodes=0;
struct Split{ vector<int> a; long long score; };
static void splits(const St&s, vector<vector<int>>&out){
  // all splits up to swap symmetry
  vector<int> a(s.size(),0); set<vector<int>> seen;
  while(true){
    vector<int> c(s.size()); for(size_t i=0;i<s.size();i++) c[i]=s[i].second-a[i];
    if(a<=c) out.push_back(a);
    size_t i=0; while(i<s.size() && a[i]==s[i].second){a[i]=0;i++;}
    if(i==s.size()) break; a[i]++;
  }
}
bool win(St s,int q, vector<int>*best=nullptr){
  if(total(s)==1) return true;
  if(q==0) return false;
  if(CLIP) s=clip(s,q);
  if(PRUNE){ long long a=0,b=0; for(auto&p:s){ if(p.first==0)a+=p.second; else b+=p.second; }
    if(q<62 && a*(q+1)+b > (1LL<<q)) return false; }
  string key=enc(s,q);
  if(!best){ auto it=memo.find(key); if(it!=memo.end()) return it->second; }
  nodes++;
  vector<vector<int>> sp; splits(s,sp);
  // order: balance of Berlekamp weights of the two outcomes
  vector<pair<long long,int>> ord;
  for(size_t j=0;j<sp.size();j++){
    long long w[2];
    for(int t=0;t<2;t++){ St o=apply(s,sp[j],t==0); long long a=0,b=0; for(auto&p:o){ if(p.first<=1)a+=p.second; else b+=p.second; } w[t]=a*q+b; }
    ord.push_back({llabs(w[0]-w[1]),(int)j});
  }
  sort(ord.begin(),ord.end());
  bool res=false;
  for(auto&pr:ord){
    auto&a=sp[pr.second]; bool good=true;
    for(int t=0;t<2&&good;t++){ St o=apply(s,a,t==0); if(o.empty()) continue; if(!win(o,q-1)) good=false; }
    if(good){ res=true; if(best)*best=a; break; }
  }
  memo[key]=res; return res;
}
// ---------- strategy extraction with labelled elements ----------
// elems: vector of (label, r) alive. Prints nodes "path : S" where path is string of answers Y/N.
void strat(vector<pair<int,int>> el,int q,string path){
  if(el.size()==1){ printf("%s = %d\n", path.empty()?"-":path.c_str(), el[0].first); return; }
  map<int,int> m; for(auto&e:el) m[min(e.second,CLIP?q:e.second)]++; St s; norm(m,s);
  vector<int> a; bool w=win(s,q,&a); if(!w){ printf("ERROR no win at %s\n",path.c_str()); exit(1);}
  // choose which labelled elements go into S: first a[i] of group i
  vector<int> need=a; vector<int> inS(el.size(),0);
  for(size_t e=0;e<el.size();e++){ int r=min(el[e].second,CLIP?q:el[e].second); for(size_t i=0;i<s.size();i++) if(s[i].first==r){ if(need[i]>0){need[i]--; inS[e]=1;} } }
  int A=0; for(int v:inS)A+=v; int ell=min(A,(int)el.size()-A);
  printf("%s ?", path.empty()?"-":path.c_str()); for(size_t e=0;e<el.size();e++) if(inS[e]) printf(" %d",el[e].first); printf("\n");
  for(int ans=1;ans>=0;ans--){
    vector<pair<int,int>> ne;
    for(size_t e=0;e<el.size();e++){ bool truthful=(inS[e]==ans); int r=el[e].second;
      if(truthful) ne.push_back({el[e].first,max(r-1,0)}); else if(r==0) ne.push_back({el[e].first,K*ell}); }
    string p2=path+(ans?"Y":"N");
    if(ne.empty()){ printf("%s X\n",p2.c_str()); continue; }
    strat(ne,q-1,p2);
  }
}
int main(int argc,char**argv){
  string mode=argv[1]; K=atoi(argv[2]);
  if(mode=="fast"){
    int n0=atoi(argv[3]),n1=atoi(argv[4]),qmax=atoi(argv[5]); if(argc>6)PRUNE=atoi(argv[6]); if(argc>7)CLIP=atoi(argv[7]);
    for(int n=n0;n<=n1;n++){ memo.clear(); nodes=0; St s={{0,n}}; int ans=-1; int q0=0; while((1LL<<q0)<n) q0++;
      for(int q=q0;q<=qmax;q++){ if(win(s,q)){ans=q;break;} }
      printf("k=%d n=%d Q=%d%s nodes=%llu prune=%d clip=%d\n",K,n,ans,ans<0?(" (>"+to_string(qmax)+")").c_str():"",nodes,(int)PRUNE,(int)CLIP); fflush(stdout); }
  } else if(mode=="fix"){
    int n0=atoi(argv[3]),n1=atoi(argv[4]);
    for(int n=n0;n<=n1;n++){
      map<St,int> id; vector<St> states; St s0={{0,n}}; id[s0]=0; states.push_back(s0);
      vector<vector<pair<int,int>>> moves; // per state: list of (o1,o2) ids, -1 = illegal(empty)
      for(size_t i=0;i<states.size();i++){
        St s=states[i]; moves.emplace_back(); if(total(s)==1) continue;
        vector<vector<int>> sp; splits(s,sp);
        for(auto&a:sp){ int o[2]; for(int t=0;t<2;t++){ St x=apply(s,a,t==0); if(x.empty()){o[t]=-1;continue;} auto it=id.find(x); if(it==id.end()){ int nid=states.size(); id[x]=nid; states.push_back(x); o[t]=nid;} else o[t]=it->second; }
          moves[i].push_back({o[0],o[1]}); }
      }
      int N=states.size(); vector<int> rank(N,-1); for(int i=0;i<N;i++) if(total(states[i])==1) rank[i]=0;
      int round=0; bool ch=true;
      while(ch){ ch=false; round++; vector<int> nw;
        for(int i=0;i<N;i++) if(rank[i]<0){ for(auto&m:moves[i]){ bool g=true; if(m.first>=0 && !(rank[m.first]>=0)) g=false; if(m.second>=0 && !(rank[m.second]>=0)) g=false; if(g){nw.push_back(i);break;} } }
        for(int i:nw){rank[i]=round;ch=true;} }
      int lost=0; for(int i=0;i<N;i++) if(rank[i]<0) lost++;
      printf("k=%d n=%d reachable_states=%d Q=%s never_winning_states=%d (fixed point after %d rounds)\n",K,n,N,rank[0]<0?"INF":to_string(rank[0]).c_str(),lost,round-1); fflush(stdout);
    }
  } else if(mode=="strat"){
    int n=atoi(argv[3]),q=atoi(argv[4]); vector<pair<int,int>> el; for(int i=1;i<=n;i++) el.push_back({i,0});
    printf("# questioner strategy: n=%d k=%d wins within q=%d. Lines: '<answer path> ? <elements of S>' (question asked after that path of Y/N answers, '-' = root), '<path> = x' (only x alive), '<path> X' (answer illegal: C empty)\n",n,K,q);
    strat(el,q,"");
  }
  else if(mode=="ek"){ // membership in E_k: win at q=P(n) (Pelc value)? also checks no win at P(n)-1
    int n0=atoi(argv[3]),n1=atoi(argv[4]); if(argc>5)PRUNE=atoi(argv[5]);
    for(int n=n0;n<=n1;n++){ memo.clear(); nodes=0; int q=0; while(true){ long long lhs=(long long)n*(q+1)+((n&1)?(q-1):0); if(lhs<=(1LL<<q))break; q++; }
      St s={{0,n}}; bool lo=win(s,q-1); bool hi=win(s,q);
      printf("k=%d n=%d P=%d win(P-1)=%d win(P)=%d inE=%d nodes=%llu\n",K,n,q,(int)lo,(int)hi,(int)(hi&&!lo),nodes); fflush(stdout); }
  }
}
