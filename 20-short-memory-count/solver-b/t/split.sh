#!/bin/bash
# split.sh W N S DEPTH TAG K R : exhaustive 'find S all' split over all surviving prefixes of length DEPTH
# (DFS numbering); this instance handles prefix lines with (line number mod K) == R.
# resumable: finished prefixes are logged in split_TAG.log.R ; witnesses appended to split_TAG.wit.R
W=$1; N=$2; S=$3; D=$4; TAG=$5; K=$6; R=$7
[ -f split_$TAG.pre ] || { echo "make split_$TAG.pre first"; exit 1; }
touch split_$TAG.log.$R
i=0
while read -r p; do
  i=$((i+1)); [ $((i % K)) = $R ] || continue
  grep -qF "PREFIX=$p:" split_$TAG.log.$R && continue
  ORDER=1 PREFIX="$p" ./fast3 $W $N find $S all 5 >> split_$TAG.wit.$R 2> split_$TAG.tmp.$R && cat split_$TAG.tmp.$R >> split_$TAG.log.$R
done < split_$TAG.pre
echo ALLDONE >> split_$TAG.log.$R
