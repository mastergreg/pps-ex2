#!/bin/bash
gnuplot << EOF
    set terminal png size 1024, 768
    set output "sandman_tiled_cilk_for.png"
    set title "sandman times"
    set xlabel "cores"
    set boxwidth 0.5
    set grid
    set style data histograms
    set style histogram cluster gap 0
    set style fill solid border -1
    set yrange[0:30]
    set multiplot layout 1,2
    set ylabel "Time"
    set title "Time"
    plot "sandman_tiled_cilk_for.res" lt 3 title "Times" with boxes
    set ylabel "Speedup"
    set title "Speedup"
    set yrange[0:25]
    plot "sandman_tiled_cilk_for_speedup.res" lt 4 title "Speedup" with lines, \
        "sandman_tiled_cilk_for_speedup.res" lt 4 notitle with points
    unset multiplot
    set key 
EOF
