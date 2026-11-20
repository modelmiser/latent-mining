// fast.cpp  usage: fast n L mode [witnessfile]
// mode 0: FIFO expiry (the problem). mode 1: no Expire. mode 2: Adversary removes any ball of its choice.
// Decides whether Adversary can force load >= L on n bins. Exhaustive over all states reachable from empty.
// State = age-ordered list of balls (receiver, other); canonical labelling = order of first appearance
// (receiver before other inside a ball); isolated bins are interchangeable. Labels 1..n, nibble-packed.
#include <bits/stdc++.h>
using namespace std;
typedef uint64_t u64; typedef uint32_t u32;
int n,L,mode;
struct St{int k; int r[8],o[8];};
static u64 packc(const St&s){ // canonicalise + pack
  int mp[16]; memset(mp,0,sizeof mp); int nx=1; u64 x=0;
  for(int i=0;i<s.k;i++){ if(!mp[s.r[i]])mp[s.r[i]]=nx++; if(!mp[s.o[i]])mp[s.o[i]]=nx++;
    x|=(u64)(mp[s.r[i]]|(mp[s.o[i]]<<4))<<(8*i);} return x;}
static St unpack(u64 x){St s; s.k=0; while(x&0xff){s.r[s.k]=x&15; s.o[s.k]=(x>>4)&15; s.k++; x>>=8; if(s.k==8)break;} return s;}
static int maxload(const St&s){int c[16]={0},m=0; for(int i=0;i<s.k;i++){ if(++c[s.r[i]]>m)m=c[s.r[i]]; } return m;}
// hash table
vector<u64> keys; vector<u32> vals; u64 mask; vector<u64> states;
static inline u64 hsh(u64 x){x^=x>>33;x*=0xff51afd7ed558ccdULL;x^=x>>33;x*=0xc4ceb9fe1a85ec53ULL;x^=x>>33;return x;}
static void rehash(){ u64 nm=(mask+1)*2-1; vector<u64> nk(nm+1,~0ULL); vector<u32> nv(nm+1);
  for(u64 i=0;i<=mask;i++) if(keys[i]!=~0ULL){u64 h=hsh(keys[i])&nm; while(nk[h]!=~0ULL)h=(h+1)&nm; nk[h]=keys[i]; nv[h]=vals[i];}
  keys.swap(nk); vals.swap(nv); mask=nm;}
static u32 getid(u64 x){ u64 h=hsh(x)&mask; while(keys[h]!=~0ULL){ if(keys[h]==x) return vals[h]; h=(h+1)&mask;}
  keys[h]=x; vals[h]=states.size(); states.push_back(x); u32 id=vals[h]; if(states.size()*2>mask) rehash(); return id;}
