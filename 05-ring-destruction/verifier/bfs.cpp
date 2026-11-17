// Referee's own solver. Layered BFS over (knowledge sets, previous destroyed set), straight from the definition.
// usage: bfs n k canon(0/1) useful(0/1) [dumpfile maxdump]
// useful=1: only destroy links whose message would add something (safe: such a destruction changes nothing now and only
// restricts the next round); prev is then intersected with links useful in the new state.
// useful=0: all subsets of all links (pure definition), prev kept in full.
#include <bits/stdc++.h>
using namespace std;
int n,k; bool canon,usefulOnly; int target=0;
struct S{ uint16_t K[12]; uint32_t prev; };
struct Key{ array<uint64_t,3> a; bool operator==(const Key&o)const{return a==o.a;} };
struct KH{ size_t operator()(const Key&x)const{ uint64_t h=x.a[0]*0x9E3779B97F4A7C15ULL; h^=x.a[1]+0x7F4A7C15+(h<<6)+(h>>2); h*=0xC2B2AE3D27D4EB4FULL; h^=x.a[2]+(h<<6)+(h>>2); return h; } };
uint16_t full;
// link index: 2*i = (i -> i+1), 2*i+1 = (i -> i-1)
inline int src(int l){return l/2;} inline int dst(int l){ int i=l/2; return (l&1)? (i+n-1)%n : (i+1)%n; }
Key pack(const S&s){ Key x; x.a={0,0,0}; int bit=0; for(int i=0;i<n;i++){ for(int b=0;b<n;b++,bit++) if(s.K[i]>>b&1) x.a[bit/64]|=1ULL<<(bit%64);} for(int b=0;b<2*n;b++,bit++) if(s.prev>>b&1) x.a[bit/64]|=1ULL<<(bit%64); return x; }
inline uint16_t rotm(uint16_t m,int r){ return ((m<<r)|(m>>(n-r)))&full; }
uint16_t refm(uint16_t m){ uint16_t o=0; for(int b=0;b<n;b++) if(m>>b&1) o|=1<<((n-b)%n); return o; }
S transform(const S&s,int r,bool f){ S o; memset(&o,0,sizeof o);
  for(int i=0;i<n;i++){ int j = f? (n-i)%n : i; uint16_t m = f? refm(s.K[i]) : s.K[i]; j=(j+r)%n; o.K[j]= r? rotm(m,r):m; }
  for(int l=0;l<2*n;l++) if(s.prev>>l&1){ int a=src(l), b=dst(l); if(f){a=(n-a)%n;b=(n-b)%n;} a=(a+r)%n;b=(b+r)%n; int nl = (b==(a+1)%n)? 2*a : 2*a+1; if(n==2) abort(); o.prev|=1u<<nl; }
  return o; }
