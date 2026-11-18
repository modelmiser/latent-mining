// fast.cpp : exact game value T(n) using charge representation + normal-form Algorithm moves.
// charge c(v) = 1 - outdeg(v) in {+1,0,-1}; orientation of a tree is determined by charges.
// Algorithm moves (normal form N, proved WLOG in proofs.txt): pick endpoint x in {u,v}, pick w reachable
// from x by a directed path all of whose vertices before w are saturated, w unsaturated; flip path (cost = length),
// orient new edge out of x. Net effect on charges: c(w) -= 1.
// usage: fast n [mode]   mode 0 = normal-form moves ; mode 1 = ALL valid orientations of merged tree (slow check)
#include <bits/stdc++.h>
using namespace std;
struct TI { int m; vector<int8_t> c; vector<int> par; vector<int> reps; vector<vector<pair<int,int>>> opts; vector<int> cls; };
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
    t.opts.push_back(o); }
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

unordered_map<string,int> memo;
static string keyOf(const vector<int>&s){ return string((const char*)s.data(),s.size()*sizeof(int)); }
int V(const vector<int>&st){
  if(st.size()<=1) return 0;
  string key=keyOf(st); auto it=memo.find(key); if(it!=memo.end()) return it->second;
  int best=0;
  for(size_t i=0;i<st.size();i++){ if(i&&st[i]==st[i-1])continue; for(size_t j=i;j<st.size();j++){ if(j==i){ if(i+1>=st.size()||st[i+1]!=st[i])continue; } else if(st[j]==st[j-1]&&j-1!=i) continue;
      size_t jj=(j==i)?i+1:j; int A=st[i],B=st[jj];
      vector<int> base; for(size_t t=0;t<st.size();t++) if(t!=i&&t!=jj) base.push_back(st[t]);
      for(size_t cu=0;cu<T[A].reps.size();cu++)for(size_t cv=(A==B?cu:0);cv<T[B].reps.size();cv++){
        int u=T[A].reps[cu], v=T[B].reps[cv]; vector<pair<int,int>> res;
        for(auto&o:T[A].opts[cu]) res.push_back({mergeT(A,u,B,v,0,o.first),o.second}); for(auto&o:T[B].opts[cv]) res.push_back({mergeT(A,u,B,v,1,o.first),o.second});
        int mn=1000; for(auto&r:res){ vector<int> ns=base; ns.insert(lower_bound(ns.begin(),ns.end(),r.first),r.first); int x=r.second+V(ns); if(x<mn)mn=x; }
        if(mn>best)best=mn; } } }
  memo[key]=best; return best; }
int F(const vector<int>&st){ int sat=0,ns=0,sg=0; for(int id:st){ bool hs=false; for(auto c:T[id].c) if(c<0)hs=true; if(hs)sat++; else if(T[id].m>=2)ns++; else sg++; } int f=sat+(ns+sg/2)/2-1; return f<0?0:f; }
int main(int argc,char**argv){ int n=atoi(argv[1]); int show=argc>2?atoi(argv[2]):20;
  vector<vector<int>> a1(1); vector<int8_t> c1(1,1); int single=intern(a1,c1);
  vector<int> st(n,single); printf("n=%d T=%d states=%zu\n",n,V(st),memo.size());
  map<int,int> hist; int shown=0; vector<pair<int,string>> ex;
  for(auto&kv:memo){ vector<int> s(kv.first.size()/4); memcpy(s.data(),kv.first.data(),kv.first.size()); int d=kv.second-F(s); hist[d]++; if(d!=0){ string e; for(int id:s){e+=canonS[id];e+=" ";} ex.push_back({d,e+" V="+to_string(kv.second)+" F="+to_string(F(s))}); } }
  for(auto&h:hist) printf("V-F=%d : %d states\n",h.first,h.second);
  sort(ex.begin(),ex.end(),[](auto&a,auto&b){return a.first!=b.first? a.first>b.first : a.second.size()<b.second.size();});
  for(auto&e:ex){ if(shown++>=show)break; printf("%d  %s\n",e.first,e.second.c_str()); }
  shown=0; for(auto it=ex.rbegin();it!=ex.rend();++it){ if(shown++>=show)break; printf("%d  %s\n",it->first,it->second.c_str()); }
}
