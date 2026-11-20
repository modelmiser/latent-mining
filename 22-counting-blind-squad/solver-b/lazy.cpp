// Lazy backtracking search for L(s).
// Rule entries are assigned only when a simulation first needs them; lengths n=2,3,... are run in order
// under the same partial rule; a branch dies at the first n that fails.  Exhaustive: every total rule
// extends exactly the branches that agree with it, and a branch is cut only when the entries already
// fixed force a failure on some n <= current (partial fire, or exact repeat of a configuration).
// Symmetry breaking (safe): unnamed states are interchangeable, so a new unnamed state may only be
// introduced as the lowest unused label.  Unsafe pruning would bias L DOWNWARD.
// usage: lazy S NMAX [nosym]
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_set>
int s, NMAX, sym=1;
int d[10][3]; int maxused=2;
int best=1; int bestd[10][3]; long long leaves=0, nodes=0, nbestleaves=0; double completions=0;
std::vector<std::unordered_set<std::string>> vis;
static void rec(int n, std::string cfg);
static void start(int n){ std::string c(n,(char)0); c[0]=1; vis[n].clear(); vis[n].insert(c); rec(n,c); vis[n].clear(); }
static void rec(int n, std::string cfg){
    std::vector<std::string> trail;
    std::string nx(n,(char)0);
    for(;;){
        int nf=0, ux=-1, uk=-1;
        for(int i=0;i<n;i++){
            int x=cfg[i],k=0;
            if(i>0&&cfg[i-1]!=x)k++;
            if(i<n-1&&cfg[i+1]!=x)k++;
            int v=d[x][k];
            if(v<0){ux=x;uk=k;break;}
            nx[i]=(char)v; nf+=(v==2);
        }
        if(ux>=0){
            int hi = sym ? (maxused+1<s-1?maxused+1:s-1) : s-1;
            int om=maxused;
            for(int v=0;v<=hi;v++){
                d[ux][uk]=v; if(v>maxused)maxused=v; nodes++;
                rec(n,cfg);
                maxused=om;
            }
            d[ux][uk]=-1;
            break;
        }
        if(nf==n){
            if(n>best){best=n;memcpy(bestd,d,sizeof d);nbestleaves=0;completions=0;
                fprintf(stderr,"best=%d nodes=%lld\n",best,nodes);}
            if(n==best){nbestleaves++; double c=1; for(int x=0;x<s;x++)if(x!=2)for(int k=0;k<3;k++)if(d[x][k]<0)c*=s; completions+=c;}
            if(n<NMAX)start(n+1); else leaves++;
            break;
        }
        if(nf>0){leaves++;break;}
        if(!vis[n].insert(nx).second){leaves++;break;}
        trail.push_back(nx);
        cfg=nx;
    }
    for(auto&t:trail)vis[n].erase(t);
}
int main(int argc,char**argv){
    s=atoi(argv[1]); NMAX=atoi(argv[2]); if(argc>3)sym=0;
    memset(d,-1,sizeof d); d[0][0]=0; vis.resize(NMAX+2);
    start(2);
    printf("s=%d L=%d%s  (sym=%d, search nodes=%lld, dead leaves=%lld, partial rules reaching L=%lld)\n",s,best,best==NMAX?" (HIT NMAX)":"",sym,nodes,leaves,nbestleaves);
    const char*nm="QGFABCDE";
    printf("total rules attaining L implied by those partial rules (meaningful with nosym): %.0f\n",completions);
    printf("one best partial rule ('?' = never consulted on n<=L+1 before failure... fill arbitrarily for n<=L):\n");
    for(int x=0;x<s;x++){ if(x==2)continue; for(int k=0;k<3;k++){ if(x==0&&k==0)continue; printf("%c %d %c\n",nm[x],k,bestd[x][k]<0?'?':nm[bestd[x][k]]); } }
    return 0;
}
