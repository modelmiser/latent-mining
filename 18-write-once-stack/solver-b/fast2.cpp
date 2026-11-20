// fast2.cpp : as fast.cpp, plus exact sub-game bounds M(x) (min free cells needed to host situation x alone), bootstrapped by remaining budget.
// fast.cpp : exhaustive DFS for existence of a top-scheme on n cells surviving t ops.
// usage: fast n t mode(L|S) [outfile] [nosym]
// mode L = literal (nothing checked before first op: Q(0^n) free), S = strict (Q(0^n)=EMPTY required)
#include <bits/stdc++.h>
using namespace std;
typedef bitset<512> BS;
int n,t; bool modeL; bool usesym=true;
struct Sit{string st;int s;};
vector<Sit> sits; map<pair<string,int>,int> sid;
vector<array<int,3>> nxt; vector<int> topv; // 0,1 bits; 2 EMPTY; 3 WILD
int NS;
vector<vector<signed char>> cmemo;
int topOf(const string&s){return s.empty()?2:(s.back()-'0');}
bool compat(int a,int b){
  if(a==b) return true; if(a>b) swap(a,b);
  signed char &m=cmemo[a][b]; if(m>=0) return m;
  bool r=true;
  if(topv[a]!=3&&topv[b]!=3&&topv[a]!=topv[b]) r=false;
  for(int op=0;op<3&&r;op++){int x=nxt[a][op],y=nxt[b][op]; if(x>=0&&y>=0&&!compat(x,y)) r=false;}
  return m=r;
}
vector<BS> cmask; vector<int> chainv;
int N; vector<BS> S; vector<int> Q; vector<array<int,3>> E;
struct Tr{int kind,a,b;}; vector<Tr> trail;
long long nodes=0;
bool addsit(int x,int c){
  if(S[c][x]) return true;
  if(chainv[x]>n-__builtin_popcount(c)) return false;
  if(topv[x]!=3){ if(Q[c]<0){Q[c]=topv[x];trail.push_back({0,c,0});} else if(Q[c]!=topv[x]) return false; }
  if((S[c]&~cmask[x]).any()) return false;
  S[c][x]=1; trail.push_back({1,c,x});
  for(int op=0;op<3;op++){int y=nxt[x][op]; if(y<0) continue; if(E[c][op]>=0){ if(!addsit(y,E[c][op])) return false; }}
  return true;
}
bool assign(int c,int op,int c2){
  E[c][op]=c2; trail.push_back({2,c,op});
  BS snap=S[c];
  for(int x=snap._Find_first();x<512;x=snap._Find_next(x)){int y=nxt[x][op]; if(y>=0 && !addsit(y,c2)) return false;}
  return true;
}
void undo(size_t mark){
  while(trail.size()>mark){Tr r=trail.back();trail.pop_back();
    if(r.kind==0)Q[r.a]=-1; else if(r.kind==1)S[r.a][r.b]=0; else E[r.a][r.b]=-1;}
}
bool dfs(){
  nodes++;
  int used=0; for(int c=0;c<N;c++) if(S[c].any()) used|=c;
  // allowed fresh mask: fresh cells must be chosen as lowest-index prefix
  int best=INT_MAX,bc=-1,bop=-1; vector<int> bestopts;
  vector<int> opts;
  for(int c=0;c<N;c++){ if(S[c].none()) continue;
    for(int op=0;op<3;op++){ if(E[c][op]>=0) continue;
      BS Y; int tp=-1; bool bad=false; int need=0;
      for(int x=S[c]._Find_first();x<512;x=S[c]._Find_next(x)){int y=nxt[x][op]; if(y<0) continue; Y[y]=1;
        if(topv[y]!=3){ if(tp<0)tp=topv[y]; else if(tp!=topv[y]) bad=true; } need=max(need,chainv[y]);}
      if(Y.none()) continue;
      if(bad) return false;
      BS ok; ok.set(); for(int y=Y._Find_first();y<512;y=Y._Find_next(y)) ok&=cmask[y];
      if((Y&~ok).any()) return false;
      opts.clear();
      int rest=(N-1)&~c;
      for(int a=rest;;a=(a-1)&rest){
        int c2=c|a;
        bool good=true;
        if(usesym){ int fr=a&~used; // must be prefix of fresh cells
          int allfresh=(N-1)&~used; // prefix: fr == lowest k bits of allfresh
          if(fr){ int low=0,rem=allfresh; int k=__builtin_popcount(fr); for(int i=0;i<k;i++){low|=rem&-rem; rem&=rem-1;} if(low!=fr) good=false; } }
        if(good && need>n-__builtin_popcount(c2)) good=false;
        if(good && tp>=0 && Q[c2]>=0 && Q[c2]!=tp) good=false;
        if(good && (S[c2]&~ok).any()) good=false;
        if(good){opts.push_back(c2); if((int)opts.size()>=best) break;}
        if(a==0) break;
      }
      if(opts.empty()) return false;
      if((int)opts.size()<best){best=opts.size();bc=c;bop=op;bestopts=opts;}
    }}
  if(bc<0) return true;
  // try smaller-weight options first
  sort(bestopts.begin(),bestopts.end(),[](int a,int b){int pa=__builtin_popcount(a),pb=__builtin_popcount(b); return pa!=pb?pa<pb:a<b;});
  for(int c2:bestopts){ size_t mark=trail.size();
    if(assign(bc,bop,c2) && dfs()) return true;
    undo(mark);}
  return false;
}
int main(int argc,char**argv){
  n=atoi(argv[1]);t=atoi(argv[2]);modeL=(argv[3][0]=='L'); const char*out=argc>4?argv[4]:nullptr;
  if(argc>5) usesym=false;
  N=1<<n;
  // enumerate situations
  sits.push_back({"",0}); sid[{"",0}]=0;
  for(size_t i=0;i<sits.size();i++){ Sit a=sits[i]; array<int,3> nx={-1,-1,-1};
    if(a.s<t) for(int op=0;op<3;op++){ string s2=a.st; if(op<2) s2.push_back('0'+op); else { if(s2.empty()) continue; s2.pop_back(); }
      auto key=make_pair(s2,a.s+1); auto it=sid.find(key); int id; if(it==sid.end()){id=sits.size();sid[key]=id;sits.push_back({s2,a.s+1});} else id=it->second; nx[op]=id; }
    nxt.push_back(nx); }
  NS=sits.size(); if(NS>512){fprintf(stderr,"too many sits\n");return 2;}
  topv.resize(NS); for(int i=0;i<NS;i++) topv[i]=topOf(sits[i].st); if(modeL) topv[0]=3;
  cmemo.assign(NS,vector<signed char>(NS,-1));
  cmask.assign(NS,BS()); for(int a=0;a<NS;a++)for(int b=0;b<NS;b++) if(compat(a,b)) cmask[a][b]=1;
  chainv.assign(NS,0); for(int a=NS-1;a>=0;a--){int m=0; for(int op=0;op<3;op++){int y=nxt[a][op]; if(y<0)continue; m=max(m,(compat(a,y)?0:1)+chainv[y]);} chainv[a]=m;}
  // bootstrap exact M(x) by decreasing step (increasing remaining budget); key = (window top r+1 entries with bottom mark, r)
  map<pair<string,int>,int> Mmemo; int nmain=n; long long subnodes=0;
  for(int a=NS-1;a>=1;a--){ int r=t-sits[a].s; string w=sits[a].st; if((int)w.size()>r+1) w="*"+w.substr(w.size()-(r+1));
    auto key=make_pair(w,r); auto it=Mmemo.find(key); if(it!=Mmemo.end()){chainv[a]=it->second;continue;}
    int m=chainv[a]; for(int op=0;op<3;op++){int y=nxt[a][op]; if(y<0)continue; m=max(m,(compat(a,y)?0:1)+chainv[y]);}
    for(;;m++){ if(m>nmain){break;} n=m;N=1<<n; S.assign(N,BS()); Q.assign(N,-1); E.assign(N,array<int,3>{-1,-1,-1}); trail.clear();
      int save=chainv[a]; chainv[a]=0; bool ok=addsit(a,0)&&dfs(); chainv[a]=save; if(ok)break; }
    chainv[a]=m; Mmemo[key]=m; if(getenv("VERBOSE")) fprintf(stderr,"M(%s,r=%d)=%d\n",w.c_str(),r,m);
  }
  subnodes=nodes; nodes=0; n=nmain; N=1<<n; trail.clear();
  S.assign(N,BS()); Q.assign(N,-1); E.assign(N,array<int,3>{-1,-1,-1});
  bool ok=addsit(0,0)&&dfs();
  printf("subnodes=%lld ",subnodes);
  printf("n=%d t=%d mode=%c sits=%d result=%s nodes=%lld\n",n,t,modeL?'L':'S',NS,ok?"SAT":"UNSAT",nodes);
  if(ok&&out){ FILE*f=fopen(out,"w"); fprintf(f,"# top-scheme witness: n=%d t=%d mode=%c. Lines: Q <config> <EMPTY|0|1> ; E <config> <PUSH0|PUSH1|POP> <config>. Config = n chars, cell 1 first. Unlisted E = identity, unlisted Q = EMPTY.\n",n,t,modeL?'L':'S');
    auto cs=[&](int c){string s; for(int i=0;i<n;i++) s.push_back((c>>i&1)?'1':'0'); return s;};
    const char*qn[3]={"0","1","EMPTY"}; const char*on[3]={"PUSH0","PUSH1","POP"};
    fprintf(f,"n %d\nt %d\n",n,t);
    for(int c=0;c<N;c++) if(Q[c]>=0) fprintf(f,"Q %s %s\n",cs(c).c_str(),qn[Q[c]]);
    for(int c=0;c<N;c++) for(int op=0;op<3;op++) if(E[c][op]>=0) fprintf(f,"E %s %s %s\n",cs(c).c_str(),on[op],cs(E[c][op]).c_str());
    fclose(f);}
  return ok?0:1;
}
