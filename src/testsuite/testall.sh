#!/bin/bash
#set -e

function speedup() {
# calculate speedup needs the line in the format "Vesrion mat_size time" and 
# the log filename
    size=$(echo $2 | awk '{print $2}')
    serial=$(grep "${size}" ${1} | tail -n 1 | awk '{print $6}')
    parallel=$(echo $2 | awk '{print $3}')
    echo -n "Speedup: "
    echo "${parallel} ${serial}" | awk '{ print $2/$1 }'
}

genpathpath=../generator/generate.exec
diffpath=../diffpy/diff.py
diffpath=echo
serialpath=../serial/main.exec
testFilesSizes=(16 32 64 128 1024 2048)
cilkTestFiles=(../cilk/lu_rec.exec ../cilk/lu_tiled.exec )
#cilkTestFiles=(../cilk/lu_tiled.exec )
cilkplusTestFiles=(../cilkplus/lu_rec.exec ../cilkplus/lu_tiled.exec)
#cilkplusTestFiles=(../cilkplus/lu_tiled.exec)
tiledBlockSizes=( 2 4 8 16 )
slog="serial.log"
NTHREADS=4
for i in ${testFilesSizes[@]}
do
    testfiles[${i}]="mat_${i}.in"
    if [[ 'x'$1 != 'xgen' && -f "mat_${i}.in" ]];
    then
        echo "Not generating mat_${i}.in, file exists."
    else
        echo "Generating mat_${i}.in"
        ${genpathpath} ${i} "mat_${i}.in"
    fi
done

# Serial execution
echo "============ SERIAL EXECUTION =============="
for i in ${testfiles[@]}
do
    out="${j//\.\.\//}"
    serialfile="serial_${i%in}out"
    inTime=$(stat -c %Y $i)
    if [[ -f ${serialfile} && $(stat -c %Y ${serialfile}) -gt ${inTime} ]];
    then
        echo "Outfile <${serialfile}> created after infile <${i}>, not executing."
        grep "${i}" ${slog} | tail -n 1 | tee -a ${slog}.new
    else
        outputline=$(${serialpath} ${i} ${serialfile})
        if [[ $? -eq 0 ]];
        then
            echo -n "Testfile : ${i} " | tee -a ${slog}.new
            echo ${outputline} | tee -a ${slog}.new
        else
            echo ${outputline}
        fi
    fi
done
mv ${slog}.new ${slog}


# Parallel execution using Cilk
echo "============ Cilk EXECUTION =============="
for j in ${cilkTestFiles[@]}
do
    echo $j
    for i in ${testfiles[@]}
    do
        echo "Running testfile:" ${i}
        out="${j//\.\.\//}"
        out="${out%.exec}"
        outfile="${out//\//_}_${i%in}out"
        serialfile="serial_${i%in}out"
        if [[ ${j} == *tiled* ]]
        then
            for block_size in ${tiledBlockSizes[@]}
            do
                line=$(${j} --nproc $NTHREADS ${i} ${outfile} ${block_size})
                echo $line
                speedup ${slog} "${line}"
                ${diffpath} ${serialfile} ${outfile}
            done
        else
            line=$(${j} --nproc $NTHREADS ${i} ${outfile})
                echo $line
                speedup ${slog} "${line}"
            ${diffpath} ${serialfile} ${outfile}
        fi
    done
done

# Parallel execution using CilkPlus
echo "============ CilkPlus EXECUTION =============="
for j in ${cilkplusTestFiles[@]}
do
    echo $j
    for i in ${testfiles[@]}
    do
        echo "Running testfile:" ${i}
        out="${j//\.\.\//}"
        out="${out%.exec}"
        outfile="${out//\//_}${i%in}out"
        serialfile="serial_${i%in}out"
        if [[ ${j} == *tiled* ]]
        then
            for block_size in ${tiledBlockSizes[@]}
            do
                line=$(${j} ${i} ${outfile} ${block_size})
                echo $line
                speedup ${slog} "${line}"
                ${diffpath} ${serialfile} ${outfile} 
            done 
        else
            line=$(${j} ${i} ${outfile})
                echo $line
                speedup ${slog} "${line}"
                ${diffpath} ${serialfile} ${outfile} 
        fi
    done
done
