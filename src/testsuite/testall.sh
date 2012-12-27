#!/bin/bash
set -e


genpathpath=../generator/generate.exec
diffpath=../diffpy/diff.py
#diffpath=echo
serialpath=../serial/main.exec
testFilesSizes=(16 128 1024 2048)
#cilkRec="../cilk/lu_rec.exec"
#cilkTiled="../cilk/lu_tiled"
#cilkplusRec="../cilkplus/lu_rec.exec"
cilkplusTiled="../cilkplus/lu_tiled.exec"
slog="serial.log"

block_size=16
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
if [[ ! -z ${cilkRec} ]];
then
    j=${cilkRec}
    echo $j
    for i in ${testfiles[@]}
    do
        echo "Running testfile:" ${i}
        out="${j//\.\.\//}"
        out="${out%.exec}"
        outfile="${out//\//_}_${i%in}out"
        serialfile="serial_${i%in}out"
        ${j} --nproc ${NTHREADS} ${i} ${outfile}
        ${diffpath} ${serialfile} ${outfile}
    done
fi

if [[ ! -z ${cilkTiled} ]];
then
    j=${cilkTiled}
    echo $j
    for i in ${testfiles[@]}
    do
        echo "Running testfile:" ${i}
        out="${j//\.\.\//}"
        out="${out%.exec}"
        outfile="${out//\//_}_${i%in}out"
        serialfile="serial_${i%in}out"
        ${j} --nproc ${NTHREADS} ${i} ${outfile} ${block_size}
        ${diffpath} ${serialfile} ${outfile}
    done
fi

# Parallel execution using CilkPlus
echo "============ CilkPlus EXECUTION =============="
if [[ ! -z ${cilkplusRec} ]];
then
    j=${cilkplusRec}
    echo $j
    for i in ${testfiles[@]}
    do
        echo "Running testfile:" ${i}
        out="${j//\.\.\//}"
        out="${out%.exec}"
        outfile="${out//\//_}_${i%in}out"
        serialfile="serial_${i%in}out"
        ${j} ${i} ${outfile}
        ${diffpath} ${serialfile} ${outfile}
    done
fi

if [[ ! -z ${cilkplusTiled} ]];
then
    j=${cilkplusTiled}
    echo $j
    for i in ${testfiles[@]}
    do
        echo "Running testfile:" ${i}
        out="${j//\.\.\//}"
        out="${out%.exec}"
        outfile="${out//\//_}_${i%in}out"
        serialfile="serial_${i%in}out"
        echo "${j} ${i} ${outfile} ${block_size}"
        ${j} ${i} ${outfile} ${block_size}
        ${diffpath} ${serialfile} ${outfile}
    done
fi
