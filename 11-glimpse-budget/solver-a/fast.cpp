// Belief-set solver for the glimpse cop/robber game on the complete binary tree T_h.
// usage: fast G h k [dump]      G in {E,D}
//  State at the start of a round: (j glimpses left, cop vertex c, set S of possible robber vertices).
//  Blind round:  cop -> c' in N[c];  S1 = S \ {c'};  win if S1 empty;  S' = N[S1] \ {c'}.
//  Glimpse round (j>0): adversary picks answer A (E: a singleton {v}, v in S; D: S cap branch(c,u) nonempty),
//                cop picks c' depending on A; A1 = A\{c'}; win if empty; next state (j-1, c', N[A1]\{c'}).
//  Win_j(c,S) = some blind walk reaches a win or a state where glimpsing wins against every answer.
// Vertices are heap indices 1..n, bit v of the mask.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 B;
static inline B bit(int v){return (B)1<<v;}
int h,n; char G;
vector<int> nbc[128];      // closed neighbourhood list: self first, then neighbours
B cl[128];                 // closed neighbourhood mask
B branchm[128][4];         // branchm[c][i]: component of T-c containing nbc[c][i] (i>=1)
B ALL;
static inline B expandS(B S,int c){
  B r=0; uint64_t lo=(uint64_t)S, hi=(uint64_t)(S>>64);
  while(lo){int v=__builtin_ctzll(lo); lo&=lo-1; r|=cl[v];}
  while(hi){int v=64+__builtin_ctzll(hi); hi&=hi-1; r|=cl[v];}
  return r & ~bit(c);
}
struct Key{B S; int c; bool operator==(const Key&o)const{return S==o.S&&c==o.c;}};
struct KH{size_t operator()(const Key&k)const{
  uint64_t a=(uint64_t)k.S, b=(uint64_t)(k.S>>64);
  uint64_t x=a*0x9E3779B97F4A7C15ULL ^ (b+0x7F4A7C15ULL)*0xC2B2AE3D27D4EB4FULL ^ ((uint64_t)k.c*0x165667B19E3779F9ULL);
  x^=x>>29; x*=0xBF58476D1CE4E5B9ULL; x^=x>>32; return x;}};
enum {UNK=0,WIN=1,LOSE=2,NG=3};
struct Info{uint8_t st; uint8_t mv;}; // mv: next cop vertex for a blind move, 255 = glimpse
vector<unordered_map<Key,Info,KH>> memo;
bool win(int j,int c,B S);
// returns -1 if glimpse wins against every answer, else the failing answer (E: vertex v; D: neighbour u)
int glimpseFail(int j,int c,B S){
  if(G=='E'){
    for(int v=1;v<=n;v++) if(S&bit(v)){
      bool ok=false;
      for(int c2:nbc[c]){ if(c2==v){ok=true;break;} }
      if(!ok) for(int c2:nbc[c]){ if(win(j-1,c2,expandS(bit(v),c2))){ok=true;break;} }
      if(!ok) return v;
    }
  } else {
    for(size_t i=1;i<nbc[c].size();i++){
      B A=S&branchm[c][i]; if(!A) continue;
      bool ok=false;
      for(int c2:nbc[c]){ B A1=A&~bit(c2); if(!A1||win(j-1,c2,expandS(A1,c2))){ok=true;break;} }
      if(!ok) return nbc[c][i];
    }
  }
  return -1;
}
struct Node{Key k; int par; int mv;};
bool win(int j,int c,B S){
  if(!S) return true;
  auto &M=memo[j];
  Key k0{S,c};
  { auto it=M.find(k0); if(it!=M.end()){ if(it->second.st==WIN) return true; if(it->second.st==LOSE) return false; } }
  vector<Node> q; unordered_set<Key,KH> vis;
  q.push_back({k0,-1,-1}); vis.insert(k0);
  int found=-1;
  for(size_t i=0;i<q.size()&&found<0;i++){
    Key cur=q[i].k;
    int st=UNK; { auto it=M.find(cur); if(it!=M.end()) st=it->second.st; }
    if(st==WIN){found=i;break;}
    if(st==LOSE) continue;
    if(j>0 && st!=NG){
      if(glimpseFail(j,cur.c,cur.S)<0){ M[cur]={WIN,255}; found=i; break; }
      M[cur]={NG,0};
    }
    for(int c2:nbc[cur.c]){
      B S1=cur.S&~bit(c2);
      if(!S1){ M[cur]={WIN,(uint8_t)c2}; found=i; break; }
      Key nk{expandS(S1,c2),c2};
      if(vis.insert(nk).second) q.push_back({nk,(int)i,c2});
    }
  }
  if(found>=0){
    int x=found;
    while(q[x].par>=0){ int p=q[x].par; M[q[p].k]={WIN,(uint8_t)q[x].mv}; x=p; }
    return true;
  }
  for(auto&nd:q) M[nd.k]={LOSE,0};
  return false;
}
string hexS(B S){ char buf[40]; uint64_t hi=(uint64_t)(S>>64), lo=(uint64_t)S;
  if(hi) snprintf(buf,40,"%llx%016llx",(unsigned long long)hi,(unsigned long long)lo); else snprintf(buf,40,"%llx",(unsigned long long)lo); return buf;}
