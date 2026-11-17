// Fast search for repairable loops. Loop = (uint64 w, int m), bit i = v_i.
// modes:
//   brute n L          : all 2^L words, no pruning except canonical-rotation skip (cross-check vs slow.py)
//   dfs n L [maxprint] : normalised DFS (w = longest 0-run first, last symbol 1), pruned by repeat count
//   check n word       : verbose check of one loop
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
typedef unsigned long long u64;
static int N; static u64 FULL;
static inline u64 lowmask(int m){ return m>=64? ~0ULL : ((1ULL<<m)-1); }
static u64 cover(u64 w,int m){
    u64 s=0; u64 nm=(1ULL<<N)-1;
    if(m>=N && m+N-1<=64){ 
        // ext = w followed by first N-1 bits
        unsigned __int128 e = (unsigned __int128)w | ((unsigned __int128)(w & lowmask(N-1)) << m);
        for(int i=0;i<m;i++) s |= 1ULL << (u64)((e>>i)&nm);
    } else {
        for(int i=0;i<m;i++){ u64 c=0; for(int j=0;j<N;j++) c |= ((w>>((i+j)%m))&1ULL)<<j; s |= 1ULL<<c; }
    }
    return s;
}
static inline u64 lossAt(u64 w,int m,int k){ u64 lo=w&lowmask(k); u64 hi=(w>>(k+1)); return lo|(hi<<k); }
static inline u64 dblAt(u64 w,int m,int k){ u64 lo=w&lowmask(k+1); u64 hi=(w>>k); return lo|(hi<<(k+1)); }
static inline bool univ(u64 w,int m){ return cover(w,m)==FULL; }
// can the mechanic make (w,m) universal with <=1 edit?  (all cells tried; edits in the same run give the same loop, skipped when adjacent bits equal)
static bool fixable(u64 w,int m){
    u64 c=cover(w,m); if(c==FULL) return true;
    if(m>=N+1 && __builtin_popcountll(c) < (1<<N)-(N-1)) return false; // an edit creates <= N-1 new windows (needs m-1>=N)
    for(int k=0;k<m;k++){
        if(k>0 && ((w>>k)&1)==((w>>(k-1))&1)) continue; // same run as previous cell: same result
        if(m>=2 && univ(lossAt(w,m,k),m-1)) return true;
        if(univ(dblAt(w,m,k),m+1)) return true;
    }
    return false;
}
static bool repairable(u64 w,int m,bool verbose=false){
    if(!fixable(w,m)){ if(verbose) printf("adversary PASS is unanswerable\n"); return false; }
    for(int k=0;k<m;k++){
        if(k>0 && ((w>>k)&1)==((w>>(k-1))&1)) continue;
        if(m>=2 && !fixable(lossAt(w,m,k),m-1)){ if(verbose) printf("adversary LOSS at cell %d is unanswerable\n",k); return false; }
        if(!fixable(dblAt(w,m,k),m+1)){ if(verbose) printf("adversary DOUBLING at cell %d is unanswerable\n",k); return false; }
    }
    return true;
}
static string str(u64 w,int m){ string s; for(int i=0;i<m;i++) s.push_back('0'+((w>>i)&1)); return s; }
static u64 canon(u64 w,int m){ u64 b=w; for(int i=1;i<m;i++){ w=((w>>1)|((w&1)<<(m-1))); if(w<b) b=w; } return b; }

// ---------- DFS -------------
static int L, MAXREP; static long long maxprint=20;
struct St{ u64 w; int len; int cnt[64]; int rep; int a; int zr; };
static long long found=0, leaves=0, nodes=0;
static void leaf(const St&s){
    #pragma omp atomic
    leaves++;
    u64 c=cover(s.w,L);
    if(__builtin_popcountll(c) < (1<<N)-(N-1)) return;
    if(repairable(s.w,L)){
        #pragma omp critical
        { found++; if(found<=maxprint) printf("REPAIRABLE %s %s\n",str(s.w,L).c_str(), c==FULL?"universal":"NOT-universal"); fflush(stdout); }
    }
}
static void dfs(St&s){
    if(s.len==L){ leaf(s); return; }
    for(int b=0;b<2;b++){
        if(s.len==L-1 && b==0) continue;           // last symbol is 1
        int zr = b? 0 : s.zr+1;
        if(zr> s.a) continue;                       // first run is a longest 0-run
        u64 w2 = s.w | ((u64)b<<s.len); int len=s.len+1; int rep=s.rep; int c=-1;
        if(len>=N){ c=(int)((w2>>(len-N))&((1ULL<<N)-1)); if(s.cnt[c]) rep++; if(rep>MAXREP) continue; }
        St t=s; t.w=w2; t.len=len; t.rep=rep; t.zr=zr; if(c>=0) t.cnt[c]++;
        dfs(t);
    }
}
static void collect(St&s,int depth,vector<St>&out){
    if(s.len==L || s.len>=depth){ out.push_back(s); return; }
    for(int b=0;b<2;b++){
        if(s.len==L-1 && b==0) continue;
        int zr = b? 0 : s.zr+1; if(zr>s.a) continue;
        u64 w2 = s.w | ((u64)b<<s.len); int len=s.len+1; int rep=s.rep; int c=-1;
        if(len>=N){ c=(int)((w2>>(len-N))&((1ULL<<N)-1)); if(s.cnt[c]) rep++; if(rep>MAXREP) continue; }
        St t=s; t.w=w2; t.len=len; t.rep=rep; t.zr=zr; if(c>=0) t.cnt[c]++;
        collect(t,depth,out);
    }
}
int main(int argc,char**argv){
    string mode=argv[1]; N=atoi(argv[2]); FULL = (N==6)? ~0ULL : ((1ULL<<(1<<N))-1);
    if(mode=="check"){
        string s=argv[3]; int m=s.size(); u64 w=0; for(int i=0;i<m;i++) if(s[i]=='1') w|=1ULL<<i;
        printf("n=%d len=%d universal=%d\n",N,m,(int)univ(w,m));
        bool r=repairable(w,m,true); printf("repairable=%d\n",(int)r); return r?0:1;
    }
    L=atoi(argv[3]);
    if(mode=="brute"){
        long long cntr=0; string first;
        for(u64 w=0; w<(1ULL<<L); w++){
            if(canon(w,L)!=w) continue;
            if(repairable(w,L)){ cntr++; if(first.empty()) first=str(w,L);} }
        printf("brute n=%d L=%d repairable necklaces: %lld %s\n",N,L,cntr,first.c_str()); return 0;
    }
    if(argc>4) maxprint=atoll(argv[4]);
    MAXREP = L-(1<<N)+(N-1);
    vector<St> pre;
    for(int a=1;a<L;a++){ St s; memset(&s,0,sizeof s); s.len=a; s.a=a; s.zr=a; s.w=0;
        // windows inside 0^a
        bool ok=true; for(int len=N; len<=a; len++){ if(s.cnt[0]) s.rep++; s.cnt[0]++; } if(s.rep>MAXREP) ok=false;
        if(!ok) continue;
        // next symbol must be 1 (run is maximal) : handled by zr>a prune
        collect(s, a+14, pre); }
    #pragma omp parallel for schedule(dynamic,1) num_threads(2)
    for(size_t i=0;i<pre.size();i++){ St s=pre[i]; dfs(s); }
    printf("dfs n=%d L=%d maxrep=%d prefixes=%zu leaves=%lld repairable(normalised words)=%lld\n",N,L,MAXREP,pre.size(),leaves,found);
    return 0;
}
