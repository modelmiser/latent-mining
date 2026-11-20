// For every depth-3 machine (root=pos 1, X canonical): every cycle, length N, and the multiset of visit counts
// of EMPTY leaves on the cycle. Records table ok[N][j] = some subset of empty leaves is visited j times in an N-cycle
// (j=0 always). Then evaluates the disjoint-product construction at depth 4:
//   period = lcm(j,k) * (N/j + M/k), T0: N-cycle with j diverted (write root) steps, T1: M-cycle with M-k diverted steps.
#include <bits/stdc++.h>
using namespace std;
typedef array<unsigned char,128> Act;
struct Sub{Act a; Act e; string txt;}; // e[x]= path mask if leaf reached is empty else 0
static void leaves(int P, const vector<int>&path, vector<Sub>&out){
    for(int w=-1;w<(int)path.size();w++){
        Sub s; for(int x=0;x<128;x++){int y=x&~P; if(w>=0) y|=1<<path[w]; s.a[x]=y; s.e[x]= w<0?P:0;}
        s.txt = w<0? "[]" : "["+to_string(path[w]+1)+"]"; out.push_back(s);
    }
}
static void gen(int P, vector<int>path, int dleft, const string&node, bool canon, vector<Sub>&out){
    leaves(P,path,out);
    if(!dleft) return;
    for(int i=0;i<7;i++){ if(P>>i&1) continue;
        if(canon){ if(node==""&&i!=1) continue; if(node=="0"&&i!=2) continue; if(node=="1"&&i!=2&&i!=3) continue; }
        vector<Sub> a,b; vector<int> p2=path; p2.push_back(i);
        gen(P|1<<i,p2,dleft-1,node+"0",canon,a); gen(P|1<<i,p2,dleft-1,node+"1",canon,b);
        for(auto&u:a)for(auto&v:b){ Sub s; for(int x=0;x<128;x++){ s.a[x]=(x>>i&1)?v.a[x]:u.a[x]; s.e[x]=(x>>i&1)?v.e[x]:u.e[x];}
            s.txt="("+to_string(i+1)+" "+u.txt+" "+v.txt+")"; out.push_back(s);}
    }
}
static bool ok[130][130]; static string wit[130][130];
int main(){
    vector<Sub> Y,X; gen(1,{0},2,"",false,Y); gen(1,{0},2,"",true,X);
    unsigned char f[128],e[128]; int stamp[128],idx[128],cur=0; memset(stamp,0,sizeof stamp);
    for(auto&xs:X) for(auto&ys:Y){
        for(int s=0;s<128;s+=2){f[s]=xs.a[s];f[s+1]=ys.a[s+1];e[s]=xs.e[s];e[s+1]=ys.e[s+1];}
        int base=++cur;
        for(int s0=0;s0<128;s0++){ if(stamp[s0]>=base) continue; int id=++cur; int s=s0,c=0;
            while(stamp[s]<base){stamp[s]=id;idx[s]=c++;s=f[s];}
            if(stamp[s]==id){int L=c-idx[s]; if(L<2) continue;
                map<int,int> cnt; int t=s; for(int q=0;q<L;q++){ if(e[t]) cnt[e[t]*128+(t&e[t])]++; t=f[t]; }
                vector<int> v; for(auto&p:cnt) v.push_back(p.second);
                for(int m=0;m<(1<<v.size());m++){int j=0; for(size_t b=0;b<v.size();b++) if(m>>b&1) j+=v[b];
                    if(!ok[L][j]){ok[L][j]=1; char buf[64]; sprintf(buf," start=%d mask=%d",s,m); wit[L][j]="(1 "+xs.txt+" "+ys.txt+")"+buf; }}
            }
        }
        if(cur>2000000000){cur=0;memset(stamp,0,sizeof stamp);}
    }
    for(int N=2;N<=42;N++){ bool any=0; for(int j=0;j<=N;j++) if(ok[N][j]){ if(!any){printf("N=%d j:",N);any=1;} printf(" %d",j);} if(any)printf("\n"); }
    long long best=0; int bN=0,bj=0,bM=0,bk=0;
    vector<array<long long,5>> all;
    for(int N=2;N<=42;N++)for(int j=1;j<=N;j++) if(ok[N][j])
      for(int M=1;M<=42;M++)for(int k=1;k<=M;k++) if((M==1&&k==1)||(M>=2&&ok[M][M-k])){
        long long l=1LL*j*k/__gcd(j,k); long long p=l*N/j+l*M/k; all.push_back({p,N,j,M,k});
        if(p>best){best=p;bN=N;bj=j;bM=M;bk=k;} }
    sort(all.rbegin(),all.rend());
    for(int i=0;i<15&&i<(int)all.size();i++) printf("period %lld N=%lld j=%lld M=%lld k=%lld\n",all[i][0],all[i][1],all[i][2],all[i][3],all[i][4]);
    printf("T0 witness (N=%d,j=%d): %s\nT1 witness (M=%d,stays=%d): %s\n",bN,bj,wit[bN][bj].c_str(),bM,bM-bk,wit[bM][bM-bk].c_str());
}
