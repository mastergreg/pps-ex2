#!/bin/bash

function plotot {
    output=$1
    machine=$2
    in_t=$3
    in_sp=$4
gnuplot << EOF
    set terminal png size 1024, 768
    set output "${output}"
    set title "${machine} times"
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
    plot "${in_t}" lt 3 title "Times" with boxes
    set ylabel "Speedup"
    set title "Speedup"
    set yrange[0:25]
    plot "${in_sp}" lt 4 title "Speedup" with lines, \
        "${in_sp}" lt 4 notitle with points
    unset multiplot
    set key 
EOF
}

plotot "sandman_tiled_cilk_for.png" "sandman" "sandman_tiled_cilk_for.res" "sandman_tiled_cilk_for_speedup.res"
