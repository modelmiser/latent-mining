// Exhaustive enumeration of ALL rules on s states (no pruning, no symmetry breaking).
// States 0=Q 1=G 2=F 3..=unnamed. Non-firing detected exactly by Brent cycle detection.
// Output: L(s), histogram of longest synchronised prefix, and per-n statistics over ALL rules:
//   number of rules synchronising that single n, and min/max firing time (n <= NSINGLE).
// usage: brute S NMAX NSINGLE
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
typedef unsigned char u8;
int s; u8 d[8][3];
// returns firing time or -1
static long run(int n){
    u8 a[64], b[64], sv[64];
    a[0]=1; for(int i=1;i<n;i++)a[i]=0;
    memcpy(sv,a,n); long power=1, lam=0, t=0;
    for(;;){
        int nf=0;
        for(int i=0;i<n;i++){
            int x=a[i],k=0;
            if(i>0&&a[i-1]!=x)k++;
            if(i<n-1&&a[i+1]!=x)k++;
            b[i]=d[x][k]; nf+=(b[i]==2);
        }
        t++;
        if(nf==n)return t;
        if(nf)return -1;
        memcpy(a,b,n);
        lam++;
        if(!memcmp(a,sv,n))return -1;
        if(lam==power){memcpy(sv,a,n);power*=2;lam=0;}
    }
}
int main(int argc,char**argv){
    s=atoi(argv[1]); int NMAX=atoi(argv[2]); int NS=atoi(argv[3]); int DUMP=argc>4?atoi(argv[4]):0; // DUMP>0: print every rule with longest prefix >= DUMP (to stderr)
    int ne=3*(s-1)-1; // entries in order: (Q,1),(Q,2),(G,0..2),(3,0..2),...
    std::vector<int> ex,ek;
    for(int x=0;x<s;x++){ if(x==2)continue; for(int k=0;k<3;k++){ if(x==0&&k==0)continue; ex.push_back(x);ek.push_back(k);} }
    long long total=1; for(int i=0;i<ne;i++)total*=s;
    std::vector<long long> hist(NMAX+2,0), cnt(NS+1,0); std::vector<long> mn(NS+1,-1), mx(NS+1,-1);
    int best=1; long long nbest=0; long long bestidx=-1;
    for(long long idx=0;idx<total;idx++){
        long long r=idx; d[0][0]=0;
        for(int i=ne-1;i>=0;i--){ d[ex[i]][ek[i]]=r%s; r/=s; }
        int N=1; bool alive=true;
        for(int n=2;n<=NMAX;n++){
            if(!alive && n>NS)break;
            long t=run(n);
            if(n<=NS && t>=0){cnt[n]++; if(mn[n]<0||t<mn[n])mn[n]=t; if(t>mx[n])mx[n]=t;}
            if(alive){ if(t>=0)N=n; else alive=false; }
        }
        hist[N]++;
        if(DUMP&&N>=DUMP){ for(int i=0;i<ne;i++)fputc("QGFABCDE"[d[ex[i]][ek[i]]],stderr); fputc('\n',stderr);} 
        if(N>best){best=N;nbest=0;bestidx=idx;}
        if(N==best)nbest++;
    }
    printf("s=%d rules=%lld L=%d%s  rules attaining it: %lld\n",s,total,best,best==NMAX?" (HIT NMAX)":"",nbest);
    printf("histogram longest prefix N (1 = fails n=2):"); for(int i=1;i<=NMAX;i++)if(hist[i])printf(" %d:%lld",i,hist[i]); printf("\n");
    for(int n=2;n<=NS;n++)printf("single n=%d: rules synchronising it=%lld min_t=%ld max_t=%ld (2n-2=%d)\n",n,cnt[n],mn[n],mx[n],2*n-2);
    // print first best rule
    long long r=bestidx; for(int i=ne-1;i>=0;i--){ d[ex[i]][ek[i]]=r%s; r/=s; }
    const char*nm="QGFABCDE";
    printf("first best rule (idx %lld):\n",bestidx);
    for(int i=0;i<ne;i++)printf("%c %d %c\n",nm[ex[i]],ek[i],nm[d[ex[i]][ek[i]]]);
    printf("TIMES\n"); for(int n=2;n<=best+1;n++){long t=run(n); if(t>=0)printf("%d %ld\n",n,t); else printf("%d FAIL\n",n);}
    return 0;
}
