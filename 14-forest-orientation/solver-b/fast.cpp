// fast.cpp : exact game value T(n) using charge representation + normal-form Algorithm moves.
// charge c(v) = 1 - outdeg(v) in {+1,0,-1}; orientation of a tree is determined by charges.
// Algorithm moves (normal form N, proved WLOG in proofs.txt): pick endpoint x in {u,v}, pick w reachable
// from x by a directed path all of whose vertices before w are saturated, w unsaturated; flip path (cost = length),
// orient new edge out of x. Net effect on charges: c(w) -= 1.
// usage: fast n [mode]   mode 0 = normal-form moves ; mode 1 = ALL valid orientations of merged tree (slow check)
#include <bits/stdc++.h>
using namespace std;
struct TI { int m; vector<int8_t> c; vector<int> par; vector<int> reps; vector<vector<pair<int,int>>> opts; vector<int> cls; };
vector<TI> T; unordered_map<string,int> tid;
int MODE=0;
static string enc(int x,int p,const vector<vector<int>>&adj,const vector<int8_t>&c){
  vector<string> ch; for(int y:adj[x]) if(y!=p) ch.push_back(enc(y,x,adj,c));
  sort(ch.begin(),ch.end()); string s="("; s+=char('1'+c[x]); for(auto&t:ch)s+=t; s+=")"; return s; }
int intern(const vector<vector<int>>&adj,const vector<int8_t>&c){
  int m=adj.size();
  // centers
  vector<int> deg(m),lay; int rem=m; vector<char> gone(m,0);
  for(int i=0;i<m;i++){deg[i]=adj[i].size(); if(deg[i]<=1)lay.push_back(i);}
  while(rem>2){ vector<int> nx; for(int x:lay){gone[x]=1;rem--; for(int y:adj[x]) if(!gone[y]){ if(--deg[y]==1) nx.push_back(y);} } lay=nx; }
  string best; for(int r:lay){ string s=enc(r,-1,adj,c); if(best.empty()||s<best)best=s; }
  auto it=tid.find(best); if(it!=tid.end()) return it->second;
  TI t; t.m=m; t.c.resize(m); t.par.assign(m,-1);
  { int n=0; vector<int> st; for(size_t i=0;i<best.size();i++){ if(best[i]=='('){ int v=n++; t.par[v]=st.empty()?-1:st.back(); t.c[v]=best[i+1]-'1'; st.push_back(v); i++; } else st.pop_back(); } }
  vector<vector<int>> a(m); for(int i=1;i<m;i++){a[i].push_back(t.par[i]);a[t.par[i]].push_back(i);}
  // subtree sums & validity
  vector<int> s(m); for(int i=0;i<m;i++)s[i]=t.c[i]; for(int i=m-1;i>=1;i--) s[t.par[i]]+=s[i];
  if(s[0]!=1){fprintf(stderr,"bad total\n");exit(1);} for(int i=1;i<m;i++) if(s[i]!=0&&s[i]!=1){fprintf(stderr,"bad cut\n");exit(1);}
  vector<vector<int>> out(m); for(int i=1;i<m;i++){ if(s[i]==0) out[i].push_back(t.par[i]); else out[t.par[i]].push_back(i);} 
  for(int i=0;i<m;i++) if((int)out[i].size()!=1-t.c[i]){fprintf(stderr,"bad outdeg\n");exit(1);}
  // classes
  map<string,int> cl; t.cls.resize(m);
  for(int i=0;i<m;i++){ string e=enc(i,-1,a,t.c); auto f=cl.find(e); if(f==cl.end()){ cl[e]=t.reps.size(); t.cls[i]=t.reps.size(); t.reps.push_back(i);} else t.cls[i]=f->second; }
  for(int u:t.reps){ vector<pair<int,int>> o; 
    if(t.c[u]>=0) o.push_back({u,0}); else { vector<pair<int,int>> q; q.push_back({u,0}); for(size_t h=0;h<q.size();h++){ auto [x,d]=q[h]; for(int y:out[x]){ if(t.c[y]>=0) o.push_back({y,d+1}); else q.push_back({y,d+1}); } } }
    t.opts.push_back(o); }
  int id=T.size(); T.push_back(t); tid[best]=id; return id; }
unordered_map<unsigned long long,int> mergeMemo;
int mergeT(int A,int u,int B,int v,int side,int w){ // side 0: w in A, 1: w in B
  unsigned long long key=((unsigned long long)A<<40)^((unsigned long long)B<<16)^((unsigned long long)u<<11)^((unsigned long long)v<<6)^((unsigned long long)side<<5)^w;
  auto it=mergeMemo.find(key); if(it!=mergeMemo.end()) return it->second;
  const TI&a=T[A]; const TI&b=T[B]; int m=a.m+b.m; vector<vector<int>> adj(m); vector<int8_t> c(m);
  for(int i=0;i<a.m;i++){c[i]=a.c[i]; if(i){adj[i].push_back(a.par[i]);adj[a.par[i]].push_back(i);} }
  for(int i=0;i<b.m;i++){c[a.m+i]=b.c[i]; if(i){adj[a.m+i].push_back(a.m+b.par[i]);adj[a.m+b.par[i]].push_back(a.m+i);} }
  adj[u].push_back(a.m+v); adj[a.m+v].push_back(u); c[side? a.m+w:w]--;
  int id=intern(adj,c); mergeMemo[key]=id; return id; }
