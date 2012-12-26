#!/bin/bash
set -e


genpathpath=../generator/generate.exec
diffpath=../diffpy/diff.py
#diffpath=echo
serialpath=../serial/main.exec
testFilesSizes=(16 128 1024 2048)
cilkTestFiles=(../cilk/lu_rec.exec ../cilk/lu_tiled.exec)
#cilkplusTestFiles=(../cilkplus/lu_rec.exec ../cilkplus/lu_tiled.exec)
cilkplusTestFiles=()
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
    echo "Running testfile :" ${i}
    out="${j//\.\.\//}"
    serialfile="serial_${i%in}out"
    inTime=$(stat -c %Y $i)
    if [[ -f ${serialfile} && $(stat -c %Y ${serialfile}) -gt ${inTime} ]];
    then
        echo "Outfile <${serialfile}> created after infile <${i}>, not executing."
    else
        ${serialpath} ${i} ${serialfile}
    fi
done


# Parallel execution using Cilk
echo "============ Cilk EXECUTION =============="
for j in ${cilkTestFiles}
do
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
done

# Parallel execution using CilkPlus
echo "============ CilkPlus EXECUTION =============="
for j in ${cilkplusTestFiles}
do
    echo $j
    for i in ${testfiles[@]}
    do
        echo "Running testfile:" ${i}
        out="${j//\.\.\//}"
        out="${out%.exec}"
        outfile="${out//\//_}${i%in}out"
        serialfile="serial_${i%in}out"
        ${j} --nproc ${NTHREADS} ${i} ${outfile}
        ${diffpath} ${serialfile} ${outfile}
    done
done
