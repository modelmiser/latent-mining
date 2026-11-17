// Exhaustive decision search: can the adversary keep every process incomplete through round T?
// Model: K_t(i) is always the arc {i-b_i .. i+a_i}; a_i' = max(a_i, a_{i+1}+1) unless link (i+1 -> i) destroyed,
//        b_i' = max(b_i, b_{i-1}+1) unless link (i-1 -> i) destroyed; complete iff a_i+b_i >= n-1.
// Link bits: bit i = "A-link into i" = (i+1 -> i); bit n+i = "B-link into i" = (i-1 -> i).
// Prunings (all safe, i.e. never turn a YES into a NO):
//  P1 never destroy a link whose message adds nothing this round (same knowledge, strictly fewer future options)
//  P2 memoise NO answers on (state, rounds left), state canonical under the dihedral group; forbidden set
//     intersected with links that are useful next round (others are never destroyed by P1)
//  P3 (flag prune>=1) chain potential: a_i(T) >= min_{0<=j<=floor((r+1)/2)} a_{i+r-j} + r - j
//  P4 (flag prune>=2) event count: pieces that must receive an event; needs <= k*r
// usage: fast n k T prune   -> prints YES + witness sequence, or NO
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128;
int n, k, T, PR;
struct St { uint8_t a[16], b[16]; uint32_t forb; };
struct H { size_t operator()(const u128& x) const { uint64_t lo=(uint64_t)x, hi=(uint64_t)(x>>64); lo^=hi*0x9E3779B97F4A7C15ULL; lo^=lo>>29; lo*=0xBF58476D1CE4E5B9ULL; lo^=lo>>32; return lo; } };
unordered_set<u128,H> memo;
uint64_t nodes=0;
vector<uint32_t> path;

static inline u128 pack(const St& s, int r, int rot, bool ref){
    u128 key=0; uint32_t f=0;
    for(int i=0;i<n;i++){
        int x,y,fa,fb;
        if(!ref){ int j=(i+rot)%n; x=s.a[j]; y=s.b[j]; fa=(s.forb>>j)&1; fb=(s.forb>>(n+j))&1; }
        else { int j=((rot-i)%n+n)%n; x=s.b[j]; y=s.a[j]; fa=(s.forb>>(n+j))&1; fb=(s.forb>>j)&1; }
        key=(key<<4)|x; key=(key<<4)|y; f|=fa<<i; f|=fb<<(n+i);
    }
    key=(key<<(2*n))|f; key=(key<<4)|r; return key;
}
static u128 canon(const St& s,int r){
    u128 best=~(u128)0;
    for(int rot=0;rot<n;rot++){ u128 x=pack(s,r,rot,false); if(x<best)best=x; x=pack(s,r,rot,true); if(x<best)best=x; }
    return best;
}
static inline uint32_t useful(const St& s){
    uint32_t u=0;
    for(int i=0;i<n;i++){
        if(s.a[(i+1)%n]+1>s.a[i]) u|=1u<<i;
        if(s.b[(i+n-1)%n]+1>s.b[i]) u|=1u<<(n+i);
    }
    return u;
}
static inline bool apply(const St& s,uint32_t D,St& o){
    for(int i=0;i<n;i++){
        int x=s.a[i], y=s.b[i];
        if(!((D>>i)&1)){ int c=s.a[(i+1)%n]+1; if(c>x)x=c; }
        if(!((D>>(n+i))&1)){ int c=s.b[(i+n-1)%n]+1; if(c>y)y=c; }
        if(x+y>=n-1) return false;
        o.a[i]=x;o.b[i]=y;
    }
    o.forb=D; return true;
}
static bool potential_ok(const St& s,int r){
    // r rounds left from state s. Lower bounds La,Lb on final extents.
    int J=(r+1)/2; int La[16],Lb[16];
    for(int i=0;i<n;i++){
        int m=1000;
        for(int j=0;j<=J;j++){ int v=s.a[((i+r-j)%n+n)%n]+r-j; if(v<m)m=v; }
        La[i]=m; m=1000;
        for(int j=0;j<=J;j++){ int v=s.b[((i-r+j)%n+n)%n]+r-j; if(v<m)m=v; }
        Lb[i]=m;
        if(La[i]+Lb[i]>=n-1) return false;
    }
    if(PR<2) return true;
    // event count: final position i, direction A: allowed a_i(T) <= n-2-Lb[i].
    uint32_t needA=0, needB=0;
    for(int i=0;i<n;i++){
        int amax=n-2-Lb[i];
        if(s.a[((i+r)%n)]+r>amax){
            int j=1; while(j<=J && s.a[((i+r-j)%n+n)%n]+r-j>amax) j++;
            // j<=J guaranteed since La[i]<=amax
            for(int m=0;m<j;m++) needA|=1u<<(((i-m)%n+n)%n);
        }
        int bmax=n-2-La[i];
        if(s.b[((i-r)%n+n)%n]+r>bmax){
            int j=1; while(j<=J && s.b[((i-r+j)%n+n)%n]+r-j>bmax) j++;
            for(int m=0;m<j;m++) needB|=1u<<((i+m)%n);
        }
    }
    return __builtin_popcount(needA)+__builtin_popcount(needB)<=k*r;
}
static bool surv(const St& s,int r){
    // can adversary survive r more rounds from s (s has no complete process)?
    if(r==0) return true;
    nodes++;
    if(PR>=1 && !potential_ok(s,r)) return false;
    u128 key=canon(s,r);
    if(memo.count(key)) return false;
    uint32_t us=useful(s)&~s.forb;
    int L[40],m=0; for(int i=0;i<2*n;i++) if((us>>i)&1) L[m++]=i;
    int kk=min(k,m);
    St o;
    for(int sz=kk;sz>=0;sz--){
        int idx[8]; for(int i=0;i<sz;i++) idx[i]=i;
        while(true){
            uint32_t D=0; for(int i=0;i<sz;i++) D|=1u<<L[idx[i]];
            if(apply(s,D,o)){
                uint32_t D2=D; o.forb=D&useful(o);
                if(surv(o,r-1)){ path.push_back(D2); return true; }
            }
            int p=sz-1; while(p>=0 && idx[p]==m-sz+p) p--;
            if(p<0) break;
            idx[p]++; for(int q=p+1;q<sz;q++) idx[q]=idx[q-1]+1;
        }
    }
    memo.insert(key);
    return false;
}
int main(int argc,char**argv){
    n=atoi(argv[1]);k=atoi(argv[2]);T=atoi(argv[3]);PR=argc>4?atoi(argv[4]):2;
    if(n>12||T>15||k>8){ fprintf(stderr,"out of range\n"); return 2; }
    St s; memset(&s,0,sizeof s);
    bool ok=surv(s,T);
    printf("n=%d k=%d T=%d prune=%d : %s nodes=%llu memo=%zu\n",n,k,T,PR,ok?"YES":"NO",(unsigned long long)nodes,memo.size());
    if(ok){
        // witness: rounds 1..T, links printed as "i>j"
        reverse(path.begin(),path.end());
        for(size_t t=0;t<path.size();t++){
            printf("W %zu:",t+1);
            for(int i=0;i<n;i++){ if((path[t]>>i)&1) printf(" %d>%d",(i+1)%n,i); if((path[t]>>(n+i))&1) printf(" %d>%d",(i+n-1)%n,i); }
            printf("\n");
        }
    }
    return 0;
}