// ---- dumps
void dumpStrategy(FILE*f,int j,int c,B S,set<tuple<int,int,B>>&seen){
  if(!S) return;
  if(!seen.insert(make_tuple(j,c,S)).second) return;
  if(!win(j,c,S)){fprintf(stderr,"dumpStrategy on non-win\n");exit(2);}
  Info inf=memo[j][Key{S,c}];
  if(inf.mv!=255){
    int c2=inf.mv; fprintf(f,"%d %d %s M %d\n",j,c,hexS(S).c_str(),c2);
    B S1=S&~bit(c2); if(S1) dumpStrategy(f,j,c2,expandS(S1,c2),seen);
  } else {
    string line; vector<tuple<int,B>> nxt;
    if(G=='E'){
      for(int v=1;v<=n;v++) if(S&bit(v)){
        int ch=-1; for(int c2:nbc[c]) if(c2==v){ch=c2;break;}
        if(ch<0) for(int c2:nbc[c]) if(win(j-1,c2,expandS(bit(v),c2))){ch=c2;break;}
        line+=" "+to_string(v)+":"+to_string(ch);
        if(ch!=v) nxt.push_back(make_tuple(ch,expandS(bit(v),ch)));
      }
    } else {
      for(size_t i=1;i<nbc[c].size();i++){ B A=S&branchm[c][i]; if(!A) continue;
        int ch=-1; for(int c2:nbc[c]){ B A1=A&~bit(c2); if(!A1||win(j-1,c2,expandS(A1,c2))){ch=c2;break;} }
        line+=" "+to_string(nbc[c][i])+":"+to_string(ch);
        B A1=A&~bit(ch); if(A1) nxt.push_back(make_tuple(ch,expandS(A1,ch)));
      }
    }
    fprintf(f,"%d %d %s G%s\n",j,c,hexS(S).c_str(),line.c_str());
    for(auto&t:nxt) dumpStrategy(f,j-1,get<0>(t),get<1>(t),seen);
  }
}
long dumpLose(FILE*f,int j0,int c0,B S0,set<tuple<int,int,B>>&seen){
  // iterative closure of losing states
  vector<tuple<int,int,B>> st; st.push_back(make_tuple(j0,c0,S0)); long cnt=0;
  while(!st.empty()){
    auto t=st.back(); st.pop_back();
    int j=get<0>(t),c=get<1>(t); B S=get<2>(t);
    if(!seen.insert(t).second) continue;
    if(win(j,c,S)){fprintf(stderr,"dumpLose on win state\n");exit(2);}
    int bad=0;
    if(j>0){ bad=glimpseFail(j,c,S); if(bad<0){fprintf(stderr,"inconsistent\n");exit(2);}
      B A = (G=='E')? bit(bad) : B(0);
      if(G=='D'){ for(size_t i=1;i<nbc[c].size();i++) if(nbc[c][i]==bad) A=S&branchm[c][i]; }
      for(int c2:nbc[c]){ B A1=A&~bit(c2); st.push_back(make_tuple(j-1,c2,expandS(A1,c2))); }
    }
    if(f) fprintf(f,"%d %d %s %d\n",j,c,hexS(S).c_str(),bad);
    cnt++;
    for(int c2:nbc[c]){ B S1=S&~bit(c2); st.push_back(make_tuple(j,c2,expandS(S1,c2))); }
  }
  return cnt;
}
int main(int argc,char**argv){
  if(argc<4){fprintf(stderr,"usage: fast G h k [dump] [allstarts]\n");return 1;}
  G=argv[1][0]; h=atoi(argv[2]); int k=atoi(argv[3]); bool dump=argc>4&&atoi(argv[4]); bool allst=argc>5&&atoi(argv[5]);
  n=(1<<(h+1))-1; if(n>127){fprintf(stderr,"too big\n");return 1;}
  ALL=0; for(int v=1;v<=n;v++) ALL|=bit(v);
  for(int v=1;v<=n;v++){ nbc[v].push_back(v); if(v>1) nbc[v].push_back(v/2); if(2*v<=n){nbc[v].push_back(2*v);nbc[v].push_back(2*v+1);}
    cl[v]=0; for(int u:nbc[v]) cl[v]|=bit(u); }
  for(int c=1;c<=n;c++) for(size_t i=1;i<nbc[c].size();i++){
    B comp=bit(nbc[c][i]); for(;;){ B e=expandS(comp,c); if(e==comp)break; comp=e; } branchm[c][i]=comp; }
  memo.resize(k+1);
  vector<int> starts; if(allst){ for(int v=1;v<=n;v++) starts.push_back(v);} else { for(int d=0;d<=h;d++) starts.push_back(1<<d); }
  int winstart=-1;
  for(int c:starts){ bool w=win(k,c,ALL&~bit(c)); printf("%c h=%d k=%d start=%d : %s\n",G,h,k,c,w?"COP WINS":"cop loses"); fflush(stdout); if(w&&winstart<0){winstart=c; if(!allst) break;} }
  size_t tot=0; for(int j=0;j<=k;j++){ printf("  memo[%d]=%zu\n",j,memo[j].size()); tot+=memo[j].size(); }
  printf("RESULT %c h=%d k=%d : %s (states cached %zu)\n",G,h,k,winstart>=0?"WIN":"LOSE",tot);
  if(dump){
    char nm[100];
    if(winstart>=0){ snprintf(nm,100,"strat_%c_h%d_k%d.txt",G,h,k); FILE*f=fopen(nm,"w");
      fprintf(f,"# game %c h=%d k=%d start=%d ; lines: j c S(hex,bit v=vertex v) M c' | G ans:move ...\n",G,h,k,winstart);
      set<tuple<int,int,B>> seen; dumpStrategy(f,k,winstart,ALL&~bit(winstart),seen); fclose(f); printf("wrote %s (%zu states)\n",nm,seen.size()); }
    else { snprintf(nm,100,"lose_%c_h%d_k%d.txt",G,h,k); FILE*f=fopen(nm,"w");
      fprintf(f,"# game %c h=%d k=%d ; closed set of cop-losing states; lines: j c S(hex) badanswer(0 if j=0)\n",G,h,k);
      set<tuple<int,int,B>> seen; long cnt=0; for(int c:starts) cnt+=dumpLose(f,k,c,ALL&~bit(c),seen); fclose(f); printf("wrote %s (%ld states)\n",nm,cnt); }
  }
  if(dump==0 && argc>6){ // dump whole cache for cross-check
    FILE*f=fopen(argv[6],"w"); for(int j=0;j<=k;j++) for(auto&kv:memo[j]) if(kv.second.st==WIN||kv.second.st==LOSE) fprintf(f,"%d %d %s %d\n",j,kv.first.c,hexS(kv.first.S).c_str(),kv.second.st==WIN); fclose(f); }
  return 0;
}
