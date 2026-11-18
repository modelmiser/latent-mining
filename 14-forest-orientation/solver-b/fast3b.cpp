// fast3.cpp : memory-lean boolean solver. Same model as fast.cpp (charges, normal-form Algorithm moves),
// plus (i) cutoff by the proved lower bound F (Lemma 2 in proofs.txt), (ii) optional restricted Adversary (lower bounds only).
// usage: fast3 n0 n1 ADV extra    -> asks: can Adversary force >= B(n)+extra ?
// ADV: 0 all pairs (exact) ; 3 = singleton-singleton, 0-0 in sat-free comps, sat-sat, (0 in sat-free comp, sat) ;
//      7 = equal-size components only, among singleton-singleton / 0-0 sat-free / sat-sat ;
//      8 = 3 + (any 0, sat) + (singleton, sat) ; 9 = 8 + (singleton, any 0)
#include <bits/stdc++.h>
using namespace std;
struct TI { vector<short> b; short m; bool hasSat; short nr;
  int c(int i)const{return b[i];} int par(int i)const{return b[m+i];}
  int rep(int r)const{return b[2*m+r];} int h(int r)const{return b[2*m+nr+r];} int nmin(int r)const{return b[2*m+2*nr+r];}
  int off(int r)const{return b[2*m+3*nr+r];} int ow(int j)const{return b[2*m+4*nr+1+2*j];} int oc(int j)const{return b[2*m+4*nr+1+2*j+1];} };
deque<TI> T; unordered_map<string,int> tid;
static string enc(int x,int p,const vector<vector<int>>&adj,const vector<int8_t>&c){
  vector<string> ch; for(int y:adj[x]) if(y!=p) ch.push_back(enc(y,x,adj,c));
  sort(ch.begin(),ch.end()); string s="("; s+=char('1'+c[x]); for(auto&t:ch)s+=t; s+=")"; return s; }
int intern(const vector<vector<int>>&adj,const vector<int8_t>&c){
  int m=adj.size();
  vector<int> deg(m),lay; int rem=m; vector<char> gone(m,0);
  for(int i=0;i<m;i++){deg[i]=adj[i].size(); if(deg[i]<=1)lay.push_back(i);}
  while(rem>2){ vector<int> nx; for(int x:lay){gone[x]=1;rem--; for(int y:adj[x]) if(!gone[y]){ if(--deg[y]==1) nx.push_back(y);} } lay=nx; }
  string best; for(int r:lay){ string s=enc(r,-1,adj,c); if(best.empty()||s<best)best=s; }
  auto it=tid.find(best); if(it!=tid.end()) return it->second;
  vector<int8_t> tc(m); vector<int> par(m,-1);
  { int n=0; vector<int> st; for(size_t i=0;i<best.size();i++){ if(best[i]=='('){ int v=n++; par[v]=st.empty()?-1:st.back(); tc[v]=best[i+1]-'1'; st.push_back(v); i++; } else st.pop_back(); } }
  vector<vector<int>> a(m); for(int i=1;i<m;i++){a[i].push_back(par[i]);a[par[i]].push_back(i);}
  vector<int> s(m); for(int i=0;i<m;i++)s[i]=tc[i]; for(int i=m-1;i>=1;i--) s[par[i]]+=s[i];
  if(s[0]!=1){fprintf(stderr,"bad total\n");exit(1);} for(int i=1;i<m;i++) if(s[i]!=0&&s[i]!=1){fprintf(stderr,"bad cut\n");exit(1);}
  vector<vector<int>> out(m); for(int i=1;i<m;i++){ if(s[i]==0) out[i].push_back(par[i]); else out[par[i]].push_back(i);} 
  for(int i=0;i<m;i++) if((int)out[i].size()!=1-tc[i]){fprintf(stderr,"bad outdeg\n");exit(1);}
  map<string,int> cl; vector<int> reps;
  for(int i=0;i<m;i++){ string e=enc(i,-1,a,tc); if(!cl.count(e)){ cl[e]=1; reps.push_back(i);} }
  int nr=reps.size(); vector<short> hh,nm,offs,ops; int cnt=0;
  for(int u:reps){ vector<pair<int,int>> o; offs.push_back(cnt);
    if(tc[u]>=0) o.push_back({u,0}); else { vector<pair<int,int>> q; q.push_back({u,0}); for(size_t h=0;h<q.size();h++){ auto [x,d]=q[h]; for(int y:out[x]){ if(tc[y]>=0) o.push_back({y,d+1}); else q.push_back({y,d+1}); } } }
    int mn=99,c2=0; for(auto&p:o){ if(p.second<mn){mn=p.second;c2=0;} if(p.second==mn)c2++; ops.push_back(p.first); ops.push_back(p.second); cnt++; } hh.push_back(mn); nm.push_back(c2); }
   offs.push_back(cnt);
  TI t; t.m=m; t.nr=nr; t.hasSat=false; for(auto x:tc) if(x<0)t.hasSat=true;
  for(int i=0;i<m;i++)t.b.push_back(tc[i]); for(int i=0;i<m;i++)t.b.push_back(par[i]); for(int r:reps)t.b.push_back(r); for(auto*x:{&hh,&nm,&offs,&ops}) t.b.insert(t.b.end(),x->begin(),x->end()); t.b.shrink_to_fit();
  int id=T.size(); T.push_back(t); tid[best]=id; return id; }
