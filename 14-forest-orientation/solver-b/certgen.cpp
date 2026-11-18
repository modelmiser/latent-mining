// fast.cpp : exact game value T(n) using charge representation + normal-form Algorithm moves.
// charge c(v) = 1 - outdeg(v) in {+1,0,-1}; orientation of a tree is determined by charges.
// Algorithm moves (normal form N, proved WLOG in proofs.txt): pick endpoint x in {u,v}, pick w reachable
// from x by a directed path all of whose vertices before w are saturated, w unsaturated; flip path (cost = length),
// orient new edge out of x. Net effect on charges: c(w) -= 1.
// usage: fast n [mode]   mode 0 = normal-form moves ; mode 1 = ALL valid orientations of merged tree (slow check)
#include <bits/stdc++.h>
using namespace std;
struct TI { int m; bool hasSat; vector<int> h; vector<int> nmin; vector<int8_t> c; vector<int> par; vector<int> reps; vector<vector<pair<int,int>>> opts; vector<int> cls; };
vector<TI> T; unordered_map<string,int> tid; vector<string> canonS;
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
    t.opts.push_back(o); int mn=99,cnt=0; for(auto&p:o){ if(p.second<mn){mn=p.second;cnt=0;} if(p.second==mn)cnt++; } t.h.push_back(mn); t.nmin.push_back(cnt); }
  t.hasSat=false; for(auto x:t.c) if(x<0)t.hasSat=true;
  int id=T.size(); T.push_back(t); tid[best]=id; canonS.push_back(best); return id; }
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

int ADV=0; int NOF=0; struct WM{int k,A,u,B,v;}; unordered_map<string,WM> winmv; // 0 = all pairs ; 2 = restricted A2 (singleton-singleton, 0-0 in nosat comps, sat-sat) ; 1 = both non-sink or both singletons
struct Bd{int8_t lo,hi;};
unordered_map<string,Bd> memo; long long nodes=0;
static string keyOf(const vector<int>&s){ return string((const char*)s.data(),s.size()*sizeof(int)); }
int F(const vector<int>&st){ int sat=0,ns=0,sg=0; for(int id:st){ if(T[id].hasSat)sat++; else if(T[id].m>=2)ns++; else sg++; } int f=sat+(ns+sg/2)/2-1; return f<0?0:f; }
bool can(const vector<int>&st,int k){
  if(k<=0) return true; if(st.size()<=1) return false;
  if(!NOF&&F(st)>=k) return true;
  string key=keyOf(st); auto it=memo.find(key); bool found=(it!=memo.end()); if(found){ if(k<=it->second.lo)return true; if(k>it->second.hi)return false; }
  nodes++;
  struct Mv{int i,j,cu,cv; long pri;}; vector<Mv> mv;
  for(size_t i=0;i<st.size();i++){ if(i&&st[i]==st[i-1])continue; for(size_t j=i;j<st.size();j++){ if(j==i){ if(i+1>=st.size()||st[i+1]!=st[i])continue; } else if(st[j]==st[j-1]&&j-1!=i) continue; 
      size_t jj=(j==i)?i+1:j; const TI&a=T[st[i]]; const TI&b=T[st[jj]];
      for(size_t cu=0;cu<a.reps.size();cu++)for(size_t cv=(st[i]==st[jj]?cu:0);cv<b.reps.size();cv++){ int ca=a.c[a.reps[cu]], cb=b.c[b.reps[cv]];
        bool ss=(a.m==1&&b.m==1); bool zz=(!a.hasSat&&!b.hasSat&&ca==0&&cb==0); bool tt=(ca<0&&cb<0);
        if((ADV==2||ADV==6)&&!(ss||zz||tt))continue; if(ADV==7&&a.m!=b.m)continue; if(ADV==7&&!(ss||zz||tt))continue; if(ADV==6&&!(a.m==b.m||(a.m%16==0&&b.m%16==0)))continue; if(ADV==3){ bool zs=((!a.hasSat&&ca==0&&cb<0)||(!b.hasSat&&cb==0&&ca<0)); if(!(ss||zz||tt||zs))continue; } if(ADV==4){ bool zs=((ca==0&&cb<0)||(cb==0&&ca<0)); if(!(ss||zz||tt||zs))continue; } if(ADV==1&&!(ss||(ca<=0&&cb<=0)))continue; if(ADV==5&&!(a.m==1||b.m==1||(ca<=0&&cb<=0)))continue;
        long p; int hm=min(a.h[cu],b.h[cv]);
        if(tt) p=-1000000L*hm + 1000L*(a.nmin[cu]+b.nmin[cv]) + abs(a.m-b.m)*10 + (a.m+b.m); else if(ss) p=-50000000; else if(zz) p=-40000000+(a.m+b.m); else p=1000000+ (ca+cb)*1000+(a.m+b.m);
        mv.push_back({(int)i,(int)jj,(int)cu,(int)cv,p}); } } }
  stable_sort(mv.begin(),mv.end(),[](const Mv&x,const Mv&y){return x.pri<y.pri;});
  bool win=false;
  for(auto&m:mv){ int A=st[m.i],B=st[m.j]; int u=T[A].reps[m.cu], v=T[B].reps[m.cv];
    vector<pair<int,int>> res;
    for(auto&o:T[A].opts[m.cu]) res.push_back({mergeT(A,u,B,v,0,o.first),o.second}); for(auto&o:T[B].opts[m.cv]) res.push_back({mergeT(A,u,B,v,1,o.first),o.second});
    sort(res.begin(),res.end(),[](auto&x,auto&y){return x.second<y.second|| (x.second==y.second&&x.first<y.first);});
    bool all=true; vector<int> base; for(size_t t=0;t<st.size();t++) if((int)t!=m.i&&(int)t!=m.j) base.push_back(st[t]);
    set<int> seen;
    for(auto&r:res){ if(!seen.insert(r.first).second) continue; if(r.second>=k) continue; vector<int> ns=base; ns.insert(lower_bound(ns.begin(),ns.end(),r.first),r.first);
      if(!can(ns,k-r.second)){all=false;break;} }
    if(all){win=true; if(NOF){ auto&w=winmv[key]; if(w.k<k){w={k,A,u,B,v};} } break;} }
  Bd&b=memo[key]; if(!found){b.lo=0;b.hi=127;}
  if(win){ if(b.lo<k)b.lo=k; } else { if(b.hi>k-1)b.hi=k-1; }
  return win; }

