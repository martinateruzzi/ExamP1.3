#!/bin/bash

#Execute
for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072; do ./main.x $i > 'times'$i ; done

#Create files
for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072; do head -n 11 'times'$i > 'tree'$i ; done
for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072; do head -n 24 'times'$i |tail -n 11 > 'map'$i ; done
for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072; do tail -n 11 'times'$i > 'balanced'$i ; done

rm 'times_tree_nobalance';
for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072
do
	awk -v I=$i '{if(NR>2){sum+=$2; sum2+=$2*$2}}END{print I, sum/9, sqrt(sum2/9-(sum/9)*(sum/9))}' 'tree'$i >> 'times_tree_nobalance'
done
for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072; do rm 'tree'$i ; done

rm 'times_tree_balance'
for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072
do
	awk -v I=$i '{if(NR>2){sum+=$2; sum2+=$2*$2}}END{print I, sum/9, sqrt(sum2/9-(sum/9)*(sum/9))}' 'balanced'$i >> 'times_tree_balance'
done
for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072; do rm 'balanced'$i ; done

rm 'times_map'
for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072
do
	awk -v I=$i '{if(NR>2){sum+=$2; sum2+=$2*$2}}END{print I, sum/9, sqrt(sum2/9-(sum/9)*(sum/9))}' 'map'$i >> 'times_map'
done
for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072; do rm 'map'$i ; done

for i in 16 32 64 128 256 512 1024 2048 4096 8192 16394 32768 65536 131072; do rm 'times'$i ; done

#Plot
gnuplot 'plot.gnu'
