#! /bin/bash

rm out-extes.csv
touch out-extes.csv
for i in {1500000..2500000..10000}; do 
  unset CPW
  while [ -z "$CPW" ]; 
  do
    echo $i de 2500000
    perf stat -e cycles ./empleats.opt2.nowrites.3 $i > stdout-extes 2>stderr-extes
    CYCLES=$(awk '/cycles/ {gsub(",",""); print $1}' stderr-extes)
    CPW=$(echo -e "scale=2\n$CYCLES/$i" | bc -l )
    echo "$i,$CPW" >> out-extes.csv
  done;
done;