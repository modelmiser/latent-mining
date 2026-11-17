// Heuristic descent (UPPER bounds only).  Start from a universal repairable loop; repeatedly delete a cell and
// re-descend with a 2-move neighbourhood (flip, double flip, move-a-cell); random kicks when stuck.
// cost as in sa2.cpp (0 <=> universal and repairable, by Lemma 4).  Every printed loop must be re-verified with explain.py.
// usage: polish2 n startstring seed kicks
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <random>
typedef uint64_t u64; typedef unsigned __int128 W;
static int n; static u64 FULL; static unsigned WM;
static inline u64 cov(W v,int len){
    u64 mask=0; unsigned x=0;
    for(int j=n-1;j>=1;j--){ int idx=((-j)%len+len)%len; x=(x<<1)|(unsigned)((v>>idx)&1); }
    for(int i=0;i<len;i++){ x=((x<<1)|(unsigned)((v>>i)&1))&WM; mask|=1ULL<<x; }
    return mask;
}
static inline W loss(W v,int k){ W lo=v&((((W)1)<<k)-1); W hi=(v>>(k+1))<<k; return lo|hi; }
static inline W dbl(W v,int k){ W lo=v&((((W)1)<<(k+1))-1); W hi=(v>>k)<<(k+1); return lo|hi; }
static inline W ins(W v,int k,int b){ W lo=v&((((W)1)<<k)-1); W hi=(v>>k)<<(k+1); return lo|hi|((W)b<<k); }
static inline int miss(W v,int len){ return __builtin_popcountll(FULL&~cov(v,len)); }
static inline int bit(W v,int i){ return (int)((v>>i)&1); }
static int best1(W v,int len){
    int b=miss(v,len); if(b==0) return 0; if(b>n) return b-n+1;
    for(int k=0;k<len;k++){
        if(k>0 && bit(v,k)==bit(v,k-1)) continue;
        int a=miss(loss(v,k),len-1); if(a<b) b=a; a=miss(dbl(v,k),len+1); if(a<b) b=a; if(b==0) return 0;
    }
    return b;
}
static int cost(W w,int L,int cap){
    int c=1000*miss(w,L); if(c>=cap) return c;
    for(int k=0;k<L && c<cap;k++){
        int p=bit(w,(k+L-1)%L), q=bit(w,(k+1)%L), b=bit(w,k);
        if(p!=b && q!=b) c+=best1(loss(w,k),L-1);
    }
    return c;
}
static void pr(W w,int L,int c){ printf("L=%d cost=%d ",L,c); for(int i=0;i<L;i++) putchar('0'+bit(w,i)); putchar('\n'); fflush(stdout); }
static int descend(W&w,int L,int c){
    for(;;){
        W bw=w; int bc=c;
        for(int i=0;i<L && bc>0;i++){
            W v=w^(((W)1)<<i); int c2=cost(v,L,bc); if(c2<bc){bc=c2;bw=v;}
            for(int j=i+1;j<L && bc>0;j++){ W v2=v^(((W)1)<<j); c2=cost(v2,L,bc); if(c2<bc){bc=c2;bw=v2;} }
            W d=loss(w,i);
            for(int j=0;j<L-1 && bc>0;j++) for(int b=0;b<2;b++){ W v2=ins(d,j,b); c2=cost(v2,L,bc); if(c2<bc){bc=c2;bw=v2;} }
        }
        if(bc<c){ w=bw; c=bc; if(c==0) return 0; } else return c;
    }
}
int main(int argc,char**argv){
    n=atoi(argv[1]); const char*s=argv[2]; unsigned seed=atoi(argv[3]); int kicks=atoi(argv[4]);
    FULL=(n==6)?~0ULL:((1ULL<<(1<<n))-1); WM=(1u<<n)-1; std::mt19937_64 rng(seed);
    int L=strlen(s); W w=0; for(int i=0;i<L;i++) if(s[i]=='1') w|=((W)1)<<i;
    int c=cost(w,L,1<<30); c=descend(w,L,c); pr(w,L,c);
    while(c==0 && L>(1<<n)){
        // shorten: best single deletion, then descend, with kicks
        W good=w; int bc=1<<30; W bw=0;
        for(int i=0;i<L;i++){ W v=loss(w,i); int c2=cost(v,L-1,bc); if(c2<bc){bc=c2;bw=v;} }
        W cur=bw; int cc=descend(cur,L-1,bc);
        for(int k=0;k<kicks && cc>0;k++){
            W v=good; int i=rng()%L; v=loss(v,i);            // random deletion from the good loop
            for(int t=0;t<2;t++){ int r=rng()%2; if(r==0) v^=((W)1)<<(rng()%(L-1)); else { int a=rng()%(L-1); v=loss(v,a); v=ins(v,rng()%(L-2),rng()&1);} }
            int c2=cost(v,L-1,1<<30); c2=descend(v,L-1,c2); if(c2<cc){cc=c2;cur=v;}
        }
        if(cc>0){ printf("stuck at L=%d best cost %d\n",L-1,cc); break; }
        w=cur; L--; c=0; pr(w,L,0);
    }
}
