// Referee brute force, straight from the definition.
// mode "small r": r=1,2: every tree over indices 0..d-1, no symmetry, full functional graph.
// mode "r3 part nparts": r=3, root index fixed to 0 (relabelling), 0-subtree is a leaf or has top index 1 (relabelling of 1..6);
//   histogram is re-weighted (x6 for non-leaf 0-subtrees) to count all trees with root index 0.
#include <bits/stdc++.h>
using namespace std;
struct Node{int kind; int v; int c0,c1;}; // kind 1 internal (v=index), 2 leaf (v=-1 or index)
vector<Node> pool;
// enumerate all subtrees hanging below a path
void gen(vector<int>&path,int depthLeft,int d,vector<int>&out){
    { pool.push_back({2,-1,-1,-1}); out.push_back(pool.size()-1); }
    for(int p:path){ pool.push_back({2,p,-1,-1}); out.push_back(pool.size()-1); }
    if(depthLeft==0) return;
    for(int i=0;i<d;i++){
        if(find(path.begin(),path.end(),i)!=path.end()) continue;
        path.push_back(i);
        vector<int> ch; gen(path,depthLeft-1,d,ch);
        path.pop_back();
        for(int a:ch) for(int b:ch){ pool.push_back({1,i,a,b}); out.push_back(pool.size()-1); }
    }
}
// one step from node t on state s, with P the mask already read; returns new state, sets empty flag
unsigned stepFrom(int t,unsigned s,unsigned P,bool&empty){
    while(pool[t].kind==1){int i=pool[t].v; P|=1u<<i; t=((s>>i)&1)?pool[t].c1:pool[t].c0;}
    unsigned o=s&~P; empty=pool[t].v<0; if(!empty) o|=1u<<pool[t].v; return o;
}
unsigned maskOf(int t){ if(pool[t].kind!=1) return 0; return (1u<<pool[t].v)|maskOf(pool[t].c0)|maskOf(pool[t].c1); }
int d; 
long long hist[200]; int bestW[40],bestM[40]; int maxEmptyOnCycle=0; long long cyclesWithManyEmpty=0;
int mx_r1=0,mx_run0=0,mx_r0=0;
int f[128]; bool emp[128]; int seen[128];
void analyse(int nidx,long long weight){
    int n=1<<d; for(int s=0;s<n;s++) seen[s]=0;
    int longest=0;
    for(int s=0;s<n;s++){ if(seen[s]) continue; int x=s; while(!seen[x]){seen[x]=s+1; x=f[x];}
        if(seen[x]!=s+1) continue;
        int len=0,minw=99,ne=0,r1=0,r0=0,run=0,maxrun=0; int y=x;
        // start the walk just after a root=1 state if any, so runs are not split
        int st=x; { int z=x; do{ if(z&1){st=f[z];break;} z=f[z]; }while(z!=x); }
        y=st; do{ len++; minw=min(minw,__builtin_popcount(y)); if(emp[y]) ne++; if(y&1){r1++; run=0;} else {r0++; run++; maxrun=max(maxrun,run);} y=f[y]; }while(y!=st);
        longest=max(longest,len);
        for(int w=0;w<=minw;w++) ; 
        bestW[minw]=max(bestW[minw],len); bestM[nidx]=max(bestM[nidx],len);
        if(len>=2){ if(ne>maxEmptyOnCycle) maxEmptyOnCycle=ne; if(ne>=2) cyclesWithManyEmpty++; }
        if(r1>0&&r0>0){ mx_r1=max(mx_r1,r1); mx_run0=max(mx_run0,maxrun); mx_r0=max(mx_r0,r0); }
    }
    hist[longest]+=weight;
}
int main(int argc,char**argv){
    string mode=argv[1];
    if(mode=="small"){
        int r=atoi(argv[2]); d=(1<<r)-1; vector<int> path,all; gen(path,r,d,all);
        for(int t:all){ for(int s=0;s<(1<<d);s++){bool e; f[s]=stepFrom(t,s,0,e); emp[s]=e;} analyse(__builtin_popcount(maskOf(t)),1); }
        printf("r=%d trees=%zu\n",r,all.size());
    } else {
        int part=atoi(argv[2]),np=atoi(argv[3]); d=7;
        vector<int> path={0},sub; gen(path,2,7,sub);
        int S=sub.size();
        vector<array<unsigned char,64>> g(S); vector<unsigned long long> ge(S); vector<unsigned> mk(S); vector<int> top(S);
        for(int k=0;k<S;k++){ ge[k]=0; mk[k]=maskOf(sub[k]); top[k]=pool[sub[k]].kind==1?pool[sub[k]].v:-1;
            // the subtree never reads bit 0 itself; root bit is erased by the root read (P=1)
            for(int x=0;x<64;x++){bool e; g[k][x]=stepFrom(sub[k],x<<1,1u,e); if(e) ge[k]|=1ull<<x;} }
        long long pairs=0; int cnt=0;
        for(int a=0;a<S;a++){ if(top[a]!=-1&&top[a]!=1) continue; if((cnt++)%np!=part) continue;
            long long wt=top[a]==-1?1:6;
            for(int x=0;x<64;x++){ f[x<<1]=g[a][x]; emp[x<<1]=(ge[a]>>x)&1; }
            for(int b=0;b<S;b++){ for(int x=0;x<64;x++){ f[(x<<1)|1]=g[b][x]; emp[(x<<1)|1]=(ge[b]>>x)&1; }
                analyse(1+__builtin_popcount(mk[a]|mk[b]),wt); pairs++; } }
        printf("r=3 part %d/%d subtrees=%d pairs=%lld\n",part,np,S,pairs);
    }
    printf("hist"); for(int i=0;i<200;i++) if(hist[i]) printf(" %d:%lld",i,hist[i]); printf("\n");
    printf("bestW(min weight exactly w)"); for(int w=0;w<=d;w++) printf(" %d",bestW[w]); printf("\n");
    printf("bestM(exactly m indices in tree)"); for(int m=0;m<=d;m++) printf(" %d",bestM[m]); printf("\n");
    printf("max empty-leaf steps on a cycle of length>=2: %d (cycles with >=2: %lld)\n",maxEmptyOnCycle,cyclesWithManyEmpty);
    printf("mixed cycles: max root=1 states %d, max run of root=0 %d, max root=0 states %d\n",mx_r1,mx_run0,mx_r0);
}
