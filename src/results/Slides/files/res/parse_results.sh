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
    set xlabel "threads"
    set boxwidth 0.5
    set grid
    set style data histogram
    set style histogram cluster gap 1
    set style fill solid border -1
    set yrange[0.0001:*]
    set multiplot layout 1,2
    set ylabel "Time"
    set title "Time"
    set logscale y
    plot "${in_t}" using 2:xtic(1) lt 3 title "Times"
    unset logscale
    unset ylabel

    set ylabel "Speedup"
    set title "Speedup"
    set yrange[0:*]
    plot "${in_sp}" using 2:xtic(1) lt 4 notitle with points, \
         "${in_sp}" using 2:xtic(1) lt 4 title "Speedup" with lines
    unset multiplot
    set key 
EOF
}

#plotot "sandman" "sandman_tiled_cilk_for.res" "sandman_tiled_cilk_for_speedup.res"
#plotot "sandman" "sandman_tiled_cilkplus_rec_for_sweep.res" "sandman_tiled_cilkplus_rec_for_sweep_speedup.res"


#plotot "clones_block_size_sweep_4k.png" clones clones_tiled_serial_rec_for_pbs.res clones_tiled_mit_rec_for_pbs.res clones_tiled_cilkplus_rec_for_pbs.res clones_tiled_mit_rec_for_pbs_speedup.res clones_tiled_cilkplus_rec_for_pbs_speedup.res 
#plotot "sandman_block_size_sweep_4k.png" sandman sandman_tiled_serial_4k.res sandman_tiled_mit_rec_for_4k.res sandman_tiled_cilkplus_rec_for_4k.res sandman_tiled_mit_rec_for_speedup_4k.res sandman_tiled_cilkplus_rec_for_speedup_4k.res 
#plotot "sandman_block_size_sweep_8k.png" sandman sandman_tiled_serial_8k.res sandman_tiled_mit_rec_for_8k.res sandman_tiled_cilkplus_rec_for_8k.res sandman_tiled_mit_rec_for_speedup_8k.res sandman_tiled_cilkplus_rec_for_speedup_8k.res
#plotot "sandman_tiled_cilkplus_rec_for.png" sandman sandman_tiled_serial_8k.res sandman_tiled_mit_rec_for_8k.res sandman_tiled_cilkplus_rec_for_8k.res sandman_tiled_mit_rec_for_speedup_8k.res sandman_tiled_cilkplus_rec_for_speedup_8k.res


plotot "clones_graph_tiled_times.png" clones clones_graph_tiled_times.res clones_graph_tiled_speedups.res




#clones_tiled_serial_rec_for_pbs.res clones_tiled_mit_rec_for_pbs.res clones_tiled_cilkplus_rec_for_pbs.res clones_tiled_mit_rec_for_pbs_speedup.res clones_tiled_cilkplus_rec_for_pbs_speedup.res