string rootedS(int id,int u){ const TI&t=T[id]; vector<vector<int>> a(t.m); for(int i=1;i<t.m;i++){a[i].push_back(t.par[i]);a[t.par[i]].push_back(i);} return enc(u,-1,a,t.c); }
set<pair<string,int>> emitted; FILE*cf; long lines=0;
void emit(const vector<int>&st,int k){
  if(k<=0) return; string key=keyOf(st); if(!emitted.insert({key,k}).second) return;
  auto it=winmv.find(key); if(it==winmv.end()||it->second.k<k){fprintf(stderr,"missing win move\n");exit(2);} WM w=it->second;
  static set<string> printed; if(printed.insert(key).second){ fprintf(cf,"%d |",w.k); for(int id:st) fprintf(cf," %s",canonS[id].c_str()); fprintf(cf," | %s %s | %s %s\n",canonS[w.A].c_str(),rootedS(w.A,w.u).c_str(),canonS[w.B].c_str(),rootedS(w.B,w.v).c_str()); lines++; }
  vector<int> base; bool ra=false,rb=false; for(int id:st){ if(!ra&&id==w.A){ra=true;continue;} if(!rb&&id==w.B){rb=true;continue;} base.push_back(id);} 
  int cu=T[w.A].cls[w.u], cv=T[w.B].cls[w.v]; vector<pair<int,int>> res;
  for(auto&o:T[w.A].opts[cu]) res.push_back({mergeT(w.A,w.u,w.B,w.v,0,o.first),o.second}); for(auto&o:T[w.B].opts[cv]) res.push_back({mergeT(w.A,w.u,w.B,w.v,1,o.first),o.second});
  for(auto&r:res){ if(r.second>=k)continue; vector<int> ns=base; ns.insert(lower_bound(ns.begin(),ns.end(),r.first),r.first); emit(ns,k-r.second); } }
int main(int argc,char**argv){ int n=atoi(argv[1]); int k=atoi(argv[2]); ADV=atoi(argv[3]); NOF=1;
  vector<vector<int>> a1(1); vector<int8_t> c1(1,1); int single=intern(a1,c1);
  vector<int> st(n,single); bool r=can(st,k); printf("n=%d adv=%d can(%d)=%d nodes=%lld states=%zu trees=%zu\n",n,ADV,k,(int)r,nodes,memo.size(),T.size());
  if(r){ cf=fopen(argv[4],"w"); fprintf(cf,"# certificate: adversary strategy forcing total cost >= %d on n=%d\n# line: k | trees of state | treeA rootedAtU | treeB rootedAtV   (digit of a vertex = 2 - outdegree)\n",k,n); emit(st,k); fclose(cf); printf("lines=%ld\n",lines);} }
