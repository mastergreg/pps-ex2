#!/bin/bash

function plotot {
    output=$1 
    machine=$2
    in_t=$3
    in_t_mit=$4
    in_t_plus=$5
    in_sp_mit=$6
    in_sp_plus=$7
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
    set yrange[0:*]
    set multiplot layout 1,2
    set ylabel "Time"
    set title "Time"
    plot "${in_t_mit}" using 2:xtic(1) lt 3 title "MIT-cilk", \
      "${in_t_plus}" using 2:xtic(1) lt 4 title "gcc-cilkplus"
    unset ylabel

    set ylabel "Speedup"
    set title "Speedup"
    set yrange[0:*]
    plot "${in_sp_mit}" using 2:xtic(1) lt 3 notitle with points, \
         "${in_sp_mit}" using 2:xtic(1) lt 3 title "MIT-cilk" with lines, \
        "${in_sp_plus}" using 2:xtic(1) lt 4 notitle with points, \
        "${in_sp_plus}" using 2:xtic(1) lt 4 title "gcc-cilkplus" with lines
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


plotot "sandman_tiled_cilkplus_rec_for.png" sandman mpourda sandman_tiled_mit_rec_for_4k_sweep.res sandman_tiled_cilkplus_rec_for_4k_sweep.res sandman_tiled_mit_rec_for_4k_sweep_speedup.res sandman_tiled_cilkplus_rec_for_4k_sweep_speedup.res 




#clones_tiled_serial_rec_for_pbs.res clones_tiled_mit_rec_for_pbs.res clones_tiled_cilkplus_rec_for_pbs.res clones_tiled_mit_rec_for_pbs_speedup.res clones_tiled_cilkplus_rec_for_pbs_speedup.res



