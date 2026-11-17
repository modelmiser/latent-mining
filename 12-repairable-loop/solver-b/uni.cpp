// Enumerate all UNIVERSAL loops u of length m (order n), in the rotation u = 0^n ... 1
// (every universal loop has a run of 0s of length >= n; rotate to start at the LAST n cells of such a run... we
//  simply require u[0..n-1]=0 and u[m-1]=1, i.e. start = beginning of a 0-run of length>=n; such a rotation exists).
// Prune: universal of length m  <=>  duplicates among the m windows == m-2^n; linear-prefix windows are windows of u,
// so prune when prefix duplicates exceed m-2^n. Exact, no heuristic.
// For each u, test candidates for repairability at target length L:
//   mode 0 (L=m):   w=u
//   mode +1 (L=m+1): w = u with one letter inserted (every position, both letters)   [mechanic's loss returns u-like]
//   mode -1 (L=m-1): w = u with one cell deleted (every cell)                         [mechanic's doubling]
// Justification: a repairable w is within one mechanic edit of a universal loop u (adversary passes):
//   pass -> w=u ; mechanic loss -> w is u plus one inserted cell ; mechanic doubling -> w is u minus one cell.
// So all repairable loops of length L are found from universal loops of lengths L-1, L, L+1.
// usage: uni n m mode [prefix]    prints canonical (least rotation) repairable loops, then COUNT line
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
typedef uint64_t u64;
static int n,m,mode; static u64 FULL; static int budget; static unsigned WM;
static inline u64 cov(u64 v,int len){
    u64 mask=0; unsigned x=0;
    for(int j=n-1;j>=1;j--){ int idx=((-j)%len+len)%len; x=(x<<1)|((v>>idx)&1); }
    for(int i=0;i<len;i++){ x=((x<<1)|((v>>i)&1))&WM; mask|=1ULL<<x; }
    return mask;
}
static inline u64 loss(u64 v,int k){ u64 lo=v&((1ULL<<k)-1); u64 hi=(v>>(k+1))<<k; return lo|hi; }
static inline u64 dbl(u64 v,int k){ u64 lo=v&((1ULL<<(k+1))-1); u64 hi=(v>>k)<<(k+1); return lo|hi; }
static inline u64 ins(u64 v,int k,int b){ u64 lo=v&((1ULL<<k)-1); u64 hi=(v>>k)<<(k+1); return lo|hi|((u64)b<<k); } // new cell at index k
static bool fixable(u64 v,int len){
    u64 c=cov(v,len); if(c==FULL) return true;
    if(__builtin_popcountll(FULL&~c)>n) return false;
    for(int k=0;k<len;k++){
        if(len>=2 && cov(loss(v,k),len-1)==FULL) return true;
        if(cov(dbl(v,k),len+1)==FULL) return true;
    }
    return false;
}
static bool repairable(u64 w,int L){
    if(!fixable(w,L)) return false;
    for(int k=0;k<L;k++){
        if(L>=2 && !fixable(loss(w,k),L-1)) return false;
        if(!fixable(dbl(w,k),L+1)) return false;
    }
    return true;
}
static void emit(u64 w,int L){
    u64 best=w, M=(L==64)?~0ULL:((1ULL<<L)-1), r=w;
    // canonical: least string in lexicographic order of cells; compare via reversed-bit integer is messy -> do string compare
    char bs[70],cur[70]; for(int i=0;i<L;i++) bs[i]='0'+((w>>i)&1); bs[L]=0;
    for(int s=1;s<L;s++){ for(int i=0;i<L;i++) cur[i]='0'+((w>>((i+s)%L))&1); cur[L]=0; if(strcmp(cur,bs)<0) strcpy(bs,cur); }
    (void)best;(void)M;(void)r; puts(bs);
}
static u64 found=0, unis=0, nodes=0, tested=0; static int cnt[64]; static const char*prefix="";static int plen=0;
static void leaf(u64 u){
    unis++;
    if(mode==0){ tested++; if(repairable(u,m)){found++; emit(u,m);} }
    else if(mode==1){ for(int k=0;k<m;k++) for(int b=0;b<2;b++){ // insert before cell k (cyclically covers all gaps)
            if(((u>>k)&1)==(unsigned)b) { int pk=(k+m-1)%m; if(((u>>pk)&1)==(unsigned)b) continue; } // same run as prev: dup of an earlier insertion
            u64 w=ins(u,k,b); tested++; if(repairable(w,m+1)){found++; emit(w,m+1);} } }
    else { for(int k=0;k<m;k++){ u64 w=loss(u,k); tested++; if(repairable(w,m-1)){found++; emit(w,m-1);} } }
}
static void rec(int pos,u64 w,unsigned x,int dups){
    nodes++;
    if(pos==m){
        if(!((w>>(m-1))&1)) return;
        int d=dups; int added[8]; int na=0; unsigned y=x;
        for(int j=0;j<n-1;j++){ y=((y<<1)|((w>>j)&1))&WM; if(cnt[y]++) d++; added[na++]=y; }
        for(int j=0;j<na;j++) cnt[added[j]]--;
        if(d!=budget) return;
        leaf(w); return;
    }
    for(int b=0;b<2;b++){
        if(pos<n && b==1) continue;
        if(pos<plen && prefix[pos]-'0'!=b) continue;
        unsigned y=((x<<1)|b)&WM; int d=dups;
        if(pos>=n-1){ if(cnt[y]) d++; if(d>budget) continue; cnt[y]++; }
        rec(pos+1,w|((u64)b<<pos),y,d);
        if(pos>=n-1) cnt[y]--;
    }
}
int main(int argc,char**argv){
    n=atoi(argv[1]); m=atoi(argv[2]); mode=atoi(argv[3]); if(argc>4){prefix=argv[4];plen=strlen(prefix);}
    FULL=(n==6)?~0ULL:((1ULL<<(1<<n))-1); WM=(1u<<n)-1; budget=m-(1<<n);
    if(budget<0){ printf("COUNT n=%d m=%d mode=%d found=0 unis=0 (m<2^n)\n",n,m,mode); return 0; }
    rec(0,0,0,0);
    printf("COUNT n=%d m=%d mode=%d prefix=%s found=%llu unis=%llu tested=%llu nodes=%llu\n",n,m,mode,prefix,(unsigned long long)found,(unsigned long long)unis,(unsigned long long)tested,(unsigned long long)nodes);
}
