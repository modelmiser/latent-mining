// sat.cpp : SAT encoding + self-contained CDCL solver for the top-scheme problem (independent of fast.cpp's DFS).
// usage: sat m r mode(L|S) init(- for empty) outfile(- none) [Mtable|-] [nosym]
// Game: m cells, initial stack = init (bottom is real), r operations, mode S: Q(start config)=top(init) required; L: not checked.
// Mtable (optional): lines "<stack|-> <r> <M>" = exact min #free cells to host a non-root situation whose top-(r+1) window is <stack>.
#include <bits/stdc++.h>
using namespace std;
// ---------------- CDCL ----------------
struct Solver{
  int nv=0; vector<vector<int>> cl; vector<vector<int>> wat; vector<signed char> val; vector<int> lev,rea; vector<int> trail,tlim; size_t qhead=0;
  vector<double> act; double inc=1; vector<char> phase,seen; priority_queue<pair<double,int>> pq; bool ok=true; long long conflicts=0,decisions=0,props=0; size_t norig=0;
  int newVar(){val.push_back(0);lev.push_back(0);rea.push_back(-1);act.push_back(0);phase.push_back(0);seen.push_back(0);wat.emplace_back();wat.emplace_back();pq.push({0,nv});return nv++;}
  inline int value(int l){int v=val[l>>1]; return (l&1)?-v:v;}
  void enqueue(int l,int r){val[l>>1]=(l&1)?-1:1;lev[l>>1]=tlim.size();rea[l>>1]=r;trail.push_back(l);}
  void addClause(vector<int> c){ if(!ok)return; sort(c.begin(),c.end()); c.erase(unique(c.begin(),c.end()),c.end());
    for(size_t i=0;i+1<c.size();i++) if((c[i]^1)==c[i+1]) return; // tautology
    // level-0 simplification
    vector<int> d; for(int l:c){int v=value(l); if(v>0)return; if(v==0)d.push_back(l);} 
    if(d.empty()){ok=false;return;} if(d.size()==1){enqueue(d[0],-1); if(propagate()>=0) ok=false; return;}
    wat[d[0]].push_back(cl.size()); wat[d[1]].push_back(cl.size()); cl.push_back(d); }
  int propagate(){
    while(qhead<trail.size()){ int p=trail[qhead++]; int fl=p^1; props++; vector<int>&ws=wat[fl]; size_t i=0,j=0;
      while(i<ws.size()){ int ci=ws[i++]; vector<int>&c=cl[ci]; if(c[0]==fl) swap(c[0],c[1]);
        if(value(c[0])>0){ws[j++]=ci;continue;}
        bool found=false; for(size_t k=2;k<c.size();k++) if(value(c[k])>=0){swap(c[1],c[k]);wat[c[1]].push_back(ci);found=true;break;}
        if(found) continue;
        ws[j++]=ci;
        if(value(c[0])<0){ while(i<ws.size()) ws[j++]=ws[i++]; ws.resize(j); qhead=trail.size(); return ci; }
        enqueue(c[0],ci); }
      ws.resize(j); }
    return -1; }
  void bump(int v){ act[v]+=inc; if(act[v]>1e100){ for(auto&a:act)a*=1e-100; inc*=1e-100; priority_queue<pair<double,int>> q; for(int u=0;u<nv;u++) if(!val[u]) q.push({act[u],u}); pq.swap(q);} else if(!val[v]) pq.push({act[v],v}); }
  void cancelUntil(int L){ if((int)tlim.size()<=L) return; for(int i=trail.size()-1;i>=tlim[L];i--){int v=trail[i]>>1; phase[v]=!(trail[i]&1); val[v]=0; pq.push({act[v],v});} trail.resize(tlim[L]); tlim.resize(L); qhead=trail.size(); }
  void analyze(int confl,vector<int>&out,int&bt){ out.clear(); out.push_back(0); int cnt=0,p=-1; int idx=trail.size()-1; int curL=tlim.size();
    do{ vector<int>&c=cl[confl]; for(size_t k=(p==-1?0:1);k<c.size();k++){ int q=c[k]; int v=q>>1; if(!seen[v]&&lev[v]>0){ seen[v]=1; bump(v); if(lev[v]>=curL) cnt++; else out.push_back(q);} }
      while(!seen[trail[idx]>>1]) idx--; p=trail[idx]; idx--; confl=rea[p>>1]; seen[p>>1]=0; cnt--;
      if(cnt>0){ // ensure reason clause has p at position 0
        vector<int>&r=cl[confl]; if(r[0]!=p){ for(size_t k=1;k<r.size();k++) if(r[k]==p){swap(r[0],r[k]);break;} } }
    }while(cnt>0);
    out[0]=p^1; bt=0; int mi=1; for(size_t k=1;k<out.size();k++){ if(lev[out[k]>>1]>bt){bt=lev[out[k]>>1];mi=k;} }
    if(out.size()>1) swap(out[1],out[mi]);
    for(size_t k=1;k<out.size();k++) seen[out[k]>>1]=0; }
  void reduceDB(){ // at level 0: drop satisfied clauses, strip false literals, drop the longer half of learnts
    vector<int> lens; for(size_t i=norig;i<cl.size();i++) if(!cl[i].empty()) lens.push_back(cl[i].size()); int cut=INT_MAX;
    if(!lens.empty()){ nth_element(lens.begin(),lens.begin()+lens.size()/2,lens.end()); cut=max(3,lens[lens.size()/2]); }
    vector<vector<int>> ncl; size_t nno=0;
    for(size_t i=0;i<cl.size();i++){ vector<int>&c=cl[i]; if(c.empty())continue; bool sat=false; vector<int> d; for(int l:c){int v=value(l); if(v>0){sat=true;break;} if(v==0)d.push_back(l);} if(sat)continue;
      if(i>=norig && (int)d.size()>cut) continue; ncl.push_back(d); if(i<norig) nno=ncl.size(); }
    cl.swap(ncl); norig=nno; for(auto&w:wat)w.clear(); for(size_t i=0;i<cl.size();i++){wat[cl[i][0]].push_back(i);wat[cl[i][1]].push_back(i);} for(int v=0;v<nv;v++) rea[v]=-1; }
  static long long luby(long long i){ long long k=1; while((1LL<<k)-1<i)k++; while((1LL<<k)-1!=i){ i-=(1LL<<(k-1))-1; k=1; while((1LL<<k)-1<i)k++; } return 1LL<<(k-1); }
  // returns 1 sat, 0 unsat
  int solve(){ if(!ok) return 0; if(propagate()>=0) return 0; norig=cl.size(); long long rs=1; size_t maxlearn=max<size_t>(20000,cl.size()/3); vector<int> learnt;
    for(;;){ long long budget=luby(rs++)*200; long long c0=conflicts;
      for(;;){ int confl=propagate();
        if(confl>=0){ conflicts++; if(tlim.empty()) return 0; int bt; analyze(confl,learnt,bt); cancelUntil(bt);
          if(learnt.size()==1){ enqueue(learnt[0],-1);} else { wat[learnt[0]].push_back(cl.size()); wat[learnt[1]].push_back(cl.size()); cl.push_back(learnt); enqueue(learnt[0],cl.size()-1);} inc/=0.95; }
        else { if(conflicts-c0>=budget){ cancelUntil(0); break; }
          int v=-1; while(!pq.empty()){ int u=pq.top().second; pq.pop(); if(!val[u]){v=u;break;} }
          if(v<0){ for(int u=0;u<nv;u++) if(!val[u]){v=u;break;} if(v<0) return 1; }
          decisions++; tlim.push_back(trail.size()); enqueue(2*v+(phase[v]?0:1),-1); } }
      if(propagate()>=0) return 0;
      if(cl.size()-norig>maxlearn){ reduceDB(); maxlearn+=maxlearn/10; } } }
};
// ---------------- problem ----------------
int n,t; bool modeL;
struct Sit{string st;int s;}; vector<Sit> sits; map<pair<string,int>,int> sid; vector<array<int,3>> nxt; vector<int> topv; int NS;
vector<vector<signed char>> cmemo;
bool compat(int a,int b){ if(a==b)return true; if(a>b)swap(a,b); signed char&m=cmemo[a][b]; if(m>=0)return m; bool r=true;
  if(topv[a]!=3&&topv[b]!=3&&topv[a]!=topv[b]) r=false;
  for(int op=0;op<3&&r;op++){int x=nxt[a][op],y=nxt[b][op]; if(x>=0&&y>=0&&!compat(x,y)) r=false;} return m=r; }
