#!/bin/bash

function plotot {
    output=$1 
    rec_cp=$2
    tiled_rcp=$3
    tiled_cfcp=$4
    rec_cp_spd=$5
    tiled_rcp_spd=$6
    tiled_cfcp_spd=$7
    obv_t=$8
    obv_s=$8

gnuplot << EOF
    set terminal png size 1024, 768
    set output "${output}"
    set title "Comparison"
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
    plot    "${rec_cp}" using 2:xtic(1) lt 3 title "rec" with lines, \
         "${tiled_rcp}" using 2:xtic(1) lt 4 title "tiled rec_for" with lines, \
        "${tiled_cfcp}" using 2:xtic(1) lt 6 title "tiled cilk_for" with lines, \
        "${obv_t}" using 2:xtic(1) lt 7 title "simple implementation" with lines
    unset ylabel

    set ylabel "Speedup"
    set title "Speedup"
    set yrange[0:*]
    plot    "${rec_cp_spd}" using 2:xtic(1) lt 3 notitle with points, \
            "${rec_cp_spd}" using 2:xtic(1) lt 3 title "rec" with lines, \
         "${tiled_rcp_spd}" using 2:xtic(1) lt 4 notitle with points, \
         "${tiled_rcp_spd}" using 2:xtic(1) lt 4 title "tiled rec_for" with lines, \
        "${tiled_cfcp_spd}" using 2:xtic(1) lt 6 notitle with points, \
        "${tiled_cfcp_spd}" using 2:xtic(1) lt 6 title "tiled cilk_for" with lines, \
        "${obv_s}" using 3:xtic(1) lt 7 notitle with points, \
        "${obv_s}" using 3:xtic(1) lt 7 title "simple implementation" with lines
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


#plotot "final_plot.png" 
#
#sandman_rec_time.res sandman_tiled_cilk_for.res sandman_tiled_cilkplus_rec_for_sweep.res
#sandman_rec_speedup.res sandman_tiled_cilk_for_speedup.res sandman_tiled_cilkplus_rec_for_sweep_speedup.res

plotot "final_plot.png" \
sandman_rec_cilkplus_time.res \
sandman_tiled_cilkplus_rec_for_sweep.res \
sandman_tiled_cilk_for.res \
sandman_rec_cilkplus_speedup.res \
sandman_tiled_cilkplus_rec_for_sweep_speedup.res \
sandman_tiled_cilk_for_speedup.res \
sandman_capt_obv.res
#clones_tiled_serial_rec_for_pbs.res clones_tiled_mit_rec_for_pbs.res clones_tiled_cilkplus_rec_for_pbs.res clones_tiled_mit_rec_for_pbs_speedup.res clones_tiled_cilkplus_rec_for_pbs_speedup.res



