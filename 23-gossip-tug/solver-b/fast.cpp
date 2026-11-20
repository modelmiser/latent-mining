// Minimax with memo on canonical forms (min over permutations consistent with a
// refinement colouring -- an isomorphism invariant ordering, so safe).
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int n; u64 FULL; int ROUNDS=2;
static inline int row(u64 s,int i){return (s>>(8*i))&0xff;}
static u64 canon(u64 s){
    int K[8]; for(int i=0;i<n;i++)K[i]=row(s,i);
    // invariant colour: (|K_i|, #who know i), refined two rounds by sorted neighbour colours
    u64 col[8];
    for(int i=0;i<n;i++){int c=0;for(int j=0;j<n;j++)c+=(K[j]>>i)&1; col[i]=__builtin_popcount(K[i])*16+c;}
    for(int r=0;r<ROUNDS;r++){
        u64 nc[8];
        for(int i=0;i<n;i++){
            u64 so=0,si=0,sb=0;
            for(int j=0;j<n;j++){ if(j==i)continue;
                bool o=(K[i]>>j)&1, in=(K[j]>>i)&1;
                u64 h=(col[j]+1)*0x9E3779B97F4A7C15ULL; h^=h>>29; h*=0xBF58476D1CE4E5B9ULL; h^=h>>32;
                if(o&&in)sb+=h; else if(o)so+=h; else if(in)si+=h; }
            u64 h=col[i]*0x94D049BB133111EBULL+so*3+si*7+sb*13; h^=h>>31; nc[i]=h;
        }
        memcpy(col,nc,sizeof nc);
    }
    int ord[8]; for(int i=0;i<n;i++)ord[i]=i;
    sort(ord,ord+n,[&](int a,int b){return col[a]<col[b];});
    // groups of equal colour
    u64 best=~0ULL;
    int perm[8]; memcpy(perm,ord,sizeof ord);
    // enumerate permutations within groups
    vector<pair<int,int>> gr; for(int i=0;i<n;){int j=i;while(j<n&&col[ord[j]]==col[ord[i]])j++; gr.push_back({i,j}); i=j;}
    for(auto&g:gr) sort(perm+g.first,perm+g.second);
    while(true){
        int pos[8]; for(int i=0;i<n;i++)pos[perm[i]]=i;
        u64 t=0;
        for(int i=0;i<n;i++){int k=K[perm[i]],m=0; while(k){int j=__builtin_ctz(k);k&=k-1;m|=1<<pos[j];} t|=(u64)m<<(8*i);}
        if(t<best)best=t;
        size_t gi=0; for(;gi<gr.size();gi++){ if(next_permutation(perm+gr[gi].first,perm+gr[gi].second))break; }
        if(gi==gr.size())break;
    }
    return best;
}
unordered_map<u64,signed char> memo;
bool NOCANON=false;
int val(u64 s){
    if(s==FULL)return 0;
    u64 c=NOCANON?s:canon(s);
    auto it=memo.find(c); if(it!=memo.end())return it->second;
    int K[8]; for(int i=0;i<n;i++)K[i]=row(c,i);
    int best=-1;
    for(int a=0;a<n;a++){
        int mn=1000;
        for(int b=0;b<n;b++){ if(b==a)continue;
            if(((K[b]>>a)&1)&&((K[a]>>b)&1))continue;
            int u=K[a]|K[b];
            u64 t=c; t&=~((u64)0xff<<(8*a)); t&=~((u64)0xff<<(8*b)); t|=(u64)u<<(8*a); t|=(u64)u<<(8*b);
            int v=1+val(t); if(v<mn)mn=v;
        }
        if(mn<1000&&mn>best)best=mn;
    }
    memo[c]=best; return best;
}
int main(int argc,char**argv){
    n=atoi(argv[1]); if(argc>2)NOCANON=true; if(getenv("ROUNDS"))ROUNDS=atoi(getenv("ROUNDS"));
    FULL=0; u64 s=0; for(int i=0;i<n;i++){FULL|=(u64)((1<<n)-1)<<(8*i); s|=(u64)(1<<i)<<(8*i);}
    int r=val(s);
    printf("n=%d A=%d states=%zu canon=%d rounds=%d\n",n,r,memo.size(),!NOCANON,ROUNDS);
    if(getenv("PV")){ u64 c=s; 
      auto show=[&](u64 c){for(int i=0;i<n;i++){for(int j=0;j<n;j++)if(row(c,i)>>j&1)printf("%d",j+1);printf(" ");}};
      auto child=[&](u64 c,int a,int b){int u=row(c,a)|row(c,b); u64 t=c; t&=~((u64)0xff<<(8*a)); t&=~((u64)0xff<<(8*b)); t|=(u64)u<<(8*a); t|=(u64)u<<(8*b);return t;};
      auto legal=[&](u64 c,int a,int b){return a!=b&&!(((row(c,b)>>a)&1)&&((row(c,a)>>b)&1));};
      while(c!=FULL){ int v=val(c); printf("val=%d [",v); show(c); printf("] a:(replies b=val) ");
        int ba=-1,bb=-1; 
        for(int a=0;a<n;a++){int mn=1000,mb=-1; for(int b=0;b<n;b++)if(legal(c,a,b)){int w=1+val(child(c,a,b)); if(w<mn){mn=w;mb=b;}}
          if(mn==v){ if(ba<0){ba=a;bb=mb;} printf(" %d:(",a+1); for(int b=0;b<n;b++)if(legal(c,a,b))printf("%d=%d ",b+1,1+val(child(c,a,b))); printf(")"); } }
        printf("\n"); c=child(c,ba,bb); } }
    if(getenv("DUMP")){FILE*f=fopen(getenv("DUMP"),"w"); vector<pair<u64,int>> v(memo.begin(),memo.end()); sort(v.begin(),v.end());
      fprintf(f,"# n=%d ; each line: value then K_1..K_n as hex bitmasks (bit j-1 set = knows secret j); value = calls remaining under optimal play; terminal state omitted (value 0)\n",n);
      for(auto&p:v){fprintf(f,"%d",p.second);for(int i=0;i<n;i++)fprintf(f," %x",row(p.first,i));fprintf(f,"\n");} fclose(f);}
}
