#!/bin/bash
# every compute step in sequence, one process at a time
cd .
ulimit -v 6000000
R="nice -n 15 taskset -c 9 timeout 3000"
{ $R python3 arith.py; echo "exit $?"; echo "== control: cap 23 (must FAIL)"; $R python3 arith.py 23 | tail -3; echo "exit ${PIPESTATUS[0]}";
  echo "== control: without the special cases j=2, j=4 (must FAIL)"; $R python3 arith.py --nospecial | tail -4; echo "exit ${PIPESTATUS[0]}"; } > out_arith.txt 2>&1
{ for a in "--D 5" "--c 4" "--hb 11" "--th 15"; do echo "== control $a (must end in VIOLATION, exit 1)"; $R python3 test_rule.py $a 2>&1 | grep -E "VIOLATION|PASS"; echo "exit ${PIPESTATUS[0]}"; done; } > out_rule_controls.txt 2>&1
{ $R ./solve C 3 9; echo "exit $?"; } > out_C.txt 2>&1
{ echo "== zeros, h=6: length 20 must WIN, length 19 must LOSE (control)"; $R ./solve L 6 $(printf '0 %.0s' $(seq 20)); $R ./solve L 6 $(printf '0 %.0s' $(seq 19));
  echo "== h=3: 0 1 0 1 0 must WIN; 0 0 0 0 0 must LOSE (control)"; $R ./solve L 3 0 1 0 1 0; $R ./solve L 3 0 0 0 0 0; } > out_L.txt 2>&1
$R python3 test_rule.py > out_rule_main.txt 2>&1; echo "exit $?" >> out_rule_main.txt
{ $R ./solve C 10 12; echo "exit $?"; } >> out_C.txt 2>&1
for w in 3 5 6; do $R ./solve S 6 19 $w 7 400; done > out_search_h6.txt 2>&1
echo done > run_all.done