int mergeT(int A,int u,int B,int v,int side,int w){
  const TI&a=T[A]; const TI&b=T[B]; int m=a.m+b.m; vector<vector<int>> adj(m); vector<int8_t> c(m);
  for(int i=0;i<a.m;i++){c[i]=a.c(i); if(i){adj[i].push_back(a.par(i));adj[a.par(i)].push_back(i);} }
  for(int i=0;i<b.m;i++){c[a.m+i]=b.c(i); if(i){adj[a.m+i].push_back(a.m+b.par(i));adj[a.m+b.par(i)].push_back(a.m+i);} }
  adj[u].push_back(a.m+v); adj[a.m+v].push_back(u); c[side? a.m+w:w]--;
  return intern(adj,c); }
int ADV=0;
struct Bd{int8_t lo,hi;};
unordered_map<string,Bd> memo; long long nodes=0;
static string keyOf(const vector<int>&s){ return string((const char*)s.data(),s.size()*sizeof(int)); }
int F(const vector<int>&st){ int sat=0,ns=0,sg=0; for(int id:st){ if(T[id].hasSat)sat++; else if(T[id].m>=2)ns++; else sg++; } int f=sat+(ns+sg/2)/2-1; return f<0?0:f; }
bool can(const vector<int>&st,int k){
  if(k<=0) return true; if(st.size()<=1) return false;
  if(F(st)>=k) return true;
  string key=keyOf(st); { auto it=memo.find(key); if(it!=memo.end()){ if(k<=it->second.lo)return true; if(k>it->second.hi)return false; } }
  nodes++;
  struct Mv{int i,j,cu,cv; long pri;}; vector<Mv> mv;
  for(size_t i=0;i<st.size();i++){ if(i&&st[i]==st[i-1])continue; for(size_t j=i;j<st.size();j++){ if(j==i){ if(i+1>=st.size()||st[i+1]!=st[i])continue; } else if(st[j]==st[j-1]&&j-1!=i) continue; 
      size_t jj=(j==i)?i+1:j; const TI&a=T[st[i]]; const TI&b=T[st[jj]];
      for(int cu=0;cu<a.nr;cu++)for(int cv=(st[i]==st[jj]?cu:0);cv<b.nr;cv++){ int ca=a.c(a.rep(cu)), cb=b.c(b.rep(cv));
        bool ss=(a.m==1&&b.m==1); bool zz=(!a.hasSat&&!b.hasSat&&ca==0&&cb==0); bool tt=(ca<0&&cb<0);
        if(ADV==7){ if(a.m!=b.m||!(ss||zz||tt))continue; } else if(ADV){ bool zs3=((!a.hasSat&&ca==0&&cb<0)||(!b.hasSat&&cb==0&&ca<0)); bool zs=((ca==0&&cb<0)||(cb==0&&ca<0)); bool gs=((a.m==1&&cb<0)||(b.m==1&&ca<0)); bool gz=((a.m==1&&cb==0)||(b.m==1&&ca==0));
          bool okm=ss||zz||tt||zs3; if(ADV>=8) okm=okm||zs||gs; if(ADV>=9) okm=okm||gz; if(!okm)continue; }
        long p; int hm=min(a.h(cu),b.h(cv));
        if(tt) p=-1000000L*hm + 1000L*(a.nmin(cu)+b.nmin(cv)) + abs(a.m-b.m)*10 + (a.m+b.m); else if(ss) p=-50000000; else if(zz) p=-40000000+(a.m+b.m); else p=1000000+ (ca+cb)*1000+(a.m+b.m);
        mv.push_back({(int)i,(int)jj,cu,cv,p}); } } }
  stable_sort(mv.begin(),mv.end(),[](const Mv&x,const Mv&y){return x.pri<y.pri;});
  bool win=false;
  for(auto&m:mv){ int A=st[m.i],B=st[m.j]; int u=T[A].rep(m.cu), v=T[B].rep(m.cv);
    vector<int> base; for(size_t t=0;t<st.size();t++) if((int)t!=m.i&&(int)t!=m.j) base.push_back(st[t]);
    vector<array<int,4>> op; // (cost, creates-sat, side, w)
    { const TI&a=T[A]; for(int j=a.off(m.cu);j<a.off(m.cu+1);j++){ int w=a.ow(j), c=a.oc(j); if(c>=k)continue; op.push_back({c,a.c(w)==0?1:0,0,w}); } }
    { const TI&bb=T[B]; for(int j=bb.off(m.cv);j<bb.off(m.cv+1);j++){ int w=bb.ow(j), c=bb.oc(j); if(c>=k)continue; op.push_back({c,bb.c(w)==0?1:0,1,w}); } }
    sort(op.begin(),op.end());
    bool all=true; vector<int> done;
    for(auto&o:op){ int id=mergeT(A,u,B,v,o[2],o[3]); if(find(done.begin(),done.end(),id)!=done.end())continue; done.push_back(id);
      vector<int> ns=base; ns.insert(lower_bound(ns.begin(),ns.end(),id),id);
      if(!can(ns,k-o[0])){all=false;break;} }
    if(all){win=true;break;} }
  auto ins=memo.emplace(key,Bd{0,127}); Bd&b=ins.first->second;
  if(win){ if(b.lo<k)b.lo=k; } else { if(b.hi>k-1)b.hi=k-1; }
  return win; }
int main(int argc,char**argv){ int n0=atoi(argv[1]); int n1=atoi(argv[2]); ADV=atoi(argv[3]); int extra=argc>4?atoi(argv[4]):1;
  vector<vector<int>> a1(1); vector<int8_t> c1(1,1); int single=intern(a1,c1);
  for(int n=n0;n<=n1;n++){ memo.clear(); nodes=0; vector<int> st(n,single); int B=max(0,n/4-1); bool r=can(st,B+extra);
    printf("n=%d adv=%d can(B+%d=%d)=%d nodes=%lld states=%zu trees=%zu\n",n,ADV,extra,B+extra,(int)r,nodes,memo.size(),T.size()); fflush(stdout);} }
