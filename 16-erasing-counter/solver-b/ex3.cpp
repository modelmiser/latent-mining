// Exhaustive D(3). Root index fixed to position 1 (bit 0) WLOG (relabelling positions).
// mode "canon": 0-subtree X restricted by relabelling of positions 2..7 (X root in {2}, X.0child in {3}, X.1child in {3,4})
// mode "full" : X ranges over all 41336 subtrees (cross-check of the symmetry pruning). args: mode lo hi (X index range fraction k of n)
#include <bits/stdc++.h>
using namespace std;
typedef array<unsigned char,128> Act;
struct Sub{Act a; string txt;};
// leaves for path mask P, path list
static void leaves(int P, const vector<int>&path, vector<Sub>&out){
    for(int w=-1;w<(int)path.size();w++){
        Sub s; for(int x=0;x<128;x++){int y=x&~P; if(w>=0) y|=1<<path[w]; s.a[x]=y;}
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
        for(auto&u:a)for(auto&v:b){ Sub s; for(int x=0;x<128;x++) s.a[x]=(x>>i&1)?v.a[x]:u.a[x];
            s.txt="("+to_string(i+1)+" "+u.txt+" "+v.txt+")"; out.push_back(s);}
    }
}
int main(int argc,char**argv){
    string mode=argv[1]; int k=argc>2?atoi(argv[2]):0, n=argc>3?atoi(argv[3]):1;
    vector<Sub> Y,X; gen(1,{0},2,"",false,Y); 
    if(mode=="canon") gen(1,{0},2,"",true,X); else X=Y;
    fprintf(stderr,"|X|=%zu |Y|=%zu\n",X.size(),Y.size());
    int best=0; string bt; vector<long long> hist(129,0);
    unsigned char f[128]; int stamp[128]; int idx[128]; int cur=0; memset(stamp,0,sizeof stamp);
    for(size_t xi=k; xi<X.size(); xi+=n){ const Act&A=X[xi].a;
        for(size_t yi=0; yi<Y.size(); yi++){ const Act&B=Y[yi].a;
            for(int s=0;s<128;s+=2){f[s]=A[s];f[s+1]=B[s+1];}
            int mx=1; int base=++cur; // stamps: per start we use cur increments
            for(int s0=0;s0<128;s0++){ if(stamp[s0]>=base) continue; int id=++cur; int s=s0,c=0;
                while(stamp[s]<base){stamp[s]=id;idx[s]=c++;s=f[s];}
                if(stamp[s]==id){int L=c-idx[s]; if(L>mx)mx=L;}
            }
            if(cur>2000000000){cur=0;memset(stamp,0,sizeof stamp);}
            hist[mx]++;
            if(mx>best){best=mx; bt="(1 "+X[xi].txt+" "+Y[yi].txt+")";}
        }
    }
    printf("mode %s part %d/%d best %d\nwitness %s\nhist",mode.c_str(),k,n,best,bt.c_str());
    for(int i=1;i<=128;i++) if(hist[i]) printf(" %d:%lld",i,hist[i]); printf("\n");
}
