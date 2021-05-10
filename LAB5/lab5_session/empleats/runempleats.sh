#! /bin/bash

for i in {1500000..2500000..10000}; do 
  unset CPW
  while [ -z "$CPW" ]; 
  do
    echo $i de 2500000
    perf stat -e cycles ./empleats.3 $i > stdout 2>stderr
    CYCLES=$(awk '/cycles/ {gsub(",",""); print $1}' stderr)
    CPW=$(echo -e "scale=2\n$CYCLES/$i" | bc -l )
    echo "$i,$CPW" >> out.csv
  done;
done;
