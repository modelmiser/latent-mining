// Referee lazy search. Tree nodes are decided when the orbit of the start state first reaches them.
// Indices are named 0,1,2,... in order of first use; the START value of an index is chosen (0/1) when it is first named.
// States are stored as XOR with the start state, so un-named indices contribute 0 and a closed cycle is diff==0.
// Prunings: (1) naming order (relabelling); (2) start is a minimum-weight state of its cycle -> stop if weight drops below start;
// (3) orbit revisits a non-start state -> start not on a cycle.
// usage: lazy r part nparts splitdepth
#include <bits/stdc++.h>
using namespace std;
int r,d,k=0; unsigned s0=0;
int kind[64],val[64];
vector<unsigned char> vis; vector<unsigned> trail;
long long nodes=0; int best=0; int bestW[40],bestM[40];
int part,nparts,splitdepth; long long splitcnt=0; int depthDec=0;
int bk[64],bv[64]; unsigned bs0;
void go(unsigned s,int len);
inline void sub(unsigned s,int len){
    if(depthDec==splitdepth){ if((splitcnt++)%nparts!=part) return; }
    depthDec++; go(s,len); depthDec--;
}
void go(unsigned s,int len){
    size_t mark=trail.size();
    for(;;){
        int n=1,pl=0,path[8]; unsigned P=0;
        while(kind[n]==1){int i=val[n]; path[pl++]=i; P|=1u<<i; n=2*n+((s>>i)&1);}
        if(kind[n]==0){
            nodes++;
            kind[n]=2; val[n]=-1; sub(s,len);
            for(int q=0;q<pl;q++){val[n]=path[q]; sub(s,len);}
            if(pl<r){
                kind[n]=1;
                for(int i=0;i<k;i++){ if(P>>i&1) continue; val[n]=i; sub(s,len); }
                if(k<d){ val[n]=k; k++; sub(s,len);
                    unsigned b=1u<<(k-1); s0|=b; sub(s|b,len); s0&=~b; k--; }
            }
            kind[n]=0; break;
        }
        unsigned t=s&~P; if(val[n]>=0) t|=1u<<val[n];
        len++;
        unsigned df=t^s0;
        if(df==0){ int w=__builtin_popcount(s0);
            if(len>best){best=len; memcpy(bk,kind,sizeof kind); memcpy(bv,val,sizeof val); bs0=s0;}
            if(len>bestM[k]) bestM[k]=len;
            for(int x=w;x<=w+d-k;x++) if(len>bestW[x]) bestW[x]=len;
            break; }
        if(vis[df]) break;
        if(__builtin_popcount(t)<__builtin_popcount(s0)) break;
        vis[df]=1; trail.push_back(df); s=t;
    }
    while(trail.size()>mark){vis[trail.back()]=0; trail.pop_back();}
}
string show(int n){ if(bk[n]==1) return "("+to_string(bv[n]+1)+" "+show(2*n)+" "+show(2*n+1)+")";
    if(bk[n]==2&&bv[n]>=0) return "["+to_string(bv[n]+1)+"]"; return "[]"; }
int main(int argc,char**argv){
    r=atoi(argv[1]); d=(1<<r)-1; part=argc>2?atoi(argv[2]):0; nparts=argc>3?atoi(argv[3]):1; splitdepth=argc>4?atoi(argv[4]):-1;
    vis.assign(1u<<d,0); vis[0]=1;
    go(0,0);
    printf("r=%d part %d/%d split %d: best=%d nodes=%lld splitsubtrees=%lld COMPLETE\n",r,part,nparts,splitdepth,best,nodes,splitcnt);
    printf("bestW(min weight w, free bits anywhere)"); for(int w=0;w<=d;w++) printf(" %d",bestW[w]); printf("\n");
    printf("bestM(exactly m indices read)"); for(int m=0;m<=d;m++) printf(" %d",bestM[m]); printf("\n");
    if(best){ printf("witness %s start ",show(1).c_str()); for(int i=0;i<d;i++) printf("%d",(bs0>>i)&1); printf("\n"); }
}
