// Heuristic search (UPPER bounds only) among UNIVERSAL loops: by Lemma 4 a universal w is repairable iff every
// deletion of a length-1 run can be answered.  cost = BIG*missing(w) + sum over singleton cells s of
// min over mechanic moves of missing(w - s + move).   cost==0 => repairable (verify independently with explain.py).
// usage: sa2 n L seed iters [T0] [startstring]
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cmath>
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
static int cost(W w,int L){
    int c=1000*miss(w,L);
    for(int k=0;k<L;k++){
        int p=bit(w,(k+L-1)%L), q=bit(w,(k+1)%L), b=bit(w,k);
        if(p!=b && q!=b) c+=best1(loss(w,k),L-1);
    }
    return c;
}
int main(int argc,char**argv){
    n=atoi(argv[1]); int L=atoi(argv[2]); unsigned seed=atoi(argv[3]); long iters=atol(argv[4]); double T0=argc>5?atof(argv[5]):1.0;
    FULL=(n==6)?~0ULL:((1ULL<<(1<<n))-1); WM=(1u<<n)-1;
    std::mt19937_64 rng(seed);
    W w=0;
    if(argc>6){ L=strlen(argv[6]); for(int i=0;i<L;i++) if(argv[6][i]=='1') w|=((W)1)<<i; }
    else for(int i=0;i<L;i++) if(rng()&1) w|=((W)1)<<i;
    int c=cost(w,L); int bestc=c; W bestw=w;
    for(long it=0;it<iters && c>0;it++){
        double T=T0*(1.0-(double)it/iters)+0.02;
        W v=w; int r=rng()%4;
        if(r==0){ v^=((W)1)<<(rng()%L); }
        else if(r==1){ int i=rng()%L,j=(i+1)%L; if(bit(v,i)==bit(v,j)) continue; v^=(((W)1)<<i)|(((W)1)<<j); }
        else { int i=rng()%L; v=loss(v,i); int j=rng()%(L-1); if(r==2) v=dbl(v,j); else { int b=rng()&1; W lo=v&((((W)1)<<j)-1); W hi=(v>>j)<<(j+1); v=lo|hi|((W)b<<j);} }
        int c2=cost(v,L);
        if(c2<=c || std::uniform_real_distribution<double>(0,1)(rng)<exp((c-c2)/T)){ w=v; c=c2; if(c<bestc){bestc=c; bestw=w; fprintf(stderr,"it %ld cost %d\n",it,c);} }
    }
    printf("n=%d L=%d seed=%u best cost %d : ",n,L,seed,bestc); for(int i=0;i<L;i++) putchar('0'+bit(bestw,i)); putchar('\n');
    return bestc==0?0:2;
}