// moves: list of (kind, a, b, succ1, succ2). kind 0 = removal (a = index of ball removed), kind 1 = offer {a,b}: succ1 = a receives, succ2 = b receives
struct Mv{int kind,a,b; u64 s1,s2;};
static void moves(const St&s, vector<Mv>&out){
  out.clear();
  int m=0; for(int i=0;i<s.k;i++){m=max(m,max(s.r[i],s.o[i]));}
  if(mode!=1 && s.k>0){ int lim=(mode==2)?s.k:1;
    for(int j=0;j<lim;j++){ St t; t.k=0; for(int i=0;i<s.k;i++) if(i!=j){t.r[t.k]=s.r[i];t.o[t.k]=s.o[i];t.k++;}
      u64 p=packc(t); out.push_back({0,j,0,p,p}); } }
  // components (labels 1..m used; m+1, m+2 stand for fresh isolated bins)
  int comp[16]; for(int i=0;i<16;i++)comp[i]=i;
  function<int(int)> f=[&](int x){while(comp[x]!=x)x=comp[x]=comp[comp[x]];return x;};
  for(int i=0;i<s.k;i++) comp[f(s.r[i])]=f(s.o[i]);
  int top=min(n,m+2);
  for(int a=1;a<=top;a++)for(int b=a+1;b<=top;b++){
    if(a<=m && b<=m && f(a)==f(b)) continue;
    if(a>m+1) continue; // both fresh: only (m+1,m+2)
    if(a<=m && b>m+1) continue; // used + fresh: only fresh = m+1
    St t=s; t.r[t.k]=a;t.o[t.k]=b;t.k++; u64 p1=packc(t); t.r[t.k-1]=b;t.o[t.k-1]=a; u64 p2=packc(t);
    out.push_back({1,a,b,p1,p2}); }
}
static string show(u64 x){St s=unpack(x); string r; for(int i=0;i<s.k;i++){r+=char('0'+s.r[i]);r+=char('0'+s.o[i]); if(i+1<s.k) r+=',';} if(r.empty())r="-"; return r;}
int main(int argc,char**argv){
  n=atoi(argv[1]);L=atoi(argv[2]);mode=atoi(argv[3]); const char*wf=argc>4?argv[4]:0;
  if(n>9||n<2){return 2;}
  mask=(1<<20)-1; keys.assign(mask+1,~0ULL); vals.resize(mask+1);
  getid(0);
  vector<u32> succ; vector<u64> off; vector<char> term; vector<Mv> mv;
  // BFS; succ layout per state: count, then triples (kind|a<<4|b<<8, id1, id2)
  for(u64 i=0;i<states.size();i++){
    St s=unpack(states[i]); off.push_back(succ.size());
    if(maxload(s)>=L){term.push_back(1); succ.push_back(0); continue;}
    term.push_back(0); moves(s,mv); succ.push_back(mv.size());
    for(auto&q:mv){ u32 a=getid(q.s1), b=getid(q.s2); succ.push_back(q.kind|q.a<<4|q.b<<8); succ.push_back(a); succ.push_back(b);} }
  u64 N=states.size(); vector<u32> rank(N,0xffffffffu), choice(N,0); u32 ctr=0;
  for(u64 i=0;i<N;i++) if(term[i]) rank[i]=0;
  bool ch=true; int sweeps=0;
  while(ch){ ch=false; sweeps++;
    for(u64 i=0;i<N;i++) if(rank[i]==0xffffffffu){ const u32*p=&succ[off[i]]; u32 c=*p++;
      for(u32 j=0;j<c;j++,p+=3) if(rank[p[1]]!=0xffffffffu && rank[p[2]]!=0xffffffffu){ rank[i]=++ctr; choice[i]=j; ch=true; break; } } }
  u64 nwin=0; for(u64 i=0;i<N;i++) nwin+=(rank[i]!=0xffffffffu);
  bool advwins = rank[0]!=0xffffffffu;
  printf("n=%d L=%d mode=%d states=%llu terminal=%llu advwin_states=%llu sweeps=%d RESULT: Adversary %s force load %d\n",n,L,mode,
    (unsigned long long)N,(unsigned long long)count(term.begin(),term.end(),1),(unsigned long long)nwin,sweeps,advwins?"CAN":"CANNOT",L);
  if(wf){ FILE*F=fopen(wf,"w");
    vector<char> seen(N,0); vector<u32> q; q.push_back(0); seen[0]=1;
    if(advwins){
      fprintf(F,"# ADVERSARY strategy n=%d L=%d mode=%d. line: state rank move ; state = age-ordered balls 'ro' (receiver,other), canonical first-appearance labels; move E<j> = remove j-th oldest ball (FIFO: E0), O<a><b> = offer {a,b} (labels > used count = fresh bins), T = load>=L reached\n",n,L,mode);
      for(size_t h=0;h<q.size();h++){ u32 i=q[h]; if(term[i]){fprintf(F,"%s 0 T\n",show(states[i]).c_str());continue;}
        const u32*p=&succ[off[i]]+1+3*choice[i]; int kind=p[0]&15,a=(p[0]>>4)&15,b=(p[0]>>8)&15;
        if(kind==0) fprintf(F,"%s %u E%d\n",show(states[i]).c_str(),rank[i],a); else fprintf(F,"%s %u O%d%d\n",show(states[i]).c_str(),rank[i],a,b);
        for(int t=1;t<=2;t++) if(!seen[p[t]]){seen[p[t]]=1;q.push_back(p[t]);} }
    } else {
      fprintf(F,"# RULE safe set n=%d L=%d mode=%d. Each line a state (age-ordered balls 'ro' receiver,other; canonical first-appearance labels). Claim: contains '-', all loads < L, closed under every removal, and for every legal offer at least one answer stays in the set.\n",n,L,mode);
      // greedy: prefer answer already seen, else the lower-id one
      for(size_t h=0;h<q.size();h++){ u32 i=q[h]; fprintf(F,"%s\n",show(states[i]).c_str());
        const u32*p=&succ[off[i]]; u32 c=*p++;
        for(u32 j=0;j<c;j++,p+=3){ int kind=p[0]&15; u32 a=p[1],b=p[2];
          if(kind==0){ if(rank[a]!=0xffffffffu){fprintf(stderr,"BUG\n");return 3;} if(!seen[a]){seen[a]=1;q.push_back(a);} continue;}
          bool oa=rank[a]==0xffffffffu, ob=rank[b]==0xffffffffu; if(!oa&&!ob){fprintf(stderr,"BUG2\n");return 3;}
          if((oa&&seen[a])||(ob&&seen[b])) continue;
          u32 pick=oa?a:b; seen[pick]=1; q.push_back(pick); } }
    }
    printf("witness lines=%zu file=%s\n",q.size(),wf); fclose(F); }
  return 0;}
