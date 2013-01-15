#!/bin/bash
#set -e

function speedup() {
# calculate speedup needs the line in the format "Version mat_size time" and 
# the log filename
    size=$(echo $2 | awk '{print $2}')
    block_size=$(echo $2 | awk '{print $6}')
    if [[ "x${block_size}" = "x" ]];
    then
        serial=$(grep "_${size}" ${1} | tail -n 1 | awk '{print $5}')
    else
        serial=$(grep " ${size} " ${1} | grep "Block Size: ${block_size}" | tail -n 1 | awk '{print $5}')
    fi
    parallel=$(echo $2 | awk '{print $3}')
    echo -n "${parallel} ${serial}"
    echo -n " Speedup: "
    echo "${parallel} ${serial}" | awk '{ print $2/$1 }'
}

genpathpath=../generator/generate.exec
diffpath=../diffpy/diff.py
#diffpath=echo
serialpath=../serial/main.exec
testFilesSizes=(64 128 1024)

tiledBlockSizes=( 2 4 8 16 32 64)
serialTiledPath=../lu/lu_tiled.exec
cilkTestFiles=(../cilk/lu_tiled.exec ../cilk/lu_rec.exec )
cilkplusTestFiles=(../cilkplus/lu_tiled.exec ../cilkplus/lu_rec.exec )

slog="serial.log"
tslog="tiled.log"
errorfile="cilk.err"
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
# Tiled Serial
for i in ${testfiles[@]}
do
    inTime=$(stat -c %Y $i)
    for block_size in ${tiledBlockSizes[@]}
    do
        serialfile="tiled_${block_size}_${i%in}out"
        if [[ -f ${serialfile} && $(stat -c %Y ${serialfile}) -gt ${inTime} ]];
        then
            echo "Outfile <${serialfile}> created after infile <${i}>, not executing."
            grep "${i}" ${tslog} | grep "Block Size: ${block_size}" |tail -n 1 | tee -a ${tslog}.new
        else
            outputline=$(${serialTiledPath} ${i} ${serialfile} ${block_size})
            if [[ $? -eq 0 ]];
            then
                echo -n "Testfile: ${i} " | tee -a ${tslog}.new
                echo ${outputline} | tee -a ${tslog}.new
            else
                echo ${outputline}
            fi
        fi
    done
done

mv ${tslog}.new ${tslog}

# Non Tiled Serial
for i in ${testfiles[@]}
do
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
            echo -n "Testfile: ${i} " | tee -a ${slog}.new
            echo ${outputline} | tee -a ${slog}.new
        else
            echo ${outputline}
        fi
    fi
done
mv ${slog}.new ${slog}


# Parallel execution using Cilk
echo "============ Cilk EXECUTION =============="
rm ${errorfile}
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
                serialfile="tiled_${block_size}_${i%in}out"
                echo ${j}" "${block_size} >> ${errorfile}
                line=$(${j} --nproc $NTHREADS ${i} ${outfile} ${block_size} 2>> ${errorfile})
                echo $line
                speedup ${tslog} "${line}"
                echo -e "\n"		
                ${diffpath} ${serialfile} ${outfile}
            done
        else
            echo ${j} >> ${errorfile}
            line=$(${j} --nproc $NTHREADS ${i} ${outfile} 2>> ${errorfile})
            echo $line
            speedup ${slog} "${line}"
            echo -e "\n"		
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
                serialfile="tiled_${block_size}_${i%in}out"
                echo ${j}" "${block_size} >> ${errorfile}
                line=$(${j} ${i} ${outfile} ${block_size} 2>> ${errorfile})
                echo $line
                speedup ${tslog} "${line}"
                echo -e "\n"		
                ${diffpath} ${serialfile} ${outfile}
            done
        else
            line=$(${j} ${i} ${outfile})
            echo $line
            echo -e "\n"		
            speedup ${slog} "${line}"
            ${diffpath} ${serialfile} ${outfile} 
        fi
    done
done