int main(int argc,char**argv){
  n=atoi(argv[1]);t=atoi(argv[2]);modeL=argv[3][0]=='L'; string init=argv[4]; if(init=="-")init=""; const char*out=(argc>5&&string(argv[5])!="-")?argv[5]:nullptr;
  const char*mt=(argc>6&&string(argv[6])!="-")?argv[6]:nullptr; bool sym=!(argc>7);
  int N=1<<n;
  sits.push_back({init,0}); sid[{init,0}]=0;
  for(size_t i=0;i<sits.size();i++){ Sit a=sits[i]; array<int,3> nx={-1,-1,-1};
    if(a.s<t) for(int op=0;op<3;op++){ string s2=a.st; if(op<2) s2.push_back('0'+op); else { if(s2.empty()) continue; s2.pop_back(); }
      auto key=make_pair(s2,a.s+1); auto it=sid.find(key); int id; if(it==sid.end()){id=sits.size();sid[key]=id;sits.push_back({s2,a.s+1});} else id=it->second; nx[op]=id; }
    nxt.push_back(nx); }
  NS=sits.size(); topv.resize(NS); for(int i=0;i<NS;i++) topv[i]=sits[i].st.empty()?2:sits[i].st.back()-'0'; if(modeL) topv[0]=3;
  cmemo.assign(NS,vector<signed char>(NS,-1));
  vector<int> lb(NS,0); for(int a=NS-1;a>=0;a--){int m=0; for(int op=0;op<3;op++){int y=nxt[a][op]; if(y<0)continue; m=max(m,(compat(a,y)?0:1)+lb[y]);} lb[a]=m;}
  if(mt){ map<pair<string,int>,int> M; ifstream f(mt); string w; int r,m; while(f>>w>>r>>m){ if(w=="-")w=""; M[{w,r}]=m; }
    for(int a=1;a<NS;a++){ int r=t-sits[a].s; string w=sits[a].st; if((int)w.size()>r+1) w=w.substr(w.size()-(r+1)); auto it=M.find({w,r}); if(it!=M.end()) lb[a]=max(lb[a],it->second); } }
  Solver S;
  // variables
  vector<vector<int>> H(N,vector<int>(NS,-1));
  for(int c=0;c<N;c++){int z=n-__builtin_popcount(c); for(int x=0;x<NS;x++) if(z>=lb[x]) H[c][x]=S.newVar();}
  vector<array<int,3>> Qv(N); for(int c=0;c<N;c++) for(int v=0;v<3;v++) Qv[c][v]=S.newVar();
  // window vars for lengths 2..4 : alphabet {0,1,B}; window string of top j entries padded with B
  const int JMAX=4; vector<map<string,int>> W(N);
  auto win=[&](int x,int j){ string s; const string&st=sits[x].st; for(int i=0;i<j;i++){ int p=(int)st.size()-1-i; s.push_back(p>=0?st[p]:'B'); } return s; };
  auto P=[](int v){return 2*v;}; auto Ng=[](int v){return 2*v+1;};
  if(H[0][0]<0){ printf("m=%d r=%d mode=%c init=%s result=UNSAT (root bound)\n",n,t,argv[3][0],init.c_str()); return 1; }
  S.addClause({P(H[0][0])});
  for(int c=0;c<N;c++){ for(int a=0;a<3;a++)for(int b=a+1;b<3;b++) S.addClause({Ng(Qv[c][a]),Ng(Qv[c][b])});
    for(int x=0;x<NS;x++){ if(H[c][x]<0) continue; if(topv[x]!=3) S.addClause({Ng(H[c][x]),P(Qv[c][topv[x]])});
      if(topv[x]==3) continue; int r=t-sits[x].s;
      for(int j=2;j<=JMAX&&j<=r+1;j++){ string key=to_string(j)+":"+win(x,j); auto it=W[c].find(key); int wv; if(it==W[c].end()){wv=S.newVar();W[c][key]=wv;} else wv=it->second; S.addClause({Ng(H[c][x]),P(wv)}); } }
    for(auto&a:W[c]) for(auto&b:W[c]) if(a.first<b.first && a.first[0]==b.first[0]) S.addClause({Ng(a.second),Ng(b.second)}); }
  // E vars + transitions
  vector<array<vector<pair<int,int>>,3>> Ev(N);
  auto isprefix=[&](int mask,int within){ // mask subset of within; must be the lowest popcount(mask) bits of within
    int low=0,rem=within; for(int i=__builtin_popcount(mask);i>0;i--){low|=rem&-rem;rem&=rem-1;} return low==mask; };
  int c0mask=-1;
  for(int c=0;c<N;c++) for(int op=0;op<3;op++){ bool need=false; for(int x=0;x<NS;x++) if(H[c][x]>=0&&nxt[x][op]>=0) need=true; if(!need) continue;
    int rest=(N-1)&~c; vector<int> alo;
    for(int a=rest;;a=(a-1)&rest){ int c2=c|a; bool allowed=true;
      if(sym && c==0 && op==0 && !isprefix(a,N-1)) allowed=false;
      if(allowed){ int e=S.newVar(); Ev[c][op].push_back({c2,e}); alo.push_back(P(e));
        for(int x=0;x<NS;x++){ if(H[c][x]<0) continue; int y=nxt[x][op]; if(y<0) continue; if(H[c2][y]>=0) S.addClause({Ng(H[c][x]),Ng(e),P(H[c2][y])}); else S.addClause({Ng(H[c][x]),Ng(e)}); } }
      if(a==0) break; }
    S.addClause(alo); }
  if(sym){ // second-level symmetry breaking: given E(0,PUSH0)=prefix P_k, E(0,PUSH1) must meet P_k in a prefix of P_k and the rest in a prefix of the rest
    for(auto&pe:Ev[0][0]){ int pk=pe.first; for(auto&qe:Ev[0][1]){ int c1=qe.first; if(!isprefix(c1&pk,pk)||!isprefix(c1&~pk,(N-1)&~pk)) S.addClause({Ng(pe.second),Ng(qe.second)}); } } }
  size_t ncl=S.cl.size(); int r=S.solve();
  printf("m=%d r=%d mode=%c init=%s sits=%d vars=%d clauses=%zu result=%s conflicts=%lld decisions=%lld\n",n,t,argv[3][0],init.empty()?"-":init.c_str(),NS,S.nv,ncl,r?"SAT":"UNSAT",S.conflicts,S.decisions);
  if(r&&out){ FILE*f=fopen(out,"w"); fprintf(f,"# top-scheme witness (sat.cpp): n=%d t=%d mode=%c init=%s. Q <config> <EMPTY|0|1>; E <config> <op> <config>; config = n chars, cell 1 first. Unlisted E = identity, unlisted Q = EMPTY.\n",n,t,argv[3][0],init.empty()?"-":init.c_str());
    fprintf(f,"n %d\nt %d\ninit %s\n",n,t,init.empty()?"-":init.c_str());
    auto cs=[&](int c){string s; for(int i=0;i<n;i++) s.push_back((c>>i&1)?'1':'0'); return s;}; const char*qn[3]={"0","1","EMPTY"}; const char*on[3]={"PUSH0","PUSH1","POP"};
    // reachable closure using first true E
    vector<char> reach(N,0); vector<int> stk={0}; reach[0]=1; vector<array<int,3>> Ech(N,array<int,3>{-1,-1,-1});
    while(!stk.empty()){ int c=stk.back(); stk.pop_back(); for(int op=0;op<3;op++){ for(auto&pe:Ev[c][op]) if(S.val[pe.second]>0){ Ech[c][op]=pe.first; if(!reach[pe.first]){reach[pe.first]=1;stk.push_back(pe.first);} break; } } }
    for(int c=0;c<N;c++) if(reach[c]) for(int v=0;v<3;v++) if(S.val[Qv[c][v]]>0) fprintf(f,"Q %s %s\n",cs(c).c_str(),qn[v]);
    for(int c=0;c<N;c++) if(reach[c]) for(int op=0;op<3;op++) if(Ech[c][op]>=0) fprintf(f,"E %s %s %s\n",cs(c).c_str(),on[op],cs(Ech[c][op]).c_str());
    fclose(f); }
  return r?0:1;
}
