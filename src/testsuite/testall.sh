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
#diffpath=echo
serialpath=../serial/main.exec
testFilesSizes=(16 32 64 128 1024 )

tiledBlockSizes=( 2 4 8 16 )
serialTiledPath=../lu/lu_tiled.exec
if [[ $1 -eq "1" ]]; 
then
    echo "Tiled Execution"
    cilkTestFiles=( ../cilk/lu_tiled.exec )
    cilkplusTestFiles=(../cilkplus/lu_tiled.exec)
else
    cilkTestFiles=(../cilk/lu_tiled.exec ../cilk/lu_rec.exec )
    cilkplusTestFiles=(../cilkplus/lu_tiled.exec ../cilkplus/lu_rec.exec )
fi

slog="serial.log"
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
if [[ $1 -eq "1" ]]; 
then
    # Tiled Serial
    for i in ${testfiles[@]}
    do
        for block_size in ${tiledBlockSizes[@]}
        do
            serialfile="tiled_${block_size}_${i%in}out"
            ${serialTiledPath} ${i} ${serialfile} ${block_size}
            grep "${i}" ${slog} | tail -n 1 | tee -a ${slog}
        done
    done
else
    # Non Tiled Serial
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
fi


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
                if [[ $1 -eq "1" ]]; 
                then
                    serialfile="tiled_${block_size}_${i%in}out"
                fi
                echo ${j}" "${block_size} >> ${errorfile}
                        line=$(${j} --nproc $NTHREADS ${i} ${outfile} ${block_size} 2>> ${errorfile})
                        echo $line
                        speedup ${slog} "${line}"
                echo -e "\n"		
                ${diffpath} ${serialfile} ${outfile}
            done
        else
		echo ${j} >> ${errorfile}
                line=$(${j} --nproc $NTHREADS ${i} ${outfile} ${block_size} 2>> ${errorfile})
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
                line=$(${j} ${i} ${outfile} ${block_size})
                echo $line
		echo -e "\n"		
                speedup ${slog} "${line}"
                ${diffpath} ${serialfile} ${outfile} 
            done 
        else
                line=$(${j} ${i} ${outfile} ${block_size})
                echo $line
		echo -e "\n"		
                speedup ${slog} "${line}"
                ${diffpath} ${serialfile} ${outfile} 
        fi
    done
done
