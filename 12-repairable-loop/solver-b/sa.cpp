// Heuristic (simulated annealing) search for repairable loops; gives UPPER bounds only.
// cost(w) = sum over adversary moves of min over mechanic moves of (#words missing from w'').  cost==0 <=> repairable.
// usage: sa n L seed iters [T0]
#include <cstdio>
#include <cstdlib>
#include <cstdint>
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
static int best1(W v,int len,int cap){ // min missing after one mechanic move
    int b=miss(v,len); if(b==0) return 0; if(b>n+cap) return b-n; // lower bound b-n; good enough as a cost when large
    for(int k=0;k<len;k++){
        if(k>0 && ((v>>k)&1)==((v>>(k-1))&1)) continue;
        int a=miss(loss(v,k),len-1); if(a<b) b=a; a=miss(dbl(v,k),len+1); if(a<b) b=a; if(b==0) return 0;
    }
    return b;
}
static int cost(W w,int L,int stop){
    int c=best1(w,L,0);
    for(int k=0;k<L && c<stop;k++){
        if(k>0 && ((w>>k)&1)==((w>>(k-1))&1)) continue;
        c+=best1(loss(w,k),L-1,0); c+=best1(dbl(w,k),L+1,0);
    }
    return c;
}
int main(int argc,char**argv){
    n=atoi(argv[1]); int L=atoi(argv[2]); unsigned seed=atoi(argv[3]); long iters=atol(argv[4]); double T0=argc>5?atof(argv[5]):1.0;
    FULL=(n==6)?~0ULL:((1ULL<<(1<<n))-1); WM=(1u<<n)-1;
    std::mt19937_64 rng(seed);
    W w=0; for(int i=0;i<L;i++) if(rng()&1) w|=((W)1)<<i;
    int c=cost(w,L,1<<30); int bestc=c;
    for(long it=0;it<iters && c>0;it++){
        double T=T0*(1.0-(double)it/iters)+0.05;
        W v=w; int r=rng()%3;
        if(r==0){ v^=((W)1)<<(rng()%L); }
        else if(r==1){ int i=rng()%L,j=(i+1)%L; int bi=(int)((v>>i)&1),bj=(int)((v>>j)&1); if(bi==bj) continue; v^=(((W)1)<<i)|(((W)1)<<j); }
        else { int i=rng()%L; v=loss(v,i); int j=rng()%(L-1); v=dbl(v,j); } // move a cell: delete one, double another
        int c2=cost(v,L,1<<30);
        if(c2<=c || std::uniform_real_distribution<double>(0,1)(rng)<exp((c-c2)/T)){ w=v; c=c2; if(c<bestc){bestc=c; fprintf(stderr,"it %ld cost %d\n",it,c);} }
    }
    printf("n=%d L=%d seed=%u final cost %d : ",n,L,seed,c); for(int i=0;i<L;i++) putchar('0'+(int)((w>>i)&1)); putchar('\n');
    return c==0?0:2;
}
