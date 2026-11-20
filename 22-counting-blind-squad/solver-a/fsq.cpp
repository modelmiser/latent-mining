// Counting-blind firing squad. States: 0=Q 1=G 2=F 3.. unnamed.
// modes: brute s   (enumerate all s^(3(s-1)-1) rules)
//        dfs s cap (lazy assignment with unnamed-state symmetry breaking)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
static int S;
static int d[16][3];           // -1 = undefined
static int needx, needk;
// one step. returns 0 ok (no F), 1 all F, 2 mixed F (fail), 3 undefined entry needed
static inline int step(const unsigned char*a, unsigned char*b, int n){
    int nf=0;
    for(int i=0;i<n;i++){
        int x=a[i],k=0;
        if(i>0 && a[i-1]!=x)k++;
        if(i<n-1 && a[i+1]!=x)k++;
        int v=d[x][k];
        if(v<0){needx=x;needk=k;return 3;}
        b[i]=v; if(v==2)nf++;
    }
    if(nf==0)return 0;
    return nf==n?1:2;
}
// returns firing time t>=1 if synchronises, 0 if fails, -1 if needs undefined entry
static ll run(int n){
    static unsigned char cur[512],nxt[512],tort[512];
    cur[0]=1; for(int i=1;i<n;i++)cur[i]=0;
    memcpy(tort,cur,n);
    ll t=0,power=1,lam=0;
    for(;;){
        int r=step(cur,nxt,n); t++;
        if(r==1)return t;
        if(r==2)return 0;
        if(r==3)return -1;
        memcpy(cur,nxt,n);
        lam++;
        if(memcmp(cur,tort,n)==0)return 0;   // cycle without F: never fires
        if(lam==power){memcpy(tort,cur,n);power*=2;lam=0;}
    }
}
static int CAP=200;
static ll hist[1024];
static int best=0; static ll nbest=0;
static void printrule(FILE*f){
    for(int x=0;x<S;x++){ if(x==2)continue; for(int k=0;k<3;k++)fprintf(f,"%d ",d[x][k]); }
}
static void report(int L, const char*tag){ // rule synchronises 2..L
    if(L>best){best=L;nbest=0;}
    if(L==best){
        nbest++;
        if(nbest<=40){
            printf("%s L=%d rule[Q0 Q1 Q2 G0 G1 G2 u..]: ",tag,L); printrule(stdout);
            printf(" times:");
            for(int n=2;n<=L;n++)printf(" %lld",run(n));
            printf("\n"); fflush(stdout);
        }
    }
}
static ll leaves=0;
static void dfs(int n0,int maxu){ // all n<n0 synchronised; maxu = largest unnamed state used as value (2 if none)
    for(int n=n0;;n++){
        if(n>CAP){printf("CAP REACHED: rule synchronises all n<=%d: ",CAP);printrule(stdout);printf("\n");report(CAP,"CAP");leaves++;return;}
        ll r=run(n);
        if(r>0)continue;
        if(r==0){leaves++;hist[n-1]++;report(n-1,"dfs");return;}
        int x=needx,k=needk;
        int lim=min(S-1,maxu+1);
        for(int v=0;v<=lim;v++){
            d[x][k]=v;
            dfs(n,max(maxu,v));
        }
        d[x][k]=-1;
        return;
    }
}
int main(int argc,char**argv){
    S=atoi(argv[2]);
    if(!strcmp(argv[1],"dfs")){
        if(argc>3)CAP=atoi(argv[3]);
        for(int x=0;x<16;x++)for(int k=0;k<3;k++)d[x][k]=-1;
        d[0][0]=0;
        dfs(2,2);
        printf("DFS s=%d best L=%d classes_at_best=%lld leaves=%lld\n",S,best,nbest,leaves);
        for(int i=1;i<1024;i++)if(hist[i])printf("  partial-rule leaves with L_rule=%d: %lld\n",i,hist[i]);
    } else {
        vector<pair<int,int>> fr;
        for(int x=0;x<S;x++){ if(x==2)continue; for(int k=0;k<3;k++) if(!(x==0&&k==0)) fr.push_back({x,k}); }
        d[0][0]=0;
        int m=fr.size(); ll tot=1; for(int i=0;i<m;i++)tot*=S;
        for(ll idx=0;idx<tot;idx++){
            ll z=idx; for(int i=0;i<m;i++){d[fr[i].first][fr[i].second]=z%S;z/=S;}
            int n=2; while(n<=CAP && run(n)>0)n++;
            hist[n-1]++; report(n-1,"brute");
        }
        printf("BRUTE s=%d rules=%lld best L=%d rules_at_best=%lld\n",S,tot,best,nbest);
        for(int i=1;i<1024;i++)if(hist[i])printf("  rules with L_rule=%d: %lld\n",i,hist[i]);
    }
}
