// Referee: literal brute force. Lists every repairable necklace (min rotation) of length L for order n.
#include <cstdio>
#include <cstdint>
#include <cstdlib>
typedef uint64_t u64;
static int n;
static bool universal(u64 v,int m){ // bit i = cell i
    if(m < (1<<n)) return false;
    uint32_t seen=0; int cnt=0; uint32_t mask=(1u<<n)-1;
    uint32_t w=0; for(int j=0;j<n;j++) w=(w<<1)|((v>>(j%m))&1);
    for(int i=0;i<m;i++){ if(!((seen>>w)&1)){seen|=1u<<w;cnt++;} w=((w<<1)|((v>>((i+n)%m))&1))&mask; }
    return cnt==(1<<n);
}
static u64 loss(u64 v,int m,int k){ u64 lo=v&((1ULL<<k)-1); u64 hi=v>>(k+1); return lo|(hi<<k); }
static u64 dbl(u64 v,int m,int k){ u64 lo=v&((1ULL<<(k+1))-1); u64 hi=v>>k; return lo|(hi<<(k+1)); }
static bool mech(u64 v,int m){
    if(universal(v,m)) return true;
    if(m>=2) for(int k=0;k<m;k++) if(universal(loss(v,m,k),m-1)) return true;
    for(int k=0;k<m;k++) if(universal(dbl(v,m,k),m+1)) return true;
    return false;
}
static bool repairable(u64 v,int m){
    if(!mech(v,m)) return false;
    if(m>=2) for(int k=0;k<m;k++) if(!mech(loss(v,m,k),m-1)) return false;
    for(int k=0;k<m;k++) if(!mech(dbl(v,m,k),m+1)) return false;
    return true;
}
int main(int argc,char**argv){
    n=atoi(argv[1]); int L=atoi(argv[2]); u64 full=(1ULL<<L)-1; long cnt=0,nonu=0;
    for(u64 v=0;v<=full;v++){
        bool canon=true; u64 r=v;
        for(int i=1;i<L&&canon;i++){ r=((r>>1)|((r&1)<<(L-1))); if(r<v) canon=false; }
        if(!canon) continue;
        if(repairable(v,L)){ cnt++; bool u=universal(v,L); if(!u) nonu++;
            for(int i=0;i<L;i++) putchar('0'+((v>>i)&1)); printf(" %c\n",u?'U':'N'); }
    }
    fprintf(stderr,"n=%d L=%d repairable necklaces=%ld nonuniversal=%ld\n",n,L,cnt,nonu);
}
