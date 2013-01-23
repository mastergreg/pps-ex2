#!/bin/bash
#set -e

speedlog="speedups.log"

function speedup() {
# calculate speedup needs the line in the format "Version mat_size time" and 
# the log filename
    size=$(echo $2 | awk '{print $2}')
    block_size=$(echo $2 | awk '{print $6}')
    if [[ "x${block_size}" == "x" ]];
    then
        serial=$(awk '/_'"${size}"'/{print $5}' < ${1})
    else
        serial=$(awk '/.*'" ${size} "'.*Block Size: '"${block_size}"'/ {print $5}' < ${1})
    fi
    parallel=$(echo $2 | awk '{print $3}')
    speedup=$(echo "${parallel} ${serial}" | awk '{ print $2/$1 }')
    echo "Parallel: ${parallel} Serial: ${serial} Speedup: ${speedup}"
    echo "${2} Serial: ${serial} Speedup: ${speedup}" >> ${speedlog}.new
}

genpathpath=../generator/generate.exec
if [ -x pypy ];
then
    diffcmd='pypy ../diffpy/diff.py'
else
    diffcmd='../diffpy/diff.py'
fi
#diffcmd=echo
serialpath=../serial/main.exec
#testFilesSizes=(64 128 1024 2048)
testFilesSizes=(64 128 1024 2048)

tiledBlockSizes=(2 4 8 16 32 64)
serialTiledPath=../lu/lu_tiled.exec
serialRecPath=../lu/lu_rec.exec
cilkTestFiles=(../cilk/lu_tiled.exec ../cilk/lu_rec.exec )
cilkTestFiles=()
cilkplusTestFiles=(../cilkplus/lu_tiled.exec ../tasks/lu_tiled.exec ../cilkplus/lu_rec.exec )
cilkplusTestFiles=(../tasks/lu_tiled.exec)


slog="serial.log"
tslog="serial_tiled.log"
rslog="serial_rec.log"
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
rm -f ${tslog}.new
# Tiled Serial
for i in ${testfiles[@]}
do
    inTime=$(stat -c %Y $i)
    for block_size in ${tiledBlockSizes[@]}
    do
        serialfile="serial_tiled_${block_size}_${i%in}out"
        # define if we need to execute or not
        if [[ -f ${serialfile} && $(stat -c %Y ${serialfile}) -gt ${inTime} ]];
        then
            echo "Outfile <${serialfile}> created after infile <${i}>, not executing."
            grep "${i}" ${tslog} | grep "Block Size: ${block_size}" |tail -n 1 | tee -a ${tslog}.new
            # print the old data and put it to the new log
        else
            outputline=$(${serialTiledPath} ${i} ${serialfile} ${block_size})
            # generate new data and if the execution was correct put them in the log
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
# replace old log with the new one
mv ${tslog}.new ${tslog}

# Rec Serial
rm -f ${rslog}.new
for i in ${testfiles[@]}
do
    serialfile="serial_rec_${i%in}out"
    inTime=$(stat -c %Y $i)
    # define if we need to execute or not
    if [[ -f ${serialfile} && $(stat -c %Y ${serialfile}) -gt ${inTime} ]];
    then
        echo "Outfile <${serialfile}> created after infile <${i}>, not executing."
        grep "${i}" ${rslog} | tail -n 1 | tee -a ${rslog}.new
        # print the old data and put it to the new log
    else
        outputline=$(${serialRecPath} ${i} ${serialfile})
        # generate new data and if the execution was correct put them in the log
        if [[ $? -eq 0 ]];
        then
            echo -n "Testfile: ${i} " | tee -a ${rslog}.new
            echo ${outputline} | tee -a ${rslog}.new
        else
            echo ${outputline}
        fi
    fi
done
# replace old log with the new one
mv ${rslog}.new ${rslog}


# Non Tiled Serial
rm -f ${slog}.new
for i in ${testfiles[@]}
do
    serialfile="serial_${i%in}out"
    inTime=$(stat -c %Y $i)
    # define if we need to execute or not
    if [[ -f ${serialfile} && $(stat -c %Y ${serialfile}) -gt ${inTime} ]];
    then
        echo "Outfile <${serialfile}> created after infile <${i}>, not executing."
        grep "${i}" ${slog} | tail -n 1 | tee -a ${slog}.new
        # print the old data and put it to the new log
    else
        outputline=$(${serialpath} ${i} ${serialfile})
        # generate new data and if the execution was correct put them in the log
        if [[ $? -eq 0 ]];
        then
            echo -n "Testfile: ${i} " | tee -a ${slog}.new
            echo ${outputline} | tee -a ${slog}.new
        else
            echo ${outputline}
        fi
    fi
done
# replace old log with the new one
mv ${slog}.new ${slog}


# Parallel execution using Cilk
echo "============ Cilk EXECUTION =============="
rm ${errorfile}
for j in ${cilkTestFiles[@]}
do
    echo $j
    for i in ${testfiles[@]}
    do
        #echo "Running testfile:" ${i}
        out="${j//\.\.\//}"
        out="${out%.exec}"
        outfile="${out//\//_}_${i%in}out"
        serialfile="serial_${i%in}out"
        if [[ ${j} == *tiled* ]]
        then
            for block_size in ${tiledBlockSizes[@]}
            do
                serialfile="serial_tiled_${block_size}_${i%in}out"
                echo ${j}" "${block_size} >> ${errorfile}
                line=$(${j} --nproc $NTHREADS ${i} ${outfile} ${block_size} 2>> ${errorfile})
                echo $line
                speedup ${tslog} "${line}"
                ${diffcmd} ${serialfile} ${outfile}
            done
        else
            echo ${j} >> ${errorfile}
            line=$(${j} --nproc $NTHREADS ${i} ${outfile} 2>> ${errorfile})
            echo $line
            speedup ${rslog} "${line}"
            ${diffcmd} ${serialfile} ${outfile}
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
        #echo "Running testfile:" ${i}
        out="${j//\.\.\//}"
        out="${out%.exec}"
        outfile="${out//\//_}_${i%in}out"
        serialfile="serial_${i%in}out"
        if [[ ${j} == *tiled* ]]
        then
            for block_size in ${tiledBlockSizes[@]}
            do
                serialfile="serial_tiled_${block_size}_${i%in}out"
                echo ${j}" "${block_size} >> ${errorfile}
                line=$(${j} ${i} ${outfile} ${block_size} 2>> ${errorfile})
                echo $line
                speedup ${tslog} "${line}"
                ${diffcmd} ${serialfile} ${outfile}
            done
        else
            echo ${j} >> ${errorfile}
            line=$(${j} ${i} ${outfile} 2>> ${errorfile})
            echo $line
            speedup ${rslog} "${line}"
            ${diffcmd} ${serialfile} ${outfile} 
        fi
    done
done


if [[ -f ${speedlog} ]]; then
    mv ${speedlog} ${speedlog}.$(date +%Y-%m-%d-%H-%M-%S)
fi
mv ${speedlog}.new ${speedlog}
