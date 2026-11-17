// Exhaustive: all 2^L loops of length L, test repairability for order n (n<=6, L<=62).
// usage: brute n L [print]   -> COUNT n L total  (total counts all strings, like slow.py)
// env ADVNOPASS=1: variant reading in which the adversary MUST edit (no pass).
#include <cstdio>
#include <cstdlib>
#include <cstdint>
typedef uint64_t u64;
static int n; static u64 FULL; static int advnopass=0;
static inline u64 cov(u64 v,int m){ // bit i of v = cell i
    u64 mask=0; unsigned W=(1u<<n)-1; unsigned x=0;
    // preload last n-1 cells before cell 0 (cyclically, may wrap repeatedly)
    for(int j=n-1;j>=1;j--){ int idx=((-j)%m+m)%m; x=(x<<1)|((v>>idx)&1); }
    for(int i=0;i<m;i++){ x=((x<<1)|((v>>i)&1))&W; mask|=1ULL<<x; }
    return mask;
}
static inline u64 loss(u64 v,int k){ u64 lo=v&((1ULL<<k)-1); u64 hi=(v>>(k+1))<<k; return lo|hi; }
static inline u64 dbl(u64 v,int k){ u64 lo=v&((1ULL<<(k+1))-1); u64 hi=(v>>k)<<(k+1); return lo|hi; }
static bool fixable(u64 v,int m){ // exists mechanic edit (or pass) making universal
    u64 c=cov(v,m); if(c==FULL) return true;
    if(__builtin_popcountll(FULL&~c)>n) return false; // one edit creates at most n new windows
    for(int k=0;k<m;k++){
        int prev=(k+m-1)%m; bool samep=((v>>k)&1)==((v>>prev)&1);
        if(samep && m>1 && k!=0) continue; // same run as previous cell: same result (k=0 kept to be safe)
        if(m>=2 && cov(loss(v,k),m-1)==FULL) return true;
        if(cov(dbl(v,k),m+1)==FULL) return true;
    }
    return false;
}
static bool repairable(u64 w,int L){
    if(!advnopass && !fixable(w,L)) return false;
    for(int k=0;k<L;k++){
        if(L>=2 && !fixable(loss(w,k),L-1)) return false;
        if(!fixable(dbl(w,k),L+1)) return false;
    }
    return true;
}
int main(int argc,char**argv){
    n=atoi(argv[1]); int L=atoi(argv[2]); bool pr=argc>3; advnopass=getenv("ADVNOPASS")!=0;
    FULL=(n==6)?~0ULL:((1ULL<<(1<<n))-1);
    u64 cnt=0;
    for(u64 w=0;w<(1ULL<<L);w++){
        if(repairable(w,L)){ cnt++; if(pr){ for(int i=0;i<L;i++) putchar('0'+((w>>i)&1)); putchar('\n'); } }
    }
    printf("COUNT %d %d %llu\n",n,L,(unsigned long long)cnt);
}