bool lessS(const S&a,const S&b){ int c=memcmp(a.K,b.K,sizeof a.K); if(c) return c<0; return a.prev<b.prev; }
S canonS(const S&s){ S best=s; for(int f=0;f<2;f++) for(int r=0;r<n;r++){ if(!f&&!r) continue; S t=transform(s,r,f); if(lessS(t,best)) best=t; } return best; }
struct Rec{ S s; int parent; uint32_t D; };
// sound prune for decision 'survive round target': no link fails twice in a row, so every link delivers at least once in any
// two consecutive rounds; hence after r more rounds process i surely knows K[i+j] and K[i-j] for all j<=floor(r/2).
bool doomed(const S&o,int tnow){ int r=target-tnow; int J=r/2; if(J<=0) return false; if(J>n) J=n; for(int i=0;i<n;i++){ uint16_t v=o.K[i]; for(int j=1;j<=J;j++){ v|=o.K[(i+j)%n]; v|=o.K[((i-j)%n+n)%n]; } if(v==full) return true; } return false; }
int main(int argc,char**argv){ n=atoi(argv[1]);k=atoi(argv[2]);canon=atoi(argv[3]);usefulOnly=atoi(argv[4]); full=(1<<n)-1;
  const char*dump= argc>5&&argv[5][0]!='-'? argv[5]:nullptr; long maxdump= argc>6? atol(argv[6]):0; if(argc>7) target=atoi(argv[7]);
  vector<vector<Rec>> layers; { S s; memset(&s,0,sizeof s); for(int i=0;i<n;i++) s.K[i]=1<<i; layers.push_back({Rec{s,-1,0}}); }
  int t=0;
  while(true){ vector<Rec>&cur=layers.back(); unordered_set<Key,KH> seen; vector<Rec> nxt;
    for(size_t idx=0; idx<cur.size(); idx++){ S s=layers.back()[idx].s;
      uint32_t avail=0; for(int l=0;l<2*n;l++){ if(s.prev>>l&1) continue; if(usefulOnly && !(s.K[src(l)] & ~s.K[dst(l)])) continue; avail|=1u<<l; }
      vector<int> ls; for(int l=0;l<2*n;l++) if(avail>>l&1) ls.push_back(l);
      int m=ls.size(); vector<int> st; // enumerate subsets size<=k via recursion
      function<void(int,uint32_t,int)> rec=[&](int pos,uint32_t D,int cnt){
        if(pos==m){ S o; o.prev=0; bool done=false; for(int i=0;i<n;i++){ uint16_t v=s.K[i]; int r=(i+1)%n, lft=(i+n-1)%n; if(!(D>>(2*r+1)&1)) v|=s.K[r]; if(!(D>>(2*lft)&1)) v|=s.K[lft]; o.K[i]=v; if(v==full) done=true; } for(int i=n;i<12;i++) o.K[i]=0;
          if(done) return; if(target && doomed(o,t+1)) return;
          if(usefulOnly){ uint32_t p=0; for(int l=0;l<2*n;l++) if((D>>l&1) && (o.K[src(l)] & ~o.K[dst(l)])) p|=1u<<l; o.prev=p; } else o.prev=D;
          if(canon) o=canonS(o);
          Key key=pack(o); if(seen.insert(key).second) nxt.push_back(Rec{o,(int)idx,D});
          return; }
        rec(pos+1,D,cnt); if(cnt<k) rec(pos+1,D|(1u<<ls[pos]),cnt+1); };
      rec(0,0,0);
    }
    if(nxt.empty()) break; t++; if(target && t>=target){ layers.push_back(move(nxt)); break; } fprintf(stderr,"n=%d k=%d layer %d states %zu\n",n,k,t,nxt.size()); layers.push_back(move(nxt));
    if(!dump){ // free older layers except last
      if(layers.size()>=2) { vector<Rec>().swap(layers[layers.size()-2]); } }
  }
  if(target) printf("n=%d k=%d survive round %d: %s (deepest layer %d)\n",n,k,target,t>=target?"YES":"NO",t); else printf("F(%d,%d)=%d canon=%d useful=%d\n",n,k,t+1,(int)canon,(int)usefulOnly);
  if(dump && !canon && t>=1){ FILE*f=fopen(dump,"w"); vector<Rec>&last=layers.back(); long cnt=min<long>(maxdump,last.size()); long step=max<long>(1,last.size()/cnt);
    for(long q=0,i=0;q<cnt && i<(long)last.size();q++,i+=step){ vector<uint32_t> Ds; int idx=i; for(int L=layers.size()-1;L>=1;L--){ Ds.push_back(layers[L][idx].D); idx=layers[L][idx].parent; } reverse(Ds.begin(),Ds.end());
      fprintf(f,"%d %d",n,k); for(uint32_t D:Ds){ fprintf(f," |"); for(int l=0;l<2*n;l++) if(D>>l&1) fprintf(f," %d>%d",src(l),dst(l)); } fprintf(f,"\n"); }
    fclose(f); }
}
