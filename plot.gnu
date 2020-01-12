set terminal png
set font 'Helvetica,16'
set output 'treeperformances.png'

set xlabel 'N'
set ylabel 'Average time (find lookup) [ns]'

set key bottom right

set title 'Comparison of lookup times'

logb(x, base) = log(x)/log(base)

p[*:*] 'times_tree_nobalance' w e pt 7 linetype 1 t 'Non-balanced tree', '' w l lw 2 linetype 1 not, \
'times_tree_balance' w e pt 7 linetype 2 t "Balanced tree", '' w l lw 2 linetype 2 not, \
'times_map' w e pt 7 linetype 3 t "std::map", '' w l lw 2 linetype 3 not, \
#logb(x,2)+60. t 'log2(N)' w l lc rgb "black" lw 2
