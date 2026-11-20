solver-b working folder.  States: K_x as bitmasks.  All programs implement exactly the rules in the brief.

CODE
 slow.py          direct definition, no symmetry, no pruning (python3 slow.py 5  -> n=2..5).
 fast.cpp         full minimax table on canonical forms (n<=8).  ./fast n [x]  (x = no canonicalisation);
                  env ROUNDS=0 uses a coarser colouring (different canonical form), DUMP=file writes the value table,
                  PV=1 prints one optimal line with all reply values.   (binary fast2 = same source, with PV)
 check_table.cpp  independent checker of a value table: recomputes every entry from the rules, finding each child
                  by brute force over ALL n! relabelings.   ./check_table n tableN.txt
 ab.cpp           first threshold solver (n<=10): "can Delayer force >= t?" with transposition table.
 ab2.cpp          current threshold solver (n<=12).  ./ab2 n tlow thigh log2(tablesize).  env:
                  START="hex rows" start position;  ORD=0|1 Delayer move order;  BORD=0..3 Hastener reply order;
                  LEM=0 disables the static Lemmas 1 and 3 (proofs.txt);  TWIN=0 disables the twin shortcut;
                  HRULE=k restricts Hastener's replies (gives upper bounds only);  CERT=file dumps a Delayer certificate.
                  Binaries kept from earlier source states:  ab2_v3 == TWIN=0 BORD=0 ./ab2 ;  ab2_v4 == TWIN=0 ./ab2 ;
                  ab2_v5 == ab2 (current source); ab2c == early ab2 with CERT (no lemmas).
 check_cert.py    independent checker of Delayer certificates.  python3 check_cert.py n T cert_nN_geT.txt
 vmr.py, vmr2.py  V(m,r) = value of "m agents who all know exactly each other's m secrets + r untouched agents".
 pv.py            optimal lines for n<=6 from the direct solver.

WITNESS FILES
 table4.txt .. table7.txt     full value tables (all canonical positions), checked by check_table  (table7 = 1.1 MB)
 cert_n4_ge6.txt .. cert_n8_ge19.txt   Delayer strategies proving A(n) >= 6,9,12,16,19 (n=8 file is 6 MB)
 proofs.txt                   lemmas and the hand proof of A(4)=6
 failtests.log, bad*.txt      deliberately corrupted tables/certificates and the checkers' rejections

LOGS
 n8_rounds0.log  n10.log n10b.log n10c.log n10d.log n10e.log  n11.log n11b.log n11c.log  vmr2.log vmr3.log v66.log v66b.log
 n11_chain6.log (n=11 from the 6-chain start: >=25 there, i.e. >=30 overall; 26 not decided in time)
