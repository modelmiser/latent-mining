import json, copy
J = json.load(open('../solver-b/wit_S3_T9_L1.json'))
def dump(n, j): json.dump(j, open('mut%d.json' % n, 'w'))
main = lambda j: j["tree"]["moves"]["r2,9"]
# m1: last release on the 'continue' main line (4,1,5) -> (4,1,6): leaf should no longer be lost
m = copy.deepcopy(J); main(m)["moves"]["e"]["moves"]["r3,7"]["moves"]["r2,7"]["rel"] = [[1, 6]]; dump(1, m)
# m2: Y (1,3,7) -> (1,3,6): later releases should break the same-cost promise
m = copy.deepcopy(J); main(m)["rel"] = [[3, 6]]
def relabel(n):
    n["moves"] = {k.replace(",7", ",6") if k.startswith("r") and k.endswith(",7") else k: relabel(v) for k, v in n["moves"].items()}; return n
relabel(main(m)); dump(2, m)
# m3: drop the scheduler reply 'finish the decoy first' (r1,9) at t=1
m = copy.deepcopy(J); del main(m)["moves"]["r1,9"]; dump(3, m)
# m4: add a job that is fine for a FREE hindsight machine but not same-cost: (3,1,5) twice at t=3 instead of once
m = copy.deepcopy(J); main(m)["moves"]["e"]["moves"]["r3,7"]["rel"] = [[1, 4]]; dump(4, m)
# m5: illegal job (d > T)
m = copy.deepcopy(J); m["tree"]["rel"] = [[2, 10]]; dump(5, m)
