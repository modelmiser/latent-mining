import subprocess,sys
# star with k leaves from agent 0 (k calls made), n agents: value of position + k
def star(n,k):
    K=[1<<i for i in range(n)]
    for j in range(1,k+1):
        u=K[0]|K[j]; K[0]=u; K[j]=u
    return K
for n in range(4,int(sys.argv[1])+1):
    out=[]
    for k in range(1,n):
        K=star(n,k)
        r=subprocess.run("ulimit -v 6000000; KS=%s nice -n 15 taskset -c 11 timeout 3000 ./g full %d"%(",".join("%x"%x for x in K),n),shell=True,capture_output=True,text=True).stdout
        v=int(r.split("value=")[1].split()[0]); out.append((k,k+v))
    print(n,out,flush=True)
