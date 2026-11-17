#!/bin/bash
# scripted attack (variant 1 = "11.11." + 2s + free alternate 2s + thin + 3s), best over 6 parameter settings
for V in 0 1 3 4 5; do echo "A=0 V=$V"; for m in 24 48 96 192 288; do best=0; for par in 0 1 2 3 4 5; do e=$(timeout 60 ./rtw att $m 0 $V 1 $par | sed -E 's/.*extent=([0-9]+).*/\1/'); [ "$e" -gt $best ] && best=$e; done; o=$(./rtw att $m 0 $V 0 | sed -E 's/.*extent=([0-9]+).*/\1/'); echo "  M=$m attack=$best ratio=$(echo "scale=4;$best/$m"|bc)  O-attack=$o  5M/3=$((5*m/3)) 7M/4=$((7*m/4))"; done; done
