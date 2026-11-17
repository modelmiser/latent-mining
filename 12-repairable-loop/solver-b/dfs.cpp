// DFS over loops of length L with w[0]=0, w[L-1]=1 (every non-constant loop has such a rotation;
// constant loops are never repairable for n>=1 since no edit creates the other letter).
// Prune: a repairable w must be within ONE edit of universal (adversary passes), and one edit creates
// at most n new windows, so w covers >= 2^n - n words, so (#windows) - (#distinct words) <= L - 2^n + n.
// Linear windows of a prefix are windows of w, so duplicates among them only grow. Safe prune.
// usage: dfs n L [prefix]   (prefix = forced first bits, to split work); prints repairable loops + COUNT
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
typedef uint64_t u64;
static int n,L; static u64 FULL; static int budget; static unsigned WM;
static inline u64 cov(u64 v,int m){
    u64 mask=0; unsigned x=0;
    for(int j=n-1;j>=1;j--){ int idx=((-j)%m+m)%m; x=(x<<1)|((v>>idx)&1); }
    for(int i=0;i<m;i++){ x=((x<<1)|((v>>i)&1))&WM; mask|=1ULL<<x; }
    return mask;
}
static inline u64 loss(u64 v,int k){ u64 lo=v&((1ULL<<k)-1); u64 hi=(v>>(k+1))<<k; return lo|hi; }
static inline u64 dbl(u64 v,int k){ u64 lo=v&((1ULL<<(k+1))-1); u64 hi=(v>>k)<<(k+1); return lo|hi; }
static bool fixable(u64 v,int m){
    u64 c=cov(v,m); if(c==FULL) return true;
    if(__builtin_popcountll(FULL&~c)>n) return false;
    for(int k=0;k<m;k++){
        if(m>=2 && cov(loss(v,k),m-1)==FULL) return true;
        if(cov(dbl(v,k),m+1)==FULL) return true;
    }
    return false;
}
static bool repairable(u64 w){
    if(!fixable(w,L)) return false;
    for(int k=0;k<L;k++){
        if(L>=2 && !fixable(loss(w,k),L-1)) return false;
        if(!fixable(dbl(w,k),L+1)) return false;
    }
    return true;
}
static u64 found=0, leaves=0, nodes=0; static int cnt[64]; static const char*prefix="";static int plen=0;
static void rec(int pos,u64 w,unsigned x,int dups){
    nodes++;
    if(pos==L){
        if(!((w>>(L-1))&1)) return;
        // wrap windows: those starting at L-n+1..L-1
        int d=dups; int added[8]; int na=0; unsigned y=x;
        for(int j=0;j<n-1;j++){ y=((y<<1)|((w>>j)&1))&WM; if(cnt[y]++) d++; added[na++]=y; }
        for(int j=0;j<na;j++) cnt[added[j]]--;
        if(d>budget) return;
        leaves++;
        if(repairable(w)){ found++; for(int i=0;i<L;i++) putchar('0'+((w>>i)&1)); putchar('\n'); }
        return;
    }
    for(int b=0;b<2;b++){
        if(pos==0 && b==1) continue;
        if(pos<plen && prefix[pos]-'0'!=b) continue;
        unsigned y=((x<<1)|b)&WM; int d=dups;
        if(pos>=n-1){ if(cnt[y]) d++; if(d>budget) continue; cnt[y]++; }
        rec(pos+1,w|((u64)b<<pos),y,d);
        if(pos>=n-1) cnt[y]--;
    }
}
int main(int argc,char**argv){
    n=atoi(argv[1]); L=atoi(argv[2]); if(argc>3){prefix=argv[3];plen=strlen(prefix);}
    FULL=(n==6)?~0ULL:((1ULL<<(1<<n))-1); WM=(1u<<n)-1; budget=L-(1<<n)+n;
    if(L<n){fprintf(stderr,"need L>=n\n");return 1;}
    rec(0,0,0,0);
    printf("COUNT n=%d L=%d prefix=%s found=%llu leaves=%llu nodes=%llu\n",n,L,prefix,(unsigned long long)found,(unsigned long long)leaves,(unsigned long long)nodes);
}
