// Lazy exhaustive search for D(r): the tree is decided node by node only when the orbit of the start state reaches
// an undecided node. Start state s0 = canonical state of weight w (ones on the top w positions), required to lie on
// the cycle and to have minimum weight on it (every cycle has a min-weight state; relabelling positions makes it canonical).
// Symmetry pruning: among positions never yet used in the tree, those with equal s0-bit are interchangeable -> try only the lowest.
// A branch ends when the orbit returns to s0 (cycle length recorded), hits another visited state (s0 not on a cycle),
// or reaches weight < w.  usage: lazy r [wlo whi] [nodelimit]
#include <bits/stdc++.h>
using namespace std;
int r,d,w; unsigned s0; 
int kind[70000]; // 0 undecided, 1 internal, 2 leaf
int val[70000];  // index, or write index (-1 none)
int usecnt[32];
vector<char> vis; long long nodes=0, limit=-1; int best=0; bool aborted=false;
int bestkind[64],bestval[64];
vector<unsigned> orbit;
void dfs(unsigned s,int len){
    if(aborted) return;
    size_t mark=orbit.size();
    while(true){
        int n=1; unsigned P=0; int path[8],pl=0;
        while(kind[n]==1){int i=val[n]; P|=1u<<i; path[pl++]=i; n=2*n+((s>>i)&1);}
        if(kind[n]==0){
            nodes++; if(limit>0&&nodes>limit){aborted=true;break;}
            // leaf options
            kind[n]=2;
            for(int q=-1;q<pl;q++){val[n]=q<0?-1:path[q]; dfs(s,len);}
            if(pl<r){
                kind[n]=1; bool f0=false,f1=false;
                for(int i=0;i<d;i++){ if(P>>i&1) continue;
                    if(!usecnt[i]){ bool c=(s0>>i)&1; if(c){if(f1)continue; f1=true;} else {if(f0)continue; f0=true;} }
                    val[n]=i; usecnt[i]++; dfs(s,len); usecnt[i]--; }
            }
            kind[n]=0; break;
        }
        unsigned t=s&~P; if(val[n]>=0) t|=1u<<val[n];
        len++;
        if(t==s0){ if(len>best){best=len; for(int k=1;k<(1<<(r+1));k++){bestkind[k]=kind[k];bestval[k]=val[k];} } break; }
        if(vis[t]||__builtin_popcount(t)<w) break;
        vis[t]=1; orbit.push_back(t); s=t;
    }
    while(orbit.size()>mark){vis[orbit.back()]=0;orbit.pop_back();}
}
string show(int n){ if(bestkind[n]==1) return "("+to_string(bestval[n]+1)+" "+show(2*n)+" "+show(2*n+1)+")";
    if(bestkind[n]==2&&bestval[n]>=0) return "["+to_string(bestval[n]+1)+"]"; return "[]"; } // undecided -> [] (never reached on the cycle)
int main(int argc,char**argv){
    r=atoi(argv[1]); d=(1<<r)-1; int wlo=argc>2?atoi(argv[2]):0, whi=argc>3?atoi(argv[3]):d; if(argc>4) limit=atoll(argv[4]);
    vis.assign(1u<<d,0);
    for(w=wlo;w<=whi;w++){ s0=0; for(int i=0;i<w;i++) s0|=1u<<(d-1-i);
        nodes=0; best=0; aborted=false; vis[s0]=1; dfs(s0,0); vis[s0]=0;
        printf("r=%d w=%d best_cycle=%d nodes=%lld %s",r,w,best,nodes,aborted?"ABORTED(incomplete)":"complete");
        if(best) printf(" witness %s start_weight_%d_top_positions",show(1).c_str(),w); printf("\n"); fflush(stdout);
    }
}