// mode 1: all valid charge vectors of merged tree with cost; returns list of (id,cost)
void allOrient(int A,int u,int B,int v,vector<pair<int,int>>&res){
  const TI&a=T[A]; const TI&b=T[B]; int m=a.m+b.m; vector<pair<int,int>> E; 
  for(int i=1;i<a.m;i++)E.push_back({i,a.par[i]}); for(int i=1;i<b.m;i++)E.push_back({a.m+i,a.m+b.par[i]});
  // old orientation bit: 1 if first->second. first is child: child->parent iff s[child]==0
  vector<int> old; { vector<int> s(a.m); for(int i=0;i<a.m;i++)s[i]=a.c[i]; for(int i=a.m-1;i>=1;i--){s[a.par[i]]+=s[i];} for(int i=1;i<a.m;i++)old.push_back(s[i]==0);
    vector<int> s2(b.m); for(int i=0;i<b.m;i++)s2[i]=b.c[i]; for(int i=b.m-1;i>=1;i--){s2[b.par[i]]+=s2[i];} for(int i=1;i<b.m;i++)old.push_back(s2[i]==0);} 
  int ne=E.size(); E.push_back({u,a.m+v});
  vector<vector<int>> adj(m); for(auto&e:E){adj[e.first].push_back(e.second);adj[e.second].push_back(e.first);} 
  map<int,int> best;
  for(unsigned mask=0;mask<(1u<<(ne+1));mask++){ vector<int8_t> c(m,1); bool ok=true; int cost=0;
    for(int i=0;i<=ne;i++){ int bit=(mask>>i)&1; int tail=bit?E[i].first:E[i].second; if(--c[tail]<-1){ok=false;break;} if(i<ne&&bit!=old[i])cost++; }
    if(!ok)continue; int id=intern(adj,c); auto it=best.find(id); if(it==best.end()||it->second>cost)best[id]=cost; }
  for(auto&p:best)res.push_back(p); }
struct Bd{int8_t lo,hi;};
unordered_map<string,Bd> memo; long long nodes=0;
static string keyOf(const vector<int>&s){ return string((const char*)s.data(),s.size()*sizeof(int)); }
bool can(const vector<int>&st,int k){
  if(k<=0) return true; if(st.size()<=1) return false;
  string key=keyOf(st); auto it=memo.find(key); bool found=(it!=memo.end()); if(found){ if(k<=it->second.lo)return true; if(k>it->second.hi)return false; }
  nodes++;
  // enumerate adversary moves
  struct Mv{int i,j,cu,cv,pri;}; vector<Mv> mv;
  for(size_t i=0;i<st.size();i++){ if(i&&st[i]==st[i-1])continue; for(size_t j=i;j<st.size();j++){ if(j==i){ if(i+1>=st.size()||st[i+1]!=st[i])continue; } else if(st[j]==st[j-1]&&j-1!=i) continue; 
      size_t jj=(j==i)?i+1:j; const TI&a=T[st[i]]; const TI&b=T[st[jj]];
      for(size_t cu=0;cu<a.reps.size();cu++)for(size_t cv=(st[i]==st[jj]?cu:0);cv<b.reps.size();cv++){ int p=a.c[a.reps[cu]]+b.c[b.reps[cv]]; mv.push_back({(int)i,(int)jj,(int)cu,(int)cv,p*100-(a.m+b.m)}); } } }
  stable_sort(mv.begin(),mv.end(),[](const Mv&x,const Mv&y){return x.pri<y.pri;});
  bool win=false;
  for(auto&m:mv){ int A=st[m.i],B=st[m.j]; int u=T[A].reps[m.cu], v=T[B].reps[m.cv];
    vector<pair<int,int>> res; // (id,cost)
    if(MODE==0){ for(auto&o:T[A].opts[m.cu]) res.push_back({mergeT(A,u,B,v,0,o.first),o.second}); for(auto&o:T[B].opts[m.cv]) res.push_back({mergeT(A,u,B,v,1,o.first),o.second}); }
    else allOrient(A,u,B,v,res);
    sort(res.begin(),res.end(),[](auto&x,auto&y){return x.second<y.second|| (x.second==y.second&&x.first<y.first);});
    res.erase(unique(res.begin(),res.end(),[](auto&x,auto&y){return x.first==y.first;}),res.end()); // same id later has >= cost? ensure
    bool all=true; vector<int> base; for(size_t t=0;t<st.size();t++) if((int)t!=m.i&&(int)t!=m.j) base.push_back(st[t]);
    set<int> seen;
    for(auto&r:res){ if(!seen.insert(r.first).second) continue; if(r.second>=k) continue; vector<int> ns=base; ns.insert(lower_bound(ns.begin(),ns.end(),r.first),r.first);
      if(!can(ns,k-r.second)){all=false;break;} }
    if(all){win=true;break;} }
  Bd&b=memo[key]; if(!found){b.lo=0;b.hi=127;}
  if(win){ if(b.lo<k)b.lo=k; } else { if(b.hi>k-1)b.hi=k-1; }
  return win; }
int main(int argc,char**argv){ int n0=atoi(argv[1]); int n1=argc>2?atoi(argv[2]):n0; if(argc>3)MODE=atoi(argv[3]);
  vector<vector<int>> a1(1); vector<int8_t> c1(1,1); int single=intern(a1,c1);
  for(int n=n0;n<=n1;n++){ memo.clear(); nodes=0; vector<int> st(n,single); int k=0; while(can(st,k+1))k++; 
    printf("n=%d T=%d nodes=%lld states=%zu trees=%zu\n",n,k,nodes,memo.size(),T.size()); fflush(stdout);} }
